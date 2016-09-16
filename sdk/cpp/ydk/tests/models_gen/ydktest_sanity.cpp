
#include <sstream>
#include <iostream>
#include "ydktest_sanity.hpp"

namespace ydk {
namespace ydktest_sanity {

BaseIdentityIdentity::BaseIdentityIdentity()
     : Identity("ydktest-sanity:base-identity")
{
}

BaseIdentityIdentity::~BaseIdentityIdentity()
{
}

SubTest::OneAug::OneAug()
    : 
	name{YType::str, "name"},
	 number{YType::int32, "number"}
    , parent(nullptr)
{
}

SubTest::OneAug::~OneAug()
{
}

bool SubTest::OneAug::has_data() const
{
    return name.is_set || number.is_set;
}

std::string SubTest::OneAug::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "one-aug";

    return path_buffer.str();

}

EntityPath SubTest::OneAug::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:sub-test/one-aug";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {name.get_name_value(), number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* SubTest::OneAug::set_child(std::string child_path)
{
    return nullptr;
}

void SubTest::OneAug::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:sub-test/one-aug/name")
    {
        name = value;
    }
    if(value_path == "/ydktest-sanity:sub-test/one-aug/number")
    {
        number = value;
    }
}

SubTest::SubTest()
    : 		one_aug(std::make_unique<SubTest::OneAug>())
    , parent(nullptr)
{
    one_aug->parent = this;
    add_child(one_aug.get());

}

SubTest::~SubTest()
{
}

bool SubTest::has_data() const
{
    return one_aug->has_data();
}

std::string SubTest::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ydktest-sanity:sub-test";

    return path_buffer.str();

}

EntityPath SubTest::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent != nullptr) {
        throw YDKInvalidArgumentException{"parent has to be nullptr"};
    }

    path_buffer << get_segment_path();
    std::vector<std::pair<std::string, std::string> > leaf_name_values {};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* SubTest::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:sub-test/one-aug")
    {
        one_aug = std::make_unique<SubTest::OneAug>();
        one_aug->parent = this;
        add_child(one_aug.get());
        return one_aug.get();
    }
    return nullptr;
}

void SubTest::set_value(std::string value_path, std::string value)
{
}

std::unique_ptr<Entity> SubTest::clone_ptr()
{
    return std::make_unique<SubTest>();
}
Runner::One::OneAug::OneAug()
    : 
	name{YType::str, "name"},
	 number{YType::int32, "number"}
    , parent(nullptr)
{
}

Runner::One::OneAug::~OneAug()
{
}

bool Runner::One::OneAug::has_data() const
{
    return name.is_set || number.is_set;
}

std::string Runner::One::OneAug::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ydktest-sanity-augm:one-aug";

    return path_buffer.str();

}

EntityPath Runner::One::OneAug::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/one/ydktest-sanity-augm:one-aug";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {name.get_name_value(), number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::One::OneAug::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::One::OneAug::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/one/one-aug/name")
    {
        name = value;
    }
    if(value_path == "/ydktest-sanity:runner/one/one-aug/number")
    {
        number = value;
    }
}

Runner::One::One()
    : 
	name{YType::str, "name"},
	 number{YType::int32, "number"}
    , 		one_aug(std::make_unique<Runner::One::OneAug>())
    , parent(nullptr)
{
    one_aug->parent = this;
    add_child(one_aug.get());

}

Runner::One::~One()
{
}

bool Runner::One::has_data() const
{
    return name.is_set || number.is_set || one_aug->has_data();
}

std::string Runner::One::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "one";

    return path_buffer.str();

}

EntityPath Runner::One::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/one";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {name.get_name_value(), number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::One::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/one/one-aug")
    {
        one_aug = std::make_unique<Runner::One::OneAug>();
        one_aug->parent = this;
        add_child(one_aug.get());
        return one_aug.get();
    }
    return nullptr;
}

void Runner::One::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/one/name")
    {
        name = value;
    }
    if(value_path == "/ydktest-sanity:runner/one/number")
    {
        number = value;
    }
}

Runner::Two::Sub1::Sub1()
    : 
	number{YType::int32, "number"}
    , parent(nullptr)
{
}

Runner::Two::Sub1::~Sub1()
{
}

bool Runner::Two::Sub1::has_data() const
{
    return number.is_set;
}

std::string Runner::Two::Sub1::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "sub1";

    return path_buffer.str();

}

EntityPath Runner::Two::Sub1::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/two/sub1";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::Two::Sub1::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::Two::Sub1::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/two/sub1/number")
    {
        number = value;
    }
}

Runner::Two::Two()
    : 
	name{YType::str, "name"},
	 number{YType::int32, "number"}
    , 		sub1(std::make_unique<Runner::Two::Sub1>())
    , parent(nullptr)
{
    sub1->parent = this;
    add_child(sub1.get());

}

Runner::Two::~Two()
{
}

bool Runner::Two::has_data() const
{
    return name.is_set || number.is_set || sub1->has_data();
}

std::string Runner::Two::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "two";

    return path_buffer.str();

}

EntityPath Runner::Two::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/two";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {name.get_name_value(), number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::Two::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/two/sub1")
    {
        sub1 = std::make_unique<Runner::Two::Sub1>();
        sub1->parent = this;
        add_child(sub1.get());
        return sub1.get();
    }
    return nullptr;
}

void Runner::Two::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/two/name")
    {
        name = value;
    }
    if(value_path == "/ydktest-sanity:runner/two/number")
    {
        number = value;
    }
}

Runner::Three::Sub1::Sub2::Sub2()
    : 
	number{YType::int32, "number"}
    , parent(nullptr)
{
}

Runner::Three::Sub1::Sub2::~Sub2()
{
}

bool Runner::Three::Sub1::Sub2::has_data() const
{
    return number.is_set;
}

std::string Runner::Three::Sub1::Sub2::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "sub2";

    return path_buffer.str();

}

EntityPath Runner::Three::Sub1::Sub2::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/three/sub1/sub2";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::Three::Sub1::Sub2::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::Three::Sub1::Sub2::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/three/sub1/sub2/number")
    {
        number = value;
    }
}

Runner::Three::Sub1::Sub1()
    : 
	number{YType::int32, "number"}
    , 		sub2(std::make_unique<Runner::Three::Sub1::Sub2>())
    , parent(nullptr)
{
    sub2->parent = this;
    add_child(sub2.get());

}

Runner::Three::Sub1::~Sub1()
{
}

bool Runner::Three::Sub1::has_data() const
{
    return number.is_set || sub2->has_data();
}

std::string Runner::Three::Sub1::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "sub1";

    return path_buffer.str();

}

EntityPath Runner::Three::Sub1::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/three/sub1";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::Three::Sub1::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/three/sub1/sub2")
    {
        sub2 = std::make_unique<Runner::Three::Sub1::Sub2>();
        sub2->parent = this;
        add_child(sub2.get());
        return sub2.get();
    }
    return nullptr;
}

void Runner::Three::Sub1::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/three/sub1/number")
    {
        number = value;
    }
}

Runner::Three::Three()
    : 
	name{YType::str, "name"},
	 number{YType::int32, "number"}
    , 		sub1(std::make_unique<Runner::Three::Sub1>())
    , parent(nullptr)
{
    sub1->parent = this;
    add_child(sub1.get());

}

Runner::Three::~Three()
{
}

bool Runner::Three::has_data() const
{
    return name.is_set || number.is_set || sub1->has_data();
}

std::string Runner::Three::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "three";

    return path_buffer.str();

}

EntityPath Runner::Three::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/three";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {name.get_name_value(), number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::Three::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/three/sub1")
    {
        sub1 = std::make_unique<Runner::Three::Sub1>();
        sub1->parent = this;
        add_child(sub1.get());
        return sub1.get();
    }
    return nullptr;
}

void Runner::Three::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/three/name")
    {
        name = value;
    }
    if(value_path == "/ydktest-sanity:runner/three/number")
    {
        number = value;
    }
}

Runner::Ytypes::BuiltInT::BuiltInT()
    : 
	bincoded{YType::str, "bincoded"},
	 bits_value{YType::bits, "bits-value"},
	 bool_value{YType::boolean, "bool-value"},
	 deci64{YType::str, "deci64"},
	 embeded_enum{YType::enumeration, "embeded-enum"},
	 emptee{YType::empty, "emptee"},
	 enum_int_value{YType::str, "enum-int-value"},
	 enum_value{YType::enumeration, "enum-value"},
	 identity_ref_value{YType::identityref, "identity-ref-value"},
	 leaf_ref{YType::str, "leaf-ref"},
	 name{YType::str, "name"},
	 number16{YType::int16, "number16"},
	 number32{YType::int32, "number32"},
	 number64{YType::int64, "number64"},
	 number8{YType::int8, "number8"},
	 u_number16{YType::uint16, "u_number16"},
	 u_number32{YType::uint32, "u_number32"},
	 u_number64{YType::uint64, "u_number64"},
	 u_number8{YType::uint8, "u_number8"},
	 younion{YType::str, "younion"},
	 younion_recursive{YType::str, "younion-recursive"}
    , parent(nullptr)
{
    embeded_enum.enum_to_string_func = ydktest_sanity::Runner_Ytypes_BuiltInT_EmbededEnumEnum_to_string;
    enum_value.enum_to_string_func = ydktest_sanity::YdkEnumTestEnum_to_string;
}

Runner::Ytypes::BuiltInT::~BuiltInT()
{
}

bool Runner::Ytypes::BuiltInT::has_data() const
{
    for (std::size_t index=0; index<enum_llist.size(); index++)
    {
        if(enum_llist[index].is_set)
            return true;
    }
    for (std::size_t index=0; index<identity_llist.size(); index++)
    {
        if(identity_llist[index].is_set)
            return true;
    }
    for (std::size_t index=0; index<llstring.size(); index++)
    {
        if(llstring[index].is_set)
            return true;
    }
    for (std::size_t index=0; index<llunion.size(); index++)
    {
        if(llunion[index].is_set)
            return true;
    }
    for (std::size_t index=0; index<younion_list.size(); index++)
    {
        if(younion_list[index].is_set)
            return true;
    }
    return bincoded.is_set || bits_value.is_set || bool_value.is_set || deci64.is_set || embeded_enum.is_set || emptee.is_set || enum_int_value.is_set || enum_value.is_set || identity_ref_value.is_set || leaf_ref.is_set || name.is_set || number16.is_set || number32.is_set || number64.is_set || number8.is_set || u_number16.is_set || u_number32.is_set || u_number64.is_set || u_number8.is_set || younion.is_set || younion_recursive.is_set;
}

std::string Runner::Ytypes::BuiltInT::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "built-in-t";

    return path_buffer.str();

}

EntityPath Runner::Ytypes::BuiltInT::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/ytypes/built-in-t";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {bincoded.get_name_value(), bits_value.get_name_value(), bool_value.get_name_value(), deci64.get_name_value(), embeded_enum.get_name_value(), emptee.get_name_value(), enum_int_value.get_name_value(), enum_value.get_name_value(), identity_ref_value.get_name_value(), leaf_ref.get_name_value(), name.get_name_value(), number16.get_name_value(), number32.get_name_value(), number64.get_name_value(), number8.get_name_value(), u_number16.get_name_value(), u_number32.get_name_value(), u_number64.get_name_value(), u_number8.get_name_value(), younion.get_name_value(), younion_recursive.get_name_value()};
    for( const Value & leaf : enum_llist )
    {
        leaf_name_values.push_back(leaf.get_name_value());
    }
    for( const Value & leaf : identity_llist )
    {
        leaf_name_values.push_back(leaf.get_name_value());
    }
    for( const Value & leaf : llstring )
    {
        leaf_name_values.push_back(leaf.get_name_value());
    }
    for( const Value & leaf : llunion )
    {
        leaf_name_values.push_back(leaf.get_name_value());
    }
    for( const Value & leaf : younion_list )
    {
        leaf_name_values.push_back(leaf.get_name_value());
    }
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::Ytypes::BuiltInT::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::Ytypes::BuiltInT::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/bincoded")
    {
        bincoded = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/bits-value")
    {
        bits_value[value] = true;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/bool-value")
    {
        bool_value = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/deci64")
    {
        deci64 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/embeded-enum")
    {
        embeded_enum = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/emptee")
    {
        emptee = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/enum-int-value")
    {
        enum_int_value = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/enum-llist")
    {
        //enum_llist.push_back(value);
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/enum-value")
    {
        enum_value = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/identity-llist")
    {
        //identity_llist.push_back(value);
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/identity-ref-value")
    {
        identity_ref_value = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/leaf-ref")
    {
        leaf_ref = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/llstring")
    {
        //llstring.push_back(value);
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/llunion")
    {
        //llunion.push_back(value);
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/name")
    {
        name = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/number16")
    {
        number16 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/number32")
    {
        number32 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/number64")
    {
        number64 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/number8")
    {
        number8 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/u_number16")
    {
        u_number16 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/u_number32")
    {
        u_number32 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/u_number64")
    {
        u_number64 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/u_number8")
    {
        u_number8 = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/younion")
    {
        younion = value;
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/younion-list")
    {
        //younion_list.push_back(value);
    }
    if(value_path == "/ydktest-sanity:runner/ytypes/built-in-t/younion-recursive")
    {
        younion_recursive = value;
    }
}

Runner::Ytypes::DerivedT::DerivedT()
    : parent(nullptr)
{
}

Runner::Ytypes::DerivedT::~DerivedT()
{
}

bool Runner::Ytypes::DerivedT::has_data() const
{
    return false;
}

std::string Runner::Ytypes::DerivedT::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "derived-t";

    return path_buffer.str();

}

EntityPath Runner::Ytypes::DerivedT::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/ytypes/derived-t";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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

Entity* Runner::Ytypes::DerivedT::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::Ytypes::DerivedT::set_value(std::string value_path, std::string value)
{
}

Runner::Ytypes::Ytypes()
    : 		built_in_t(std::make_unique<Runner::Ytypes::BuiltInT>()),
 	derived_t(std::make_unique<Runner::Ytypes::DerivedT>())
    , parent(nullptr)
{
    built_in_t->parent = this;
    add_child(built_in_t.get());

    derived_t->parent = this;
    add_child(derived_t.get());

}

Runner::Ytypes::~Ytypes()
{
}

bool Runner::Ytypes::has_data() const
{
    return built_in_t->has_data() || derived_t->has_data();
}

std::string Runner::Ytypes::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ytypes";

    return path_buffer.str();

}

EntityPath Runner::Ytypes::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/ytypes";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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

Entity* Runner::Ytypes::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/ytypes/built-in-t")
    {
        built_in_t = std::make_unique<Runner::Ytypes::BuiltInT>();
        built_in_t->parent = this;
        add_child(built_in_t.get());
        return built_in_t.get();
    }
    if(child_path == "/ydktest-sanity:runner/ytypes/derived-t")
    {
        derived_t = std::make_unique<Runner::Ytypes::DerivedT>();
        derived_t->parent = this;
        add_child(derived_t.get());
        return derived_t.get();
    }
    return nullptr;
}

void Runner::Ytypes::set_value(std::string value_path, std::string value)
{
}

Runner::OneList::Ldata::Ldata()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , parent(nullptr)
{
}

Runner::OneList::Ldata::~Ldata()
{
}

bool Runner::OneList::Ldata::has_data() const
{
    return number.is_set || name.is_set;
}

std::string Runner::OneList::Ldata::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ldata" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::OneList::Ldata::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/one-list/ldata";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::OneList::Ldata::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::OneList::Ldata::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/one-list/ldata/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/one-list/ldata/name")
    {
        name = value;
    }
}

Runner::OneList::OneAugList::Ldata::Ldata()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , parent(nullptr)
{
}

Runner::OneList::OneAugList::Ldata::~Ldata()
{
}

bool Runner::OneList::OneAugList::Ldata::has_data() const
{
    return number.is_set || name.is_set;
}

std::string Runner::OneList::OneAugList::Ldata::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ldata" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::OneList::OneAugList::Ldata::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/one-list/ydktest-sanity-augm:one-aug-list/ldata";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::OneList::OneAugList::Ldata::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::OneList::OneAugList::Ldata::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/one-list/one-aug-list/ldata/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/one-list/one-aug-list/ldata/name")
    {
        name = value;
    }
}

Runner::OneList::OneAugList::OneAugList()
    : 
	enabled{YType::boolean, "enabled"}
    , parent(nullptr)
{
}

Runner::OneList::OneAugList::~OneAugList()
{
}

bool Runner::OneList::OneAugList::has_data() const
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        if(ldata[index]->has_data())
            return true;
    }
    return enabled.is_set;
}

std::string Runner::OneList::OneAugList::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ydktest-sanity-augm:one-aug-list";

    return path_buffer.str();

}

EntityPath Runner::OneList::OneAugList::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/one-list/ydktest-sanity-augm:one-aug-list";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {enabled.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::OneList::OneAugList::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/one-list/one-aug-list/ldata")
    {
        auto child = std::make_unique<Runner::OneList::OneAugList::Ldata>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        ldata.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & Runner::OneList::OneAugList::get_children()
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        children.push_back(ldata[index].get());
    }
    return children;
}
void Runner::OneList::OneAugList::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/one-list/one-aug-list/enabled")
    {
        enabled = value;
    }
}

Runner::OneList::OneList()
    : 		one_aug_list(std::make_unique<Runner::OneList::OneAugList>())
    , parent(nullptr)
{
    one_aug_list->parent = this;
    add_child(one_aug_list.get());

}

Runner::OneList::~OneList()
{
}

bool Runner::OneList::has_data() const
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        if(ldata[index]->has_data())
            return true;
    }
    return one_aug_list->has_data();
}

std::string Runner::OneList::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "one-list";

    return path_buffer.str();

}

EntityPath Runner::OneList::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/one-list";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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

Entity* Runner::OneList::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/one-list/ldata")
    {
        auto child = std::make_unique<Runner::OneList::Ldata>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        ldata.push_back(std::move(child));
        return child_ptr;
    }
    if(child_path == "/ydktest-sanity:runner/one-list/one-aug-list")
    {
        one_aug_list = std::make_unique<Runner::OneList::OneAugList>();
        one_aug_list->parent = this;
        add_child(one_aug_list.get());
        return one_aug_list.get();
    }
    return nullptr;
}

std::vector<Entity*> & Runner::OneList::get_children()
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        children.push_back(ldata[index].get());
    }
    return children;
}
void Runner::OneList::set_value(std::string value_path, std::string value)
{
}

Runner::TwoList::Ldata::Subl1::Subl1()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , parent(nullptr)
{
}

Runner::TwoList::Ldata::Subl1::~Subl1()
{
}

bool Runner::TwoList::Ldata::Subl1::has_data() const
{
    return number.is_set || name.is_set;
}

std::string Runner::TwoList::Ldata::Subl1::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "subl1" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::TwoList::Ldata::Subl1::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        throw YDKInvalidArgumentException{"parent cannot be nullptr"};
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::TwoList::Ldata::Subl1::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::TwoList::Ldata::Subl1::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/two-list/ldata/subl1/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/two-list/ldata/subl1/name")
    {
        name = value;
    }
}

Runner::TwoList::Ldata::Ldata()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , parent(nullptr)
{
}

Runner::TwoList::Ldata::~Ldata()
{
}

bool Runner::TwoList::Ldata::has_data() const
{
    for (std::size_t index=0; index<subl1.size(); index++)
    {
        if(subl1[index]->has_data())
            return true;
    }
    return number.is_set || name.is_set;
}

std::string Runner::TwoList::Ldata::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ldata" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::TwoList::Ldata::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/two-list/ldata";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::TwoList::Ldata::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/two-list/ldata/subl1")
    {
        auto child = std::make_unique<Runner::TwoList::Ldata::Subl1>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        subl1.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & Runner::TwoList::Ldata::get_children()
{
    for (std::size_t index=0; index<subl1.size(); index++)
    {
        children.push_back(subl1[index].get());
    }
    return children;
}
void Runner::TwoList::Ldata::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/two-list/ldata/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/two-list/ldata/name")
    {
        name = value;
    }
}

Runner::TwoList::TwoList()
    : parent(nullptr)
{
}

Runner::TwoList::~TwoList()
{
}

bool Runner::TwoList::has_data() const
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        if(ldata[index]->has_data())
            return true;
    }
    return false;
}

std::string Runner::TwoList::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "two-list";

    return path_buffer.str();

}

EntityPath Runner::TwoList::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/two-list";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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

Entity* Runner::TwoList::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/two-list/ldata")
    {
        auto child = std::make_unique<Runner::TwoList::Ldata>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        ldata.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & Runner::TwoList::get_children()
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        children.push_back(ldata[index].get());
    }
    return children;
}
void Runner::TwoList::set_value(std::string value_path, std::string value)
{
}

Runner::ThreeList::Ldata::Subl1::SubSubl1::SubSubl1()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , parent(nullptr)
{
}

Runner::ThreeList::Ldata::Subl1::SubSubl1::~SubSubl1()
{
}

bool Runner::ThreeList::Ldata::Subl1::SubSubl1::has_data() const
{
    return number.is_set || name.is_set;
}

std::string Runner::ThreeList::Ldata::Subl1::SubSubl1::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "sub-subl1" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::ThreeList::Ldata::Subl1::SubSubl1::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        throw YDKInvalidArgumentException{"parent cannot be nullptr"};
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::ThreeList::Ldata::Subl1::SubSubl1::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::ThreeList::Ldata::Subl1::SubSubl1::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/three-list/ldata/subl1/sub-subl1/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/three-list/ldata/subl1/sub-subl1/name")
    {
        name = value;
    }
}

Runner::ThreeList::Ldata::Subl1::Subl1()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , parent(nullptr)
{
}

Runner::ThreeList::Ldata::Subl1::~Subl1()
{
}

bool Runner::ThreeList::Ldata::Subl1::has_data() const
{
    for (std::size_t index=0; index<sub_subl1.size(); index++)
    {
        if(sub_subl1[index]->has_data())
            return true;
    }
    return number.is_set || name.is_set;
}

std::string Runner::ThreeList::Ldata::Subl1::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "subl1" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::ThreeList::Ldata::Subl1::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        throw YDKInvalidArgumentException{"parent cannot be nullptr"};
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::ThreeList::Ldata::Subl1::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/three-list/ldata/subl1/sub-subl1")
    {
        auto child = std::make_unique<Runner::ThreeList::Ldata::Subl1::SubSubl1>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        sub_subl1.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & Runner::ThreeList::Ldata::Subl1::get_children()
{
    for (std::size_t index=0; index<sub_subl1.size(); index++)
    {
        children.push_back(sub_subl1[index].get());
    }
    return children;
}
void Runner::ThreeList::Ldata::Subl1::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/three-list/ldata/subl1/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/three-list/ldata/subl1/name")
    {
        name = value;
    }
}

Runner::ThreeList::Ldata::Ldata()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , parent(nullptr)
{
}

Runner::ThreeList::Ldata::~Ldata()
{
}

bool Runner::ThreeList::Ldata::has_data() const
{
    for (std::size_t index=0; index<subl1.size(); index++)
    {
        if(subl1[index]->has_data())
            return true;
    }
    return number.is_set || name.is_set;
}

std::string Runner::ThreeList::Ldata::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ldata" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::ThreeList::Ldata::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/three-list/ldata";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::ThreeList::Ldata::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/three-list/ldata/subl1")
    {
        auto child = std::make_unique<Runner::ThreeList::Ldata::Subl1>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        subl1.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & Runner::ThreeList::Ldata::get_children()
{
    for (std::size_t index=0; index<subl1.size(); index++)
    {
        children.push_back(subl1[index].get());
    }
    return children;
}
void Runner::ThreeList::Ldata::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/three-list/ldata/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/three-list/ldata/name")
    {
        name = value;
    }
}

Runner::ThreeList::ThreeList()
    : parent(nullptr)
{
}

Runner::ThreeList::~ThreeList()
{
}

bool Runner::ThreeList::has_data() const
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        if(ldata[index]->has_data())
            return true;
    }
    return false;
}

std::string Runner::ThreeList::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "three-list";

    return path_buffer.str();

}

EntityPath Runner::ThreeList::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/three-list";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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

Entity* Runner::ThreeList::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/three-list/ldata")
    {
        auto child = std::make_unique<Runner::ThreeList::Ldata>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        ldata.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & Runner::ThreeList::get_children()
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        children.push_back(ldata[index].get());
    }
    return children;
}
void Runner::ThreeList::set_value(std::string value_path, std::string value)
{
}

Runner::InbtwList::Ldata::Subc::SubcSubl1::SubcSubl1()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , parent(nullptr)
{
}

Runner::InbtwList::Ldata::Subc::SubcSubl1::~SubcSubl1()
{
}

bool Runner::InbtwList::Ldata::Subc::SubcSubl1::has_data() const
{
    return number.is_set || name.is_set;
}

std::string Runner::InbtwList::Ldata::Subc::SubcSubl1::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "subc-subl1" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::InbtwList::Ldata::Subc::SubcSubl1::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        throw YDKInvalidArgumentException{"parent cannot be nullptr"};
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::InbtwList::Ldata::Subc::SubcSubl1::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::InbtwList::Ldata::Subc::SubcSubl1::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/inbtw-list/ldata/subc/subc-subl1/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/inbtw-list/ldata/subc/subc-subl1/name")
    {
        name = value;
    }
}

Runner::InbtwList::Ldata::Subc::Subc()
    : 
	name{YType::str, "name"},
	 number{YType::int32, "number"}
    , parent(nullptr)
{
}

Runner::InbtwList::Ldata::Subc::~Subc()
{
}

bool Runner::InbtwList::Ldata::Subc::has_data() const
{
    for (std::size_t index=0; index<subc_subl1.size(); index++)
    {
        if(subc_subl1[index]->has_data())
            return true;
    }
    return name.is_set || number.is_set;
}

std::string Runner::InbtwList::Ldata::Subc::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "subc";

    return path_buffer.str();

}

EntityPath Runner::InbtwList::Ldata::Subc::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        throw YDKInvalidArgumentException{"parent cannot be nullptr"};
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {name.get_name_value(), number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::InbtwList::Ldata::Subc::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/inbtw-list/ldata/subc/subc-subl1")
    {
        auto child = std::make_unique<Runner::InbtwList::Ldata::Subc::SubcSubl1>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        subc_subl1.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & Runner::InbtwList::Ldata::Subc::get_children()
{
    for (std::size_t index=0; index<subc_subl1.size(); index++)
    {
        children.push_back(subc_subl1[index].get());
    }
    return children;
}
void Runner::InbtwList::Ldata::Subc::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/inbtw-list/ldata/subc/name")
    {
        name = value;
    }
    if(value_path == "/ydktest-sanity:runner/inbtw-list/ldata/subc/number")
    {
        number = value;
    }
}

Runner::InbtwList::Ldata::Ldata()
    : 
	number{YType::int32, "number"},
	 name{YType::str, "name"}
    , 		subc(std::make_unique<Runner::InbtwList::Ldata::Subc>())
    , parent(nullptr)
{
    subc->parent = this;
    add_child(subc.get());

}

Runner::InbtwList::Ldata::~Ldata()
{
}

bool Runner::InbtwList::Ldata::has_data() const
{
    return number.is_set || name.is_set || subc->has_data();
}

std::string Runner::InbtwList::Ldata::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ldata" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::InbtwList::Ldata::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/inbtw-list/ldata";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value(), name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::InbtwList::Ldata::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/inbtw-list/ldata/subc")
    {
        subc = std::make_unique<Runner::InbtwList::Ldata::Subc>();
        subc->parent = this;
        add_child(subc.get());
        return subc.get();
    }
    return nullptr;
}

void Runner::InbtwList::Ldata::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/inbtw-list/ldata/number")
    {
        number = value;
    }
    if(value_path == "/ydktest-sanity:runner/inbtw-list/ldata/name")
    {
        name = value;
    }
}

Runner::InbtwList::InbtwList()
    : parent(nullptr)
{
}

Runner::InbtwList::~InbtwList()
{
}

bool Runner::InbtwList::has_data() const
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        if(ldata[index]->has_data())
            return true;
    }
    return false;
}

std::string Runner::InbtwList::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "inbtw-list";

    return path_buffer.str();

}

EntityPath Runner::InbtwList::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/inbtw-list";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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

Entity* Runner::InbtwList::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/inbtw-list/ldata")
    {
        auto child = std::make_unique<Runner::InbtwList::Ldata>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        ldata.push_back(std::move(child));
        return child_ptr;
    }
    return nullptr;
}

std::vector<Entity*> & Runner::InbtwList::get_children()
{
    for (std::size_t index=0; index<ldata.size(); index++)
    {
        children.push_back(ldata[index].get());
    }
    return children;
}
void Runner::InbtwList::set_value(std::string value_path, std::string value)
{
}

Runner::LeafRef::One::Two::Two()
    : 
	self_ref_one_name{YType::str, "self-ref-one-name"}
    , parent(nullptr)
{
}

Runner::LeafRef::One::Two::~Two()
{
}

bool Runner::LeafRef::One::Two::has_data() const
{
    return self_ref_one_name.is_set;
}

std::string Runner::LeafRef::One::Two::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "two";

    return path_buffer.str();

}

EntityPath Runner::LeafRef::One::Two::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/leaf-ref/one/two";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {self_ref_one_name.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::LeafRef::One::Two::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::LeafRef::One::Two::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/leaf-ref/one/two/self-ref-one-name")
    {
        self_ref_one_name = value;
    }
}

Runner::LeafRef::One::One()
    : 
	name_of_one{YType::str, "name-of-one"}
    , 		two(std::make_unique<Runner::LeafRef::One::Two>())
    , parent(nullptr)
{
    two->parent = this;
    add_child(two.get());

}

Runner::LeafRef::One::~One()
{
}

bool Runner::LeafRef::One::has_data() const
{
    return name_of_one.is_set || two->has_data();
}

std::string Runner::LeafRef::One::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "one";

    return path_buffer.str();

}

EntityPath Runner::LeafRef::One::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/leaf-ref/one";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {name_of_one.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::LeafRef::One::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/leaf-ref/one/two")
    {
        two = std::make_unique<Runner::LeafRef::One::Two>();
        two->parent = this;
        add_child(two.get());
        return two.get();
    }
    return nullptr;
}

void Runner::LeafRef::One::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/leaf-ref/one/name-of-one")
    {
        name_of_one = value;
    }
}

Runner::LeafRef::LeafRef()
    : 
	ref_inbtw{YType::str, "ref-inbtw"},
	 ref_one_name{YType::str, "ref-one-name"},
	 ref_three_sub1_sub2_number{YType::str, "ref-three-sub1-sub2-number"},
	 ref_two_sub1_number{YType::str, "ref-two-sub1-number"}
    , 		one(std::make_unique<Runner::LeafRef::One>())
    , parent(nullptr)
{
    one->parent = this;
    add_child(one.get());

}

Runner::LeafRef::~LeafRef()
{
}

bool Runner::LeafRef::has_data() const
{
    return ref_inbtw.is_set || ref_one_name.is_set || ref_three_sub1_sub2_number.is_set || ref_two_sub1_number.is_set || one->has_data();
}

std::string Runner::LeafRef::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "leaf-ref";

    return path_buffer.str();

}

EntityPath Runner::LeafRef::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/leaf-ref";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {ref_inbtw.get_name_value(), ref_one_name.get_name_value(), ref_three_sub1_sub2_number.get_name_value(), ref_two_sub1_number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::LeafRef::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/leaf-ref/one")
    {
        one = std::make_unique<Runner::LeafRef::One>();
        one->parent = this;
        add_child(one.get());
        return one.get();
    }
    return nullptr;
}

void Runner::LeafRef::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/leaf-ref/ref-inbtw")
    {
        ref_inbtw = value;
    }
    if(value_path == "/ydktest-sanity:runner/leaf-ref/ref-one-name")
    {
        ref_one_name = value;
    }
    if(value_path == "/ydktest-sanity:runner/leaf-ref/ref-three-sub1-sub2-number")
    {
        ref_three_sub1_sub2_number = value;
    }
    if(value_path == "/ydktest-sanity:runner/leaf-ref/ref-two-sub1-number")
    {
        ref_two_sub1_number = value;
    }
}

Runner::NotSupported1::NotSupported12::NotSupported12()
    : 
	some_leaf{YType::str, "some-leaf"}
    , parent(nullptr)
{
}

Runner::NotSupported1::NotSupported12::~NotSupported12()
{
}

bool Runner::NotSupported1::NotSupported12::has_data() const
{
    return some_leaf.is_set;
}

std::string Runner::NotSupported1::NotSupported12::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "not-supported-1-2";

    return path_buffer.str();

}

EntityPath Runner::NotSupported1::NotSupported12::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/not-supported-1/not-supported-1-2";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {some_leaf.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::NotSupported1::NotSupported12::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::NotSupported1::NotSupported12::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/not-supported-1/not-supported-1-2/some-leaf")
    {
        some_leaf = value;
    }
}

Runner::NotSupported1::NotSupported1()
    : 
	not_supported_leaf{YType::str, "not-supported-leaf"}
    , 		not_supported_1_2(std::make_unique<Runner::NotSupported1::NotSupported12>())
    , parent(nullptr)
{
    not_supported_1_2->parent = this;
    add_child(not_supported_1_2.get());

}

Runner::NotSupported1::~NotSupported1()
{
}

bool Runner::NotSupported1::has_data() const
{
    return not_supported_leaf.is_set || not_supported_1_2->has_data();
}

std::string Runner::NotSupported1::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "not-supported-1";

    return path_buffer.str();

}

EntityPath Runner::NotSupported1::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/not-supported-1";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {not_supported_leaf.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::NotSupported1::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/not-supported-1/not-supported-1-2")
    {
        not_supported_1_2 = std::make_unique<Runner::NotSupported1::NotSupported12>();
        not_supported_1_2->parent = this;
        add_child(not_supported_1_2.get());
        return not_supported_1_2.get();
    }
    return nullptr;
}

void Runner::NotSupported1::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/not-supported-1/not-supported-leaf")
    {
        not_supported_leaf = value;
    }
}

Runner::NotSupported2::NotSupported2()
    : 
	number{YType::int32, "number"}
    , parent(nullptr)
{
}

Runner::NotSupported2::~NotSupported2()
{
}

bool Runner::NotSupported2::has_data() const
{
    return number.is_set;
}

std::string Runner::NotSupported2::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "not-supported-2" <<"[number=" <<number.get() <<"]";

    return path_buffer.str();

}

EntityPath Runner::NotSupported2::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/not-supported-2";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {number.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::NotSupported2::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::NotSupported2::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/not-supported-2/number")
    {
        number = value;
    }
}

Runner::Runner2::Runner2()
    : 
	some_leaf{YType::str, "some-leaf"}
    , parent(nullptr)
{
}

Runner::Runner2::~Runner2()
{
}

bool Runner::Runner2::has_data() const
{
    return some_leaf.is_set;
}

std::string Runner::Runner2::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "runner-2";

    return path_buffer.str();

}

EntityPath Runner::Runner2::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent == nullptr) {
        path_buffer << "ydktest-sanity:runner/runner-2";
    } else { 
        // check if the parent is a parent
        auto p = this->parent;
        std::vector<Entity*> parents {};
        while (p != nullptr && p != parent) {
            parents.push_back(p);
            p = p->parent;
        }

        if (p == nullptr) {
            throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};
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
    std::vector<std::pair<std::string, std::string> > leaf_name_values {some_leaf.get_name_value()};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::Runner2::set_child(std::string child_path)
{
    return nullptr;
}

void Runner::Runner2::set_value(std::string value_path, std::string value)
{
    if(value_path == "/ydktest-sanity:runner/runner-2/some-leaf")
    {
        some_leaf = value;
    }
}

Runner::Runner()
    : 		inbtw_list(std::make_unique<Runner::InbtwList>()),
 	leaf_ref(std::make_unique<Runner::LeafRef>()),
 	not_supported_1(std::make_unique<Runner::NotSupported1>()),
 	one(std::make_unique<Runner::One>()),
 	one_list(std::make_unique<Runner::OneList>()),
 	runner_2(std::make_unique<Runner::Runner2>()),
 	three(std::make_unique<Runner::Three>()),
 	three_list(std::make_unique<Runner::ThreeList>()),
 	two(std::make_unique<Runner::Two>()),
 	two_list(std::make_unique<Runner::TwoList>()),
 	ytypes(std::make_unique<Runner::Ytypes>())
    , parent(nullptr)
{
    inbtw_list->parent = this;
    add_child(inbtw_list.get());

    leaf_ref->parent = this;
    add_child(leaf_ref.get());

    not_supported_1->parent = this;
    add_child(not_supported_1.get());

    one->parent = this;
    add_child(one.get());

    one_list->parent = this;
    add_child(one_list.get());

    runner_2->parent = this;
    add_child(runner_2.get());

    three->parent = this;
    add_child(three.get());

    three_list->parent = this;
    add_child(three_list.get());

    two->parent = this;
    add_child(two.get());

    two_list->parent = this;
    add_child(two_list.get());

    ytypes->parent = this;
    add_child(ytypes.get());

}

Runner::~Runner()
{
}

bool Runner::has_data() const
{
    for (std::size_t index=0; index<not_supported_2.size(); index++)
    {
        if(not_supported_2[index]->has_data())
            return true;
    }
    return inbtw_list->has_data() || leaf_ref->has_data() || not_supported_1->has_data() || one->has_data() || one_list->has_data() || runner_2->has_data() || three->has_data() || three_list->has_data() || two->has_data() || two_list->has_data() || ytypes->has_data();
}

std::string Runner::get_segment_path() const
{
    std::ostringstream path_buffer;
    path_buffer << "ydktest-sanity:runner";

    return path_buffer.str();

}

EntityPath Runner::get_entity_path(Entity* parent) const
{
    std::ostringstream path_buffer;
    if (parent != nullptr) {
        throw YDKInvalidArgumentException{"parent has to be nullptr"};
    }

    path_buffer << get_segment_path();
    std::vector<std::pair<std::string, std::string> > leaf_name_values {};
    EntityPath entity_path {path_buffer.str(), leaf_name_values};
    return entity_path;

}

Entity* Runner::set_child(std::string child_path)
{
    if(child_path == "/ydktest-sanity:runner/inbtw-list")
    {
        inbtw_list = std::make_unique<Runner::InbtwList>();
        inbtw_list->parent = this;
        add_child(inbtw_list.get());
        return inbtw_list.get();
    }
    if(child_path == "/ydktest-sanity:runner/leaf-ref")
    {
        leaf_ref = std::make_unique<Runner::LeafRef>();
        leaf_ref->parent = this;
        add_child(leaf_ref.get());
        return leaf_ref.get();
    }
    if(child_path == "/ydktest-sanity:runner/not-supported-1")
    {
        not_supported_1 = std::make_unique<Runner::NotSupported1>();
        not_supported_1->parent = this;
        add_child(not_supported_1.get());
        return not_supported_1.get();
    }
    if(child_path == "/ydktest-sanity:runner/not-supported-2")
    {
        auto child = std::make_unique<Runner::NotSupported2>();
        child->parent = this;
        Entity* child_ptr = child.get();
        add_child(child_ptr);
        not_supported_2.push_back(std::move(child));
        return child_ptr;
    }
    if(child_path == "/ydktest-sanity:runner/one")
    {
        one = std::make_unique<Runner::One>();
        one->parent = this;
        add_child(one.get());
        return one.get();
    }
    if(child_path == "/ydktest-sanity:runner/one-list")
    {
        one_list = std::make_unique<Runner::OneList>();
        one_list->parent = this;
        add_child(one_list.get());
        return one_list.get();
    }
    if(child_path == "/ydktest-sanity:runner/runner-2")
    {
        runner_2 = std::make_unique<Runner::Runner2>();
        runner_2->parent = this;
        add_child(runner_2.get());
        return runner_2.get();
    }
    if(child_path == "/ydktest-sanity:runner/three")
    {
        three = std::make_unique<Runner::Three>();
        three->parent = this;
        add_child(three.get());
        return three.get();
    }
    if(child_path == "/ydktest-sanity:runner/three-list")
    {
        three_list = std::make_unique<Runner::ThreeList>();
        three_list->parent = this;
        add_child(three_list.get());
        return three_list.get();
    }
    if(child_path == "/ydktest-sanity:runner/two")
    {
        two = std::make_unique<Runner::Two>();
        two->parent = this;
        add_child(two.get());
        return two.get();
    }
    if(child_path == "/ydktest-sanity:runner/two-list")
    {
        two_list = std::make_unique<Runner::TwoList>();
        two_list->parent = this;
        add_child(two_list.get());
        return two_list.get();
    }
    if(child_path == "/ydktest-sanity:runner/ytypes")
    {
        ytypes = std::make_unique<Runner::Ytypes>();
        ytypes->parent = this;
        add_child(ytypes.get());
        return ytypes.get();
    }
    return nullptr;
}

std::vector<Entity*> & Runner::get_children()
{
    for (std::size_t index=0; index<not_supported_2.size(); index++)
    {
        children.push_back(not_supported_2[index].get());
    }
    return children;
}
void Runner::set_value(std::string value_path, std::string value)
{
}

std::unique_ptr<Entity> Runner::clone_ptr()
{
    return std::make_unique<Runner>();
}
ChildIdentityIdentity::ChildIdentityIdentity()
     : Identity("ydktest-sanity:child-identity")
{
}

ChildIdentityIdentity::~ChildIdentityIdentity()
{
}

ChildChildIdentityIdentity::ChildChildIdentityIdentity()
     : Identity("ydktest-sanity:child-child-identity")
{
}

ChildChildIdentityIdentity::~ChildChildIdentityIdentity()
{
}


std::string YdkEnumIntTestEnum_to_string(int val)
{
    #define TOSTRING(k, v) case YdkEnumIntTestEnum::k: return v;
    switch(val)
    {
        TOSTRING(any, "any")
    }
    #undef TOSTRING
    return "";
} // YdkEnumIntTestEnum

std::string YdkEnumTestEnum_to_string(int val)
{
    #define TOSTRING(k, v) case YdkEnumTestEnum::k: return v;
    switch(val)
    {
        TOSTRING(not_set, "not-set")
        TOSTRING(none, "none")
        TOSTRING(local, "local")
        TOSTRING(remote, "remote")
    }
    #undef TOSTRING
    return "";
} // YdkEnumTestEnum

std::string Runner_Ytypes_BuiltInT_EmbededEnumEnum_to_string(int val)
{
    #define TOSTRING(k, v) case Runner::Ytypes::BuiltInT::EmbededEnumEnum::k: return v;
    switch(val)
    {
        TOSTRING(zero, "zero")
        TOSTRING(two, "two")
        TOSTRING(seven, "seven")
    }
    #undef TOSTRING
    return "";
} // EmbededEnumEnum


}
}

