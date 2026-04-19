//karin: SplashDamage materialStage shader
#ifndef _KARIN_RENDERPROGRAM_H
#define _KARIN_RENDERPROGRAM_H

class idMaterial;
class idImage;
class sdDeclRenderProgram;

class sdRenderProgram
{
public:
    sdRenderProgram(void);

    // parse
    bool LoadProgram(const char *name);
    bool LoadProgram(const sdDeclRenderProgram *name);

    // bind
    bool Bind(const materialStage_t *stage, const float *regs) const;
    void Unbind(const materialStage_t *stage) const;
	void BindVector(const char *name, const float regs[]) const;

    // state
    bool IsValid(void) const {
        return SHADER_HANDLE_IS_VALID(shaderProgram);
    }

    const sdDeclRenderProgram * GetDeclRenderProgram(void) const {
        return declRenderProgram;
    }

    shaderHandle_t GetShaderProgram(void) const {
        return shaderProgram;
    }

private:
    void LoadSource(idStr &vsOut, idStr &fsOut) const;
    void LoadVertexSource(idStr &out) const;
    void LoadFragmentSource(idStr &out) const;
    void InsertBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const;
	void InsertBuiltinBinding(sdStringBuilder_Heap &buf, const char *rawName) const;
    void InsertBindings(sdStringBuilder_Heap &buf, const sdRenderProgramShader *shader) const;
    void InsertAttribBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const;
    void InsertTextureBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const;
    void InsertUniformBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const;
	void GetLocations(shaderHandle_t handle);
	void GetShaderLocations(GLuint glHandle, const sdRenderProgramShader *shader);
    int GetLocation(GLuint glHandle, const sdDeclRenderBinding *binding, const char *rawName) const;
    void BindUniform(const materialStage_t *stage, const float *regs) const;
    void UnbindUniform(const materialStage_t *stage) const;
    GLint GetBindingLocation(const sdDeclRenderBinding *binding) const;
	void BindTexelSize(const char *name, const idImage *img) const;


    shaderHandle_t 				                    shaderProgram;
    const sdDeclRenderProgram *declRenderProgram;
    idList<const sdDeclRenderBinding *>				bindings;
    idList<idStr>									bindingNames;
    idList<GLint>									locations;

    static void LoadSourceCallback(struct GLSLShaderProp *prop);
    static void BindingLocationCallback(struct GLSLShaderProp *prop);

    friend class sdRenderProgramManager;
};

#endif //_KARIN_RENDERPROGRAM_H
