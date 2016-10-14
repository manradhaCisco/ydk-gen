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


#include "core_private.hpp"


///////////////////////////////////////////////////////////////////////////
/// Statement
///////////////////////////////////////////////////////////////////////////

ydk::core::Statement::Statement(): keyword{}, arg{}
{

}

ydk::core::Statement::Statement(const std::string& mkeyword, const std::string& marg) : keyword{mkeyword}, arg{marg}
{

}

ydk::core::Statement::Statement(const ydk::core::Statement& stmt): keyword{stmt.keyword}, arg{stmt.arg}
{

}

ydk::core::Statement::Statement(ydk::core::Statement&& stmt): keyword{std::move(stmt.keyword)}, arg{std::move(stmt.arg)}
{

}

ydk::core::Statement::~Statement()
{

}

ydk::core::Statement&
ydk::core::Statement::operator=(const ydk::core::Statement& stmt)
{
    keyword = stmt.keyword;
    arg = stmt.arg;
    return *this;
}

ydk::core::Statement&
ydk::core::Statement::operator=(ydk::core::Statement&& stmt)
{
    keyword = std::move(stmt.keyword);
    arg = std::move(stmt.arg);
    return *this;
}
