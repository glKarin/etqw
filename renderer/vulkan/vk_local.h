#ifndef _VULKAN_LOCAL_H
#define _VULKAN_LOCAL_H

#define IDVK_CONTAINER_SIZE_CONFIG 1

#ifdef _MULTITHREAD
#if 1
#define VKASSERT_THREAD assert(!multithreadActive || !renderThread->IsRunning() || Sys_InRenderThread());
//#define VKASSERT_THREAD if(!multithreadActive || !renderThread->IsActive() || Sys_InRenderThread()){}else{printf("ttt\n"); memset((void *)12, 0, 100);};
#else
#define VKASSERT_THREAD
#endif
#else
#define VKASSERT_THREAD
#endif

#include "vk_type.h"
#include "vk.h"
#include "vk_enum.h"
#include "vk_sampler.h"
#include "vk_query.h"
#include "vk_buffer.h"
#include "vk_state.h"
#include "vk_ubo.h"
#include "vk_backend.h"
#include "Volk/volk.h"
#include "vk_glsl.h"
#include "vk_shader.h"

#define RB_OPENGL "OpenGL"
#define RB_VULKAN "Vulkan"

#define VULKAN_BINARY_FORMAT ((unsigned int)('s' << 24 | 'p' << 16 | 'v' << 8 | '1'))

extern bool USING_VULKAN;

extern idCVar harm_r_renderBackend;
extern idCVar r_vkUsePipelineCache;
extern idCVar r_vkUseGlobalUniform;
extern idCVar r_vkSetDebugInfo;
extern idCVar r_vkEnableValidationLayer;

#if IDVK_CONTAINER_SIZE_CONFIG
extern idCVar r_vkContextPipelineSize;
extern idCVar r_vkContextPipelineGranularity;
extern idCVar r_vkContextPipelineLayoutSize;
extern idCVar r_vkContextPipelineLayoutGranularity;
extern idCVar r_vkContextShaderSize;
extern idCVar r_vkContextShaderGranularity;
extern idCVar r_vkContextSamplerSize;
extern idCVar r_vkContextSamplerGranularity;
extern idCVar r_vkContextImageViewSize;
extern idCVar r_vkContextImageViewGranularity;
extern idCVar r_vkContextImageSize;
extern idCVar r_vkContextImageGranularity;
extern idCVar r_vkContextBufferSize;
extern idCVar r_vkContextBufferGranularity;

extern idCVar r_vkBackendSamplerSize;
extern idCVar r_vkBackendSamplerGranularity;
extern idCVar r_vkBackendFreeBufferSize;
extern idCVar r_vkBackendFreeBufferGranularity;
extern idCVar r_vkBackendPurgeImageSize;
extern idCVar r_vkBackendPurgeImageGranularity;
extern idCVar r_vkBackendPipelineLayoutSize;
extern idCVar r_vkBackendPipelineLayoutGranularity;

extern idCVar r_vkQueryFrameCount;

extern idCVar r_vkStagingWriteBufferBlockSize;
extern idCVar r_vkStagingReadBufferBlockSize;
extern idCVar r_vkUniformBufferBlockSize;
extern idCVar r_vkVertexBufferBlockSize;
extern idCVar r_vkIndexBufferBlockSize;
#endif

void R_VK_ByteListAppend(idList<byte> &list, const void *data, int length);
int RB_VK_GetCacheShaderBinary(byte **ret);
void RB_VK_WriteCacheShaderBinary(const char *path, const byte *binary, int length);
int RB_VK_ReadShaderBinaryData(const char *path, byte **ret);
void RB_VK_RemoveShaderBinaryCache(const char *path);
bool RB_VK_CreateShaderProgramFromBinary(const char *name, const byte *binary, int length);

void R_VK_CheckPortableExtensions(void);
bool RB_VK_LoadShaderProgramFromProp(const GLSLShaderProp *prop);
void R_VK_ClipSpaceZAndFlipY( float dst[ 16 ], const float src[ 16 ] );
void R_VK_ClipSpaceZAndFlipY_RowMajor( float dst[ 16 ], const float src[ 16 ] );
void * R_VK_VertexAttribPointer (GLint size, const void *pointer);
void R_VK_BindTexture( GLenum target, VKImage_t *image );

void R_VK_AddLoadImageList( idImage * image, bool checkForPrecompressed );
void R_VK_AddUploadScratchImageList( idImage * image, byte *data, int cols, int rows );
void R_VK_AddUpdateCinematicImageList( idImage * image, idCinematic *cinematic, int time );
void R_VK_HandleLoadImageList( void );
void R_VK_RemoveUploadImageList( idImage * image );
bool R_VK_DisableUploadImage( void );

void R_VK_AddLoadBufferList( struct vertCache_s *block );
void R_VK_HandleLoadBufferList( void );
bool R_VK_DisableUploadBuffer(void);

void R_VK_AddCommands();
#if 0
void RB_ClearColor(const void *data);
void R_AddClearColorCmd(void);
#endif

#endif //_VULKAN_LOCAL_H
