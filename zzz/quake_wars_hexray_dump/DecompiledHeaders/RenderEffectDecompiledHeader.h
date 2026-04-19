/* 750 */
const struct __cppobj renderEffect_s
{
  const rvDeclEffect *declEffect;
  unsigned __int16 suppressSurfaceInViewID;
  unsigned __int16 allowSurfaceInViewID;
  unsigned __int16 suppressLightsInViewID;
  int groupID;
  idVec3 origin;
  idMat3 axis;
  idVec3 gravity;
  idVec3 endOrigin;
  idVec3 windVector;
  idVec3 materialColor;
  float attenuation;
  bool hasEndOrigin;
  bool loop;
  bool useRenderBounds;
  bool isStatic;
  unsigned __int16 weaponDepthHackInViewID;
  float weaponDepthHackFOV_x;
  float weaponDepthHackFOV_y;
  float modelDepthHack;
  bool foliageDepthHack;
  float distanceOffset;
  float maxVisDist;
  float shaderParms[12];
};

/* 815 */
struct __cppobj idList<rvRenderEffectLocal *>
{
  int num;
  int size;
  int granularity;
  rvRenderEffectLocal **list;
};
