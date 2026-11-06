#if !defined(UBO_MAT4) || !defined(UBO_VEC4) || !defined(UBO_FLOAT) || !defined(UBO_INT) || !defined(UBO_BOOL)
#error "you must define UBO_MAT4/UBO_VEC4/UBO_FLOAT/UBO_INT/UBO_BOOL before including this file"
#endif

    UBO_MAT4(modelViewProjectionMatrix);
    UBO_MAT4(projectionMatrix);
    UBO_MAT4(modelViewMatrix);
    UBO_MAT4(modelMatrix);
    UBO_MAT4(textureMatrix);

    UBO_MAT4(fogMatrix);
    UBO_VEC4(fogColor);
    UBO_VEC4(clipPlane);

    UBO_VEC4(lightProjectionS);
    UBO_VEC4(lightProjectionT);
    UBO_VEC4(lightProjectionQ);
    UBO_VEC4(lightFalloff);
    UBO_VEC4(bumpMatrixS);
    UBO_VEC4(bumpMatrixT);
    UBO_VEC4(diffuseMatrixS);
    UBO_VEC4(diffuseMatrixT);
    UBO_VEC4(specularMatrixS);
    UBO_VEC4(specularMatrixT);

    UBO_VEC4(diffuseColor);
    UBO_VEC4(specularColor);
    UBO_VEC4(specularExponent);

    UBO_VEC4(nonPowerOfTwo);
    UBO_VEC4(windowCoords);
    UBO_VEC4(texgenS);
    UBO_VEC4(texgenT);
    UBO_VEC4(texgenQ);

    UBO_MAT4(shadowMVPMatrix[6]);
    UBO_VEC4(globalLightOrigin);

    UBO_VEC4(uniformParm[8]);
    UBO_VEC4(vertexParm[8]);
    UBO_VEC4(fragmentParm[8]);
    UBO_VEC4(megaTextureLevel[9]);

    UBO_INT(iuniformParm[8]);
    UBO_BOOL(buniformParm[8]);

    UBO_VEC4(lightOrigin);
    UBO_VEC4(viewOrigin);

    UBO_VEC4(glColor);
    UBO_FLOAT(colorModulate);
    UBO_FLOAT(colorAdd);
    UBO_FLOAT(alphaTest);

#undef UBO_MAT4
#undef UBO_VEC4
#undef UBO_FLOAT
#undef UBO_INT
#undef UBO_BOOL

