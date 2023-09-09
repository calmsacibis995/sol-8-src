#
# Copyright (c) 1998-1999 by Sun Microsystems, Inc.
# All rights reserved.
#
#pragma ident	"@(#)FN.spec	1.2	99/05/14 SMI"
#
# lib/fn/libxfn/spec/FN.spec

function	fn_attribute_create
include		<xfn/xfn.h>
declaration	FN_attribute_t *fn_attribute_create( \
			const FN_identifier_t *attribute_id, \
			const FN_identifier_t *attribute_syntax)
version		SUNW_1.1
end		

function	fn_attribute_destroy
include		<xfn/xfn.h>
declaration	void fn_attribute_destroy(FN_attribute_t *attr)
version		SUNW_1.1
end		

function	fn_attribute_copy
include		<xfn/xfn.h>
declaration	FN_attribute_t *fn_attribute_copy(const FN_attribute_t *attr)
version		SUNW_1.1
exception	
end		

function	fn_attribute_assign
include		<xfn/xfn.h>
declaration	FN_attribute_t *fn_attribute_assign(FN_attribute_t *dst, \
			const	FN_attribute_t *src)
version		SUNW_1.1
end		

function	fn_attribute_identifier
include		<xfn/xfn.h>
declaration	const FN_identifier_t *fn_attribute_identifier( \
			const FN_attribute_t *attr)
version		SUNW_1.1
end		

function	fn_attribute_syntax
include		<xfn/xfn.h>
declaration	const FN_identifier_t *fn_attribute_syntax( \
			const FN_attribute_t *attr)
version		SUNW_1.1
end		

function	fn_attribute_valuecount
include		<xfn/xfn.h>
declaration	unsigned int fn_attribute_valuecount( \
			const FN_attribute_t *attr)
version		SUNW_1.1
end		

function	fn_attribute_first
include		<xfn/xfn.h>
declaration	const FN_attrvalue_t *fn_attribute_first( \
			const FN_attribute_t *attr, void **iter_pos)
version		SUNW_1.1
end		

function	fn_attribute_next
include		<xfn/xfn.h>
declaration	const FN_attrvalue_t *fn_attribute_next( \
			const FN_attribute_t *attr, void **iter_pos)
version		SUNW_1.1
end		

function	fn_attribute_add
include		<xfn/xfn.h>
declaration	int fn_attribute_add(FN_attribute_t *attr, \
			const FN_attrvalue_t *attribute_value, \
			unsigned int exclusive)
version		SUNW_1.1
end		

function	fn_attribute_remove
include		<xfn/xfn.h>
declaration	int fn_attribute_remove(FN_attribute_t *attr, \
			const FN_attrvalue_t *attribute_value)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_attrmodlist_create
include		<xfn/xfn.h>
declaration	FN_attrmodlist_t *fn_attrmodlist_create(void)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attrmodlist_destroy
include		<xfn/xfn.h>
declaration	void fn_attrmodlist_destroy(FN_attrmodlist_t *modlist)
version		SUNW_1.1
end		

function	fn_attrmodlist_copy
include		<xfn/xfn.h>
declaration	FN_attrmodlist_t *fn_attrmodlist_copy( \
			const FN_attrmodlist_t *modlist)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attrmodlist_assign
include		<xfn/xfn.h>
declaration	FN_attrmodlist_t *fn_attrmodlist_assign( \
			FN_attrmodlist_t *dst, \
			const FN_attrmodlist_t *src)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attrmodlist_count
include		<xfn/xfn.h>
declaration	unsigned int fn_attrmodlist_count( \
			const FN_attrmodlist_t *modlist)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_attrmodlist_first
include		<xfn/xfn.h>
declaration	const FN_attribute_t *fn_attrmodlist_first( \
			const FN_attrmodlist_t *modlist, \
			void **iter_pos, unsigned int *first_mod_op)
version		SUNW_1.1
end		

function	fn_attrmodlist_next
include		<xfn/xfn.h>
declaration	const FN_attribute_t *fn_attrmodlist_next( \
			const FN_attrmodlist_t *modlist, \
			void **iter_pos, unsigned int *mod_op)
version		SUNW_1.1
end		

function	fn_attrmodlist_add
include		<xfn/xfn.h>
declaration	int fn_attrmodlist_add(FN_attrmodlist_t *modlist, \
			unsigned int mod_op, const FN_attribute_t *attr)
version		SUNW_1.1
end		

function	fn_attrset_create
include		<xfn/xfn.h>
declaration	FN_attrset_t *fn_attrset_create(void)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attrset_remove
include		<xfn/xfn.h>
declaration	int fn_attrset_remove(FN_attrset_t *aset, \
			const	FN_identifier_t	*attr_id)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_attrset_destroy
include		<xfn/xfn.h>
declaration	void fn_attrset_destroy(FN_attrset_t *aset)
version		SUNW_1.1
end		

function	fn_attrset_copy
include		<xfn/xfn.h>
declaration	FN_attrset_t *fn_attrset_copy(const FN_attrset_t *aset)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attrset_assign
include		<xfn/xfn.h>
declaration	FN_attrset_t *fn_attrset_assign(FN_attrset_t *dst, \
			const FN_attrset_t *src)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attrset_get
include		<xfn/xfn.h>
declaration	const FN_attribute_t *fn_attrset_get( \
			const FN_attrset_t *aset, \
			const FN_identifier_t *attr_id)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attrset_count
include		<xfn/xfn.h>
declaration	unsigned int fn_attrset_count(const FN_attrset_t *aset)
version		SUNW_1.1
end		

function	fn_attrset_first
include		<xfn/xfn.h>
declaration	const FN_attribute_t *fn_attrset_first( \
			const FN_attrset_t *aset, void **iter_pos)
version		SUNW_1.1
end		

function	fn_attrset_next
include		<xfn/xfn.h>
declaration	const FN_attribute_t *fn_attrset_next( \
			const FN_attrset_t *aset, void **iter_pos)
version		SUNW_1.1
end		

function	fn_attrset_add
include		<xfn/xfn.h>
declaration	int fn_attrset_add(FN_attrset_t *aset, \
			const FN_attribute_t *attr, unsigned int exclusive)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_composite_name_create
include		<xfn/xfn.h>
declaration	FN_composite_name_t *fn_composite_name_create(void)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_composite_name_first
include		<xfn/xfn.h>
declaration	const FN_string_t *fn_composite_name_first( \
			const FN_composite_name_t *name, void **iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_composite_name_next
include		<xfn/xfn.h>
declaration	const FN_string_t *fn_composite_name_next( \
			const FN_composite_name_t *name, void **iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_composite_name_prev
include		<xfn/xfn.h>
declaration	const FN_string_t *fn_composite_name_prev( \
			const FN_composite_name_t *name, void **iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_composite_name_last
include		<xfn/xfn.h>
declaration	const FN_string_t *fn_composite_name_last( \
			const FN_composite_name_t *name, void **iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_composite_name_prefix
include		<xfn/xfn.h>
declaration	FN_composite_name_t *fn_composite_name_prefix( \
			const FN_composite_name_t *name, const void *iter_pos)
version		SUNW_1.1
end		

function	fn_composite_name_suffix
include		<xfn/xfn.h>
declaration	FN_composite_name_t *fn_composite_name_suffix( \
			const FN_composite_name_t *name, \
			const void *iter_pos)
version		SUNW_1.1
end		

function	fn_composite_name_is_equal
include		<xfn/xfn.h>
declaration	int fn_composite_name_is_equal( \
			const FN_composite_name_t *name, \
			const FN_composite_name_t *name2, \
			unsigned int *status)
version		SUNW_1.1
end		

function	fn_composite_name_is_prefix
include		<xfn/xfn.h>
declaration	int fn_composite_name_is_prefix( \
			const FN_composite_name_t *name, \
			const FN_composite_name_t *prefix, \
			void **iter_pos, unsigned int *status)
version		SUNW_1.1
end		

function	fn_composite_name_is_suffix
include		<xfn/xfn.h>
declaration	int fn_composite_name_is_suffix( \
			const FN_composite_name_t *name, \
			const FN_composite_name_t *suffix, \
			void **iter_pos, unsigned int *status)
version		SUNW_1.1
end		

function	fn_composite_name_prepend_comp
include		<xfn/xfn.h>
declaration	int fn_composite_name_prepend_comp( \
			FN_composite_name_t *name, \
			const FN_string_t *newcomp)
version		SUNW_1.1
end		

function	fn_composite_name_destroy
include		<xfn/xfn.h>
declaration	void fn_composite_name_destroy(FN_composite_name_t *name)
version		SUNW_1.1
end		

function	fn_composite_name_append_comp
include		<xfn/xfn.h>
declaration	int fn_composite_name_append_comp( \
			FN_composite_name_t *name, \
			const FN_string_t *newcomp)
version		SUNW_1.1
end		

function	fn_composite_name_insert_comp
include		<xfn/xfn.h>
declaration	int fn_composite_name_insert_comp( \
			FN_composite_name_t *name, \
			void **iter_pos, const FN_string_t *newcomp)
version		SUNW_1.1
end		

function	fn_composite_name_delete_comp
include		<xfn/xfn.h>
declaration	int fn_composite_name_delete_comp( \
			FN_composite_name_t *name, \
			void **iter_pos)
version		SUNW_1.1
end		

function	fn_composite_name_prepend_name
include		<xfn/xfn.h>
declaration	int fn_composite_name_prepend_name( \
			FN_composite_name_t *name, \
			const FN_composite_name_t *newcomps)
version		SUNW_1.1
end		

function	fn_composite_name_append_name
include		<xfn/xfn.h>
declaration	int fn_composite_name_append_name( \
			FN_composite_name_t *name, \
			const FN_composite_name_t *newcomps)
version		SUNW_1.1
end		

function	fn_composite_name_insert_name
include		<xfn/xfn.h>
declaration	int fn_composite_name_insert_name( \
			FN_composite_name_t *name, \
			void **iter_pos, \
			const FN_composite_name_t *newcomps)
version		SUNW_1.1
end		

function	fn_composite_name_from_str
include		<xfn/xfn.h>
declaration	FN_composite_name_t *fn_composite_name_from_str( \
			const unsigned char *cstr)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_composite_name_copy
include		<xfn/xfn.h>
declaration	FN_composite_name_t *fn_composite_name_copy( \
			const FN_composite_name_t *name)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_composite_name_assign
include		<xfn/xfn.h>
declaration	FN_composite_name_t *fn_composite_name_assign( \
			FN_composite_name_t *dst, \
			const	FN_composite_name_t *src)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_composite_name_is_empty
include		<xfn/xfn.h>
declaration	int fn_composite_name_is_empty( \
			const FN_composite_name_t *name)
version		SUNW_1.1
end		

function	fn_composite_name_count
include		<xfn/xfn.h>
declaration	unsigned int fn_composite_name_count( \
			const FN_composite_name_t *name)
version		SUNW_1.1
end		

function	fn_compound_name_from_syntax_attrs
include		<xfn/xfn.h>
declaration	FN_compound_name_t *fn_compound_name_from_syntax_attrs( \
			const FN_attrset_t *aset, \
			const FN_string_t *name, \
			FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_prev
include		<xfn/xfn.h>
declaration	const FN_string_t *fn_compound_name_prev( \
			const FN_compound_name_t *name, void **iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_last
include		<xfn/xfn.h>
declaration	const FN_string_t *fn_compound_name_last( \
			const FN_compound_name_t *name, void **iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_prefix
include		<xfn/xfn.h>
declaration	FN_compound_name_t *fn_compound_name_prefix( \
			const FN_compound_name_t *name, \
			const void *iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_suffix
include		<xfn/xfn.h>
declaration	FN_compound_name_t *fn_compound_name_suffix( \
			const FN_compound_name_t *name, \
			const void *iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_is_empty
include		<xfn/xfn.h>
declaration	int fn_compound_name_is_empty( \
			const FN_compound_name_t *name)
version		SUNW_1.1
end		

function	fn_compound_name_is_equal
include		<xfn/xfn.h>
declaration	int fn_compound_name_is_equal( \
			const FN_compound_name_t *name1, \
			const FN_compound_name_t *name2, \
			unsigned int *status)
version		SUNW_1.1
end		

function	fn_compound_name_is_prefix
include		<xfn/xfn.h>
declaration	int fn_compound_name_is_prefix( \
			const FN_compound_name_t *name, \
			const FN_compound_name_t *pre, \
			void **iter_pos, unsigned int *status)
version		SUNW_1.1
end		

function	fn_compound_name_is_suffix
include		<xfn/xfn.h>
declaration	int fn_compound_name_is_suffix( \
			const FN_compound_name_t *name, \
			const FN_compound_name_t *suffix, \
			void **iter_pos, unsigned int *status)
version		SUNW_1.1
end		

function	fn_compound_name_prepend_comp
include		<xfn/xfn.h>
declaration	int fn_compound_name_prepend_comp( \
			FN_compound_name_t *name, \
			const FN_string_t *atomic_comp, \
			unsigned int *status)
version		SUNW_1.1
end		

function	fn_compound_name_append_comp
include		<xfn/xfn.h>
declaration	int fn_compound_name_append_comp( \
			FN_compound_name_t *name, \
			const FN_string_t *atomic_comp, \
			unsigned int *status)
version		SUNW_1.1
end		

function	fn_compound_name_get_syntax_attrs
include		<xfn/xfn.h>
declaration	FN_attrset_t *fn_compound_name_get_syntax_attrs( \
			const FN_compound_name_t *name)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_insert_comp
include		<xfn/xfn.h>
declaration	int fn_compound_name_insert_comp( \
			FN_compound_name_t *name, \
			void **iter_pos, \
			const FN_string_t *atomic_comp, \
			unsigned int *status)
version		SUNW_1.1
end		

function	fn_compound_name_delete_comp
include		<xfn/xfn.h>
declaration	int fn_compound_name_delete_comp( \
			FN_compound_name_t *name, void **iter_pos)
version		SUNW_1.1
end		

function	fn_compound_name_delete_all
include		<xfn/xfn.h>
declaration	int fn_compound_name_delete_all(FN_compound_name_t *name)
version		SUNW_1.1
end		

function	fn_compound_name_destroy
include		<xfn/xfn.h>
declaration	void fn_compound_name_destroy(FN_compound_name_t *name)
version		SUNW_1.1
end		

function	fn_string_from_compound_name
include		<xfn/xfn.h>
declaration	FN_string_t *fn_string_from_compound_name( \
			const FN_compound_name_t *name)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_copy
include		<xfn/xfn.h>
declaration	FN_compound_name_t *fn_compound_name_copy( \
			const FN_compound_name_t *name)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_assign
include		<xfn/xfn.h>
declaration	FN_compound_name_t *fn_compound_name_assign( \
			FN_compound_name_t *dst, \
			const FN_compound_name_t *src)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_count
include		<xfn/xfn.h>
declaration	unsigned int fn_compound_name_count( \
			const FN_compound_name_t *name)
version		SUNW_1.1
end		

function	fn_compound_name_first
include		<xfn/xfn.h>
declaration	const FN_string_t *fn_compound_name_first( \
			const FN_compound_name_t *name, void **iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_compound_name_next
include		<xfn/xfn.h>
declaration	const FN_string_t *fn_compound_name_next( \
			const FN_compound_name_t *name, \
			void **iter_pos)
version		SUNW_1.1
exception	$return == NULL
end		

