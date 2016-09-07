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
from ydkgen.api_model import Class, Package
from ydkgen.common import sort_classes_at_same_level
from ydkgen.printer.file_printer import FilePrinter


class HeaderPrinter(FilePrinter):
    def __init__(self, ctx, sort_clazz):
        super(HeaderPrinter, self).__init__(ctx)
        self.sort_clazz = sort_clazz

    def print_header(self, package):
        self._print_include_guard_header(package)
        self._print_imports(package)
        self.ctx.writeln('namespace ydk {')
        self.ctx.writeln('namespace %s {' % package.name)
        self.ctx.bline()

    def _print_imports(self, package):
        self._print_common_imports(package)
        self._print_unique_imports(package)

    def _print_common_imports(self, package):
        self.ctx.writeln('#include <memory>')
        self.ctx.writeln('#include <vector>')
        self.ctx.writeln('#include <string>')
        self.ctx.writeln('#include "ydk/types.hpp"')
        self.ctx.bline()

    def _print_unique_imports(self, package):
        if len(package.imported_types()) == 0:
            return
        imports_to_print = []
        for imported_type in package.imported_types():
            import_stmt = '#include "{0}"'.format(imported_type.get_cpp_header_name())
            if import_stmt in imports_to_print:
                continue
            else:
                imports_to_print.append(import_stmt)
        imports_to_print = sorted(imports_to_print)
        for import_to_print in imports_to_print:
            self.ctx.writeln('%s' % import_to_print)
        self.ctx.bline()

    def print_trailer(self, package):
        self.ctx.bline()
        self.ctx.writeln('}')
        self.ctx.writeln('}')
        self._print_include_guard_trailer(package)
        self.ctx.bline()

    def print_body(self, package):
        self._print_classes([clazz for clazz in package.owned_elements if isinstance(clazz, Class)])

    def _print_classes(self, clazzes):
        sorted_classes = sort_classes_at_same_level(clazzes, self.sort_clazz)
        for clazz in sorted_classes:
            self._print_class(clazz)

    def _print_class(self, clazz):
        self._print_class_header(clazz)
        self._print_class_body(clazz)
        self._print_class_trailer(clazz)

    def _print_class_header(self, clazz):
        parents = 'Entity'
        if isinstance(clazz.owner, Class):
            self.ctx.bline()
            self.ctx.writeln('public:')
        if len(clazz.extends) > 0:
            parents = ', '.join([sup.fully_qualified_cpp_name() for sup in clazz.extends])
            if clazz.is_identity():
                parents += ', virtual Identity'
            self.ctx.writeln('class ' + clazz.name + ' : public ' + parents)
        elif clazz.is_identity():
            self.ctx.writeln('class ' + clazz.name + ' : public virtual Identity')
        else:
            self.ctx.writeln('class ' + clazz.name + ' : public Entity')
        self.ctx.writeln('{')
        self.ctx.lvl_inc()

    def _print_class_body(self, clazz):
        self._print_constructor_destructor(clazz)
        self._print_class_method_declarations(clazz)
        self._print_class_members(clazz)
        child_classes = [nested_class for nested_class in clazz.owned_elements if isinstance(nested_class, Class)]
        if len(child_classes) > 0:
            self._print_classes(child_classes)
        self._print_class_inits(clazz)

    def _print_constructor_destructor(self, clazz):
        self.ctx.writeln('public:')
        self.ctx.lvl_inc()
        self.ctx.writeln(clazz.name + '();')
        self.ctx.writeln('~' + clazz.name + '();')        
        self.ctx.bline()

    def _print_class_method_declarations(self, clazz):
        if clazz.is_identity():
            return
        self._print_common_method_declarations(clazz)
        self._print_clone_ptr_method(clazz)
        self._print_get_children_method(clazz)

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
        self.ctx.writeln('EntityPath get_entity_path() const;')                
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
            return 'std::unique_ptr<%s> %s;' % (prop.property_type.fully_qualified_cpp_name(), prop.name,)

    def _get_class_inits_many(self, prop):
        if prop.is_many and isinstance(prop.property_type, Class) and not prop.property_type.is_identity():
            return 'std::vector<std::unique_ptr<%s> > %s;' % (prop.property_type.fully_qualified_cpp_name(), prop.name,)

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

    def _print_class_trailer(self, clazz):
        self.ctx.lvl_dec()
        self.ctx.bline()
        self.ctx.writeln('}; // ' + clazz.name)
        self.ctx.bline()

    def _print_include_guard_header(self, package):
        self.ctx.writeln('#ifndef _{0}_'.format(package.name.upper()))
        self.ctx.writeln('#define _{0}_'.format(package.name.upper()))
        self.ctx.bline()

    def _print_include_guard_trailer(self, package):
        self.ctx.bline()
        self.ctx.writeln('#endif /* _{0}_ */'.format(package.name.upper()))
