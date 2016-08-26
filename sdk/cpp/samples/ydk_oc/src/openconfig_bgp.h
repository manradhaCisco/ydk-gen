#ifndef _OPENCONFIG_BGP_
#define _OPENCONFIG_BGP_

#include <memory>
#include <vector>
#include <string>
#include "ydk/entity.hpp"
#include "ydk/value.hpp"

//#include "ydk/models/openconfig_bgp_types.h"
//#include "ydk/models/openconfig_routing_policy.h"

namespace ydk {
namespace openconfig_bgp {

class Bgp : public Entity {
    public:
        Bgp();
        ~Bgp();

        bool has_data() const;
        EntityPath get_entity_path() const;
        Entity* set_child(std::string path);
        void set_value(std::string value_path, std::string value);

		Entity * parent;
    private:
		std::vector<Entity*> children;

    public:
    class Global : public Entity {
        public:
            Global();
            ~Global();

            bool has_data() const;
            EntityPath get_entity_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);

			Entity * parent;

        private:
			std::vector<Entity*> children;

        public:
		class AfiSafis : public Entity {
			public:
				AfiSafis();
				~AfiSafis();

		        bool has_data() const;
		        EntityPath get_entity_path() const;
		        std::vector<Entity*> & get_children();
		        Entity* set_child(std::string path);
		        void set_value(std::string value_path, std::string value);

		        Entity * parent;

	        private:
				std::vector<Entity*> children;

	        public:
			class AfiSafi : public Entity {
				public:
					AfiSafi();
					~AfiSafi();

			        bool has_data() const;
			        EntityPath get_entity_path() const;
			        Entity* set_child(std::string path);
			        void set_value(std::string value_path, std::string value);

			        Value afi_safi_name;
			        Entity * parent;

		        private:
					std::vector<Entity*> children;

				class Config : public Entity {
					public:
						Config();
						~Config();

				        bool has_data() const;
				        EntityPath get_entity_path() const;
				        Entity* set_child(std::string path);
				        void set_value(std::string value_path, std::string value);

				        Value afi_safi_name;
				        Value enabled;
				        Entity * parent;

			        private:
						std::vector<Entity*> children;
				}; // Config

				public:
					std::unique_ptr<openconfig_bgp::Bgp::Global::AfiSafis::AfiSafi::Config> config;
			}; // AfiSafi

			public:
				std::vector< std::unique_ptr<openconfig_bgp::Bgp::Global::AfiSafis::AfiSafi> > afi_safi;
		}; // AfiSafis

		public:
        class Config : public Entity {
            public:
                Config();
                ~Config();

                bool has_data() const;
                EntityPath get_entity_path() const;
		        Entity* set_child(std::string path);
		        void set_value(std::string value_path, std::string value);

    			Value as_;
    			Value router_id;
    			Entity * parent;

            private:
    			std::vector<Entity*> children;
        }; // Config

        public:
        	std::unique_ptr<openconfig_bgp::Bgp::Global::AfiSafis> afi_safis;
        	std::unique_ptr<openconfig_bgp::Bgp::Global::Config> config;
    }; // Global

    public:
    class Neighbors : public Entity {
    	public:
			Neighbors();
			~Neighbors();

			bool has_data() const;
			EntityPath get_entity_path() const;
			std::vector<Entity*> & get_children();
	        Entity* set_child(std::string path);
	        void set_value(std::string value_path, std::string value);

			Entity * parent;
    	private:
			std::vector<Entity*> children;

    	public:
			class Neighbor : public Entity {
				public:
					Neighbor();
					~Neighbor();

					bool has_data() const;
					EntityPath get_entity_path() const;
			        Entity* set_child(std::string path);
			        void set_value(std::string value_path, std::string value);

					Value neighbor_address;
					Entity * parent;
		    	private:
					std::vector<Entity*> children;

		    	public:
					class Config : public Entity {
						public:
							Config();
							~Config();

							bool has_data() const;
							EntityPath get_entity_path() const;
					        Entity* set_child(std::string path);
					        void set_value(std::string value_path, std::string value);

							Value neighbor_address;
							Value enabled;
							Value local_as;
							Value peer_as;
							Entity * parent;

						private:
							std::vector<Entity*> children;
					}; // Config

					public:
						std::unique_ptr<openconfig_bgp::Bgp::Neighbors::Neighbor::Config> config;
			}; // Neighbor
    	public:
			std::vector< std::unique_ptr<openconfig_bgp::Bgp::Neighbors::Neighbor> > neighbor;
    }; // Neighbors

    public:
		std::unique_ptr<openconfig_bgp::Bgp::Global> global_;
		std::unique_ptr<openconfig_bgp::Bgp::Neighbors> neighbors;
}; // Bgp

}
}

#endif /* _OPENCONFIG_BGP_ */

