#define GLFW_EXPOSE_NATIVE_COCOA

#include <glfw3.h>
#include <time.h>
#include <wgpu.h>
#include <wren.h>

#include "../src/platform.h"
#include "../src/script.h"
#include "../src/gpu.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    return 1;
  }

  Runtime* runtime = runtimeInit();

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Peregrine", NULL, NULL);

  WGPUSurfaceDescriptorFromMetalLayer* surfaceDescriptor = NULL;
  // TODO: Platform window integration with Linux & Windows
#ifdef __OSX__
  surfaceDescriptor = &(WGPUSurfaceDescriptorFromMetalLayer){
    .chain = (WGPUChainedStruct){.next = NULL, .sType = WGPUSType_SurfaceDescriptorFromMetalLayer},
    .layer = peregrine_platform_GetMetalLayer((void*) glfwGetCocoaWindow(window)),
  };
#endif
  assert(surfaceDescriptor);

  WGPUSurface surface = wgpuInstanceCreateSurface(NULL, &(WGPUSurfaceDescriptor){
    .label = NULL,
    .nextInChain = (const WGPUChainedStruct*) surfaceDescriptor,
  });
  gpuInit(runtime, surface);

  scriptLoad(runtime, argv[1], "main");
  WrenHandle* app = wrenGetClass(runtime->vm, "main", "App");
  WrenHandle* app_tick = wrenMakeCallHandle(runtime->vm, "tick(_)");

  #pragma region Main Loop
  struct timespec clock;
  assert(clock_gettime(CLOCK_MONOTONIC, &clock) == 0);
  long start = clock.tv_nsec;

  glfwShowWindow(window);
  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    assert(clock_gettime(CLOCK_MONOTONIC, &clock) == 0);
    long elapsed = clock.tv_nsec - start;
    wrenEnsureSlots(runtime->vm, 2);
    wrenSetSlotHandle(runtime->vm, 0, app);
    wrenSetSlotDouble(runtime->vm, 1, elapsed / 1000000.0f);
    if (wrenCall(runtime->vm, app_tick) != WREN_RESULT_SUCCESS) {
      scriptError(runtime);
    }

    start = clock.tv_nsec;
  }
  #pragma endregion

  glfwTerminate();
  wrenFreeVM(runtime->vm);
  free(runtime);

  return 0;
}

const char* test = "class App {\
    static tick(dt) {\
        System.print(dt)\
    }\
    static render() {}\
}";
