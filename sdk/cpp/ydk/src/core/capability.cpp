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
#include <boost/log/trivial.hpp>


/////////////////////////////////////////////////////////////////////////////
/// Capability
/////////////////////////////////////////////////////////////////////////////

ydk::core::Capability::Capability(const std::string& mod, const std::string& rev): module{mod}, revision{rev}
{

}

ydk::core::Capability::Capability(const std::string& mod, const std::string& rev, const std::vector<std::string>& f,
           const std::vector<std::string>& d): module{mod}, revision{rev}, features{f}, deviations({d})
{

}

ydk::core::Capability::Capability(const Capability& cap): module{cap.module}, revision{cap.revision}, features{cap.features}, deviations{cap.deviations}
{

}

ydk::core::Capability::Capability(ydk::core::Capability&& cap): module{std::move(cap.module)}, revision{std::move(cap.revision)},
features{std::move(cap.features)}, deviations{std::move(cap.deviations)}
{

}

ydk::core::Capability&
ydk::core::Capability::operator=(const ydk::core::Capability& cap)
{
    module = cap.module;
    revision = cap.revision;
    features = cap.features;
    deviations = cap.deviations;

    return *this;
}


ydk::core::Capability&
ydk::core::Capability::operator=(ydk::core::Capability&& cap)
{
    module = std::move(cap.module);
    revision = std::move(cap.revision);
    features = std::move(cap.features);
    deviations = std::move(cap.deviations);

    return *this;
}

bool
ydk::core::Capability::operator==(const ydk::core::Capability& cap)
{
    if( cap.module != module || cap.revision != revision ) {
        return false;
    }

    if (cap.features.size() != features.size()){
        return false;
    } else if(cap.features.size() != 0){

        //sort and compare
        std::vector<std::string> cap_features{cap.features};
        std::sort(cap_features.begin(), cap_features.end());

        //sort ourselves
        std::sort(features.begin(), features.end());

        if(cap_features != features) {
            return false;
        }

    }

    if (cap.deviations.size() != deviations.size()) {
        return false;
    } else if(cap.deviations.size() != 0){

        //sort and compare
        std::vector<std::string> cap_deviations{cap.deviations};
        std::sort(cap_deviations.begin(), cap_deviations.end());

        //sort ourselves
        std::sort(deviations.begin(), deviations.end());

        if(cap_deviations != deviations) {
            return false;
        }

    }

    return true;
}
