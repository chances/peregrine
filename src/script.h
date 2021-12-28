#ifndef PEREGRINE_SCRIPT_H
#define PEREGRINE_SCRIPT_H

#include <stdio.h>
#include <wren.h>

#include "runtime.h"

#pragma region Wren VM
static void peregrine_wren_writeFn(WrenVM* vm, const char* text) {
  printf("%s", text);
}

void peregrine_wren_errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType) {
    case WREN_ERROR_COMPILE:
      printf("[%s line %d] [Error] %s\n", module, line, msg);
      break;
    case WREN_ERROR_STACK_TRACE:
      printf("[%s line %d] in %s\n", module, line, msg);
      break;
    case WREN_ERROR_RUNTIME:
      printf("[Runtime Error] %s\n", msg);
      break;
  }
}
#pragma endregion

Runtime runtimeInit() {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &peregrine_wren_writeFn;
  config.errorFn = &peregrine_wren_errorFn;
  WrenVM* vm = wrenNewVM(&config);

  return (Runtime) { config, vm, .adapter = NULL, .device = NULL };
}

#endif // PEREGRINE_SCRIPT_H
