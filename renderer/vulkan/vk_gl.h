#ifndef _VULKAN_VKGL_H
#define _VULKAN_VKGL_H

#include "../qgl.h"


#if 1
#define VKGL_FUNC(x) vk##x
#else
#define VKGL_FUNC(x) x
#endif

#define QGLPROC(name, rettype, args) rettype (vk##name) args;
#include "../qgl_proc.h"

void R_VK_LoadOpenGLCompatFunc(void);

#endif //_VULKAN_VKGL_H
