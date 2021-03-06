**2016-06-17 version 0.4.2:**
 * Error handling improvements
  * Fixed local validation to correctly check for types and values ([#116](https://github.com/CiscoDevNet/ydk-gen/issues/116))
  * Introduced error hierarchy to represent errors from various components, viz.: YPYModelErrors, YPYServiceError, YPYServiceProviderError ([#133](https://github.com/CiscoDevNet/ydk-gen/issues/133))
    * When raising YPYModelErrors, include errors dictionary with key as path to data, and value as tuple of error code and error message
  * Added more extensive negative test cases to ydk-gen to test handling of error ([#134](https://github.com/CiscoDevNet/ydk-gen/issues/134))
 * CRUD service / Codec service / Netconf service provider improvements
  * Added support for multiple objects to codec service ([#122](https://github.com/CiscoDevNet/ydk-gen/issues/122))
  * Added logging for codec service ([#97](https://github.com/CiscoDevNet/ydk-gen/issues/97))
  * Have logging hierarchy automatically follow package hierarchy ([#100](https://github.com/CiscoDevNet/ydk-gen/issues/100))
  * Have netconf service return YDK python objects instead of XML strings ([#120](https://github.com/CiscoDevNet/ydk-gen/issues/120))
  * Fixed decoding issue with leaf-list of enums ([#150](https://github.com/CiscoDevNet/ydk-gen/issues/150))
 * Removed requirements.txt from ydk-py and added all requirements to setup.py
 * Enforce PEP8 naming for Identity classes ([#152](https://github.com/CiscoDevNet/ydk-gen/issues/152))
 * Added full ydk-py version to the documentation ([#144](https://github.com/CiscoDevNet/ydk-gen/issues/144))

**2016-05-20 version 0.4.1:**
 * Added openconfig bgp-policy APIs to ydk-py ([#102](https://github.com/CiscoDevNet/ydk-gen/issues/102))
 * Introduced ability to programmatically retrieve SDK version of ydk-py ([#8](https://github.com/CiscoDevNet/ydk-gen/issues/8))
 * Removed unused dependencies from ydk-py's requirements.txt ([#48](https://github.com/CiscoDevNet/ydk-gen/issues/48))
 * Introduced [coveralls](https://coveralls.io) and improved [travis CI](https://travis-ci.org) integration for ydk-gen github ([#84](https://github.com/CiscoDevNet/ydk-gen/issues/84), [#54](https://github.com/CiscoDevNet/ydk-gen/issues/54), [#15](https://github.com/CiscoDevNet/ydk-gen/issues/15), [#46](https://github.com/CiscoDevNet/ydk-gen/issues/46))
 * CRUD service / Netconf service provider improvements
  * Added timeout parameter to NetconfServiceProvider ([#1](https://github.com/CiscoDevNet/ydk-gen/issues/1))
  * Fixed issues with decoding leafs of union type and nodes defined in sub-modules  ([#5](https://github.com/CiscoDevNet/ydk-gen/issues/5), [#56](https://github.com/CiscoDevNet/ydk-gen/issues/56))
  * Fixed issue with encoding enums, identities defined in external modules ([#30](https://github.com/CiscoDevNet/ydk-gen/issues/30), [#103](https://github.com/CiscoDevNet/ydk-gen/issues/103))
  * Improved support for deleting leafs, leaf-lists and lists ([#55](https://github.com/CiscoDevNet/ydk-gen/issues/55), [#103](https://github.com/CiscoDevNet/ydk-gen/issues/103))
 * Documentation improvements
  * Added 'About ydk-py' page with information about ydk-gen used to generate ydk-py ([#6](https://github.com/CiscoDevNet/ydk-gen/issues/6))
  * Indicate in documentation YDK class attributes that are keys ([#41](https://github.com/CiscoDevNet/ydk-gen/issues/41))
  * Made top containers show up at the top of the table of contents for every module document ([#39](https://github.com/CiscoDevNet/ydk-gen/issues/39))

**2016-04-15 version 0.4.0:**

  * Introduced netconf service and codec service
    * Netconf service provides APIs to execute netconf operations
    * Codec service provides APIs to encode python objects and decode payloads
  * Support for yang deviation
  * Support for subscribing to model-driven telemetry
  * Logging made more consistent
    * CRUDService outputs type of operation
    * When logging is enabled, all NETCONF messages are logged including commit
    * Log messages at various stages (send RPC request, receive reply, commit 
      etc) instead of logging all at once at the end
  * Updated enums in YDK classes to use enum34
    * Improved enum documentation
  * Improved error reporting for ydk-py and ydk-gen

**2016-03-11 version 0.3.0:**

  * First public release.
