SchemaValueEmptyType
====================


.. cpp:namespace:: ydk::core

.. cpp:class:: SchemaValueEmptyType : public SchemaValueType

Empty type.

    .. cpp:member:: std::string leaf_name

    .. cpp:function:: SchemaValueEmptyType(const std::string& mleaf_name)

    .. cpp:function:: ~SchemaValueEmptyType()

    .. cpp:function:: DiagnosticNode<std::string, ValidationError>\
                         validate(const std::string& value) const
