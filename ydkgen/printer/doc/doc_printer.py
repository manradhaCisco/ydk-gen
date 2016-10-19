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
from pyang.error import EmitError

"""
doc_printer.py

Print rst documents for the generated Python api
"""

from ydkgen.api_model import Class, Enum, Package
from ydkgen.common import get_rst_file_name
from ydkgen.printer.meta_data_util import get_class_docstring, get_enum_class_docstring


class DocPrinter(object):
    def __init__(self, ctx, language):
        self.ctx = ctx
        self.lang = language

    def print_module_documentation(self, named_element, identity_subclasses):
        self.identity_subclasses = identity_subclasses
        self.lines = []

        if isinstance(named_element, Enum):
            self._print_enum_rst(named_element)
        elif isinstance(named_element, Class):
            self._print_class_rst(named_element)
        elif isinstance(named_element, Package):
            self._print_package_rst(named_element)
        else:
            raise EmitError('Unrecognized named_element')

        self.ctx.writelns(self.lines)
        del self.lines

    def print_table_of_contents(self, packages, bundle_name):
        self.lines = []
        if bundle_name == '':
            title = 'YDK Model API'
        else:
            title = 'YDK {0} Bundle API'.format(bundle_name)
        self._print_title(title)
        self._print_toctree(packages, is_package=True)

        self.ctx.writelns(self.lines)
        del self.lines

    def _print_package_rst(self, package):
        self._print_header(package)
        # Body / Package Comment
        self._append('%s\n' % package.name)
        if package.comment is not None:
            self._append(package.comment)

    def _print_class_rst(self, clazz):
        self._print_header(clazz)
        # Body
        self.ctx.lvl_inc()
        self._print_bases(clazz=clazz)
        self._print_class_hierarchy(clazz)
        if clazz.stmt.search_one('presence') is not None:
            self._append('This class is a :ref:`presence class<presence-class>`\n')
        self._print_docstring(clazz, get_class_docstring(
            clazz, self.lang, identity_subclasses=self.identity_subclasses))
        if not clazz.is_identity() and not clazz.is_grouping():
            self._print_class_config_method()
        self.ctx.lvl_dec()

    def _print_enum_rst(self, enumz):
        self._print_header(enumz)
        # Body
        self.ctx.lvl_inc()
        self._print_bases(clazz=enumz)
        self._print_docstring(enumz, get_enum_class_docstring(enumz))
        self.ctx.lvl_dec()

    def _append(self, line):
        _line = '%s%s' % (self.ctx.get_indent(), line)
        self.lines.append(_line)

    def _extend(self, lines):
        for line in lines:
            self._append(line)

    def _print_header(self, named_element):
        # Title
        title = named_element.name
        if isinstance(named_element, Package) and named_element.stmt.keyword == 'module':
            title = '%s module' % title
        self._print_title(title)

        # TOC Tree
        if not isinstance(named_element, Enum):
            self._print_toctree(named_element.owned_elements)

        # Tagging
        tags = []
        if isinstance(named_element, Package):
            if self.lang == 'py':
                tags.append(_get_py_module_tag(named_element))
        else:
            if self.lang == 'py':
                tags.append(_get_py_curr_module_tag(named_element))
            tags.append(_get_class_tag(named_element, self.lang))
        self._extend(tags)

    def _print_title(self, title):
        self._append(title)
        self._append('=' * len(title))
        self._append('\n')

    def _print_toctree(self, elements, is_package=False):
        self._append('.. toctree::')
        self.ctx.lvl_inc()
        self._append(':maxdepth: 1\n')

        if not is_package:
            elements.reverse()
            for elem in elements:
                if isinstance(elem, Class) or isinstance(elem, Enum):
                    self._append('%s <%s>' % (elem.name, get_rst_file_name(elem)))
        else:
            for elem in elements:
                self._append('%s <%s>' % (elem.name, get_rst_file_name(elem)))

        self._append('')
        self.ctx.lvl_dec()

    def _print_bases(self, clazz=None):
        bases = _get_class_bases(clazz, self.lang)
        if bases:
            self._append('Bases: %s\n' % (', '.join(bases)))

    def _print_class_hierarchy(self, clazz):
        if not clazz.is_identity() and not clazz.is_grouping():
            clazz_hierarchy = self._get_class_hierarchy(clazz)
            if clazz_hierarchy is not None:
                self._append(clazz_hierarchy)
                self._append('\n\n')

    def _get_class_hierarchy(self, clazz):
        parent_list = []
        parent = clazz
        while isinstance(parent, Class):
            parent_list.append(parent)
            parent = parent.owner

        clazz_hierarchy = ['Class Hierarchy \:']
        if len(parent_list) > 0:
            for parent in reversed(parent_list):
                if not clazz_hierarchy[0][-1:] == ':':
                    clazz_hierarchy.append(' \>')

                tag = _get_hierarchy_class_tag(parent, self.lang)
                clazz_hierarchy.append(tag)

            return ''.join(clazz_hierarchy)
        else:
            return None

    def _print_docstring(self, named_element, docstring):
        if(len(docstring) > 0):
            for line in docstring.split('\n'):
                if line.strip() != '':
                    self._append(line)
                    self._append('\n')

    def _print_class_config_method(self):
        self._append('.. method:: is_config()\n')
        self.ctx.lvl_inc()
        self._append("Returns True if this instance \
            represents config data else returns False")
        self.ctx.lvl_dec()
        self._append('\n')


def _get_py_module_tag(named_element):
    return '.. py:module:: %s.%s\n' % (named_element.get_py_mod_name(),
                                       named_element.name)


def _get_py_curr_module_tag(named_element):
    return '.. py:currentmodule:: %s\n' % (named_element.get_py_mod_name())


def _get_class_tag(named_element, language):
    if language == 'py':
        return '.. py:class:: %s\n' % (named_element.qn())
    elif language == 'cpp':
        return '.. cpp:class:: %s\n' % (named_element.qualified_cpp_name())


def _get_hierarchy_class_tag(named_element, language):
    if language == 'py':
        return ' :py:class:`%s <%s.%s>`' % (named_element.name,
                                             named_element.get_py_mod_name(),
                                             named_element.qn())
    elif language == 'cpp':
        return ' :cpp:class:`%s <%s>`' % (named_element.name,
                                           named_element.qualified_cpp_name())


def _get_class_bases(clazz, language):
    # check is clazz is enum class or identity class
    bases = []
    if isinstance(clazz, Enum):
        bases.append(_get_enum_base_tag(clazz, language))
        return bases

    if clazz.is_identity():
        bases.append(_get_identity_base_tag(clazz, language))
    else:
        base_tag = _get_class_base_tag(clazz, language, is_base=True)
        if base_tag:
            bases.append(base_tag)
    for item in clazz.extends:
        base_tag = _get_class_base_tag(item, language)
        if base_tag:
            bases.append(base_tag)
    return bases


def _get_enum_base_tag(clazz, language):
    if language == 'py':
        return ':class:`%s`' % 'enum.Enum'
    elif language == 'cpp':
        return ':class:`%s`' % 'ydk::Enum'


def _get_identity_base_tag(clazz, language):
    if language == 'py':
        return ':class:`%s`' % 'object'
    elif language == 'cpp':
        return ':class:`%s`' % 'ydk::Identity'


def _get_class_base_tag(clazz, language, is_base=False):
    base_tag = None
    if language == 'py':
        if is_base:
            ':class:`%s`' % (clazz.name)
        else:
            ':class:`%s`' % 'object'
    return base_tag
