#include "lainjs_binding.h"

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
