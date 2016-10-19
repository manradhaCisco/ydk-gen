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
from pyang.types import IntTypeSpec

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
        self.ctx.writeln('EntityPath %s::get_entity_path(Entity* parent) const' % clazz.qualified_cpp_name())
        self.ctx.writeln('{')
        self.ctx.lvl_inc()

    def _is_parent_needed_for_abs_path(self, clazz):
        c = clazz.owner
        parents = []
        
        while c is not None and not isinstance(c,Package):
            parents.append(c)
            c = c.owner
      
        for p in parents:
            key_props = p.get_key_props()
            if key_props is not None and len(key_props) > 0:
                return True 
        return False

    def _print_get_ydk_path_func(self, clazz, leafs):
        #can this class handle a nullptr
        #in which case it's absolute path can be determined
        
        self.ctx.writeln('std::ostringstream path_buffer;')

        if clazz.owner is not None and isinstance(clazz.owner, Package):
            #the parent is irrelevant here 
            self.ctx.writeln('if (parent != nullptr) {')
            self.ctx.lvl_inc()
            self.ctx.writeln('throw YDKInvalidArgumentException{"parent has to be nullptr"};')
            self.ctx.lvl_dec()
            self.ctx.writeln('}')
            self.ctx.bline()
            self.ctx.writeln('path_buffer << get_segment_path();');
        else:
            #this is not a top level 
            # is nullptr a valid parameter here
            self.ctx.writeln('if (parent == nullptr) {')
            self.ctx.lvl_inc()
            
            if self._is_parent_needed_for_abs_path(clazz):
                self.ctx.writeln('throw YDKInvalidArgumentException{"parent cannot be nullptr"};')
            else:
                parents = []
                p = clazz
                while p is not None and not isinstance(p, Package):
                    parents.append(p)
                    p = p.owner

                parents.reverse()
                path = ''
                for p in parents:
                    if len(path) == 0:
                        path += p.owner.stmt.arg
                        path += ':'
                        path += p.stmt.arg
                    else:
                        path += '/'
                        if p.stmt.i_module.arg != p.owner.stmt.i_module.arg:
                            path += p.stmt.i_module.arg
                            path += ':'
                        path += p.stmt.arg

                self.ctx.writeln('path_buffer << "%s";' % path)

            self.ctx.lvl_dec()
            self.ctx.writeln('} else { ')
            self.ctx.lvl_inc()
            
            self.ctx.writeln("// check if the parent is a parent")
            self.ctx.writeln('auto p = this->parent;')
            self.ctx.writeln('std::vector<Entity*> parents {};')
            self.ctx.writeln('while (p != nullptr && p != parent) {')
            self.ctx.lvl_inc()
            self.ctx.writeln('parents.push_back(p);')
            self.ctx.writeln('p = p->parent;')
            self.ctx.lvl_dec()
            self.ctx.writeln('}')
            self.ctx.bline()
            self.ctx.writeln('if (p == nullptr) {')
            self.ctx.lvl_inc()
            self.ctx.writeln('throw YDKInvalidArgumentException{"parent is not in the ancestor hierarchy."};')
            self.ctx.lvl_dec()
            self.ctx.writeln('}')
            self.ctx.bline()
               
            self.ctx.writeln('std::reverse(parents.begin(), parents.end());')

            self.ctx.bline()
            self.ctx.writeln('p = nullptr;')
            self.ctx.writeln('for (auto p1 : parents) {')
            self.ctx.lvl_inc()

            self.ctx.writeln('if (p) {')
            self.ctx.lvl_inc()
            self.ctx.writeln('path_buffer << "/";')
            self.ctx.lvl_dec()
            self.ctx.writeln('} else { ');
            self.ctx.lvl_inc()
            self.ctx.writeln(' p = p1;')
            self.ctx.lvl_dec()
            self.ctx.writeln('}')
            self.ctx.writeln('path_buffer << p1->get_segment_path();')  
            self.ctx.lvl_dec()
            self.ctx.writeln('}')

            self.ctx.lvl_dec()
            self.ctx.writeln('}')
       
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

class GetSegmentPathPrinter(object):

    """
        Print get_segment_path method

        :attribute ctx The printer context

    """

    def __init__(self, ctx):
        self.ctx = ctx

    def print_output(self, clazz):
        """
            Print the get_entity_path method for the clazz.

            :param `api_model.Class` clazz The class object.

        """
        self._print_get_ydk_segment_path_header(clazz)
        self._print_get_ydk_segment_path_body(clazz)
        self._print_get_ydk_segment_path_trailer(clazz)

    def _print_get_ydk_segment_path_header(self, clazz):
        self.ctx.writeln('std::string %s::get_segment_path() const' % clazz.qualified_cpp_name())
        self.ctx.writeln('{')
        
        self.ctx.lvl_inc()


    def _print_get_ydk_segment_path_body(self, clazz):
       
        path='"'
        if clazz.owner is not None:
            if isinstance(clazz.owner, Package):
                path+= clazz.owner.stmt.arg + ':'
            elif clazz.owner.stmt.i_module.arg != clazz.stmt.i_module.arg:
                path+=clazz.stmt.i_module.arg + ':'

        path+= clazz.stmt.arg
        path+='"'
        predicates = ''
        insert_token = ' <<'
        
        key_props = clazz.get_key_props()
        for key_prop in key_props:
            predicates += insert_token
            property_type = key_prop.property_type
            
            predicates += '"['
            if key_prop.stmt.i_module.arg != clazz.stmt.i_module.arg:
                predicates += key_prop.stmt.i_module.arg
                predicates += ':'
            
            predicates += key_prop.stmt.arg + '='
            
            if not isinstance(property_type, IntTypeSpec):
                predicates+= "'"
                
            predicates+='"'

            predicates += insert_token
            
            predicates += ('%s.get()') % key_prop.name + insert_token

            predicates += '"'
                
            if not isinstance(property_type, IntTypeSpec):
                predicates += "'"
                
            predicates += ']"'
            
        self.ctx.writeln('std::ostringstream path_buffer;')
        self.ctx.writeln('path_buffer << %s%s;' % (path, predicates))
        self.ctx.bline()
        self.ctx.writeln('return path_buffer.str();')

    def _print_get_ydk_segment_path_trailer(self, clazz):
        self.ctx.lvl_dec()
        self.ctx.bline()
        self.ctx.writeln('}')
        self.ctx.bline()

