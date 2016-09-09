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

class ClassGetChildrenPrinter(object):
    def __init__(self, ctx):
        self.ctx = ctx

    def print_class_get_children(self, clazz, children):
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

