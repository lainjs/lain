#include "lainjs_binding.h"

void lainjs_binding_create_object(duk_context *ctx) {
  duk_push_object(ctx);
}

// Binding type function.
// Binding object on stash with name.
void lainjs_binding_object_on_stash(duk_context *ctx, char* name) {
  duk_push_global_stash(ctx);
  lainjs_binding_create_object(ctx);
  duk_put_prop_string(ctx, -2, name);
  duk_pop(ctx);
  // After all, stack is '0'.
}

void lainjs_binding_object_on_module_object(duk_context* ctx, char* module, char* name) {
  lainjs_binding_get_object_on_stash(ctx, module);
  lainjs_binding_create_object(ctx);
  duk_put_prop_string(ctx, -2, name);
  duk_pop(ctx);
  // After all, stack is '0'.
}

void lainjs_binding_this_on_stash(duk_context *ctx, char* name) {
  duk_push_global_stash(ctx);
  duk_push_this(ctx);
  duk_put_prop_string(ctx, -2, name);
  duk_pop(ctx);
  // After all, stack is '0'.
}

void lainjs_binding_index_on_stash(duk_context *ctx, int idx, char* name) {
  duk_push_global_stash(ctx);
  duk_dup(ctx, idx);
  duk_put_prop_string(ctx, -2, name);
  duk_pop(ctx);
  // After all, stack is '0'.
}

void lainjs_binding_func_on_stash(duk_context *ctx, JS_NATIVE_CALLBACK func, char* name) {
  duk_push_global_stash(ctx);
  duk_push_c_function(ctx, func, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, name);
  duk_pop(ctx);
  // After all, stack is '0'.
}

void lainjs_binding_func_on_stashed_object(duk_context *ctx, char* obj,
                                           JS_NATIVE_CALLBACK func, char* name) {
  duk_push_global_stash(ctx);
  duk_get_prop_string(ctx, -1, obj);
  duk_push_c_function(ctx, func, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, name);
  duk_pop_2(ctx);
  // After all, stack is '0'.
}

void lainjs_binding_index_on_this(duk_context *ctx, int idx, char* name) {
  duk_push_this(ctx);
  duk_dup(ctx, idx);
  duk_put_prop_string(ctx, -2, name);
  duk_pop(ctx);
  // After all, stack is '0'.
}

void lainjs_binding_native_on_index(duk_context *ctx, int idx, void* native, char* name) {
  duk_dup(ctx, idx);
  duk_push_pointer(ctx, native);
  duk_put_prop_string(ctx, -2, name);
  duk_pop(ctx);
  // After all, stack is '0'.
}

void lainjs_binding_func_on_top(duk_context* ctx, JS_NATIVE_CALLBACK func, char* name) {
  // TODO: Add assert to check whether there is object or not.
  duk_push_c_function(ctx, func, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, name);
}

void lainjs_binding_native_on_top(duk_context* ctx, void* native) {
  duk_push_pointer(ctx, native);
  duk_put_prop_string(ctx, -2, "##native##");
}

void lainjs_binding_get_object_on_stash(duk_context* ctx, char* name) {
  duk_push_global_stash(ctx);
  duk_get_prop_string(ctx, -1, name);
  duk_remove(ctx, -2);
  // After all, remain the object called 'name'.
}

void lainjs_binding_get_object_on_index(duk_context* ctx, int index, char* name) {
  duk_dup(ctx, index);
  duk_get_prop_string(ctx, -1, name);
  duk_remove(ctx, -2);
  // After all, remain the object called 'name'.
}

void lainjs_binding_get_object_on_index_and_remove_index(duk_context* ctx,
                                                         int index,
                                                         char* name) {
  lainjs_binding_get_object_on_index(ctx, index, name);

  if (index < 0)
    index -= 1;
  duk_remove(ctx, index);
}

lainjs_func_t* lainjs_create_func_t() {
  lainjs_func_t *func = (lainjs_func_t*)malloc(sizeof(lainjs_func_t));
  func->args.size = 0;
  return func;
}

void lainjs_free_func_t(lainjs_func_t *this_) {
  if (this_)
    free(this_);
}

void lainjs_set_function(lainjs_func_t *this_, int idx) {
  if (idx >= 0)
    return;

  this_->function = idx;
}

void lainjs_add_argument(lainjs_func_t *this_, int idx) {
  if (idx >= 0)
    return;

  if (this_->args.size >= 20)
    return;

  this_->args.args[this_->args.size++] = idx; 
}

int lainjs_call_mathod(duk_context *ctx, lainjs_func_t *this_, LAIN_BOOL has_this) {
  duk_dup(ctx, this_->function);
  duk_remove(ctx, this_->function - 1);

  int i;
  for (i = 0; i < this_->args.size; i++) {
      int idx = this_->args.args[i];
      duk_dup(ctx, idx);
      duk_remove(ctx, idx - 1);
  }

  duk_int_t rc;
  if (has_this)
    rc = duk_pcall_method(ctx, this_->args.size - 1);
  else
    rc = duk_pcall(ctx, this_->args.size);

  lainjs_eval_exception(ctx, rc);

  int ret = 0;

  if (duk_is_number(ctx, -1))
    ret = duk_get_int(ctx, -1);
  duk_pop(ctx);

  return ret;
}

void lainjs_eval_exception(duk_context *ctx, duk_int_t rc) {
  if (rc != DUK_EXEC_SUCCESS) {
    // TODO: Some more information
    printf("Runtime Error : %s\n", duk_to_string(ctx, -1));
  }
}

char* lainjs_gen_key_on_stach(duk_context *ctx) {
  duk_push_global_stash(ctx);
  char *id = 0;
  do {
    if (id)
      free(id);
    id = lainjs_random_generate_id(20);
  } while (duk_has_prop_string(ctx, -1, id));

  duk_pop(ctx);

  return id;
}
