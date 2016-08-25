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

'''
   YDK PY converter

'''
from __future__ import print_function
from ydkgen.printer.language_bindings_printer import LanguageBindingsPrinter, _EmitArgs

from .header_printer import HeaderPrinter
from .source_printer import SourcePrinter
from .entity_lookup_printer import EntityLookUpPrinter


class CppBindingsPrinter(LanguageBindingsPrinter):

    def __init__(self, ydk_root_dir, bundle_name, generate_tests, sort_clazz):
        super(CppBindingsPrinter, self).__init__(ydk_root_dir, bundle_name, generate_tests, sort_clazz)

    def print_files(self):
        only_modules = [package.stmt for package in self.packages]
        size = len(only_modules)

        for index, package in enumerate(self.packages):
            self.print_module(index, package, size)

        self._print_entity_lookup(self.models_dir, self.packages)

    def print_module(self, index, package, size):
        print('Processing %d of %d %s' % (index + 1, size, package.stmt.pos.ref))

        self._print_header_file(package, self.models_dir)
        self._print_source_file(package, self.models_dir)

    def _print_header_file(self, package, path):
        self.print_file(get_header_file_name(path, package),
                        emit_header,
                        _EmitArgs(self.ypy_ctx, package, self.sort_clazz))

    def _print_source_file(self, package, path):
        self.print_file(get_source_file_name(path, package),
                        emit_source,
                        _EmitArgs(self.ypy_ctx, package, self.sort_clazz))

    def _print_entity_lookup(self, path, packages):
        self.print_file(get_entity_lookup_file_name(path),
                        emit_entity_lookup,
                        _EmitArgs(self.ypy_ctx, packages, self.sort_clazz))


def get_source_file_name(path, package):
    return '%s/%s.cpp' % (path, package.name)


def get_header_file_name(path, package):
    return '%s/%s.h' % (path, package.name)


def get_entity_lookup_file_name(path):
    return '%s/top_entity_lookup_gen.cpp' % (path)


def emit_header(ctx, package, sort_clazz):
    HeaderPrinter(ctx, sort_clazz).print_output(package)


def emit_source(ctx, package, sort_clazz):
    SourcePrinter(ctx, sort_clazz).print_output(package)


def emit_entity_lookup(ctx, packages, sort_clazz):
    EntityLookUpPrinter(ctx).print_entity_lookup(packages, sort_clazz)
