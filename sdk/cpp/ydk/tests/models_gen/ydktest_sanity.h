#ifndef _YDKTEST_SANITY_
#define _YDKTEST_SANITY_

#include <memory>
#include <vector>
#include <string>
#include "ydk/types.hpp"
#include "ydk/errors.hpp"

namespace ydk {
namespace ydktest_sanity {

class BaseIdentityIdentity : public virtual Identity
{
    public:
        BaseIdentityIdentity();
        ~BaseIdentityIdentity();



    public:


}; // BaseIdentityIdentity

class SubTest : public Entity
{
    public:
        SubTest();
        ~SubTest();

        bool has_data() const;
        EntityPath get_entity_path(ydk::Entity* parent) const;
        std::string get_segment_path() const;
        Entity* set_child(std::string path);
        void set_value(std::string value_path, std::string value);
        std::unique_ptr<Entity> clone_ptr();

        Entity * parent;

    private:
        std::vector<Entity*> children;

    public:
    class OneAug : public Entity
    {
        public:
            OneAug();
            ~OneAug();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value number;
            Value name;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:


    }; // SubTest::OneAug


    public:
        std::unique_ptr<ydktest_sanity::SubTest::OneAug> one_aug;
    public:


}; // SubTest

class Runner : public Entity
{
    public:
        Runner();
        ~Runner();

        bool has_data() const;
        EntityPath get_entity_path(ydk::Entity* parent) const;
        std::string get_segment_path() const;
        Entity* set_child(std::string path);
        void set_value(std::string value_path, std::string value);
        std::unique_ptr<Entity> clone_ptr();
        std::vector<Entity*> & get_children();

        Entity * parent;

    private:
        std::vector<Entity*> children;

    public:
    class One : public Entity
    {
        public:
            One();
            ~One();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value number;
            Value name;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class OneAug : public Entity
        {
            public:
                OneAug();
                ~OneAug();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value number;
                Value name;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:


        }; // Runner::One::OneAug


        public:
            std::unique_ptr<ydktest_sanity::Runner::One::OneAug> one_aug;
        public:


    }; // Runner::One


    public:
    class Two : public Entity
    {
        public:
            Two();
            ~Two();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value number;
            Value name;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class Sub1 : public Entity
        {
            public:
                Sub1();
                ~Sub1();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value number;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:


        }; // Runner::Two::Sub1


        public:
            std::unique_ptr<ydktest_sanity::Runner::Two::Sub1> sub1;
        public:


    }; // Runner::Two


    public:
    class Three : public Entity
    {
        public:
            Three();
            ~Three();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value number;
            Value name;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class Sub1 : public Entity
        {
            public:
                Sub1();
                ~Sub1();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value number;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:
            class Sub2 : public Entity
            {
                public:
                    Sub2();
                    ~Sub2();

                    bool has_data() const;
                    EntityPath get_entity_path(ydk::Entity* parent) const;
                    std::string get_segment_path() const;
                    Entity* set_child(std::string path);
                    void set_value(std::string value_path, std::string value);
                    Value number;

                    Entity * parent;

                private:
                    std::vector<Entity*> children;

                public:


            }; // Runner::Three::Sub1::Sub2


            public:
                std::unique_ptr<ydktest_sanity::Runner::Three::Sub1::Sub2> sub2;
            public:


        }; // Runner::Three::Sub1


        public:
            std::unique_ptr<ydktest_sanity::Runner::Three::Sub1> sub1;
        public:


    }; // Runner::Three


    public:
    class Ytypes : public Entity
    {
        public:
            Ytypes();
            ~Ytypes();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class BuiltInT : public Entity
        {
            public:
                BuiltInT();
                ~BuiltInT();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value number8;
                Value number16;
                Value number32;
                Value number64;
                Value u_number8;
                Value u_number16;
                Value u_number32;
                Value u_number64;
                Value leaf_ref;
                Value deci64;
                Value name;
                Value emptee;
                Value bool_value;
                Value embeded_enum;
                Value enum_value;
                Value enum_int_value;
                Value identity_ref_value;
                Value bincoded;
                Value bits_value;
                Value younion;
                Value younion_recursive;
                std::vector<Value> llstring;
                std::vector<Value> enum_llist;
                std::vector<Value> identity_llist;
                std::vector<Value> llunion;
                std::vector<Value> younion_list;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:
                class EmbededEnumEnum;
                class YdkEnumIntTestEnum;
                class YdkEnumTestEnum;
                //class YdkEnumTestEnum;
                //class YdkEnumTestEnum;


        }; // Runner::Ytypes::BuiltInT


        public:
        class DerivedT : public Entity
        {
            public:
                DerivedT();
                ~DerivedT();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:


        }; // Runner::Ytypes::DerivedT


        public:
            std::unique_ptr<ydktest_sanity::Runner::Ytypes::BuiltInT> built_in_t;
            std::unique_ptr<ydktest_sanity::Runner::Ytypes::DerivedT> derived_t;
        public:


    }; // Runner::Ytypes


    public:
    class OneList : public Entity
    {
        public:
            OneList();
            ~OneList();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            std::vector<Entity*> & get_children();

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class Ldata : public Entity
        {
            public:
                Ldata();
                ~Ldata();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value number;
                Value name;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:


        }; // Runner::OneList::Ldata


        public:
        class OneAugList : public Entity
        {
            public:
                OneAugList();
                ~OneAugList();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                std::vector<Entity*> & get_children();
                Value enabled;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:
            class Ldata : public Entity
            {
                public:
                    Ldata();
                    ~Ldata();

                    bool has_data() const;
                    EntityPath get_entity_path(ydk::Entity* parent) const;
                    std::string get_segment_path() const;
                    Entity* set_child(std::string path);
                    void set_value(std::string value_path, std::string value);
                    Value number;
                    Value name;

                    Entity * parent;

                private:
                    std::vector<Entity*> children;

                public:


            }; // Runner::OneList::OneAugList::Ldata


            public:
                std::vector<std::unique_ptr<ydktest_sanity::Runner::OneList::OneAugList::Ldata> > ldata;
            public:


        }; // Runner::OneList::OneAugList


        public:
            std::vector<std::unique_ptr<ydktest_sanity::Runner::OneList::Ldata> > ldata;
            std::unique_ptr<ydktest_sanity::Runner::OneList::OneAugList> one_aug_list;
        public:


    }; // Runner::OneList


    public:
    class TwoList : public Entity
    {
        public:
            TwoList();
            ~TwoList();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            std::vector<Entity*> & get_children();

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class Ldata : public Entity
        {
            public:
                Ldata();
                ~Ldata();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                std::vector<Entity*> & get_children();
                Value number;
                Value name;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:
            class Subl1 : public Entity
            {
                public:
                    Subl1();
                    ~Subl1();

                    bool has_data() const;
                    EntityPath get_entity_path(ydk::Entity* parent) const;
                    std::string get_segment_path() const;
                    Entity* set_child(std::string path);
                    void set_value(std::string value_path, std::string value);
                    Value number;
                    Value name;

                    Entity * parent;

                private:
                    std::vector<Entity*> children;

                public:


            }; // Runner::TwoList::Ldata::Subl1


            public:
                std::vector<std::unique_ptr<ydktest_sanity::Runner::TwoList::Ldata::Subl1> > subl1;
            public:


        }; // Runner::TwoList::Ldata


        public:
            std::vector<std::unique_ptr<ydktest_sanity::Runner::TwoList::Ldata> > ldata;
        public:


    }; // Runner::TwoList


    public:
    class ThreeList : public Entity
    {
        public:
            ThreeList();
            ~ThreeList();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            std::vector<Entity*> & get_children();

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class Ldata : public Entity
        {
            public:
                Ldata();
                ~Ldata();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                std::vector<Entity*> & get_children();
                Value number;
                Value name;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:
            class Subl1 : public Entity
            {
                public:
                    Subl1();
                    ~Subl1();

                    bool has_data() const;
                    EntityPath get_entity_path(ydk::Entity* parent) const;
                    std::string get_segment_path() const;
                    Entity* set_child(std::string path);
                    void set_value(std::string value_path, std::string value);
                    std::vector<Entity*> & get_children();
                    Value number;
                    Value name;

                    Entity * parent;

                private:
                    std::vector<Entity*> children;

                public:
                class SubSubl1 : public Entity
                {
                    public:
                        SubSubl1();
                        ~SubSubl1();

                        bool has_data() const;
                        EntityPath get_entity_path(ydk::Entity* parent) const;
                        std::string get_segment_path() const;
                        Entity* set_child(std::string path);
                        void set_value(std::string value_path, std::string value);
                        Value number;
                        Value name;

                        Entity * parent;

                    private:
                        std::vector<Entity*> children;

                    public:


                }; // Runner::ThreeList::Ldata::Subl1::SubSubl1


                public:
                    std::vector<std::unique_ptr<ydktest_sanity::Runner::ThreeList::Ldata::Subl1::SubSubl1> > sub_subl1;
                public:


            }; // Runner::ThreeList::Ldata::Subl1


            public:
                std::vector<std::unique_ptr<ydktest_sanity::Runner::ThreeList::Ldata::Subl1> > subl1;
            public:


        }; // Runner::ThreeList::Ldata


        public:
            std::vector<std::unique_ptr<ydktest_sanity::Runner::ThreeList::Ldata> > ldata;
        public:


    }; // Runner::ThreeList


    public:
    class InbtwList : public Entity
    {
        public:
            InbtwList();
            ~InbtwList();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            std::vector<Entity*> & get_children();

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class Ldata : public Entity
        {
            public:
                Ldata();
                ~Ldata();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value number;
                Value name;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:
            class Subc : public Entity
            {
                public:
                    Subc();
                    ~Subc();

                    bool has_data() const;
                    EntityPath get_entity_path(ydk::Entity* parent) const;
                    std::string get_segment_path() const;
                    Entity* set_child(std::string path);
                    void set_value(std::string value_path, std::string value);
                    std::vector<Entity*> & get_children();
                    Value number;
                    Value name;

                    Entity * parent;

                private:
                    std::vector<Entity*> children;

                public:
                class SubcSubl1 : public Entity
                {
                    public:
                        SubcSubl1();
                        ~SubcSubl1();

                        bool has_data() const;
                        EntityPath get_entity_path(ydk::Entity* parent) const;
                        std::string get_segment_path() const;
                        Entity* set_child(std::string path);
                        void set_value(std::string value_path, std::string value);
                        Value number;
                        Value name;

                        Entity * parent;

                    private:
                        std::vector<Entity*> children;

                    public:


                }; // Runner::InbtwList::Ldata::Subc::SubcSubl1


                public:
                    std::vector<std::unique_ptr<ydktest_sanity::Runner::InbtwList::Ldata::Subc::SubcSubl1> > subc_subl1;
                public:


            }; // Runner::InbtwList::Ldata::Subc


            public:
                std::unique_ptr<ydktest_sanity::Runner::InbtwList::Ldata::Subc> subc;
            public:


        }; // Runner::InbtwList::Ldata


        public:
            std::vector<std::unique_ptr<ydktest_sanity::Runner::InbtwList::Ldata> > ldata;
        public:


    }; // Runner::InbtwList


    public:
    class LeafRef : public Entity
    {
        public:
            LeafRef();
            ~LeafRef();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value ref_one_name;
            Value ref_two_sub1_number;
            Value ref_three_sub1_sub2_number;
            Value ref_inbtw;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class One : public Entity
        {
            public:
                One();
                ~One();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value name_of_one;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:
            class Two : public Entity
            {
                public:
                    Two();
                    ~Two();

                    bool has_data() const;
                    EntityPath get_entity_path(ydk::Entity* parent) const;
                    std::string get_segment_path() const;
                    Entity* set_child(std::string path);
                    void set_value(std::string value_path, std::string value);
                    Value self_ref_one_name;

                    Entity * parent;

                private:
                    std::vector<Entity*> children;

                public:


            }; // Runner::LeafRef::One::Two


            public:
                std::unique_ptr<ydktest_sanity::Runner::LeafRef::One::Two> two;
            public:


        }; // Runner::LeafRef::One


        public:
            std::unique_ptr<ydktest_sanity::Runner::LeafRef::One> one;
        public:


    }; // Runner::LeafRef


    public:
    class NotSupported1 : public Entity
    {
        public:
            NotSupported1();
            ~NotSupported1();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value not_supported_leaf;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class NotSupported12 : public Entity
        {
            public:
                NotSupported12();
                ~NotSupported12();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value some_leaf;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:


        }; // Runner::NotSupported1::NotSupported12


        public:
            std::unique_ptr<ydktest_sanity::Runner::NotSupported1::NotSupported12> not_supported_1_2;
        public:


    }; // Runner::NotSupported1


    public:
    class NotSupported2 : public Entity
    {
        public:
            NotSupported2();
            ~NotSupported2();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value number;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:


    }; // Runner::NotSupported2


    public:
    class Runner2 : public Entity
    {
        public:
            Runner2();
            ~Runner2();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value some_leaf;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:


    }; // Runner::Runner2


    public:
        std::unique_ptr<ydktest_sanity::Runner::InbtwList> inbtw_list;
        std::unique_ptr<ydktest_sanity::Runner::LeafRef> leaf_ref;
        std::unique_ptr<ydktest_sanity::Runner::NotSupported1> not_supported_1;
        std::vector<std::unique_ptr<ydktest_sanity::Runner::NotSupported2> > not_supported_2;
        std::unique_ptr<ydktest_sanity::Runner::One> one;
        std::unique_ptr<ydktest_sanity::Runner::OneList> one_list;
        std::unique_ptr<ydktest_sanity::Runner::Runner2> runner_2;
        std::unique_ptr<ydktest_sanity::Runner::Three> three;
        std::unique_ptr<ydktest_sanity::Runner::ThreeList> three_list;
        std::unique_ptr<ydktest_sanity::Runner::Two> two;
        std::unique_ptr<ydktest_sanity::Runner::TwoList> two_list;
        std::unique_ptr<ydktest_sanity::Runner::Ytypes> ytypes;
    public:


}; // Runner

class ChildIdentityIdentity : public ydktest_sanity::BaseIdentityIdentity, virtual Identity
{
    public:
        ChildIdentityIdentity();
        ~ChildIdentityIdentity();


    public:


}; // ChildIdentityIdentity

class ChildChildIdentityIdentity : public ydktest_sanity::ChildIdentityIdentity, virtual Identity
{
    public:
        ChildChildIdentityIdentity();
        ~ChildChildIdentityIdentity();


    public:


}; // ChildChildIdentityIdentity

class YdkEnumTestEnum : public Enum
{
    public:
        static const int not_set = 0;
        static const int none = 1;
        static const int local = 2;
        static const int remote = 3;

}; // YdkEnumTestEnum

std::string YdkEnumTestEnum_to_string(int val);
    
class YdkEnumIntTestEnum : public Enum
{
    public:
        static const int any = 4096;

}; // YdkEnumIntTestEnum

std::string YdkEnumIntTestEnum_to_string(int val);
    
class Runner::Ytypes::BuiltInT::EmbededEnumEnum : public Enum
{
    public:
        static const int zero = 0;
        static const int two = 1;
        static const int seven = 7;

}; // EmbededEnumEnum

std::string Runner_Ytypes_BuiltInT_EmbededEnumEnum_to_string(int val);
}
}

#endif /* _YDKTEST_SANITY_ */

