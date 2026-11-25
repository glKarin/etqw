#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"


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
	vec4_x_identity(globalUBO.lightProjectionS);
	vec4_y_identity(globalUBO.lightProjectionT);
	vec4_z_identity(globalUBO.lightProjectionQ);
	vec4_identity(globalUBO.lightFalloff);
	vec4_x_identity(globalUBO.bumpMatrixS);
	vec4_y_identity(globalUBO.bumpMatrixT);
	vec4_x_identity(globalUBO.diffuseMatrixS);
	vec4_y_identity(globalUBO.diffuseMatrixT);
	vec4_x_identity(globalUBO.specularMatrixS);
	vec4_y_identity(globalUBO.specularMatrixT);
	vec4_identity(globalUBO.diffuseColor);
	vec4_identity(globalUBO.specularColor);
	vec4_identity(globalUBO.specularExponent);
	vec4_identity(globalUBO.nonPowerOfTwo);
	vec4_identity(globalUBO.windowCoords);
	vec4_x_identity(globalUBO.texgenS);
	vec4_y_identity(globalUBO.texgenT);
	vec4_z_identity(globalUBO.texgenQ);
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

#ifdef _RAVEN
	vec2_one_identity(globalUBO.textureScale);
	vec2_identity(globalUBO.textureHalfScale);
	vec4_identity(globalUBO.backgroundColor);
	globalUBO.scrollX = 0.0f;
	globalUBO.scrollY = 0.0f;
	globalUBO.sizeX = 0.0f;
	globalUBO.sizeY = 0.0f;
	globalUBO.texCoordSize = 0.0f;
#endif

	VKZERO(vk_backend.uboState);
}


#include "ubo_copy.inc.h"

#include "ubo_clear.inc.h"

#include "ubo_loc.inc.h"

#include "ubo_check.inc.h"
