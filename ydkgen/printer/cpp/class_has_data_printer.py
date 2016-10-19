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

class ClassHasDataPrinter(object):
    def __init__(self, ctx):
        self.ctx = ctx

    def print_class_has_data(self, clazz, ls, children):
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

