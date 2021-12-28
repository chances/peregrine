#ifndef PEREGRINE_PLATFORM_H
#define PEREGRINE_PLATFORM_H

#ifdef __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#define GLFW_EXPOSE_NATIVE_COCOA
#include <glfw3native.h>
#define __OSX__
#include "platform/mac.h"
#endif
#endif

#endif // PEREGRINE_PLATFORM_H
