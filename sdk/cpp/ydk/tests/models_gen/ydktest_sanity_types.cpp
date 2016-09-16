
#include <sstream>
#include <iostream>
#include "ydktest_sanity_types.hpp"

namespace ydk {
namespace ydktest_sanity_types {

YdktestTypeIdentity::YdktestTypeIdentity()
     : Identity("ydktest-sanity-types:ydktest-type")
{
}

YdktestTypeIdentity::~YdktestTypeIdentity()
{
}

OtherIdentity::OtherIdentity()
     : Identity("ydktest-sanity-types:other")
{
}

OtherIdentity::~OtherIdentity()
{
}

AnotherOneIdentity::AnotherOneIdentity()
     : Identity("ydktest-sanity-types:another-one")
{
}

AnotherOneIdentity::~AnotherOneIdentity()
{
}



}
}

