.. _ref-codecservice:

CodecService
============


.. cpp:namespace:: ydk::core

.. cpp:class:: CodecService

The Encode/Decode Service.

    .. cpp:function:: virtual ~CodecService()

    .. cpp:function:: virtual std::string encode(const DataNode* dn, Format format, bool pretty)

        Encode the given DataNode Tree.

        :param dn: The DataNode to encode.
        :param format: Format to encode to.
        :param pretty: The output is indented for human consumption if pretty is ``true``.
        :return: The encoded string.
        :raises: :cpp:class:`YDKInvalidArgumentException<YDKInvalidArgumentException>` if the arguments are invalid.

    .. cpp:function:: virtual DataNode* decode(const RootSchemaNode* root_schema, const std::string& buffer, Format format)

        Decode the buffer to return a DataNode.

        :param root_schema: The root schema to use.
        :param buffer: The string representation of the :cpp:class:`DataNode<DataNode>`.
        :param format: Decode format.

            .. note:: TREE is not supported.
        :return: The :cpp:class:`DataNode<DataNode>` instantiated or ``nullptr`` in case of error.
        :raises: :cpp:class:`YDKInvalidArgumentException<YDKInvalidArgumentException>` if the arguments are invalid.

    .. cpp:enum:: Format

        These options can be used for encoding the given tree.

        .. cpp:enumerator:: XML

            XML

        .. cpp:enumerator:: JSON

            JSON
