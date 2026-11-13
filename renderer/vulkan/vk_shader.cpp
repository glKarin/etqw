#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

#define MAX_OSPATH 1024

#define PIPELINE_POOL_INIT_SIZE 1
#define PIPELINE_POOL_GRANULARITY 1

#undef snprintf
#define snprintf idStr::snPrintf

extern bool shaderRequired;
#if !defined(_MSC_VER)
#define SHADER_ERROR(fmt, args...) { \
	if(shaderRequired) {               \
		common->Error(fmt, ##args); \
	} else {                           \
		common->Warning(fmt, ##args);   \
	}                                    \
}
#else
#define SHADER_ERROR(fmt, ...) { \
	if(shaderRequired) {               \
		common->Error(fmt, __VA_ARGS__); \
	} else {                           \
		common->Warning(fmt, __VA_ARGS__);   \
	}                                    \
}
#endif

extern int RB_GLSL_ReadExternalShaderSource(idStr &fullPath, idStr &buffer);
extern void RB_GLSL_ExpandMacros(idStr &res, const char *source, const char *macros, int highp = 0);

void R_CreateVertexShader(VkPipelineShaderStageCreateInfo *createInfo, VkShaderModule module)
{
    createInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo->stage = VK_SHADER_STAGE_VERTEX_BIT; // 着色器阶段
    createInfo->module = module; // 着色器模块
    createInfo->pName = "main"; // 入口函数名
}

void R_CreateFragmentShader(VkPipelineShaderStageCreateInfo *createInfo, VkShaderModule module)
{
    createInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo->stage = VK_SHADER_STAGE_FRAGMENT_BIT; // 着色器阶段
    createInfo->module = module; // 着色器模块
    createInfo->pName = "main"; // 入口函数名
}

VkShaderModule R_CompileShader(const char *path, int stage, const char *marcos)
{
	uint8_t *data;
	int shaderLength = VK_GLSL_LoadShaderFile(path, &data, stage, marcos);
	if (shaderLength <= 0)
		return VK_NULL_HANDLE;

	VkShaderModule *shaderModule;
	VKHandle_t handle = VK_AllocShader(&shaderModule);

	*shaderModule = VK_CreateShaderModule(data, shaderLength);
	free(data);
	return vk_context.shaders.list[handle];
}

//#undef VKGLOBAL_PARM_OFFSET
//#undef VKGLOBAL_PARMS_OFFSET
//#define VKGLOBAL_PARM_OFFSET(p) (int)offsetof(globalUniformBufferObject_t, p); printf("yyy " #p " %d\n", offsetof(globalUniformBufferObject_t, p));
//#define VKGLOBAL_PARMS_OFFSET(p,n) (int)(offsetof(globalUniformBufferObject_t, p) + (n) * sizeof(((globalUniformBufferObject_t *)0)->p[0])); printf("yyy " #p "[%d] %d\n",n, (offsetof(globalUniformBufferObject_t, p) + (n) * sizeof(((globalUniformBufferObject_t *)0)->p[0])));
static void RB_VK_GetOffsets(shaderProgram_t *shader)
{
	shader->localLightOrigin = VKGLOBAL_PARM_OFFSET(lightOrigin);
	shader->localViewOrigin = VKGLOBAL_PARM_OFFSET(viewOrigin);
	shader->lightProjectionS = VKGLOBAL_PARM_OFFSET(lightProjectionS);
	shader->lightProjectionT = VKGLOBAL_PARM_OFFSET(lightProjectionT);
	shader->lightProjectionQ = VKGLOBAL_PARM_OFFSET(lightProjectionQ);
	shader->lightFalloff = VKGLOBAL_PARM_OFFSET(lightFalloff);
	shader->bumpMatrixS = VKGLOBAL_PARM_OFFSET(bumpMatrixS);
	shader->bumpMatrixT = VKGLOBAL_PARM_OFFSET(bumpMatrixT);
	shader->diffuseMatrixS = VKGLOBAL_PARM_OFFSET(diffuseMatrixS);
	shader->diffuseMatrixT = VKGLOBAL_PARM_OFFSET(diffuseMatrixT);
	shader->specularMatrixS = VKGLOBAL_PARM_OFFSET(specularMatrixS);
	shader->specularMatrixT = VKGLOBAL_PARM_OFFSET(specularMatrixT);
	shader->colorModulate = VKGLOBAL_PARM_OFFSET(colorModulate);
	shader->colorAdd = VKGLOBAL_PARM_OFFSET(colorAdd);
	shader->diffuseColor = VKGLOBAL_PARM_OFFSET(diffuseColor);
	shader->specularColor = VKGLOBAL_PARM_OFFSET(specularColor);
	shader->glColor = VKGLOBAL_PARM_OFFSET(glColor);
	shader->alphaTest = VKGLOBAL_PARM_OFFSET(alphaTest);
	shader->specularExponent = VKGLOBAL_PARM_OFFSET(specularExponent);

	shader->eyeOrigin = VKGLOBAL_PARM_OFFSET(viewOrigin); // TODO
	shader->localEyeOrigin = VKGLOBAL_PARM_OFFSET(viewOrigin); // TODO
	shader->nonPowerOfTwo = VKGLOBAL_PARM_OFFSET(nonPowerOfTwo);
	shader->windowCoords = VKGLOBAL_PARM_OFFSET(windowCoords);

	shader->modelViewProjectionMatrix = VKGLOBAL_PARM_OFFSET(modelViewProjectionMatrix);

	shader->modelMatrix = VKGLOBAL_PARM_OFFSET(modelMatrix);
	shader->textureMatrix = VKGLOBAL_PARM_OFFSET(textureMatrix);
	//k: add modelView matrix uniform
	shader->modelViewMatrix = VKGLOBAL_PARM_OFFSET(modelViewMatrix);
	shader->projectionMatrix = VKGLOBAL_PARM_OFFSET(projectionMatrix);
	//k: add clip plane uniform
	shader->clipPlane = VKGLOBAL_PARM_OFFSET(clipPlane);
	//k: add fog matrix uniform
	shader->fogMatrix = VKGLOBAL_PARM_OFFSET(fogMatrix);
	//k: add fog color uniform
	shader->fogColor = VKGLOBAL_PARM_OFFSET(fogColor);
	//k: add texgen S T Q uniform
	shader->texgenS = VKGLOBAL_PARM_OFFSET(texgenS);
	shader->texgenT = VKGLOBAL_PARM_OFFSET(texgenT);
	shader->texgenQ = VKGLOBAL_PARM_OFFSET(texgenQ);

	int i;
	for (i = 0; i < MAX_VERTEX_PARMS; i++) {
		shader->u_vertexParm[i] = VKGLOBAL_PARMS_OFFSET(vertexParm, i);
	}
#if defined(_GLSL_PROGRAM) || defined(_RAVEN) || defined(_HUMANHEAD) //karin: fragment shader parms //karin: fragment shader parms
    for (i = 0; i < MAX_FRAGMENT_PARMS; i++) {
		shader->u_fragmentParm[i] = VKGLOBAL_PARMS_OFFSET(fragmentParm, i);
    }
#endif

	for (i = 0; i < MAX_FRAGMENT_IMAGES; i++) {
		shader->u_fragmentMap[i] = i;
	}
	//k: add cubemap texture units
	for ( i = 0; i < MAX_FRAGMENT_IMAGES; i++ ) {
		shader->u_fragmentCubeMap[i] = i;
	}

	for (i = 0; i < MAX_UNIFORM_PARMS; i++) {
		shader->u_uniformParm[i] = VKGLOBAL_PARMS_OFFSET(uniformParm, i);
	}

    for (i = 0; i < MAX_MEGATEXTURE_PARMS; i++) {
        shader->u_megaTextureLevel[i] = VKGLOBAL_PARMS_OFFSET(megaTextureLevel, i);
    }

#ifdef _SHADOW_MAPPING
	shader->shadowMVPMatrix = VKGLOBAL_PARM_OFFSET(shadowMVPMatrix);
    shader->globalLightOrigin = VKGLOBAL_PARM_OFFSET(globalLightOrigin);
#endif

	// get attribute location
	shader->attr_TexCoord = PC_ATTRIB_INDEX_ST;
	shader->attr_Tangent = PC_ATTRIB_INDEX_TANGENT;
	shader->attr_Bitangent = PC_ATTRIB_INDEX_BITANGENT;
	shader->attr_Normal = PC_ATTRIB_INDEX_NORMAL;
	shader->attr_Vertex = PC_ATTRIB_INDEX_VERTEX;
	shader->attr_Color = PC_ATTRIB_INDEX_COLOR;

#if 0
	for(int i = offsetof(shaderProgram_t, glColor); i < offsetof(shaderProgram_t, name); i += 4)
	{
		Sys_Printf("Uniform location: %d -> %d\n", i, *(int*)(((char *)shader) +i));
	}
#endif
}

static int RB_VK_LoadShaderProgram(
		const char *name,
        int type,
		shaderProgram_t * const program,
		const char *default_vertex_shader_source,
		const char *default_fragment_shader_source,
		const char *vertex_shader_source_file,
		const char *fragment_shader_source_file,
		const char *macros,
		VKHandle_t &vertexHandleRet,
		VKHandle_t &fragmentHandleRet,
		VkShaderModule &vertexShaderRet,
		VkShaderModule &fragmentShaderRet
		)
{
    enum {
        VK_LOAD_EXTERNAL_SOURCE = 1,
        VK_LOAD_BUILT_IN_SOURCE = 2,
        VK_LOAD_EXTERNAL_SPIRV = 3,
        VK_LOAD_BUILT_IN_SPIRV = 4,
    };
	// R_InitShaderProgram(program)
	VkShaderModule *vertexShaderModule = NULL;
	VkShaderModule *fragmentShaderModule = NULL;
	VKHandle_t vertexHandle = VKHANDLE_INVALID;
	VKHandle_t fragmentHandle = VKHANDLE_INVALID;

	common->Printf("\nLoad Vulkan GLSL shader program: %s -> %d\n", name, type);
    int step = 1;
#if 0
    if(harm_r_useGLSLShaderBinaryCache.GetInteger() == 2)
    {
        common->Printf(" %d. Load external shader binary:\n", step);
        if(RB_GLSL_LoadShaderBinaryCacheUncheck(program, name, type, true))
        {
            common->Printf("    Load external shader binary success!\n");
            return VK_LOAD_EXTERNAL_BINARY;
        }
        step++;

        common->Printf(" %d. Load built-in shader binary:\n", step);
        if(RB_GLSL_LoadShaderBinaryCacheUncheck(program, name, type, false))
        {
            common->Printf("    Load built-in shader binary success!\n");
            return GLSL_LOAD_BUILT_IN_BINARY;
        }
        step++;
    }
#endif

	common->Printf(" %d. Load external shader source: Vertex(%s), Fragment(%s)\n", step, vertex_shader_source_file, fragment_shader_source_file);

    // Load external GLSL shader source
    idStr vertexShaderStr;
    idStr vertexFullPath = vertex_shader_source_file;
    int vertexFileBuffer = RB_GLSL_ReadExternalShaderSource(vertexFullPath, vertexShaderStr);
    common->Printf("    Load Vulkan GLSL shader file: %s -> %s\n", vertexFullPath.c_str(), vertexFileBuffer ? "success" : "fail");
    idStr fragmentShaderStr;
    idStr fragmentFullPath = fragment_shader_source_file;
    int fragmentFileBuffer = RB_GLSL_ReadExternalShaderSource(fragmentFullPath, fragmentShaderStr);
	common->Printf("    Load Vulkan GLSL shader file: %s -> %s\n", fragmentFullPath.c_str(), fragmentFileBuffer ? "success" : "fail");

	uint8_t *vertexSpirv = NULL;
	int vertexSpirvLength = 0;
	uint8_t *fragmentSpirv = NULL;
	int fragmentSpirvLength = 0;

    if(vertexFileBuffer && fragmentFileBuffer)
    {
#if 0
        // Create external GLSL shader with binary
        if(USING_GLES3 && harm_r_useGLSLShaderBinaryCache.GetBool())
        {
            common->Printf("    Load and check external shader binary: Vertex(%s), Fragment(%s)\n", vertex_shader_source_file, fragment_shader_source_file);
            if(RB_GLSL_LoadShaderBinaryCache(program, name, vertexShaderStr, fragmentShaderStr, type, true))
            {
                common->Printf("    Load and check external shader binary success!\n");
                return VK_LOAD_EXTERNAL_BINARY;
            }
        }
#endif
        // Create external GLSL shader with source

    	do
    	{
    		vertexSpirvLength = VK_GLSL_LoadShaderData(vertexFullPath.c_str(), vertexShaderStr.c_str(), &vertexSpirv, SHADER_STAGE_VERTEX, macros);
    		if (vertexSpirvLength <= 0)
    			break;

    		fragmentSpirvLength = VK_GLSL_LoadShaderData(fragmentFullPath.c_str(), fragmentShaderStr.c_str(), &fragmentSpirv, SHADER_STAGE_FRAGMENT, macros);
    		if (fragmentSpirvLength <= 0)
    			break;

    		vertexHandle = VK_AllocShader(&vertexShaderModule);
    		*vertexShaderModule = VK_CreateShaderModule(vertexSpirv, vertexSpirvLength);

    		if (!*vertexShaderModule)
    			break;

    		fragmentHandle = VK_AllocShader(&fragmentShaderModule);
    		*fragmentShaderModule = VK_CreateShaderModule(fragmentSpirv, fragmentSpirvLength);

    		if (!*fragmentShaderModule)
    			break;

    		common->Printf("    Load external shader program success!\n");

#if 0
    		// Cache external GLSL shader binary
    		if(USING_GLES3 && harm_r_useGLSLShaderBinaryCache.GetBool())
    		{
    			RB_GLSL_CacheShaderBinary(program->program, name, vertexShaderStr, fragmentShaderStr, type, true);
    		}
#endif
    	} while (false);

    	if (vertexSpirv)
    	{
    		VK_GLSL_FreeSource(vertexSpirv);
    	}
    	if (fragmentSpirv)
    	{
    		VK_GLSL_FreeSource(fragmentSpirv);
    	}

    	if (*vertexShaderModule && *fragmentShaderModule)
    	{
			VKDEBUG_SET_NAME(VK_OBJECT_TYPE_SHADER_MODULE, (uint64_t)*vertexShaderModule, "shader::vertex%d::%s", vertexHandle, name);
			VKDEBUG_SET_NAME(VK_OBJECT_TYPE_SHADER_MODULE, (uint64_t)*fragmentShaderModule, "shader::fragment%d::%s", fragmentHandle, name);

    		vertexHandleRet = vertexHandle;
    		fragmentHandleRet = fragmentHandle;
    		vertexShaderRet = *vertexShaderModule;
    		fragmentShaderRet = *fragmentShaderModule;
    		return VK_LOAD_EXTERNAL_SOURCE;
    	}
    }

    step++;
    // Load built-in GLSL shader source
    common->Printf(" %d. Load built-in shader source\n", step);
    idStr vs;
    RB_GLSL_ExpandMacros(vs, default_vertex_shader_source, macros, harm_r_useHighPrecision.GetInteger());
    idStr fs;
    RB_GLSL_ExpandMacros(fs, default_fragment_shader_source, macros, harm_r_useHighPrecision.GetInteger());

#if 0
    // Create built-in GLSL shader with binary
    if(USING_GLES3 && harm_r_useGLSLShaderBinaryCache.GetBool())
    {
        common->Printf("    Load and check built-in shader binary\n");
        if(RB_GLSL_LoadShaderBinaryCache(program, name, vs, fs, type, false))
        {
            common->Printf("    Load and check built-in shader binary success!\n");
            return GLSL_LOAD_BUILT_IN_BINARY;
        }
    }
#endif

	// Create built-in GLSL shader with source
	do
	{
		vertexSpirvLength = VK_GLSL_LoadShaderData(vertexFullPath.c_str(), vs.c_str(), &vertexSpirv, SHADER_STAGE_VERTEX, macros);
		if (vertexSpirvLength <= 0)
			break;

		fragmentSpirvLength = VK_GLSL_LoadShaderData(fragmentFullPath.c_str(), fs.c_str(), &fragmentSpirv, SHADER_STAGE_FRAGMENT, macros);
		if (fragmentSpirvLength <= 0)
			break;

		vertexHandle = VK_AllocShader(&vertexShaderModule);
		*vertexShaderModule = VK_CreateShaderModule(vertexSpirv, vertexSpirvLength);
		if (!*vertexShaderModule)
			break;

		fragmentHandle = VK_AllocShader(&fragmentShaderModule);
		*fragmentShaderModule = VK_CreateShaderModule(fragmentSpirv, fragmentSpirvLength);
		if (!*fragmentShaderModule)
			break;

#if 0
		// Cache external GLSL shader binary
		if(USING_GLES3 && harm_r_useGLSLShaderBinaryCache.GetBool())
		{
			RB_GLSL_CacheShaderBinary(program->program, name, vs, fs, type, false);
		}
#endif

		common->Printf("    Load built-in shader program success!\n");

	} while (false);

	if (vertexSpirv)
	{
		VK_GLSL_FreeSource(vertexSpirv);
	}
	if (fragmentSpirv)
	{
		VK_GLSL_FreeSource(fragmentSpirv);
	}

	if (*vertexShaderModule && *fragmentShaderModule)
	{
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_SHADER_MODULE, (uint64_t)*vertexShaderModule, "shader::vertex%d::%s", vertexHandle, name);
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_SHADER_MODULE, (uint64_t)*fragmentShaderModule, "shader::fragment%d::%s", fragmentHandle, name);

		vertexHandleRet = vertexHandle;
		fragmentHandleRet = fragmentHandle;
		vertexShaderRet = *vertexShaderModule;
		fragmentShaderRet = *fragmentShaderModule;
		return VK_LOAD_BUILT_IN_SOURCE;
	}

	SHADER_ERROR("    Load built-in shader program fail!\n");
	return -1;
}

typedef struct VKShaderProp_t
{
	int type;
	int uboSize_vertex;
	int uboSize_fragment;
	copyUniform_f copyUniform_vertex;
	copyUniform_f copyUniform_fragment;
	int numTextureUnits;
    globalState_t vertexInputAttribute;
    int textureTypes[VKBINDING_MAX_TRXTURE_UNITS];
} VKShaderProp_t;

#define SHADER_PROGRAM_PROP_DECL(nameVal, type, texUnits, attrs, ...) { type, sizeof(VERTEX_UBO_DEFINE(nameVal)), sizeof(FRAGMENT_UBO_DEFINE(nameVal)), &UBO0_TRANSFORM_NAME(nameVal), &UBO1_TRANSFORM_NAME(nameVal), texUnits, attrs, { __VA_ARGS__ } },
#define SHADER_PROGRAM_VERTEX_PROP_DECL(nameVal, type, texUnits, attrs, ...) { type, sizeof(VERTEX_UBO_DEFINE(nameVal)), 0, &UBO0_TRANSFORM_NAME(nameVal), NULL, texUnits, attrs, { __VA_ARGS__ } },
#define SHADER_PROGRAM_FRAGMENT_PROP_DECL(nameVal, type, texUnits, attrs, ...) { type, 0, sizeof(FRAGMENT_UBO_DEFINE(nameVal)), NULL, &UBO1_TRANSFORM_NAME(nameVal), texUnits, attrs, { __VA_ARGS__ } },

static const VKShaderProp_t shaderProps[] = {
SHADER_PROGRAM_PROP_DECL(interaction, SHADER_INTERACTION, 6, VIA_ALL_BIT | VIA_BITANGENT_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D )
SHADER_PROGRAM_VERTEX_PROP_DECL(shadow, SHADER_SHADOW, 1, VIA_VERTEX_BIT | VIA_VERTEX4_BIT)
SHADER_PROGRAM_PROP_DECL(default, SHADER_DEFAULT, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT, TUT_2D)
SHADER_PROGRAM_PROP_DECL(zfill, SHADER_ZFILL, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
SHADER_PROGRAM_PROP_DECL(zfillClip, SHADER_ZFILLCLIP, 2, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(cubemap, SHADER_CUBEMAP, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT | VIA_TEXCOORD3_BIT, TUT_CUBEMAP)
SHADER_PROGRAM_PROP_DECL(environment, SHADER_ENVIRONMENT, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT | VIA_NORMAL_BIT, TUT_CUBEMAP)
SHADER_PROGRAM_PROP_DECL(bumpyEnvironment, SHADER_BUMPY_ENVIRONMENT, 2, VIA_ALL_BIT, TUT_CUBEMAP, TUT_2D)
SHADER_PROGRAM_PROP_DECL(fog, SHADER_FOG, 2, VIA_VERTEX_BIT, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionPBR, SHADER_INTERACTION_PBR, 6, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(blendLight, SHADER_BLENDLIGHT, 2, VIA_VERTEX_BIT, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionBlinnPhong, SHADER_INTERACTION_BLINNPHONG, 6, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(ambientLighting, SHADER_AMBIENT_LIGHTING, 5, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_CUBEMAP)
#ifdef _GLOBAL_ILLUMINATION
SHADER_PROGRAM_PROP_DECL(globalIllumination, SHADER_GLOBAL_ILLUMINATION, 3, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D)
#endif
SHADER_PROGRAM_PROP_DECL(diffuseCubemap, SHADER_DIFFUSECUBEMAP, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT | VIA_TEXCOORD3_BIT, TUT_CUBEMAP)
SHADER_PROGRAM_PROP_DECL(texgen, SHADER_TEXGEN, 1, VIA_VERTEX_BIT | VIA_COLOR_BIT, TUT_2D)
#if !defined(_SPLASHDAMAGE)
SHADER_PROGRAM_PROP_DECL(colorProcess, SHADER_COLORPROCESS, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
SHADER_PROGRAM_PROP_DECL(heatHaze, SHADER_HEATHAZE, 2, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(heatHazeWithMask, SHADER_HEATHAZE_WITH_MASK, 3, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(heatHazeWithMaskAndVertex, SHADER_HEATHAZE_WITH_MASK_AND_VERTEX, 3, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT, TUT_2D, TUT_2D, TUT_2D)
#endif
SHADER_PROGRAM_VERTEX_PROP_DECL(megaTexture, SHADER_MEGATEXTURE, 8, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
#ifdef _SHADOW_MAPPING
SHADER_PROGRAM_VERTEX_PROP_DECL(depth, SHADER_DEPTH, 0, VIA_VERTEX_BIT | VIA_VERTEX4_BIT)
SHADER_PROGRAM_PROP_DECL(depthPerforated, SHADER_DEPTH_PERFORATED, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingPointLight, SHADER_INTERACTION_POINT_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingBlinnPhongPointLight, SHADER_INTERACTION_BLINNPHONG_POINT_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingPBRPointLight, SHADER_INTERACTION_PBR_POINT_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(ambientLightingShadowMappingPointLight, SHADER_AMBIENT_LIGHTING_POINT_LIGHT, 7, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_CUBEMAP, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingParallelLight, SHADER_INTERACTION_PARALLEL_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingBlinnPhongParallelLight, SHADER_INTERACTION_BLINNPHONG_PARALLEL_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingPBRParallelLight, SHADER_INTERACTION_PBR_PARALLEL_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(ambientLightingShadowMappingParallelLight, SHADER_AMBIENT_LIGHTING_PARALLEL_LIGHT, 7, VIA_ALL_BIT, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_CUBEMAP, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingSpotLight, SHADER_INTERACTION_SPOT_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingBlinnPhongSpotLight, SHADER_INTERACTION_BLINNPHONG_SPOT_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingPBRSpotLight, SHADER_INTERACTION_PBR_SPOT_LIGHT, 8, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D_ARRAY, TUT_2D)
SHADER_PROGRAM_PROP_DECL(ambientLightingShadowMappingSpotLight, SHADER_AMBIENT_LIGHTING_SPOT_LIGHT, 7, VIA_ALL_BIT, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_CUBEMAP, TUT_2D_ARRAY, TUT_2D)
#endif
#ifdef _STENCIL_SHADOW_IMPROVE
SHADER_PROGRAM_PROP_DECL(interactionTranslucent, SHADER_INTERACTION_TRANSLUCENT, 6, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionBlinnPhongTranslucent, SHADER_INTERACTION_PBR_TRANSLUCENT, 6, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionPBRTranslucent, SHADER_INTERACTION_BLINNPHONG_TRANSLUCENT, 6, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(ambientLightingTranslucent, SHADER_AMBIENT_LIGHTING_TRANSLUCENT, 5, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_CUBEMAP)
SHADER_PROGRAM_PROP_DECL(interactionSoft, SHADER_INTERACTION_SOFT, 7, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionBlinnPhongSoft, SHADER_INTERACTION_PBR_SOFT, 7, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(interactionPBRSoft, SHADER_INTERACTION_BLINNPHONG_SOFT, 7, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(ambientLightingSoft, SHADER_AMBIENT_LIGHTING_SOFT, 6, VIA_ALL_BIT, TUT_2D, TUT_2D, TUT_2D, TUT_2D, TUT_CUBEMAP, TUT_2D)
#endif
#ifdef _POSTPROCESS
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retro2Bit, SHADER_RETRO_2BIT, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retroC64, SHADER_RETRO_C64, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retroCPC, SHADER_RETRO_CPC, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retroGenesis, SHADER_RETRO_GENESIS, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retroPS1, SHADER_RETRO_PSX, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
#endif
};

static const VKShaderProp_t * RB_VK_FindShaderProp(int type)
{
	const VKShaderProp_t *ptr = &shaderProps[0];
	for (int i = 0; i < COUNT_OF(shaderProps); i++, ptr++)
	{
		if (ptr->type == type)
			return ptr;
	}
	return NULL;
}

static void RB_VK_CreatePipelineLayoutFromShaderProp(VKShaderProgram_t *program, const VKShaderProp_t *prop, const char *name, int type, VKHandle_t vertexShaderHandle, VKHandle_t fragmentShaderHandle, VkShaderModule vertexShader, VkShaderModule fragmentShader)
{
	memset(program, 0, sizeof(*program));
	program->uboSize_vertex = prop->uboSize_vertex;
	program->ubo_vertex = program->uboSize_vertex > 0 ? malloc(program->uboSize_vertex) : NULL;
	program->uboSize_fragment = prop->uboSize_fragment;
	program->ubo_fragment = program->uboSize_fragment > 0 ? malloc(program->uboSize_fragment) : NULL;
	program->copyUniform_vertex = prop->copyUniform_vertex;
	program->copyUniform_fragment = prop->copyUniform_fragment;
	program->numTextureUnits = prop->numTextureUnits;
	program->numUBO = 0;
	if (program->ubo_vertex) program->numUBO++;
	if (program->ubo_fragment) program->numUBO++;
	program->vertexShader = vertexShader;
	program->fragmentShader = fragmentShader;
	program->vertexShaderHandle = vertexShaderHandle;
	program->fragmentShaderHandle = fragmentShaderHandle;
	program->vertexInputAttribute = prop->vertexInputAttribute;
	for(int i = 0; i < program->numTextureUnits; i++)
		program->textureTypes[i] = prop->textureTypes[i];
	idStr::Copynz(program->name, name, sizeof(program->name));
	program->type = type;
	R_VK_CreatePipelineLayout(program);
	VKMAP_INIT(VKPipeline_t, VKPipelineStateSet_t, program->pipelinePool, PIPELINE_POOL_INIT_SIZE, PIPELINE_POOL_GRANULARITY);
	Sys_Printf("Create shader: %d %s\n", prop->type, program->name);
}

static void RB_VK_InitShaderProgram(shaderProgram_t *shader, VKHandle_t vertexShaderHandle, VKHandle_t fragmentShaderHandle, VkShaderModule vertexShader, VkShaderModule fragmentShader)
{
	const VKShaderProp_t *prop = RB_VK_FindShaderProp(shader->type);
	if (!prop)
		return;

	shader->vk_shader = (VKShaderProgram_t *)malloc(sizeof(*shader->vk_shader));
	RB_VK_CreatePipelineLayoutFromShaderProp(shader->vk_shader, prop, shader->name, shader->type, vertexShaderHandle, fragmentShaderHandle, vertexShader, fragmentShader);

	RB_VK_GetOffsets(shader);

	VKLIST_ALLOC(VKShaderProgram_t *, vk_backend.programs);
	*_vkc_value = shader->vk_shader;
}

bool RB_VK_LoadShaderProgramFromProp(const GLSLShaderProp *prop)
{
	if (!glConfig.isInitialized) {
		return false;
	}

	const char *vs;
	const char *fs;
	const char *macros;
	VkShaderModule vertexShaderModule = VK_NULL_HANDLE;
	VkShaderModule fragmentShaderModule = VK_NULL_HANDLE;
	VKHandle_t vertexHandle = VKHANDLE_INVALID;
	VKHandle_t fragmentHandle = VKHANDLE_INVALID;

	vs = prop->default_vertex_shader_source.c_str();
	fs = prop->default_fragment_shader_source.c_str();
	macros = prop->macros.c_str();

	if(RB_VK_LoadShaderProgram(
			prop->name.c_str(),
			prop->type,
			prop->program,
			vs,
			fs,
			prop->vertex_shader_source_file.c_str(),
			prop->fragment_shader_source_file.c_str(),
			macros,
			vertexHandle, fragmentHandle, vertexShaderModule, fragmentShaderModule
	) < 0)
		return false;

	shaderProgram_t *shaderProgram = prop->program;
	int type = prop->type;
	const char *name = prop->name;

	idStr::Copynz(shaderProgram->name, name, sizeof(shaderProgram->name));
	shaderProgram->type = type;
	shaderProgram->program = vk_backend.programs.num;

	RB_VK_InitShaderProgram(shaderProgram, vertexHandle, fragmentHandle, vertexShaderModule, fragmentShaderModule);

	return true;
}

void R_VK_ListPipeline(const idCmdArgs &)
{
	common->Printf("Pipeline pool usage: %d/%d\n", vk_backend.programs.num, vk_backend.programs.size);

	VKShaderProgram_t *program;
	idStr str;
	VKLIST_FOREACH(VKShaderProgram_t, vk_backend.programs)
	{
		program = vk_backend.programs.list[i];
		str.Clear();
		for (int n = 0; n < program->numTextureUnits; n++)
		{
			switch (program->textureTypes[n])
			{
				case TUT_CUBEMAP:
					str.Append("CubeMap");
					break;
				case TUT_2D_ARRAY:
					str.Append("2DArray");
					break;
				case TUT_2D:
					str.Append("2D");
					break;
			}
			str.Append(" ");
		}
		common->Printf("%3d: %s %p -> pipelines=%d/%d; UBO(%d) size: vertex=%d, fragment=%d; textures(%d): %s\n", i + 1, program->name, program->pipelineLayout->pipelineLayout,
			program->pipelinePool.num, program->pipelinePool.size,
			program->numUBO, program->uboSize_vertex, program->uboSize_fragment,
			program->numTextureUnits, str.c_str()
			);

		VKPipelineStateSet_t *st;
		VKMAP_FOREACH(VKPipeline_t, VKPipelineStateSet_t, program->pipelinePool)
		{
			st = &program->pipelinePool.keys[i];
			str = R_VK_StateStr(*st);
			common->Printf("    %3d: %p -> %s\n", i + 1, program->pipelinePool.list[i], str.c_str());
		}
	}
}
