#ifndef _VULKAN_VKGLSL_H
#define _VULKAN_VKGLSL_H

#include <stdint.h>

typedef enum shaderType_e
{
    SHADER_STAGE_VERTEX = 1,
    SHADER_STAGE_FRAGMENT,
    SHADER_STAGE_COMPUTE,
} shaderType_t;

// int VK_LoadFile(const char *path, uint8_t **data);
// int VK_SaveFile(const char *path, uint8_t *data, int length);

int VK_GLSL_LoadShaderFile(const char *path, uint8_t **data, int stage, const char *marcos);
int VK_GLSL_LoadShaderData(const char *path, const char *source, uint8_t **data, int stage, const char *marcos);

int VK_GLSL_CompileSPIRV( const char* filename, const char* dataGLSL, const int stage, uint32_t** spirvBuffer );
void VK_GLSL_FreeSource(void* spirvBuffer);

#endif //_VULKAN_VKGLSL_H