#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../../renderer/tr_local.h"
#include "RenderProgram.h"
#include "../decllib/declRenderProgram.h"
#include "../decllib/declRenderBinding.h"

#if 0
#define NSS_DEBUG(x) x
#else
#define NSS_DEBUG(x)
#endif

extern void RB_GLSL_ConvertGL2ESVertexShader(idStr &ret, const char *text, int version);
extern void RB_GLSL_ConvertGL2ESFragmentShader(idStr &ret, const char *text, int version);

sdRenderProgram::sdRenderProgram(void)
        : shaderProgram(idGLSLShaderManager::INVALID_SHADER_HANDLE),
        declRenderProgram(NULL)
{
}

bool sdRenderProgram::LoadProgram(const char *name) {
    shaderHandle_t handle;

    handle = shaderManager->GetHandle(name);
    if(SHADER_HANDLE_IS_VALID(handle))
    {
        shaderProgram = handle;
        return true;
    }

    const idDecl* decl = declManager->FindType(DECL_RENDERPROGRAM, name, false);
    if (!decl) {
        common->Warning("sdRenderProgram::LoadProgram: render program decl '%s' not found", name);
        return false;
    }

    return LoadProgram(static_cast<const sdDeclRenderProgram *>(decl));
}

bool sdRenderProgram::LoadProgram(const sdDeclRenderProgram *decl)
{
    shaderHandle_t handle;
    declRenderProgram = decl;

    if (!declRenderProgram->IsCompleted()) {
        common->Warning("sdRenderProgram::LoadProgram: render program decl '%s' not completed", decl->GetName());
        return false;
    }

#if 1
    if (declRenderProgram->GetVertexShader()->GetLang() != sdRenderProgramShader::SL_GLSL) {
        common->Warning("sdRenderProgram::LoadProgram: render program vertex shader '%s' language not GLSL: %d", decl->GetName(), declRenderProgram->GetVertexShader()->GetLang());
        return false;
    }

    if (declRenderProgram->GetFragmentShader()->GetLang() != sdRenderProgramShader::SL_GLSL) {
        common->Warning("sdRenderProgram::LoadProgram: render program fragment shader '%s' language not GLSL: %d", decl->GetName(), declRenderProgram->GetFragmentShader()->GetLang());
        return false;
    }
#endif

    GLSLShaderProp prop(decl->GetName(), this, &sdRenderProgram::LoadSourceCallback, &sdRenderProgram::BindingLocationCallback);
    handle = shaderManager->Load(prop);
    if(SHADER_HANDLE_IS_INVALID(handle))
    {
        common->Warning("Load GLSL shader program fail: %s.", decl->GetName());
        return false;
    }
    shaderProgram = handle;

    return true;
}

void sdRenderProgram::LoadSourceCallback(GLSLShaderProp *prop) {
    sdRenderProgram *self = (sdRenderProgram *)prop->data;
    prop->default_vertex_shader_source.Clear();
    prop->default_fragment_shader_source.Clear();
    self->LoadSource(prop->default_vertex_shader_source, prop->default_fragment_shader_source);
}

void sdRenderProgram::BindingLocationCallback(struct GLSLShaderProp *prop) {
    sdRenderProgram *self = (sdRenderProgram *)prop->data;
    self->GetLocations(prop->handle);
}

void sdRenderProgram::BindUniform(const shaderProgram_t *shader, const float *regs)
{
    //common->Printf("BBB %d\n", shaderProgram);
    // setting local parameters (specified in material definition)
    /*for ( int i = 0; i < numShaderParms; i++ ) {
        rvNewShaderStageParm<int[4]> *p = shaderParms + i;

        idVec4 vparm;
        for (int d = 0; d < 4; d++)
        {
            int m = p->value[d];
            switch (m) {
                case VIEW_ORIGIN_X:
                    vparm[d] = backEnd.viewDef->renderView.vieworg[0];
                    break;
                case VIEW_ORIGIN_Y:
                    vparm[d] = backEnd.viewDef->renderView.vieworg[1];
                    break;
                case VIEW_ORIGIN_Z:
                    vparm[d] = backEnd.viewDef->renderView.vieworg[2];
                    break;
                default:
                    vparm[d] = regs[ m ];
                    break;
            }
        }

        GLint location = GetLocation(shader, p);
        switch(p->numValue)
        {
            case 1:
                qglUniform1fv(location, 1, vparm.ToFloatPtr());
                break;
            case 2:
                qglUniform2fv(location, 1, vparm.ToFloatPtr());
                break;
            case 3:
                qglUniform3fv(location, 1, vparm.ToFloatPtr());
                break;
            case 4:
            default:
                qglUniform4fv(location, 1, vparm.ToFloatPtr());
                break;
        }
        //printf("UUU %d %d %s %s\n", i,location, p->name.c_str(), vparm.ToString(6));
    }*/

    // setting textures
    // note: the textures are also bound to TUs at this moment
    /*for ( int i = 0; i < numShaderTextures; i++ ) {
        if ( shaderTextures[i].value ) {
            rvNewShaderStageParm<idImage *> *p = shaderTextures + i;
            GLint location = GetLocation(shader, p);
            GL_SelectTexture( i );
            p->value->Bind();
            qglUniform1i(location, i);
            //printf("TTT %d %d %s %s\n", i,location, p->name.c_str(), p->value ? p->value->imgName.c_str() : "NULL");
        }
    }*/
}

bool sdRenderProgram::Bind(const float *regs)
{
    if(!IsValid())
        return false;

    const shaderProgram_t *shader = shaderManager->Get(shaderProgram);
    if(!shader)
        return false;
    GL_UseProgram((shaderProgram_t *)shader);

    BindUniform(shader, regs);

    return true;
}

void sdRenderProgram::UnbindUniform(void)
{
    /*for ( int i = 0; i < numShaderTextures; i++ ) {
        if ( shaderTextures[i].value ) {
            GL_SelectTexture( i );
            globalImages->BindNull();
        }
    }*/
}

void sdRenderProgram::Unbind(void)
{
    UnbindUniform();
    GL_SelectTextureForce(0);
    GL_UseProgram(NULL);
}

void sdRenderProgram::LoadSource(idStr &vsOut, idStr &fsOut) const
{
    common->Printf("Convert GLSL shader %s:\n\n", declRenderProgram->GetName());
    LoadVertexSource(vsOut);
    LoadFragmentSource(fsOut);
    common->Printf("Vertex shader:\n%s\n\n", vsOut.c_str());
    common->Printf("Fragment shader:\n%s\n\n", fsOut.c_str());
}

void sdRenderProgram::LoadVertexSource(idStr &out) const {
    const sdRenderProgramShader *shader = declRenderProgram->GetVertexShader();

    sdStringBuilder_Heap buf;

    InsertBindings(buf, shader);
    buf.Append("\n");

    buf.Append(shader->GetSource());

    const int Version = USING_GLES3 ? 300 : 100;
    RB_GLSL_ConvertGL2ESVertexShader(out, buf.c_str(), Version);
}

void sdRenderProgram::LoadFragmentSource(idStr &out) const {
    const sdRenderProgramShader *shader = declRenderProgram->GetFragmentShader();

    sdStringBuilder_Heap buf;

    InsertBindings(buf, shader);
    buf.Append("\n");

    buf.Append(shader->GetSource());

    const int Version = USING_GLES3 ? 300 : 100;
    RB_GLSL_ConvertGL2ESFragmentShader(out, buf.c_str(), Version);
}

void sdRenderProgram::InsertBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const {
    if (!rawName || !rawName[0])
        rawName = binding->GetName();
    switch (binding->GetBindingType()) {
        case sdDeclRenderBinding::BT_ATTRIB:
            InsertAttribBinding(buf, binding, rawName);
            break;
        case sdDeclRenderBinding::BT_TEXTURE:
            InsertTextureBinding(buf, binding, rawName);
            break;
        case sdDeclRenderBinding::BT_VECTOR:
            InsertUniformBinding(buf, binding, rawName);
            break;
        default:
            common->Warning("sdRenderProgram::InsertBinding: unknown render binding %s type: %d", binding->GetName(), binding->GetBindingType());
            break;
    }
}

void sdRenderProgram::InsertUniformBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const {
    buf.Append("uniform vec4 ");
    buf.Append(rawName);
    buf.Append(";\n");
}

void sdRenderProgram::InsertTextureBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const {
    buf.Append("uniform ");
    switch (binding->GetCubeMap()) {
        case CF_CAMERA:
        case CF_NATIVE:
            buf.Append("samplerCube ");
            break;
        case CF_2D:
        default:
            buf.Append("sampler2D ");
            break;
    }
    buf.Append(rawName);
    buf.Append(";\n");
}

void sdRenderProgram::InsertAttribBinding(sdStringBuilder_Heap &buf, const sdDeclRenderBinding *binding, const char *rawName) const {
    // hardcode fix
#if 1
    buf.Append("#define ");
    buf.Append(rawName);
    buf.Append(" ");
    if (!idStr::Icmp(rawName, "positionAttrib"))
        buf.Append("attr_Vertex");
    else if (!idStr::Icmp(rawName, "texCoordAttrib"))
        buf.Append("attr_TexCoord");
    else if (!idStr::Icmp(rawName, "colorAttrib"))
        buf.Append("attr_Color");
    else if (!idStr::Icmp(rawName, "normalAttrib"))
        buf.Append("attr_Normal");
    else if (!idStr::Icmp(rawName, "tangentAttrib"))
        buf.Append("attr_Tangent");
    else if (!idStr::Icmp(rawName, "signAttrib"))
        buf.Append("attr_Bitangent");
    buf.Append("\n");
#else
    buf.Append("attribute vec4 ");
    buf.Append(binding->GetName());
    buf.Append(";\n");
#endif
}

void sdRenderProgram::InsertBindings(sdStringBuilder_Heap &buf, const sdRenderProgramShader *shader) const {
    const sdDeclRenderBinding *binding;

    for (int i = 0; i < shader->NumBindings(); i++) {
        binding = shader->GetBinding(i);
        if (!binding)
            continue;
        InsertBinding(buf, binding, shader->GetPlaceholder(i));
    }
}

void sdRenderProgram::GetLocations(shaderHandle_t handle)
{
	shaderProgram = handle;
    const shaderProgram_t *shader = shaderManager->Get(shaderProgram);
    if(!shader) {
		common->Warning("sdRenderProgram::GetLocations: invalid program %d", shaderProgram);
        return;
	}

	bindings.Clear();
	locations.Clear();
	GetShaderLocations(shader->program, declRenderProgram->GetVertexShader());
	GetShaderLocations(shader->program, declRenderProgram->GetFragmentShader());
	bindings.Resize(bindings.Num());
	bindings.SetGranularity(1);
	locations.Resize(locations.Num());
	locations.SetGranularity(1);
}

void sdRenderProgram::GetShaderLocations(GLuint glHandle, const sdRenderProgramShader *shader)
{
    const sdDeclRenderBinding *binding;

    for (int i = 0; i < shader->NumBindings(); i++) {
        binding = shader->GetBinding(i);
        if (!binding)
            continue;
		if(bindings.FindIndex(binding) < 0)
			continue;
		bindings.Append(binding);
		locations.Append(GetLocation(glHandle, binding, shader->GetPlaceholder(i)));
    }
}

int sdRenderProgram::GetLocation(GLuint glHandle, const sdDeclRenderBinding *binding, const char *rawName) const {
	if(!binding)
		return -1;
    if (!rawName || !rawName[0])
        rawName = binding->GetName();
    switch (binding->GetBindingType()) {
        case sdDeclRenderBinding::BT_ATTRIB:
			return qglGetAttribLocation(glHandle, rawName);
        case sdDeclRenderBinding::BT_TEXTURE:
        case sdDeclRenderBinding::BT_VECTOR:
			return qglGetUniformLocation(glHandle, rawName);
        default:
            common->Warning("sdRenderProgram::GetLocation: unknown render binding %s type: %d", binding->GetName(), binding->GetBindingType());
            break;
    }
}
