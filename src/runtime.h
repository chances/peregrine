#pragma once

#include <wgpu.h>
#include <wren.h>

struct Runtime {
  WrenConfiguration config;
  WrenVM* vm;
  char* error;
  WGPUAdapter adapter;
  WGPUDevice device;
};
typedef struct Runtime Runtime;
