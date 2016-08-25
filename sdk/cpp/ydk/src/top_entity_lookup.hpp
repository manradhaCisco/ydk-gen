//
// @file ydk.hpp
// @brief The main ydk public header.
//
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

#ifndef TOP_ENTITY_LOOKUP_HPP
#define TOP_ENTITY_LOOKUP_HPP

#include <string>
#include <memory>
#include <map>
#include "entity.hpp"

namespace ydk {

std::unique_ptr<Entity> lookup_path(std::string path);

void insert(std::string path, std::unique_ptr<Entity> entity);

class TopEntityLookUp {
    std::map<std::string, std::unique_ptr<Entity>> m_entity;

public:
    TopEntityLookUp ();
    ~TopEntityLookUp ();
    std::unique_ptr<Entity> lookup(std::string path);
    void insert(std::string path, std::unique_ptr<Entity> top_entity);
    static std::unique_ptr<TopEntityLookUp> get_entity_lookup();
};

}

#endif /* TOP_ENTITY_LOOKUP_HPP */
