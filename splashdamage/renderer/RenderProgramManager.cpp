#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../../renderer/tr_local.h"
#include "RenderProgram.h"
#include "RenderProgramManager.h"

sdRenderProgramManager::sdRenderProgramManager(void)
{
}

sdRenderProgramManager::~sdRenderProgramManager(void)
{
    programs.DeleteContents(true);
}

const sdRenderProgram * sdRenderProgramManager::Alloc(const char *name) {
    sdRenderProgram *program = new sdRenderProgram;

    programs.Append(program);
    program->LoadProgram(name);

    return program;
}

const sdRenderProgram * sdRenderProgramManager::LoadProgram(const char *name) {
    const sdRenderProgram *program;

    program = Find(name);
    if (program)
        return program;

    program = Alloc(name);

    return program;
}

const sdRenderProgram * sdRenderProgramManager::Find(const char *name) {
    const idDecl* decl = declManager->FindType(DECL_RENDERPROGRAM, name, false);
    if (!decl) {
        common->Warning("sdRenderProgramManager::Find: render program decl '%s' not found", name);
        return NULL;
    }

    for (int i = 0; i < programs.Num(); i++) {
        if (programs[i]->GetDeclRenderProgram() == decl) {
            return programs[i];
        }
    }

    return NULL;
}

static sdRenderProgramManager renderProgramManagerLocal;
sdRenderProgramManager *renderProgramManager = &renderProgramManagerLocal;

void sdRenderProgramManager::LoadProgram_f(const idCmdArgs &args) {
    if (args.Argc() < 2) {
        common->Printf("Usage: %s <shader name>\n", args.Argv(0));
        return;
    }

    renderProgramManagerLocal.LoadProgram(args.Argv(1));
}

void sdRenderProgramManager::ListPrograms_f(const idCmdArgs &args) {
	common->Printf("----- %d shader programs -----\n", renderProgramManagerLocal.programs.Num());

    for (int i = 0; i < renderProgramManagerLocal.programs.Num(); i++) {
        const sdRenderProgram *program = renderProgramManagerLocal.programs[i];
        if (program->IsValid()) {
            const shaderProgram_t *shader = shaderManager->Get(program->GetShaderProgram());
            common->Printf("[%2d] %s: loaded: type=%d(%s), handle=%d, OpenGL handle=%d, in %s\n", i, program->GetDeclRenderProgram()->GetName(),
                shader ? shader->type : -1, shader ? shader->type >= SHADER_CUSTOM ? "custom" : "built-in" : "unload", program->GetShaderProgram(),
                shader ? shader->program : -1, program->GetDeclRenderProgram()->GetFileName()
                );
        } else {
            common->Printf("[%2d] %s: not load, in %s\n", i,
                program->GetDeclRenderProgram()->GetName(), program->GetDeclRenderProgram()->GetFileName()
                );
        }
    }
}
