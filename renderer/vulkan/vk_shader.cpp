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

typedef struct shaderSource_e
{
	const char *path;
	const char *macros;
	int ref;
} shaderSource_t;

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

#define SHADER_PROGRAM_NAME(type) R_VK_CreateRenderPipeline_##type()
#define SHADER_PROGRAM_DEFINE(type) static void SHADER_PROGRAM_NAME(type)
#define SHADER_PROGRAM_DECLARATION(type) SHADER_PROGRAM_DEFINE(type) {
#define END_SHADER_PROGRAM }

#define SHADER_PROGRAM_STAGE_INIT(stage, program, nameVal, type, texUnits, attrs) \
{ \
	memset(&program, 0, sizeof(program)); \
	program.uboSize_##stage = sizeof(UBO_DEFINE(stage, nameVal)); \
	program.ubo_##stage = malloc(program.uboSize_##stage); \
	program.copyUniform_##stage = UBOX_TRANSFORM_NAME(stage, nameVal); \
 \
	program.numTextureUnits = texUnits; \
	program.numUBO = 1; \
 \
    program.vertexShader = vk_backend.vertexShaders[type]; \
    program.fragmentShader = vk_backend.fragmentShaders[type]; \
    program.vertexInputAttribute = attrs; \
 \
    R_VK_CreatePipelineLayout(&program); \
 \
	strncpy(program.name, #nameVal, sizeof(program.name)); \
 \
	VKMAP_INIT(VKPipeline_t, globalStateSet_t, program.pipelinePool, PIPELINE_POOL_INIT_SIZE, PIPELINE_POOL_GRANULARITY); \
	Sys_Printf("Create shader: %d %s\n", type, #nameVal); \
}

#define SHADER_PROGRAM_VERTEX_INIT(program, nameVal, type, texUnits, attrs) SHADER_PROGRAM_STAGE_INIT(vertex, program, nameVal, type, texUnits, attrs)

#define SHADER_PROGRAM_FRAGMENT_INIT(program, nameVal, type, texUnits, attrs) SHADER_PROGRAM_STAGE_INIT(fragment, program, nameVal, type, texUnits, attrs)

#define SHADER_PROGRAM_INIT(program, nameVal, type, texUnits, attrs) \
{ \
	memset(&program, 0, sizeof(program)); \
	program.uboSize_vertex = sizeof(VERTEX_UBO_DEFINE(nameVal)); \
	program.uboSize_fragment = sizeof(FRAGMENT_UBO_DEFINE(nameVal)); \
	program.ubo_vertex = malloc(program.uboSize_vertex); \
	program.ubo_fragment = malloc(program.uboSize_fragment); \
	program.copyUniform_vertex = UBO0_TRANSFORM_NAME(nameVal); \
	program.copyUniform_fragment = UBO1_TRANSFORM_NAME(nameVal); \
 \
	program.numTextureUnits = texUnits; \
	program.numUBO = 2; \
 \
    program.vertexShader = vk_backend.vertexShaders[type]; \
    program.fragmentShader = vk_backend.fragmentShaders[type]; \
    program.vertexInputAttribute = attrs; \
 \
    R_VK_CreatePipelineLayout(&program); \
 \
	strncpy(program.name, #nameVal, sizeof(program.name)); \
 \
	VKMAP_INIT(VKPipeline_t, globalStateSet_t, program.pipelinePool, PIPELINE_POOL_INIT_SIZE, PIPELINE_POOL_GRANULARITY); \
	Sys_Printf("Create shader: %d %s\n", type, #nameVal); \
}

#define SHADER_PROGRAM_FUNC_DECL(name, type, texUnits, attrs) \
SHADER_PROGRAM_DECLARATION(name) \
SHADER_PROGRAM_INIT(name##VKShader, name, type, texUnits, attrs) \
END_SHADER_PROGRAM

#define SHADER_PROGRAM_VERTEX_FUNC_DECL(name, type, texUnits, attrs) \
SHADER_PROGRAM_DECLARATION(name) \
SHADER_PROGRAM_VERTEX_INIT(name##VKShader, name, type, texUnits, attrs) \
END_SHADER_PROGRAM

#define SHADER_PROGRAM_FRAGMENT_FUNC_DECL(name, type, texUnits, attrs) \
SHADER_PROGRAM_DECLARATION(name) \
SHADER_PROGRAM_FRAGMENT_INIT(name##VKShader, name, type, texUnits, attrs) \
END_SHADER_PROGRAM

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

    		// TODO:VK
    		fragmentHandle = VK_AllocShader(&fragmentShaderModule);
    		*fragmentShaderModule = VK_CreateShaderModule(fragmentSpirv, fragmentSpirvLength);

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

		// TODO:VK
		fragmentHandle = VK_AllocShader(&fragmentShaderModule);
		*fragmentShaderModule = VK_CreateShaderModule(fragmentSpirv, fragmentSpirvLength);

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
#define SHADER_PROGRAM_VERTEX_PROP_DECL(nameVal, type, texUnits, attrs) { type, sizeof(VERTEX_UBO_DEFINE(nameVal)), 0, &UBO0_TRANSFORM_NAME(nameVal), NULL, texUnits, attrs },
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
SHADER_PROGRAM_PROP_DECL(globalIllumination, SHADER_GLOBAL_ILLUMINATION, 3, VIA_ALL_BIT)
#endif
SHADER_PROGRAM_PROP_DECL(diffuseCubemap, SHADER_DIFFUSECUBEMAP, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT | VIA_TEXCOORD3_BIT, TUT_CUBEMAP)
SHADER_PROGRAM_PROP_DECL(texgen, SHADER_TEXGEN, 1, VIA_VERTEX_BIT | VIA_COLOR_BIT, TUT_2D)
#if !defined(_SPLASHDAMAGE)
SHADER_PROGRAM_PROP_DECL(colorProcess, SHADER_COLORPROCESS, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D)
SHADER_PROGRAM_PROP_DECL(heatHaze, SHADER_HEATHAZE, 2, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(heatHazeWithMask, SHADER_HEATHAZE_WITH_MASK, 3, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT, TUT_2D, TUT_2D, TUT_2D)
SHADER_PROGRAM_PROP_DECL(heatHazeWithMaskAndVertex, SHADER_HEATHAZE_WITH_MASK_AND_VERTEX, 3, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT, TUT_2D, TUT_2D, TUT_2D)
#endif
SHADER_PROGRAM_VERTEX_PROP_DECL(megaTexture, SHADER_MEGATEXTURE, 8, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT | VIA_COLOR_BIT)
#ifdef _SHADOW_MAPPING
SHADER_PROGRAM_VERTEX_FUNC_DECL(depth, SHADER_DEPTH, 0, VIA_VERTEX_BIT)
SHADER_PROGRAM_VERTEX_FUNC_DECL(depthPerforated, SHADER_DEPTH_PERFORATED, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingPointLight, SHADER_INTERACTION_POINT_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingBlinnPhongPointLight, SHADER_INTERACTION_BLINNPHONG_POINT_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingPBRPointLight, SHADER_INTERACTION_PBR_POINT_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(ambientLightingShadowMappingPointLight, SHADER_AMBIENT_LIGHTING_POINT_LIGHT, 7, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingParallelLight, SHADER_INTERACTION_PARALLEL_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingBlinnPhongParallelLight, SHADER_INTERACTION_BLINNPHONG_PARALLEL_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingPBRParallelLight, SHADER_INTERACTION_PBR_PARALLEL_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(ambientLightingShadowMappingParallelLight, SHADER_AMBIENT_LIGHTING_PARALLEL_LIGHT, 7, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingSpotLight, SHADER_INTERACTION_SPOT_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingBlinnPhongSpotLight, SHADER_INTERACTION_BLINNPHONG_SPOT_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionShadowMappingPBRSpotLight, SHADER_INTERACTION_PBR_SPOT_LIGHT, 8, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(ambientLightingShadowMappingSpotLight, SHADER_AMBIENT_LIGHTING_SPOT_LIGHT, 7, VIA_ALL_BIT)
#endif
#ifdef _STENCIL_SHADOW_IMPROVE
SHADER_PROGRAM_PROP_DECL(interactionTranslucent, SHADER_INTERACTION_TRANSLUCENT, 6, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionBlinnPhongTranslucent, SHADER_INTERACTION_PBR_TRANSLUCENT, 6, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionPBRTranslucent, SHADER_INTERACTION_BLINNPHONG_TRANSLUCENT, 6, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(ambientLightingTranslucent, SHADER_AMBIENT_LIGHTING_TRANSLUCENT, 5, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionSoft, SHADER_INTERACTION_SOFT, 7, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionBlinnPhongSoft, SHADER_INTERACTION_PBR_SOFT, 7, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(interactionPBRSoft, SHADER_INTERACTION_BLINNPHONG_SOFT, 7, VIA_ALL_BIT)
SHADER_PROGRAM_PROP_DECL(ambientLightingSoft, SHADER_AMBIENT_LIGHTING_SOFT, 6, VIA_ALL_BIT)
#endif
#ifdef _POSTPROCESS
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retro2Bit, SHADER_RETRO_2BIT, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT)
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retroC64, SHADER_RETRO_C64, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT)
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retroCPC, SHADER_RETRO_CPC, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT)
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retroGenesis, SHADER_RETRO_GENESIS, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT)
SHADER_PROGRAM_FRAGMENT_PROP_DECL(retroPS1, SHADER_RETRO_PSX, 1, VIA_VERTEX_BIT | VIA_TEXCOORD_BIT)
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

static void RB_VK_CreatePipelineLayoutFromShaderProp(VKShaderProgram_t *program, const VKShaderProp_t *prop, const char *name, VKHandle_t vertexShaderHandle, VKHandle_t fragmentShaderHandle, VkShaderModule vertexShader, VkShaderModule fragmentShader)
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
	R_VK_CreatePipelineLayout(program);
	VKMAP_INIT(VKPipeline_t, globalStateSet_t, program->pipelinePool, PIPELINE_POOL_INIT_SIZE, PIPELINE_POOL_GRANULARITY);
	Sys_Printf("Create shader: %d %s\n", prop->type, program->name);
}

static void RB_VK_InitShaderProgram(shaderProgram_t *shader, VKHandle_t vertexShaderHandle, VKHandle_t fragmentShaderHandle, VkShaderModule vertexShader, VkShaderModule fragmentShader)
{
	const VKShaderProp_t *prop = RB_VK_FindShaderProp(shader->type);
	if (!prop)
		return;

	shader->vk_shader = (VKShaderProgram_t *)malloc(sizeof(*shader->vk_shader));
	RB_VK_CreatePipelineLayoutFromShaderProp(shader->vk_shader, prop, shader->name, vertexShaderHandle, fragmentShaderHandle, vertexShader, fragmentShader);

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

#if 0
	const shaderSource_t shaderNames_vertex[] = {
		{ "interaction", "PHONG", 0 },
		{ "shadow", "", 0 },
		{ "default", "", 0 },
		{ "zfill", "", 0 },
		{ "zfillClip", "", 0 },
		{ "cubemap", "", 0 },
		{ "environment", "", 0 },
		{ "bumpyEnvironment", "", 0 },
		{ "fog", "", 0 },
		{ "blendLight", "", 0 },
		{ "interaction", "_PBR", 0 },
		{ "interaction", "BLINN_PHONG", 0 },
		{ "interaction", "_AMBIENT", 0 },
		{ "globalIllumination", "_BFG", 0 },
		{ "diffuseCubemap", "", 0 },
		{ "texgen", "", 0 },
		{ "colorProcess", "", 0 },
		{ "heatHaze", "", 0 },
		{ "heatHazeWithMask", "", 0 },
		{ "heatHazeWithMaskAndVertex", "", 0 },
		{ "megaTexture", "", 0 },
#ifdef _SHADOW_MAPPING
		{ "depthShadowMapping", "", 0 },
		{ "depthPerforated", "", 0 },
		{ "interactionShadowMapping", "_POINT_LIGHT,PHONG", 0 },
		{ "interactionShadowMapping", "_POINT_LIGHT,_PBR", 0 },
		{ "interactionShadowMapping", "_POINT_LIGHT,BLINN_PHONG", 0 },
		{ "interactionShadowMapping", "_POINT_LIGHT,_AMBIENT", 0 },
		{ "interactionShadowMapping", "_PARALLEL_LIGHT,PARALLEL_LIGHT_CASCADE_FRUSTUM,PHONG", 0 },
		{ "interactionShadowMapping", "_PARALLEL_LIGHT,PARALLEL_LIGHT_CASCADE_FRUSTUM,_PBR", 0 },
		{ "interactionShadowMapping", "_PARALLEL_LIGHT,PARALLEL_LIGHT_CASCADE_FRUSTUM,BLINN_PHONG", 0 },
		{ "interactionShadowMapping", "_PARALLEL_LIGHT,PARALLEL_LIGHT_CASCADE_FRUSTUM,_AMBIENT", 0 },
		{ "interactionShadowMapping", "_SPOT_LIGHT,PHONG", 0 },
		{ "interactionShadowMapping", "_SPOT_LIGHT,_PBR", 0 },
		{ "interactionShadowMapping", "_SPOT_LIGHT,BLINN_PHONG", 0 },
		{ "interactionShadowMapping", "_SPOT_LIGHT,_AMBIENT", 0 },
#endif
#ifdef _STENCIL_SHADOW_IMPROVE
		{ "interactionStencilShadow", "_TRANSLUCENT,PHONG", 0 },
		{ "interactionStencilShadow", "_TRANSLUCENT,_PBR", 0 },
		{ "interactionStencilShadow", "_TRANSLUCENT,BLINN_PHONG", 0 },
		{ "interactionStencilShadow", "_TRANSLUCENT,_AMBIENT", 0 },
		{ "interactionStencilShadow", "_SOFT,PHONG", 0 },
		{ "interactionStencilShadow", "_SOFT,_PBR", 0 },
		{ "interactionStencilShadow", "_SOFT,BLINN_PHONG", 0 },
		{ "interactionStencilShadow", "_SOFT,_AMBIENT", 0 },
#endif
#ifdef _POSTPROCESS
		{ "retro_postprocess_2d", "", 0 },
		{ "", "", SHADER_RETRO_2BIT },
		{ "", "", SHADER_RETRO_2BIT },
		{ "", "", SHADER_RETRO_2BIT },
		{ "", "", SHADER_RETRO_2BIT },
#endif
		{ "simpleVertexTexcoord", "", SHADER_DEPTH_TO_COLOR },
		{ "", "", SHADER_DEPTH_TO_COLOR },
	};
	static_assert(COUNT_OF(shaderNames_vertex) == SHADER_CUSTOM);

	const shaderSource_t shaderNames_fragment[] = {
		{ "interaction", "PHONG", 0 },
		{ "shadow", "", 0 },
		{ "default", "", 0 },
		{ "zfill", "", 0 },
		{ "zfillClip", "", 0 },
		{ "cubemap", "", 0 },
		{ "environment", "", 0 },
		{ "bumpyEnvironment", "", 0 },
		{ "fog", "", 0 },
		{ "", "", SHADER_FOG },
		{ "interaction", "_PBR", 0 },
		{ "interaction", "BLINN_PHONG", 0 },
		{ "interaction", "_AMBIENT", 0 },
		{ "globalIllumination", "_BFG", 0 },
		{ "", "", SHADER_CUBEMAP },
		{ "texgen", "", 0 },
		{ "colorProcess", "", 0 },
		{ "heatHaze", "", 0 },
		{ "heatHazeWithMask", "", 0 },
		{ "heatHazeWithMaskAndVertex", "", 0 },
		{ "megaTexture", "", 0 },
#ifdef _SHADOW_MAPPING
		{ "depthShadowMapping", "", 0 },
		{ "depthPerforated", "", 0 },
		{ "interactionShadowMapping", "_POINT_LIGHT,PHONG", 0 },
		{ "interactionShadowMapping", "_POINT_LIGHT,_PBR", 0 },
		{ "interactionShadowMapping", "_POINT_LIGHT,BLINN_PHONG", 0 },
		{ "interactionShadowMapping", "_POINT_LIGHT,_AMBIENT", 0 },
		{ "interactionShadowMapping", "_PARALLEL_LIGHT,PARALLEL_LIGHT_CASCADE_FRUSTUM,PHONG", 0 },
		{ "interactionShadowMapping", "_PARALLEL_LIGHT,PARALLEL_LIGHT_CASCADE_FRUSTUM,_PBR", 0 },
		{ "interactionShadowMapping", "_PARALLEL_LIGHT,PARALLEL_LIGHT_CASCADE_FRUSTUM,BLINN_PHONG", 0 },
		{ "interactionShadowMapping", "_PARALLEL_LIGHT,PARALLEL_LIGHT_CASCADE_FRUSTUM,_AMBIENT", 0 },
		{ "interactionShadowMapping", "_SPOT_LIGHT,PHONG", 0 },
		{ "interactionShadowMapping", "_SPOT_LIGHT,_PBR", 0 },
		{ "interactionShadowMapping", "_SPOT_LIGHT,BLINN_PHONG", 0 },
		{ "interactionShadowMapping", "_SPOT_LIGHT,_AMBIENT", 0 },
#endif
#ifdef _STENCIL_SHADOW_IMPROVE
		{ "interactionStencilShadow", "_TRANSLUCENT,PHONG", 0 },
		{ "interactionStencilShadow", "_TRANSLUCENT,_PBR", 0 },
		{ "interactionStencilShadow", "_TRANSLUCENT,BLINN_PHONG", 0 },
		{ "interactionStencilShadow", "_TRANSLUCENT,_AMBIENT", 0 },
		{ "interactionStencilShadow", "_SOFT,PHONG", 0 },
		{ "interactionStencilShadow", "_SOFT,_PBR", 0 },
		{ "interactionStencilShadow", "_SOFT,BLINN_PHONG", 0 },
		{ "interactionStencilShadow", "_SOFT,_AMBIENT", 0 },
#endif
#ifdef _POSTPROCESS
		{ "retro_2bit", "", 0 },
		{ "retro_c64", "", 0 },
		{ "retro_cpc", "", 0 },
		{ "retro_genesis", "", 0 },
		{ "retro_ps1", "", 0 },
#endif
		{ "depthToColor", "", 0 },
		{ "stencilToColor", "", 0 },
	};
	static_assert(COUNT_OF(shaderNames_fragment) == SHADER_CUSTOM);

	idStr vertex_shader_source_file = shaderNames_vertex[prop->type].path;
	if (vertex_shader_source_file == "")
		vertex_shader_source_file = shaderNames_vertex[shaderNames_vertex[prop->type].ref].path;
	vertex_shader_source_file += ".vert";
	idStr fragment_shader_source_file = shaderNames_fragment[prop->type].path;
	if (fragment_shader_source_file == "")
		fragment_shader_source_file = shaderNames_fragment[shaderNames_fragment[prop->type].ref].path;
	fragment_shader_source_file += ".frag";
#endif

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
	shaderProgram->program = type;

	RB_VK_InitShaderProgram(shaderProgram, vertexHandle, fragmentHandle, vertexShaderModule, fragmentShaderModule);

	return true;
}
