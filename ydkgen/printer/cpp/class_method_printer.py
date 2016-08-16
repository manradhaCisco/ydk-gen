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
from ydkgen.api_model import Class
from ydkgen.common import get_module_name


class ClassPathMethodPrinter(object):

    """
        Print get_ydk_path method

        :attribute ctx The printer context

    """

    def __init__(self, ctx):
        self.ctx = ctx

    def print_output(self, clazz):
        """
            Print the get_ydk_path method for the clazz.

            :param `api_model.Class` clazz The class object.

        """
        self._print_get_ydk_path_header(clazz)
        self._print_get_ydk_path_body(clazz)
        self._print_get_ydk_path_trailer(clazz)

    def _print_get_ydk_path_header(self, clazz):
        self.ctx.lvl_inc()

    def _print_get_ydk_path_body(self, clazz):
        owners = []
        owner_key_props = []
        current_owner = clazz.owner
        while current_owner is not None and isinstance(current_owner, Class):
            owners.append(current_owner)
            owner_key_props.extend(current_owner.get_key_props())
            current_owner = current_owner.owner

        path = ''
        objs = list(reversed(owners)) + [clazz]
        for obj in objs:
            if path is '':
                path = "\"/%s:" % (obj.module.arg)
            path = "%s%s/" % (path, obj.stmt.arg)

        path = path.rstrip('/') + '"'

        predicates = ''
        insert_token = ' << '
        key_props = clazz.get_key_props()
        for key_prop in key_props:
            predicates += insert_token
            predicates += '"[' + get_module_name(key_prop.stmt)
            predicates += ':' + key_prop.stmt.arg + '=\'"'
            predicates += insert_token
            predicates += key_prop.name + insert_token + '"\']"'

        self.ctx.writeln('std::ostringstream path;')
        self.ctx.writeln('path << %s%s;' % (path, predicates))
        self.ctx.writeln('return path.str();')

    def _print_get_ydk_path_trailer(self, clazz):
        self.ctx.lvl_dec()
        self.ctx.bline()
