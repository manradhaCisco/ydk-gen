/*  ----------------------------------------------------------------
 Copyright 2016 Cisco Systems

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
------------------------------------------------------------------*/

#ifndef _NETCONF_PROVIDER_H_
#define _NETCONF_PROVIDER_H_

#include <memory>
#include <string>

#include "core.hpp"

namespace ydk {

class NetconfClient;
   
class NetconfServiceProvider : public core::ServiceProvider, public core::ModelProvider {
public:
        NetconfServiceProvider(core::Repository* repo,
                                std::string address,
                               std::string username,
                               std::string password,
                               int port);
        
        ~NetconfServiceProvider();

        virtual core::RootSchemaNode* get_root_schema() const;
        
        virtual core::DataNode* invoke(core::Rpc* rpc) const;
        
        virtual std::string get_model(const std::string& name, const std::string& version, Format format);
        
        static const char* WRITABLE_RUNNING;
        static const char* CANDIDATE;
        static const char* ROLLBACK_ON_ERROR;
        static const char* STARTUP;
        static const char* URL;
        static const char* XPATH;
        static const char* BASE_1_1;
        static const char* CONFIRMED_COMMIT_1_1;
        static const char* VALIDATE_1_1;
        static const char* NS;
        static const char* MODULE_NAME;
        

private:
        core::DataNode*
        handle_edit(core::Rpc* rpc, core::Annotation ann) const;
        
        
        core::DataNode*
        handle_read(core::Rpc* rpc) const;
        
        
        core::Repository* m_repo;
        std::unique_ptr<NetconfClient> client;
        std::unique_ptr<ydk::core::RootSchemaNode> root_schema;
        std::vector<ydk::core::Capability> capabilities;

        std::vector<std::string> client_capabilities;
        //crud related stuff
        core::SchemaNode* create_schema;
        core::SchemaNode* read_schema;
        core::SchemaNode* update_schema;
        core::SchemaNode* delete_schema;
        bool ietf_nc_monitoring_available = false;

};
}

#endif /*_NETCONF_PROVIDER_H_*/
