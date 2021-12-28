#ifndef PEREGRINE_RUNTIME_H
#define PEREGRINE_RUNTIME_H

#include <wgpu.h>
#include <wren.h>

struct Runtime {
  WrenConfiguration config;
  WrenVM* vm;
  WGPUAdapter adapter;
  WGPUDevice device;
};
typedef struct Runtime Runtime;

#endif // PEREGRINE_RUNTIME_H
