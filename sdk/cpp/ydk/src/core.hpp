//
// @file core.hpp
// @brief The main ydk public header.
//
// YANG Development Kit
// Copyright 2016 Cisco Systems. All rights reserved
//
////////////////////////////////////////////////////////////////
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
//////////////////////////////////////////////////////////////////

#ifndef YDK_CORE_HPP
#define YDK_CORE_HPP

#include <string>
#include <vector>

namespace ydk {
    namespace core {
        
        ///
        /// @mainpage About
        ///
        /// A generic API for working with YANG modelled data.
        ///
        /// @section about-features Main Features
        ///
        /// - A ServiceProvider interface for ServiceProviders
        /// - A Repository class to help create a SchemaNode Tree based on capabilities.
        /// - Creation of DataNode Tree's for config and oper
        /// - Create and Invoke RPC's
        /// - ValidationService that validates DataNode Tree's based on criteria.
        /// - CodecService that can encode a DataNode to XML/JSON and decode from XML/JSON to a dataNode tree
        ///
        /// @section about-license License
        ///
        /// YANG Development Kit
        /// Copyright 2016 Cisco Systems. All rights reserved
        ///
        ///
        /// Licensed to the Apache Software Foundation (ASF) under one
        /// or more contributor license agreements.  See the NOTICE file
        /// distributed with this work for additional information
        /// regarding copyright ownership.  The ASF licenses this file
        /// to you under the Apache License, Version 2.0 (the
        /// "License"); you may not use this file except in compliance
        /// with the License.  You may obtain a copy of the License at
        ///
        ///   http://www.apache.org/licenses/LICENSE-2.0
        ///
        ///  Unless required by applicable law or agreed to in writing,
        /// software distributed under the License is distributed on an
        /// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
        /// KIND, either express or implied.  See the License for the
        /// specific language governing permissions and limitations
        /// under the License.
        ///
        
        
        ///
        /// @page howto How To ...
        ///
        /// - @subpage howtoexceptions
        /// - @subpage howtoserviceprovider
        /// - @subpage howtoschemas
        /// - @subpage howtodata
        /// - @subpage howtomemory
        /// - @subpage howtopath
        /// - @subpage howtorpc
        /// - @subpage howtovalidation
        /// - @subpage howtocodec
        /// - @subpage howtologger
        ///
        
        ///
        /// @page howtoexceptions Exceptions and Error Handling.
        /// TODO
        ///
        
        ///
        /// @page howtoserviceprovider ServiceProvider.
        /// A ServiceProvider extends the class ydk::ServiceProvider
        /// and provides an interface to obtain the root SchemaTree
        /// based on the set of Capability(s) supported by it.
        ///
        /// The ServiceProvider is also responsible for Servicing RPC's.
        ///
        /// @section ServiceProvider Errors
        /// TODO
        ///
        /// @section Capability
        /// A capability is a tuple that contains
        /// - module-name
        /// - revision
        /// - set of enabled features
        /// - set of deviations active on this module
        ///
        /// @note to ServiceProvider implementors
        /// Use the Repository class to instantiate a SchemaTree based on the Capabilities.
        ///
        ///
        
        ///
        /// @page howtoschemas SchemaNode Tree
        /// Talk about SchemaNodeTree , RootSchemaNodeTree.
        /// Memory management of trees. DO NOT get rid of the SchemaTree if there are DataTree's referencing it
        /// Thread safety
        /// Inspecting YANG meta data
        /// Traversing the hierarchy (iterations and find)
        ///
        
        ///
        /// @page howtodata DataNode Tree
        /// How to create DataNode Trees
        /// Memory management
        /// Quirks
        /// Accessing the Schema Tree
        /// Traversing the hierarchy (iteration and find)
        ///
        
        ///
        /// @page howtomemory
        /// Node containment hierarchies
        /// Best practices
        /// TODO
        
        /// @page howtopath Path Syntax.
        /// Full XPath notation is supported for find operations on DataNode(s). This XPath conforms to the YANG specification
        /// (RFC 6020 section 6.4). Some useful examples:
        ///
        /// - get all top-level nodes of the __module-name__
        ///
        /// /module-name:*
        ///
        /// - get all the descendants of __container__ (excluding __container__)
        ///
        ///  /module-name:container//\asterisk
        ///
        /// - get __list__ instance with __key1__ of value __1__ and
        ///    __key2__ of value __2__ (this can return more __list__ instances if there are more keys
        ///    than __key1__ and __key2__)
        ///
        /// /module-name:container/list[key1='1'][key2='2']
        ///
        /// - get __leaf-list__ instance with the value __val__
        ///
        /// /module-name:container/leaf-list[.='val']
        ///
        /// - get __aug-leaf__, which was added to __module-name__ from an augment module __augment-module__
        ///
        /// /module-name:container/container2/augment-module:aug-cont/aug-leaf
        ///
        ///
        /// A very small subset of this full XPath is recognized by DataNode#create. Basically, only a relative or absolute
        /// path can be specified to identify a new data node. However, lists must be identified by all their keys and created
        /// with all of them, so for those cases predicates are allowed. Predicates must be ordered the way the keys are
        /// ordered and all the keys must be specified. Every predicate includes a single key with its value. Optionally,
        /// leaves and leaf-lists can have predicates specifying their value in the path itself. All these paths are valid
        /// XPath expressions. Example: (Relative to Root Data or RootSchemaNode)
        ///
        /// ietf-yang-library:modules-state/module[name='ietf-yang-library'][revision='']/conformance[.='implement']
        ///
        /// Almost the same XPath is accepted by SchemaNode methods. The difference is that it is not used on data, but schema,
        /// which means there are no key values and only one node matches one path. In effect, lists do not have to have any
        /// predicates. If they do, they do not need to have all the keys specified and if values are included, they are
        /// ignored. Nevertheless, any such expression is still a valid XPath, but can return more nodes if executed on a
        /// data tree.
        /// Examples (all returning the same node):
        ///
        /// ietf-yang-library:modules-state/module/submodules
        /// ietf-yang-library:modules-state/module[name]/submodules
        /// ietf-yang-library:modules-state/module[name][revision]/submodules
        /// ietf-yang-library:modules-state/module[name='ietf-yang-library'][revision]/submodules
        ///
        ///
        /// Also note, that in all cases the node's prefix is specified as the name of the appropriate YANG schema. Any node
        /// can be prefixed by the module name. However, if the prefix is omitted, the module name is inherited from the
        /// previous (parent) node. It means, that the first node in the path is always supposed to have a prefix.
        ///
        
        ///
        /// @page howtorpc Rpc
        /// An Rpc represents an instance of the YANG Rpc schema node.
        /// To invoke and Rpc the user first creates and Rpc using the RootSchema#create_rpc call passing in
        /// a path with the name of the rpc For example auto get_config = root_schema->create_rpc("ietf-netconf:get-config")
        ///
        /// The input DataNode can be obtained using Rpc#input. This can be used to populate/create the child
        /// nodes of input as per this rpc's schema.
        /// The Rpc is a callable that takes  a single argument which is the ServiceProvider. To invoke the rpc
        /// do this
        /// auto config = get_config(sp); /// sp is a service provider
        /// The config variable above is the DataNode representing the output of the rpc
        ///
        ///
        
        ///
        /// @page howtovalidation Validation
        /// DataNode Tree can be validated using the ValidationService
        ///
        
        
        
        ///
        /// @page howtocodec Encoding and Decoding.
        /// A given DataNode Tree can be encoded and decoded into a variety of formats using the CodecService.
        //
        /// DataNode Tree can be validated using the ValidationService
        ///
        
        ///
        /// @page howtologger
        /// TODO
        ///
        
        // Forward References
        class DataNode ;
        class Rpc;
        class SchemaNode ;
        class RootSchemaNode ;
        
        ///
        /// @brief Validation Service
        ///
        /// Instances of this class can validate the DataNode Tree based
        /// on the option supplied.
        ///
        class ValidationService
        {
        public:
            
            virtual ~ValidationService() {};
            
            ///
            /// @brief Options for validation.
            ///
            /// All validation is performed in the context of some operation.
            /// These options capture the context of use.
            ///
            enum class Option {
                DATASTORE,  /// Datastore validation. Note the DataNode Tree should contain everything for cross reference resolution
                GET_CONFIG, // Get config validation. Checks to see if only config nodes are references
                GET, // Get validation
                EDIT_CONFIG // Edit validation. Checks on the values of leafs etc
            };
            
            ///
            /// @brief validates dn based on the option
            ///
            /// @param[in] dn The root of DataNode tree to validate.
            /// @param[in] option The context for validation.
            /// @throws YDKValidationException if validation errors were detected.
            /// @throws YDKInvalidArgumentException if the arguments are invalid.
            ///
            virtual void validate(const DataNode* dn, Option option);
        };
        
        ///
        /// @brief CodecService
        ///
        /// The Encode/Decode Service.
        ///
        class CodecService
        {
        public:
            
            virtual ~CodecService() {}
            
            ///
            /// @brief Options for encode
            ///
            /// These options can be used for encoding the given tree
            enum class Format {
                XML, /// XML
                JSON /// JSON
                
            };
            
            
            
            ///
            /// @brief encode the given DataNode Tree
            ///
            /// @param[in] dn The DataNode to encode
            /// @param[in] format to encode to.
            /// @param[in] pretty if true the output is indented for human consumption.
            /// @return The encoded string.
            //  @throws YDKInvalidArgumentException if the arguments are invalid.
            ///
            virtual std::string encode(const DataNode* dn, Format format, bool pretty);
            
            ///
            /// @brief decode the buffer to return a DataNode
            ///
            /// @param[in] root_schema The root schema to use.
            /// @param[in] buffer The string representation of the DataNode.
            /// @param[in] format .Note ::TREE is not supported.
            /// @return The DataNode instantiated or nullptr in case of error.
            /// @throws YDKInvalidArgumentException if the arguments are invalid.
            ///
            virtual DataNode* decode(const RootSchemaNode* root_schema, const std::string& buffer, Format format);
            
        };
        
        
        ///
        /// @brief Base class for YDK Exceptions
        ///
        /// The subclasses give a specialized view of the error that has occurred.
        ///
        struct YDKException
        {
            YDKException(const std::string& msg) : err_msg{msg}
            {
                
            }
            
            std::string err_msg;
        };
        
        ///
        /// @brief Invalid Argument
        ///
        /// Use the err_msg for the error.
        ///
        struct YDKInvalidArgumentException : public YDKException
        {
            YDKInvalidArgumentException(const std::string& msg) : YDKException{msg}
            {
                
            }
        };
        
        ///
        /// @brief Operation Not Supported Exception
        ///
        /// Thrown when an operation is not supported.
        ///
        struct YDKOperationNotSupportedException : public YDKException
        {
            
        };
        
        ///
        /// @brief Core Library Exception.
        ///
        /// If thrown it could indicate a problem with the library.
        /// Analyze logs.
        ///
        struct YDKCoreException : public YDKException
        {
            YDKCoreException(): YDKException{"Core exception"}
            {
                
            }
        };
        
        ///
        /// @brief Exception that encapsualtes the validation errors
        ///        on a data tree
        ///
        struct YDKDataValidationException : public YDKException
        {
            /// Data Validation Error Enum
            enum class Error {
                SUCCESS,  /// no error
                
                TOOMANY,      /// too many instances of some object
                DUPLEAFLIST,  /// multiple instances of leaf-list
                DUPLIST,      /// multiple instances of list
                NOUNIQ,       /// unique leaves match on 2 list instances (data)
                OBSDATA,      /// obsolete data instantiation (data) */
                /* */
                NORESOLV,     /// no resolvents found for an expression (data) */
                INELEM,       /// nvalid element (data) */
                /* */
                MISSELEM,     /// missing required element (data) */
                INVAL,        /// invalid value of an element (data) */
                INVALATTR,    /// invalid attribute value (data) */
                INATTR,       /// invalid attribute in an element (data) */
                MISSATTR,     /// missing attribute in an element (data) */
                NOCONSTR,     /// value out of range/length/pattern (data) */
                INCHAR,       /// unexpected characters (data) */
                INPRED,       /// predicate resolution fail (data) */
                MCASEDATA,    /// data for more cases of a choice (data) */
                NOMUST,       /// unsatisfied must condition (data) */
                NOWHEN,       /// unsatisfied when condition (data) */
                INORDER,      /// invalid order of elements (data) */
                INWHEN,      /// irresolvable when condition (data) */
                NOMIN,        /// min-elements constraint not honored (data) */
                NOMAX,        /// max-elements constraint not honored (data) */
                NOREQINS,     /// required instance does not exits (data) */
                NOLEAFREF,    /// eaf pointed to by leafref does not exist (data) */
                NOMANDCHOICE, /// no mandatory choice case branch exists (data) */
                
                
            };
            
            /// List of pair<DataNode, ValidationError>. The Validation Error is specific to
            /// this node
            std::vector<std::pair<DataNode*,Error>> errors;
            
        };
        
        struct YDKPathException : public YDKException
        {
            enum class Error {
                SUCCESS,  /// no error
                
                XPATH_INTOK,  /// unexpected XPath token
                XPATH_EOF,    /// unexpected end of an XPath expression
                XPATH_INOP,   /// invalid XPath operation operands
                /* */
                XPATH_INCTX,  /// invalid XPath context type
                
                PATH_INCHAR,  /// invalid characters (path)
                PATH_INMOD,   /// invalid module name (path)
                PATH_MISSMOD, /// missing module name (path)
                PATH_INNODE,  /// invalid node name (path)
                PATH_INKEY,   /// invalid key name (path)
                PATH_MISSKEY, /// missing some list keys (path)
                PATH_EXISTS,  /// target node already exists (path)
                PATH_MISSPAR, /// some parent of the target node is missing (path)
                PATH_AMBIGUOUS //// thrown in create where the path expression cannot uniquely identify a given node
            };
            
            Error err;
            
            YDKPathException(YDKPathException::Error error_code) : YDKException{"Invalid path expression"}, err{error_code}
            {
                
            }
        };
        
        struct YDKCodecException : public YDKException
        {
            enum class Error {
                SUCCESS,  /// no error
                
                XML_MISS,     ///  missing XML object
                XML_INVAL,    ///  invalid XML object
                XML_INCHAR,   /// invalid XML character
                
                EOF_ERR,      /// unexpected end of input data
                
            };
            
            Error err;
            
            YDKCodecException(YDKCodecException::Error merror);
        };
        
        struct YDKSchemaValidationException : public YDKException
        {
            enum class Error {
                SUCCESS,  /// no error
                
                INSTMT,       /// invalid statement (schema)
                /* */
                INID,         /// nvalid identifier (schema)
                INDATE,       /// invalid date format
                INARG,        /// invalid value of a statement argument (schema)
                MISSSTMT,     /// missing required statement (schema)
                /* */
                MISSARG,      /// missing required statement argument (schema)
                TOOMANY,      /// too many instances of some object
                DUPID,        /// duplicated identifier (schema)
                DUPLEAFLIST,  /// multiple instances of leaf-list
                DUPLIST,      /// multiple instances of list
                NOUNIQ,       /// unique leaves match on 2 list instances (data)
                ENUM_DUPVAL,  //// duplicated enum value (schema)
                ENUM_DUPNAME, /// duplicated enum name (schema)
                ENUM_WS,      /// enum name with leading/trailing whitespaces (schema)
                BITS_DUPVAL,  /// duplicated bits value (schema)
                BITS_DUPNAME, //// duplicated bits name (schema)
                INMOD,        /// nvalid module name
                /* */
                KEY_NLEAF,    /// list key is not a leaf (schema)
                KEY_TYPE,     /// invalid list key type (schema)
                KEY_CONFIG,   /// key config value differs from the list config value
                KEY_MISS,     /// list key not found (schema)
                KEY_DUP,      /// duplicated key identifier (schema)
                INREGEX,      /// nvalid regular expression (schema)
                INRESOLV,     /// no resolvents found (schema)
                INSTATUS,     /// nvalid derivation because of status (schema) */
                CIRC_LEAFREFS,/// circular chain of leafrefs detected (schema) */
                CIRC_IMPORTS, /// circular chain of imports detected (schema) */
                CIRC_INCLUDES,/// circular chain of includes detected (schema) */
                
            };
            
            /// The errors in form of pair {SchemaNode*, Error}
            std::vector<std::pair<SchemaNode*, Error>> errors;
        };
        
        ///
        /// @brief represents the YANG Statement
        ///
        struct Statement {
            
            Statement(): keyword{}, arg{}
            {
                
            }
            
            Statement(const std::string& mkeyword, const std::string& marg) : keyword{mkeyword}, arg{marg}
            {
                
            }
            
            
            Statement(const Statement& stmt) : keyword{stmt.keyword}, arg{stmt.arg}
            {
                
                
            }
            
            Statement(Statement&& stmt) : keyword{std::move(stmt.keyword)}, arg{std::move(stmt.arg)}
            {
                
            }
            
            ~Statement(){};
            
            Statement& operator=(const Statement& stmt)
            {
                keyword = stmt.keyword;
                arg = stmt.arg;
                return *this;
            }
            
            Statement& operator=(Statement&& stmt)
            {
                keyword = std::move(stmt.keyword);
                arg = std::move(stmt.arg);
                return *this;
            }
            
            /// YANG keyword corresponding to the Statement
            std::string keyword;
            /// the arg if any
            std::string  arg;
            
        };
        
        ///
        /// @brief Represents a Node in the SchemaTree.
        ///
        class SchemaNode
        {
            
        public:
            ///
            /// @brief The destructor.
            ///
            /// Note a SchemaNode represents a containment hierarchy. So
            /// invocation of the destructor will lead to the children of this
            /// node being destroyed.
            virtual ~SchemaNode() {};
            
            ///
            /// @brief returns the XPath expression of this Node in the NodeTree
            ///
            /// Get the path expression representing this Node in in the NodeTree.
            /// @return std::string representing the path to this Node.
            ///
            virtual std::string path() const = 0;
            
            ///
            /// @brief finds descendant nodes that match the given xpath expression
            ///
            /// This API finds descendant nodes in the Schema tree that satisfy
            /// the given path expression. See @see howtopath
            /// @param path The path expression.
            /// @return vector of SchemaNode  that satisfies the criterion.
            /// @throws YDKPathException if the path expression in invalid, See error code for details.
            /// @throws YDKInvalidArgumentException if the argument is invalid.
            ///
            virtual std::vector<SchemaNode*> find(const std::string& path) const = 0;
            
            ///
            /// @brief get the Parent Node of this SchemaNode in the tree.
            ///
            /// Returns the parent Node of this SchemaNode in the tree
            /// @return pointer the parent Node or nullptr in case this is the root.
            ///
            virtual const SchemaNode* parent() const noexcept = 0 ;
            
            ///
            /// @brief return the children of this SchemaNode in the NodeTree.
            ///
            /// Returns the children of this SchemaNode.
            ///@return the children of this node.
            virtual std::vector<SchemaNode*> children() const = 0;
            
            ///
            /// @brief get the root of NodeTree this node is part of
            ///
            /// Returns the root of the NodeTree this nodes is part of
            /// @return the pointer to the root
            ///
            virtual const SchemaNode* root() const noexcept = 0;
            
            ///
            /// @brief return the YANG statement associated with this SchemaNode
            ///
            /// Returns the YANG statement associated with this SchemaNode
            /// @return the yang statement for this SchemaNode
            ///
            virtual Statement statement() const = 0;
            
        };
        
        ///
        /// @brief The RootSchemeNode.
        ///
        /// Instances of this class represent the Root of the SchemaTree.
        /// A RootSchemaNode can be used to instantiate a DataNodeTree or an RPC object
        /// The children of the RootSchemaNode represent the top level SchemaNodes
        /// in the YANG module submodules.
        ///
        class RootSchemaNode : public SchemaNode
        {
        public:
            ///
            /// @brief Destructor for the RootSchemaNode
            ///
            virtual ~RootSchemaNode() {};
            
            ///
            /// @brief returns the XPath expression of this Node in the NodeTree
            ///
            /// Get the path expression representing this Node in in the NodeTree.
            /// @return std::string representing the path to this Node.
            ///
            std::string path() const { return "/"; }
            
            ///
            /// @brief finds descendant nodes that match the given xpath expression
            ///
            /// This API finds descendant nodes in the Schema tree that satisfy
            /// the given path expression. See @see howtopath
            /// @param path The path expression.
            /// @return vector of SchemaNode  that satisfies the criterion.
            /// @throws YDKPathException if the path expression in invalid, See error code for details.
            /// @throws YDKInvalidArgumentException if the argument is invalid.
            ///
            virtual std::vector<SchemaNode*> find(const std::string& path) const = 0;
            
            ///
            /// @brief get the Parent Node of this SchemaNode in the tree.
            ///
            /// Returns the parent Node of this SchemaNode in the tree
            /// @return pointer the parent Node or nullptr in case this is the root.
            ///
            virtual SchemaNode* parent() const noexcept { return nullptr;}
            
            ///
            /// @brief return the children of this SchemaNode in the NodeTree.
            ///
            /// Returns the children of this SchemaNode.
            ///@return the children of this node.
            virtual std::vector<SchemaNode*> children() const = 0;
            
            ///
            /// @brief get the root of NodeTree this node is part of
            ///
            /// Returns the root of the NodeTree this nodes is part of
            /// @return the pointer to the root
            ///
            virtual const SchemaNode* root() const noexcept { return this; }
            
            ///
            /// @brief create a DataNode corresponding to the path and set its value
            ///
            /// This methods creates a DataNode tree based on the path passed in. The path
            /// expression must identify a single node. If the last node created is of schema
            /// type list, leaf-list or anyxml that value is also set in the node.
            /// The returned DataNode is the last node created (the terminal part of the path).
            /// The user is responsible for managing the memory of this returned tree. Use DataNode#root()
            /// to get the root element of the this tree and use that pointer to dispose of the entire tree.
            /// Note in the case of List nodes the keys must be present in the path expression in the form
            /// of predicates.
            ///
            /// @param[in] path The XPath expression identifying the node relative to the root
            /// of the schema tree
            /// @param[in] value The string representation of the value to set.
            /// @return Pointer to DataNode created.
            /// @throws YDKInvalidArgumentException In case the argument is invalid.
            /// @throws YDKPathException In case the path is invalid.
            ///
            virtual DataNode* create(const std::string& path, const std::string& value) const = 0;
            
            ///
            /// @brief create a DataNode corresponding to the path and set its value
            ///
            /// This methods creates a DataNode tree based on the path passed in. The path
            /// expression must identify a single node. If the last node created is of schema
            /// type list, leaf-list or anyxml that value is also set in the node.
            /// The returned DataNode is the last node created (the terminal part of the path).
            /// The user is responsible for managing the memory of this returned tree. Use DataNode#root()
            /// to get the root element of the this tree and use that pointer to dispose of the entire tree.
            /// Note in the case of List nodes the keys must be present in the path expression in the form
            /// of predicates.
            ///
            /// @param[in] path The XPath expression identifying the node.
            /// @return DataNode created or nullptr
            /// @throws YDKInvalidArgumentException In case the argument is invalid.
            /// @throws YDKPathException In case the path is invalid.
            ///
            virtual DataNode* create(const std::string& path) const  = 0;
            
            ///
            /// @brief return the Statement representing this SchemaNode
            ///
            /// Note the RootSchemaNode has no YANG statement representing it.
            /// So this method returns an empty statement.
            /// @return an empty statement
            ///
            virtual Statement statement() const { return Statement{}; }
            //virtual DataNode* from_xml(const std::string& xml) const = 0;
            
            ///
            /// @brief create an rpc instance
            ///
            /// The path expression should point to a SchemaNode that represents the Rpc
            /// @param[in] path The path to the rpc schema node
            /// @return rpc or nullptr
            /// @throws YDKInvalidArgumentException if the argument is invalid.
            /// @throws YDKPathException if the path is invalid
            ///
            virtual Rpc* rpc(const std::string& path) const = 0;
            
        };
        
        ///
        /// @brief DataNode
        ///
        /// Class represents the DataNode
        class DataNode{
            
        public:
            ///
            /// @brief The destructor.
            ///
            /// Note a DataNode represents a containment hierarchy. So
            /// invocation of the destructor will lead to the children of this
            /// node being destroyed.
            virtual ~DataNode() {};
            
            ///
            /// @brief Return the SchemaNode associated with this DataNode.
            ///
            /// Return the SchemaNode associated with this DataNode.
            /// @return SchemaNode associated with this DataNode
            ///
            virtual const SchemaNode* schema() const = 0;
            
            ///
            /// @brief returns the XPath expression of this Node in the NodeTree
            ///
            /// Get the path expression representing this Node in in the NodeTree.
            /// @return std::string representing the path to this Node.
            ///
            virtual std::string path() const = 0;
            
            ///
            /// @brief create a DataNode corresponding to the path and set its value
            ///
            /// This methods creates a DataNode tree based on the path passed in. The path
            /// expression must identify a single node. If the last node created is of schema
            /// type list, leaf-list or anyxml that value is also set in the node.
            /// The returned DataNode is the last node created (the terminal part of the path).
            /// The user is responsible for managing the memory of this returned tree. Use DataNode#root()
            /// to get the root element of the this tree and use that pointer to dispose of the entire tree.
            /// Note in the case of List nodes the keys must be present in the path expression in the form
            /// of predicates.
            ///
            /// @param[in] path The XPath expression identifying the node.
            /// @param[in] value The string representation of the value to set.
            /// @return Pointer to DataNode created.
            /// @throws YDKInvalidArgumentException In case the argument is invalid.
            /// @throws YDKPathException In case the path is invalid.
            ///
            virtual DataNode* create(const std::string& path)
            {
                return create(path, "");
            }
            
            ///
            /// @brief create a DataNode corresponding to the path and set its value
            ///
            /// This methods creates a DataNode tree based on the path passed in. The path
            /// expression must identify a single node. If the last node created is of schema
            /// type list, leaf-list or anyxml that value is also set in the node.
            /// The returned DataNode is the last node created (the terminal part of the path).
            /// The user is responsible for managing the memory of this returned tree. Use DataNode#root()
            /// to get the root element of the this tree and use that pointer to dispose of the entire tree.
            /// Note in the case of List nodes the keys must be present in the path expression in the form
            /// of predicates.
            ///
            /// @param[in] path The XPath expression identifying the node.
            /// @return Pointer to DataNode created.
            /// @throws YDKInvalidArgumentException In case the argument is invalid.
            /// @throws YDKPathException In case the path is invalid.
            ///
            virtual DataNode* create(const std::string& path, const std::string& value) = 0;
            
            
            virtual void set(const std::string& value) = 0;
            virtual std::string get() const = 0;
            virtual std::vector<DataNode*> find(const std::string& path) const = 0 ;
            virtual DataNode* parent() const = 0;
            virtual std::vector<DataNode*> children() const = 0;
            virtual const DataNode* root() const = 0;
            //virtual std::string xml() const = 0;
        };
        
        struct Capability {
            Capability(const std::string& mod, const std::string& rev) : module{mod}, revision{rev}
            {
                
            }
            std::string module;
            std::string revision;
            std::vector<std::string> features;
            std::vector<std::string> deviations;
        };
        
        class Repository {
        public:
            Repository(const std::string& search_dir);
            
            RootSchemaNode* create_root_schema(const std::vector<Capability> capabilities);
            
            
        private:
            std::string m_search_dir;
        };
        
        
        
        class ServiceProvider
        {
        public:
            virtual RootSchemaNode* get_root_schema() = 0;
            virtual ~ServiceProvider() {};
            virtual DataNode* invoke(Rpc* rpc) const = 0 ;
            
        };
        
        
        class Rpc
        {
        public:
            
            virtual ~Rpc() {};
            
            virtual DataNode* operator()(const ServiceProvider& provider) = 0;
            
            
            virtual DataNode* input() const = 0;
            
            virtual SchemaNode* schema() const = 0;
            
            //std::string xml() const;
            
        };
        
    }
    
}

#endif /* YDK_CORE_HPP */
