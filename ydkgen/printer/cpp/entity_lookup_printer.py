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
entity_lookup_printer.py

 Prints top entity lookup map

"""
from ydkgen.printer.file_printer import FilePrinter


class EntityLookUpPrinter(FilePrinter):
    def __init__(self, ctx):
        super(EntityLookUpPrinter, self).__init__(ctx)
        self.headers = None
        self.entity_lookup = None

    def print_entity_lookup(self, packages, sort_clazz):
        if sort_clazz:
            packages = sorted(packages, key=lambda p: p.name)

        self._init_headers(packages)
        self._init_insert_stmts(packages, sort_clazz)
        self._print_headers()
        self._print_get_entity_lookup_func()

    def _init_headers(self, packages):
        unique_headers = set()
        self._add_common_headers(unique_headers)
        for package in packages:
            self._add_package_headers(unique_headers, package)
        self.headers = list(sorted(unique_headers))

    def _add_common_headers(self, unique_headers):
        unique_headers.add('#include "../src/top_entity_lookup.hpp"')
        unique_headers.add('#include "../src/make_unique.hpp"')
        unique_headers.add('#include "../src/entity.hpp"')

    def _add_package_headers(self, unique_headers, package):
        self._add_import_statement(unique_headers, package)
        for imported_type in package.imported_types():
            self._add_import_statement(unique_headers, imported_type)

    def _add_import_statement(self, unique_headers, named_element):
        header_name = named_element.get_cpp_header_name()
        unique_headers.add('#include "%s"' % header_name)

    def _init_insert_stmts(self, packages, sort_clazz):
        entity_lookup = {}
        for package in packages:
            top_level_entities = self._get_top_level_entities(package)
            self._add_top_level_entities(top_level_entities, entity_lookup)
        self.entity_lookup = entity_lookup

    def _get_top_level_entities(self, package):
        return [entity
                for entity in package.owned_elements
                if hasattr(entity, 'stmt') and
                entity.stmt.keyword in ('container', 'list')]

    def _add_top_level_entities(self, top_level_entities, entity_lookup):
        for top_entity in top_level_entities:
            path = '/%s:%s' % (top_entity.module.arg, top_entity.stmt.arg)
            entity_lookup[path] = top_entity.fully_qualified_cpp_name()

    def _print_headers(self):
        for header in self.headers:
            self.ctx.writeln(header)

    def _print_get_entity_lookup_func(self):
        self._print_get_entity_lookup_func_header()
        self._print_get_entity_lookup_func_body()
        self._print_get_entity_lookup_func_trailer()

    def _print_get_entity_lookup_func_header(self):
        self.ctx.bline()
        self.ctx.writelns(["namespace ydk {",
                           "std::unique_ptr<TopEntityLookUp>",
                           "TopEntityLookUp::get_entity_lookup()",
                           "{"])
        self.ctx.bline()
        self.ctx.lvl_inc()

    def _print_get_entity_lookup_func_body(self):
        self.ctx.writeln("std::unique_ptr<TopEntityLookUp> entity_lookup"
                         " = std::make_unique<TopEntityLookUp>();")
        for path in self.entity_lookup:
            self._print_insert_statement(path)
        self.ctx.writeln("return entity_lookup;")

    def _print_insert_statement(self, path):
        qualified_name = self.entity_lookup[path]
        self.ctx.writeln("entity_lookup->insert(std::string{\"%s\"}, "
                         "std::unique_ptr<%s>());"
                         % (path, qualified_name))

    def _print_get_entity_lookup_func_trailer(self):
        self.ctx.lvl_dec()
        self.ctx.writeln('}')
        self.ctx.writeln('}')
