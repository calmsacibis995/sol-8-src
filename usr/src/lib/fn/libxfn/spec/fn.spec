#
# Copyright (c) 1998-1999 by Sun Microsystems, Inc.
# All rights reserved.
#
#pragma ident	"@(#)fn.spec	1.2	99/05/14 SMI"
#
# lib/fn/libxfn/spec/fn.spec

function	fn_attr_get
include		<xfn/xfn.h>
declaration	FN_attribute_t *fn_attr_get(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			const FN_identifier_t *attribute_id, \
			unsigned int follow_link, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attr_get_ids
include		<xfn/xfn.h>
declaration	FN_attrset_t *fn_attr_get_ids(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			unsigned int follow_link, FN_status_t *status);
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attr_get_values
include		<xfn/xfn.h>
declaration	FN_valuelist_t *fn_attr_get_values(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			const FN_identifier_t *attribute_id, \
			unsigned int follow_link, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_valuelist_next
include		<xfn/xfn.h>
declaration	FN_attrvalue_t *fn_valuelist_next(FN_valuelist_t *vl, \
			FN_identifier_t **attr_syntax, FN_status_t *status)
version		SUNW_1.1
exception	
end		

function	fn_valuelist_destroy
include		<xfn/xfn.h>
declaration	void fn_valuelist_destroy(FN_valuelist_t *vl)
version		SUNW_1.1
end		

function	fn_attr_modify
include		<xfn/xfn.h>
declaration	int fn_attr_modify(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			unsigned int mod_op, const FN_attribute_t *attr, \
			unsigned int follow_link, FN_status_t *status)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_attr_multi_get
include		<xfn/xfn.h>
declaration	FN_multigetlist_t *fn_attr_multi_get(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			const FN_attrset_t *attr_ids, \
			unsigned int follow_link, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_multigetlist_next
include		<xfn/xfn.h>
declaration	FN_attribute_t *fn_multigetlist_next(FN_multigetlist_t *ml, \
			FN_status_t *status)
version		SUNW_1.1
exception	
end		

function	fn_multigetlist_destroy
include		<xfn/xfn.h>
declaration	void fn_multigetlist_destroy(FN_multigetlist_t *ml)
version		SUNW_1.1
end		

function	fn_attr_multi_modify
include		<xfn/xfn.h>
declaration	int fn_attr_multi_modify(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			const FN_attrmodlist_t *mods, \
			unsigned int follow_link, \
			FN_attrmodlist_t **unexecuted_mods, \
			FN_status_t *status)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_ctx_bind
include		<xfn/xfn.h>
declaration	int fn_ctx_bind(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			const FN_ref_t *ref, unsigned int exclusive, \
			FN_status_t *status)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_ctx_create_subcontext
include		<xfn/xfn.h>
declaration	FN_ref_t *fn_ctx_create_subcontext(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_ctx_destroy_subcontext
include		<xfn/xfn.h>
declaration	int fn_ctx_destroy_subcontext(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, FN_status_t *status)
version		SUNW_1.1
exception	
end		

function	fn_ctx_get_ref
include		<xfn/xfn.h>
declaration	FN_ref_t *fn_ctx_get_ref(const FN_ctx_t *ctx, \
			FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_ctx_get_syntax_attrs
include		<xfn/xfn.h>
declaration	FN_attrset_t *fn_ctx_get_syntax_attrs(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_ctx_handle_destroy
include		<xfn/xfn.h>
declaration	void fn_ctx_handle_destroy(FN_ctx_t *ctx)
version		SUNW_1.1
end		

function	fn_ctx_handle_from_initial
include		<xfn/xfn.h>
declaration	FN_ctx_t *fn_ctx_handle_from_initial( \
			unsigned int authoritative, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_ctx_handle_from_ref
include		<xfn/xfn.h>
declaration	FN_ctx_t *fn_ctx_handle_from_ref(const FN_ref_t *ref, \
			 unsigned int authoritative, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_ctx_list_bindings
include		<xfn/xfn.h>
declaration	FN_bindinglist_t *fn_ctx_list_bindings(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_bindinglist_next
include		<xfn/xfn.h>
declaration	FN_string_t *fn_bindinglist_next(FN_bindinglist_t *bl, \
			FN_ref_t **ref, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_bindinglist_destroy
include		<xfn/xfn.h>
declaration	void fn_bindinglist_destroy(FN_bindinglist_t *bl)
version		SUNW_1.1
end		

function	fn_ctx_list_names
include		<xfn/xfn.h>
declaration	FN_namelist_t *fn_ctx_list_names(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, FN_status_t *status)
version		SUNW_1.1
exception	
end		

function	fn_namelist_next
include		<xfn/xfn.h>
declaration	FN_string_t *fn_namelist_next(FN_namelist_t *nl, \
			FN_status_t *status)
version		SUNW_1.1
exception	
end		

function	fn_namelist_destroy
include		<xfn/xfn.h>
declaration	void fn_namelist_destroy(FN_namelist_t *nl)
version		SUNW_1.1
end		

function	fn_ctx_lookup
include		<xfn/xfn.h>
declaration	FN_ref_t *fn_ctx_lookup(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_ctx_rename
include		<xfn/xfn.h>
declaration	int fn_ctx_rename(FN_ctx_t *ctx, \
			const FN_composite_name_t *oldname, \
			const FN_composite_name_t *newname, \
			unsigned int exclusive, FN_status_t *status)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_ctx_unbind
include		<xfn/xfn.h>
declaration	int fn_ctx_unbind(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, FN_status_t *status)
version		SUNW_1.1
end		

function	fn_status_create
include		<xfn/xfn.h>
declaration	FN_status_t *fn_status_create(void)
version		SUNW_1.1
exception	$return == NULL
end		

function	fn_attr_bind
declaration	int fn_attr_bind(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			const FN_ref_t *ref, \
			const FN_attrset_t *attrs, \
			unsigned int exclusive, \
			FN_status_t *status)
version		SUNW_1.1
exception	$return == 0
end		

function	fn_attr_create_subcontext
declaration	FN_ref_t *fn_attr_create_subcontext(FN_ctx_t *ctx, \
			const FN_composite_name_t *name, \
			const FN_attrset_t *attrs, FN_status_t *status)
version		SUNW_1.1
exception	$return == NULL
end		

