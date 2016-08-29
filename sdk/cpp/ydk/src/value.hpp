//
// @file value.hpp
// @brief Header for ydk value
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

#ifndef VALUE_HPP
#define VALUE_HPP

#include <sstream>
#include <string>

#include "value_types.hpp"

namespace ydk {

enum class Type {
	uint8,
	uint16,
	uint32,
	uint64,
	int8,
	int16,
	int32,
	int64,
	empty,
	identityref,
	str,
	boolean
};

class Value {
  public:
	Value(Type type, std::string name);
	~Value();

	const std::string & get() const;
	std::pair<std::string, std::string> get_name_value() const;

	void operator = (uint8 val);
//	void operator = (uint16 val);
	void operator = (uint32 val);
	void operator = (uint64 val);
	void operator = (int8 val);
//	void operator = (int16 val);
	void operator = (int32 val);
	void operator = (int64 val);
	void operator = (Empty val);
//	void operator = (Identity val);
//	void operator = (Enum val);
	void operator = (std::string val);
//	void operator = (bool val);

	operator std::string() const;
	bool operator == (Value & other) const;
	bool operator == (const Value & other) const;

	bool is_set;

  private:
	void store_value();

	std::string name;
	std::ostringstream value_buffer;
	std::string value;
	Type type;
};

std::ostream& operator<< (std::ostream& stream, const Value& value);

}

#endif /* VALUE_HPP */
