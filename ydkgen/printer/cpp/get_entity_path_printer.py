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
class_path_printer.py

 Printer for class methods

"""
from ydkgen.api_model import Package

class GetEntityPathPrinter(object):

    """
        Print get_ydk_path method

        :attribute ctx The printer context

    """

    def __init__(self, ctx):
        self.ctx = ctx

    def print_output(self, clazz, leafs):
        """
            Print the get_entity_path method for the clazz.

            :param `api_model.Class` clazz The class object.

        """
        self._print_get_ydk_path_header(clazz)
        self._print_get_ydk_path_func(clazz, leafs)
        self._print_get_ydk_path_trailer(clazz)

    def _print_get_ydk_path_header(self, clazz):
        self.ctx.writeln('EntityPath %s::get_entity_path() const' % clazz.qualified_cpp_name())
        self.ctx.writeln('{')
        self.ctx.lvl_inc()

    def _print_get_ydk_path_func(self, clazz, leafs):
        path = '"'
        if clazz.owner is not None and isinstance(clazz.owner, Package):
            path += clazz.owner.stmt.arg + ':'
        path += '%s"' % (clazz.stmt.arg)
        self.ctx.writeln('std::ostringstream path_buffer;')
        self.ctx.writeln('path_buffer << %s%s;' % (path, self._get_path_predicate(clazz)))
        self.ctx.writeln('std::vector<std::pair<std::string, std::string> > leaf_name_values {%s};' % (', '.join('%s.get_name_value()' % (prop.name) for prop in leafs if not prop.is_many)))
        self._print_get_ydk_path_leaflists(leafs)
        self.ctx.writeln('EntityPath entity_path {path_buffer.str(), leaf_name_values};')
        self.ctx.writeln('return entity_path;')

    def _print_get_ydk_path_leaflists(self, leafs):
        leaf_lists = [leaf for leaf in leafs if leaf.is_many]
        for leaf in leaf_lists:
            self.ctx.writeln('for( const Value & leaf : %s )' % leaf.name)
            self.ctx.writeln('{')
            self.ctx.lvl_inc()
            self.ctx.writeln('leaf_name_values.push_back(leaf.get_name_value());')
            self.ctx.lvl_dec()
            self.ctx.writeln('}')

    def _get_path_predicate(self, clazz):
        predicates = ''
        insert_token = ' << '
        key_props = clazz.get_key_props()
        for key_prop in key_props:
            predicates += insert_token
            predicates += '"[' + key_prop.stmt.arg + '=\'"'
            predicates += insert_token
            predicates += ('%s.get()') % key_prop.name + insert_token + '"\']"'
        return predicates

    def _print_get_ydk_path_trailer(self, clazz):
        self.ctx.lvl_dec()
        self.ctx.bline()
        self.ctx.writeln('}')
        self.ctx.bline()
