.. _ref-schemanode:

SchemaNode
==========


.. toctree::
   :maxdepth: 2

   core_node_rootschema.rst


.. cpp:namespace:: ydk::core

.. cpp:class:: SchemaNode

Represents a Node in the SchemaTree.

    .. cpp:function:: ~SchemaNode()

        The destructor.

        .. note::

            A :cpp:class:`SchemaNode<SchemaNode>` represents a containment hierarchy. So invocation of the destructor will lead to the children of this node being destroyed.

    .. cpp:function:: virtual std::string path() const

        Get the path expression representing this Node in in the NodeTree.

        :return: ``std::string`` representing the path to this Node.

    .. cpp:function:: virtual std::vector<SchemaNode*> find(const std::string& path) const

        Finds descendant nodes that match the given xpath expression.

        This API finds descendant nodes in the :cpp:class:`SchemaNode<SchemaNode>` tree that satisfy the given path expression. See :ref:`how to path<ref-howtopath>`.

        :param path: The path expression.
        :return: Vector of :cpp:class:`SchemaNode<SchemaNode>` that satisfies the criterion.
        :raises: :cpp:class:`YDKPathException<YDKPathException>` if the path expression in invalid, see error code for details.
        :raises: :cpp:class:`YDKInvalidArgumentException<YDKInvalidArgumentException>` if the argument is invalid.

    .. cpp:function:: virtual const SchemaNode* parent() const noexcept

        Get the Parent Node of this SchemaNode in the tree.

        :return: ``SchemaNode*`` the children of this node.

    .. cpp:function:: virtual std::vector<SchemaNode*> children() const

        Returns the children of this :cpp:class:`SchemaNode <SchemaNode>` in the NodeTree.

        :return: ``std::vector<SchemaNode*>`` the pointer to the root.

    .. cpp:function:: virtual const SchemaNode* root() const noexcept

        Get the root of NodeTree this node is part of

        :return: The pointer to the root.

    .. cpp:function:: virtual Statement statement() const

        Returns the YANG statement associated with this :cpp:class:`SchemaNode<SchemaNode>`

        :return: The pointer to the yang statement for this :cpp:class:`SchemaNode<SchemaNode>`

    .. cpp:function:: virtual std::vector<Statement> keys() const

        Returns vector of YANG statement corresponding the the keys.

        :return: Vector of :cpp:class:`Statement` that represent keys.

    .. cpp:function:: virtual SchemaValueType* type() const

        Returns the pointer to the type associated with this schema node.

        .. note::

            This method will only work for :cpp:class:`SchemaNode<SchemaNode>` that represent a ``leaf`` or ``leaf-list``. Otherwise a ``nullptr`` will be returned.

        :return: Pointer to :cpp:class:`SchemaValueType<SchemaValueType>` or ``nullptr``. User should not free this pointer it is contained within the :cpp:class:`SchemaNode<SchemaNode>` so destroying the :cpp:class:`SchemaNode<SchemaNode>`.
