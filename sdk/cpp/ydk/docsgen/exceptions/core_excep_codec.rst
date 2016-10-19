YDKCodecException
=================


.. cpp:namespace:: ydk::core

.. cpp:class:: YDKCodecException : public YDKCoreException

Exception that encapsualtes the validation errors for YDK :cpp:class:`CodecService`.

    .. cpp:member: Error err

    .. cpp:function:: YDKCodecException(YDKCodecException::Error merror)

    .. cpp:enum:: Error

        .. cpp:enumerator:: SUCCESS

            No error.

        .. cpp:enumerator:: XML_MISS

            Missing XML object.

        .. cpp:enumerator:: XML_INVAL

            Invalid XML object.

        .. cpp:enumerator:: XML_INCHAR

            Invalid XML character.

        .. cpp:enumerator:: EOF_ERR

            Unexpected end of input data.
