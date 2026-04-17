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
