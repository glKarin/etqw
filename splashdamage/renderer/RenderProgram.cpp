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
    prop.vertex_shader_source_file = decl->GetName();
    prop.vertex_shader_source_file.SetFileExtension(".vert");
    prop.fragment_shader_source_file = decl->GetName();
    prop.fragment_shader_source_file.SetFileExtension(".frag");
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

void sdRenderProgram::BindUniform(const materialStage_t *stage, const float *regs) const
{
    const stageVector_t *vec;
    const stageTextureMatrix_t *mat;
    const stageTexture_t *tex;
    GLint location;

    //Sys_Printf("BBB %d %d %d %d\n", shaderProgram, stage->numVectors, stage->numTextureMatrices, stage->numTextures);
    // setup vectors uniform
    for ( int i = 0; i < stage->numVectors; i++ ) {
        vec = &stage->vectors[i];

        location = GetBindingLocation(vec->renderBinding);
        //Sys_Printf("VVV %d %d %p\n", i,location, vec->renderBinding);
        if (location < 0)
            continue;

        idVec4 vparm;
        for (int d = 0; d < 4; d++) {
            int m = vec->registers[d];
            vparm[d] = regs[ m ];
        }

        qglUniform4fv(location, 1, vparm.ToFloatPtr());
        //Sys_Printf("VVV %d %d %s %s\n", i,location, vec->renderBinding->GetName(), vparm.ToString());
    }

    // setup matrix uniform vec3 x 2
    for ( int i = 0; i < stage->numTextureMatrices; i++ ) {
        mat = &stage->textureMatrices[i];

        location = GetBindingLocation(mat->renderBinding_s);
        //Sys_Printf("MMM111 %d %d %p\n", i,location, mat->renderBinding_s);
        if (location != -1) {
            idVec3 vparm;
            for (int d = 0; d < 3; d++) {
                int m = mat->matrix[0][d];
                vparm[d] = regs[ m ];
            }

            qglUniform3fv(location, 1, vparm.ToFloatPtr());
            //Sys_Printf("MMM111 %d %d %s %s\n", i,location, mat->renderBinding_s->GetName(), vparm.ToString());
        }

        location = GetBindingLocation(mat->renderBinding_t);
        //Sys_Printf("MMM222 %d %d %p\n", i,location, mat->renderBinding_t);
        if (location != -1) {
            idVec3 vparm;
            for (int d = 0; d < 3; d++) {
                int m = mat->matrix[1][d];
                vparm[d] = regs[ m ];
            }

            qglUniform3fv(location, 1, vparm.ToFloatPtr());
            //Sys_Printf("MMM222 %d %d %s %s\n", i,location, mat->renderBinding_t->GetName(), vparm.ToString());
        }
    }

    // setup sampler uniform
    for ( int i = 0; i < stage->numTextures; i++ ) {
        tex = &stage->textures[i];

        if (!tex->image)
            continue;

        location = GetBindingLocation(tex->renderBinding);
        //Sys_Printf("TTT %d %d %s %s\n", i,location, tex->renderBinding ? tex->renderBinding->GetName(): "<NULL>", tex->image->imgName.c_str());
        if (location < 0)
            continue;

        GL_SelectTexture( i );
        tex->image->Bind();
        qglUniform1i(location, i);
    }
}

bool sdRenderProgram::Bind(const materialStage_t *stage, const float *regs) const
{
    if(!IsValid())
        return false;

    const shaderProgram_t *shader = shaderManager->Get(shaderProgram);
    if(!shader)
        return false;
    GL_UseProgram((shaderProgram_t *)shader);

    BindUniform(stage, regs);

    return true;
}

void sdRenderProgram::UnbindUniform(const materialStage_t *stage) const
{
    const stageTexture_t *tex;
    GLint location;

    // binding sampler uniform to null
    for ( int i = 0; i < stage->numTextures; i++ ) {
        tex = &stage->textures[i];

        if (!tex->image)
            continue;

        location = GetBindingLocation(tex->renderBinding);
        if (location < 0)
            continue;

        GL_SelectTexture( i );
        globalImages->BindNull();
    }
}

void sdRenderProgram::Unbind(const materialStage_t *stage) const
{
    UnbindUniform(stage);
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
    GLint location;

    for (int i = 0; i < shader->NumBindings(); i++) {
        binding = shader->GetBinding(i);
        if (!binding)
            continue;
		if(bindings.FindIndex(binding) != -1)
			continue;
        location = GetLocation(glHandle, binding, shader->GetPlaceholder(i));
        if(location < 0)
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
		return -1;
    }
}

GLint sdRenderProgram::GetBindingLocation(const sdDeclRenderBinding *binding) const {
    if (!binding)
        return -1;
    int index = bindings.FindIndex(binding);
    if (index < 0)
        return -1;
    return locations[index];
}
