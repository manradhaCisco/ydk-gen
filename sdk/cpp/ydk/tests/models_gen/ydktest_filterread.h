#ifndef _YDKTEST_FILTERREAD_
#define _YDKTEST_FILTERREAD_

#include <memory>
#include <vector>
#include <string>
#include "ydk/types.hpp"
#include "ydk/errors.hpp"

namespace ydk {
namespace ydktest_filterread {

class A : public Entity
{
    public:
        A();
        ~A();

        bool has_data() const;
        EntityPath get_entity_path(ydk::Entity* parent) const;
        std::string get_segment_path() const;
        Entity* set_child(std::string path);
        void set_value(std::string value_path, std::string value);
        std::unique_ptr<Entity> clone_ptr();
        std::vector<Entity*> & get_children();
        Value a1;
        Value a2;
        Value a3;

        Entity * parent;

    private:
        std::vector<Entity*> children;

    public:
    class B : public Entity
    {
        public:
            B();
            ~B();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value b1;
            Value b2;
            Value b3;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:
        class C : public Entity
        {
            public:
                C();
                ~C();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:


        }; // A::B::C


        public:
        class D : public Entity
        {
            public:
                D();
                ~D();

                bool has_data() const;
                EntityPath get_entity_path(ydk::Entity* parent) const;
                std::string get_segment_path() const;
                Entity* set_child(std::string path);
                void set_value(std::string value_path, std::string value);
                Value d1;
                Value d2;
                Value d3;

                Entity * parent;

            private:
                std::vector<Entity*> children;

            public:
            class E : public Entity
            {
                public:
                    E();
                    ~E();

                    bool has_data() const;
                    EntityPath get_entity_path(ydk::Entity* parent) const;
                    std::string get_segment_path() const;
                    Entity* set_child(std::string path);
                    void set_value(std::string value_path, std::string value);
                    Value e1;
                    Value e2;

                    Entity * parent;

                private:
                    std::vector<Entity*> children;

                public:


            }; // A::B::D::E


            public:
                std::unique_ptr<ydktest_filterread::A::B::D::E> e;
            public:


        }; // A::B::D


        public:
            std::unique_ptr<ydktest_filterread::A::B::C> c;
            std::unique_ptr<ydktest_filterread::A::B::D> d;
        public:


    }; // A::B


    public:
    class Lst : public Entity
    {
        public:
            Lst();
            ~Lst();

            bool has_data() const;
            EntityPath get_entity_path(ydk::Entity* parent) const;
            std::string get_segment_path() const;
            Entity* set_child(std::string path);
            void set_value(std::string value_path, std::string value);
            Value number;
            Value value_;

            Entity * parent;

        private:
            std::vector<Entity*> children;

        public:


    }; // A::Lst


    public:
        std::unique_ptr<ydktest_filterread::A::B> b;
        std::vector<std::unique_ptr<ydktest_filterread::A::Lst> > lst;
    public:


}; // A


}
}

#endif /* _YDKTEST_FILTERREAD_ */

