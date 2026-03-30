void __thiscall rvLinkedParticle::HandleTiling(rvLinkedParticle *this, rvParticleTemplate *pt)
{
  if ( this->mFlags & 0x100000 )
  {
    this->mTextureScale = pt->mTiling;
    this->mTextureOffset = 0.0;
  }
}
unsigned int __thiscall rvOrientedParticle::GetArrayIndex(rvOrientedParticle *this, rvParticle *p)
{
  unsigned int result; // eax

  if ( p )
    result = ((char *)p - (char *)this) / 0x128u;
  else
    result = -1;
  return result;
}
unsigned int __thiscall rvElectricityParticle::GetArrayIndex(rvElectricityParticle *this, rvParticle *p)
{
  unsigned int result; // eax

  if ( p )
    result = ((char *)p - (char *)this) / 0x144u;
  else
    result = -1;
  return result;
}
unsigned int __thiscall rvDecalParticle::GetArrayIndex(rvDecalParticle *this, rvParticle *p)
{
  unsigned int result; // eax

  if ( p )
    result = ((char *)p - (char *)this) / 0x108u;
  else
    result = -1;
  return result;
}
unsigned int __thiscall rvModelParticle::GetArrayIndex(rvModelParticle *this, rvParticle *p)
{
  unsigned int result; // eax

  if ( p )
    result = ((char *)p - (char *)this) / 0x11Cu;
  else
    result = -1;
  return result;
}
unsigned int __thiscall rvLightParticle::GetArrayIndex(rvLightParticle *this, rvParticle *p)
{
  unsigned int result; // eax

  if ( p )
    result = ((char *)p - (char *)this) / 0x224u;
  else
    result = -1;
  return result;
}
unsigned int __thiscall rvLinkedParticle::GetArrayIndex(rvLinkedParticle *this, rvParticle *p)
{
  unsigned int result; // eax

  if ( p )
    result = ((char *)p - (char *)this) / 0xF0u;
  else
    result = -1;
  return result;
}
unsigned int __thiscall rvDebrisParticle::GetArrayIndex(rvDebrisParticle *this, rvParticle *p)
{
  unsigned int result; // eax

  if ( p )
    result = (unsigned int)((char *)p - (char *)this) >> 8;
  else
    result = -1;
  return result;
}
void __thiscall rvParticle::EvaluateVelocity(rvParticle *this, rvBSE *effect, idVec3 *velocity, float time)
{
  float timea; // [esp+Ch] [ebp+Ch]

  if ( this->mFlags & 1 )
  {
    velocity->x = 1.0;
    velocity->y = 0.0;
    velocity->z = 0.0;
  }
  else
  {
    velocity->x = this->mAcceleration.x * time + this->mVelocity.x;
    velocity->y = this->mAcceleration.y * time + this->mVelocity.y;
    velocity->z = this->mAcceleration.z * time + this->mVelocity.z;
    timea = 1.0 - time * this->mFriction;
    velocity->x = velocity->x * timea;
    velocity->y = velocity->y * timea;
    velocity->z = timea * velocity->z;
  }
}
void __thiscall rvParticle::ScaleAngle(rvParticle *this, float constant)
{
  this->mAngleEnv.mStart.x = this->mAngleEnv.mStart.x * constant;
  this->mAngleEnv.mStart.y = this->mAngleEnv.mStart.y * constant;
  this->mAngleEnv.mStart.z = this->mAngleEnv.mStart.z * constant;
  this->mAngleEnv.mEnd.x = this->mAngleEnv.mEnd.x * constant;
  this->mAngleEnv.mEnd.y = this->mAngleEnv.mEnd.y * constant;
  this->mAngleEnv.mEnd.z = constant * this->mAngleEnv.mEnd.z;
}
double __thiscall rvParticleTemplate::GetDuration(rvParticleTemplate *this)
{
  return rvRandom::flrand(this->mDuration.x, this->mDuration.y);
}
void __thiscall rvParticle::SetOriginUsingEndOrigin(rvParticle *this, rvBSE *effect, rvParticleTemplate *pt, idVec3 *normal, idVec3 *centre)
{
  rvParticleTemplate *v5; // esi
  VBRState *v6; // ebx
  int v7; // ecx
  rvBSE *v8; // eax
  float v9; // ST24_4
  rvParticleParms *v10; // ebp
  int v11; // ecx
  void (__cdecl *v12)(VBRState *); // eax
  rvParticleParms temp; // [esp+14h] [ebp-34h]
  int v14; // [esp+44h] [ebp-4h]
  float effecta; // [esp+4Ch] [ebp+4h]
  float dista; // [esp+50h] [ebp+8h]
  float distb; // [esp+50h] [ebp+8h]
  float dist; // [esp+50h] [ebp+8h]

  *(_DWORD *)&temp.mSpawnType = this;
  temp.mMins.x = 0.0;
  BYTE2(temp.mRange) = 0;
  v5 = pt;
  v6 = (VBRState *)&this->mInitPos;
  v7 = pt->mpSpawnPosition->mSpawnType;
  v14 = 0;
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v7])(v6);
  v8 = effect;
  effecta = effect->mCurrentEndOrigin.x - effect->mOriginalOrigin.x;
  dista = v8->mCurrentEndOrigin.y - v8->mOriginalOrigin.y;
  v9 = v8->mCurrentEndOrigin.z - v8->mOriginalOrigin.z;
  distb = dista * dista + effecta * effecta + v9 * v9;
  dist = sqrt(distb);
  v10 = v5->mpSpawnPosition;
  qmemcpy(&temp.mRange, v5->mpSpawnPosition, sizeof(rvParticleParms));
  if ( v10->mModelInfo )
  {
    LODWORD(temp.mMins.x) = operator new(0x2Cu);
    qmemcpy((void *)LODWORD(temp.mMins.x), v10->mModelInfo, 0x2Cu);
  }
  temp.mMins.y = v6->energy_alpha;
  v11 = LOBYTE(temp.mRange);
  temp.mMaxs.y = dist;
  v6->energy_alpha = *(float *)(*(_DWORD *)&temp.mSpawnType + 36);
  v12 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v11];
  BYTE2(temp.mRange) = 0;
  v12(v6);
  v14 = -1;
  operator delete((void *)LODWORD(temp.mMins.x));
}
void __thiscall rvParticle::HandleEndOrigin(rvParticle *this, rvBSE *effect, rvParticleTemplate *pt, idVec3 *normal, idVec3 *centre)
{
  this->mInitPos.x = this->mFraction;
  if ( ((unsigned int)effect->mFlags >> 1) & 1 && pt->mpSpawnPosition->mFlags & 2 )
    rvParticle::SetOriginUsingEndOrigin(this, effect, pt, normal, centre);
  else
    ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[pt->mpSpawnPosition->mSpawnType])((VBRState *)&this->mInitPos);
}
void __stdcall rvParticle::SetLengthUsingEndOrigin(rvBSE *effect, rvParticleParms *parms, float *length)
{
  rvBSE *v3; // eax
  double v4; // st7
  float v5; // ST20_4
  bool v6; // zf
  sdModelInfo *v7; // eax
  sdModelInfo *v8; // esi
  void (__cdecl *v9)(VBRState *); // eax
  rvParticleParms temp; // [esp+10h] [ebp-34h]
  int v11; // [esp+40h] [ebp-4h]
  float dista; // [esp+48h] [ebp+4h]
  float distb; // [esp+48h] [ebp+4h]
  float dist; // [esp+48h] [ebp+4h]

  v3 = effect;
  v4 = effect->mCurrentEndOrigin.x;
  v11 = 0;
  v5 = v4 - effect->mCurrentOrigin.x;
  dista = effect->mCurrentEndOrigin.y - effect->mCurrentOrigin.y;
  *(float *)&temp.mSpawnType = v3->mCurrentEndOrigin.z - v3->mCurrentOrigin.z;
  distb = dista * dista + v5 * v5 + *(float *)&temp.mSpawnType * *(float *)&temp.mSpawnType;
  dist = sqrt(distb);
  v6 = parms->mModelInfo == 0;
  qmemcpy(&temp.mRange, parms, sizeof(rvParticleParms));
  if ( !v6 )
  {
    v7 = (sdModelInfo *)operator new(0x2Cu);
    v8 = parms->mModelInfo;
    LODWORD(temp.mMins.x) = v7;
    qmemcpy(v7, v8, sizeof(sdModelInfo));
  }
  v9 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[LOBYTE(temp.mRange)];
  temp.mMins.y = temp.mMins.y + dist;
  *(_WORD *)((char *)&temp.mRange + 1) = 0;
  temp.mMaxs.y = dist + temp.mMaxs.y;
  v9((VBRState *)length);
  v11 = -1;
  operator delete((void *)LODWORD(temp.mMins.x));
}
void __stdcall rvParticle::HandleEndLength(rvBSE *effect, rvParticleTemplate *pt, rvParticleParms *parms, float *length)
{
  if ( ((unsigned int)effect->mFlags >> 1) & 1 && pt->mpSpawnLength->mFlags & 2 )
    rvParticle::SetLengthUsingEndOrigin(effect, parms, length);
  else
    ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[(unsigned __int8)parms->mSpawnType])((VBRState *)length);
}
// local variable allocation has failed, the output may be wrong!
void __thiscall rvParticle::FinishSpawn(rvParticle *this, rvBSE *effect, rvSegment *segment, float birthTime, float fraction, idVec3 *initOffset, idMat3 *initAxis)
{
  rvParticle *v7; // ebp
  rvSegmentTemplate *v8; // eax
  rvSegmentTemplate *v9; // esi
  int v10; // ebx
  unsigned __int8 *v11; // eax
  int v12; // edx
  float *v13; // edi
  void (__cdecl *v14)(VBRState *); // eax
  rvBSE *v15; // esi
  double v16; // st7
  float v17; // ecx
  float v18; // edx
  float v19; // ecx
  idVec3 *v20; // eax
  unsigned int v21; // ecx
  char v22; // dl
  void (__cdecl *v23)(VBRState *); // ecx
  double v24; // st7
  rvAngles *v25; // eax
  float v26; // ecx
  float *v27; // eax
  double v28; // st7
  double v29; // st6
  double v30; // st7
  float *v31; // eax
  float *v32; // ecx
  double v33; // st7
  idVec3 *v34; // eax
  unsigned __int8 *v35; // eax
  int v36; // edx
  rvParticleVtbl *v37; // edx
  VBRState *v38; // eax
  void (__cdecl *v39)(VBRState *); // edx
  rvParticleVtbl *v40; // edx
  VBRState *v41; // eax
  void (__cdecl *v42)(VBRState *); // edx
  void (__cdecl *v43)(VBRState *); // eax
  void (__cdecl *v44)(VBRState *); // eax
  unsigned __int8 *v45; // eax
  int v46; // edx
  float *(__thiscall *v47)(rvParticle *); // eax
  VBRState *v48; // eax
  void (__cdecl *v49)(VBRState *); // edx
  rvParticleVtbl *v50; // edx
  VBRState *v51; // eax
  void (__cdecl *v52)(VBRState *); // edx
  rvParticleVtbl *v53; // edx
  float v54; // eax
  rvParticleVtbl *v55; // edx
  float *v56; // eax
  float *(__thiscall *v57)(rvParticle *); // eax
  float v58; // eax
  rvParticleVtbl *v59; // edx
  float *v60; // eax
  int v61; // eax
  int v62; // eax
  void (__thiscall *v63)(rvParticle *, idRenderModel *); // edx
  double v64; // st7
  double v65; // st7
  int v66; // ecx
  float v67; // ecx
  double v68; // st7
  void (__thiscall *v69)(rvParticle *, rvEnvParms *, float); // edx
  float v70; // ecx
  void (__thiscall *v71)(rvParticle *, rvEnvParms *, float); // edx
  float v72; // ecx
  float v73; // ecx
  double v74; // st6
  double v75; // st5
  float *v76; // eax
  float v77; // [esp+30h] [ebp-B4h]
  float atten; // [esp+34h] [ebp-B0h]
  float min; // [esp+38h] [ebp-ACh]
  float halfOpeningAngle; // [esp+3Ch] [ebp-A8h]
  int v81; // [esp+40h] [ebp-A4h]
  float f; // [esp+50h] [ebp-94h]
  idVec3 normal; // [esp+54h] [ebp-90h]
  float v84; // [esp+60h] [ebp-84h]
  float v85; // [esp+64h] [ebp-80h]
  float v86; // [esp+68h] [ebp-7Ch]
  float v87; // [esp+6Ch] [ebp-78h]
  float v88; // [esp+70h] [ebp-74h]
  sdRenderProgram *v89; // [esp+74h] [ebp-70h]
  float t1; // [esp+78h] [ebp-6Ch]
  float v91; // [esp+7Ch] [ebp-68h] OVERLAPPED
  char v92; // [esp+82h] [ebp-62h]
  char v93; // [esp+83h] [ebp-61h]
  float *v94; // [esp+84h] [ebp-60h]
  float v95; // [esp+88h] [ebp-5Ch]
  float v96; // [esp+8Ch] [ebp-58h]
  float v97; // [esp+90h] [ebp-54h]
  float windStrength; // [esp+94h] [ebp-50h]
  rvAngles angles; // [esp+98h] [ebp-4Ch]
  float v100; // [esp+A4h] [ebp-40h]
  idVec3 result; // [esp+A8h] [ebp-3Ch]
  char v102; // [esp+B4h] [ebp-30h]
  idMat3 v103; // [esp+C0h] [ebp-24h]

  v7 = this;
  *(float *)&v8 = COERCE_FLOAT(rvSegment::GetSegmentTemplate(segment));
  v9 = v8;
  v91 = *(float *)&v8;
  if ( *(float *)&v8 == 0.0 )
    return;
  v10 = (int)&v8->mParticleTemplate;
  v7->mFlags = v8->mParticleTemplate.mFlags;
  if ( (unsigned __int8)rvSegment::GetLocked(segment) )
    v7->mFlags |= 2u;
  else
    v7->mFlags &= 0xFFFFFFFD;
  if ( ((unsigned int)v9->mFlags >> 9) & 1 )
    v7->mFlags |= (unsigned int)&unk_1000000;
  else
    v7->mFlags &= 0xFEFFFFFF;
  v11 = (unsigned __int8 *)v9->mParticleTemplate.mpSpawnVelocity;
  v12 = *v11;
  halfOpeningAngle = 0.0;
  min = 0.0;
  atten = *(float *)&v11;
  v13 = &v7->mVelocity.x;
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v12])((VBRState *)&v7->mVelocity);
  v14 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v9->mParticleTemplate.mpSpawnAcceleration->mSpawnType];
  v94 = &v7->mAcceleration.x;
  v14((VBRState *)&v7->mAcceleration);
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v9->mParticleTemplate.mpSpawnWindStrength->mSpawnType])((VBRState *)&angles);
  v15 = effect;
  v16 = 0.0;
  if ( angles.pitch != 0.0 )
  {
    *(float *)&v89 = effect->mCurrentWindVector.y * effect->mCurrentWindVector.y
                   + effect->mCurrentWindVector.x * effect->mCurrentWindVector.x
                   + effect->mCurrentWindVector.z * effect->mCurrentWindVector.z;
    if ( *(float *)&v89 > 0.0001 )
    {
      v17 = effect->mCurrentWindVector.x;
      v18 = effect->mCurrentWindVector.y;
      v89 = *(sdRenderProgram **)(v10 + 92);
      t1 = angles.pitch;
      halfOpeningAngle = *(float *)&v89;
      v77 = v17;
      v19 = effect->mCurrentWindVector.z;
      atten = v18;
      min = v19;
      v20 = idRandom::RandomVectorInCone(&idRandom::staticRandom, &result, *(idVec3 *)&v77, *(float *)&v89);
      v86 = t1 * v20->x;
      v87 = v20->y * t1;
      v88 = t1 * v20->z;
      *v13 = v86 + *v13;
      v7->mVelocity.y = v87 + v7->mVelocity.y;
      v7->mVelocity.z = v7->mVelocity.z + v88;
      v16 = 0.0;
    }
  }
  if ( (*(_DWORD *)v10 >> 25) & 1 )
  {
    f = effect->mCurrentAxis.mat[2].z;
    v97 = effect->mCurrentAxis.mat[1].z;
    normal.x = effect->mCurrentAxis.mat[0].z;
    v85 = effect->mCurrentAxis.mat[2].y;
    v95 = effect->mCurrentAxis.mat[1].y;
    t1 = effect->mCurrentAxis.mat[0].y;
    v96 = effect->mCurrentAxis.mat[2].x;
    windStrength = effect->mCurrentAxis.mat[1].x;
    v89 = (sdRenderProgram *)LODWORD(effect->mCurrentAxis.mat[0].x);
    v86 = *(float *)&v89 * effect->mCurrentVelocity.x
        + t1 * effect->mCurrentVelocity.y
        + normal.x * effect->mCurrentVelocity.z;
    v87 = windStrength * effect->mCurrentVelocity.x
        + v95 * effect->mCurrentVelocity.y
        + v97 * effect->mCurrentVelocity.z;
    v88 = v96 * effect->mCurrentVelocity.x + v85 * effect->mCurrentVelocity.y + f * effect->mCurrentVelocity.z;
    *v13 = v86 + *v13;
    v7->mVelocity.y = v87 + v7->mVelocity.y;
    v7->mVelocity.z = v7->mVelocity.z + v88;
  }
  v7->mFraction = fraction;
  v7->mTextureScale = 1.0;
  v7->mTextureOffset = v16;
  if ( *(_BYTE *)(v10 + 103) )
  {
    LODWORD(f) = *(unsigned __int8 *)(v10 + 103);
    normal.x = 1.0 / (double)SLODWORD(f);
    v7->mTextureScale = normal.x;
    f = COERCE_FLOAT(rvRandom::irand(0, *(unsigned __int8 *)(v10 + 103) - 1));
    v7->mTextureOffset = (double)SLODWORD(f) * normal.x;
    v16 = 0.0;
  }
  v21 = *(_DWORD *)v10;
  v22 = (*(_DWORD *)v10 >> 11) & 1;
  v92 = (*(_DWORD *)v10 >> 12) & 1;
  v93 = v22;
  if ( v92 )
  {
    halfOpeningAngle = 0.0;
    min = COERCE_FLOAT((idVec3 *)((char *)&normal + 4));
  }
  else if ( v22 )
  {
    LODWORD(halfOpeningAngle) = v10 + 68;
    min = COERCE_FLOAT((idVec3 *)((char *)&normal + 4));
  }
  else
  {
    halfOpeningAngle = 0.0;
    min = 0.0;
    if ( (v21 >> 14) & 1 )
    {
      v23 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 116)];
      atten = *(float *)(v10 + 116);
      v23((VBRState *)&normal.y);
      v24 = effect->mCurrentAxisTransposed.mat[1].x;
      halfOpeningAngle = 0.0;
      min = 0.0;
      f = effect->mCurrentAxisTransposed.mat[2].x * v84
        + v24 * normal.z
        + normal.y * effect->mCurrentAxisTransposed.mat[0].x;
      v85 = effect->mCurrentAxisTransposed.mat[1].y * normal.z
          + effect->mCurrentAxisTransposed.mat[0].y * normal.y
          + effect->mCurrentAxisTransposed.mat[2].y * v84;
      v84 = v84 * effect->mCurrentAxisTransposed.mat[2].z
          + normal.z * effect->mCurrentAxisTransposed.mat[1].z
          + normal.y * effect->mCurrentAxisTransposed.mat[0].z;
      normal.y = f;
      normal.z = v85;
    }
    else
    {
      normal.y = 1.0;
      normal.z = v16;
      v84 = v16;
    }
  }
  rvParticle::HandleEndOrigin(
    v7,
    effect,
    (rvParticleTemplate *)v10,
    (idVec3 *)LODWORD(min),
    (idVec3 *)LODWORD(halfOpeningAngle));
  if ( (*(_DWORD *)v10 >> 19) & 1 )
  {
    f = initAxis->mat[1].x * v7->mVelocity.y
      + v7->mVelocity.x * initAxis->mat[0].x
      + initAxis->mat[2].x * v7->mVelocity.z;
    v85 = initAxis->mat[0].y * v7->mVelocity.x
        + initAxis->mat[1].y * v7->mVelocity.y
        + initAxis->mat[2].y * v7->mVelocity.z;
    v7->mVelocity.z = initAxis->mat[0].z * v7->mVelocity.x
                    + initAxis->mat[1].z * v7->mVelocity.y
                    + initAxis->mat[2].z * v7->mVelocity.z;
    *v13 = f;
    v7->mVelocity.y = v85;
  }
  v25 = idVec3::ToRadians((idVec3 *)((char *)&normal + 4), (rvAngles *)&result);
  angles.yaw = v25->pitch;
  angles.roll = v25->yaw;
  v100 = v25->roll;
  if ( v93 || v92 || (LODWORD(v26) = *(_DWORD *)v10 >> 14, LOBYTE(v26) & 1) )
  {
    f = v84 * v84 + normal.z * normal.z + normal.y * normal.y;
    f = sqrt(f);
    if ( f >= 0.00000011920929 )
    {
      f = 1.0 / f;
      normal.y = f * normal.y;
      normal.z = f * normal.z;
      v84 = f * v84;
    }
    v27 = (float *)idVec3::ToMat3((idVec3 *)((char *)&normal + 4), &v103);
    f = v27[3] * v7->mVelocity.y + v7->mVelocity.x * *v27 + v27[6] * v7->mVelocity.z;
    v85 = v27[1] * v7->mVelocity.x + v27[4] * v7->mVelocity.y + v27[7] * v7->mVelocity.z;
    v28 = v27[2] * v7->mVelocity.x + v27[5] * v7->mVelocity.y;
    v29 = v27[8] * v7->mVelocity.z;
    atten = normal.y;
    min = normal.z;
    halfOpeningAngle = v84;
    v7->mVelocity.z = v28 + v29;
    *v13 = f;
    v7->mVelocity.y = v85;
    ((void (__thiscall *)(rvParticle *, _DWORD, _DWORD, _DWORD))v7->vfptr->TransformLength)(
      v7,
      LODWORD(atten),
      LODWORD(min),
      LODWORD(halfOpeningAngle));
  }
  if ( (*(_DWORD *)v10 >> 13) & 1 )
  {
    v30 = *v13;
    halfOpeningAngle = v26;
    *v13 = v30 * -1.0;
    v7->mVelocity.y = v7->mVelocity.y * -1.0;
    v7->mVelocity.z = -1.0 * v7->mVelocity.z;
    ((void (__thiscall *)(rvParticle *, _DWORD))v7->vfptr->ScaleLength)(v7, -1.0);
  }
  f = v7->mVelocity.y * v7->mVelocity.y + *v13 * *v13 + v7->mVelocity.z * v7->mVelocity.z;
  if ( f != 0.0 )
  {
    normal.y = *v13;
    normal.z = v7->mVelocity.y;
    v84 = v7->mVelocity.z;
    f = v84 * v84 + normal.z * normal.z + normal.y * normal.y;
    f = sqrt(f);
    if ( f >= 0.00000011920929 )
    {
      f = 1.0 / f;
      normal.y = f * normal.y;
      normal.z = f * normal.z;
      v84 = f * v84;
    }
  }
  v31 = (float *)idVec3::ToMat3((idVec3 *)((char *)&normal + 4), &v103);
  v32 = v94;
  f = v31[3] * v94[1] + *v94 * *v31 + v31[6] * v94[2];
  v85 = v31[4] * v94[1] + v31[1] * *v94 + v31[7] * v94[2];
  v94[2] = v31[5] * v94[1] + v31[2] * *v94 + v31[8] * v94[2];
  *v32 = f;
  v32[1] = v85;
  f = v32[1] * v32[1] + *v32 * *v32 + v32[2] * v32[2];
  v33 = 0.0;
  if ( 0.0 != f )
  {
    normal.y = *v32;
    normal.z = v32[1];
    v84 = v32[2];
    f = v84 * v84 + normal.z * normal.z + normal.y * normal.y;
    f = sqrt(f);
    if ( f >= 0.00000011920929 )
    {
      f = 1.0 / f;
      normal.y = f * normal.y;
      normal.z = f * normal.z;
      v84 = f * v84;
    }
    v33 = 0.0;
  }
  if ( v7->mFlags & 2 )
  {
    v7->mInitAxis.mat[0].x = 1.0;
    v7->mInitAxis.mat[1].y = 1.0;
    v7->mInitAxis.mat[2].z = 1.0;
    v7->mInitAxis.mat[0].y = v33;
    v7->mInitAxis.mat[0].z = v33;
    v7->mInitAxis.mat[1].x = v33;
    v7->mInitAxis.mat[1].z = v33;
    v7->mInitAxis.mat[2].x = v33;
    v7->mInitAxis.mat[2].y = v33;
    v7->mInitEffectPos = vec3_origin;
  }
  else
  {
    qmemcpy(&v7->mInitAxis, &effect->mCurrentAxis, sizeof(v7->mInitAxis));
    halfOpeningAngle = *(float *)&effect;
    v7->mInitEffectPos.x = effect->mCurrentOrigin.x;
    v7->mInitEffectPos.y = effect->mCurrentOrigin.y;
    v7->mInitEffectPos.z = effect->mCurrentOrigin.z;
    normal.x = v7->mInitAxis.mat[2].z;
    windStrength = v7->mInitAxis.mat[1].z;
    v96 = v7->mInitAxis.mat[0].z;
    v89 = (sdRenderProgram *)LODWORD(v7->mInitAxis.mat[2].y);
    v97 = v7->mInitAxis.mat[1].y;
    f = v7->mInitAxis.mat[0].y;
    t1 = v7->mInitAxis.mat[2].x;
    v95 = v7->mInitAxis.mat[1].x;
    v85 = v7->mInitAxis.mat[0].x;
    v34 = rvBSE::GetInterpolatedOffset(effect, &result, birthTime);
    v15 = effect;
    v86 = f * v34->y + v85 * v34->x + v96 * v34->z;
    v87 = v97 * v34->y + v95 * v34->x + windStrength * v34->z;
    v88 = *(float *)&v89 * v34->y + t1 * v34->x + normal.x * v34->z;
    v7->mInitPos.x = v7->mInitPos.x - v86;
    v7->mInitPos.y = v7->mInitPos.y - v87;
    v7->mInitPos.z = v7->mInitPos.z - v88;
  }
  if ( (*(_DWORD *)v10 >> 19) & 1 )
  {
    v7->mInitPos.x = initOffset->x + v7->mInitPos.x;
    v7->mInitPos.y = initOffset->y + v7->mInitPos.y;
    v7->mInitPos.z = initOffset->z + v7->mInitPos.z;
  }
  v35 = *(unsigned __int8 **)(v10 + 132);
  v36 = *v35;
  halfOpeningAngle = 0.0;
  min = 0.0;
  atten = *(float *)&v35;
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v36])((VBRState *)&v7->mTintEnv);
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 136)])((VBRState *)&v7->mFadeEnv);
  v37 = v7->vfptr;
  f = *(float *)(v10 + 140);
  v38 = (VBRState *)v37->GetInitSize(v7);
  v39 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 140)];
  halfOpeningAngle = 0.0;
  min = 0.0;
  atten = f;
  v39(v38);
  v40 = v7->vfptr;
  f = *(float *)(v10 + 144);
  v41 = (VBRState *)v40->GetInitRotation(v7);
  v42 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 144)];
  halfOpeningAngle = 0.0;
  min = 0.0;
  atten = f;
  v42(v41);
  v43 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 148)];
  LODWORD(t1) = (char *)v7 + 168;
  v43((VBRState *)&v7->mAngleEnv);
  v44 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 152)];
  v89 = (sdRenderProgram *)&v7->mOffsetEnv;
  v44((VBRState *)&v7->mOffsetEnv);
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 192)])((VBRState *)&v7->mTintEnv.mEnd);
  v45 = *(unsigned __int8 **)(v10 + 196);
  v46 = *v45;
  halfOpeningAngle = 0.0;
  min = 0.0;
  atten = *(float *)&v45;
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v46])((VBRState *)&v7->mFadeEnv.mEnd);
  v47 = v7->vfptr->GetDestSize;
  f = *(float *)(v10 + 200);
  v48 = (VBRState *)v47(v7);
  v49 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 200)];
  halfOpeningAngle = 0.0;
  min = 0.0;
  atten = f;
  v49(v48);
  v50 = v7->vfptr;
  f = *(float *)(v10 + 204);
  v51 = (VBRState *)v50->GetDestRotation(v7);
  v52 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 204)];
  halfOpeningAngle = 0.0;
  min = 0.0;
  atten = f;
  v52(v51);
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 208)])((VBRState *)&v7->mAngleEnv.mEnd);
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v10 + 212)])((VBRState *)&v7->mOffsetEnv.mEnd);
  if ( **(_BYTE **)(v10 + 152) != 3 || **(_BYTE **)(v10 + 212) != 3 )
    v7->mFlags |= 4u;
  rvParticleParms::HandleRelativeParms(
    *(rvParticleParms **)(v10 + 192),
    &v7->mTintEnv.mEnd.x,
    &v7->mTintEnv.mStart.x,
    3);
  rvParticleParms::HandleRelativeParms(*(rvParticleParms **)(v10 + 196), &v7->mFadeEnv.mEnd, &v7->mFadeEnv.mStart, 1);
  v53 = v7->vfptr;
  LODWORD(halfOpeningAngle) = *(unsigned __int8 *)(v10 + 101);
  v54 = COERCE_FLOAT((int)v53->GetInitSize(v7));
  v55 = v7->vfptr;
  min = v54;
  v56 = v55->GetDestSize(v7);
  rvParticleParms::HandleRelativeParms(
    *(rvParticleParms **)(v10 + 200),
    v56,
    (float *)LODWORD(min),
    SLODWORD(halfOpeningAngle));
  v57 = v7->vfptr->GetInitRotation;
  LODWORD(halfOpeningAngle) = *(unsigned __int8 *)(v10 + 102);
  v58 = COERCE_FLOAT((int)v57(v7));
  v59 = v7->vfptr;
  min = v58;
  v60 = (float *)((int (__thiscall *)(rvParticle *, float))v59->GetDestRotation)(v7, COERCE_FLOAT(LODWORD(v58)));
  rvParticleParms::HandleRelativeParms(*(rvParticleParms **)(v10 + 204), v60, (float *)LODWORD(halfOpeningAngle), v81);
  rvParticleParms::HandleRelativeParms(
    *(rvParticleParms **)(v10 + 208),
    &v7->mAngleEnv.mEnd.x,
    (float *)LODWORD(v91),
    3);
  rvParticleParms::HandleRelativeParms(
    *(rvParticleParms **)(v10 + 212),
    &v7->mOffsetEnv.mEnd.x,
    (float *)LODWORD(t1),
    3);
  ((void (__thiscall *)(rvParticle *, _DWORD))v7->vfptr->ScaleRotation)(v7, LODWORD(idMath::TWO_PI));
  rvParticle::ScaleAngle(v7, idMath::TWO_PI);
  v7->vfptr->HandleOrientation(v7, (rvAngles *)((char *)&angles + 4));
  v61 = *(_DWORD *)(v10 + 104);
  halfOpeningAngle = *(float *)(v61 + 44);
  min = *(float *)(v61 + 40);
  v7->mTrailTime = rvRandom::flrand(min, halfOpeningAngle);
  v62 = rvParticleTemplate::GetTrailCount((rvParticleTemplate *)v10);
  v63 = v7->vfptr->SetModel;
  v7->mTrailCount = v62;
  v7->mTrailRepeat = *(unsigned __int8 *)(v10 + 100);
  v63(v7, *(idRenderModel **)(v10 + 16));
  v7->vfptr->SetupElectricity(v7, (rvParticleTemplate *)v10);
  normal.x = rvBSE::GetAttenuation(v15, (rvSegmentTemplate *)LODWORD(v91));
  rvParticle::Attenuate(normal.x, *(rvParticleParms **)(v10 + 136), &v7->mFadeEnv);
  ((void (__thiscall *)(rvParticle *, _DWORD, _DWORD))v7->vfptr->AttenuateSize)(
    v7,
    LODWORD(normal.x),
    *(_DWORD *)(v10 + 140));
  if ( !((*(_DWORD *)(LODWORD(v91) + 36) >> 14) & 1) || v7->mVelocity.x <= 0.0 )
  {
    f = *(float *)(v10 + 64);
    v85 = *(float *)(v10 + 60);
    v65 = rvRandom::flrand(v85, f);
    goto LABEL_61;
  }
  v86 = v15->mCurrentEndOrigin.x - v15->mOriginalOrigin.x;
  v87 = v15->mCurrentEndOrigin.y - v15->mOriginalOrigin.y;
  v88 = v15->mCurrentEndOrigin.z - v15->mOriginalOrigin.z;
  f = v86 * v86 + v87 * v87 + v88 * v88;
  f = sqrt(f);
  v91 = f;
  if ( *v94 > 0.0 )
  {
    f = v7->mVelocity.x;
    f = f * f;
    f = f - (*v94 + *v94) * v91;
    f = sqrt(f);
    v64 = f / *v94;
    f = v64 - v7->mVelocity.x;
    v91 = -v7->mVelocity.x - v64;
    if ( f >= 0.0 )
    {
      v65 = v91;
      if ( v91 >= 0.0 )
      {
        if ( f < v65 )
          v65 = f;
      }
      else
      {
        v65 = f;
      }
    }
    else
    {
      v65 = v91;
      if ( v91 < 0.0 )
        v65 = rvParticleTemplate::GetDuration((rvParticleTemplate *)v10);
    }
    goto LABEL_61;
  }
  normal.x = v91 / v7->mVelocity.x;
  if ( normal.x < 0.0 )
  {
    v65 = rvParticleTemplate::GetDuration((rvParticleTemplate *)v10);
LABEL_61:
    normal.x = v65;
  }
  halfOpeningAngle = 0.0;
  v7->mStartTime = birthTime;
  min = 0.0;
  v7->mMotionStartTime = birthTime;
  v7->mLastTrailTime = birthTime;
  v7->mEndTime = birthTime + normal.x;
  v66 = **(unsigned __int8 **)(v10 + 128);
  atten = *(float *)(v10 + 128);
  ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v66])((VBRState *)&v102);
  v68 = 0.0;
  if ( *(float *)&v102 != 0.0 )
    v68 = 1.0 / (*(float *)&v102 + normal.x);
  v7->mFriction = v68;
  halfOpeningAngle = v67;
  rvDecalParticle::InitSizeEnv(*(idJointMat **)(v10 + 164), SLODWORD(normal.x));
  rvDecalParticle::InitSizeEnv(*(idJointMat **)(v10 + 168), SLODWORD(normal.x));
  v69 = v7->vfptr->InitSizeEnv;
  halfOpeningAngle = v70;
  ((void (__thiscall *)(rvParticle *, _DWORD, _DWORD))v69)(v7, *(_DWORD *)(v10 + 172), LODWORD(normal.x));
  v71 = v7->vfptr->InitRotationEnv;
  halfOpeningAngle = v72;
  ((void (__thiscall *)(rvParticle *, _DWORD, _DWORD))v71)(v7, *(_DWORD *)(v10 + 176), LODWORD(normal.x));
  rvDecalParticle::InitSizeEnv(*(idJointMat **)(v10 + 180), SLODWORD(normal.x));
  halfOpeningAngle = v73;
  rvDecalParticle::InitSizeEnv(*(idJointMat **)(v10 + 184), SLODWORD(normal.x));
  f = *(float *)(v10 + 56);
  v85 = *(float *)(v10 + 52);
  f = rvRandom::flrand(v85, f);
  v86 = f * v15->mGravity.x;
  v87 = v15->mGravity.y * f;
  v88 = f * v15->mGravity.z;
  v74 = v87;
  v75 = v86;
  v76 = v94;
  v86 = v15->mCurrentAxisTransposed.mat[2].x * v88
      + v15->mCurrentAxisTransposed.mat[1].x * v87
      + v86 * v15->mCurrentAxisTransposed.mat[0].x;
  v87 = v15->mCurrentAxisTransposed.mat[1].y * v87
      + v15->mCurrentAxisTransposed.mat[0].y * v75
      + v15->mCurrentAxisTransposed.mat[2].y * v88;
  v88 = v88 * v15->mCurrentAxisTransposed.mat[2].z
      + v74 * v15->mCurrentAxisTransposed.mat[1].z
      + v75 * v15->mCurrentAxisTransposed.mat[0].z;
  *v94 = *v94 + v86;
  v76[1] = v87 + v76[1];
  v76[2] = v76[2] + v88;
}
void __userpurge rvLineParticle::FinishSpawn(rvLineParticle *this@<ecx>, int a2@<edi>, rvBSE *effect, rvSegment *segment, float birthTime, float fraction, idVec3 *initOffset, idMat3 *initAxis)
{
  rvLineParticle *v8; // esi
  rvSegmentTemplate *v9; // ebp
  float *v10; // eax
  float *v11; // eax
  int v12; // eax
  float *v13; // eax
  float atten; // [esp+28h] [ebp+8h]
  float attena; // [esp+28h] [ebp+8h]

  v8 = this;
  v9 = rvSegment::GetSegmentTemplate(segment);
  if ( v9 )
  {
    v10 = v8->vfptr->GetInitLength((rvParticle *)&v8->vfptr);
    rvParticle::HandleEndLength(effect, &v9->mParticleTemplate, v9->mParticleTemplate.mpSpawnLength, v10);
    v11 = v8->vfptr->GetDestLength((rvParticle *)&v8->vfptr);
    rvParticle::HandleEndLength(effect, &v9->mParticleTemplate, v9->mParticleTemplate.mpDeathLength, v11);
    rvParticle::FinishSpawn((rvParticle *)&v8->vfptr, effect, segment, birthTime, fraction, initOffset, initAxis);
    v12 = (int)v8->vfptr->GetInitLength((rvParticle *)&v8->vfptr);
    v13 = (float *)((int (__thiscall *)(rvLineParticle *, int))v8->vfptr->GetDestLength)(v8, v12);
    rvParticleParms::HandleRelativeParms(v9->mParticleTemplate.mpDeathLength, v13, (float *)3, a2);
    v8->vfptr->HandleTiling((rvParticle *)&v8->vfptr, &v9->mParticleTemplate);
    atten = rvBSE::GetAttenuation(effect, v9);
    ((void (__thiscall *)(rvLineParticle *, _DWORD, rvParticleParms *))v8->vfptr->AttenuateLength)(
      v8,
      LODWORD(atten),
      v9->mParticleTemplate.mpSpawnLength);
    attena = v8->mEndTime - v8->mStartTime;
    rvDecalParticle::InitSizeEnv((idJointMat *)v9->mParticleTemplate.mpLengthEnvelope, SLODWORD(attena));
  }
}
void __thiscall rvLinkedParticle::FinishSpawn(rvLinkedParticle *this, rvBSE *effect, rvSegment *segment, float birthTime, float fraction, idVec3 *initOffset, idMat3 *initAxis)
{
  rvLinkedParticle *v7; // esi
  rvSegmentTemplate *v8; // edi

  v7 = this;
  v8 = rvSegment::GetSegmentTemplate(segment);
  if ( v8 )
  {
    rvParticle::FinishSpawn((rvParticle *)&v7->vfptr, effect, segment, birthTime, fraction, initOffset, initAxis);
    v7->vfptr->HandleTiling((rvParticle *)&v7->vfptr, &v8->mParticleTemplate);
  }
}
void __thiscall rvDebrisParticle::FinishSpawn(rvDebrisParticle *this, rvBSE *effect, rvSegment *segment, float birthTime, float fraction, idVec3 *initOffset, idMat3 *initAxis)
{
  rvDebrisParticle *v7; // ebx
  rvSegmentTemplate *v8; // eax
  rvParticleTemplate *v9; // edi
  rvParticleParms *v10; // eax
  int v11; // edx
  void (__cdecl *v12)(VBRState *); // eax
  float *v13; // esi
  float v14; // ecx
  float *v15; // eax
  double v16; // st7
  double v17; // st6
  double v18; // st7
  idVec3 *v19; // eax
  int v20; // esi
  float v21; // edi
  VBRState *v22; // eax
  int v23; // edx
  rvParticleVtbl *v24; // edx
  VBRState *v25; // eax
  void (__cdecl *v26)(VBRState *); // edx
  void (__thiscall *v27)(rvParticle *, rvEnvParms *, float); // edx
  float v28; // ecx
  double v29; // st6
  double v30; // st5
  idGameVtbl *v31; // edx
  float v32; // [esp+18h] [ebp-D8h]
  float min; // [esp+1Ch] [ebp-D4h]
  float time; // [esp+20h] [ebp-D0h]
  signed int v35; // [esp+34h] [ebp-BCh]
  idVec3 normal; // [esp+38h] [ebp-B8h]
  float v37; // [esp+44h] [ebp-ACh]
  float v38; // [esp+48h] [ebp-A8h]
  float v39; // [esp+4Ch] [ebp-A4h]
  float v40; // [esp+50h] [ebp-A0h]
  float v41; // [esp+54h] [ebp-9Ch]
  float v42; // [esp+58h] [ebp-98h]
  idVec3 velocity; // [esp+5Ch] [ebp-94h]
  rvParticleTemplate *pt; // [esp+68h] [ebp-88h]
  rvParticleTemplate *v45; // [esp+6Ch] [ebp-84h]
  idVec3 v46; // [esp+70h] [ebp-80h]
  float v47; // [esp+7Ch] [ebp-74h]
  idVec3 angularVel; // [esp+80h] [ebp-70h]
  float v49; // [esp+8Ch] [ebp-64h]
  idVec3 origin; // [esp+90h] [ebp-60h]
  float v51; // [esp+9Ch] [ebp-54h]
  float v52; // [esp+A0h] [ebp-50h]
  idMat3 axis; // [esp+A4h] [ebp-4Ch]
  float v54; // [esp+C8h] [ebp-28h]
  idMat3 result; // [esp+CCh] [ebp-24h]

  v7 = this;
  if ( bse_debris.internalVar->integerValue )
  {
    v8 = rvSegment::GetSegmentTemplate(segment);
    if ( v8 )
    {
      v9 = &v8->mParticleTemplate;
      v7->mFlags = v8->mParticleTemplate.mFlags;
      v10 = v8->mParticleTemplate.mpSpawnVelocity;
      v11 = (unsigned __int8)v10->mSpawnType;
      time = 0.0;
      min = 0.0;
      v32 = *(float *)&v10;
      v12 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v11];
      v13 = &v7->mVelocity.x;
      v45 = v9;
      v12((VBRState *)&v7->mVelocity);
      v7->mFraction = fraction;
      v7->mTextureScale = 1.0;
      v7->mTextureOffset = 0.0;
      if ( ((unsigned int)v9->mFlags >> 12) & 1 )
      {
        rvParticle::HandleEndOrigin((rvParticle *)&v7->vfptr, effect, v9, (idVec3 *)((char *)&normal + 4), 0);
      }
      else if ( ((unsigned int)v9->mFlags >> 11) & 1 )
      {
        rvParticle::HandleEndOrigin((rvParticle *)&v7->vfptr, effect, v9, (idVec3 *)((char *)&normal + 4), &v9->mCentre);
      }
      else
      {
        rvParticle::HandleEndOrigin((rvParticle *)&v7->vfptr, effect, v9, 0, 0);
        normal.y = 1.0;
        normal.z = 0.0;
        v37 = 0.0;
      }
      if ( ((unsigned int)v9->mFlags >> 11) & 1 || ((unsigned int)v9->mFlags >> 12) & 1 )
      {
        *(float *)&v35 = normal.y * normal.y + normal.z * normal.z + v37 * v37;
        *(float *)&v35 = sqrt(*(float *)&v35);
        if ( *(float *)&v35 >= 0.00000011920929 )
        {
          *(float *)&v35 = 1.0 / *(float *)&v35;
          normal.y = normal.y * *(float *)&v35;
          normal.z = normal.z * *(float *)&v35;
          v37 = *(float *)&v35 * v37;
        }
        v15 = (float *)idVec3::ToMat3((idVec3 *)((char *)&normal + 4), &result);
        *(float *)&v35 = v15[3] * v7->mVelocity.y + v7->mVelocity.x * *v15 + v15[6] * v7->mVelocity.z;
        normal.x = v15[4] * v7->mVelocity.y + v15[1] * v7->mVelocity.x + v15[7] * v7->mVelocity.z;
        v16 = v15[5] * v7->mVelocity.y + v15[2] * v7->mVelocity.x;
        v17 = v15[8] * v7->mVelocity.z;
        v32 = normal.y;
        min = normal.z;
        time = v37;
        v7->mVelocity.z = v16 + v17;
        *v13 = *(float *)&v35;
        v7->mVelocity.y = normal.x;
        ((void (__thiscall *)(rvDebrisParticle *, float, _DWORD, _DWORD))v7->vfptr->TransformLength)(
          v7,
          COERCE_FLOAT(LODWORD(v32)),
          LODWORD(min),
          LODWORD(time));
      }
      if ( ((unsigned int)v9->mFlags >> 13) & 1 )
      {
        v18 = *v13;
        time = v14;
        *v13 = v18 * -1.0;
        v7->mVelocity.y = v7->mVelocity.y * -1.0;
        v7->mVelocity.z = -1.0 * v7->mVelocity.z;
        ((void (__thiscall *)(rvDebrisParticle *, _DWORD))v7->vfptr->ScaleLength)(v7, -1.0);
      }
      normal.x = v7->mVelocity.y * v7->mVelocity.y + *v13 * *v13 + v7->mVelocity.z * v7->mVelocity.z;
      if ( normal.x != 0.0 )
      {
        normal.y = *v13;
        normal.z = v7->mVelocity.y;
        v37 = v7->mVelocity.z;
        normal.x = normal.y * normal.y + normal.z * normal.z + v37 * v37;
        normal.x = sqrt(normal.x);
        if ( normal.x >= 0.00000011920929 )
        {
          normal.x = 1.0 / normal.x;
          normal.y = normal.x * normal.y;
          normal.z = normal.z * normal.x;
          v37 = normal.x * v37;
        }
      }
      LODWORD(v39) = (char *)effect + 300;
      qmemcpy(&v7->mInitAxis, &effect->mCurrentAxis, sizeof(v7->mInitAxis));
      v7->mInitEffectPos.x = effect->mCurrentOrigin.x;
      v7->mInitEffectPos.y = effect->mCurrentOrigin.y;
      v7->mInitEffectPos.z = effect->mCurrentOrigin.z;
      v38 = v7->mInitAxis.mat[2].z;
      axis.mat[0].x = v7->mInitAxis.mat[1].z;
      v47 = v7->mInitAxis.mat[0].z;
      time = 0.0;
      angularVel.x = v7->mInitAxis.mat[2].y;
      v52 = v7->mInitAxis.mat[1].y;
      normal.x = v7->mInitAxis.mat[0].y;
      velocity.x = v7->mInitAxis.mat[2].x;
      origin.x = v7->mInitAxis.mat[1].x;
      v35 = SLODWORD(v7->mInitAxis.mat[0].x);
      v19 = rvBSE::GetInterpolatedOffset(effect, &v46, birthTime);
      v20 = (int)v45;
      v40 = v19->y * normal.x + v19->x * *(float *)&v35 + v19->z * v47;
      v41 = v19->y * v52 + v19->x * origin.x + v19->z * axis.mat[0].x;
      v42 = v19->y * angularVel.x + v19->x * velocity.x + v19->z * v38;
      v7->mInitPos.x = v7->mInitPos.x - v40;
      v7->mInitPos.y = v7->mInitPos.y - v41;
      v7->mInitPos.z = v7->mInitPos.z - v42;
      v21 = *(float *)(v20 + 144);
      v22 = (VBRState *)v7->vfptr->GetInitRotation((rvParticle *)&v7->vfptr);
      v23 = **(unsigned __int8 **)(v20 + 144);
      time = 0.0;
      min = 0.0;
      v32 = v21;
      ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[v23])(v22);
      ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v20 + 148)])((VBRState *)&v7->mAngleEnv);
      v24 = v7->vfptr;
      v38 = *(float *)(v20 + 204);
      v25 = (VBRState *)v24->GetDestRotation((rvParticle *)&v7->vfptr);
      v26 = (void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[**(unsigned __int8 **)(v20 + 204)];
      time = 0.0;
      min = 0.0;
      v32 = v38;
      v26(v25);
      ((void (__thiscall *)(rvDebrisParticle *, _DWORD))v7->vfptr->ScaleRotation)(v7, LODWORD(idMath::TWO_PI));
      rvParticle::ScaleAngle((rvParticle *)&v7->vfptr, idMath::TWO_PI);
      v27 = v7->vfptr->InitRotationEnv;
      time = v28;
      ((void (__thiscall *)(rvDebrisParticle *, _DWORD, _DWORD))v27)(v7, *(_DWORD *)(v20 + 176), 1.0);
      rvDecalParticle::InitSizeEnv(*(idJointMat **)(v20 + 180), COERCE_CONST_INT(1.0));
      v7->mStartTime = birthTime;
      v7->mTrailCount = 0;
      v7->mMotionStartTime = birthTime;
      v7->mLastTrailTime = birthTime;
      v7->mEndTime = birthTime;
      v7->mTrailTime = 0.0;
      v38 = *(float *)(v20 + 56);
      velocity.x = *(float *)(v20 + 52);
      v38 = rvRandom::flrand(velocity.x, v38);
      v40 = effect->mGravity.x * v38;
      v41 = effect->mGravity.y * v38;
      v42 = v38 * effect->mGravity.z;
      v29 = v41;
      v30 = v40;
      v40 = effect->mCurrentAxisTransposed.mat[2].x * v42
          + effect->mCurrentAxisTransposed.mat[1].x * v41
          + v40 * effect->mCurrentAxisTransposed.mat[0].x;
      v41 = effect->mCurrentAxisTransposed.mat[1].y * v41
          + effect->mCurrentAxisTransposed.mat[0].y * v30
          + effect->mCurrentAxisTransposed.mat[2].y * v42;
      v42 = v42 * effect->mCurrentAxisTransposed.mat[2].z
          + v29 * effect->mCurrentAxisTransposed.mat[1].z
          + v30 * effect->mCurrentAxisTransposed.mat[0].z;
      v7->mAcceleration.x = v40 + v7->mAcceleration.x;
      v7->mAcceleration.y = v41 + v7->mAcceleration.y;
      v7->mAcceleration.z = v7->mAcceleration.z + v42;
      v40 = effect->mOriginalAxis.mat[1].x * v7->mInitPos.y
          + v7->mInitPos.x * effect->mOriginalAxis.mat[0].x
          + effect->mOriginalAxis.mat[2].x * v7->mInitPos.z;
      v41 = effect->mOriginalAxis.mat[1].y * v7->mInitPos.y
          + effect->mOriginalAxis.mat[0].y * v7->mInitPos.x
          + effect->mOriginalAxis.mat[2].y * v7->mInitPos.z;
      v42 = effect->mOriginalAxis.mat[1].z * v7->mInitPos.y
          + effect->mOriginalAxis.mat[0].z * v7->mInitPos.x
          + effect->mOriginalAxis.mat[2].z * v7->mInitPos.z;
      v46.x = v40 + effect->mOriginalOrigin.x;
      v46.y = effect->mOriginalOrigin.y + v41;
      qmemcpy(&axis.mat[0].y, (const void *)LODWORD(v39), sizeof(idMat3));
      v46.z = effect->mOriginalOrigin.z + v42;
      origin.y = v46.x;
      origin.z = v46.y;
      v51 = v46.z;
      velocity.y = v7->mVelocity.x;
      velocity.z = v7->mVelocity.y;
      pt = (rvParticleTemplate *)LODWORD(v7->mVelocity.z);
      v39 = axis.mat[2].y * *(float *)&pt + axis.mat[0].y * velocity.y + axis.mat[1].y * velocity.z;
      v38 = axis.mat[1].z * velocity.z + axis.mat[0].z * velocity.y + axis.mat[2].z * *(float *)&pt;
      *(float *)&pt = velocity.y * axis.mat[1].x + velocity.z * axis.mat[2].x + *(float *)&pt * v54;
      velocity.y = v39;
      velocity.z = v38;
      angularVel.y = v7->mRotationEnv.mEnd.x;
      angularVel.z = v7->mRotationEnv.mEnd.y;
      v49 = v7->mRotationEnv.mEnd.z;
      v39 = v7->mEndTime - v7->mStartTime;
      v39 = v39 * 1000.0;
      v35 = (signed int)v39;
      v31 = game->vfptr;
      time = 0.0;
      min = COERCE_FLOAT((idVec3 *)((char *)&angularVel + 4));
      v32 = COERCE_FLOAT((idVec3 *)((char *)&velocity + 4));
      ((void (__stdcall *)(char *, signed int, float *, float *, float *, float *, _DWORD))v31->SpawnClientMoveable)(
        v45->mEntityDefName.data,
        v35,
        &origin.y,
        &axis.mat[0].y,
        &velocity.y,
        &angularVel.y,
        0);
    }
  }
}
void __thiscall rvLineParticle::HandleTiling(rvLineParticle *this, rvParticleTemplate *pt)
{
  rvLineParticle *v2; // esi
  float *v3; // eax
  float len; // ST04_4
  float v5; // ST04_4

  v2 = this;
  if ( this->mFlags & 0x100000 )
  {
    v3 = (float *)((int (*)(void))this->vfptr->GetInitLength)();
    len = *v3 * *v3 + v3[1] * v3[1] + v3[2] * v3[2];
    v5 = sqrt(len);
    v2->mTextureScale = v5 / pt->mTiling;
    v2->mTextureOffset = 0.0;
  }
}
void __thiscall rvLineParticle::Refresh(rvLineParticle *this, rvBSE *effect, rvSegmentTemplate *st, rvParticleTemplate *pt)
{
  rvLineParticle *v4; // esi
  VBRState *v5; // eax
  rvParticleTemplate *v6; // ebp
  rvParticleParms *v7; // ecx
  VBRState *v8; // eax
  rvParticleParms *v9; // ecx
  int v10; // eax
  float *v11; // eax
  int v12; // [esp+8h] [ebp-Ch]
  int pta; // [esp+20h] [ebp+Ch]

  v4 = this;
  v5 = (VBRState *)((int (*)(void))this->vfptr->GetInitLength)();
  v6 = pt;
  v7 = pt->mpSpawnLength;
  if ( ((unsigned int)effect->mFlags >> 1) & 1 && v7->mFlags & 2 )
    rvParticle::SetLengthUsingEndOrigin(effect, v7, &v5->energy_alpha);
  else
    ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[(unsigned __int8)v7->mSpawnType])(v5);
  v8 = (VBRState *)v4->vfptr->GetDestLength((rvParticle *)&v4->vfptr);
  v9 = pt->mpDeathLength;
  if ( ((unsigned int)effect->mFlags >> 1) & 1 && pt->mpSpawnLength->mFlags & 2 )
    rvParticle::SetLengthUsingEndOrigin(effect, v9, &v8->energy_alpha);
  else
    ((void (__cdecl *)(VBRState *))rvParticleParms::spawnFunctions[(unsigned __int8)v9->mSpawnType])(v8);
  v10 = (int)v4->vfptr->GetInitLength((rvParticle *)&v4->vfptr);
  v11 = (float *)((int (__thiscall *)(rvLineParticle *, int))v4->vfptr->GetDestLength)(v4, v10);
  rvParticleParms::HandleRelativeParms(pt->mpDeathLength, v11, (float *)3, v12);
  v4->vfptr->HandleTiling((rvParticle *)&v4->vfptr, pt);
  *(float *)&pta = v4->mEndTime - v4->mStartTime;
  rvDecalParticle::InitSizeEnv((idJointMat *)v6->mpLengthEnvelope, pta);
}
void __thiscall rvSpriteParticle::GetSpawnInfo(rvSpriteParticle *this, idVec4 *tint, idVec3 *size, idVec3 *rotate)
{
  tint->x = this->mTintEnv.mStart.x;
  tint->y = this->mTintEnv.mStart.y;
  tint->z = this->mTintEnv.mStart.z;
  tint->w = this->mTintEnv.mEnd.x;
  size->x = this->mSizeEnv.mStart.x;
  size->y = this->mSizeEnv.mStart.y;
  size->z = 0.0;
  rotate->x = this->mRotationEnv.mStart;
  rotate->y = 0.0;
  rotate->z = 0.0;
}
void __thiscall rvLineParticle::GetSpawnInfo(rvLineParticle *this, idVec4 *tint, idVec3 *size, idVec3 *rotate)
{
  tint->x = this->mTintEnv.mStart.x;
  tint->y = this->mTintEnv.mStart.y;
  tint->z = this->mTintEnv.mStart.z;
  tint->w = this->mTintEnv.mEnd.x;
  size->x = this->mSizeEnv.mStart;
  size->y = 0.0;
  size->z = 0.0;
  rotate->x = 0.0;
  rotate->y = 0.0;
  rotate->z = 0.0;
}
void __thiscall rvOrientedParticle::GetSpawnInfo(rvOrientedParticle *this, idVec4 *tint, idVec3 *size, idVec3 *rotate)
{
  size->z = 0.0;
  size->y = 0.0;
  size->x = 0.0;
  tint->x = this->mTintEnv.mStart.x;
  tint->y = this->mTintEnv.mStart.y;
  tint->z = this->mTintEnv.mStart.z;
  tint->w = this->mTintEnv.mEnd.x;
  size->x = this->mSizeEnv.mStart.x;
  size->y = this->mSizeEnv.mStart.y;
  size->z = 0.0;
  rotate->x = this->mRotationEnv.mStart.x;
  rotate->y = this->mRotationEnv.mStart.y;
  rotate->z = this->mRotationEnv.mStart.z;
}
void __thiscall rvModelParticle::GetSpawnInfo(rvModelParticle *this, idVec4 *tint, idVec3 *size, idVec3 *rotate)
{
  tint->x = this->mTintEnv.mStart.x;
  tint->y = this->mTintEnv.mStart.y;
  tint->z = this->mTintEnv.mStart.z;
  tint->w = this->mTintEnv.mEnd.x;
  size->x = this->mSizeEnv.mStart.x;
  size->y = this->mSizeEnv.mStart.y;
  size->z = this->mSizeEnv.mStart.z;
  rotate->x = this->mRotationEnv.mStart.x;
  rotate->y = this->mRotationEnv.mStart.y;
  rotate->z = this->mRotationEnv.mStart.z;
}
void __thiscall rvLightParticle::GetSpawnInfo(rvLightParticle *this, idVec4 *tint, idVec3 *size, idVec3 *rotate)
{
  tint->x = this->mTintEnv.mStart.x;
  tint->y = this->mTintEnv.mStart.y;
  tint->z = this->mTintEnv.mStart.z;
  tint->w = this->mTintEnv.mEnd.x;
  size->x = this->mSizeEnv.mStart.x;
  size->y = this->mSizeEnv.mStart.y;
  size->z = this->mSizeEnv.mStart.z;
  rotate->x = 0.0;
  rotate->y = 0.0;
  rotate->z = 0.0;
}
void __thiscall rvDecalParticle::GetSpawnInfo(rvDecalParticle *this, idVec4 *tint, idVec3 *size, idVec3 *rotate)
{
  tint->x = this->mTintEnv.mStart.x;
  tint->y = this->mTintEnv.mStart.y;
  tint->z = this->mTintEnv.mStart.z;
  tint->w = this->mTintEnv.mEnd.x;
  size->x = this->mSizeEnv.mStart.x;
  size->y = this->mSizeEnv.mStart.y;
  size->z = this->mSizeEnv.mStart.z;
  rotate->x = this->mRotationEnv.mStart;
  rotate->y = 0.0;
  rotate->z = 0.0;
}
rvDebrisParticle *__thiscall rvDebrisParticle::GetArrayEntry(rvDebrisParticle *this, int i)
{
  rvDebrisParticle *result; // eax

  if ( i < 0 )
    result = 0;
  else
    result = &this[i];
  return result;
}
rvOrientedParticle *__thiscall rvOrientedParticle::GetArrayEntry(rvOrientedParticle *this, int i)
{
  rvOrientedParticle *result; // eax

  if ( i < 0 )
    result = 0;
  else
    result = &this[i];
  return result;
}
rvElectricityParticle *__thiscall rvElectricityParticle::GetArrayEntry(rvElectricityParticle *this, int i)
{
  rvElectricityParticle *result; // eax

  if ( i < 0 )
    result = 0;
  else
    result = &this[i];
  return result;
}
rvDecalParticle *__thiscall rvDecalParticle::GetArrayEntry(rvDecalParticle *this, int i)
{
  rvDecalParticle *result; // eax

  if ( i < 0 )
    result = 0;
  else
    result = &this[i];
  return result;
}
rvModelParticle *__thiscall rvModelParticle::GetArrayEntry(rvModelParticle *this, int i)
{
  rvModelParticle *result; // eax

  if ( i < 0 )
    result = 0;
  else
    result = &this[i];
  return result;
}
rvLightParticle *__thiscall rvLightParticle::GetArrayEntry(rvLightParticle *this, int i)
{
  rvLightParticle *result; // eax

  if ( i < 0 )
    result = 0;
  else
    result = &this[i];
  return result;
}
rvLinkedParticle *__thiscall rvLinkedParticle::GetArrayEntry(rvLinkedParticle *this, int i)
{
  rvLinkedParticle *result; // eax

  if ( i < 0 )
    result = 0;
  else
    result = &this[i];
  return result;
}
void __thiscall rvParticle::Bounce(rvParticle *this, rvBSE *effect, rvParticleTemplate *pt, idVec3 endPos, idVec3 normal, float time)
{
  float v6; // ST0C_4
  float *v7; // edx
  int v8; // ecx
  float v9; // ST10_4
  float v10; // ST14_4
  double v11; // st5
  double v12; // st4
  double v13; // st3
  float v14; // ST10_4
  float v15; // ST14_4
  float v16; // ST10_4
  float v17; // ST14_4
  idVec3 oldVelocity; // [esp+10h] [ebp-10h]
  float v19; // [esp+1Ch] [ebp-4h]
  float effecta; // [esp+24h] [ebp+4h]
  float effectb; // [esp+24h] [ebp+4h]
  float effectc; // [esp+24h] [ebp+4h]
  float endPosa; // [esp+2Ch] [ebp+Ch]
  float endPos_4; // [esp+30h] [ebp+10h]
  float endPos_8; // [esp+34h] [ebp+14h]
  float timea; // [esp+44h] [ebp+24h]
  float timeb; // [esp+44h] [ebp+24h]

  v6 = time - this->mMotionStartTime;
  rvParticle::EvaluateVelocity(this, effect, (idVec3 *)((char *)&oldVelocity + 4), v6);
  v9 = v7[81] * v19 + v7[75] * oldVelocity.y + v7[78] * oldVelocity.z;
  v10 = v7[79] * oldVelocity.z + v7[76] * oldVelocity.y + v7[82] * v19;
  oldVelocity.x = oldVelocity.y * v7[77] + oldVelocity.z * v7[80] + v19 * v7[83];
  v11 = v10;
  v12 = v9;
  v13 = oldVelocity.x;
  effecta = normal.z * oldVelocity.x + v9 * normal.x + normal.y * v10;
  effectb = effecta + effecta;
  v14 = normal.x * effectb;
  v15 = normal.y * effectb;
  oldVelocity.x = effectb * normal.z;
  oldVelocity.y = v12 - v14;
  oldVelocity.z = v11 - v15;
  v19 = v13 - oldVelocity.x;
  effectc = pt->mBounce;
  v16 = oldVelocity.y * effectc;
  v17 = oldVelocity.z * effectc;
  oldVelocity.x = effectc * v19;
  oldVelocity.y = v7[90] * oldVelocity.x + v7[87] * v17 + v16 * v7[84];
  oldVelocity.z = v7[88] * v17 + v7[85] * v16 + v7[91] * oldVelocity.x;
  v19 = oldVelocity.x * v7[92] + v17 * v7[89] + v16 * v7[86];
  *(float *)(v8 + 108) = oldVelocity.y;
  *(float *)(v8 + 112) = oldVelocity.z;
  *(float *)(v8 + 116) = v19;
  *(float *)(v8 + 8) = time;
  oldVelocity.y = endPos.x - v7[66];
  oldVelocity.z = endPos.y - v7[67];
  v19 = endPos.z - v7[68];
  endPosa = v7[90] * v19 + v7[84] * oldVelocity.y + v7[87] * oldVelocity.z;
  endPos_4 = v7[88] * oldVelocity.z + v7[85] * oldVelocity.y + v7[91] * v19;
  endPos_8 = oldVelocity.y * v7[86] + oldVelocity.z * v7[89] + v19 * v7[92];
  *(float *)(v8 + 96) = endPosa;
  *(float *)(v8 + 100) = endPos_4;
  *(float *)(v8 + 104) = endPos_8;
  timea = *(float *)(v8 + 112) * *(float *)(v8 + 112)
        + *(float *)(v8 + 108) * *(float *)(v8 + 108)
        + *(float *)(v8 + 116) * *(float *)(v8 + 116);
  if ( timea < 2500.0 )
  {
    timeb = normal.y * v7[60] + normal.x * v7[59] + v7[61] * normal.z;
    if ( -1.0 / 1.4142135 > timeb )
    {
      *(_DWORD *)(v8 + 16) |= 1u;
      *(float *)(v8 + 116) = 0.0;
      *(float *)(v8 + 112) = 0.0;
      *(float *)(v8 + 108) = 0.0;
    }
  }
}
// local variable allocation has failed, the output may be wrong!
void __thiscall rvParticle::EvaluatePosition(rvParticle *this, rvBSE *effect, rvParticleTemplate *pt, idVec3 *pos, float time)
{
  rvParticle *v5; // esi
  idVec3 *v6; // edi
  double v7; // st7
  float v8; // ST1C_4
  float v9; // ST20_4
  double v10; // st3
  double v11; // st7
  rvBSE *v12; // eax
  double v13; // st4
  double v14; // st3
  float v15; // [esp+20h] [ebp-54h]
  float v16; // [esp+24h] [ebp-50h]
  float v17; // [esp+28h] [ebp-4Ch]
  float *v18; // [esp+2Ch] [ebp-48h]
  double v19; // [esp+30h] [ebp-44h] OVERLAPPED
  idVec3 offset; // [esp+34h] [ebp-40h]
  rvAngles angle; // [esp+40h] [ebp-34h]
  idMat3 mat; // [esp+4Ch] [ebp-28h]
  float v23; // [esp+70h] [ebp-4h]
  float pta; // [esp+7Ch] [ebp+8h]
  float ptb; // [esp+7Ch] [ebp+8h]
  float ooduration; // [esp+80h] [ebp+Ch]
  float oodurationa; // [esp+80h] [ebp+Ch]
  float oodurationb; // [esp+80h] [ebp+Ch]
  float oodurationc; // [esp+80h] [ebp+Ch]
  float oodurationd; // [esp+80h] [ebp+Ch]
  float timea; // [esp+84h] [ebp+10h]
  float timeb; // [esp+84h] [ebp+10h]
  float timec; // [esp+84h] [ebp+10h]

  v5 = this;
  v6 = pos;
  if ( this->mFlags & 1 )
  {
    pos->x = this->mInitPos.x;
    pos->y = this->mInitPos.y;
    v11 = this->mInitPos.z;
  }
  else
  {
    timea = time - time * (this->mFriction * 0.5) * time;
    v7 = timea;
    angle.yaw = timea * this->mVelocity.x;
    angle.roll = this->mVelocity.y * timea;
    mat.mat[0].x = this->mVelocity.z * timea;
    offset.y = this->mInitPos.x + angle.yaw;
    offset.z = this->mInitPos.y + angle.roll;
    angle.pitch = this->mInitPos.z + mat.mat[0].x;
    pos->x = offset.y;
    pos->y = offset.z;
    pos->z = angle.pitch;
    if ( this->mFlags & 4 )
    {
      ooduration = this->mEndTime - this->mStartTime;
      oodurationa = 1.0 / ooduration;
      rvEnvParms::Evaluate(pt->mpAngleEnvelope, &this->mAngleEnv, timea, oodurationa, &angle.yaw);
      rvEnvParms::Evaluate(pt->mpOffsetEnvelope, &v5->mOffsetEnv, timea, oodurationa, &offset.y);
      v8 = cos(angle.roll);
      v9 = sin(angle.roll);
      pta = cos(angle.yaw);
      oodurationb = sin(angle.yaw);
      v17 = mat.mat[0].x;
      v18 = &v16;
      v16 = cos(mat.mat[0].x);
      v15 = sin(mat.mat[0].x);
      mat.mat[0].y = pta * v8;
      mat.mat[0].z = pta * v9;
      mat.mat[1].x = -oodurationb;
      v10 = oodurationb * v15;
      mat.mat[1].y = v10 * v8 - v16 * v9;
      mat.mat[1].z = v16 * v8 + v9 * v10;
      mat.mat[2].x = v15 * pta;
      v19 = v16 * oodurationb;
      mat.mat[2].y = v19 * v8 - -v9 * v15;
      mat.mat[2].z = v9 * v19 - v8 * v15;
      v23 = pta * v16;
      oodurationc = mat.mat[2].y * angle.pitch + mat.mat[1].y * offset.z + mat.mat[0].y * offset.y;
      ptb = mat.mat[0].z * offset.y + mat.mat[1].z * offset.z + mat.mat[2].z * angle.pitch;
      angle.pitch = angle.pitch * v23 + offset.y * mat.mat[1].x + offset.z * mat.mat[2].x;
      v6->x = v6->x + oodurationc;
      v6->y = v6->y + ptb;
      v6->z = angle.pitch + v6->z;
      v7 = timea;
    }
    timeb = v7 * (0.5 * v7);
    angle.yaw = v5->mAcceleration.x * timeb;
    angle.roll = v5->mAcceleration.y * timeb;
    mat.mat[0].x = timeb * v5->mAcceleration.z;
    v6->x = v6->x + angle.yaw;
    v6->y = v6->y + angle.roll;
    v11 = mat.mat[0].x + v6->z;
  }
  v6->z = v11;
  if ( !(v5->mFlags & 2) )
  {
    v12 = effect;
    mat.mat[0].y = effect->mCurrentAxis.mat[0].y * v5->mInitAxis.mat[0].y
                 + effect->mCurrentAxis.mat[0].x * v5->mInitAxis.mat[0].x
                 + effect->mCurrentAxis.mat[0].z * v5->mInitAxis.mat[0].z;
    mat.mat[0].z = effect->mCurrentAxis.mat[1].y * v5->mInitAxis.mat[0].y
                 + effect->mCurrentAxis.mat[1].x * v5->mInitAxis.mat[0].x
                 + v5->mInitAxis.mat[0].z * effect->mCurrentAxis.mat[1].z;
    mat.mat[1].x = v5->mInitAxis.mat[0].x * effect->mCurrentAxis.mat[2].x
                 + effect->mCurrentAxis.mat[2].y * v5->mInitAxis.mat[0].y
                 + effect->mCurrentAxis.mat[2].z * v5->mInitAxis.mat[0].z;
    mat.mat[1].y = effect->mCurrentAxis.mat[0].y * v5->mInitAxis.mat[1].y
                 + v5->mInitAxis.mat[1].x * effect->mCurrentAxis.mat[0].x
                 + effect->mCurrentAxis.mat[0].z * v5->mInitAxis.mat[1].z;
    mat.mat[1].z = effect->mCurrentAxis.mat[1].y * v5->mInitAxis.mat[1].y
                 + effect->mCurrentAxis.mat[1].x * v5->mInitAxis.mat[1].x
                 + effect->mCurrentAxis.mat[1].z * v5->mInitAxis.mat[1].z;
    mat.mat[2].x = v5->mInitAxis.mat[1].x * effect->mCurrentAxis.mat[2].x
                 + effect->mCurrentAxis.mat[2].y * v5->mInitAxis.mat[1].y
                 + effect->mCurrentAxis.mat[2].z * v5->mInitAxis.mat[1].z;
    mat.mat[2].y = effect->mCurrentAxis.mat[0].x * v5->mInitAxis.mat[2].x
                 + v5->mInitAxis.mat[2].y * effect->mCurrentAxis.mat[0].y
                 + v5->mInitAxis.mat[2].z * effect->mCurrentAxis.mat[0].z;
    mat.mat[2].z = effect->mCurrentAxis.mat[1].x * v5->mInitAxis.mat[2].x
                 + v5->mInitAxis.mat[2].y * effect->mCurrentAxis.mat[1].y
                 + v5->mInitAxis.mat[2].z * effect->mCurrentAxis.mat[1].z;
    v23 = effect->mCurrentAxis.mat[2].y * v5->mInitAxis.mat[2].y
        + effect->mCurrentAxis.mat[2].x * v5->mInitAxis.mat[2].x
        + effect->mCurrentAxis.mat[2].z * v5->mInitAxis.mat[2].z;
    timec = v6->y * mat.mat[1].y + mat.mat[0].y * v6->x + mat.mat[2].y * v6->z;
    oodurationd = v6->y * mat.mat[1].z + mat.mat[0].z * v6->x + mat.mat[2].z * v6->z;
    v6->z = v6->y * mat.mat[2].x + mat.mat[1].x * v6->x + v23 * v6->z;
    v6->x = timec;
    v6->y = oodurationd;
    angle.yaw = v5->mInitEffectPos.x - effect->mCurrentOrigin.x;
    angle.roll = v5->mInitEffectPos.y - effect->mCurrentOrigin.y;
    mat.mat[0].x = v5->mInitEffectPos.z - effect->mCurrentOrigin.z;
    v13 = angle.roll;
    v14 = angle.yaw;
    angle.yaw = v12->mCurrentAxisTransposed.mat[2].x * mat.mat[0].x
              + v12->mCurrentAxisTransposed.mat[1].x * angle.roll
              + angle.yaw * v12->mCurrentAxisTransposed.mat[0].x;
    angle.roll = v12->mCurrentAxisTransposed.mat[0].y * v14
               + v12->mCurrentAxisTransposed.mat[1].y * angle.roll
               + effect->mCurrentAxisTransposed.mat[2].y * mat.mat[0].x;
    mat.mat[0].x = mat.mat[0].x * effect->mCurrentAxisTransposed.mat[2].z
                 + v13 * effect->mCurrentAxisTransposed.mat[1].z
                 + v14 * effect->mCurrentAxisTransposed.mat[0].z;
    v6->x = timec + angle.yaw;
    v6->y = oodurationd + angle.roll;
    v6->z = mat.mat[0].x + v6->z;
  }
  v5->mPosition.x = v6->x;
  v5->mPosition.y = v6->y;
  v5->mPosition.z = v6->z;
}
void __thiscall rvParticle::CheckTimeoutEffect(rvParticle *this, rvBSE *effect, rvSegmentTemplate *st, float time)
{
  rvParticle *v4; // edi
  int v5; // eax
  rvParticleTemplate *v6; // ebx
  int v7; // ebp
  rvDeclEffect *v8; // ebx
  int v9; // eax
  idGameVtbl *v10; // ebx
  int v11; // edi
  idMat3 *v12; // eax
  int v13; // [esp+Ch] [ebp-50h]
  idVec3 velocity; // [esp+10h] [ebp-4Ch]
  idVec3 position; // [esp+1Ch] [ebp-40h]
  float v16; // [esp+28h] [ebp-34h]
  float v17; // [esp+2Ch] [ebp-30h]
  float v18; // [esp+30h] [ebp-2Ch]
  float v19; // [esp+34h] [ebp-28h]
  float v20; // [esp+38h] [ebp-24h]
  float v21; // [esp+3Ch] [ebp-20h]
  idVec3 v22; // [esp+48h] [ebp-14h]
  char v23; // [esp+54h] [ebp-8h]
  float sta; // [esp+64h] [ebp+8h]
  float timea; // [esp+68h] [ebp+Ch]
  float timeb; // [esp+68h] [ebp+Ch]
  float v27; // [esp+70h] [ebp+14h]
  float v28; // [esp+70h] [ebp+14h]

  v4 = this;
  if ( st->mFlags & 1 )
  {
    v5 = st->mParticleTemplate.mNumTimeoutEffects;
    v6 = &st->mParticleTemplate;
    if ( v5 )
    {
      v7 = rvRandom::irand(0, v5 - 1);
      sta = time - v4->mMotionStartTime;
      rvParticle::EvaluatePosition(v4, effect, v6, (idVec3 *)((char *)&position + 4), sta);
      timea = time - v4->mMotionStartTime;
      rvParticle::EvaluateVelocity(v4, effect, (idVec3 *)((char *)&velocity + 4), timea);
      timeb = velocity.z * velocity.z + velocity.y * velocity.y + position.x * position.x;
      v27 = sqrt(timeb);
      if ( v27 >= 0.00000011920929 )
      {
        v28 = 1.0 / v27;
        position.x = position.x * v28;
        position.y = position.y * v28;
        position.z = v28 * position.z;
      }
      v8 = v6->mTimeoutEffects[v7];
      v19 = effect->mCurrentAxis.mat[1].x * position.y
          + effect->mCurrentAxis.mat[0].x * position.x
          + effect->mCurrentAxis.mat[2].x * position.z;
      v20 = effect->mCurrentAxis.mat[1].y * position.y
          + effect->mCurrentAxis.mat[0].y * position.x
          + effect->mCurrentAxis.mat[2].y * position.z;
      v21 = position.z * effect->mCurrentAxis.mat[2].z
          + position.y * effect->mCurrentAxis.mat[1].z
          + position.x * effect->mCurrentAxis.mat[0].z;
      position.x = effect->mCurrentAxis.mat[2].x * v18
                 + effect->mCurrentAxis.mat[0].x * v16
                 + effect->mCurrentAxis.mat[1].x * v17;
      position.y = effect->mCurrentAxis.mat[1].y * v17
                 + effect->mCurrentAxis.mat[0].y * v16
                 + effect->mCurrentAxis.mat[2].y * v18;
      position.z = v16 * effect->mCurrentAxis.mat[0].z
                 + v17 * effect->mCurrentAxis.mat[1].z
                 + v18 * effect->mCurrentAxis.mat[2].z;
      v16 = effect->mCurrentOrigin.x + position.x;
      v17 = effect->mCurrentOrigin.y + position.y;
      v18 = effect->mCurrentOrigin.z + position.z;
      v9 = ((int (__stdcall *)(int, _DWORD, _DWORD, _DWORD, _DWORD))v8->base->vfptr->Index)(
             v13,
             LODWORD(velocity.x),
             LODWORD(velocity.y),
             LODWORD(velocity.z),
             LODWORD(position.x));
      v10 = game->vfptr;
      v11 = v9;
      position.x = 0.0;
      LODWORD(velocity.z) = &vec3_origin;
      velocity.y = 0.0;
      v12 = idVec3::ToMat3(&v22, (idMat3 *)&v23);
      ((void (__thiscall *)(idGame *, int, idVec3 *, float *, idMat3 *))v10->PlayEffect)(
        game,
        v11,
        &effect->mMaterialColor,
        &v21,
        v12);
    }
  }
}
void __stdcall rvParticle::CalcImpactPoint(idVec3 *endPos, idVec3 *origin, idVec3 *motion, idBounds *bounds, idVec3 *normal)
{
  float *v5; // esi
  idVec3 *v6; // edi
  double v7; // st4
  double v8; // st2
  double v9; // st3
  signed int v10; // eax
  idVec3 work; // [esp+4h] [ebp-28h]
  float v12; // [esp+10h] [ebp-1Ch]
  float v13; // [esp+14h] [ebp-18h]
  float v14; // [esp+18h] [ebp-14h]
  float v15; // [esp+1Ch] [ebp-10h]
  float v16; // [esp+20h] [ebp-Ch]
  float v17; // [esp+24h] [ebp-8h]
  float v18; // [esp+28h] [ebp-4h]
  float endPosa; // [esp+30h] [ebp+4h]
  float scaleb; // [esp+34h] [ebp+8h]
  float scalec; // [esp+34h] [ebp+8h]
  float scaled; // [esp+34h] [ebp+8h]
  float scale; // [esp+34h] [ebp+8h]
  float scalee; // [esp+34h] [ebp+8h]
  float scalef; // [esp+34h] [ebp+8h]
  float scaleg; // [esp+34h] [ebp+8h]
  float scalea; // [esp+34h] [ebp+8h]
  float scaleh; // [esp+34h] [ebp+8h]
  float scalei; // [esp+34h] [ebp+8h]
  float boundsa; // [esp+3Ch] [ebp+10h]

  v5 = (float *)bounds;
  v6 = endPos;
  *endPos = *origin;
  work.y = v5[3] - *v5;
  work.z = v5[4] - v5[1];
  v12 = v5[5] - v5[2];
  scaleb = v12 * v12 + work.y * work.y + work.z * work.z;
  if ( 0.0 != scaleb )
  {
    scalec = motion->y * motion->y + motion->x * motion->x + motion->z * motion->z;
    if ( scalec != 0.0 )
    {
      work.y = motion->x;
      work.z = motion->y;
      v12 = motion->z;
      v13 = bounds->b[1].x - bounds->b[0].x;
      v14 = bounds->b[1].y - bounds->b[0].y;
      v15 = bounds->b[1].z - bounds->b[0].z;
      work.y = work.y / v13;
      work.z = work.z / v14;
      v12 = v12 / v15;
      scaled = v12 * v12 + work.y * work.y + work.z * work.z;
      scale = sqrt(scaled);
      if ( scale >= 0.00000011920929 )
      {
        scalee = 1.0 / scale;
        work.y = scalee * work.y;
        work.z = work.z * scalee;
        v12 = scalee * v12;
      }
      scalef = fabs(work.y);
      endPosa = scalef;
      scaleg = fabs(work.z);
      boundsa = scaleg;
      scalea = fabs(v12);
      v7 = endPosa;
      if ( boundsa > (double)endPosa || scalea > v7 )
      {
        v8 = boundsa;
        v9 = scalea;
        if ( boundsa >= v7 && v8 >= v9 )
        {
          v10 = 1;
LABEL_14:
          scaleh = fabs(*(&work.y + v10));
          scalei = 0.5 / scaleh;
          v13 = v5[3] - *v5;
          v14 = v5[4] - v5[1];
          v15 = v5[5] - v5[2];
          v16 = v13 * scalei;
          v17 = v14 * scalei;
          v18 = scalei * v15;
          work.y = work.y * v16;
          work.z = work.z * v17;
          v12 = v12 * v18;
          v16 = normal->x * 2.0;
          v17 = normal->y * 2.0;
          v18 = 2.0 * normal->z;
          v13 = v16 + work.y;
          v14 = v17 + work.z;
          v15 = v18 + v12;
          v6->x = v6->x + v13;
          v6->y = v14 + v6->y;
          v6->z = v6->z + v15;
          return;
        }
        if ( endPosa <= v9 && v9 >= v8 )
        {
          v10 = 2;
          goto LABEL_14;
        }
      }
      v10 = 0;
      goto LABEL_14;
    }
  }
}
void __thiscall rvParticle::EmitSmokeParticles(rvParticle *this, rvBSE *effect, rvSegment *child, rvParticleTemplate *pt, float time)
{
  rvParticle *v5; // esi
  double v6; // st7
  rvSegment *v7; // ebp
  double v8; // st7
  rvSegmentTemplate *v9; // ebx
  double v10; // st6
  idMat3 *v11; // eax
  idVec3 velocity; // [esp+1Ch] [ebp-40h]
  idVec3 position; // [esp+28h] [ebp-34h]
  idMat3 result; // [esp+38h] [ebp-24h]
  float childb; // [esp+64h] [ebp+8h]
  float childc; // [esp+64h] [ebp+8h]
  float childd; // [esp+64h] [ebp+8h]
  float childa; // [esp+64h] [ebp+8h]
  float childe; // [esp+64h] [ebp+8h]
  float childf; // [esp+64h] [ebp+8h]

  v5 = this;
  velocity.x = this->mLastTrailTime - time;
  v6 = velocity.x;
  if ( velocity.x < 0.016000001 )
  {
    v7 = child;
    do
    {
      v8 = velocity.x;
      v9 = rvSegment::GetSegmentTemplate(v7);
      v10 = time;
      if ( v5->mStartTime - time <= velocity.x && v5->mEndTime - v10 > v8 )
      {
        childb = v8 + v10 - v5->mStartTime;
        rvParticle::EvaluatePosition(v5, effect, pt, (idVec3 *)((char *)&position + 4), childb);
        childc = time - v5->mStartTime + velocity.x;
        rvParticle::EvaluateVelocity(v5, effect, (idVec3 *)((char *)&velocity + 4), childc);
        childd = velocity.y * velocity.y + velocity.z * velocity.z + position.x * position.x;
        childa = sqrt(childd);
        if ( childa >= 0.00000011920929 )
        {
          childe = 1.0 / childa;
          velocity.y = velocity.y * childe;
          velocity.z = childe * velocity.z;
          position.x = childe * position.x;
        }
        v11 = idVec3::ToMat3((idVec3 *)((char *)&velocity + 4), &result);
        childf = velocity.x + time;
        rvSegment::SpawnParticle(v7, effect, v9, childf, (idVec3 *)((char *)&position + 4), v11);
      }
      velocity.x = rvSegment::AttenuateInterval(v7, effect, v9) + velocity.x;
      v6 = velocity.x;
    }
    while ( velocity.x < 0.016000001 );
  }
  v5->mLastTrailTime = v6 + time;
}
bool __thiscall rvParticle::RunPhysics(rvParticle *this, rvBSE *effect, rvSegmentTemplate *st, float time)
{
  rvParticle *v4; // ebp
  rvParticleTemplate *v5; // edi
  int v6; // eax
  float *v7; // esi
  idTraceModel *v8; // eax
  char v9; // cl
  idTraceModel *v10; // ebx
  double v11; // st7
  double v12; // st6
  double v13; // st7
  idBounds *v14; // eax
  int v15; // eax
  int v16; // ebx
  idMat3 *v17; // eax
  idVec3 v18; // ST28_12
  idVec3 v19; // ST1C_12
  idVec3 endPos; // [esp+48h] [ebp-E8h]
  float v22; // [esp+54h] [ebp-DCh]
  idVec3 motion; // [esp+58h] [ebp-D8h]
  float v24; // [esp+64h] [ebp-CCh]
  float v25; // [esp+68h] [ebp-C8h]
  idVec3 dest; // [esp+6Ch] [ebp-C4h]
  idVec3 origin; // [esp+78h] [ebp-B8h]
  trace_t trace; // [esp+84h] [ebp-ACh]
  idMat3 result; // [esp+10Ch] [ebp-24h]

  v4 = this;
  if ( !bse_physics.internalVar->integerValue )
    goto LABEL_28;
  if ( this->mFlags & 1 )
    goto LABEL_28;
  if ( !(st->mFlags & 1) )
    goto LABEL_28;
  v5 = &st->mParticleTemplate;
  if ( !(((unsigned int)st->mParticleTemplate.mFlags >> 9) & 1) || time - this->mMotionStartTime < 0.1000000014901161 )
    goto LABEL_28;
  if ( bse_speeds.internalVar->integerValue )
    ++dword_11F4D50;
  v7 = (float *)effect;
  origin.y = effect->mCurrentOrigin.x;
  origin.z = effect->mCurrentOrigin.y;
  trace.fraction = effect->mCurrentOrigin.z;
  endPos.x = time - this->mMotionStartTime - 0.1000000014901161;
  rvParticle::EvaluatePosition(this, effect, v5, (idVec3 *)&v24, endPos.x);
  motion.x = v7[81] * dest.x + v7[75] * v24 + v7[78] * v25;
  motion.y = v7[79] * v25 + v7[76] * v24 + v7[82] * dest.x;
  motion.z = v24 * v7[77] + v25 * v7[80] + dest.x * v7[83];
  endPos.y = motion.x + origin.y;
  endPos.z = motion.y + origin.z;
  v22 = motion.z + trace.fraction;
  v24 = endPos.y;
  v25 = endPos.z;
  dest.x = v22;
  endPos.x = time - v4->mMotionStartTime;
  rvParticle::EvaluatePosition(v4, effect, v5, (idVec3 *)((char *)&dest + 4), endPos.x);
  endPos.y = v7[81] * origin.x + v7[78] * dest.z + dest.y * v7[75];
  endPos.z = v7[79] * dest.z + v7[76] * dest.y + v7[82] * origin.x;
  v22 = origin.x * v7[83] + dest.z * v7[80] + dest.y * v7[77];
  motion.x = endPos.y + origin.y;
  motion.y = endPos.z + origin.z;
  motion.z = v22 + trace.fraction;
  dest.y = motion.x;
  dest.z = motion.y;
  origin.x = motion.z;
  v8 = rvParticleTemplate::GetTraceModel(v5);
  v9 = ((unsigned int)v5->mFlags >> 26) & 1;
  v10 = v8;
  if ( !v9 && !v8 )
    goto LABEL_28;
  if ( st->mParticleTemplate.mPhysicsDistance > 0.0 )
  {
    endPos.x = st->mParticleTemplate.mPhysicsDistance;
    endPos.y = v24 - effect->mViewOrg.x;
    endPos.z = v25 - effect->mViewOrg.y;
    v22 = dest.x - effect->mViewOrg.z;
    v11 = endPos.x;
    endPos.x = endPos.z * endPos.z + endPos.y * endPos.y + v22 * v22;
    v12 = v11 * v11;
    v13 = endPos.x;
    endPos.x = v12;
    if ( endPos.x < v13 )
      goto LABEL_28;
  }
  if ( v9 )
    ((void (__stdcall *)(idVec3 *, float *, float *, signed int))game->vfptr->TracePoint)(
      &trace.endpos,
      &v24,
      &dest.y,
      2049);
  else
    ((void (__stdcall *)(idVec3 *, float *, float *, idTraceModel *, signed int))game->vfptr->Translation)(
      &trace.endpos,
      &v24,
      &dest.y,
      v8,
      2049);
  if ( trace.endpos.x < 1.0 )
  {
    if ( st->mParticleTemplate.mNumImpactEffects
      && ((unsigned __int8 (__stdcall *)(signed int))bse->vfptr->CanPlayRateLimited)(2) )
    {
      v14 = &v10->bounds;
      if ( !v10 )
        v14 = &bounds_zero;
      endPos.y = dest.y - v24;
      endPos.z = dest.z - v25;
      v22 = origin.x - dest.x;
      motion.x = endPos.y * trace.endpos.x;
      motion.y = endPos.z * trace.endpos.x;
      motion.z = trace.endpos.x * v22;
      rvParticle::CalcImpactPoint(
        (idVec3 *)((char *)&endPos + 4),
        (idVec3 *)((char *)&trace.endpos + 4),
        &motion,
        v14,
        (idVec3 *)((char *)&trace.c.normal + 4));
      v15 = rvRandom::irand(0, st->mParticleTemplate.mNumImpactEffects - 1);
      v16 = ((int (*)(void))v5->mImpactEffects[v15]->base->vfptr->Index)();
      LODWORD(endPos.x) = (idGame)game->vfptr;
      v17 = idVec3::ToMat3((idVec3 *)((char *)&trace.c.normal + 4), &result);
      (*(void (__thiscall **)(idGame *, int, idVec3 *, float *, idMat3 *, _DWORD, idVec3 *, _DWORD))(LODWORD(endPos.x) + 276))(
        game,
        v16,
        &effect->mMaterialColor,
        &endPos.y,
        v17,
        0,
        &vec3_origin,
        0.0);
    }
    if ( 0.0 != st->mParticleTemplate.mBounce )
    {
      v18.x = trace.c.normal.y;
      *(_QWORD *)&v18.y = *(_QWORD *)&trace.c.normal.z;
      v19.x = trace.endpos.y;
      *(_QWORD *)&v19.y = *(_QWORD *)&trace.endpos.z;
      rvParticle::Bounce(v4, effect, v5, v19, v18, time);
    }
    v6 = ((unsigned int)v5->mFlags >> 10) & 1;
  }
  else
  {
LABEL_28:
    LOBYTE(v6) = 0;
  }
  return v6;
}
int dynamic_initializer_for__singletonLock___115()
{
  sdLock::sdLock(&singletonLock_115);
  return atexit(dynamic_atexit_destructor_for__singletonLock___115);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___115()
{
  sdLock::~sdLock(&singletonLock_115);
}
