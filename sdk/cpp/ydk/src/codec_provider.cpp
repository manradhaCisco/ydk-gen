// Copyright 2016 Cisco Systems. All rights reserved
//
////////////////////////////////////////////////////////////////
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
//////////////////////////////////////////////////////////////////

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/xpressive/xpressive.hpp>
#include "codec_provider.hpp"

namespace re = boost::xpressive;

namespace ydk
{

re::sregex XML_LOOKUP_KEY = re::sregex::compile("<(?P<entity>[^ ]+) xmlns=\"(?P<ns>[^\"]+)\"");
re::sregex JSON_LOOKUP_KEY = re::sregex::compile("{[^\"]+\"(?P<key>[^\"]+)\"");
std::string ERROR_MSG{"Failed to find namespace from %1% payload,"
                      " please make sure payload format is consistent with encoding format."};


std::vector<core::Capability>
create_capability(std::vector<std::string> models)
{
    std::vector<core::Capability> caps;
    for (std::string m: models)
    {
        caps.emplace_back(core::Capability{m, "", {}, {}});
    }
    return caps;
}

std::string get_xml_lookup_key(std::string & payload)
{
    std::string lookup_key;
    re::smatch what;
    if (re::regex_search(payload, what, XML_LOOKUP_KEY))
    {
        lookup_key += what["ns"];
        lookup_key += ":";
        lookup_key += what["entity"];
    }
    else
    {
        std::string error_msg{boost::str(boost::format(ERROR_MSG) % "XML")};
        BOOST_LOG_TRIVIAL(debug) << error_msg;
        throw YDKServiceProviderException(error_msg);
    }
    return lookup_key;
}

std::string get_json_lookup_key(std::string & payload)
{
    std::string lookup_key;
    re::smatch what;
    if (re::regex_search(payload, what, JSON_LOOKUP_KEY))
    {
        lookup_key += "/";
        lookup_key += what["key"];
    }
    else
    {
        std::string error_msg{boost::str(boost::format(ERROR_MSG) % "JSON")};
        BOOST_LOG_TRIVIAL(debug) << error_msg;
        throw YDKServiceProviderException(error_msg);
    }
    return lookup_key;
}

CodecServiceProvider::CodecServiceProvider(const char * path, std::vector<std::string> & models,
    Encoding encoding, bool pretty, TopEntityLookUp & lookup)
    : m_pretty{pretty}, m_encoding{encoding}, m_lookup{lookup}
{
    m_repo = std::make_unique<core::Repository>(path);
    std::vector<core::Capability> caps = create_capability(models);
    m_root_schema = std::unique_ptr<ydk::core::RootSchemaNode>(m_repo->create_root_schema(caps));
}

CodecServiceProvider::~CodecServiceProvider() {}

void
CodecServiceProvider::add_lookup_table(TopEntityLookUp & lookup)
{
    m_lookup += lookup;
}

core::RootSchemaNode*
CodecServiceProvider::get_root_schema()
{
    return m_root_schema.get();
}

std::unique_ptr<Entity>
CodecServiceProvider::get_top_entity(std::string & payload)
{
    if (m_encoding == Encoding::XML)
    {
        return m_lookup.lookup(get_xml_lookup_key(payload));
    }
    else
    {
        return m_lookup.lookup(get_json_lookup_key(payload));
    }
}

}
