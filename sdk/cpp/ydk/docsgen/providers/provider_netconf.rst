NetconfServiceProvider
======================

.. cpp:namespace:: ydk

.. cpp:class:: NetconfServiceProvider : public core::ServiceProvider

TODO.
    
    .. cpp:member:: static const char* WRITABLE_RUNNING

    .. cpp:member:: static const char* CANDIDATE

    .. cpp:member:: static const char* ROLLBACK_ON_ERROR

    .. cpp:member:: static const char* STARTUP

    .. cpp:member:: static const char* URL

    .. cpp:member:: static const char* XPATH

    .. cpp:member:: static const char* BASE_1_1

    .. cpp:member:: static const char* CONFIRMED_COMMIT_1_1

    .. cpp:member:: static const char* VALIDATE_1_1

    .. cpp:member:: static const char* NS

    .. cpp:member:: static const char* MODULE_NAME

    .. cpp:function:: NetconfServiceProvider(const core::Repository* repo,\
                         std::string address,\
                             std::string username,\
                                 std::string password, int port)

    .. cpp:function:: ~NetconfServiceProvider()

    .. cpp:function:: virtual core::RootSchemaNode* get_root_schema() const

    .. cpp:function:: virtual core::DataNode* invoke(core::Rpc* rpc) const
