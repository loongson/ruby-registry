/* -*- coding: utf-8; mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
  Copyright (C) 2009-2021  Sutou Kouhei <kou@clear-code.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "rb-grn.h"

#define SELF(object) ((RbGrnAccessor *)RTYPEDDATA_DATA(object))

VALUE rb_cGrnAccessor;

grn_obj *
rb_grn_accessor_from_ruby_object (VALUE object)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(object, rb_cGrnAccessor))) {
        rb_raise(rb_eTypeError, "not a groonga accessor");
    }

    return RVAL2GRNOBJECT(object, NULL);
}

VALUE
rb_grn_accessor_to_ruby_object (grn_ctx *context, grn_obj *table,
                                grn_bool owner)
{
    return GRNOBJECT2RVAL(rb_cGrnAccessor, context, table, owner);
}

void
rb_grn_accessor_bind (RbGrnAccessor *rb_accessor,
                      grn_ctx *context, grn_obj *accessor)
{
    RbGrnObject *rb_grn_object;
    RbGrnNamedObject *rb_grn_named_object;
    char name[GRN_TABLE_MAX_KEY_SIZE];
    int name_size;

    rb_grn_object = RB_GRN_OBJECT(rb_accessor);
    rb_grn_named_object = RB_GRN_NAMED_OBJECT(rb_accessor);
    rb_grn_named_object_bind(rb_grn_named_object, context, accessor);

    name_size = grn_column_name(context, accessor, name, sizeof(name));
    if (name_size > 0) {
        rb_grn_named_object_set_name(rb_grn_named_object, name, name_size);
    }

    rb_accessor->value = grn_obj_open(context, GRN_BULK, 0,
                                      rb_grn_object->range_id);
}


void
rb_grn_accessor_finalizer (grn_ctx *context, grn_obj *grn_object,
                           RbGrnAccessor *rb_accessor)
{
    rb_grn_context_unregister_floating_object(RB_GRN_OBJECT(rb_accessor));
    rb_grn_named_object_finalizer(context, grn_object,
                                  RB_GRN_NAMED_OBJECT(rb_accessor));
    if (context && rb_accessor->value)
        grn_obj_unlink(context, rb_accessor->value);
    rb_accessor->value = NULL;
}

/*
 * ???????????????????????????
 *
 * @overload local_name
 *
 * @example
 *   items = Groonga::Array.create(:name => "Items")
 *   id = items.column("_id")
 *   id.name # => nil
 *   id.local_name # => "_id"
 */
static VALUE
rb_grn_accessor_get_local_name (VALUE self)
{
    RbGrnAccessor *rb_grn_accessor;
    RbGrnNamedObject *rb_grn_named_object;

    rb_grn_accessor = SELF(self);
    rb_grn_named_object = RB_GRN_NAMED_OBJECT(rb_grn_accessor);
    if (rb_grn_named_object->name_size == 0)
        return Qnil;

    return rb_str_new(rb_grn_named_object->name,
                      rb_grn_named_object->name_size);
}

/*
 * Document-class: Groonga::Accessor < Groonga::Object
 *
 * ????????????????????????????????????????????????????????????????????????????????????
 * ????????????????????? {Groonga::Table#column} ?????????????????????
 */
void
rb_grn_init_accessor (VALUE mGrn)
{
    rb_cGrnAccessor = rb_define_class_under(mGrn, "Accessor", rb_cGrnObject);

    rb_define_method(rb_cGrnAccessor, "local_name",
                     rb_grn_accessor_get_local_name, 0);
}
