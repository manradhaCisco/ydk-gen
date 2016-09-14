Getting Started
===============

Overview:
----------

YDK or YANG Development Kit is a Software Development Kit that provides runtime library and API's
that are modeled in YANG. The main goal of YDK is to reduce the learning curve by expressing the
model semantics in API and abstracting protocol/encoding details. The API's could be generated from
YANG models using `example profile files <https://github.com/CiscoDevNet/ydk-gen/blob/master/profiles>`_.

System Requirements:
--------------------

Linux
~~~~~

Ubuntu (Debian-based): The following packages must be present in your system before installing YDK:

.. code-block:: bash

    $ sudo apt-get install python-pip zlib1g-dev python-lxml libxml2-dev libxslt1-dev python-dev libboost-dev libboost-python-dev libssh-dev libcurl4-openssl-dev libtool-bin


Mac
~~~
It is recommended to install `homebrew <http://brew.sh>`_ and Xcode command line tools on your system before installing YDK:

.. code-block:: bash

    $ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    $ xcode-select --install
    $ brew install boost boost-python pkg-config cmake libssh

.. note::

    Please make sure `CMake 3.6 <https://cmake.org/download/>`_ is installed.


Installation:
-------------

Initialize install environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash
    
    $ source install.sh

This script will create a virtual environment and set the environment variable for ``YDKGEN_HOME``.

Install YDK Core Library
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    $ python generate.py --core --cpp  # add option --generate-doc to generate documentation

Install YDK Bundle:
~~~~~~~~~~~~~~~~~~~

For example to install YDK ietf bundle, use command:

.. code-block:: bash
    
    $ python generate.py --cpp --bundle profiles/bundles/ietf.json


Notes:
------
- YANG Development Kit is a SDK that provides an API to access/modify configuration and operational entities that are modeled using YANG
- The language bindings are derived from YANG models
- YDK provides a simple CRUD (Create/Read/Update/Delete) api that allows the developer to perform these operations on entities on a server that supports them

Example Usage
========================



Release Notes
--------------
The current release version is 0.5.0 (beta). YDK is licensed under the Apache 2.0 License.

Documentation and Support
--------------------------
- Samples can be found under the <git_root>/samples directory
- API documentation can be found at `API documentation <#>`_
- Additional samples can be found at `sample apps <#>`_
- For queries related to usage of the API, please join the `YDK community <https://communities.cisco.com/community/developer/ydk>`_.
