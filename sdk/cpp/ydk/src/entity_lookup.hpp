
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

#ifndef ENTITY_LOOKUP_HPP
#define ENTITY_LOOKUP_HPP

#include <map>
#include <string>
#include "types.hpp"

namespace ydk
{
    class TopEntityLookUp
    {
        public:

            std::map<std::string, std::unique_ptr<Entity>> m_entities;

            TopEntityLookUp();
            TopEntityLookUp(const TopEntityLookUp & rhs);
            ~TopEntityLookUp();

            TopEntityLookUp& operator=(TopEntityLookUp rhs);
            TopEntityLookUp& operator+=(const TopEntityLookUp & rhs);
            TopEntityLookUp operator+(const TopEntityLookUp & rhs);

            std::unique_ptr<Entity> lookup(std::string path);
            void insert(std::string path, std::unique_ptr<Entity> top_entity);

            friend void swap(TopEntityLookUp & first, TopEntityLookUp & second)
            {
                using std::swap;
                swap(first.m_entities, second.m_entities);
            }

        private:
            void copy(const TopEntityLookUp & rhs);
    };
    void augment_lookup_tables();
}

#endif /* ENTITY_LOOKUP_HPP */
