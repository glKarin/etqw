
#define UBOPTR_TEMP(stage) void *ptr = vk_backend.currentProgram->ubo_##stage
#define UBO_TYPEDEF(stage, T) typedef UBO_DEFINE(stage, T) Type_t

#define UBOX_TRANSFORM_DECLARATION(stage, T) UBOX_TRANSFORM_DEFINE(stage, T) { \
	ASSERT_PROGRAM; \
	UBOPTR_TEMP(stage); \
	UBO_TYPEDEF(stage, T);
#define UBO0_TRANSFORM_DECLARATION(T) UBOX_TRANSFORM_DECLARATION(vertex, T)
#define UBO1_TRANSFORM_DECLARATION(T) UBOX_TRANSFORM_DECLARATION(fragment, T)
#define END_UBO_TRANSFORM }


#define UBOTYPE_OFFSET(name) offsetof(Type_t, name)
#define UBOTYPE_OFFSET_N(name, n) offsetof(Type_t, name##n)

#define UBOXCPY(name) R_VK_TransformUniform(UBOTYPE_OFFSET(name), VKSHADER_PARM_ADDR(name), ptr, sizeof(globalUBO.name))
#define UBOXCPYN(name, n) R_VK_TransformUniform(UBOTYPE_OFFSET_N(name, n), SHADER_PARM_N_ADDR(name, n), ptr, sizeof(globalUBO.name[n]))
#define UBOXCPY1F(name) R_VK_TransformUniform1f(UBOTYPE_OFFSET(name), VKSHADER_PARM_ADDR(name), ptr)
#define UBOXCPY4F(name) R_VK_TransformUniform4f(UBOTYPE_OFFSET(name), VKSHADER_PARM_ADDR(name), ptr)
#define UBOXCPYM4(name) R_VK_TransformUniformMatrix4f(UBOTYPE_OFFSET(name), VKSHADER_PARM_ADDR(name), ptr)
#define UBOXCPYM4N(name) UBOXCPY(name)
#define UBOXCPYN1I(name, n) R_VK_TransformUniform1i(UBOTYPE_OFFSET_N(name, n), VKSHADER_PARM_ADDR(name), ptr)
#define UBOXCPYN4F(name, n) R_VK_TransformUniform4f(UBOTYPE_OFFSET_N(name, n), VKSHADER_PARM_ADDR(name), ptr)


UBO0_TRANSFORM_DECLARATION(default)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(textureMatrix);
	UBOXCPY1F(colorModulate);
	UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(default)
	UBOXCPY4F(glColor);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(zfill)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(textureMatrix);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(zfill)
	UBOXCPY4F(glColor);
	UBOXCPY1F(alphaTest);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(zfillClip)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(textureMatrix);
	UBOXCPY4F(clipPlane);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(zfillClip)
	UBOXCPY4F(glColor);
	UBOXCPY1F(alphaTest);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(fog)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(fogMatrix);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(fog)
	UBOXCPY4F(fogColor);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(blendLight)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(fogMatrix);
END_UBO_TRANSFORM

#define R_VK_TransformUniform0_fragment_blendLight R_VK_TransformUniform0_fragment_fog


UBO0_TRANSFORM_DECLARATION(shadow)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPY4F(glColor);
	UBOXCPY4F(lightOrigin);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(texgen)
	UBOXCPY4F(texgenS);
	UBOXCPY4F(texgenT);
	UBOXCPY4F(texgenQ);
	UBOXCPYM4(textureMatrix);
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPY1F(colorModulate);
	UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(texgen)
	UBOXCPY4F(glColor);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(simpleVertexTexcoord)
	UBOXCPYM4(modelViewProjectionMatrix);
END_UBO_TRANSFORM


UBO1_TRANSFORM_DECLARATION(stencilToColor)
	UBOXCPYN1I(iuniformParm, 0);
END_UBO_TRANSFORM


UBO1_TRANSFORM_DECLARATION(depthToColor)
	UBOXCPYN1I(buniformParm, 0);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(cubemap)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(textureMatrix);
	UBOXCPY4F(viewOrigin);
	UBOXCPY1F(colorModulate);
	UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(cubemap)
	UBOXCPY4F(glColor);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(diffuseCubemap)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(textureMatrix);
	UBOXCPY1F(colorModulate);
	UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

#define R_VK_TransformUniform0_fragment_diffuseCubemap R_VK_TransformUniform0_fragment_cubemap


UBO0_TRANSFORM_DECLARATION(bumpyEnvironment)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(modelMatrix);
	UBOXCPY4F(viewOrigin);
	UBOXCPY1F(colorModulate);
	UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(bumpyEnvironment)
	UBOXCPY4F(glColor);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(colorProcess)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYN4F(vertexParm, 0);
	UBOXCPYN4F(vertexParm, 1);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(colorProcess)
	UBOXCPY4F(nonPowerOfTwo);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(depthPerforated)
	UBOXCPYM4(textureMatrix);
	UBOXCPYM4(modelViewProjectionMatrix);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(depthPerforated)
	UBOXCPY4F(glColor);
	UBOXCPY1F(alphaTest);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(depth)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPY4F(lightOrigin);
	//UBOXCPY4F(uniformParm0);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(heatHaze)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(modelViewMatrix);
	UBOXCPYM4(projectionMatrix);
	UBOXCPYN4F(vertexParm, 0);
	UBOXCPYN4F(vertexParm, 1);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(heatHaze)
	UBOXCPY4F(nonPowerOfTwo);
	UBOXCPY4F(windowCoords);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(heatHazeWithMask)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(modelViewMatrix);
	UBOXCPYM4(projectionMatrix);
	UBOXCPYN4F(vertexParm, 0);
	UBOXCPYN4F(vertexParm, 1);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(heatHazeWithMask)
	UBOXCPY4F(nonPowerOfTwo);
	UBOXCPY4F(windowCoords);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(heatHazeWithMaskAndVertex)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(modelViewMatrix);
	UBOXCPYM4(projectionMatrix);
	UBOXCPYN4F(vertexParm, 0);
	UBOXCPYN4F(vertexParm, 1);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(heatHazeWithMaskAndVertex)
	UBOXCPY4F(nonPowerOfTwo);
	UBOXCPY4F(windowCoords);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(environment)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(modelViewMatrix);
	UBOXCPYM4(textureMatrix);
	UBOXCPY4F(viewOrigin);
	UBOXCPY1F(colorModulate);
	UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(environment)
	UBOXCPY4F(glColor);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(interaction)
	UBOXCPY4F(lightProjectionS);
	UBOXCPY4F(lightProjectionT);
	UBOXCPY4F(lightFalloff);
	UBOXCPY4F(lightProjectionQ);
	UBOXCPY4F(lightOrigin);
	UBOXCPY4F(viewOrigin);
	UBOXCPY4F(bumpMatrixS);
	UBOXCPY4F(bumpMatrixT);
	UBOXCPY4F(diffuseMatrixS);
	UBOXCPY4F(diffuseMatrixT);
	UBOXCPY4F(specularMatrixS);
	UBOXCPY4F(specularMatrixT);
	UBOXCPYM4(modelViewProjectionMatrix);
#if defined(_AMBIENT)
	UBOXCPYM4(modelMatrix);
#endif
	UBOXCPY1F(colorModulate);
    UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(interaction)
	UBOXCPY4F(diffuseColor);
	UBOXCPY4F(specularColor);
	UBOXCPY4F(specularExponent);
END_UBO_TRANSFORM


#define R_VK_TransformUniform0_vertex_interactionBlinnPhong R_VK_TransformUniform0_vertex_interaction
#define R_VK_TransformUniform0_fragment_interactionBlinnPhong R_VK_TransformUniform0_fragment_interaction


#define R_VK_TransformUniform0_vertex_ambientLighting R_VK_TransformUniform0_vertex_interaction
#define R_VK_TransformUniform0_fragment_ambientLighting R_VK_TransformUniform0_fragment_interaction


#define R_VK_TransformUniform0_vertex_interactionPBR R_VK_TransformUniform0_vertex_interaction
#define R_VK_TransformUniform0_fragment_interactionPBR R_VK_TransformUniform0_fragment_interaction


UBO0_TRANSFORM_DECLARATION(interactionTranslucent)
	UBOXCPY4F(lightProjectionS);
	UBOXCPY4F(lightProjectionT);
	UBOXCPY4F(lightFalloff);
	UBOXCPY4F(lightProjectionQ);
	UBOXCPY4F(lightOrigin);
	UBOXCPY4F(viewOrigin);
	UBOXCPY4F(bumpMatrixS);
	UBOXCPY4F(bumpMatrixT);
	UBOXCPY4F(diffuseMatrixS);
	UBOXCPY4F(diffuseMatrixT);
	UBOXCPY4F(specularMatrixS);
	UBOXCPY4F(specularMatrixT);
	UBOXCPYM4(modelViewProjectionMatrix);
#if defined(_AMBIENT)
	UBOXCPYM4(modelMatrix);
#endif
	UBOXCPY1F(colorModulate);
    UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(interactionTranslucent)
	UBOXCPY4F(diffuseColor);
	UBOXCPY4F(specularColor);
	UBOXCPY4F(specularExponent);
	UBOXCPYN4F(uniformParm, 0);
END_UBO_TRANSFORM


#define R_VK_TransformUniform0_vertex_interactionBlinnPhongTranslucent R_VK_TransformUniform0_vertex_interactionTranslucent
#define R_VK_TransformUniform0_fragment_interactionBlinnPhongTranslucent R_VK_TransformUniform0_fragment_interactionTranslucent

#define R_VK_TransformUniform0_vertex_interactionPBRTranslucent R_VK_TransformUniform0_vertex_interactionTranslucent
#define R_VK_TransformUniform0_fragment_interactionPBRTranslucent R_VK_TransformUniform0_fragment_interactionTranslucent

#define R_VK_TransformUniform0_vertex_ambientLightingTranslucent R_VK_TransformUniform0_vertex_interactionTranslucent
#define R_VK_TransformUniform0_fragment_ambientLightingTranslucent R_VK_TransformUniform0_fragment_interactionTranslucent


UBO1_TRANSFORM_DECLARATION(interactionSoft)
	UBOXCPY4F(diffuseColor);
	UBOXCPY4F(specularColor);
	UBOXCPY4F(nonPowerOfTwo);
	UBOXCPY4F(windowCoords);
	UBOXCPY4F(specularExponent);
	UBOXCPYN4F(uniformParm, 0);
	UBOXCPYN4F(uniformParm, 1);
END_UBO_TRANSFORM

#define R_VK_TransformUniform0_vertex_interactionSoft R_VK_TransformUniform0_vertex_interactionTranslucent

#define R_VK_TransformUniform0_vertex_interactionBlinnPhongSoft R_VK_TransformUniform0_vertex_interactionTranslucent
#define R_VK_TransformUniform0_fragment_interactionBlinnPhongSoft R_VK_TransformUniform0_fragment_interactionSoft

#define R_VK_TransformUniform0_vertex_interactionPBRSoft R_VK_TransformUniform0_vertex_interactionTranslucent
#define R_VK_TransformUniform0_fragment_interactionPBRSoft R_VK_TransformUniform0_fragment_interactionSoft

#define R_VK_TransformUniform0_vertex_ambientLightingSoft R_VK_TransformUniform0_vertex_interactionTranslucent
#define R_VK_TransformUniform0_fragment_ambientLightingSoft R_VK_TransformUniform0_fragment_interactionSoft


UBO0_TRANSFORM_DECLARATION(interactionShadowMappingPointLight)
	UBOXCPY4F(lightProjectionS);
	UBOXCPY4F(lightProjectionT);
	UBOXCPY4F(lightFalloff);
	UBOXCPY4F(lightProjectionQ);
	UBOXCPY4F(lightOrigin);
	UBOXCPY4F(viewOrigin);
	UBOXCPY4F(bumpMatrixS);
	UBOXCPY4F(bumpMatrixT);
	UBOXCPY4F(diffuseMatrixS);
	UBOXCPY4F(diffuseMatrixT);
	UBOXCPY4F(specularMatrixS);
	UBOXCPY4F(specularMatrixT);
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(modelMatrix);
	UBOXCPY4F(globalLightOrigin);
	UBOXCPY1F(colorModulate);
    UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(interactionShadowMappingPointLight)
	UBOXCPY4F(diffuseColor);
	UBOXCPY4F(specularColor);
	UBOXCPY4F(globalLightOrigin);
	UBOXCPYN4F(uniformParm, 1);
	UBOXCPYN4F(uniformParm, 2);
	UBOXCPYM4N(shadowMVPMatrix);
	UBOXCPY4F(specularExponent);
	UBOXCPYN4F(uniformParm, 0);
	UBOXCPYN4F(uniformParm, 4);
END_UBO_TRANSFORM


#define R_VK_TransformUniform0_vertex_interactionShadowMappingBlinnPhongPointLight R_VK_TransformUniform0_vertex_interactionShadowMappingPointLight
#define R_VK_TransformUniform0_fragment_interactionShadowMappingBlinnPhongPointLight R_VK_TransformUniform0_fragment_interactionShadowMappingPointLight

#define R_VK_TransformUniform0_vertex_interactionShadowMappingPBRPointLight R_VK_TransformUniform0_vertex_interactionShadowMappingPointLight
#define R_VK_TransformUniform0_fragment_interactionShadowMappingPBRPointLight R_VK_TransformUniform0_fragment_interactionShadowMappingPointLight

#define R_VK_TransformUniform0_vertex_ambientLightingShadowMappingPointLight R_VK_TransformUniform0_vertex_interactionShadowMappingPointLight
#define R_VK_TransformUniform0_fragment_ambientLightingShadowMappingPointLight R_VK_TransformUniform0_fragment_interactionShadowMappingPointLight


UBO0_TRANSFORM_DECLARATION(interactionShadowMappingParallelLight)
	UBOXCPY4F(lightProjectionS);
	UBOXCPY4F(lightProjectionT);
	UBOXCPY4F(lightFalloff);
	UBOXCPY4F(lightProjectionQ);
	UBOXCPY4F(lightOrigin);
	UBOXCPY4F(viewOrigin);
	UBOXCPY4F(bumpMatrixS);
	UBOXCPY4F(bumpMatrixT);
	UBOXCPY4F(diffuseMatrixS);
	UBOXCPY4F(diffuseMatrixT);
	UBOXCPY4F(specularMatrixS);
	UBOXCPY4F(specularMatrixT);
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(modelMatrix);
	UBOXCPY4F(globalLightOrigin);
	UBOXCPYM4(modelViewMatrix);
	UBOXCPY1F(colorModulate);
    UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(interactionShadowMappingParallelLight)
	UBOXCPY4F(diffuseColor);
	UBOXCPY4F(specularColor);
	UBOXCPY4F(globalLightOrigin);
	UBOXCPYN4F(uniformParm, 1);
	UBOXCPYN4F(uniformParm, 2);
	UBOXCPYN4F(uniformParm, 3);
	UBOXCPYM4N(shadowMVPMatrix);
	UBOXCPY4F(specularExponent);
	UBOXCPYN4F(uniformParm, 0);
	UBOXCPYN4F(uniformParm, 4);
END_UBO_TRANSFORM


#define R_VK_TransformUniform0_vertex_interactionShadowMappingBlinnPhongParallelLight R_VK_TransformUniform0_vertex_interactionShadowMappingParallelLight
#define R_VK_TransformUniform0_fragment_interactionShadowMappingBlinnPhongParallelLight R_VK_TransformUniform0_fragment_interactionShadowMappingParallelLight

#define R_VK_TransformUniform0_vertex_interactionShadowMappingPBRParallelLight R_VK_TransformUniform0_vertex_interactionShadowMappingParallelLight
#define R_VK_TransformUniform0_fragment_interactionShadowMappingPBRParallelLight R_VK_TransformUniform0_fragment_interactionShadowMappingParallelLight

#define R_VK_TransformUniform0_vertex_ambientLightingShadowMappingParallelLight R_VK_TransformUniform0_vertex_interactionShadowMappingParallelLight
#define R_VK_TransformUniform0_fragment_ambientLightingShadowMappingParallelLight R_VK_TransformUniform0_fragment_interactionShadowMappingParallelLight


UBO0_TRANSFORM_DECLARATION(interactionShadowMappingSpotLight)
	UBOXCPY4F(lightProjectionS);
	UBOXCPY4F(lightProjectionT);
	UBOXCPY4F(lightFalloff);
	UBOXCPY4F(lightProjectionQ);
	UBOXCPY4F(lightOrigin);
	UBOXCPY4F(viewOrigin);
	UBOXCPY4F(bumpMatrixS);
	UBOXCPY4F(bumpMatrixT);
	UBOXCPY4F(diffuseMatrixS);
	UBOXCPY4F(diffuseMatrixT);
	UBOXCPY4F(specularMatrixS);
	UBOXCPY4F(specularMatrixT);
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYM4(modelMatrix);
	UBOXCPY4F(globalLightOrigin);
	UBOXCPYM4N(shadowMVPMatrix);
	UBOXCPY1F(colorModulate);
    UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(interactionShadowMappingSpotLight)
	UBOXCPY4F(diffuseColor);
	UBOXCPY4F(specularColor);
	UBOXCPY4F(globalLightOrigin);
	UBOXCPYN4F(uniformParm, 1);
	UBOXCPYN4F(uniformParm, 2);
	UBOXCPY4F(specularExponent);
	UBOXCPYN4F(uniformParm, 0);
	UBOXCPYN4F(uniformParm, 4);
END_UBO_TRANSFORM

#define R_VK_TransformUniform0_vertex_interactionShadowMappingBlinnPhongSpotLight R_VK_TransformUniform0_vertex_interactionShadowMappingSpotLight
#define R_VK_TransformUniform0_fragment_interactionShadowMappingBlinnPhongSpotLight R_VK_TransformUniform0_fragment_interactionShadowMappingSpotLight

#define R_VK_TransformUniform0_vertex_interactionShadowMappingPBRSpotLight R_VK_TransformUniform0_vertex_interactionShadowMappingSpotLight
#define R_VK_TransformUniform0_fragment_interactionShadowMappingPBRSpotLight R_VK_TransformUniform0_fragment_interactionShadowMappingSpotLight

#define R_VK_TransformUniform0_vertex_ambientLightingShadowMappingSpotLight R_VK_TransformUniform0_vertex_interactionShadowMappingSpotLight
#define R_VK_TransformUniform0_fragment_ambientLightingShadowMappingSpotLight R_VK_TransformUniform0_fragment_interactionShadowMappingSpotLight


UBO0_TRANSFORM_DECLARATION(megaTexture)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPYN4F(megaTextureLevel, 0);
	UBOXCPYN4F(megaTextureLevel, 1);
	UBOXCPYN4F(megaTextureLevel, 2);
	UBOXCPYN4F(megaTextureLevel, 3);
	UBOXCPYN4F(megaTextureLevel, 4);
	UBOXCPYN4F(megaTextureLevel, 5);
	UBOXCPYN4F(megaTextureLevel, 6);
	UBOXCPYN4F(megaTextureLevel, 7);
	UBOXCPYN4F(megaTextureLevel, 8);
END_UBO_TRANSFORM


UBO0_TRANSFORM_DECLARATION(globalIllumination)
	UBOXCPYM4(modelViewProjectionMatrix);
	UBOXCPY4F(viewOrigin);
	UBOXCPY4F(bumpMatrixS);
	UBOXCPY4F(bumpMatrixT);
	UBOXCPY4F(diffuseMatrixS);
	UBOXCPY4F(diffuseMatrixT);
	UBOXCPY4F(specularMatrixS);
	UBOXCPY4F(specularMatrixT);
	UBOXCPY1F(colorModulate);
    UBOXCPY1F(colorAdd);
END_UBO_TRANSFORM

UBO1_TRANSFORM_DECLARATION(globalIllumination)
	UBOXCPY4F(diffuseColor);
	UBOXCPY4F(specularColor);
	UBOXCPY4F(specularExponent);
END_UBO_TRANSFORM


UBO1_TRANSFORM_DECLARATION(retro2Bit)
	UBOXCPY4F(windowCoords);
	UBOXCPY4F(nonPowerOfTwo);
	UBOXCPYN4F(uniformParm, 0);
END_UBO_TRANSFORM


UBO1_TRANSFORM_DECLARATION(retroC64)
	UBOXCPY4F(windowCoords);
	UBOXCPY4F(nonPowerOfTwo);
	UBOXCPYN4F(uniformParm, 0);
END_UBO_TRANSFORM


UBO1_TRANSFORM_DECLARATION(retroCPC)
	UBOXCPY4F(windowCoords);
	UBOXCPY4F(nonPowerOfTwo);
	UBOXCPYN4F(uniformParm, 0);
END_UBO_TRANSFORM


UBO1_TRANSFORM_DECLARATION(retroGenesis)
	UBOXCPY4F(windowCoords);
	UBOXCPY4F(nonPowerOfTwo);
	UBOXCPYN4F(uniformParm, 0);
END_UBO_TRANSFORM


UBO1_TRANSFORM_DECLARATION(retroPS1)
	UBOXCPY4F(windowCoords);
	UBOXCPY4F(nonPowerOfTwo);
END_UBO_TRANSFORM

