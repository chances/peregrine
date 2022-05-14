#pragma once

#include <assert.h>
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
  Runtime* runtime = wrenGetUserData(vm);
  assert(runtime != NULL);
  // assert(runtime->error == NULL);

  const char* fmt;
  switch (errorType) {
    case WREN_ERROR_COMPILE:
      fmt = "[%s line %d] [Error] %s\n";
      runtime->error = malloc(snprintf(NULL, 0, fmt, module, line, msg) + 1);
      sprintf(runtime->error, fmt, module, line, msg);
      break;
    case WREN_ERROR_STACK_TRACE:
      fmt = "[%s line %d] in %s\n";
      runtime->error = malloc(snprintf(NULL, 0, fmt, module, line, msg) + 1);
      sprintf(runtime->error, fmt, module, line, msg);
      break;
    case WREN_ERROR_RUNTIME:
      fmt = "[Runtime Error] %s\n";
      runtime->error = malloc(snprintf(NULL, 0, fmt, msg) + 1);
      sprintf(runtime->error, fmt, msg);
      break;
  }
}
#pragma endregion

Runtime* runtimeInit() {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &peregrine_wren_writeFn;
  config.errorFn = &peregrine_wren_errorFn;

  Runtime runtime = {0};
  runtime.config = config;
  runtime.vm = wrenNewVM(&config);
  Runtime* runtimePtr = (Runtime*) malloc(sizeof(Runtime));
  *runtimePtr = runtime;
  wrenSetUserData(runtime.vm, (void*) runtimePtr);
  return runtimePtr;
}

void scriptError(Runtime* runtime) {
  if (runtime->error == NULL) return;
  printf("%s\n", runtime->error);
  free(runtime->error);
}

bool scriptLoad(Runtime* runtime, const char* file, const char* module) {
  assert(runtime != NULL);

  FILE* f = fopen(file, "r");
  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  char* script = malloc(length);
  fseek(f, 0, SEEK_SET);
  size_t readLength = fread(script, sizeof(char), length, f);
  assert(readLength == length);
  // TODO: Error handling, ugh
  // if (feof(f)) printf("Error reading test.bin: unexpected end of file\n");
  // else if (ferror(f)) perror("Error reading test.bin");
  fclose(f);

  WrenInterpretResult res = wrenInterpret(runtime->vm, module, script);
  if (res != WREN_RESULT_SUCCESS) {
    scriptError(runtime);
    return false;
  }
  return true;
}

inline WrenHandle* wrenGetClass(WrenVM* vm, const char* module, const char* class) {
  assert(wrenHasVariable(vm, module, class));
  wrenEnsureSlots(vm, 1);
  wrenGetVariable(vm, module, class, 0);
  return wrenGetSlotHandle(vm, 0);
}
