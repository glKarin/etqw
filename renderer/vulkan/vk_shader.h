#ifndef _VULKAN_VKSHADER_H
#define _VULKAN_VKSHADER_H

#define VKBINDING_TRXTURES_START 2 // 着色器纹理绑定的偏移(1顶点着色器Uniform + 1片段着色器Uniform)

#define VKBINDING_TRXTURES_SLOT(x) (VKBINDING_TRXTURES_START + (x))
#define VKBINDING_NUM (VKBINDING_MAX_TRXTURE_UNITS + VKBINDING_TRXTURES_START) // 全部着色器绑定数

typedef struct VKShaderProgram_t
{
    char name[64]; // 着色器名称
    int type; // 着色器类型
    VKPipeline_t *pipeline; // 当前的渲染管线
    VKPipelineStateSet_t currentState; // 当前的状态码

    // 只在创建渲染管线布局时使用(3)
    VKHandle_t vertexShaderHandle; // 顶点着色器
    VKHandle_t fragmentShaderHandle; // 片段着色器
    VkShaderModule vertexShader; // 顶点着色器
    VkShaderModule fragmentShader; // 片段着色器
    globalState_t vertexInputAttribute; // 启用的顶点参数

    VKPipelineLayout_t *pipelineLayout; // 渲染管线布局(唯一)

    void *ubo_vertex; // CPU端顶点着色器Uniform缓冲区
    int uboSize_vertex; // CPU端顶点着色器Uniform缓冲区大小(未对齐)
    void *ubo_fragment; // CPU端片段着色器Uniform缓冲区
    int uboSize_fragment; // CPU端片段着色器Uniform缓冲区大小(未对齐)

    int textureTypes[VKBINDING_MAX_TRXTURE_UNITS]; // 绑定的纹理图像
    const VKImage_t *textures[VKBINDING_MAX_TRXTURE_UNITS]; // 绑定的纹理图像
    int numTextureUnits; // 最大使用的纹理图像数
    int numUBO; // Uniform对象数量

    // 用于数据脏检
    bool uboUpdate; // 是否更新
    int uboUpdateMask; // 更新类型

    // 用于数据脏检
    int frameCount; // 帧数
    globalUniformBufferObjectState_t uboState; // 数据脏检

    // 拷贝数据函数: 可能为NULL
    copyUniform_f copyUniform_vertex; // CPU端从全局Uniform内存复制到此顶点着色器内存
    copyUniform_f copyUniform_fragment; // CPU端从全局Uniform内存复制到此片段着色器内存

    // 如果数据未脏, 则使用旧的缓冲区
    VKBufferMap_t uniformBuffer_vertex; // 顶点着色器当前引用的Uniform缓冲区映射
    VKBufferMap_t uniformBuffer_fragment; // 片段着色器当前引用的Uniform缓冲区映射
	VkDescriptorSet descriptorSet; // 上次绑定的描述符

    VKMAP(VKPipeline_t, VKPipelineStateSet_t) pipelinePool; // 渲染管线布局创建的各个状态的渲染管线池
} VKShaderProgram_t;

void R_CreateVertexShader(VkPipelineShaderStageCreateInfo *createInfo, VkShaderModule module);
void R_CreateFragmentShader(VkPipelineShaderStageCreateInfo *createInfo, VkShaderModule module);
void R_VK_ListPipeline(const idCmdArgs &);

#endif //_VULKAN_VKSHADER_H
