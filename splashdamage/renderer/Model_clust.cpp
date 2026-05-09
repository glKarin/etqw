// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "renderer/tr_local.h"
#include "renderer/Model_local.h"

#include "Model_clust.h"

#define CLUSTB_VERSION "Version 2"

static const char *Clust_SnapshotName = "_Clust_Snapshot_";
static idRandom clustRandom;

static idCVar harm_r_stuffClustDistance("harm_r_stuffClustDistance", "2000", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_FLOAT, "max stuff distance with view origin, -1 to no limit");

sdRenderModelStuffInstance::sdRenderModelStuffInstance(void)
{

}

bool sdRenderModelStuffInstance::ParseBinary(idFile *file)
{
    file->ReadVec3(origin);
    file->ReadAngles(angles);
    file->ReadVec3(color);
    //rotation = angles.ToMat3();
	R_AxisToModelMatrix(angles.ToMat3(), origin, modelMatrix);

    return true;
}

void sdRenderModelStuffInstance::UpdateSurface(int index, srfTriangles_t *tri, const modelSurface_t *surf) const {
    idDrawVert *dv = tri->verts + index * surf->geometry->numVerts;
    glIndex_t *idx = tri->indexes + index * surf->geometry->numIndexes;
    idDrawVert *src = surf->geometry->verts;

    for (int i = 0; i < surf->geometry->numVerts; i++, src++, dv++) {
        *dv = *src;
        //dv->xyz *= rotation;
        //dv->xyz += origin;
		R_LocalPointToGlobal(modelMatrix, src->xyz, dv->xyz);
		R_LocalVectorToGlobal(modelMatrix, src->normal, dv->normal);
        dv->color[0] = (byte)((float)src->color[0] * color[0]);
        dv->color[1] = (byte)((float)src->color[1] * color[1]);
        dv->color[2] = (byte)((float)src->color[2] * color[2]);
    }

    glIndex_t base = index * surf->geometry->numVerts;
    for (int i = 0; i < surf->geometry->numIndexes; i++) {
        idx[i] = base + surf->geometry->indexes[i];
    }
}

bool sdRenderModelStuffInstance::IsVisible(const struct renderEntity_s *ent, const struct viewDef_s *view, const idBounds &bounds, float distanceSqr) const {
    if (distanceSqr > 0.0f && (view->renderView.vieworg - (ent->origin + origin)).LengthSqr() > distanceSqr)
        return false;

    return R_CullLocalBox(bounds, modelMatrix, 6, view->frustum);
}



sdStuffSurface::sdStuffSurface(void)
    : numInstances(0),
    stuffType(NULL),
    instanceScale(1.0f)
{
    bounds.Clear();
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
        bounds.AddPoint(instance->origin);
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

const idRenderModelStatic * sdStuffSurface::GetModel(void) const {
    const idRenderModel *model;

    if (stuffType->GetNumModels() == 0)
        model = renderModelManager->DefaultModel();
    else
        model = renderModelManager->FindModel(stuffType->GetModelName(0/*clustRandom.RandomInt(stuffType->GetNumModels())*/));
    return static_cast<const idRenderModelStatic *>(model);
}

void sdStuffSurface::UpdateSurface(const struct renderEntity_s *ent, const struct viewDef_s *view, modelSurface_t *surf, const idList<int> &indexList) const {
	srfTriangles_t *tri;
    const int *idx;

    const idRenderModelStatic *model = GetModel();

    const modelSurface_t &src = model->surfaces[0];

    surf->material = src.material;

    if (surf->geometry) {
        /*if (surf->geometry->numVerts == deformInfo->numOutputVerts && surf->geometry->numIndexes == deformInfo->numIndexes) {
            R_FreeStaticTriSurfVertexCaches(surf->geometry);
        } else */
            {
            R_FreeStaticTriSurf(surf->geometry);
            surf->geometry = R_AllocStaticTriSurf();
        }
    } else {
        surf->geometry = R_AllocStaticTriSurf();
    }

    tri = surf->geometry;
    tri->numVerts = indexList.Num() * src.geometry->numVerts;
    tri->numIndexes = indexList.Num() * src.geometry->numIndexes;

    R_AllocStaticTriSurfVerts(tri, tri->numVerts);
    R_AllocStaticTriSurfIndexes(tri, tri->numIndexes);
    idx = indexList.Ptr();
    for (int i = 0; i < indexList.Num(); i++, idx++) {
        instanceList[*idx].UpdateSurface(i, tri, &src);
    }

    R_BoundTriSurf(tri);
}

int sdStuffSurface::GetModelNum(idList<int> &list, const struct renderEntity_s *ent, const struct viewDef_s *view, float distanceSqr) const {
	if(list.NumAllocated() < instanceList.Num())
		list.Resize(instanceList.Num());
	list.SetNum(0);
    const idRenderModelStatic *model = GetModel();
    for (int i = 0; i < instanceList.Num(); i++) {
        if (instanceList[i].IsVisible(ent, view, model->bounds, distanceSqr))
            list.Append(i);
    }
    return list.Num();
}



sdRenderModelClust::sdRenderModelClust(void)
{
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
    int					i, surfaceNum;
    idRenderModelStatic	*staticModel;
    modelSurface_t *surf;
    sdStuffSurface *mesh;

    if (cachedModel && !r_useCachedDynamicModels.GetBool()) {
        delete cachedModel;
        cachedModel = NULL;
    }

    if (purged) {
        common->DWarning("model %s instantiated while purged", Name());
        LoadModel();
    }

    if (cachedModel) {
        assert(dynamic_cast<idRenderModelStatic *>(cachedModel) != NULL);
        assert(idStr::Icmp(cachedModel->Name(), Clust_SnapshotName) == 0);
        staticModel = static_cast<idRenderModelStatic *>(cachedModel);
    } else {
        staticModel = new idRenderModelStatic;
        staticModel->InitEmpty(Clust_SnapshotName);
    }

    staticModel->bounds.Clear();

    idList<int> indexList;
    mesh = surfaces.Ptr();
    float distance = harm_r_stuffClustDistance.GetFloat();
    if (distance > 0.0f) {
        distance *= distance;
    }
    for (i = 0; i < surfaces.Num(); i++, mesh++) {
        surfaceNum = mesh->GetModelNum(indexList, ent, view, distance);
        if (!surfaceNum) {
            staticModel->DeleteSurfaceWithId(i);
            continue;
        }

        if (staticModel->FindSurfaceWithId(i, surfaceNum)) {
            surf = &staticModel->surfaces[surfaceNum];
            R_FreeStaticTriSurfVertexCaches(surf->geometry);
        } else {
            surf = &staticModel->surfaces.Alloc();
            surf->geometry = NULL;
            surf->material = NULL;
            surf->id = i;
        }

        mesh->UpdateSurface(ent, view, surf, indexList);
    }

    staticModel->bounds = bounds;
    return staticModel;
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

    idStr binPath = name;
    binPath.SetFileExtension(".clustb");

    file = fileSystem->OpenFileRead(binPath);
    if (!file)
        return false;

    file->ReadString(version);
    if (idStr::Icmp(version, CLUSTB_VERSION))
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

    bounds.Clear();
    for (int i = 0; i < surfaces.Num(); i++) {
        bounds.AddBounds(surfaces[i].bounds);
    }

    return true;
}

void sdRenderModelClust::PurgeModel()
{
    surfaces.Clear();
    purged = true;
}


