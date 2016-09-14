CrudService
===========

.. cpp:namespace:: ydk

.. cpp:class:: CrudService : public Service

CRUD Service class for supporting CRUD operations on entities.

    .. cpp:function:: CrudService()

    .. cpp:functoin:: bool create(core::ServiceProvider & provider, Entity & entity)

        Create the entity.

    .. cpp:function:: bool update(core::ServiceProvider & provider, Entity & entity)

        Update the entity.

    .. cpp:function:: bool delete\_(core::ServiceProvider & provider, Entity & entity)

        Delete the entity.

    .. cpp:function:: std::unique_ptr<Entity> read(core::ServiceProvider & provider, Entity & filter)

        Read the entity.

    .. cpp:function:: std::unique_ptr<Entity> read(core::ServiceProvider & provider, Entity & filter, bool config_only)

        Read only config.
