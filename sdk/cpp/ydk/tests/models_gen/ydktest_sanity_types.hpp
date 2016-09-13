#ifndef _YDKTEST_SANITY_TYPES_
#define _YDKTEST_SANITY_TYPES_

#include <memory>
#include <vector>
#include <string>
#include "ydk/types.hpp"
#include "ydk/errors.hpp"

#include "ydktest_sanity.hpp"

namespace ydk {
namespace ydktest_sanity_types {

class YdktestTypeIdentity : public ydktest_sanity::BaseIdentityIdentity, virtual Identity
{
    public:
        YdktestTypeIdentity();
        ~YdktestTypeIdentity();


    public:


}; // YdktestTypeIdentity

class OtherIdentity : public ydktest_sanity_types::YdktestTypeIdentity, virtual Identity
{
    public:
        OtherIdentity();
        ~OtherIdentity();


    public:


}; // OtherIdentity

class AnotherOneIdentity : public ydktest_sanity_types::YdktestTypeIdentity, virtual Identity
{
    public:
        AnotherOneIdentity();
        ~AnotherOneIdentity();


    public:


}; // AnotherOneIdentity


}
}

#endif /* _YDKTEST_SANITY_TYPES_ */

