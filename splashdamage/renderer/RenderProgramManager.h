//karin: SplashDamage materialStage shader manager
#ifndef _KARIN_RENDERPROGRAMMANAGER_H
#define _KARIN_RENDERPROGRAMMANAGER_H
#include "idlib/containers/List.h"

class sdRenderProgram;

class sdRenderProgramManager
{
public:
    									sdRenderProgramManager(void);
    									~sdRenderProgramManager();

    // parse
    const sdRenderProgram *				LoadProgram(const char *name);

    static void							LoadProgram_f(const idCmdArgs &args);
    static void							ListPrograms_f(const idCmdArgs &args);

private:
    const sdRenderProgram *				Alloc(const char *name);
    const sdRenderProgram *				Find(const char *name);

private:
    idList<sdRenderProgram *>			programs;
};

extern sdRenderProgramManager *renderProgramManager;

#endif //_KARIN_RENDERPROGRAMMANAGER_H
