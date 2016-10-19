#  ----------------------------------------------------------------
# Copyright 2016 Cisco Systems
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------

"""
header_printer.py

 prints C++ classes

"""
from pyang.types import UnionTypeSpec

from ydkgen.api_model import Class, Enum, Package
from ydkgen.builder import TypesExtractor


class ClassMembersPrinter(object):
    def __init__(self, ctx):
        self.ctx = ctx

    def print_class_members(self, clazz):
        self._print_constructor_destructor(clazz)
        self._print_class_method_declarations(clazz)
        self._print_class_members(clazz)

    def print_class_inits(self, clazz):
        self._print_class_inits(clazz)
        self._print_class_enums_forward_declarations(clazz)

    def _print_class_method_declarations(self, clazz):
        if clazz.is_identity():
            return
        self._print_common_method_declarations(clazz)
        self._print_clone_ptr_method(clazz)
        self._print_get_children_method(clazz)

    def _print_constructor_destructor(self, clazz):
        self.ctx.writeln('public:')
        self.ctx.lvl_inc()
        self.ctx.writeln(clazz.name + '();')
        self.ctx.writeln('~' + clazz.name + '();')
        self.ctx.bline()

    def _get_leafs(self, clazz):
        leafs = []
        for child in clazz.owned_elements:
            if child.stmt.keyword == 'leaf':
                leafs.append(child)
        return leafs

    def _get_leaf_lists(self, clazz):
        leaf_lists = []
        for child in clazz.owned_elements:
            if child.stmt.keyword == 'leaf-list':
                leaf_lists.append(child)
        return leaf_lists

    def _has_list(self, clazz):
        has_list = False
        for child in clazz.owned_elements:
            has_list = has_list or child.stmt.keyword == 'list'
        return has_list

    def _print_common_method_declarations(self, clazz):
        self.ctx.writeln('bool has_data() const;')
        self.ctx.writeln('EntityPath get_entity_path(Entity* parent) const;')
        self.ctx.writeln('std::string get_segment_path() const;')
        self.ctx.writeln('Entity* set_child(std::string path);')
        self.ctx.writeln('void set_value(std::string value_path, std::string value);')

    def _print_clone_ptr_method(self, clazz):
        if clazz.owner is not None and isinstance(clazz.owner, Package):
            self.ctx.writeln('std::unique_ptr<Entity> clone_ptr();')

    def _print_get_children_method(self, clazz):
        if self._has_list(clazz):
            self.ctx.writeln('std::vector<Entity*> & get_children();')

    def _print_class_members(self, clazz):
        if clazz.is_identity():
            self.ctx.lvl_dec()
            return
        self._print_value_members(clazz)
        self._print_parent_member(clazz)
        self._print_children_member(clazz)

    def _print_parent_member(self, clazz):
        self.ctx.writeln('Entity * parent;')
        self.ctx.lvl_dec()
        self.ctx.bline()

    def _print_children_member(self, clazz):
        self.ctx.writeln('private:')
        self.ctx.lvl_inc()
        self.ctx.writeln('std::vector<Entity*> children;')
        self.ctx.lvl_dec()

    def _print_value_members(self, clazz):
        for leaf in self._get_leafs(clazz):
            self.ctx.writeln('Value %s;' % leaf.name)
        for leaf in self._get_leaf_lists(clazz):
            self.ctx.writeln('std::vector<Value> %s;' % leaf.name)
        self.ctx.bline()

    def _print_class_inits(self, clazz):
        if clazz.is_identity() and len(clazz.extends) == 0:
            self.ctx.bline()
            return
        class_inits_properties = self._get_children(clazz)
        if len(class_inits_properties) > 0:
            self.ctx.bline()
            self.ctx.writeln('public:')
            self.ctx.lvl_inc()
            self.ctx.writelns(class_inits_properties)
            self.ctx.lvl_dec()

    def _get_class_inits_unique(self, prop):
        if isinstance(prop.property_type, Class) and not prop.property_type.is_identity():
            return 'std::unique_ptr<%s> %s;' % (prop.property_type.fully_qualified_cpp_name(), prop.name)

    def _get_class_inits_many(self, prop):
        if prop.is_many and isinstance(prop.property_type, Class) and not prop.property_type.is_identity():
            return 'std::vector<std::unique_ptr<%s> > %s;' % (prop.property_type.fully_qualified_cpp_name(), prop.name)

    def _get_children(self, clazz):
        class_inits_properties = []
        for prop in clazz.properties():
            result = None
            if not prop.is_many:
                result = self._get_class_inits_unique(prop)
            else:
                result = self._get_class_inits_many(prop)
            if result is not None:
                class_inits_properties.append(result)
        return class_inits_properties

    def _print_class_enums_forward_declarations(self, clazz):
        self.ctx.bline()
        self.ctx.writeln('public:');
        self.ctx.lvl_inc()
        for prop in clazz.properties():
            if isinstance(prop.property_type, Enum):
                self.ctx.writeln('class %s;' % prop.property_type.name)
            elif isinstance(prop.property_type, UnionTypeSpec):
                for enum_name in self._get_union_contained_enums(prop.property_type):
                    self.ctx.writeln('class %s;' % enum_name)
        self.ctx.lvl_dec()
        self.ctx.bline()

    def _get_union_contained_enums(self, union_type):
        contained_enums = set()
        for contained_type_stmt in union_type.types:
            contained_property_type = TypesExtractor().get_property_type(contained_type_stmt)
            if isinstance(contained_property_type, Enum):
                contained_enums.add(contained_property_type.name)
            elif isinstance(contained_property_type, UnionTypeSpec):
                contained_enums.update(self._get_union_contained_enums(contained_property_type))
        return contained_enums
