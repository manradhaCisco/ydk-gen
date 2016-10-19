/// YANG Development Kit
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
#ifndef CODEC_SERVICE_HPP
#define CODEC_SERVICE_HPP

#include "core.hpp"
#include "types.hpp"
#include "codec_provider.hpp"

namespace ydk
{
    class CodecService
    {
    public:

        core::CodecService m_core_codec_service;
        core::CodecService::Format m_core_format;

        CodecService();
        ~CodecService();

        std::string encode(CodecServiceProvider & provider, Entity & entity);
        std::map<std::string, std::string> encode(CodecServiceProvider & provider, std::map<std::string, std::unique_ptr<Entity>> & entity);

        std::unique_ptr<Entity> decode(CodecServiceProvider & provider, std::string & payload);
        std::map<std::string, std::unique_ptr<Entity>> decode(CodecServiceProvider & provider, std::map<std::string, std::string> & payload_map);
    private:
        void convert_format(CodecServiceProvider & provider);
    };
}

#endif /* CODEC_SERVICE_HPP */
