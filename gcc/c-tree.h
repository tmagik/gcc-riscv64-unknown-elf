/* Definitions for C parsing and type checking.
   Copyright (C) 1987, 93, 94, 95, 97, 98, 1999 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _C_TREE_H
#define _C_TREE_H

#include "c-common.h"

/* Language-dependent contents of an identifier.  */

/* The limbo_value is used for block level extern declarations, which need
   to be type checked against subsequent extern declarations.  They can't
   be referenced after they fall out of scope, so they can't be global.  */

struct lang_identifier
{
  struct tree_identifier ignore;
  tree global_value, local_value, label_value, implicit_decl;
  tree error_locus, limbo_value;
};

/* Macros for access to language-specific slots in an identifier.  */
/* Each of these slots contains a DECL node or null.  */

/* This represents the value which the identifier has in the
   file-scope namespace.  */
#define IDENTIFIER_GLOBAL_VALUE(NODE)	\
  (((struct lang_identifier *) (NODE))->global_value)
/* This represents the value which the identifier has in the current
   scope.  */
#define IDENTIFIER_LOCAL_VALUE(NODE)	\
  (((struct lang_identifier *) (NODE))->local_value)
/* This represents the value which the identifier has as a label in
   the current label scope.  */
#define IDENTIFIER_LABEL_VALUE(NODE)	\
  (((struct lang_identifier *) (NODE))->label_value)
/* This records the extern decl of this identifier, if it has had one
   at any point in this compilation.  */
#define IDENTIFIER_LIMBO_VALUE(NODE)	\
  (((struct lang_identifier *) (NODE))->limbo_value)
/* This records the implicit function decl of this identifier, if it
   has had one at any point in this compilation.  */
#define IDENTIFIER_IMPLICIT_DECL(NODE)	\
  (((struct lang_identifier *) (NODE))->implicit_decl)
/* This is the last function in which we printed an "undefined variable"
   message for this identifier.  Value is a FUNCTION_DECL or null.  */
#define IDENTIFIER_ERROR_LOCUS(NODE)	\
  (((struct lang_identifier *) (NODE))->error_locus)

/* In identifiers, C uses the following fields in a special way:
   TREE_PUBLIC        to record that there was a previous local extern decl.
   TREE_USED          to record that such a decl was used.
   TREE_ADDRESSABLE   to record that the address of such a decl was used.  */

/* Nonzero means reject anything that ANSI standard C forbids.  */
extern int pedantic;

/* In a RECORD_TYPE or UNION_TYPE, nonzero if any component is read-only.  */
#define C_TYPE_FIELDS_READONLY(type) TREE_LANG_FLAG_1 (type)

/* In a RECORD_TYPE or UNION_TYPE, nonzero if any component is volatile.  */
#define C_TYPE_FIELDS_VOLATILE(type) TREE_LANG_FLAG_2 (type)

/* In a RECORD_TYPE or UNION_TYPE or ENUMERAL_TYPE
   nonzero if the definition of the type has already started.  */
#define C_TYPE_BEING_DEFINED(type) TYPE_LANG_FLAG_0 (type)

/* C types are partitioned into three subsets: object, function, and
   incomplete types.  */
#define C_TYPE_OBJECT_P(type) \
  (TREE_CODE (type) != FUNCTION_TYPE && TYPE_SIZE (type))

#define C_TYPE_FUNCTION_P(type) \
  (TREE_CODE (type) == FUNCTION_TYPE)

#define C_TYPE_INCOMPLETE_P(type) \
  (TREE_CODE (type) != FUNCTION_TYPE && TYPE_SIZE (type) == 0)

/* For convenience we define a single macro to identify the class of
   object or incomplete types.  */
#define C_TYPE_OBJECT_OR_INCOMPLETE_P(type) \
  (!C_TYPE_FUNCTION_P (type))

/* In a RECORD_TYPE, a sorted array of the fields of the type.  */
struct lang_type
{
  int len;
  tree elts[1];
};

/* Mark which labels are explicitly declared.
   These may be shadowed, and may be referenced from nested functions.  */
#define C_DECLARED_LABEL_FLAG(label) TREE_LANG_FLAG_1 (label)

/* Record whether a type or decl was written with nonconstant size.
   Note that TYPE_SIZE may have simplified to a constant.  */
#define C_TYPE_VARIABLE_SIZE(type) TYPE_LANG_FLAG_1 (type)
#define C_DECL_VARIABLE_SIZE(type) DECL_LANG_FLAG_0 (type)

/* Record in each node resulting from a binary operator
   what operator was specified for it.  */
#define C_EXP_ORIGINAL_CODE(exp) ((enum tree_code) TREE_COMPLEXITY (exp))

#if 0 /* Not used.  */
/* Record whether a decl for a function or function pointer has
   already been mentioned (in a warning) because it was called
   but didn't have a prototype.  */
#define C_MISSING_PROTOTYPE_WARNED(decl) DECL_LANG_FLAG_2(decl)
#endif

/* Store a value in that field.  */
#define C_SET_EXP_ORIGINAL_CODE(exp, code) \
  (TREE_COMPLEXITY (exp) = (int) (code))

/* Record whether a typedef for type `int' was actually `signed int'.  */
#define C_TYPEDEF_EXPLICITLY_SIGNED(exp) DECL_LANG_FLAG_1 ((exp))

/* Nonzero for a declaration of a built in function if there has been no
   occasion that would declare the function in ordinary C.
   Using the function draws a pedantic warning in this case.  */
#define C_DECL_ANTICIPATED(exp) DECL_LANG_FLAG_3 ((exp))

/* For FUNCTION_TYPE, a hidden list of types of arguments.  The same as
   TYPE_ARG_TYPES for functions with prototypes, but created for functions
   without prototypes.  */
#define TYPE_ACTUAL_ARG_TYPES(NODE) TYPE_NONCOPIED_PARTS (NODE)

/* In a FIELD_DECL, nonzero if the decl was originally a bitfield.  */
#define DECL_C_BIT_FIELD(NODE) DECL_LANG_FLAG_4 (NODE)

/* Nonzero if the type T promotes to itself.
   ANSI C states explicitly the list of types that promote;
   in particular, short promotes to int even if they have the same width.  */
#define C_PROMOTING_INTEGER_TYPE_P(t)				\
  (TREE_CODE ((t)) == INTEGER_TYPE				\
   && (TYPE_MAIN_VARIANT (t) == char_type_node			\
       || TYPE_MAIN_VARIANT (t) == signed_char_type_node	\
       || TYPE_MAIN_VARIANT (t) == unsigned_char_type_node	\
       || TYPE_MAIN_VARIANT (t) == short_integer_type_node	\
       || TYPE_MAIN_VARIANT (t) == short_unsigned_type_node))

/* In a VAR_DECL, means the variable is really an iterator.  */
#define ITERATOR_P(D) (DECL_LANG_FLAG_4(D))

/* In a VAR_DECL for an iterator, means we are within
   an explicit loop over that iterator.  */
#define ITERATOR_BOUND_P(NODE) ((NODE)->common.readonly_flag)

/* in c-lang.c and objc-act.c */
extern tree lookup_interface			PROTO((tree));
extern tree is_class_name			PROTO((tree));
extern void maybe_objc_check_decl		PROTO((tree));
extern void finish_file				PROTO((void));
extern int maybe_objc_comptypes                 PROTO((tree, tree, int));
extern tree maybe_building_objc_message_expr    PROTO((void));
extern tree maybe_objc_method_name		PROTO((tree));
extern int recognize_objc_keyword		PROTO((void));
extern tree build_objc_string			PROTO((int, const char *));

/* in c-aux-info.c */
extern void gen_aux_info_record                 PROTO((tree, int, int, int));

/* in c-convert.c */
extern tree convert                             PROTO((tree, tree));

/* in c-decl.c */
extern tree build_enumerator                    PROTO((tree, tree));
/* Add qualifiers to a type, in the fashion for C.  */
extern tree c_build_qualified_type              PROTO((tree, int));
#define c_build_type_variant(TYPE, CONST_P, VOLATILE_P)		  \
  c_build_qualified_type (TYPE, 				  \
			  ((CONST_P) ? TYPE_QUAL_CONST : 0) |	  \
			  ((VOLATILE_P) ? TYPE_QUAL_VOLATILE : 0))
extern int  c_decode_option                     PROTO((int, char **));
extern void c_mark_varargs                      PROTO((void));
extern tree check_identifier                    PROTO((tree, tree));
extern void clear_parm_order                    PROTO((void));
extern tree combine_parm_decls                  PROTO((tree, tree, int));
extern int  complete_array_type                 PROTO((tree, tree, int));
extern void declare_parm_level                  PROTO((int));
extern tree define_label                        PROTO((char *, int, tree));
extern void delete_block                        PROTO((tree));
extern void finish_decl                         PROTO((tree, tree, tree));
extern void finish_decl_top_level               PROTO((tree, tree, tree));
extern tree finish_enum                         PROTO((tree, tree, tree));
extern void finish_function                     PROTO((int));
extern tree finish_struct                       PROTO((tree, tree, tree));
extern tree get_parm_info                       PROTO((int));
extern tree getdecls                            PROTO((void));
extern tree gettags                             PROTO((void));
extern int  global_bindings_p                   PROTO((void));
extern tree grokfield                           PROTO((const char *, int, tree, tree, tree));
extern tree groktypename                        PROTO((tree));
extern tree groktypename_in_parm_context        PROTO((tree));
extern tree implicitly_declare                  PROTO((tree));
extern int  in_parm_level_p                     PROTO((void));
extern void init_decl_processing                PROTO((void));
extern void insert_block                        PROTO((tree));
extern void keep_next_level                     PROTO((void));
extern int  kept_level_p                        PROTO((void));
extern tree lookup_label                        PROTO((tree));
extern tree lookup_name                         PROTO((tree));
extern tree lookup_name_current_level		PROTO((tree));
extern tree lookup_name_current_level_global	PROTO((tree));
extern tree maybe_build_cleanup                 PROTO((tree));
extern void parmlist_tags_warning               PROTO((void));
extern void pending_xref_error                  PROTO((void));
extern void pop_c_function_context              PROTO((void));
extern void pop_label_level                     PROTO((void));
extern tree poplevel                            PROTO((int, int, int));
extern void print_lang_decl                     PROTO((FILE *, tree, int));
extern void print_lang_identifier               PROTO((FILE *, tree, int));
extern void print_lang_type                     PROTO((FILE *, tree, int));
extern void push_c_function_context             PROTO((void));
extern void push_label_level                    PROTO((void));
extern void push_parm_decl                      PROTO((tree));
extern tree pushdecl                            PROTO((tree));
extern tree pushdecl_top_level                  PROTO((tree));
extern void pushlevel                           PROTO((int));
extern void pushtag                             PROTO((tree, tree));
extern void set_block                           PROTO((tree));
extern tree shadow_label                        PROTO((tree));
extern void shadow_record_fields                PROTO((tree));
extern void shadow_tag                          PROTO((tree));
extern void shadow_tag_warned                   PROTO((tree, int));
extern tree start_enum                          PROTO((tree));
extern int  start_function                      PROTO((tree, tree, tree,
						       tree, int));
extern tree start_decl                          PROTO((tree, tree, int,
						       tree, tree));
extern tree start_struct                        PROTO((enum tree_code, tree));
extern void store_parm_decls                    PROTO((void));
extern tree xref_tag                            PROTO((enum tree_code, tree));

/* in c-typeck.c */
extern tree require_complete_type		PROTO((tree));
extern void incomplete_type_error		PROTO((tree, tree));
/* Given two integer or real types, return the type for their sum.
   Given two compatible ANSI C types, returns the merged type.  */
extern tree common_type                         PROTO((tree, tree));
extern int comptypes				PROTO((tree, tree));
extern int self_promoting_args_p		PROTO((tree));
extern tree c_sizeof                            PROTO((tree));
extern tree c_sizeof_nowarn                     PROTO((tree));
extern tree c_size_in_bytes                     PROTO((tree));
extern tree c_alignof				PROTO((tree));
extern tree c_alignof_expr			PROTO((tree));
extern tree default_conversion                  PROTO((tree));
extern tree build_component_ref                 PROTO((tree, tree));
extern tree build_indirect_ref                  PROTO((tree, const char *));
extern tree build_array_ref                     PROTO((tree, tree));
extern tree build_function_call                 PROTO((tree, tree));
extern tree parser_build_binary_op              PROTO((enum tree_code,
						       tree, tree));
extern tree build_binary_op                     PROTO((enum tree_code,
						       tree, tree, int));
extern tree build_unary_op                      PROTO((enum tree_code,
						       tree, int));
extern int lvalue_p				PROTO((tree));
extern int lvalue_or_else			PROTO((tree, const char *));
extern void readonly_warning			PROTO((tree, const char *));
extern int mark_addressable			PROTO((tree));
extern tree build_conditional_expr              PROTO((tree, tree, tree));
extern tree build_compound_expr                 PROTO((tree));
extern tree build_c_cast                        PROTO((tree, tree));
extern tree build_modify_expr                   PROTO((tree, enum tree_code,
						       tree));
extern void store_init_value                    PROTO((tree, tree));
extern void error_init				PROTO((const char *));
extern void pedwarn_init			PROTO((const char *));
extern void start_init				PROTO((tree, tree, int));
extern void finish_init				PROTO((void));
extern void really_start_incremental_init	PROTO((tree));
extern void push_init_level			PROTO((int));
extern tree pop_init_level			PROTO((int));
extern void set_init_index			PROTO((tree, tree));
extern void set_init_label			PROTO((tree));
extern void process_init_element		PROTO((tree));
extern void c_expand_asm_operands		PROTO((tree, tree, tree, tree,
						       int, char *, int));
extern void c_expand_return			PROTO((tree));
extern tree c_expand_start_case                 PROTO((tree));

/* in c-iterate.c */
extern void init_iterators			PROTO((void));
extern void iterator_expand			PROTO((tree));
extern void iterator_for_loop_start		PROTO((tree));
extern void iterator_for_loop_end		PROTO((tree));
extern void iterator_for_loop_record		PROTO((tree));
extern void push_iterator_stack			PROTO((void));
extern void pop_iterator_stack			PROTO((void));

/* Set to 0 at beginning of a function definition, set to 1 if
   a return statement that specifies a return value is seen.  */

extern int current_function_returns_value;

/* Set to 0 at beginning of a function definition, set to 1 if
   a return statement with no argument is seen.  */

extern int current_function_returns_null;

/* Nonzero means the expression being parsed will never be evaluated.
   This is a count, since unevaluated expressions can nest.  */

extern int skip_evaluation;

/* Nonzero means `$' can be in an identifier.  */

extern int dollars_in_ident;

/* Nonzero means allow type mismatches in conditional expressions;
   just make their values `void'.   */

extern int flag_cond_mismatch;

/* Nonzero means don't recognize the keyword `asm'.  */

extern int flag_no_asm;

/* Nonzero means environment is hosted (i.e., not freestanding) */

extern int flag_hosted;

/* Nonzero means warn about implicit declarations.  */

extern int warn_implicit;

/* Nonzero means give string constants the type `const char *'
   to get extra warnings from them.  These warnings will be too numerous
   to be useful, except in thoroughly ANSIfied programs.  */

extern int flag_const_strings;

/* Nonzero means warn about sizeof (function) or addition/subtraction
   of function pointers.  */

extern int warn_pointer_arith;

/* Nonzero means warn for all old-style non-prototype function decls.  */

extern int warn_strict_prototypes;

/* Nonzero means warn about multiple (redundant) decls for the same single
   variable or function.  */

extern int warn_redundant_decls;

/* Nonzero means warn about extern declarations of objects not at
   file-scope level and about *all* declarations of functions (whether
   extern or static) not at file-scope level.  Note that we exclude
   implicit function declarations.  To get warnings about those, use
   -Wimplicit.  */

extern int warn_nested_externs;

/* Nonzero means warn about pointer casts that can drop a type qualifier
   from the pointer target type.  */

extern int warn_cast_qual;

/* Nonzero means warn when casting a function call to a type that does
   not match the return type (e.g. (float)sqrt() or (anything*)malloc()
   when there is no previous declaration of sqrt or malloc.  */

extern int warn_bad_function_cast;

/* Warn about functions which might be candidates for attribute noreturn. */

extern int warn_missing_noreturn;

/* Warn about traditional constructs whose meanings changed in ANSI C.  */

extern int warn_traditional;

/* Warn about *printf or *scanf format/argument anomalies. */

extern int warn_format;

/* Warn about a subscript that has type char.  */

extern int warn_char_subscripts;

/* Warn if a type conversion is done that might have confusing results.  */

extern int warn_conversion;

/* Warn if main is suspicious. */

extern int warn_main;

/* Nonzero means do some things the same way PCC does.  */

extern int flag_traditional;

/* Nonzero means use the ISO C9x dialect of C.  */

extern int flag_isoc9x;

/* Nonzero means to allow single precision math even if we're generally
   being traditional. */
extern int flag_allow_single_precision;

/* Nonzero means warn about suggesting putting in ()'s.  */

extern int warn_parentheses;

/* Warn if initializer is not completely bracketed.  */

extern int warn_missing_braces;

/* Warn about comparison of signed and unsigned values.  */

extern int warn_sign_compare;

/* Warn about multicharacter constants.  */

extern int warn_multichar;

/* Warn about long long.  */

extern int warn_long_long;

/* Nonzero means we are reading code that came from a system header file.  */

extern int system_header_p;

/* Nonzero enables objc features.  */

extern int doing_objc_thang;

/* In c-decl.c */
extern void finish_incomplete_decl PROTO((tree));

#endif /* not _C_TREE_H */
