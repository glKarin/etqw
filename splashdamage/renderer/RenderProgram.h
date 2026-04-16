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
    bool Bind(const float *regs);
    void Unbind(void);

    // state
    bool IsValid(void) const {
        return SHADER_HANDLE_IS_VALID(shaderProgram);
    }

    shaderHandle_t 				                    shaderProgram;

private:
    void LoadSource(idStr &vsOut, idStr &fsOut) const;
    void LoadVertexSource(idStr &out) const;
    void LoadFragmentSource(idStr &out) const;
    void InsertBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const;
    void InsertBindings(sdStringBuilder_Heap &buf, const sdRenderProgramShader *shader) const;
    void InsertAttribBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const;
    void InsertTextureBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const;
    void InsertUniformBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const;
    bool LoadGLSLProgram(const char *name);
    void BindUniform(const shaderProgram_t *shader, const float *regs);
    void UnbindUniform(void);

    const sdDeclRenderProgram *declRenderProgram;

    static void LoadSource(struct GLSLShaderProp *prop);
    static void BindingLocation(struct GLSLShaderProp *prop);
};

#endif //_KARIN_RENDERPROGRAM_H
