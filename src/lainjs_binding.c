#include "lainjs_binding.h"

lainjs_func_t* lainjs_create_func_t() {
  lainjs_func_t *func = (lainjs_func_t*)malloc(sizeof(lainjs_func_t));
  func->args.size = 0;
  return func;
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

void lainjs_call_mathod_with_this(duk_context *ctx, lainjs_func_t *this_) {
  duk_dup(ctx, this_->function);
  duk_remove(ctx, this_->function - 1);

  int i;
  for (i = 0; i < this_->args.size; i++) {
      int idx = this_->args.args[i];
      duk_dup(ctx, idx);
      duk_remove(ctx, idx - 1);
  }

  duk_int_t rc = duk_pcall_method(ctx, this_->args.size - 1);
  lainjs_eval_exception(ctx, rc);

  duk_pop(ctx); 
}

void lainjs_eval_exception(duk_context *ctx, duk_int_t rc) {
  if (rc != DUK_EXEC_SUCCESS) {
    // TODO: Some more information
    printf("Runtime Error : %s\n", duk_to_string(ctx, -1));
  }
}
