
#include "vk_ubo.h"

#define layout_binding_0_uniform VERTEX_UBO_DECLARATION
#define end_layout_binding_0_uniform END_VERTEX_UBO
#define layout_binding_1_uniform FRAGMENT_UBO_DECLARATION
#define end_layout_binding_1_uniform END_FRAGMENT_UBO


layout_binding_0_uniform(default)
    mat4 modelViewProjectionMatrix;
	mat4 textureMatrix;
	float colorModulate;
	float colorAdd;
end_layout_binding_0_uniform(default)

layout_binding_1_uniform(default)
	vec4 glColor;
end_layout_binding_1_uniform(default)


layout_binding_0_uniform(zfill)
	mat4 modelViewProjectionMatrix;
	mat4 textureMatrix;
end_layout_binding_0_uniform(zfill)

layout_binding_1_uniform(zfill)
	vec4 glColor;
	float alphaTest;
end_layout_binding_1_uniform(zfill)


layout_binding_0_uniform(zfillClip)
	mat4 modelViewProjectionMatrix;
	mat4 textureMatrix;
    vec4 clipPlane;
end_layout_binding_0_uniform(zfillClip)

layout_binding_1_uniform(zfillClip)
	vec4 glColor;
	float alphaTest;
end_layout_binding_1_uniform(zfillClip)


layout_binding_0_uniform(fog)
    mat4 modelViewProjectionMatrix;
	mat4 fogMatrix;
end_layout_binding_0_uniform(fog)

layout_binding_1_uniform(fog)
	vec4 fogColor;
end_layout_binding_1_uniform(fog)

layout_binding_0_uniform(blendLight)
    mat4 modelViewProjectionMatrix;
	mat4 fogMatrix;
end_layout_binding_0_uniform(blendLight)

FRAGMENT_UBO_ALIAS_DEFINE(blendLight, fog)


layout_binding_0_uniform(shadow)
	mat4 modelViewProjectionMatrix;
	vec4 glColor;
	vec4 lightOrigin;
end_layout_binding_0_uniform(shadow)


layout_binding_0_uniform(texgen)
	vec4 texgenS;
	vec4 texgenT;
	vec4 texgenQ;
	mat4 textureMatrix;
	mat4 modelViewProjectionMatrix;
	float colorModulate;
	float colorAdd;
end_layout_binding_0_uniform(texgen)

layout_binding_1_uniform(texgen)
	vec4 glColor;
end_layout_binding_1_uniform(texgen)


layout_binding_0_uniform(simpleVertexTexcoord)
	mat4 modelViewProjectionMatrix;
end_layout_binding_0_uniform(simpleVertexTexcoord)

layout_binding_1_uniform(stencilToColor)
	int iuniformParm0;
end_layout_binding_1_uniform(stencilToColor)

layout_binding_1_uniform(depthToColor)
	bool32 buniformParm0;
end_layout_binding_1_uniform(depthToColor)

layout_binding_0_uniform(cubemap)
    mat4 modelViewProjectionMatrix;
    mat4 textureMatrix;
    vec4 viewOrigin;
    float colorModulate;
    float colorAdd;
end_layout_binding_0_uniform(cubemap)

layout_binding_1_uniform(cubemap)
    vec4 glColor;
end_layout_binding_1_uniform(cubemap)


layout_binding_0_uniform(diffuseCubemap)
    mat4 modelViewProjectionMatrix;
    mat4 textureMatrix;
    float colorModulate;
    float colorAdd;
end_layout_binding_0_uniform(diffuseCubemap)

FRAGMENT_UBO_ALIAS_DEFINE(diffuseCubemap, cubemap)


layout_binding_0_uniform(bumpyEnvironment)
    mat4 modelViewProjectionMatrix;
    mat4 modelMatrix;
    vec4 viewOrigin;
    float colorModulate;
    float colorAdd;
end_layout_binding_0_uniform(bumpyEnvironment)

layout_binding_1_uniform(bumpyEnvironment)
    vec4 glColor;
end_layout_binding_1_uniform(bumpyEnvironment)


layout_binding_0_uniform(colorProcess)
    mat4 modelViewProjectionMatrix;
    vec4 vertexParm0;
    vec4 vertexParm1;
end_layout_binding_0_uniform(colorProcess)

layout_binding_1_uniform(colorProcess)
    vec4 nonPowerOfTwo;
end_layout_binding_1_uniform(colorProcess)


layout_binding_0_uniform(depthPerforated)
    mat4 textureMatrix;
    mat4 modelViewProjectionMatrix;
end_layout_binding_0_uniform(depthPerforated)

layout_binding_1_uniform(depthPerforated)
    vec4 glColor;
    float alphaTest;
end_layout_binding_1_uniform(depthPerforated)


layout_binding_0_uniform(depth)
    mat4 modelViewProjectionMatrix;
    vec4 lightOrigin;
    // vec4 uniformParm0; // .w 1.0 or 0.0
end_layout_binding_0_uniform(depth)


layout_binding_0_uniform(heatHaze)
    mat4 modelViewProjectionMatrix;
    mat4 modelViewMatrix;
    mat4 projectionMatrix;
    vec4 vertexParm0;
    vec4 vertexParm1;
end_layout_binding_0_uniform(heatHaze)

layout_binding_1_uniform(heatHaze)
    vec4 nonPowerOfTwo;
    vec4 windowCoords;
end_layout_binding_1_uniform(heatHaze)


layout_binding_0_uniform(heatHazeWithMask)
    mat4 modelViewProjectionMatrix;
    mat4 modelViewMatrix;
    mat4 projectionMatrix;
    vec4 vertexParm0;
    vec4 vertexParm1;
end_layout_binding_0_uniform(heatHazeWithMask)

layout_binding_1_uniform(heatHazeWithMask)
    vec4 nonPowerOfTwo;
    vec4 windowCoords;
end_layout_binding_1_uniform(heatHazeWithMask)


layout_binding_0_uniform(heatHazeWithMaskAndVertex)
    mat4 modelViewProjectionMatrix;
    mat4 modelViewMatrix;
    mat4 projectionMatrix;
    vec4 vertexParm0;
    vec4 vertexParm1;
end_layout_binding_0_uniform(heatHazeWithMaskAndVertex)

layout_binding_1_uniform(heatHazeWithMaskAndVertex)
    vec4 nonPowerOfTwo;
    vec4 windowCoords;
end_layout_binding_1_uniform(heatHazeWithMaskAndVertex)


layout_binding_0_uniform(environment)
    mat4 modelViewProjectionMatrix;
    mat4 modelViewMatrix;
    mat4 textureMatrix;
    vec4 viewOrigin;
    float colorModulate;
    float colorAdd;
end_layout_binding_0_uniform(environment)

layout_binding_1_uniform(environment)
    vec4 glColor;
end_layout_binding_1_uniform(environment)


layout_binding_0_uniform(interaction)
	vec4 lightProjectionS;
	vec4 lightProjectionT;
	vec4 lightFalloff;
	vec4 lightProjectionQ;

	vec4 lightOrigin;
	vec4 viewOrigin;

	vec4 bumpMatrixS;
	vec4 bumpMatrixT;
	vec4 diffuseMatrixS;
	vec4 diffuseMatrixT;
	vec4 specularMatrixS;
	vec4 specularMatrixT;

	mat4 modelViewProjectionMatrix;
	float colorModulate;
    float colorAdd;
end_layout_binding_0_uniform(interaction)

layout_binding_1_uniform(interaction)
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 specularExponent;
end_layout_binding_1_uniform(interaction)


VERTEX_UBO_ALIAS_DEFINE(interactionBlinnPhong, interaction)
FRAGMENT_UBO_ALIAS_DEFINE(interactionBlinnPhong, interaction)


VERTEX_UBO_ALIAS_DEFINE(interactionPBR, interaction)
FRAGMENT_UBO_ALIAS_DEFINE(interactionPBR, interaction)


layout_binding_0_uniform(ambientLighting)
    vec4 lightProjectionS;
    vec4 lightProjectionT;
    vec4 lightFalloff;
    vec4 lightProjectionQ;

    vec4 lightOrigin;
    vec4 viewOrigin;

    vec4 bumpMatrixS;
    vec4 bumpMatrixT;
    vec4 diffuseMatrixS;
    vec4 diffuseMatrixT;
    vec4 specularMatrixS;
    vec4 specularMatrixT;

    mat4 modelViewProjectionMatrix;
    mat4 modelMatrix;
    mat4 colorModulate;
    mat4 colorAdd;
end_layout_binding_0_uniform(ambientLighting)

FRAGMENT_UBO_ALIAS_DEFINE(ambientLighting, interaction)


layout_binding_0_uniform(interactionTranslucent)
    vec4 lightProjectionS;
    vec4 lightProjectionT;
    vec4 lightFalloff;
    vec4 lightProjectionQ;

    vec4 lightOrigin;
    vec4 viewOrigin;

    vec4 bumpMatrixS;
    vec4 bumpMatrixT;
    vec4 diffuseMatrixS;
    vec4 diffuseMatrixT;
    vec4 specularMatrixS;
    vec4 specularMatrixT;

    mat4 modelViewProjectionMatrix;
#if defined(_AMBIENT)
    mat4 modelMatrix;
#endif
    mat4 colorModulate;
    mat4 colorAdd;
end_layout_binding_0_uniform(interactionTranslucent)

layout_binding_1_uniform(interactionTranslucent)
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 specularExponent;
    mat4 uniformParm0;
end_layout_binding_1_uniform(interactionTranslucent)

VERTEX_UBO_ALIAS_DEFINE(interactionBlinnPhongTranslucent, interactionTranslucent)
FRAGMENT_UBO_ALIAS_DEFINE(interactionBlinnPhongTranslucent, interactionTranslucent)

VERTEX_UBO_ALIAS_DEFINE(interactionPBRTranslucent, interactionTranslucent)
FRAGMENT_UBO_ALIAS_DEFINE(interactionPBRTranslucent, interactionTranslucent)

layout_binding_0_uniform(ambientLightingTranslucent)
    vec4 lightProjectionS;
vec4 lightProjectionT;
    vec4 lightFalloff;
    vec4 lightProjectionQ;

    vec4 lightOrigin;
    vec4 viewOrigin;

    vec4 bumpMatrixS;
    vec4 bumpMatrixT;
    vec4 diffuseMatrixS;
    vec4 diffuseMatrixT;
    vec4 specularMatrixS;
    vec4 specularMatrixT;

    mat4 modelViewProjectionMatrix;
    mat4 modelMatrix;
    mat4 colorModulate;
    mat4 colorAdd;
end_layout_binding_0_uniform(ambientLightingTranslucent)

FRAGMENT_UBO_ALIAS_DEFINE(ambientLightingTranslucent, interactionTranslucent)

layout_binding_1_uniform(interactionSoft)
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 nonPowerOfTwo;
    vec4 windowCoords;
    vec4 specularExponent;
    mat4 uniformParm0;
    mat4 uniformParm1;
end_layout_binding_1_uniform(interactionSoft)

VERTEX_UBO_ALIAS_DEFINE(interactionSoft, interactionTranslucent)

VERTEX_UBO_ALIAS_DEFINE(interactionBlinnPhongSoft, interactionTranslucent)
FRAGMENT_UBO_ALIAS_DEFINE(interactionBlinnPhongSoft, interactionSoft)

VERTEX_UBO_ALIAS_DEFINE(interactionPBRSoft, interactionPBRTranslucent)
FRAGMENT_UBO_ALIAS_DEFINE(interactionPBRSoft, interactionSoft)

VERTEX_UBO_ALIAS_DEFINE(ambientLightingSoft, ambientLightingTranslucent)
FRAGMENT_UBO_ALIAS_DEFINE(ambientLightingSoft, interactionSoft)


layout_binding_0_uniform(interactionShadowMappingPointLight)
    vec4 lightProjectionS;
    vec4 lightProjectionT;
    vec4 lightFalloff;
    vec4 lightProjectionQ;

    vec4 lightOrigin;
    vec4 viewOrigin;

    vec4 bumpMatrixS;
    vec4 bumpMatrixT;
    vec4 diffuseMatrixS;
    vec4 diffuseMatrixT;
    vec4 specularMatrixS;
    vec4 specularMatrixT;

    mat4 modelViewProjectionMatrix;

    mat4 modelMatrix;
    vec4 globalLightOrigin;
    mat4 colorModulate;
    mat4 colorAdd;
end_layout_binding_0_uniform(interactionShadowMappingPointLight)

VERTEX_UBO_ALIAS_DEFINE(interactionShadowMappingBlinnPhongPointLight, interactionShadowMappingPointLight)

VERTEX_UBO_ALIAS_DEFINE(interactionShadowMappingPBRPointLight, interactionShadowMappingPointLight)

VERTEX_UBO_ALIAS_DEFINE(ambientLightingShadowMappingPointLight, interactionShadowMappingPointLight)


layout_binding_0_uniform(interactionShadowMappingParallelLight)
    vec4 lightProjectionS;
    vec4 lightProjectionT;
    vec4 lightFalloff;
    vec4 lightProjectionQ;

    vec4 lightOrigin;
    vec4 viewOrigin;

    vec4 bumpMatrixS;
    vec4 bumpMatrixT;
    vec4 diffuseMatrixS;
    vec4 diffuseMatrixT;
    vec4 specularMatrixS;
    vec4 specularMatrixT;

    mat4 modelViewProjectionMatrix;

    mat4 modelMatrix;
    vec4 globalLightOrigin;
    mat4 modelViewMatrix;
    mat4 colorModulate;
    mat4 colorAdd;
end_layout_binding_0_uniform(interactionShadowMappingParallelLight)

VERTEX_UBO_ALIAS_DEFINE(interactionShadowMappingBlinnPhongParallelLight, interactionShadowMappingParallelLight)

VERTEX_UBO_ALIAS_DEFINE(interactionShadowMappingPBRParallelLight, interactionShadowMappingParallelLight)

VERTEX_UBO_ALIAS_DEFINE(ambientLightingShadowMappingParallelLight, interactionShadowMappingParallelLight)


layout_binding_0_uniform(interactionShadowMappingSpotLight)
    vec4 lightProjectionS;
    vec4 lightProjectionT;
    vec4 lightFalloff;
    vec4 lightProjectionQ;

    vec4 lightOrigin;
    vec4 viewOrigin;

    vec4 bumpMatrixS;
    vec4 bumpMatrixT;
    vec4 diffuseMatrixS;
    vec4 diffuseMatrixT;
    vec4 specularMatrixS;
    vec4 specularMatrixT;

    mat4 modelViewProjectionMatrix;

    mat4 modelMatrix;
    vec4 globalLightOrigin;
    mat4 shadowMVPMatrix;
    mat4 colorModulate;
    mat4 colorAdd;
end_layout_binding_0_uniform(interactionShadowMappingSpotLight)

VERTEX_UBO_ALIAS_DEFINE(interactionShadowMappingBlinnPhongSpotLight, interactionShadowMappingSpotLight)

VERTEX_UBO_ALIAS_DEFINE(interactionShadowMappingPBRSpotLight, interactionShadowMappingSpotLight)

VERTEX_UBO_ALIAS_DEFINE(ambientLightingShadowMappingSpotLight, interactionShadowMappingSpotLight)


layout_binding_1_uniform(interactionShadowMappingPointLight)
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 globalLightOrigin;
    vec4 uniformParm1;
    vec4 uniformParm2;
    mat4 shadowMVPMatrix[6];
    vec4 specularExponent;
    mat4 uniformParm0;
    mat4 uniformParm4;
end_layout_binding_1_uniform(interactionShadowMappingPointLight)

FRAGMENT_UBO_ALIAS_DEFINE(interactionShadowMappingBlinnPhongPointLight, interactionShadowMappingPointLight)

FRAGMENT_UBO_ALIAS_DEFINE(interactionShadowMappingPBRPointLight, interactionShadowMappingPointLight)

FRAGMENT_UBO_ALIAS_DEFINE(ambientLightingShadowMappingPointLight, interactionShadowMappingPointLight)

layout_binding_1_uniform(interactionShadowMappingParallelLight)
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 globalLightOrigin;
    vec4 uniformParm1;
    vec4 uniformParm2;
    vec4 uniformParm3;
    mat4 shadowMVPMatrix[6];
    vec4 specularExponent;
    mat4 uniformParm0;
    mat4 uniformParm4;
end_layout_binding_1_uniform(interactionShadowMappingParallelLight)

FRAGMENT_UBO_ALIAS_DEFINE(interactionShadowMappingBlinnPhongParallelLight, interactionShadowMappingParallelLight)

FRAGMENT_UBO_ALIAS_DEFINE(interactionShadowMappingPBRParallelLight, interactionShadowMappingParallelLight)

FRAGMENT_UBO_ALIAS_DEFINE(ambientLightingShadowMappingParallelLight, interactionShadowMappingParallelLight)

layout_binding_1_uniform(interactionShadowMappingSpotLight)
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 globalLightOrigin;
    vec4 uniformParm1;
    vec4 uniformParm2;
    vec4 specularExponent;
    vec4 uniformParm0;
    vec4 uniformParm4;
end_layout_binding_1_uniform(interactionShadowMappingSpotLight)

FRAGMENT_UBO_ALIAS_DEFINE(interactionShadowMappingBlinnPhongSpotLight, interactionShadowMappingSpotLight)

FRAGMENT_UBO_ALIAS_DEFINE(interactionShadowMappingPBRSpotLight, interactionShadowMappingSpotLight)

FRAGMENT_UBO_ALIAS_DEFINE(ambientLightingShadowMappingSpotLight, interactionShadowMappingSpotLight)


layout_binding_0_uniform(megaTexture)
    mat4 modelViewProjectionMatrix;
    vec4 megaTextureLevel0;
    vec4 megaTextureLevel1;
    vec4 megaTextureLevel2;
    vec4 megaTextureLevel3;
    vec4 megaTextureLevel4;
    vec4 megaTextureLevel5;
    vec4 megaTextureLevel6;
    vec4 megaTextureLevel7;
    vec4 megaTextureLevel8;
end_layout_binding_0_uniform(megaTexture)


layout_binding_0_uniform(globalIllumination)
    mat4 modelViewProjectionMatrix;
    vec4 viewOrigin;
    vec4 bumpMatrixS;
    vec4 bumpMatrixT;
    vec4 diffuseMatrixS;
    vec4 diffuseMatrixT;
    vec4 specularMatrixS;
    vec4 specularMatrixT;
    float colorModulate;
    float colorAdd;
end_layout_binding_0_uniform(globalIllumination)

layout_binding_1_uniform(globalIllumination)
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 specularExponent;
end_layout_binding_1_uniform(globalIllumination)


layout_binding_1_uniform(retro2Bit)
    vec4 windowCoords;
    vec4 nonPowerOfTwo;
    vec4 uniformParm0;
end_layout_binding_1_uniform(retro2Bit)


layout_binding_1_uniform(retroC64)
    vec4 windowCoords;
    vec4 nonPowerOfTwo;
    vec4 uniformParm0;
end_layout_binding_1_uniform(retroC64)


layout_binding_1_uniform(retroCPC)
    vec4 windowCoords;
    vec4 nonPowerOfTwo;
    vec4 uniformParm0;
end_layout_binding_1_uniform(retroCPC)


layout_binding_1_uniform(retroGenesis)
    vec4 windowCoords;
    vec4 nonPowerOfTwo;
    vec4 uniformParm0;
end_layout_binding_1_uniform(retroGenesis)


layout_binding_1_uniform(retroPS1)
    vec4 windowCoords;
    vec4 nonPowerOfTwo;
end_layout_binding_1_uniform(retroPS1)
