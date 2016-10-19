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
from ydkgen.api_model import Class, DataType, Enum


class ClassConstructorPrinter(object):
    def __init__(self, ctx):
        self.ctx = ctx

    def print_constructor(self, clazz, leafs, children):
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
