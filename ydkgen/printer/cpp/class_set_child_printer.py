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


class ClassSetChildPrinter(object):
    def __init__(self, ctx, get_path):
        self.ctx = ctx
        self.get_path = get_path

    def print_class_set_child(self, clazz, children):
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
        child_path = self.get_path(child)
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

    def _print_class_set_child_trailer(self, clazz):
        self.ctx.writeln('return nullptr;')
        self.ctx.lvl_dec()
        self.ctx.writeln('}')
        self.ctx.bline()
