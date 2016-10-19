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

from ydkgen.api_model import Class, Enum

from ydkgen.common import sort_classes_at_same_level
from ydkgen.printer.file_printer import FilePrinter

from .header_enum_printer import EnumPrinter
from .class_members_printer import ClassMembersPrinter


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

    def print_trailer(self, package):
        self.ctx.bline()
        self.ctx.writeln('}')
        self.ctx.writeln('}')
        self._print_include_guard_trailer(package)
        self.ctx.bline()

    def print_body(self, package):
        self._print_classes([clazz for clazz in package.owned_elements if isinstance(clazz, Class)])
        self._print_enums_for_element(package)

    def _print_imports(self, package):
        self._print_common_imports(package)
        self._print_unique_imports(package)

    def _print_common_imports(self, package):
        self.ctx.writeln('#include <memory>')
        self.ctx.writeln('#include <vector>')
        self.ctx.writeln('#include <string>')
        self.ctx.writeln('#include "ydk/types.hpp"')
        self.ctx.writeln('#include "ydk/errors.hpp"')
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

    def _print_enums_for_element(self, element):
        enums = []
        enum_name_map = {}
        for child in element.owned_elements:
            if isinstance(child, Enum) and not child.name in enum_name_map:
                enums.append(child)
                enum_name_map[child.name] = child
        self._print_enums(enums)
        for child in element.owned_elements:
            self._print_enums_for_element(child)

    def _print_enums(self, enums):
        for enum in enums:
            self._print_enum(enum)

    def _print_enum(self, enum):
        EnumPrinter(self.ctx).print_enum(enum)

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
        members_printer = ClassMembersPrinter(self.ctx)
        members_printer.print_class_members(clazz)
        child_classes = [nested_class for nested_class in clazz.owned_elements if isinstance(nested_class, Class)]
        if len(child_classes) > 0:
            self._print_classes(child_classes)
        members_printer.print_class_inits(clazz)

    def _print_class_trailer(self, clazz):
        self.ctx.bline()
        self.ctx.lvl_dec()
        self.ctx.writeln('}; // ' + clazz.qualified_cpp_name())
        self.ctx.bline()

    def _print_include_guard_header(self, package):
        self.ctx.writeln('#ifndef _{0}_'.format(package.name.upper()))
        self.ctx.writeln('#define _{0}_'.format(package.name.upper()))
        self.ctx.bline()

    def _print_include_guard_trailer(self, package):
        self.ctx.bline()
        self.ctx.writeln('#endif /* _{0}_ */'.format(package.name.upper()))
