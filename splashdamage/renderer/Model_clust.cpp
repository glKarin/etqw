// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "renderer/Model_local.h"

#include "Model_clust.h"

#define CLUSTB_VERSION "Version 2"

sdRenderModelStuffInstance::sdRenderModelStuffInstance(void)
{

}

bool sdRenderModelStuffInstance::ParseBinary(idFile *file)
{
    file->ReadVec3(origin);
    file->ReadVec3(angles);
    file->ReadVec3(color);

    return true;
}



sdStuffSurface::sdStuffSurface(void)
    : numInstances(0),
    stuffType(NULL),
    instanceScale(1.0f)
{

}

bool sdStuffSurface::ParseBinary(idFile *file)
{
    idStr token;
    const idDecl *decl;
    int num;
    sdRenderModelStuffInstance *instance;

    file->ReadInt(numInstances);
    file->ReadString(token);
    decl = declManager->FindType(DECL_STUFFTYPE, token, false);
    if (!decl)
    {
        common->Warning("Stuff type '%s' not found, using default", token.c_str());
        decl = declManager->FindType(DECL_STUFFTYPE, token, true);
    }
    stuffType = static_cast<const sdDeclStuffType *>(decl);
    file->ReadFloat(instanceScale);
    file->ReadInt(num);
    if (num < 0)
    {
        common->Warning("invalid size: %d", num);
        return false;
    }
    instanceList.SetNum(num);
    instance = instanceList.Ptr();
    for (int i = 0; i < num; i++, instance++)
    {
        if (!instance->ParseBinary(file))
            return false;
    }

    if (!stuffType)
    {
        common->Warning("No stuff type specified in file");
        return false;
    }
    if (instanceList.Num() == 0)
    {
        common->Warning("No models specified in file");
        return false;
    }

    return true;
}



void sdRenderModelClust::InitFromFile(const char* fileName)
{
    name = fileName;
    LoadModel();
}

dynamicModel_t sdRenderModelClust::IsDynamicModel() const
{
    return DM_CONTINUOUS;
}

idRenderModel * sdRenderModelClust::InstantiateDynamicModel(const struct renderEntity_s *ent, const struct viewDef_s *view, idRenderModel *cachedModel)
{
    return NULL;
}

idBounds sdRenderModelClust::Bounds(const struct renderEntity_s *ent) const
{
    return bounds;
}

void sdRenderModelClust::LoadModel(void)
{
    if (!purged) {
        PurgeModel();
    }
	purged = false;

    if (!ParseBinary())
    {
        MakeDefaultModel();
        return;
    }
}

bool sdRenderModelClust::ParseBinary(void)
{
    idFile *file;
    idStr version;

    idStr binPath = "generated/md5binary";
    binPath.AppendPath(name);
    binPath.SetFileExtension(".clustb");

    file = fileSystem->OpenFileRead(binPath);
    if (!file)
        return false;

    file->ReadString(version);
    if (!idStr::Icmp(version, CLUSTB_VERSION))
    {
        common->Warning("sdRenderModelStuff::InitFromFile: bad id '%s' instead of '%s'", version.c_str(), CLUSTB_VERSION);
        fileSystem->CloseFile(file);
        return false;
    }

    while (file->Tell() < file->Length())
    {
        sdStuffSurface &surface = surfaces.Alloc();
        if (!surface.ParseBinary(file))
        {
            fileSystem->CloseFile(file);
            return false;
        }
    }

    return true;
}

void sdRenderModelClust::PurgeModel()
{
    surfaces.Clear();
    purged = true;
}


