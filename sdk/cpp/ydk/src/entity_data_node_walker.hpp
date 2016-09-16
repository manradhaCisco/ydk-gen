//
// @file entity.hpp
// @brief Header for ydk entity
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

#ifndef WALKER_HPP
#define WALKER_HPP

#include <vector>
#include <map>
#include "core.hpp"

namespace ydk {


class Entity;

core::DataNode* get_data_node_from_entity(Entity & entity, const core::RootSchemaNode & root);

void get_entity_from_data_node(core::DataNode * node, Entity* entity);




struct EntityDiagnostic : public ydk::core::DiagnosticNode<Entity*, ydk::core::ValidationError>
{

    std::vector<DiagnosticNode<std::string, ydk::core::ValidationError>> attrs;

    bool has_errors();
};




class ValidationService {

    ///
    /// @brief Options for validation.
    ///
    /// All validation is performed in the context of some operation.
    /// These options capture the context of use.
    ///
  public:
    enum class Option {
        DATASTORE,  /// Datastore validation. Note the DataNode Tree should contain everything for cross reference resolution
        GET_CONFIG, // Get config validation. Checks to see if only config nodes are references
        GET, // Get validation
        EDIT_CONFIG // Edit validation. Checks on the values of leafs etc
    };

    ValidationService();
    virtual ~ValidationService();

    virtual EntityDiagnostic validate(const ydk::core::ServiceProvider& sp, ydk::Entity& entity, ydk::ValidationService::Option option);
};


}

#endif /* WALKER_HPP */
