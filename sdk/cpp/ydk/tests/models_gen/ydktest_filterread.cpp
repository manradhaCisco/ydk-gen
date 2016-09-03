
#include <sstream>
#include <iostream>
#include "ydktest_filterread.h"

namespace ydk {
namespace ydktest_filterread {

A::B::C::C()
    : parent(nullptr)
{
}

A::B::C::~C()
{
}

bool A::B::C::has_data() const
{
    return false;
}

std::string A::B::C::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "c";

    return path_buffer.str();

}

EntityPath A::B::C::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-filterread:a/b/c";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw ydk::YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
        }

        std::reverse(parents.begin(), parents.end());

        p = nullptr;
        for (auto p1 : parents) {
            if (p) {
                path_buffer << "/";
            } else { 
                 p = p1;
            }
            path_buffer << p1->get_segment_path();
        }
    }
    std::vector<std::pair<std::string, std::string> > leaf_name_values {};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* A::B::C::set_child(std::string child_path)
{
    return nullptr;
}

void A::B::C::set_value(std::string value_path, std::string value)
{
}

A::B::D::E::E()
    : e1{YType::str, "e1"}, e2{YType::str, "e2"}
    , parent(nullptr)
{
}

A::B::D::E::~E()
{
}

bool A::B::D::E::has_data() const
{
    return e1.is_set || e2.is_set;
}

std::string A::B::D::E::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "e";

    return path_buffer.str();

}

EntityPath A::B::D::E::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-filterread:a/b/d/e";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw ydk::YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
        }

        std::reverse(parents.begin(), parents.end());

        p = nullptr;
        for (auto p1 : parents) {
            if (p) {
                path_buffer << "/";
            } else { 
                 p = p1;
            }
            path_buffer << p1->get_segment_path();
        }
    }
    std::vector<std::pair<std::string, std::string> > leaf_name_values {e1.get_name_value(), e2.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* A::B::D::E::set_child(std::string child_path)
{
    return nullptr;
}

void A::B::D::E::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-filterread:a/b/d/e/e1")
    {
        e1 = value;
    }
    if(value_path == "/ydktest-filterread:a/b/d/e/e2")
    {
        e2 = value;
    }
}

A::B::D::D()
    : d1{YType::str, "d1"}, d2{YType::str, "d2"}, d3{YType::str, "d3"}
    , e(std::make_unique<A::B::D::E>())
    , parent(nullptr)
{
    e->parent = this;
    add_child(e.get());

}

A::B::D::~D()
{
}

bool A::B::D::has_data() const
{
    return d1.is_set || d2.is_set || d3.is_set || e->has_data();
}

std::string A::B::D::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "d";

    return path_buffer.str();

}

EntityPath A::B::D::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-filterread:a/b/d";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw ydk::YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
        }

        std::reverse(parents.begin(), parents.end());

        p = nullptr;
        for (auto p1 : parents) {
            if (p) {
                path_buffer << "/";
            } else { 
                 p = p1;
            }
            path_buffer << p1->get_segment_path();
        }
    }
    std::vector<std::pair<std::string, std::string> > leaf_name_values {d1.get_name_value(), d2.get_name_value(), d3.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* A::B::D::set_child(std::string child_path)
{
    if(child_path == "/ydktest-filterread:a/b/d/e")
    {
        e = std::make_unique<A::B::D::E>();
        e->parent = this;
        add_child(e.get());
        return e.get();
    }
    return nullptr;
}

void A::B::D::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-filterread:a/b/d/d1")
    {
        d1 = value;
    }
    if(value_path == "/ydktest-filterread:a/b/d/d2")
    {
        d2 = value;
    }
    if(value_path == "/ydktest-filterread:a/b/d/d3")
    {
        d3 = value;
    }
}

A::B::B()
    : b1{YType::str, "b1"}, b2{YType::str, "b2"}, b3{YType::str, "b3"}
    , c(std::make_unique<A::B::C>()), d(std::make_unique<A::B::D>())
    , parent(nullptr)
{
    c->parent = this;
    add_child(c.get());

    d->parent = this;
    add_child(d.get());

}

A::B::~B()
{
}

bool A::B::has_data() const
{
    return b1.is_set || b2.is_set || b3.is_set || c->has_data() || d->has_data();
}

std::string A::B::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "b";

    return path_buffer.str();

}

EntityPath A::B::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-filterread:a/b";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw ydk::YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
        }

        std::reverse(parents.begin(), parents.end());

        p = nullptr;
        for (auto p1 : parents) {
            if (p) {
                path_buffer << "/";
            } else { 
                 p = p1;
            }
            path_buffer << p1->get_segment_path();
        }
    }
    std::vector<std::pair<std::string, std::string> > leaf_name_values {b1.get_name_value(), b2.get_name_value(), b3.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* A::B::set_child(std::string child_path)
{
    if(child_path == "/ydktest-filterread:a/b/c")
    {
        c = std::make_unique<A::B::C>();
        c->parent = this;
        add_child(c.get());
        return c.get();
    }
    if(child_path == "/ydktest-filterread:a/b/d")
    {
        d = std::make_unique<A::B::D>();
        d->parent = this;
        add_child(d.get());
        return d.get();
    }
    return nullptr;
}

void A::B::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-filterread:a/b/b1")
    {
        b1 = value;
    }
    if(value_path == "/ydktest-filterread:a/b/b2")
    {
        b2 = value;
    }
    if(value_path == "/ydktest-filterread:a/b/b3")
    {
        b3 = value;
    }
}

A::Lst::Lst()
    : number{YType::int32, "number"}, value_{YType::str, "value"}
    , parent(nullptr)
{
}

A::Lst::~Lst()
{
}

bool A::Lst::has_data() const
{
    return number.is_set || value_.is_set;
}

std::string A::Lst::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "lst" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath A::Lst::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-filterread:a/lst";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw ydk::YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
        }

        std::reverse(parents.begin(), parents.end());

        p = nullptr;
        for (auto p1 : parents) {
            if (p) {
                path_buffer << "/";
            } else { 
                 p = p1;
            }
            path_buffer << p1->get_segment_path();
        }
    }
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), value_.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* A::Lst::set_child(std::string child_path)
{
    return nullptr;
}

void A::Lst::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-filterread:a/lst/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-filterread:a/lst/value")
    {
        value_ = value;
    }
}

A::A()
    : a1{YType::str, "a1"}, a2{YType::str, "a2"}, a3{YType::str, "a3"}
    , b(std::make_unique<A::B>())
    , parent(nullptr)
{
    b->parent = this;
    add_child(b.get());

}

A::~A()
{
}

bool A::has_data() const
{
    for (std::size_t index=0; index<lst.size(); index++)
    {
        if(lst[index]->has_data())
            return true;
    }
    return a1.is_set || a2.is_set || a3.is_set || b->has_data();
}

std::string A::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ydktest-filterread:a";

    return path_buffer.str();

}

EntityPath A::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent != nullptr) {
        throw ydk::YDKInvalidArgumentException{"parent has to be nullptr"};
    }

    path_buffer << get_segment_path();
    std::vector<std::pair<std::string, std::string> > leaf_name_values {a1.get_name_value(), a2.get_name_value(), a3.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* A::set_child(std::string child_path)
{
    if(child_path == "/ydktest-filterread:a/b")
    {
        b = std::make_unique<A::B>();
        b->parent = this;
        add_child(b.get());
        return b.get();
    }
    if(child_path == "/ydktest-filterread:a/lst")
    {
        auto child = std::make_unique<A::Lst>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        lst.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & A::get_children()
{
    for (std::size_t index=0; index<lst.size(); index++)
    {
        children.push_back(lst[index].get());
    }
    return children;
}
void A::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-filterread:a/a1")
    {
        a1 = value;
    }
    if(value_path == "/ydktest-filterread:a/a2")
    {
        a2 = value;
    }
    if(value_path == "/ydktest-filterread:a/a3")
    {
        a3 = value;
    }
}

std::unique_ptr<Entity> A::clone_ptr()
{
    return std::make_unique<A>();
}

}
}

