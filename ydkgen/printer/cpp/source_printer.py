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
source_printer.py

 prints C++ classes

"""
from ydkgen.api_model import Class, DataType, Enum, Package
from ydkgen.common import sort_classes_at_same_level
from ydkgen.printer.file_printer import FilePrinter
from .get_entity_path_printer import GetEntityPathPrinter


class SourcePrinter(FilePrinter):
    def __init__(self, ctx, sort_clazz):
        super(SourcePrinter, self).__init__(ctx)
        self.sort_clazz = sort_clazz

    def print_header(self, package):
        self.ctx.bline()
        self.ctx.writeln('#include <sstream>')
        self.ctx.writeln('#include <iostream>')
        self.ctx.writeln('#include "{0}.h"'.format(package.name))
        self.ctx.bline()
        self.ctx.writeln('namespace ydk {')
        self.ctx.writeln('namespace %s {' % package.name)
        self.ctx.bline()

    def print_trailer(self, package):
        self.ctx.bline()
        self.ctx.writeln('}')
        self.ctx.writeln('}')
        self.ctx.bline()

    def print_body(self, package):
        self._print_classes([clazz for clazz in package.owned_elements if isinstance(clazz, Class)])

    def _print_classes(self, clazzes):
        sorted_classes = sort_classes_at_same_level(clazzes, self.sort_clazz)
        for clazz in sorted_classes:
            self._print_class(clazz)

    def _print_class(self, clazz):
        self._print_classes([nested_class for nested_class in clazz.owned_elements if isinstance(nested_class, Class)])
        self._print_class_body(clazz)

    def _print_class_body(self, clazz):
        leafs = []
        children = []
        self._get_class_members(clazz, leafs, children)
        self._print_class_constructor(clazz, leafs, children)
        self._print_class_destructor(clazz)
        self._print_class_method_definitions(clazz, leafs, children)

    def _print_class_method_definitions(self, clazz, leafs, children):
        if clazz.is_identity():
            return
        self._print_class_has_data(clazz, leafs, children)
        self._print_class_get_path(clazz, leafs)
        self._print_class_set_child(clazz, children)
        self._print_class_get_children(clazz, children)
        self._print_class_set_value(clazz, leafs)
        self._print_clone_ptr_method(clazz, leafs)

    def _print_class_constructor(self, clazz, leafs, children):
        self._print_class_constructor_header(clazz, leafs, children)
        self._print_class_constructor_body(clazz, leafs, children)
        self._print_class_constructor_trailer()

    def _print_class_constructor_header(self, clazz, leafs, children):
        self.ctx.writeln(clazz.qualified_cpp_name() + '::' + clazz.name + '()')
        self.ctx.lvl_inc()
        if clazz.is_identity():
            self.ctx.writeln(' : Identity("%s:%s")' % (clazz.module.arg, clazz.stmt.arg))
        else:
            self._print_class_inits(clazz, leafs, children)
        self.ctx.lvl_dec()
        self.ctx.writeln('{')
        self.ctx.lvl_inc()

    def _print_class_constructor_body(self, clazz, leafs, children):
        self._print_init_children(children)
        self._print_init_leafs(leafs)

    def _print_init_children(self, children):
        for child in children:
            if child.is_many:
                continue
            self.ctx.writeln('%s->parent = this;' % child.name)
            self.ctx.writeln('add_child(%s.get());' % child.name)
            self.ctx.bline()

    def _print_init_leafs(self, leafs):
        for leaf in leafs:
            if not leaf.is_many and isinstance(leaf.property_type, Enum):
                self.ctx.writeln('%s.enum_to_string_func = %s::%s_to_string;' % (leaf.name, leaf.property_type.get_package().name, leaf.property_type.qualified_cpp_name().replace('::', '_')))

    def _print_class_constructor_trailer(self):
        self.ctx.lvl_dec()
        self.ctx.writeln('}')
        self.ctx.bline()

    def _print_class_destructor(self, clazz):
        self.ctx.writeln(clazz.qualified_cpp_name() + '::~' + clazz.name + '()')
        self.ctx.writeln('{')
        self.ctx.writeln('}')
        self.ctx.bline()

    def _print_class_inits(self, clazz, ls, children):
        leafs = [prop for prop in ls if not prop.is_many]
        if len(leafs) > 0:
            self.ctx.writeln(': %s' % ', '.join('%s{YType::%s, "%s"}' % (prop.name, get_type_name(prop.property_type), prop.stmt.arg) for prop in leafs))
        children_init = ''
        parent_init = ''
        if len(leafs) > 0:
            children_init = ', '
        else:
            children_init = ': '
        chs = [prop for prop in children if not prop.is_many]
        children_init += '%s' % (', '.join('%s(std::make_unique<%s>())' % (prop.name, prop.property_type.qualified_cpp_name()) for prop in chs))
        if len(chs) > 0:
            self.ctx.writeln('%s' % (children_init))
        if len(chs) > 0 or len(leafs) > 0:
            parent_init = ', '
        if len(chs) == 0 and len(leafs) == 0:
            parent_init = ': '
        parent_init += 'parent(nullptr)'
        self.ctx.writeln('%s' % parent_init)

    def _print_class_has_data(self, clazz, ls, children):
        leafs = [prop for prop in ls if not prop.is_many]
        chs = [prop for prop in children if not prop.is_many]
        conditions = [ '%s.is_set' % (p.name) for p in leafs ]
        conditions.extend(['%s->has_data()' % (p.name) for p in chs])
        self.ctx.writeln('bool %s::has_data() const' % clazz.qualified_cpp_name())
        self.ctx.writeln('{')
        self.ctx.lvl_inc()
        for child in children:
            if child.is_many:
                self._print_class_has_data_many(child, '->has_data()')
        for leaf in ls:
            if leaf.is_many:
                self._print_class_has_data_many(leaf, '.is_set')
        if len(conditions) == 0:
            self.ctx.writeln('return false;')
        else:
            self.ctx.writeln('return %s;' % ' || '.join(conditions))
        self.ctx.lvl_dec()
        self.ctx.writeln('}')
        self.ctx.bline()

    def _print_class_get_children(self, clazz, children):
        chs = [child  for child in children if child.is_many]
        if(len(chs) == 0):
            return
        self.ctx.writeln('std::vector<Entity*> & %s::get_children()' % clazz.qualified_cpp_name())
        self.ctx.writeln('{')
        self.ctx.lvl_inc()
        for child in chs:
            self._print_class_get_child(child)
        self.ctx.writeln('return children;')
        self.ctx.lvl_dec()
        self.ctx.writeln('}')

    def _print_class_get_child(self, child):
        self.ctx.writeln('for (std::size_t index=0; index<%s.size(); index++)' % child.name)
        self.ctx.writeln('{')
        self.ctx.lvl_inc()
        self.ctx.writeln('children.push_back(%s[index].get());' % child.name)
        self.ctx.lvl_dec()
        self.ctx.writeln('}')

    def _print_class_has_data_many(self, child, check_function):
        self.ctx.writeln('for (std::size_t index=0; index<%s.size(); index++)' % child.name)
        self.ctx.writeln('{')
        self.ctx.lvl_inc()
        self.ctx.writeln('if(%s[index]%s)' % (child.name, check_function))
        self.ctx.lvl_inc()
        self.ctx.writeln('return true;')
        self.ctx.lvl_dec()
        self.ctx.lvl_dec()
        self.ctx.writeln('}')

    def _print_class_get_path(self, clazz, leafs):
        GetEntityPathPrinter(self.ctx).print_output(clazz, leafs)

    def _print_class_set_child(self, clazz, children):
        self._print_class_set_child_header(clazz)
        self._print_class_set_children_body(children)
        self._print_class_set_child_trailer(clazz)

    def _print_class_set_child_header(self, clazz):
        self.ctx.writeln('Entity* %s::set_child(std::string child_path)' % clazz.qualified_cpp_name())
        self.ctx.writeln('{')
        self.ctx.lvl_inc()

    def _print_class_set_children_body(self, children):
        for child in children:
            self._print_class_set_child_body(child)
            
    def _print_class_set_child_body(self, child):
        child_path = self._get_path(child)
        self.ctx.writeln('if(child_path == "%s")' % (child_path))
        self.ctx.writeln('{')
        self.ctx.lvl_inc()
        if child.is_many:
            self._print_class_set_child_body_many(child)
        else:
            self._print_class_set_child_body_unique(child)
        self.ctx.lvl_dec()
        self.ctx.writeln('}')

    def _print_class_set_child_body_many(self, child):
        self.ctx.writeln('auto child = std::make_unique<%s>();' % (child.property_type.qualified_cpp_name()))
        self.ctx.writeln('child->parent = this;')
        self.ctx.writeln('Entity* child_ptr = child.get();')
        self.ctx.writeln('add_child(child_ptr);')
        self.ctx.writeln('%s.push_back(std::move(child));' % child.name)
        self.ctx.writeln('return child_ptr;')

    def _print_class_set_child_body_unique(self, child):
        self.ctx.writeln('%s = std::make_unique<%s>();' % (child.name, child.property_type.qualified_cpp_name()))
        self.ctx.writeln('%s->parent = this;' % child.name)
        self.ctx.writeln('add_child(%s.get());' % child.name)
        self.ctx.writeln('return %s.get();' % child.name)

    def _get_path(self, prop):
        path = prop.stmt.arg
        current_owner = prop.owner
        while(current_owner is not None) and not isinstance(current_owner, Package):
            if current_owner.owner is not None and isinstance(current_owner.owner, Package):
                path = '/' + current_owner.owner.stmt.arg + ':' + current_owner.stmt.arg + '/' + path
            else:
                path = current_owner.stmt.arg + '/' + path
            current_owner = current_owner.owner
        return path

    def _print_class_set_child_trailer(self, clazz):
        self.ctx.writeln('return nullptr;')
        self.ctx.lvl_dec()
        self.ctx.writeln('}')
        self.ctx.bline()

    def _print_class_set_value(self, clazz, leafs):
        self._print_class_set_value_header(clazz)
        self._print_class_set_value_body(leafs)
        self._print_class_set_value_trailer(clazz)

    def _print_class_set_value_header(self, clazz):
        self.ctx.writeln('void %s::set_value(std::string value_path, std::string value)' % clazz.qualified_cpp_name())
        self.ctx.writeln('{')
        self.ctx.lvl_inc()

    def _print_class_set_value_body(self, leafs):
        for leaf in leafs:
            self._print_class_set_values(leaf)

    def _print_class_set_values(self, leaf):
        child_path = self._get_path(leaf)
        self.ctx.writeln('if(value_path == "%s")' % (child_path))
        self.ctx.writeln('{')
        self.ctx.lvl_inc()
        if(leaf.is_many):
            self.ctx.writeln('//%s.push_back(value);' % leaf.name)
        else:
            self.ctx.writeln('%s = value;' % leaf.name)
        self.ctx.lvl_dec()
        self.ctx.writeln('}')

    def _print_class_set_value_trailer(self, clazz):
        self.ctx.lvl_dec()
        self.ctx.writeln('}')
        self.ctx.bline()

    def _print_clone_ptr_method(self, clazz, leafs):
        if clazz.owner is not None and isinstance(clazz.owner, Package):
            self.ctx.writeln('std::unique_ptr<Entity> %s::clone_ptr()' % clazz.qualified_cpp_name())
            self.ctx.writeln('{')
            self.ctx.lvl_inc()
            self.ctx.writeln('return std::make_unique<%s>();' % clazz.qualified_cpp_name())
            self.ctx.lvl_dec()
            self.ctx.writeln('}')

    def _get_class_members(self, clazz, leafs, children):
        for prop in clazz.properties():
            ptype = prop.property_type
            if isinstance(prop.property_type, Class) and not prop.property_type.is_identity():
                children.append(prop)
            elif ptype is not None:
                leafs.append(prop)


def get_type_name(prop_type):
    if prop_type.name == 'string':
        return 'str'
    elif prop_type.name == 'leafref':
        return 'str'
    elif prop_type.name == 'decimal64':
        return 'str'
    elif prop_type.name == 'union':
        return 'str'
    elif prop_type.name == 'binary':
        return 'str'
    elif isinstance(prop_type, Class) and prop_type.is_identity():
        return 'identityref'
    elif isinstance(prop_type, Enum):
        return 'enumeration'
    elif isinstance(prop_type, DataType):
        return 'str'
    return prop_type.name
