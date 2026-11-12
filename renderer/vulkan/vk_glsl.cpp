#include "vk_glsl.h"

#include "glslang/Public/ShaderLang.h"
#include "glslang/Include/ResourceLimits.h"
#include "SPIRV/GlslangToSpv.h"

#define MAX_OSPATH 1024

static bool glslangInitialized = false;

//#define ID_VK_SAVE_SPV 1

/**
 * 直接读取文件内容
 * @param path 文件路径
 * @param data 返回字节数据
 * @return 文件长度
 */
int VK_LoadFile(const char *path, uint8_t **data)
{
    FILE *file = fopen(path, "rb");
    if(!file)
        return -1;

    fseek(file, 0, SEEK_END);
    int length = (int)ftell(file);
    if(data && length > 0)
    {
        *data = (uint8_t *)malloc(length * sizeof(uint8_t));
        fseek(file, 0, SEEK_SET);
        fread(*data, sizeof(uint8_t), length, file);
    }
    fclose(file);
    return length;
}

int VK_SaveFile(const char *path, uint8_t *data, int length)
{
    FILE *file = fopen(path, "wb");
    if(!file)
        return -1;

    fwrite(data, sizeof(uint8_t), length, file);
    int cur = ftell(file);
    fclose(file);
    return cur;
}

int VK_StrSplit(const char *str, char delim, char **tokens)
{
    if(!str || !str[0])
        return 0;

    const char *start = str;
    const char *index;
    int length = (int)strlen(str);
    int counter = 0;
    while((index = strchr(start, delim)))
    {
        if(index - start > 0)
        {
            counter++;
        }
        start = index + 1;
        if(index == str + length - 1)
            break;
    }
    if(start <= str + length - 1)
    {
        counter++;
    }

    if (tokens)
    {
        char *tmp = strdup(str);
        int i = 0;
        char ch;
        char *cur = tmp;
        char *idx = tmp;
        while((idx = strchr(cur, delim)))
        {
            if(idx - cur > 0)
            {
                ch = cur[idx - cur];
                cur[idx - cur] = '\0';
                tokens[i] = strdup(cur);
                cur[idx - cur] = ch;
                i++;
            }
            cur = idx + 1;
            if(idx == tmp + length - 1)
                break;
        }
        if(cur <= tmp + length - 1)
        {
            idx = tmp + length;
            ch = cur[idx - cur];
            cur[idx - cur] = '\0';
            tokens[i] = strdup(cur);
            cur[idx - cur] = ch;
            i++;
        }
        free(tmp);
    }

    return counter;
}

static int VK_GLSL_FindNextLinePositionOfVersion(const char *res)
{
    const char *index = strstr(res, "#version");
    if(!index)
    {
        printf("GLSL shader source can not find '#version'\n.");
        return -1;
    }
    index = strchr(index, '\n');
    if(!index || index + 1 == res + strlen(res))
    {
        printf("GLSL shader source '#version' not completed\n.");
        return -1;
    }
    return (int)(index - res + 1); // previous char is '\n'
}

/**
#version XXX\n
\n
#define list[n]\n
\n
void main() {...}
 */
static char * VK_GLSL_InsertGlobalDefines(const char *res, const char *list[], int n)
{
    int index = VK_GLSL_FindNextLinePositionOfVersion(res);

    int grow = 1; // \n
    for (int i = 0; i < n; i++)
    {
    	// #define list[i]\n
        grow += (int)strlen("#define ");
        grow += (int)strlen(list[i]);
		grow += 1; // \n
    }
	grow += 1; // \n

    char *str = (char *)malloc(strlen(res) + grow + 1);
    *str = '\0';

    strncat(str, res, index);
    strcat(str, "\n");
    for (int i = 0; i < n; i++)
    {
        strcat(str, "#define ");
        strcat(str, list[i]);
        strcat(str, "\n");
    }
    strcat(str, "\n");

    strcat(str, res + index);

    return str;
}

static char * VK_GLSL_ExpandMacros(const char *source, const char *macros)
{
    if(!macros || !macros[0])
        return NULL;

    char **list;
    int n = VK_StrSplit(macros, ',', NULL);
    if(0 == n)
        return NULL;
    list = (char **)malloc(sizeof(*list) * n);
    VK_StrSplit(macros, ',', list);

    char *str = VK_GLSL_InsertGlobalDefines(source, (const char **)list, n);

    for (int i = 0; i < n; i++)
    {
        free(list[i]);
    }
    free(list);

    return str;
}

static void VK_SaveSpvShaderFile(const char *path, const uint32_t *data, int length)
{
    const char *lastSlash = strrchr(path, '/');
    const char *lastBackSlash = strrchr(path, '\\');
    const char *lastSlashRes = NULL;
    if (lastSlash && lastBackSlash)
        lastBackSlash = lastBackSlash > lastSlash ? lastBackSlash : lastSlash;
    else if (lastSlash)
        lastSlashRes = lastSlash;
    else if (lastBackSlash)
        lastSlashRes = lastBackSlash;

    char tmp[MAX_OSPATH] = {0};
    if (lastSlashRes)
    {
        strncat(tmp, path, lastSlashRes - path + 1);
    }
    strcat(tmp, "spv/");
    strcat(tmp, lastSlashRes + 1);
    strcat(tmp, ".spv");

    printf("Save spirv %s\n", tmp);
    VK_SaveFile(tmp, (uint8_t *)data, length);
}

int VK_GLSL_LoadShaderData(const char *path, const char *glslData, uint8_t **data, int stage, const char *marcos)
{
	int glslLength = (int)strlen(glslData);
	if (glslLength <= 0)
		return glslLength;

	char *glslText = (char *)malloc(glslLength + 1);
	snprintf(glslText, glslLength + 1, "%s", (char *)glslData);
	glslText[glslLength] = '\0';

	if (marcos && marcos[0])
	{
		char *str = VK_GLSL_ExpandMacros(glslText, marcos);
		if (str)
		{
			free(glslText);
			glslText = str;
		}
	}

	//printf("|%s|\n", glslText);
	uint32_t** data32 = (uint32_t**)data;
	int res = VK_GLSL_CompileSPIRV(path, glslText, stage, data32);
	free(glslText);
#ifdef ID_VK_SAVE_SPV
	if (res > 0)
	{
		VK_SaveSpvShaderFile(path, *data32, res);
	}
#endif
	return res;
}

int VK_GLSL_LoadShaderFile(const char *path, uint8_t **data, int stage, const char *marcos)
{
    uint8_t *glslData = NULL;
    int glslLength = VK_LoadFile(path, &glslData);
    if (glslLength <= 0)
        return glslLength;

    char *glslText = (char *)malloc(glslLength + 1);
    snprintf(glslText, glslLength + 1, "%s", (char *)glslData);
    glslText[glslLength] = '\0';
    free(glslData);

    if (marcos && marcos[0])
    {
        char *str = VK_GLSL_ExpandMacros(glslText, marcos);
        if (str)
        {
            free(glslText);
            glslText = str;
        }
    }

    //printf("|%s|\n", glslText);
    uint32_t** data32 = (uint32_t**)data;
    int res = VK_GLSL_CompileSPIRV(path, glslText, stage, data32);
    free(glslText);
#ifdef ID_VK_SAVE_SPV
    if (res > 0)
    {
        VK_SaveSpvShaderFile(path, *data32, res);
    }
#endif
    return res;
}

static void VK_GLSL_InitCompiler()
{
	if( !glslangInitialized )
	{
		glslang::InitializeProcess();
		glslangInitialized = true;
	}
}

static void * VK_GLSL_AllocSource(int size)
{
	return malloc( size );
}

int VK_GLSL_CompileSPIRV( const char* filename, const char* dataGLSL, const int stage, uint32_t** spirvBuffer )
{
	VK_GLSL_InitCompiler();

	EShLanguage shaderType;
	if( stage == SHADER_STAGE_VERTEX )
	{
		shaderType = EShLangVertex;
	}
	else if( stage == SHADER_STAGE_COMPUTE )
	{
		shaderType = EShLangCompute;
	}
	else
	{
		shaderType = EShLangFragment;
	}

	glslang::TShader shader( shaderType );
	shader.setStrings( &dataGLSL, 1 );

	int clientInputSemanticsVersion = 100; // maps to, say, #define VULKAN 100
	glslang::EShTargetClientVersion vulkanClientVersion = glslang::EShTargetVulkan_1_0;
	glslang::EShTargetLanguageVersion targetVersion = glslang::EShTargetSpv_1_0;

	shader.setEnvInput( glslang::EShSourceGlsl, shaderType, glslang::EShClientVulkan, clientInputSemanticsVersion );
	shader.setEnvClient( glslang::EShClientVulkan, vulkanClientVersion );
	shader.setEnvTarget( glslang::EShTargetSpv, targetVersion );

	// RB: see RBDOOM-3-BFG\neo\extern\glslang\StandAlone\ResourceLimits.cpp
	static TBuiltInResource resources;
	resources.maxLights = 32;
	resources.maxClipPlanes = 6;
	resources.maxTextureUnits = 32;
	resources.maxTextureCoords = 32;
	resources.maxVertexAttribs = 64;
	resources.maxVertexUniformComponents = 4096;
	resources.maxVaryingFloats = 64;
	resources.maxVertexTextureImageUnits = 32;
	resources.maxCombinedTextureImageUnits = 80;
	resources.maxTextureImageUnits = 32;
	resources.maxFragmentUniformComponents = 4096;
	resources.maxDrawBuffers = 32;
	resources.maxVertexUniformVectors = 128;
	resources.maxVaryingVectors = 8;
	resources.maxFragmentUniformVectors = 16;
	resources.maxVertexOutputVectors = 16;
	resources.maxFragmentInputVectors = 15;
	resources.minProgramTexelOffset = -8;
	resources.maxProgramTexelOffset = 7;
	resources.maxClipDistances = 8;
	resources.maxComputeWorkGroupCountX = 65535;
	resources.maxComputeWorkGroupCountY = 65535;
	resources.maxComputeWorkGroupCountZ = 65535;
	resources.maxComputeWorkGroupSizeX = 1024;
	resources.maxComputeWorkGroupSizeY = 1024;
	resources.maxComputeWorkGroupSizeZ = 64;
	resources.maxComputeUniformComponents = 1024;
	resources.maxComputeTextureImageUnits = 16;
	resources.maxComputeImageUniforms = 8;
	resources.maxComputeAtomicCounters = 8;
	resources.maxComputeAtomicCounterBuffers = 1;
	resources.maxVaryingComponents = 60;
	resources.maxVertexOutputComponents = 64;
	resources.maxGeometryInputComponents = 64;
	resources.maxGeometryOutputComponents = 128;
	resources.maxFragmentInputComponents = 128;
	resources.maxImageUnits = 8;
	resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
	resources.maxCombinedShaderOutputResources = 8;
	resources.maxImageSamples = 0;
	resources.maxVertexImageUniforms = 0;
	resources.maxTessControlImageUniforms = 0;
	resources.maxTessEvaluationImageUniforms = 0;
	resources.maxGeometryImageUniforms = 0;
	resources.maxFragmentImageUniforms = 8;
	resources.maxCombinedImageUniforms = 8;
	resources.maxGeometryTextureImageUnits = 16;
	resources.maxGeometryOutputVertices = 256;
	resources.maxGeometryTotalOutputComponents = 1024;
	resources.maxGeometryUniformComponents = 1024;
	resources.maxGeometryVaryingComponents = 64;
	resources.maxTessControlInputComponents = 128;
	resources.maxTessControlOutputComponents = 128;
	resources.maxTessControlTextureImageUnits = 16;
	resources.maxTessControlUniformComponents = 1024;
	resources.maxTessControlTotalOutputComponents = 4096;
	resources.maxTessEvaluationInputComponents = 128;
	resources.maxTessEvaluationOutputComponents = 128;
	resources.maxTessEvaluationTextureImageUnits = 16;
	resources.maxTessEvaluationUniformComponents = 1024;
	resources.maxTessPatchComponents = 120;
	resources.maxPatchVertices = 32;
	resources.maxTessGenLevel = 64;
	resources.maxViewports = 16;
	resources.maxVertexAtomicCounters = 0;
	resources.maxTessControlAtomicCounters = 0;
	resources.maxTessEvaluationAtomicCounters = 0;
	resources.maxGeometryAtomicCounters = 0;
	resources.maxFragmentAtomicCounters = 8;
	resources.maxCombinedAtomicCounters = 8;
	resources.maxAtomicCounterBindings = 1;
	resources.maxVertexAtomicCounterBuffers = 0;
	resources.maxTessControlAtomicCounterBuffers = 0;
	resources.maxTessEvaluationAtomicCounterBuffers = 0;
	resources.maxGeometryAtomicCounterBuffers = 0;
	resources.maxFragmentAtomicCounterBuffers = 1;
	resources.maxCombinedAtomicCounterBuffers = 1;
	resources.maxAtomicCounterBufferSize = 16384;
	resources.maxTransformFeedbackBuffers = 4;
	resources.maxTransformFeedbackInterleavedComponents = 64;
	resources.maxCullDistances = 8;
	resources.maxCombinedClipAndCullDistances = 8;
	resources.maxSamples = 4;
	resources.maxMeshOutputVerticesNV = 256;
	resources.maxMeshOutputPrimitivesNV = 512;
	resources.maxMeshWorkGroupSizeX_NV = 32;
	resources.maxMeshWorkGroupSizeY_NV = 1;
	resources.maxMeshWorkGroupSizeZ_NV = 1;
	resources.maxTaskWorkGroupSizeX_NV = 32;
	resources.maxTaskWorkGroupSizeY_NV = 1;
	resources.maxTaskWorkGroupSizeZ_NV = 1;
	resources.maxMeshViewCountNV = 4;

	resources.limits.nonInductiveForLoops = true;
	resources.limits.whileLoops = true;
	resources.limits.doWhileLoops = true;
	resources.limits.generalUniformIndexing = true;
	resources.limits.generalAttributeMatrixVectorIndexing = true;
	resources.limits.generalVaryingIndexing = true;
	resources.limits.generalSamplerIndexing = true;
	resources.limits.generalVariableIndexing = true;
	resources.limits.generalConstantMatrixVectorIndexing = true;

	EShMessages messages = ( EShMessages )( EShMsgSpvRules | EShMsgVulkanRules );

	const int defaultVersion = 100;

	if( !shader.parse( &resources, defaultVersion, false, messages ) )
	{
		printf( "GLSL parsing failed for: %s\n", filename );
		printf( "%s\n", shader.getInfoLog() );
		printf( "%s\n", shader.getInfoDebugLog() );

		//*spirvBuffer = NULL;
		//return 0;
	}

	glslang::TProgram program;
	program.addShader( &shader );

	if( !program.link( messages ) )
	{
		printf( "GLSL linking failed for: %s\n", filename );
		printf( "%s\n", shader.getInfoLog() );
		printf( "%s\n", shader.getInfoDebugLog() );
	}

	// All that's left to do now is to convert the program's intermediate representation into SpirV:
	std::vector<unsigned int>	spirV;
	spv::SpvBuildLogger			logger;
	glslang::SpvOptions			spvOptions;

	glslang::GlslangToSpv( *program.getIntermediate( shaderType ), spirV, &logger, &spvOptions );

	int32_t spirvLen = (int32_t)(spirV.size() * sizeof( unsigned int ));

	void* buffer = VK_GLSL_AllocSource( spirvLen );
	memcpy( buffer, spirV.data(), spirvLen );

	*spirvBuffer = ( uint32_t* ) buffer;
	return spirvLen;
}

void VK_GLSL_FreeSource(void* spirvBuffer)
{
	free( spirvBuffer );
}
