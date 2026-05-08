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

    idVec3                      origin;
    idVec3                      angles;
    idVec3                      color;
};

class sdStuffSurface
{
public:
    sdStuffSurface(void);

    bool                        ParseBinary(idFile *file);

private:
    int                         numInstances;
    const sdDeclStuffType       *stuffType;
    float                       instanceScale;
    idList<sdRenderModelStuffInstance> instanceList;
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
