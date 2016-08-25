#include "../../src/entity.hpp"
#include "../../src/make_unique.hpp"
#include "../../src/top_entity_lookup.hpp"
#include "openconfig_bgp.h"

namespace ydk {
std::unique_ptr<TopEntityLookUp>
TopEntityLookUp::get_entity_lookup()
{
    std::unique_ptr<TopEntityLookUp> entity_lookup = std::make_unique<TopEntityLookUp>();
    entity_lookup->insert(std::string{"/openconfig-bgp:bgp"}, std::unique_ptr<openconfig_bgp::Bgp>());
    return entity_lookup;
}
}
