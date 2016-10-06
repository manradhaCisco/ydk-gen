// YANG Development Kit
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

#include "entity_lookup.hpp"

namespace ydk
{

TopEntityLookUp::TopEntityLookUp ()
{
    m_entities = std::map<std::string, std::unique_ptr<Entity>>{};
}

TopEntityLookUp::TopEntityLookUp(const TopEntityLookUp & rhs)
{
    m_entities = std::map<std::string, std::unique_ptr<Entity>>{};
    copy(rhs);
}

TopEntityLookUp::~TopEntityLookUp () {}

std::unique_ptr<Entity>
TopEntityLookUp::lookup(std::string path)
{
    return std::move(m_entities.at(path));
}

void
TopEntityLookUp::insert(std::string path, std::unique_ptr<Entity> top_entity)
{
    m_entities[path] = std::move(top_entity);
}

TopEntityLookUp&
TopEntityLookUp::operator=(TopEntityLookUp rhs)
{
    swap(*this, rhs);
    return *this;
}

TopEntityLookUp&
TopEntityLookUp::operator+=(const TopEntityLookUp & rhs)
{
    copy(rhs);
    return *this;
}

TopEntityLookUp
TopEntityLookUp::operator+(const TopEntityLookUp & rhs)
{
    TopEntityLookUp result(*this);
    result += rhs;
    return result;
}

void
TopEntityLookUp::copy(const TopEntityLookUp & rhs)
{
    for (auto it = rhs.m_entities.begin(); it != rhs.m_entities.end(); it++)
    {
        m_entities[it->first] = it->second->clone_ptr();
    }
}

}
