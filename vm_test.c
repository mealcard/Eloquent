/*
 * vm_test.c
 *
 * The Test Driver for the Virtual Machine
 *
 * Copyright (C) 2013-06-07 liutos <mat.liutos@gmail.com>
 */
#include <string.h>

#include "compiler.h"
#include "macros.h"
#include "object.h"
#include "prims.h"
#include "type.h"
#include "vm.h"

int main(int argc, char *argv[])
{
  char *inputs[] = {
      "make-input-string",
      "(set! is (make-input-string \"Hello\"))",
      "read-char-from-string",
      "(read-char-from-string is)", // #\H
      "(read-char-from-string is)", // #\e
      "(read-char-from-string is)", // #\l
      "(unget-char-to-string #\\m is)", // #\m
      "(read-char-from-string is)", // #\m
  };
  init_global_variable();
  init_prims();
  init_compiled_prims();
  init_macros();
//  load_init_file();
  for (int i = 0; i < sizeof(inputs) / sizeof(char *); i++) {
    writef(standard_out, ">> %s\n", make_string(inputs[i]));
    lisp_object_t *expr = read_object_from_string(strdup(inputs[i]));
    expr = compile_to_bytecode(expr);
    if (!is_signaled(expr))
      expr = run_by_llam(expr);
    if (is_signaled(expr))
      writef(standard_out, "%?\n", expr);
    else
      writef(standard_out, "=> %?\n", expr);
  }
  return 0;
}
