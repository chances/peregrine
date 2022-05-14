#pragma once

#ifdef __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#define GLFW_EXPOSE_NATIVE_COCOA
#include <glfw3native.h>
#define __OSX__
#include "platform/mac.h"
#endif
#endif
