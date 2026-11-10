#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

static void vec4_identity(vec4 vec)
{
	vec[0] = vec[1] = vec[2] = 0.0f;
	vec[3] = 1.0f;
}

static void vec4s_identity(vec4 vecs[], int num)
{
	for (int i = 0; i < num; i++)
	{
		vec4_identity(vecs[i]);
	}
}
#define VEC4S_IDENTITY(vec4s) vec4s_identity(vec4s, COUNT_OF(vec4s))

static void mat4_make_identity(mat4 mat)
{
	mat[1] = mat[2] = mat[3] =
	mat[4] = mat[6] = mat[7] =
	mat[8] = mat[9] = mat[11] =
	mat[12] = mat[13] = mat[14] =
	0.0f;
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
}

static void mat4s_identity(mat4 mats[], int num)
{
	for (int i = 0; i < num; i++)
	{
		mat4_make_identity(mats[i]);
	}
}
#define MAT4S_IDENTITY(mat4s) mat4s_identity(mat4s, COUNT_OF(mat4s))

void R_VK_InitGlobalUBO()
{
	VKZERO(globalUBO);

	mat4_make_identity(globalUBO.modelViewProjectionMatrix);
	mat4_make_identity(globalUBO.projectionMatrix);
	mat4_make_identity(globalUBO.modelViewMatrix);
	mat4_make_identity(globalUBO.modelMatrix);
	mat4_make_identity(globalUBO.textureMatrix);
	mat4_make_identity(globalUBO.fogMatrix);
	vec4_identity(globalUBO.fogColor);
	vec4_identity(globalUBO.clipPlane);
	vec4_identity(globalUBO.lightProjectionS); globalUBO.lightProjectionS[0] = 1.0f;
	vec4_identity(globalUBO.lightProjectionT); globalUBO.lightProjectionS[1] = 1.0f;
	vec4_identity(globalUBO.lightProjectionQ); globalUBO.lightProjectionS[2] = 1.0f;
	vec4_identity(globalUBO.lightFalloff);
	vec4_identity(globalUBO.bumpMatrixS); globalUBO.bumpMatrixS[0] = 1.0f;
	vec4_identity(globalUBO.bumpMatrixT); globalUBO.bumpMatrixS[1] = 1.0f;
	vec4_identity(globalUBO.diffuseMatrixS); globalUBO.diffuseMatrixS[0] = 1.0f;
	vec4_identity(globalUBO.diffuseMatrixT); globalUBO.diffuseMatrixS[1] = 1.0f;
	vec4_identity(globalUBO.specularMatrixS); globalUBO.specularMatrixS[0] = 1.0f;
	vec4_identity(globalUBO.specularMatrixT); globalUBO.specularMatrixS[1] = 1.0f;
	vec4_identity(globalUBO.diffuseColor);
	vec4_identity(globalUBO.specularColor);
	vec4_identity(globalUBO.specularExponent);
	vec4_identity(globalUBO.nonPowerOfTwo);
	vec4_identity(globalUBO.windowCoords);
	vec4_identity(globalUBO.texgenS); globalUBO.texgenS[0] = 1.0f;
	vec4_identity(globalUBO.texgenT); globalUBO.texgenT[1] = 1.0f;
	vec4_identity(globalUBO.texgenQ); globalUBO.texgenQ[2] = 1.0f;
#ifdef _SHADOW_MAPPING
	MAT4S_IDENTITY(globalUBO.shadowMVPMatrix);
	vec4_identity(globalUBO.globalLightOrigin);
#endif
	VEC4S_IDENTITY(globalUBO.uniformParm);
	VEC4S_IDENTITY(globalUBO.vertexParm);
	VEC4S_IDENTITY(globalUBO.fragmentParm);
	VEC4S_IDENTITY(globalUBO.megaTextureLevel);
	vec4_identity(globalUBO.lightOrigin);
	vec4_identity(globalUBO.viewOrigin);
	vec4_identity(globalUBO.glColor);
	globalUBO.colorModulate = 0.0f;
	globalUBO.colorAdd = 1.0f;
	globalUBO.alphaTest = 0.0f;

	VKZERO(vk_backend.uboState);
}

#include "ubo_copy.inc.h"
