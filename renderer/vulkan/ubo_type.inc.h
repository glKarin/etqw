#if !defined(UBO_VERTEX_PROC_BEGIN) || !defined(UBO_FRAGMENT_PROC_BEGIN) || !defined(UBO_VERTEX_PROC_END) || !defined(UBO_FRAGMENT_PROC_END) || !defined(UBO_MAT4_PROC) || !defined(UBO_FLOAT_PROC) || !defined(UBO_VEC4_PROC) || !defined(UBO_INT_N_PROC) || !defined(UBO_VEC4_N_PROC) || !defined(UBO_MAT4_N_PROC)
#error "you must define UBO_VERTEX_PROC_BEGIN/UBO_FRAGMENT_PROC_BEGIN/UBO_VERTEX_PROC_END/UBO_FRAGMENT_PROC_END/UBO_MAT4_PROC/UBO_FLOAT_PROC/UBO_VEC4_PROC/UBO_INT_N_PROC/UBO_VEC4_N_PROC/UBO_MAT4_N_PROC before including this file"
#endif


UBO_VERTEX_PROC_BEGIN(default)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(textureMatrix);
	UBO_FLOAT_PROC(colorModulate);
	UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(default)

UBO_FRAGMENT_PROC_BEGIN(default)
	UBO_VEC4_PROC(glColor);
UBO_FRAGMENT_PROC_END(default)


UBO_VERTEX_PROC_BEGIN(zfill)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(textureMatrix);
UBO_VERTEX_PROC_END(zfill)

UBO_FRAGMENT_PROC_BEGIN(zfill)
	UBO_VEC4_PROC(glColor);
	UBO_FLOAT_PROC(alphaTest);
UBO_FRAGMENT_PROC_END(zfill)


UBO_VERTEX_PROC_BEGIN(zfillClip)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(textureMatrix);
	UBO_VEC4_PROC(clipPlane);
UBO_VERTEX_PROC_END(zfillClip)

UBO_FRAGMENT_PROC_BEGIN(zfillClip)
	UBO_VEC4_PROC(glColor);
	UBO_FLOAT_PROC(alphaTest);
UBO_FRAGMENT_PROC_END(zfillClip)


UBO_VERTEX_PROC_BEGIN(fog)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(fogMatrix);
UBO_VERTEX_PROC_END(fog)

UBO_FRAGMENT_PROC_BEGIN(fog)
	UBO_VEC4_PROC(fogColor);
UBO_FRAGMENT_PROC_END(fog)


UBO_VERTEX_PROC_BEGIN(blendLight)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(fogMatrix);
UBO_VERTEX_PROC_END(blendLight)



UBO_VERTEX_PROC_BEGIN(shadow)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_VEC4_PROC(glColor);
	UBO_VEC4_PROC(lightOrigin);
UBO_VERTEX_PROC_END(shadow)


UBO_VERTEX_PROC_BEGIN(texgen)
	UBO_VEC4_PROC(texgenS);
	UBO_VEC4_PROC(texgenT);
	UBO_VEC4_PROC(texgenQ);
	UBO_MAT4_PROC(textureMatrix);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_FLOAT_PROC(colorModulate);
	UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(texgen)

UBO_FRAGMENT_PROC_BEGIN(texgen)
	UBO_VEC4_PROC(glColor);
UBO_FRAGMENT_PROC_END(texgen)


UBO_VERTEX_PROC_BEGIN(simpleVertexTexcoord)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
UBO_VERTEX_PROC_END(simpleVertexTexcoord)


UBO_FRAGMENT_PROC_BEGIN(stencilToColor)
	UBO_INT_N_PROC(iuniformParm, 0);
UBO_FRAGMENT_PROC_END(stencilToColor)


UBO_FRAGMENT_PROC_BEGIN(depthToColor)
	UBO_INT_N_PROC(buniformParm, 0);
UBO_FRAGMENT_PROC_END(depthToColor)


UBO_VERTEX_PROC_BEGIN(cubemap)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(textureMatrix);
	UBO_VEC4_PROC(viewOrigin);
	UBO_FLOAT_PROC(colorModulate);
	UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(cubemap)

UBO_FRAGMENT_PROC_BEGIN(cubemap)
	UBO_VEC4_PROC(glColor);
UBO_FRAGMENT_PROC_END(cubemap)


UBO_VERTEX_PROC_BEGIN(diffuseCubemap)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(textureMatrix);
	UBO_FLOAT_PROC(colorModulate);
	UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(diffuseCubemap)



UBO_VERTEX_PROC_BEGIN(bumpyEnvironment)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelMatrix);
	UBO_VEC4_PROC(viewOrigin);
	UBO_FLOAT_PROC(colorModulate);
	UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(bumpyEnvironment)

UBO_FRAGMENT_PROC_BEGIN(bumpyEnvironment)
	UBO_VEC4_PROC(glColor);
UBO_FRAGMENT_PROC_END(bumpyEnvironment)


UBO_VERTEX_PROC_BEGIN(colorProcess)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_VEC4_N_PROC(vertexParm, 0);
	UBO_VEC4_N_PROC(vertexParm, 1);
UBO_VERTEX_PROC_END(colorProcess)

UBO_FRAGMENT_PROC_BEGIN(colorProcess)
	UBO_VEC4_PROC(nonPowerOfTwo);
UBO_FRAGMENT_PROC_END(colorProcess)


UBO_VERTEX_PROC_BEGIN(depthPerforated)
	UBO_MAT4_PROC(textureMatrix);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
UBO_VERTEX_PROC_END(depthPerforated)

UBO_FRAGMENT_PROC_BEGIN(depthPerforated)
	UBO_VEC4_PROC(glColor);
	UBO_FLOAT_PROC(alphaTest);
UBO_FRAGMENT_PROC_END(depthPerforated)


UBO_VERTEX_PROC_BEGIN(depth)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_VEC4_PROC(lightOrigin);
	//UBO_VEC4_PROC(uniformParm0);
UBO_VERTEX_PROC_END(depth)


UBO_VERTEX_PROC_BEGIN(heatHaze)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelViewMatrix);
	UBO_MAT4_PROC(projectionMatrix);
	UBO_VEC4_N_PROC(vertexParm, 0);
	UBO_VEC4_N_PROC(vertexParm, 1);
UBO_VERTEX_PROC_END(heatHaze)

UBO_FRAGMENT_PROC_BEGIN(heatHaze)
	UBO_VEC4_PROC(nonPowerOfTwo);
	UBO_VEC4_PROC(windowCoords);
UBO_FRAGMENT_PROC_END(heatHaze)


UBO_VERTEX_PROC_BEGIN(heatHazeWithMask)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelViewMatrix);
	UBO_MAT4_PROC(projectionMatrix);
	UBO_VEC4_N_PROC(vertexParm, 0);
	UBO_VEC4_N_PROC(vertexParm, 1);
UBO_VERTEX_PROC_END(heatHazeWithMask)

UBO_FRAGMENT_PROC_BEGIN(heatHazeWithMask)
	UBO_VEC4_PROC(nonPowerOfTwo);
	UBO_VEC4_PROC(windowCoords);
UBO_FRAGMENT_PROC_END(heatHazeWithMask)


UBO_VERTEX_PROC_BEGIN(heatHazeWithMaskAndVertex)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelViewMatrix);
	UBO_MAT4_PROC(projectionMatrix);
	UBO_VEC4_N_PROC(vertexParm, 0);
	UBO_VEC4_N_PROC(vertexParm, 1);
UBO_VERTEX_PROC_END(heatHazeWithMaskAndVertex)

UBO_FRAGMENT_PROC_BEGIN(heatHazeWithMaskAndVertex)
	UBO_VEC4_PROC(nonPowerOfTwo);
	UBO_VEC4_PROC(windowCoords);
UBO_FRAGMENT_PROC_END(heatHazeWithMaskAndVertex)


UBO_VERTEX_PROC_BEGIN(environment)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelViewMatrix);
	UBO_MAT4_PROC(textureMatrix);
	UBO_VEC4_PROC(viewOrigin);
	UBO_FLOAT_PROC(colorModulate);
	UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(environment)

UBO_FRAGMENT_PROC_BEGIN(environment)
	UBO_VEC4_PROC(glColor);
UBO_FRAGMENT_PROC_END(environment)


UBO_VERTEX_PROC_BEGIN(interaction)
	UBO_VEC4_PROC(lightProjectionS);
	UBO_VEC4_PROC(lightProjectionT);
	UBO_VEC4_PROC(lightFalloff);
	UBO_VEC4_PROC(lightProjectionQ);
	UBO_VEC4_PROC(lightOrigin);
	UBO_VEC4_PROC(viewOrigin);
	UBO_VEC4_PROC(bumpMatrixS);
	UBO_VEC4_PROC(bumpMatrixT);
	UBO_VEC4_PROC(diffuseMatrixS);
	UBO_VEC4_PROC(diffuseMatrixT);
	UBO_VEC4_PROC(specularMatrixS);
	UBO_VEC4_PROC(specularMatrixT);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_FLOAT_PROC(colorModulate);
    UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(interaction)

UBO_FRAGMENT_PROC_BEGIN(interaction)
	UBO_VEC4_PROC(diffuseColor);
	UBO_VEC4_PROC(specularColor);
	UBO_VEC4_PROC(specularExponent);
UBO_FRAGMENT_PROC_END(interaction)

UBO_VERTEX_PROC_BEGIN(ambientLighting)
	UBO_VEC4_PROC(lightProjectionS);
	UBO_VEC4_PROC(lightProjectionT);
	UBO_VEC4_PROC(lightFalloff);
	UBO_VEC4_PROC(lightProjectionQ);
	UBO_VEC4_PROC(lightOrigin);
	UBO_VEC4_PROC(viewOrigin);
	UBO_VEC4_PROC(bumpMatrixS);
	UBO_VEC4_PROC(bumpMatrixT);
	UBO_VEC4_PROC(diffuseMatrixS);
	UBO_VEC4_PROC(diffuseMatrixT);
	UBO_VEC4_PROC(specularMatrixS);
	UBO_VEC4_PROC(specularMatrixT);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelMatrix);
	UBO_FLOAT_PROC(colorModulate);
    UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(ambientLighting)



UBO_VERTEX_PROC_BEGIN(interactionTranslucent)
	UBO_VEC4_PROC(lightProjectionS);
	UBO_VEC4_PROC(lightProjectionT);
	UBO_VEC4_PROC(lightFalloff);
	UBO_VEC4_PROC(lightProjectionQ);
	UBO_VEC4_PROC(lightOrigin);
	UBO_VEC4_PROC(viewOrigin);
	UBO_VEC4_PROC(bumpMatrixS);
	UBO_VEC4_PROC(bumpMatrixT);
	UBO_VEC4_PROC(diffuseMatrixS);
	UBO_VEC4_PROC(diffuseMatrixT);
	UBO_VEC4_PROC(specularMatrixS);
	UBO_VEC4_PROC(specularMatrixT);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_FLOAT_PROC(colorModulate);
    UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(interactionTranslucent)

UBO_FRAGMENT_PROC_BEGIN(interactionTranslucent)
	UBO_VEC4_PROC(diffuseColor);
	UBO_VEC4_PROC(specularColor);
	UBO_VEC4_PROC(specularExponent);
	UBO_VEC4_N_PROC(uniformParm, 0);
UBO_FRAGMENT_PROC_END(interactionTranslucent)

UBO_VERTEX_PROC_BEGIN(ambientLightingTranslucent)
	UBO_VEC4_PROC(lightProjectionS);
	UBO_VEC4_PROC(lightProjectionT);
	UBO_VEC4_PROC(lightFalloff);
	UBO_VEC4_PROC(lightProjectionQ);
	UBO_VEC4_PROC(lightOrigin);
	UBO_VEC4_PROC(viewOrigin);
	UBO_VEC4_PROC(bumpMatrixS);
	UBO_VEC4_PROC(bumpMatrixT);
	UBO_VEC4_PROC(diffuseMatrixS);
	UBO_VEC4_PROC(diffuseMatrixT);
	UBO_VEC4_PROC(specularMatrixS);
	UBO_VEC4_PROC(specularMatrixT);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelMatrix);
	UBO_FLOAT_PROC(colorModulate);
    UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(ambientLightingTranslucent)



UBO_FRAGMENT_PROC_BEGIN(interactionSoft)
	UBO_VEC4_PROC(diffuseColor);
	UBO_VEC4_PROC(specularColor);
	UBO_VEC4_PROC(nonPowerOfTwo);
	UBO_VEC4_PROC(windowCoords);
	UBO_VEC4_PROC(specularExponent);
	UBO_VEC4_N_PROC(uniformParm, 0);
	UBO_VEC4_N_PROC(uniformParm, 1);
UBO_FRAGMENT_PROC_END(interactionSoft)



UBO_VERTEX_PROC_BEGIN(interactionShadowMappingPointLight)
	UBO_VEC4_PROC(lightProjectionS);
	UBO_VEC4_PROC(lightProjectionT);
	UBO_VEC4_PROC(lightFalloff);
	UBO_VEC4_PROC(lightProjectionQ);
	UBO_VEC4_PROC(lightOrigin);
	UBO_VEC4_PROC(viewOrigin);
	UBO_VEC4_PROC(bumpMatrixS);
	UBO_VEC4_PROC(bumpMatrixT);
	UBO_VEC4_PROC(diffuseMatrixS);
	UBO_VEC4_PROC(diffuseMatrixT);
	UBO_VEC4_PROC(specularMatrixS);
	UBO_VEC4_PROC(specularMatrixT);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelMatrix);
	UBO_VEC4_PROC(globalLightOrigin);
	UBO_FLOAT_PROC(colorModulate);
    UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(interactionShadowMappingPointLight)

UBO_FRAGMENT_PROC_BEGIN(interactionShadowMappingPointLight)
	UBO_VEC4_PROC(diffuseColor);
	UBO_VEC4_PROC(specularColor);
	UBO_VEC4_PROC(globalLightOrigin);
	UBO_VEC4_N_PROC(uniformParm, 1);
	UBO_VEC4_N_PROC(uniformParm, 2);
	UBO_MAT4_N_PROC(shadowMVPMatrix, 6);
	UBO_VEC4_PROC(specularExponent);
	UBO_VEC4_N_PROC(uniformParm, 0);
	UBO_VEC4_N_PROC(uniformParm, 4);
UBO_FRAGMENT_PROC_END(interactionShadowMappingPointLight)




UBO_VERTEX_PROC_BEGIN(interactionShadowMappingParallelLight)
	UBO_VEC4_PROC(lightProjectionS);
	UBO_VEC4_PROC(lightProjectionT);
	UBO_VEC4_PROC(lightFalloff);
	UBO_VEC4_PROC(lightProjectionQ);
	UBO_VEC4_PROC(lightOrigin);
	UBO_VEC4_PROC(viewOrigin);
	UBO_VEC4_PROC(bumpMatrixS);
	UBO_VEC4_PROC(bumpMatrixT);
	UBO_VEC4_PROC(diffuseMatrixS);
	UBO_VEC4_PROC(diffuseMatrixT);
	UBO_VEC4_PROC(specularMatrixS);
	UBO_VEC4_PROC(specularMatrixT);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelMatrix);
	UBO_VEC4_PROC(globalLightOrigin);
	UBO_MAT4_PROC(modelViewMatrix);
	UBO_FLOAT_PROC(colorModulate);
    UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(interactionShadowMappingParallelLight)

UBO_FRAGMENT_PROC_BEGIN(interactionShadowMappingParallelLight)
	UBO_VEC4_PROC(diffuseColor);
	UBO_VEC4_PROC(specularColor);
	UBO_VEC4_PROC(globalLightOrigin);
	UBO_VEC4_N_PROC(uniformParm, 1);
	UBO_VEC4_N_PROC(uniformParm, 2);
	UBO_VEC4_N_PROC(uniformParm, 3);
	UBO_MAT4_N_PROC(shadowMVPMatrix, 6);
	UBO_VEC4_PROC(specularExponent);
	UBO_VEC4_N_PROC(uniformParm, 0);
	UBO_VEC4_N_PROC(uniformParm, 4);
UBO_FRAGMENT_PROC_END(interactionShadowMappingParallelLight)




UBO_VERTEX_PROC_BEGIN(interactionShadowMappingSpotLight)
	UBO_VEC4_PROC(lightProjectionS);
	UBO_VEC4_PROC(lightProjectionT);
	UBO_VEC4_PROC(lightFalloff);
	UBO_VEC4_PROC(lightProjectionQ);
	UBO_VEC4_PROC(lightOrigin);
	UBO_VEC4_PROC(viewOrigin);
	UBO_VEC4_PROC(bumpMatrixS);
	UBO_VEC4_PROC(bumpMatrixT);
	UBO_VEC4_PROC(diffuseMatrixS);
	UBO_VEC4_PROC(diffuseMatrixT);
	UBO_VEC4_PROC(specularMatrixS);
	UBO_VEC4_PROC(specularMatrixT);
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_MAT4_PROC(modelMatrix);
	UBO_VEC4_PROC(globalLightOrigin);
	UBO_MAT4_N_PROC(shadowMVPMatrix, 6);
	UBO_FLOAT_PROC(colorModulate);
    UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(interactionShadowMappingSpotLight)

UBO_FRAGMENT_PROC_BEGIN(interactionShadowMappingSpotLight)
	UBO_VEC4_PROC(diffuseColor);
	UBO_VEC4_PROC(specularColor);
	UBO_VEC4_PROC(globalLightOrigin);
	UBO_VEC4_N_PROC(uniformParm, 1);
	UBO_VEC4_N_PROC(uniformParm, 2);
	UBO_VEC4_PROC(specularExponent);
	UBO_VEC4_N_PROC(uniformParm, 0);
	UBO_VEC4_N_PROC(uniformParm, 4);
UBO_FRAGMENT_PROC_END(interactionShadowMappingSpotLight)



UBO_VERTEX_PROC_BEGIN(megaTexture)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_VEC4_N_PROC(megaTextureLevel, 0);
	UBO_VEC4_N_PROC(megaTextureLevel, 1);
	UBO_VEC4_N_PROC(megaTextureLevel, 2);
	UBO_VEC4_N_PROC(megaTextureLevel, 3);
	UBO_VEC4_N_PROC(megaTextureLevel, 4);
	UBO_VEC4_N_PROC(megaTextureLevel, 5);
	UBO_VEC4_N_PROC(megaTextureLevel, 6);
	UBO_VEC4_N_PROC(megaTextureLevel, 7);
	UBO_VEC4_N_PROC(megaTextureLevel, 8);
UBO_VERTEX_PROC_END(megaTexture)


UBO_VERTEX_PROC_BEGIN(globalIllumination)
	UBO_MAT4_PROC(modelViewProjectionMatrix);
	UBO_VEC4_PROC(viewOrigin);
	UBO_VEC4_PROC(bumpMatrixS);
	UBO_VEC4_PROC(bumpMatrixT);
	UBO_VEC4_PROC(diffuseMatrixS);
	UBO_VEC4_PROC(diffuseMatrixT);
	UBO_VEC4_PROC(specularMatrixS);
	UBO_VEC4_PROC(specularMatrixT);
	UBO_FLOAT_PROC(colorModulate);
    UBO_FLOAT_PROC(colorAdd);
UBO_VERTEX_PROC_END(globalIllumination)

UBO_FRAGMENT_PROC_BEGIN(globalIllumination)
	UBO_VEC4_PROC(diffuseColor);
	UBO_VEC4_PROC(specularColor);
	UBO_VEC4_PROC(specularExponent);
UBO_FRAGMENT_PROC_END(globalIllumination)


UBO_FRAGMENT_PROC_BEGIN(retro2Bit)
	UBO_VEC4_PROC(windowCoords);
	UBO_VEC4_PROC(nonPowerOfTwo);
	UBO_VEC4_N_PROC(uniformParm, 0);
UBO_FRAGMENT_PROC_END(retro2Bit)


UBO_FRAGMENT_PROC_BEGIN(retroC64)
	UBO_VEC4_PROC(windowCoords);
	UBO_VEC4_PROC(nonPowerOfTwo);
	UBO_VEC4_N_PROC(uniformParm, 0);
UBO_FRAGMENT_PROC_END(retroC64)


UBO_FRAGMENT_PROC_BEGIN(retroCPC)
	UBO_VEC4_PROC(windowCoords);
	UBO_VEC4_PROC(nonPowerOfTwo);
	UBO_VEC4_N_PROC(uniformParm, 0);
UBO_FRAGMENT_PROC_END(retroCPC)


UBO_FRAGMENT_PROC_BEGIN(retroGenesis)
	UBO_VEC4_PROC(windowCoords);
	UBO_VEC4_PROC(nonPowerOfTwo);
	UBO_VEC4_N_PROC(uniformParm, 0);
UBO_FRAGMENT_PROC_END(retroGenesis)


UBO_FRAGMENT_PROC_BEGIN(retroPS1)
	UBO_VEC4_PROC(windowCoords);
	UBO_VEC4_PROC(nonPowerOfTwo);
UBO_FRAGMENT_PROC_END(retroPS1)


#undef UBO_VERTEX_PROC_BEGIN
#undef UBO_FRAGMENT_PROC_BEGIN
#undef UBO_VERTEX_PROC_END
#undef UBO_FRAGMENT_PROC_END
#undef UBO_MAT4_PROC
#undef UBO_FLOAT_PROC
#undef UBO_VEC4_PROC
#undef UBO_INT_N_PROC
#undef UBO_VEC4_N_PROC
#undef UBO_MAT4_N_PROC
