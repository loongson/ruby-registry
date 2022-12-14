/* -*- coding: utf-8; mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set sts=4 sw=4 ts=8 noet: */
/*
  Copyright (C) 2009-2022  Sutou Kouhei <kou@clear-code.com>
  Copyright (C) 2016  Masafumi Yokoyama <yokoyama@clear-code.com>

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

#define SELF(object) ((RbGrnColumn *)RTYPEDDATA_DATA(object))

VALUE rb_cGrnColumn;

/*
 * Document-class: Groonga::Column < Groonga::Object
 *
 * テーブルに情報を付加するためのオブジェクト。テーブルに複
 * 数のカラムを定義することによりレコード毎に複数の情報を付
 * 加することができる。
 *
 * カラムには大きく分けて3種類ある。
 *
 * - {Groonga::FixSizeColumn} :=
 *   固定長のデータを格納するカラム。 =:
 * - {Groonga::VariableSizeColumn} :=
 *   可変長のデータを格納するカラム。 =:
 * - {Groonga::IndexColumn} :=
 *   転置インデックスを格納するカラム。全文検索や参照元レコー
 *   ドの検索を行う場合はこのカラムを使用する。 =:
 *
 * 固定長データ用カラム・可変長データ用カラムは1つのデータだ
 * けを格納するか複数のデータを格納するかを選ぶことができる。
 * 1つのデータの場合はスカラ値、複数のデータの場合はスカラー
 * 値を格納するという。
 *
 * カラムは名前を持ち、1つのテーブルでは同じカラム名を持つカ
 * ラムを複数定義することはできない。
 */

grn_obj *
rb_grn_column_from_ruby_object (VALUE object, grn_ctx **context)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(object, rb_cGrnColumn))) {
        rb_raise(rb_eTypeError, "not a Groonga column");
    }

    return RVAL2GRNOBJECT(object, context);
}

VALUE
rb_grn_column_to_ruby_object (VALUE klass, grn_ctx *context, grn_obj *column,
                              grn_bool owner)
{
    return GRNOBJECT2RVAL(klass, context, column, owner);
}

void
rb_grn_column_bind (RbGrnColumn *rb_column,
                    grn_ctx *context, grn_obj *column)
{
    RbGrnObject *rb_grn_object;
    grn_column_flags flags;
    int column_type;
    unsigned char value_type;

    rb_grn_object = RB_GRN_OBJECT(rb_column);
    rb_grn_named_object_bind(RB_GRN_NAMED_OBJECT(rb_column), context, column);

    flags = grn_column_get_flags(context, column);
    column_type = (flags & GRN_OBJ_COLUMN_TYPE_MASK);
    if (column_type == GRN_OBJ_COLUMN_VECTOR) {
        switch (rb_grn_object->range->header.type) {
        case GRN_TABLE_HASH_KEY:
        case GRN_TABLE_PAT_KEY:
        case GRN_TABLE_DAT_KEY:
        case GRN_TABLE_NO_KEY:
            value_type = GRN_UVECTOR;
            break;
        default:
            value_type = GRN_VECTOR;
            break;
        }
    } else {
        value_type = GRN_BULK;
    }
    rb_column->value = grn_obj_open(context, value_type, 0,
                                    rb_grn_object->range_id);
}

void
rb_grn_column_finalizer (grn_ctx *context, grn_obj *grn_object,
                         RbGrnColumn *rb_column)
{
    rb_grn_named_object_finalizer(context, grn_object,
                                  RB_GRN_NAMED_OBJECT(rb_column));
    if (context && rb_column->value)
        grn_obj_unlink(context, rb_column->value);
    rb_column->value = NULL;
}

void
rb_grn_column_deconstruct (RbGrnColumn *rb_column,
                           grn_obj **column,
                           grn_ctx **context,
                           grn_id *domain_id,
                           grn_obj **domain,
                           grn_obj **value,
                           grn_id *range_id,
                           grn_obj **range)
{
    RbGrnObject *rb_grn_object;

    rb_grn_object = RB_GRN_OBJECT(rb_column);
    rb_grn_object_deconstruct(rb_grn_object, column, context,
                              domain_id, domain,
                              range_id, range);

    if (value)
        *value = rb_column->value;
}

/*
 * カラムが所属するテーブルを返す。
 *
 * @overload table
 *   @return [Groonga::Table]
 */
static VALUE
rb_grn_column_get_table (VALUE self)
{
    grn_ctx *context = NULL;
    grn_obj *column;
    grn_obj *table;

    rb_grn_object_deconstruct((RbGrnObject *)(SELF(self)), &column, &context,
                              NULL, NULL,
                              NULL, NULL);
    table = grn_column_table(context, column);
    rb_grn_context_check(context, self);

    return GRNOBJECT2RVAL(Qnil, context, table, GRN_FALSE);
}

/*
 * テーブル名を除いたカラム名を返す。
 * @example
 *   items = Groonga::Array.create(:name => "Items")
 *   title = items.define_column("title", "ShortText")
 *   title.name # => "Items.title"
 *   title.local_name # => "title"
 *
 * @overload local_name
 */
static VALUE
rb_grn_column_get_local_name (VALUE self)
{
    RbGrnColumn *rb_grn_column;
    grn_ctx *context = NULL;
    grn_obj *column;
    VALUE rb_name;
    char *name;
    int name_size;

    rb_grn_column = SELF(self);
    rb_grn_object_deconstruct(RB_GRN_OBJECT(rb_grn_column), &column, &context,
                              NULL, NULL,
                              NULL, NULL);
    name_size = grn_column_name(context, column, NULL, 0);
    if (name_size == 0)
        return Qnil;

    name = xmalloc(name_size);
    grn_column_name(context, column, name, name_size);
    rb_name = rb_str_new(name, name_size);
    xfree(name);

    return rb_name;
}

 /*
 * カラムが所属するテーブルからブロックまたは文字列で指定し
 * た条件にマッチするレコードを返す。返されたテーブルには
 * +expression+ という特異メソッドがあり、指定した条件を表し
 * ている {Groonga::Expression} を取得できる。
 * {Groonga::Expression#snippet} を使うことにより、指定した条件
 * 用のスニペットを簡単に生成できる。
 *
 * @example Shows texts around "Groonga"
 *   results = description_column.select do |column|
 *     column =~ "Groonga"
 *   end
 *   snippet = results.expression.snippet([["<em>", "</em>"]])
 *   results.each do |record|
 *     puts "#{record['name']}の説明文の中で「Groonga」を含む部分"
 *     snippet.execute(record["description"]).each do |snippet|
 *       puts "---"
 *       puts "#{snippet}..."
 *       puts "---"
 *     end
 *   end
 *
 *   # Sample output:
 *   #
 *   # Rroongaの説明文の中で「Groonga」を含む部分
 *   # ---
 *   # Rroongaは<em>Groonga</em>のいわゆるDB-APIの層の...
 *   # ---
 *
 * @return [Groonga::Hash] 検索結果
 * @overload select(options)
 *   @yieldparam [Groonga::Record] record
 *     検索条件を指定するのに、 _record_ に対して次のメソッドを使って検索条
 *     件を指定できます。
 *
 *     @==@ 、 @=~@ 、 @<@ 、 @<=@ 、 @>@ 、 @>=@ 、 @+@ 、 @-@ 、
 *     @*@ 、 @/@ 、 @%@ 、
 *     @match(query, :syntax => :query/:script, &block)@
 *     （ _query_ に指定した
 *     "grn_expr":http://groonga.org/ja/docs/reference/grn_expr.html
 *     とのマッチ）、
 *     @similar_search(text)@ （ _text_ との類似文書探索）、
 *     @term_extract(text)@ （ _text_ 内から _record_ の単語を抽出）
 *
 *   @!macro [new] column.select.options
 *     @param [::Hash] options The name and value
 *       pairs. Omitted names are initialized as the default value.
 *     @option options :operator (Groonga::Operator::OR)
 *       マッチしたレコードをどのように扱うか。指定可能な値は以
 *       下の通り。
 *
 *       - Groonga::Operator::OR :=
 *         マッチしたレコードを追加。すでにレコードが追加され
 *         ている場合は何もしない。 =:
 *       - Groonga::Operator::AND :=
 *         マッチしたレコードのスコアを増加。マッチしなかった
 *         レコードを削除。 =:
 *       - Groonga::Operator::AND_NOT :=
 *         マッチしたレコードを削除。 =:
 *       - Groonga::Operator::ADJUST :=
 *         マッチしたレコードのスコアを増加。 =:
 *
 *     @option options :result
 *       検索結果を格納するテーブル。マッチしたレコードが追加さ
 *       れていく。省略した場合は新しくテーブルを作成して返す。
 *
 *     @option options :name
 *       条件の名前。省略した場合は名前を付けない。
 *
 *     @option options :syntax (:query)
 *       _query_ の構文。
 *
 *       参考: {Groonga::Expression#parse} .
 *
 *     @option options :allow_pragma
 *       query構文時にプラグマを利用するかどうか。省略した場合は
 *       利用する。
 *
 *       参考: {Groonga::Expression#parse} .
 *
 *     @option options :allow_column The allow_column
 *       query構文時にカラム指定を利用するかどうか。省略した場合
 *       は利用する。
 *
 *       参考: {Groonga::Expression#parse} .
 *
 *     @option options :allow_update
 *       script構文時に更新操作を利用するかどうか。省略した場合
 *       は利用する。
 *
 *       参考: {Groonga::Expression#parse} .
 *
 *     @option options :allow_leading_not
 *       指定語句の先頭に-を付けることで指定した語句を含まないレコードの
 *       検索を有効にするかどうかを指定する。省略した場合は無効にする。
 *
 *       参考: {Groonga::Expression#parse} .
 *
 *   @!macro column.select.options
 * @overload select(query, options)
 *   @param [String] query 条件の指定
 *     _query_ には「[カラム名]:[演算子][値]」という書式で条件を
 *     指定する。演算子は以下の通り。
 *
 *     - なし := [カラム値] == [値]
 *     - @!@ := [カラム値] != [値]
 *     - @<@ := [カラム値] < [値]
 *     - @>@ := [カラム値] > [値]
 *     - @<=@ := [カラム値] <= [値]
 *     - @>=@ := [カラム値] >= [値]
 *     - @@@ := [カラム値]が[値]を含んでいるかどうか
 *
 *     例:
 *
 *     - @"groonga"@ := _column_ カラムの値が @"groonga"@ のレコードにマッチ
 *     - @"name:daijiro"@ :=
 *       _column_ カラムが属しているテーブルの @"name"@ カラムの値が
 *       @"daijiro"@ のレコードにマッチ  =:
 *     - @"description:@@@groonga"@ :=
 *       _column_ カラムが属しているテーブルの @"description"@ カラムが
 *       @"groonga"@ を含んでいるレコードにマッチ =:
 *
 *   @!macro column.select.options
 * @overload select(expression, options)
 *   @param [Groonga::Expression] expression 条件を表すオブジェクト
 *     _expression_ には既に作成済みの {Groonga::Expression} を渡す。
 *   @!macro column.select.options
 */
static VALUE
rb_grn_column_select (int argc, VALUE *argv, VALUE self)
{
    grn_ctx *context;
    grn_obj *table, *column, *result, *expression;
    grn_operator operator = GRN_OP_OR;
    VALUE options;
    VALUE rb_query, condition_or_options;
    VALUE rb_name, rb_operator, rb_result, rb_syntax;
    VALUE rb_allow_pragma, rb_allow_column, rb_allow_update, rb_allow_leading_not;
    VALUE builder;
    VALUE rb_expression = Qnil;

    rb_query = Qnil;

    rb_scan_args(argc, argv, "02", &condition_or_options, &options);

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                              NULL, NULL,
                              NULL, NULL, NULL);
    table = grn_column_table(context, column);

    if (RVAL2CBOOL(rb_obj_is_kind_of(condition_or_options, rb_cString))) {
        rb_query = condition_or_options;
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(condition_or_options,
                                            rb_cGrnExpression))) {
        rb_expression = condition_or_options;
    } else {
        if (!NIL_P(options))
            rb_raise(rb_eArgError,
                     "should be [query_string, option_hash], "
                     "[expression, option_hash] "
                     "or [option_hash]: %s",
                     rb_grn_inspect(rb_ary_new_from_values(argc, argv)));
        options = condition_or_options;
    }

    rb_grn_scan_options(options,
                        "operator", &rb_operator,
                        "result", &rb_result,
                        "name", &rb_name,
                        "syntax", &rb_syntax,
                        "allow_pragma", &rb_allow_pragma,
                        "allow_column", &rb_allow_column,
                        "allow_update", &rb_allow_update,
                        "allow_leading_not", &rb_allow_leading_not,
                        NULL);

    if (!NIL_P(rb_operator))
        operator = NUM2INT(rb_operator);

    if (NIL_P(rb_result)) {
        result = grn_table_create(context, NULL, 0, NULL,
                                  GRN_TABLE_HASH_KEY | GRN_OBJ_WITH_SUBREC,
                                  table,
                                  0);
        rb_result = GRNTABLE2RVAL(context, result, GRN_TRUE);
    } else {
        result = RVAL2GRNTABLE(rb_result, &context);
    }

    if (NIL_P(rb_expression)) {
      builder = rb_grn_column_expression_builder_new(self, rb_name, rb_query);
      rb_funcall(builder, rb_intern("syntax="), 1, rb_syntax);
      rb_funcall(builder, rb_intern("allow_pragma="), 1, rb_allow_pragma);
      rb_funcall(builder, rb_intern("allow_column="), 1, rb_allow_column);
      rb_funcall(builder, rb_intern("allow_update="), 1, rb_allow_update);
      rb_funcall(builder, rb_intern("allow_leading_not="), 1, rb_allow_leading_not);
      rb_expression = rb_grn_column_expression_builder_build(builder);
    }
    rb_grn_object_deconstruct(RB_GRN_OBJECT(RTYPEDDATA_DATA(rb_expression)),
                              &expression, NULL,
                              NULL, NULL, NULL, NULL);

    grn_table_select(context, table, expression, result, operator);
    rb_grn_context_check(context, self);

    rb_attr(rb_singleton_class(rb_result),
            rb_intern("expression"),
            GRN_TRUE, GRN_FALSE, GRN_FALSE);
    rb_iv_set(rb_result, "@expression", rb_expression);

    return rb_result;
}

/*
 * _column_ のロックを解除する。
 * @overload unlock(options={})
 *   @param [::Hash] options 利用可能なオプションは以下の通り。
 *   @option options :id
 *     _:id_ で指定したレコードのロックを解除する。
 *     （注: groonga側が未実装のため、現在は無視される）
 */
static VALUE
rb_grn_column_unlock (int argc, VALUE *argv, VALUE self)
{
    grn_id id = GRN_ID_NIL;
    grn_ctx *context;
    grn_obj *column;
    grn_rc rc;
    VALUE options, rb_id;

    rb_scan_args(argc, argv, "01",  &options);

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    rb_grn_scan_options(options,
                        "id", &rb_id,
                        NULL);

    if (!NIL_P(rb_id))
        id = NUM2UINT(rb_id);

    rc = grn_obj_unlock(context, column, id);
    rb_grn_context_check(context, self);
    rb_grn_rc_check(rc, self);

    return Qnil;
}

static VALUE
rb_grn_column_unlock_ensure (VALUE self)
{
    return rb_grn_column_unlock(0, NULL, self);
}

/*
 * _column_ をロックする。ロックに失敗した場合は
 * {Groonga::ResourceDeadlockAvoided} 例外が発生する。
 *
 * @overload lock(options={})
 *   @!macro [new] column.lock.options
 *     @param [::Hash] options 利用可能なオプションは以下の通り。
 *     @option options :timeout
 *       ロックを獲得できなかった場合は _:timeout_ 秒間ロックの獲
 *       得を試みる。 _:timeout_ 秒以内にロックを獲得できなかった
 *       場合は例外が発生する。
 *     @option options :id
 *       _:id_で指定したレコードをロックする。（注: groonga側が
 *       未実装のため、現在は無視される）
 *   @!macro column.lock.options
 * @overload lock(options={})
 *   @yield ブロックを指定した場合はブロックを抜けたときにunlockする。
 *   @!macro column.lock.options
 */
static VALUE
rb_grn_column_lock (int argc, VALUE *argv, VALUE self)
{
    grn_id id = GRN_ID_NIL;
    grn_ctx *context;
    grn_obj *column;
    int timeout = 0;
    grn_rc rc;
    VALUE options, rb_timeout, rb_id;

    rb_scan_args(argc, argv, "01",  &options);

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    rb_grn_scan_options(options,
                        "timeout", &rb_timeout,
                        "id", &rb_id,
                        NULL);

    if (!NIL_P(rb_timeout))
        timeout = NUM2UINT(rb_timeout);

    if (!NIL_P(rb_id))
        id = NUM2UINT(rb_id);

    rc = grn_obj_lock(context, column, id, timeout);
    rb_grn_context_check(context, self);
    rb_grn_rc_check(rc, self);

    if (rb_block_given_p()) {
        return rb_ensure(rb_yield, Qnil, rb_grn_column_unlock_ensure, self);
    } else {
        return Qnil;
    }
}

/*
 * Forces to clear lock of the `column`.
 *
 * @overload clear_lock
 *   @return [void]
 */
static VALUE
rb_grn_column_clear_lock (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    grn_obj_clear_lock(context, column);

    return Qnil;
}

/*
 * Checks whether the `column` is locked or not.
 *
 * @overload locked?
 *   @return [Boolean] `true` if the `column` is locked, `false` otherwise.
 */
static VALUE
rb_grn_column_locked_p (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    return CBOOL2RVAL(grn_obj_is_locked(context, column));
}

/*
 * _column_ の値がテーブルのレコードとなる場合は +true+ を返し、
 * そうでない場合は +false+ を返す。
 *
 * @since 1.0.5
 *
 * @overload reference?
 */
static VALUE
rb_grn_column_reference_p (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;
    grn_id range_id;
    grn_obj *range;
    unsigned short int type;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    range_id = grn_obj_get_range(context, column);
    range = grn_ctx_at(context, range_id);
    type = range->header.type;
    grn_obj_unlink(context, range);
    switch (type) {
      case GRN_TABLE_HASH_KEY:
      case GRN_TABLE_PAT_KEY:
      case GRN_TABLE_NO_KEY:
        return Qtrue;
      default:
        return Qfalse;
    }
}

/*
 * Clears all values in column.
 *
 * This method is a dangerous method. You can't use this method when
 * other process is using the column. The other process that is using
 * the column must re-open its database after this method is done.
 *
 * @since 4.0.8
 *
 * @overload truncate
 *   @return void
 */
static VALUE
rb_grn_column_truncate (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;
    grn_rc rc;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                              NULL, NULL,
                              NULL, NULL, NULL);
    rc = grn_column_truncate(context, column);

    rb_grn_context_check(context, self);
    rb_grn_rc_check(rc, self);

    return Qnil;
}

/*
 * @overload index?
 *   @return [Bool] `true` if the column is an index column
 *     ({Groonga::IndexColumn}), `false` otherwise.
 *
 * @since 1.0.5
 */
static VALUE
rb_grn_column_index_p (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    if (column->header.type == GRN_COLUMN_INDEX) {
        return Qtrue;
    } else {
        return Qfalse;
    }
}

/*
 * @overload vector?
 *   @return [Bool] `true` if the column is a vector column,
 *     `false` otherwise.
 *
 * @since 1.0.5
 */
static VALUE
rb_grn_column_vector_p (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    return CBOOL2RVAL(grn_obj_is_vector_column(context, column));
}

/*
 * @overload weight_vector?
 *   @return [Bool] `true` if the column is a weight vector column,
 *     `false` otherwise.
 *
 * @since 7.0.2
 */
static VALUE
rb_grn_column_weight_vector_p (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    return CBOOL2RVAL(grn_obj_is_weight_vector_column(context, column));
}

/*
 * @overload scalar?
 *   @return [Bool] `true` if the column is a scalar column,
 *     `false` otherwise.
 *
 * @since 1.0.5
 */
static VALUE
rb_grn_column_scalar_p (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    return CBOOL2RVAL(grn_obj_is_scalar_column(context, column));
}

/*
 * @overload data?
 *   @return [Bool] `true` if the column is a data column (a scalar
 *     column or a vector column), `false` otherwise.
 *
 * @since 7.0.2
 */
static VALUE
rb_grn_column_data_p (VALUE self)
{
    grn_ctx *context;
    grn_obj *column;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                             NULL, NULL,
                             NULL, NULL, NULL);

    return CBOOL2RVAL(grn_obj_is_data_column(context, column));
}

/*
 * @overload with_weight?
 * @return [Boolean] @true@ if the column is created with
 *   @:with_weight => true@ flag, @false@ otherwise.
 * @since 4.0.1
 */
static VALUE
rb_grn_column_with_weight_p(VALUE self)
{
    grn_obj *column;
    grn_ctx *context;
    grn_column_flags flags;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                              NULL, NULL,
                              NULL, NULL, NULL);

    flags = grn_column_get_flags(context, column);
    return CBOOL2RVAL(flags & GRN_OBJ_WITH_WEIGHT);
}

/*
 * @overload weight_float32?
 * @return [Boolean] @true@ if the column uses 32 bit float for weight.
 * @since 12.0.2
 */
static VALUE
rb_grn_column_weight_float32_p(VALUE self)
{
    grn_obj *column;
    grn_ctx *context;
    grn_column_flags flags;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                              NULL, NULL,
                              NULL, NULL, NULL);

    flags = grn_column_get_flags(context, column);
    return CBOOL2RVAL(flags & GRN_OBJ_WEIGHT_FLOAT32);
}

/*
 * Return indexes on `column`. If operator is specified, indexes that
 * can executes the operator are only returned. Otherwise, all indexes
 * are returned.
 *
 * Index means that index column and section.
 *
 * @overload find_indexes(options={})
 *   @param options [::Hash] The name and value pairs.
 *      Omitted names are initialized as the default value.
 *   @option options :operator (nil) The operator that should be
 *      executable by indexes. `nil` means that all operators.
 *   @return [Array<Groonga::Index>] Target indexes on `column`.
 *
 * @since 6.0.0
 */
static VALUE
rb_grn_column_find_indexes (int argc, VALUE *argv, VALUE self)
{
    VALUE rb_options;
    VALUE rb_operator;
    grn_ctx *context;
    grn_obj *column;
    grn_index_datum *index_data = NULL;
    int i, n_indexes;
    VALUE rb_indexes;

    rb_scan_args(argc, argv, "01", &rb_options);
    rb_grn_scan_options(rb_options,
                        "operator", &rb_operator,
                        NULL);

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                              NULL, NULL,
                              NULL, NULL, NULL);

    if (NIL_P(rb_operator)) {
        n_indexes = grn_column_get_all_index_data(context, column, NULL, 0);
        if (n_indexes == 0)
            return rb_ary_new();

        index_data = xmalloc(sizeof(grn_index_datum) * n_indexes);
        n_indexes = grn_column_get_all_index_data(context, column,
                                                  index_data, n_indexes);
    } else {
        grn_operator operator;
        operator = RVAL2GRNOPERATOR(rb_operator);
        n_indexes = grn_column_find_index_data(context, column, operator,
                                               NULL, 0);
        if (n_indexes == 0)
            return rb_ary_new();

        index_data = xmalloc(sizeof(grn_index_datum) * n_indexes);
        n_indexes = grn_column_find_index_data(context, column, operator,
                                               index_data, n_indexes);
    }

    rb_indexes = rb_ary_new_capa(n_indexes);
    for (i = 0; i < n_indexes; i++) {
        VALUE rb_index_column;
        VALUE rb_section;
        rb_index_column = GRNOBJECT2RVAL(Qnil,
                                         context,
                                         index_data[i].index,
                                         GRN_FALSE);
        rb_section = UINT2NUM(index_data[i].section);
        rb_ary_push(rb_indexes, rb_grn_index_new(rb_index_column, rb_section));
        grn_obj_unlink(context, index_data[i].index);
    }
    xfree(index_data);
    return rb_indexes;
}

/*
 * Renames the column to name.
 * @since 1.3.0
 * @overload rename(name)
 *   @param name [String] the new name
 */
static VALUE
rb_grn_column_rename (VALUE self, VALUE rb_name)
{
    int rc;
    grn_ctx *context;
    grn_obj *column;
    char *name;
    int name_size;

    rb_grn_column_deconstruct(SELF(self), &column, &context,
                              NULL, NULL,
                              NULL, NULL, NULL);

    name = StringValueCStr(rb_name);
    name_size = RSTRING_LEN(rb_name);

    rc = grn_column_rename(context, column, name, name_size);
    rb_grn_context_check(context, self);
    rb_grn_rc_check(rc, self);
    rb_grn_named_object_set_name(RB_GRN_NAMED_OBJECT(RTYPEDDATA_DATA(self)),
                                 name, name_size);
    return self;
}

void
rb_grn_init_column (VALUE mGrn)
{
    rb_cGrnColumn = rb_define_class_under(mGrn, "Column", rb_cGrnObject);

    rb_include_module(rb_cGrnColumn, rb_mGrnFlushable);

    rb_define_method(rb_cGrnColumn, "table", rb_grn_column_get_table, 0);
    rb_define_method(rb_cGrnColumn, "local_name",
                     rb_grn_column_get_local_name, 0);

    rb_define_method(rb_cGrnColumn, "select", rb_grn_column_select, -1);
    rb_define_method(rb_cGrnColumn, "lock", rb_grn_column_lock, -1);
    rb_define_method(rb_cGrnColumn, "unlock", rb_grn_column_unlock, -1);
    rb_define_method(rb_cGrnColumn, "clear_lock", rb_grn_column_clear_lock, 0);
    rb_define_method(rb_cGrnColumn, "locked?", rb_grn_column_locked_p, 0);
    rb_define_method(rb_cGrnColumn, "reference?", rb_grn_column_reference_p, 0);
    rb_define_method(rb_cGrnColumn, "truncate", rb_grn_column_truncate, 0);
    /* deprecated: backward compatibility */
    rb_define_alias(rb_cGrnColumn, "reference_column?", "reference?");
    rb_define_method(rb_cGrnColumn, "index?", rb_grn_column_index_p, 0);
    /* deprecated: backward compatibility */
    rb_define_alias(rb_cGrnColumn, "index_column?", "index?");
    rb_define_method(rb_cGrnColumn, "vector?", rb_grn_column_vector_p, 0);
    rb_define_method(rb_cGrnColumn, "weight_vector?",
                     rb_grn_column_weight_vector_p, 0);
    rb_define_method(rb_cGrnColumn, "scalar?", rb_grn_column_scalar_p, 0);
    rb_define_method(rb_cGrnColumn, "data?", rb_grn_column_data_p, 0);

    rb_define_method(rb_cGrnColumn, "with_weight?",
                     rb_grn_column_with_weight_p, 0);
    rb_define_method(rb_cGrnColumn, "weight_float32?",
                     rb_grn_column_weight_float32_p, 0);

    rb_define_method(rb_cGrnColumn, "find_indexes",
                     rb_grn_column_find_indexes, -1);

    rb_define_method(rb_cGrnColumn, "rename", rb_grn_column_rename, 1);

    rb_grn_init_data_column(mGrn);
    rb_grn_init_index_column(mGrn);
}
