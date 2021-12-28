#ifndef PEREGRINE_GPU_H
#define PEREGRINE_GPU_H

#include <assert.h>
#include <wgpu.h>

#include "runtime.h"
#include "platform.h"

#pragma region Initialization
void peregrine_gpu_requestAdapterCallback(
  WGPURequestAdapterStatus status,
  WGPUAdapter received, const char* message,
  void* userdata
) {
  assert(status == WGPURequestAdapterStatus_Success);
#ifdef DEBUG
  if (message != NULL) printf("%s\n", message);
#endif
  ((Runtime*)userdata)->adapter = received;
}

void peregrine_gpu_requestDeviceCallback(
  WGPURequestDeviceStatus status,
  WGPUDevice received, const char* message,
  void* userdata
) {
  assert(status == WGPURequestDeviceStatus_Success);
#ifdef DEBUG
  if (message != NULL) printf("%s\n", message);
#endif
  ((Runtime*)userdata)->device = received;
}

void peregrine_gpu_logCallback(WGPULogLevel level, const char *msg) {
  char *level_str;
  switch (level) {
    case WGPULogLevel_Error:
      level_str = "Error";
      break;
    case WGPULogLevel_Warn:
      level_str = "Warn";
      break;
    case WGPULogLevel_Info:
      level_str = "Info";
      break;
    case WGPULogLevel_Debug:
      level_str = "Debug";
      break;
    case WGPULogLevel_Trace:
      level_str = "Trace";
      break;
    default:
      level_str = "Unknown Level";
  }
  printf("[%s] %s\n", level_str, msg);
}

void gpuInit(Runtime* runtime, WGPUSurface surface) {
  wgpuSetLogCallback(&peregrine_gpu_logCallback);
  wgpuSetLogLevel(WGPULogLevel_Warn);

  wgpuInstanceRequestAdapter(NULL, &(WGPURequestAdapterOptions){
    .nextInChain = NULL,
    .compatibleSurface = surface
  }, &peregrine_gpu_requestAdapterCallback, (void*) runtime);
  wgpuAdapterRequestDevice(runtime->adapter, &(WGPUDeviceDescriptor){
    .nextInChain = (const WGPUChainedStruct*) &(WGPUDeviceExtras){
      .chain = (WGPUChainedStruct){.next = NULL, .sType = (WGPUSType) WGPUSType_DeviceExtras},
      .label = "Device",
      .tracePath = NULL,
    },
    .requiredLimits = &(WGPURequiredLimits){
      .nextInChain = NULL,
      .limits = (WGPULimits){
        .maxBindGroups = 1,
      },
    },
  }, &peregrine_gpu_requestDeviceCallback, (void*) runtime);
}
#pragma endregion

#endif // PEREGRINE_GPU_H
