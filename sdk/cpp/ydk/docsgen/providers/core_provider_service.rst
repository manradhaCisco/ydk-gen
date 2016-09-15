.. _ref-serviceprovider:

ServiceProvider
===============

.. toctree::
   :maxdepth: 2

   provider_netconf.rst

.. cpp:namespace:: ydk::core

.. cpp:class:: ServiceProvider

Interface for all ServiceProvider implementations.

Concretes instances of ServiceProviders are expected to extend this interface.

    .. cpp:function:: virtual RootSchemaNode* get_root_schema()

        Returns The :cpp:class:`SchemaNode<SchemaNode>` tree supported by this instance of the :cpp:class:`ServiceProvider<ServiceProvider>`.

        :return: Pointer to the :cpp:class:`RootSchemaNode<RootSchemaNode>` or ``nullptr`` if one could not be created.

    .. cpp:function:: virtual ~ServiceProvider()

    .. cpp:function:: virtual DataNode* invoke(Rpc* rpc) const

        Invokes or executes the given rpc and returns a :cpp:class:`DataNode<DataNode>` pointer if the Rpc has an output modelled in YANG.

        :param rpc: Pointer to the :cpp:class:`Rpc<Rpc>` node.
        :return: Pointer to the :cpp:class:`DataNode<DataNode>` representing the output.
