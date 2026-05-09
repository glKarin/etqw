// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __MODEL_CLUST_H__
#define __MODEL_CLUST_H__

class sdDeclStuffType;

class sdRenderModelStuffInstance
{
    public:
                                sdRenderModelStuffInstance(void);
    bool                        ParseBinary(idFile *file);
    void                        UpdateSurface(int index, srfTriangles_t *tri, const modelSurface_t *surf) const;
    bool                        IsVisible(const struct renderEntity_s *ent, const struct viewDef_s *view, const idBounds &bounds, float distance = -1.0f) const;

    idVec3                      origin;
    idAngles                    angles;
    idVec3                      color;
    //idMat3                      rotation;
	float						modelMatrix[16];
};

class sdStuffSurface
{
public:
                                sdStuffSurface(void);

    bool                        ParseBinary(idFile *file);
    void                        UpdateSurface(const struct renderEntity_s *ent, const struct viewDef_s *view, modelSurface_t *surf, const idList<int> &indexList) const;
    int                         GetModelNum(idList<int> &list, const struct renderEntity_s *ent, const struct viewDef_s *view, float distanceSqr = -1.0f) const;

private:
    const idRenderModelStatic *  GetModel(void) const;

private:
    int                         numInstances;
    const sdDeclStuffType       *stuffType;
    float                       instanceScale;
    idList<sdRenderModelStuffInstance> instanceList;
    idBounds                    bounds;

    friend class sdRenderModelClust;
};

class sdRenderModelClust : public idRenderModelStatic
{
public:
                                sdRenderModelClust(void);
    virtual void                InitFromFile(const char* fileName);
    virtual dynamicModel_t		IsDynamicModel() const;
    virtual idRenderModel 		*InstantiateDynamicModel(const struct renderEntity_s *ent, const struct viewDef_s *view, idRenderModel *cachedModel);
    virtual idBounds			Bounds(const struct renderEntity_s *ent) const;
    void                        LoadModel(void);
    void                        PurgeModel(void);

private:
    bool                        ParseBinary(void);

private:
    idList<sdStuffSurface>      surfaces;
};

#endif /* !__MODEL_CLUST_H__ */
