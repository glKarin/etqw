#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

extern int GL_GetDrawMode(GLenum mode);

void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
	if (index == PC_ATTRIB_INDEX_VERTEX) // only binding if vertex position(xyz is starting in idDrawVert)
	R_VK_CmdBindVertexBuffer(vertexCache.BufferBinding_VK(), (uintptr_t)pointer);
}

void glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices)
{
	const int size = type == GL_UNSIGNED_SHORT ? sizeof(unsigned short) : sizeof(unsigned int);
	const bool indexCacheBinding = VK_NULL_HANDLE != vertexCache.IndexBufferBinding_VK(); // indices are in GPU memory

	if (indexCacheBinding)
		R_VK_CmdBindIndexBuffer(vertexCache.IndexBufferBinding_VK(), (uintptr_t)indices);
	else // indices are in CPU memory, transfer to GPU temp memory
	{
		vertCache_t *indexCache = vertexCache.AllocFrameTemp_VK((void *)indices, count * size, true);
		R_VK_CmdBindIndexBuffer(vertexCache.IndexBufferBinding_VK(), (uintptr_t)vertexCache.Position(indexCache));
	}

	globalState_t lastSt1 = vk_backend.state.st1;
	R_VK_Begin(GL_GetDrawMode(mode));
    R_VK_CmdDrawIndexed(count, 0/*offset / size*/, 0);
	vk_backend.state.st1 = lastSt1;

	if (!indexCacheBinding) // reset if indices are in CPU memory
		vertexCache.UnbindIndex();
}

void glBindBuffer (GLenum target, GLuint buffer)
{
	VKBuffer_t *buf = vk_context.buffers.list[buffer];
	if(target == GL_ELEMENT_ARRAY_BUFFER)
		vertexCache.BindIndexBuffer_VK(buffer && buf ? buf->buffer : NULL);
	else
		vertexCache.BindBuffer_VK(buffer && buf ? buf->buffer : NULL);
}

static int GL_GetTextureType(GLenum target)
{
	switch(target)
	{
		case GL_TEXTURE_2D:
			return TUT_2D;
		case GL_TEXTURE_CUBE_MAP:
			return TUT_CUBEMAP;
		case GL_TEXTURE_2D_ARRAY:
			return TUT_2D_ARRAY;
			break;
		default:
			return -1;
	}
}

void glBindTexture( GLenum target, GLuint texture )
{
	int tut = GL_GetTextureType(target);
	if (tut == -1)
		return;

	if (*((int *)&texture) != VKHANDLE_INVALID)
		R_VK_BindType(tut, vk_context.images.list[texture]);
	else
		R_VK_BindType(tut, NULL);
}

void R_VK_BindTexture( GLenum target, VKImage_t *image )
{
	int tut = GL_GetTextureType(target);
	if (tut == -1)
		return;

	R_VK_BindType(tut, image);
}

// pointer must in CPU
void * R_VK_VertexAttribPointer (GLint size, const void *pointer)
{
	vertCache_t *vertCache = vertexCache.AllocFrameTemp_VK((void *)pointer, size, false);
	return (idDrawVert *)vertexCache.Position_VK(vertCache);
}

float srgb_to_linear_f(float x)
{
	if (x < 0.04045f)
		return x / 12.92f;
	return powf((x + 0.055f) / 1.055f, 2.4f);
}

// 输入：srgb编码的字节 0~255
// 输出：线性 0~255 的uint8
uint8_t srgb_byte_to_linear_rgba8(uint8_t src)
{
	float f = src / 255.0f;
	float lin = srgb_to_linear_f(f);
	if (lin < 0.f) lin = 0.f;
	if (lin > 1.f) lin = 1.f;
	return (uint8_t)(lin * 255.0f + 0.5f);
}

void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
	int size = width * height * 4;
	VKBufferMap_t map;
	VKZERO(map);

	R_VK_BeginStageReadBuffer();

	R_VK_AllocStageReadBuffer(size, &map);

	VK_CopyColorAttachmentImageToBuffer(map.buffer, x, vk_context.swapchain.height - y - height, width, height, map.offset, width, height, vk_buffer.stageReadBuffer.currentStageCommandBuffer);

	R_VK_EndStageReadBuffer();

	//VK_ReadBuffer(map.buffer, pixels, map.offset, map.size, map.ptr);
	memcpy(pixels, map.ptr, map.size);

#if 0

	byte *dst = (byte *)pixels;
	for (int i = 0; i < width * height; i++)
	{
		int off = i * 4;
		dst[off + 0] = srgb_byte_to_linear_rgba8(dst[off + 0]);
		dst[off + 1] = srgb_byte_to_linear_rgba8(dst[off + 1]);
		dst[off + 2] = srgb_byte_to_linear_rgba8(dst[off + 2]);
		dst[off + 3] = dst[off + 3]; // alpha不变
	}

	R_WritePNG("test_capture.png", (const byte *)dst, width, height, 4, false);
#endif
}

/**
1    0    0    0
0   -1    0    0
0    0  0.5  0.5
0    0    0    1
 */
void R_VK_ClipSpaceZAndFlipY( float dst[ 16 ], const float src[ 16 ] )
{
	if ( dst != src )
	{
		memcpy( dst, src, 16 * sizeof( float ) );
	}
	for ( int col = 0; col < 4; col++ )
	{
		// Z: [-1,1] -> [0,1]
		dst[ col * 4 + 2 ] = 0.5f * ( src[ col * 4 + 2 ] + src[ col * 4 + 3 ] );
		// Y 轴翻转：NDC Y = -Y
		dst[ col * 4 + 1 ] = -src[ col * 4 + 1 ];
	}
}

/**
1    0    0    0
0   -1    0    0
0    0  0.5  0.5
0    0    0    1
 */
void R_VK_ClipSpaceZAndFlipY_RowMajor( float dst[ 16 ], const float src[ 16 ] )
{
	if (dst != src) memcpy(dst, src, 16 * sizeof(float));

	// 行主序：m[row*4+col]
	for (int col = 0; col < 4; col++)
	{
		// 第2行 = 0.5 * 第2行 + 0.5 * 第3行（Z修正）
		dst[2 * 4 + col] = 0.5f * (src[2 * 4 + col] + src[3 * 4 + col]);
		// 翻转第1行（Y）
		dst[1 * 4 + col] = -src[1 * 4 + col];
	}
}

void R_VK_ClipSpaceZ( float dst[ 16 ], const float src[ 16 ] )
{
	if ( dst != src )
	{
		memcpy( dst, src, 16 * sizeof( float ) );
	}
	for ( int col = 0; col < 4; col++ )
	{
		// Z: [-1,1] -> [0,1]
		dst[ col * 4 + 2 ] = 0.5f * ( src[ col * 4 + 2 ] + src[ col * 4 + 3 ] );
	}
}

void R_VK_FlipY( float dst[ 16 ], const float src[ 16 ] )
{
	if ( dst != src )
	{
		memcpy( dst, src, 16 * sizeof( float ) );
	}
	for ( int col = 0; col < 4; col++ )
	{
		// Y 轴翻转：NDC Y = -Y
		dst[ col * 4 + 1 ] = -src[ col * 4 + 1 ];
	}
}
