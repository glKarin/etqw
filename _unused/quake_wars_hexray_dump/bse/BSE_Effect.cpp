double __thiscall rvBSE::GetAttenuation(rvBSE *this, rvSegmentTemplate *st)
{
  double result; // st7
  rvSegmentTemplate *v3; // edx
  float sta; // [esp+4h] [ebp+4h]
  float stb; // [esp+4h] [ebp+4h]
  float stc; // [esp+4h] [ebp+4h]
  float std; // [esp+4h] [ebp+4h]

  result = 0.0;
  v3 = st;
  if ( st->mAttenuation.x <= 0.0 && st->mAttenuation.y <= 0.0 )
    return this->mAttenuation;
  sta = st->mAttenuation.x + 1.0;
  if ( sta > (double)this->mShortestDistanceToCamera )
    return this->mAttenuation;
  stb = v3->mAttenuation.y - 1.0;
  if ( stb >= (double)this->mShortestDistanceToCamera )
  {
    stc = (this->mShortestDistanceToCamera - v3->mAttenuation.x) / (v3->mAttenuation.y - v3->mAttenuation.x);
    std = (1.0 - stc) * this->mAttenuation;
    result = std;
  }
  return result;
}
void __thiscall rvBSE::UpdateSoundEmitter(rvBSE *this, rvSegmentTemplate *st, rvSegment *seg)
{
  rvBSE *v3; // esi
  idSoundEmitter *v4; // ecx
  soundShaderParms_t parms; // [esp+0h] [ebp-28h]
  int v6; // [esp+24h] [ebp-4h]

  parms.maxDistance = 0.0;
  v3 = this;
  parms.farDistance = 0.0;
  parms.volume = 0.0;
  parms.shakes = 0.0;
  parms.soundShaderFlags = 0;
  parms.pitchShift = 0.0;
  parms.soundClass = 0;
  parms.soundArea = 0;
  v6 = 0;
  if ( ((unsigned int)this->mFlags >> 3) & 1 )
  {
    if ( rvSegmentTemplate::GetSoundLooping(st) )
    {
      if ( ((unsigned int)seg->mFlags >> 1) & 1 )
        ((void (__stdcall *)(int))v3->mReferenceSound->vfptr->StopSound)(seg->mSegmentTemplateHandle + 1);
    }
  }
  else
  {
    v4 = this->mReferenceSound;
    parms.shakes = seg->mSoundVolume;
    *(float *)&parms.soundClass = seg->mFreqShift;
    ((void (__stdcall *)(idVec3 *, _DWORD, float *))v4->vfptr->UpdateEmitter)(
      &v3->mCurrentOrigin,
      0,
      &parms.maxDistance);
  }
}
idVec3 *__thiscall rvBSE::GetInterpolatedOffset(rvBSE *this, idVec3 *result, float time)
{
  idVec3 *v3; // edx
  idVec3 *v4; // eax
  float v5; // ST00_4
  float v6; // ST04_4
  float v7; // ST08_4
  float v8; // ST0C_4
  float v9; // ST10_4
  float v10; // ST14_4
  float deltaTime; // [esp+1Ch] [ebp+4h]
  float deltaTimea; // [esp+1Ch] [ebp+4h]

  v3 = result;
  result->z = 0.0;
  result->y = 0.0;
  result->x = 0.0;
  deltaTime = this->mCurrentTime - this->mLastTime;
  if ( deltaTime < 0.0020000001 )
    return v3;
  v4 = v3;
  deltaTimea = 1.0 - (time - this->mLastTime) / deltaTime;
  v5 = this->mCurrentOrigin.x - this->mLastOrigin.x;
  v6 = this->mCurrentOrigin.y - this->mLastOrigin.y;
  v7 = this->mCurrentOrigin.z - this->mLastOrigin.z;
  v8 = v5 * deltaTimea;
  v9 = v6 * deltaTimea;
  v10 = deltaTimea * v7;
  v3->x = v8;
  v3->y = v9;
  v3->z = v10;
  return v4;
}
void __thiscall rvBSE::SetDuration(rvBSE *this, float time)
{
  double v2; // st7

  v2 = time;
  if ( time < 0.0 )
  {
    v2 = this->mDeclEffect->mMinDuration;
LABEL_3:
    this->mDuration = v2;
    return;
  }
  if ( this->mDuration < v2 )
    goto LABEL_3;
}
double __thiscall idVec3::Dist(idVec3 *this, idVec3 *Pt)
{
  float v2; // ST10_4
  float v3; // ST14_4
  float v4; // ST18_4
  double v5; // st7
  double v6; // st6
  double v7; // st5
  float Pta; // [esp+20h] [ebp+4h]
  idVec3 *Ptb; // [esp+20h] [ebp+4h]
  float Ptc; // [esp+20h] [ebp+4h]
  float Ptd; // [esp+20h] [ebp+4h]

  v2 = this->x - Pt->x;
  v3 = this->y - Pt->y;
  v4 = this->z - Pt->z;
  Pta = v3 * v3 + v2 * v2 + v4 * v4;
  v5 = Pta;
  Ptb = (idVec3 *)(1597463007 - (SLODWORD(Pta) >> 1));
  v6 = *(float *)&Ptb;
  v7 = *(float *)&Ptb;
  Ptc = v5 * 0.5;
  Ptd = v6 * (1.5 - v7 * v7 * Ptc);
  return (float)(v5 * Ptd);
}
void __thiscall rvSegment::rvSegment(rvSegment *this)
{
  this->mFlags = 0;
  this->mParticles = 0;
  this->mUsedHead = 0;
  this->mFreeHead = 0;
  this->mParticleCount = 0;
  this->mLoopParticleCount = 0;
}
const char *__thiscall rvBSE::GetDeclName(rvBSE *this)
{
  return (const char *)(*(int (**)(void))this->mDeclEffect->base->vfptr->gap4)();
}
void __thiscall rvBSE::UpdateFromOwner(rvBSE *this, renderEffect_s *parms, float time, bool init)
{
  renderEffect_s *v4; // edx
  rvBSE *v5; // ebx
  float *v6; // ebp
  double v7; // st7
  double v8; // st7
  renderEffect_s *v9; // ecx
  float *v10; // eax
  double v11; // st7
  double v12; // st7
  double v13; // st6
  double v14; // st5
  float *v15; // esi
  float v16; // edi
  float v17; // xmm1_4
  float v18; // xmm3_4
  float v19; // xmm5_4
  double v20; // st4
  float *v21; // esi
  float v22; // xmm1_4
  float v23; // xmm3_4
  float v24; // xmm5_4
  float *v25; // eax
  double v26; // st7
  float *v27; // esi
  float v28; // edi
  float v29; // xmm1_4
  float v30; // xmm3_4
  float v31; // xmm5_4
  double v32; // st4
  float *v33; // esi
  float v34; // xmm1_4
  float v35; // xmm3_4
  float v36; // xmm5_4
  float *v37; // ecx
  int v38; // eax
  signed int v39; // edx
  double v40; // st7
  idRenderWorldVtbl *v41; // edx
  double v42; // st7
  double v43; // st7
  float *v44; // esi
  float v45; // xmm1_4
  float v46; // xmm3_4
  float v47; // xmm5_4
  double v48; // st4
  float *v49; // esi
  float v50; // xmm1_4
  float v51; // xmm3_4
  float v52; // xmm5_4
  float *v53; // eax
  signed int v54; // ecx
  double v55; // st7
  float *v56; // esi
  float v57; // edi
  float v58; // xmm1_4
  float v59; // xmm3_4
  float v60; // xmm5_4
  double v61; // st7
  idMat3 *v62; // eax
  double v63; // st7
  double v64; // st7
  idVec4 *v65; // [esp+4h] [ebp-B4h]
  float *v66; // [esp+8h] [ebp-B0h]
  float *v67; // [esp+Ch] [ebp-ACh]
  int v68; // [esp+10h] [ebp-A8h]
  int v69; // [esp+14h] [ebp-A4h]
  float v70; // [esp+28h] [ebp-90h]
  float v71; // [esp+2Ch] [ebp-8Ch]
  float v72; // [esp+30h] [ebp-88h]
  float v73; // [esp+34h] [ebp-84h]
  float length; // [esp+38h] [ebp-80h]
  idVec3 size; // [esp+3Ch] [ebp-7Ch]
  idVec3 corner; // [esp+48h] [ebp-70h]
  idVec3 dir; // [esp+54h] [ebp-64h]
  float v78; // [esp+60h] [ebp-58h]
  float v79; // [esp+64h] [ebp-54h]
  float v80; // [esp+68h] [ebp-50h]
  float v81; // [esp+6Ch] [ebp-4Ch]
  float v82; // [esp+70h] [ebp-48h]
  float v83; // [esp+74h] [ebp-44h]
  float v84; // [esp+78h] [ebp-40h]
  float v85; // [esp+7Ch] [ebp-3Ch]
  float v86; // [esp+80h] [ebp-38h]
  float v87; // [esp+84h] [ebp-34h]
  float v88; // [esp+88h] [ebp-30h]
  float v89; // [esp+8Ch] [ebp-2Ch]
  float v90; // [esp+90h] [ebp-28h]
  idMat3 result; // [esp+94h] [ebp-24h]

  v4 = parms;
  v5 = this;
  this->mLastTime = this->mCurrentTime;
  v6 = &this->mCurrentOrigin.x;
  this->mLastOrigin.x = this->mCurrentOrigin.x;
  this->mLastOrigin.y = this->mCurrentOrigin.y;
  this->mLastOrigin.z = this->mCurrentOrigin.z;
  this->mCurrentTime = time;
  *v6 = parms->origin.x;
  v6[1] = parms->origin.y;
  v6[2] = parms->origin.z;
  qmemcpy(&this->mCurrentAxis, &parms->axis, sizeof(this->mCurrentAxis));
  v70 = this->mCurrentAxis.mat[2].z;
  v71 = this->mCurrentAxis.mat[1].z;
  v85 = this->mCurrentAxis.mat[0].z;
  v86 = this->mCurrentAxis.mat[2].y;
  v88 = this->mCurrentAxis.mat[1].y;
  v90 = this->mCurrentAxis.mat[0].y;
  v89 = this->mCurrentAxis.mat[2].x;
  v87 = this->mCurrentAxis.mat[1].x;
  size.x = this->mCurrentAxis.mat[0].x;
  dir.y = size.x;
  dir.z = v87;
  v78 = v89;
  v79 = v90;
  v80 = v88;
  v81 = v86;
  v82 = v85;
  v83 = v71;
  v84 = v70;
  qmemcpy(&this->mCurrentAxisTransposed, &dir.y, sizeof(this->mCurrentAxisTransposed));
  size.x = this->mCurrentAxis.mat[2].z;
  v90 = this->mCurrentAxis.mat[1].z;
  v85 = this->mCurrentAxis.mat[0].z;
  v89 = this->mCurrentAxis.mat[2].y;
  v88 = this->mCurrentAxis.mat[1].y;
  v71 = this->mCurrentAxis.mat[0].y;
  v87 = this->mCurrentAxis.mat[2].x;
  v86 = this->mCurrentAxis.mat[1].x;
  v70 = this->mCurrentAxis.mat[0].x;
  size.y = v4->windVector.x * v70 + v4->windVector.y * v71 + v4->windVector.z * v85;
  size.z = v4->windVector.x * v86 + v4->windVector.y * v88 + v4->windVector.z * v90;
  corner.x = v4->windVector.y * v89 + v4->windVector.x * v87 + v4->windVector.z * size.x;
  this->mCurrentWindVector.x = size.y;
  this->mCurrentWindVector.y = size.z;
  this->mCurrentWindVector.z = corner.x;
  v7 = v5->mCurrentTime - v5->mLastTime;
  if ( v7 > 0.002000000094994903 )
  {
    size.y = *v6 - this->mLastOrigin.x;
    size.z = this->mCurrentOrigin.y - this->mLastOrigin.y;
    corner.x = this->mCurrentOrigin.z - this->mLastOrigin.z;
    v70 = v7;
    v70 = 1.0 / v70;
    dir.y = v70 * size.y;
    dir.z = size.z * v70;
    v78 = v70 * corner.x;
    this->mCurrentVelocity.x = dir.y;
    this->mCurrentVelocity.y = dir.z;
    this->mCurrentVelocity.z = v78;
  }
  this->mGravity.x = parms->gravity.x;
  this->mGravity.y = parms->gravity.y;
  this->mGravity.z = parms->gravity.z;
  this->mGravityDir.x = this->mGravity.x;
  this->mGravityDir.y = this->mGravity.y;
  this->mGravityDir.z = this->mGravity.z;
  v70 = this->mGravityDir.y * this->mGravityDir.y
      + this->mGravityDir.x * this->mGravityDir.x
      + this->mGravityDir.z * this->mGravityDir.z;
  v70 = sqrt(v70);
  if ( v70 >= 0.00000011920929 )
  {
    v70 = 1.0 / v70;
    v8 = v70;
    this->mGravityDir.x = this->mGravityDir.x * v70;
    this->mGravityDir.y = this->mGravityDir.y * v8;
    this->mGravityDir.z = v8 * this->mGravityDir.z;
  }
  v9 = parms;
  if ( parms->useRenderBounds || parms->isStatic )
  {
    if ( v5->mGrownRenderBounds.b[1].x >= (double)v5->mGrownRenderBounds.b[0].x )
    {
      v37 = &dir.y;
      size.y = v5->mGrownRenderBounds.b[1].x + 10.0;
      v38 = (int)&v5->mCurrentWorldBounds.b[0].y;
      v39 = 2;
      size.z = v5->mGrownRenderBounds.b[1].y + 10.0;
      corner.x = v5->mGrownRenderBounds.b[1].z + 10.0;
      v72 = v5->mGrownRenderBounds.b[0].x - 10.0;
      v73 = v5->mGrownRenderBounds.b[0].y - 10.0;
      length = v5->mGrownRenderBounds.b[0].z - 10.0;
      dir.y = v72;
      dir.z = v73;
      v78 = length;
      v79 = size.y;
      v80 = size.z;
      v81 = corner.x;
      do
      {
        v40 = *v37;
        v37 += 3;
        *(float *)(v38 - 4) = v40;
        v38 += 12;
        --v39;
        *(float *)(v38 - 12) = *(float *)((char *)&dir.y - (char *)&v5->mCurrentWorldBounds + v38 - 12);
        *(float *)(v38 - 8) = *(float *)((char *)&dir.z - (char *)&v5->mCurrentWorldBounds + v38 - 12);
      }
      while ( v39 );
      idBounds::FromTransformedBounds(
        &v5->mCurrentWorldBounds,
        &v5->mCurrentWorldBounds,
        &vec3_origin,
        &v5->mCurrentAxis);
      v9 = parms;
      v5->mCurrentWorldBounds.b[0].x = v5->mCurrentWorldBounds.b[0].x + *v6;
      v5->mCurrentWorldBounds.b[0].y = v6[1] + v5->mCurrentWorldBounds.b[0].y;
      v5->mCurrentWorldBounds.b[0].z = v6[2] + v5->mCurrentWorldBounds.b[0].z;
      v5->mCurrentWorldBounds.b[1].x = *v6 + v5->mCurrentWorldBounds.b[1].x;
      v5->mCurrentWorldBounds.b[1].y = v6[1] + v5->mCurrentWorldBounds.b[1].y;
      v5->mCurrentWorldBounds.b[1].z = v5->mCurrentWorldBounds.b[1].z + v6[2];
      v13 = size.z;
      v14 = corner.x;
      v12 = size.y;
    }
    else
    {
      size.y = v5->mDeclEffect->mSize;
      v70 = COERCE_FLOAT(&v72);
      size.z = size.y;
      v25 = (float *)&v5->mCurrentWorldBounds;
      corner.x = size.y;
      v71 = *(float *)&v25;
      v25[2] = 1.0e30;
      v25[1] = 1.0e30;
      *v25 = 1.0e30;
      size.x = -1.0e30;
      v26 = size.x;
      v25[5] = size.x;
      v25[4] = v26;
      v25[3] = v26;
      v12 = size.y;
      v72 = size.y + *v6;
      v13 = size.z;
      v73 = v6[1] + size.z;
      v14 = corner.x;
      length = v6[2] + corner.x;
      v27 = (float *)LODWORD(v71);
      v28 = v70;
      v29 = *(float *)LODWORD(v70);
      *(float *)LODWORD(v71) = fminf(*(float *)LODWORD(v71), *(float *)LODWORD(v70));
      v30 = *(float *)(LODWORD(v28) + 4);
      v27[1] = fminf(v27[1], v30);
      v31 = *(float *)(LODWORD(v28) + 8);
      v27[2] = fminf(v27[2], v31);
      v27[3] = fmaxf(v29, v27[3]);
      v27[4] = fmaxf(v30, v27[4]);
      v27[5] = fmaxf(v31, v27[5]);
      v32 = *v6 - v12;
      v70 = COERCE_FLOAT(&v72);
      LODWORD(v71) = (char *)v5 + 396;
      v72 = v32;
      v73 = v6[1] - v13;
      length = v6[2] - v14;
      v33 = (float *)&v5->mCurrentWorldBounds;
      v34 = v72;
      *v33 = fminf(v5->mCurrentWorldBounds.b[0].x, v72);
      v35 = v73;
      v33[1] = fminf(v5->mCurrentWorldBounds.b[0].y, v73);
      v36 = length;
      v33[2] = fminf(v5->mCurrentWorldBounds.b[0].z, length);
      v33[3] = fmaxf(v34, v5->mCurrentWorldBounds.b[1].x);
      v33[4] = fmaxf(v35, v5->mCurrentWorldBounds.b[1].y);
      v33[5] = fmaxf(v36, v5->mCurrentWorldBounds.b[1].z);
    }
  }
  else
  {
    size.y = v5->mDeclEffect->mSize;
    v70 = COERCE_FLOAT(&v72);
    size.z = size.y;
    v10 = (float *)&v5->mCurrentWorldBounds;
    corner.x = size.y;
    v71 = *(float *)&v10;
    v10[2] = 1.0e30;
    v10[1] = 1.0e30;
    *v10 = 1.0e30;
    size.x = -1.0e30;
    v11 = size.x;
    v10[5] = size.x;
    v10[4] = v11;
    v10[3] = v11;
    v12 = size.y;
    v72 = *v6 + size.y;
    v13 = size.z;
    v73 = v6[1] + size.z;
    v14 = corner.x;
    length = v6[2] + corner.x;
    v15 = (float *)LODWORD(v71);
    v16 = v70;
    v17 = *(float *)LODWORD(v70);
    *(float *)LODWORD(v71) = fminf(*(float *)LODWORD(v71), *(float *)LODWORD(v70));
    v18 = *(float *)(LODWORD(v16) + 4);
    v15[1] = fminf(v15[1], v18);
    v19 = *(float *)(LODWORD(v16) + 8);
    v15[2] = fminf(v15[2], v19);
    v15[3] = fmaxf(v17, v15[3]);
    v15[4] = fmaxf(v18, v15[4]);
    v15[5] = fmaxf(v19, v15[5]);
    v20 = *v6 - v12;
    v70 = COERCE_FLOAT(&v72);
    LODWORD(v71) = (char *)v5 + 396;
    v72 = v20;
    v73 = v6[1] - v13;
    length = v6[2] - v14;
    v21 = (float *)&v5->mCurrentWorldBounds;
    v22 = v72;
    *v21 = fminf(v5->mCurrentWorldBounds.b[0].x, v72);
    v23 = v73;
    v21[1] = fminf(v5->mCurrentWorldBounds.b[0].y, v73);
    v24 = length;
    v21[2] = fminf(v5->mCurrentWorldBounds.b[0].z, length);
    v21[3] = fmaxf(v22, v5->mCurrentWorldBounds.b[1].x);
    v21[4] = fmaxf(v23, v5->mCurrentWorldBounds.b[1].y);
    v21[5] = fmaxf(v24, v5->mCurrentWorldBounds.b[1].z);
    v5->mForcePush = 0;
  }
  if ( bse_debug.internalVar->integerValue > 2 )
  {
    v41 = session->rw->vfptr;
    v69 = 0;
    v68 = 10000;
    v67 = &v5->mLastOrigin.x;
    ((void (__stdcall *)(idVec4 *, float *, idVec3 *, signed int, _DWORD))v41->DebugLine)(
      &colorWhite,
      v6,
      &v5->mLastOrigin,
      10000,
      0);
    v72 = *v6;
    v42 = v5->mCurrentOrigin.y;
    v69 = 0;
    v73 = v42;
    v68 = 10000;
    v43 = v5->mCurrentOrigin.z + 10.0;
    v67 = &v72;
    v66 = v6;
    v65 = &colorGreen;
    length = v43;
    ((void (__stdcall *)(idVec4 *, float *, float *, signed int, _DWORD))session->rw->vfptr->DebugLine)(
      &colorGreen,
      v6,
      &v72,
      10000,
      0);
    v13 = size.z;
    v9 = parms;
    v14 = corner.x;
    v12 = size.y;
  }
  if ( ((unsigned int)v5->mFlags >> 1) & 1
    && ((unsigned int)v5->mDeclEffect->mFlags >> 1) & 1
    && (init
     || v9->endOrigin.x != v5->mCurrentEndOrigin.x
     || v9->endOrigin.y != v5->mCurrentEndOrigin.y
     || v9->endOrigin.z != v5->mCurrentEndOrigin.z
     || v5->mLastOrigin.x != *v6
     || v5->mLastOrigin.y != v6[1]
     || v5->mLastOrigin.z != v6[2]) )
  {
    v5->mCurrentEndOrigin.x = v9->endOrigin.x;
    LODWORD(v71) = (char *)v5 + 396;
    v5->mCurrentEndOrigin.y = v9->endOrigin.y;
    v5->mCurrentEndOrigin.z = v9->endOrigin.z;
    v70 = COERCE_FLOAT(&v72);
    v72 = v12 + v5->mCurrentEndOrigin.x;
    v73 = v5->mCurrentEndOrigin.y + v13;
    length = v5->mCurrentEndOrigin.z + v14;
    v44 = (float *)LODWORD(v71);
    v45 = v72;
    *(float *)LODWORD(v71) = fminf(*(float *)LODWORD(v71), v72);
    v46 = v73;
    v44[1] = fminf(v44[1], v73);
    v47 = length;
    v44[2] = fminf(v44[2], length);
    v44[3] = fmaxf(v45, v44[3]);
    v44[4] = fmaxf(v46, v44[4]);
    v44[5] = fmaxf(v47, v44[5]);
    v48 = v5->mCurrentEndOrigin.x;
    v70 = COERCE_FLOAT(&v72);
    LODWORD(v71) = (char *)v5 + 396;
    v72 = v48 - v12;
    v73 = v5->mCurrentEndOrigin.y - v13;
    length = v5->mCurrentEndOrigin.z - v14;
    v49 = (float *)&v5->mCurrentWorldBounds;
    v50 = v72;
    *v49 = fminf(v5->mCurrentWorldBounds.b[0].x, v72);
    v51 = v73;
    v49[1] = fminf(v5->mCurrentWorldBounds.b[0].y, v73);
    v52 = length;
    v49[2] = fminf(v5->mCurrentWorldBounds.b[0].z, length);
    v49[3] = fmaxf(v50, v5->mCurrentWorldBounds.b[1].x);
    v49[4] = fmaxf(v51, v5->mCurrentWorldBounds.b[1].y);
    v49[5] = fmaxf(v52, v5->mCurrentWorldBounds.b[1].z);
    v5->mFlags |= 4u;
  }
  v53 = (float *)&v5->mCurrentLocalBounds;
  v53[2] = 1.0e30;
  v54 = 0;
  v53[1] = 1.0e30;
  v70 = COERCE_FLOAT((idVec3 *)((char *)&corner + 4));
  LODWORD(v71) = (char *)v5 + 372;
  *v53 = 1.0e30;
  size.x = -1.0e30;
  v55 = size.x;
  v53[5] = size.x;
  v53[4] = v55;
  v53[3] = v55;
  do
  {
    corner.y = *((float *)&v5->vfptr + 3 * ((v54 & 1) + 33));
    corner.z = v5->mCurrentWorldBounds.b[(v54 >> 1) & 1].y;
    dir.x = v5->mCurrentWorldBounds.b[(v54 >> 2) & 1].z;
    corner.y = corner.y - *v6;
    corner.z = corner.z - v6[1];
    dir.x = dir.x - v6[2];
    v72 = v5->mCurrentAxisTransposed.mat[2].x * dir.x
        + v5->mCurrentAxisTransposed.mat[0].x * corner.y
        + corner.z * v5->mCurrentAxisTransposed.mat[1].x;
    v73 = v5->mCurrentAxisTransposed.mat[0].y * corner.y
        + v5->mCurrentAxisTransposed.mat[1].y * corner.z
        + dir.x * v5->mCurrentAxisTransposed.mat[2].y;
    length = dir.x * v5->mCurrentAxisTransposed.mat[2].z
           + corner.y * v5->mCurrentAxisTransposed.mat[0].z
           + corner.z * v5->mCurrentAxisTransposed.mat[1].z;
    corner.y = v72;
    corner.z = v73;
    dir.x = length;
    v56 = (float *)LODWORD(v71);
    v57 = v70;
    v58 = *(float *)LODWORD(v70);
    *(float *)LODWORD(v71) = fminf(*(float *)LODWORD(v71), *(float *)LODWORD(v70));
    v59 = *(float *)(LODWORD(v57) + 4);
    v56[1] = fminf(v56[1], v59);
    v60 = *(float *)(LODWORD(v57) + 8);
    v56[2] = fminf(v56[2], v60);
    v56[3] = fmaxf(v58, v56[3]);
    v56[4] = fmaxf(v59, v56[4]);
    v56[5] = fmaxf(v60, v56[5]);
    ++v54;
  }
  while ( v54 < 8 );
  dir.y = v5->mCurrentEndOrigin.x - *v6;
  dir.z = v5->mCurrentEndOrigin.y - v6[1];
  v78 = v5->mCurrentEndOrigin.z - v6[2];
  v70 = dir.y * dir.y + dir.z * dir.z + v78 * v78;
  v70 = sqrt(v70);
  v61 = v70;
  if ( v70 >= 0.00000011920929 )
  {
    v70 = 1.0 / v61;
    dir.y = v70 * dir.y;
    dir.z = dir.z * v70;
    v78 = v70 * v78;
  }
  else
  {
    v61 = 0.0;
  }
  size.x = v61;
  v62 = idVec3::ToMat3((idVec3 *)((char *)&dir + 4), &result);
  v63 = size.x / 100.0;
  qmemcpy(&v5->mLightningAxis, v62, sizeof(v5->mLightningAxis));
  v70 = v63;
  v64 = v70;
  v5->mLightningAxis.mat[0].x = v5->mLightningAxis.mat[0].x * v70;
  v5->mLightningAxis.mat[0].y = v64 * v5->mLightningAxis.mat[0].y;
  v5->mLightningAxis.mat[0].z = v64 * v5->mLightningAxis.mat[0].z;
  v5->mTint.x = parms->shaderParms[0];
  v5->mTint.y = parms->shaderParms[1];
  v5->mTint.z = parms->shaderParms[2];
  v5->mTint.w = parms->shaderParms[3];
  v5->mBrightness = parms->shaderParms[6];
  v5->mSuppressLightsInViewID = parms->suppressLightsInViewID;
  v5->mAttenuation = parms->attenuation;
  v5->mMaterialColor.x = parms->materialColor.x;
  v5->mMaterialColor.y = parms->materialColor.y;
  v5->mMaterialColor.z = parms->materialColor.z;
}
void __thiscall rvBSE::UpdateAttenuation(rvBSE *this)
{
  rvBSE *v1; // esi
  double v2; // st7
  float v3; // ST14_4
  double v4; // st7
  float v5; // ST14_4
  float fovx; // [esp+Ch] [ebp-50h]
  float fovxa; // [esp+Ch] [ebp-50h]
  char v8; // [esp+10h] [ebp-4Ch]
  float v9; // [esp+14h] [ebp-48h]
  float v10; // [esp+18h] [ebp-44h]
  idVec3 origin; // [esp+1Ch] [ebp-40h]
  idVec3 localOrigin; // [esp+28h] [ebp-34h]
  idMat3 axis; // [esp+34h] [ebp-28h]

  v1 = this;
  if ( ((unsigned int)this->mDeclEffect->mFlags >> 2) & 1 )
  {
    ((void (__stdcall *)(float *, float *, char *))game->vfptr->GetPlayerView)(&origin.y, &axis.mat[0].y, &v8);
    fovx = idVec3::Dist(&v1->mCurrentOrigin, (idVec3 *)((char *)&origin + 4));
    v2 = 1.0;
    if ( fovx >= 1.0 )
    {
      v2 = fovx;
      if ( fovx > 131072.0 )
        v2 = 131072.0;
    }
    v3 = v2;
    v1->mOriginDistanceToCamera = v3;
    v9 = origin.y - v1->mCurrentOrigin.x;
    v10 = origin.z - v1->mCurrentOrigin.y;
    origin.x = localOrigin.x - v1->mCurrentOrigin.z;
    localOrigin.y = v1->mCurrentAxis.mat[2].x * origin.x
                  + v1->mCurrentAxis.mat[0].x * v9
                  + v1->mCurrentAxis.mat[1].x * v10;
    localOrigin.z = v1->mCurrentAxis.mat[1].y * v10
                  + v1->mCurrentAxis.mat[0].y * v9
                  + v1->mCurrentAxis.mat[2].y * origin.x;
    axis.mat[0].x = v9 * v1->mCurrentAxis.mat[0].z
                  + v10 * v1->mCurrentAxis.mat[1].z
                  + origin.x * v1->mCurrentAxis.mat[2].z;
    fovxa = idBounds::ShortestDistance(&v1->mCurrentLocalBounds, (idVec3 *)((char *)&localOrigin + 4));
    v4 = 1.0;
    if ( fovxa < 1.0 || (v4 = fovxa, fovxa <= 131072.0) )
    {
      v5 = v4;
      v1->mShortestDistanceToCamera = v5;
    }
    else
    {
      v1->mShortestDistanceToCamera = 131072.0;
    }
  }
}
void __thiscall rvBSE::LoopInstant(rvBSE *this, float time)
{
  rvBSE *v2; // esi
  int v3; // edi
  bool v4; // zf
  bool v5; // sf
  int v6; // ebx
  unsigned __int64 v7; // st7

  v2 = this;
  if ( 0.0 == this->mDuration )
  {
    v3 = 0;
    v4 = this->mSegments.num == 0;
    v5 = this->mSegments.num < 0;
    this->mStartTime = this->mDeclEffect->mMaxDuration + 0.5 + this->mStartTime;
    if ( !v5 && !v4 )
    {
      v6 = 0;
      do
      {
        rvSegment::ResetTime(&v2->mSegments.list[v6], v2, v2->mStartTime);
        ++v3;
        ++v6;
      }
      while ( v3 < v2->mSegments.num );
    }
    if ( bse_debug.internalVar->integerValue == 2 )
    {
      *(double *)&v7 = v2->mDeclEffect->mMaxDuration + 0.5;
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
        common.type,
        "BSE: Looping duration: %g\n",
        (_DWORD)v7,
        (_DWORD)(v7 >> 32));
    }
    ++v2->mDeclEffect->mLoopCount;
  }
}
void __thiscall rvBSE::LoopLooping(rvBSE *this, float time)
{
  rvBSE *v2; // esi
  int v3; // edi
  bool v4; // zf
  bool v5; // sf
  int v6; // ebx

  v2 = this;
  if ( 0.0 != this->mDuration )
  {
    v3 = 0;
    v4 = this->mSegments.num == 0;
    v5 = this->mSegments.num < 0;
    this->mStartTime = this->mStartTime + this->mDuration;
    this->mDuration = 0.0;
    if ( !v5 && !v4 )
    {
      v6 = 0;
      do
      {
        rvSegment::ResetTime(&v2->mSegments.list[v6], v2, v2->mStartTime);
        ++v3;
        ++v6;
      }
      while ( v3 < v2->mSegments.num );
    }
    if ( bse_debug.internalVar->integerValue == 2 )
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
        common.type,
        "BSE: Looping duration: %g\n",
        v2->mDuration);
    ++v2->mDeclEffect->mLoopCount;
  }
}
bool __thiscall rvBSE::Service(rvBSE *this, renderEffect_s *parms, float time, bool spawn, bool *forcePush)
{
  renderEffect_s *v5; // ebp
  rvBSE *v6; // esi
  int v7; // edi
  int v8; // ebx
  int v9; // edi
  char v10; // bl
  int v11; // ebp
  unsigned int v12; // ecx
  float spawna; // [esp+24h] [ebp+Ch]
  float spawnb; // [esp+24h] [ebp+Ch]
  float spawnc; // [esp+24h] [ebp+Ch]
  float spawnd; // [esp+24h] [ebp+Ch]

  v5 = parms;
  v6 = this;
  rvBSE::UpdateFromOwner(this, parms, time, 0);
  rvBSE::UpdateAttenuation(v6);
  if ( spawn )
  {
    v7 = 0;
    if ( v6->mSegments.num > 0 )
    {
      v8 = 0;
      do
      {
        spawna = (double)(v6->mSegments.num - v7) * -10.0;
        rvSegment::Check(&v6->mSegments.list[v8], v8 * 80, v7++, v6, time, spawna);
        ++v8;
      }
      while ( v7 < v6->mSegments.num );
    }
  }
  if ( !(((unsigned int)v6->mFlags >> 3) & 1) )
  {
    if ( parms->loop )
    {
      spawnb = v6->mDuration + v6->mStartTime;
      if ( spawnb < (double)time )
        rvBSE::LoopLooping(v6, time);
    }
  }
  v9 = 0;
  v10 = 0;
  if ( v6->mSegments.num > 0 )
  {
    v11 = 0;
    do
    {
      if ( rvSegment::UpdateParticles(&v6->mSegments.list[v11], v6, time) )
        v10 = 1;
      ++v9;
      ++v11;
    }
    while ( v9 < v6->mSegments.num );
    v5 = parms;
  }
  v6->mFlags &= 0xFFFFFFFB;
  *forcePush = v6->mForcePush;
  v12 = (unsigned int)v6->mFlags >> 3;
  v6->mForcePush = 0;
  if ( v12 & 1 )
    return v10 == 0;
  if ( v5->loop )
  {
    spawnc = v6->mDuration + v6->mStartTime;
    if ( spawnc < (double)time )
      rvBSE::LoopInstant(v6, time);
    if ( v5->loop )
      return 0;
  }
  spawnd = v6->mDuration + v6->mStartTime;
  return spawnd < (double)time;
}
double __thiscall rvBSE::EvaluateCost(rvBSE *this, int segment)
{
  rvBSE *v2; // esi
  double result; // st7
  int v4; // edi
  int v5; // ebx
  double v6; // st7

  v2 = this;
  if ( segment < 0 )
  {
    v4 = 0;
    this->mCost = 0.0;
    if ( this->mSegments.num > 0 )
    {
      v5 = 0;
      do
      {
        v6 = rvDeclEffect::EvaluateCost(v2->mDeclEffect, v2->mSegments.list[v5].mActiveCount, segment);
        ++v4;
        ++v5;
        v2->mCost = v6 + v2->mCost;
      }
      while ( v4 < v2->mSegments.num );
    }
    result = v2->mCost;
  }
  else
  {
    this->mCost = rvDeclEffect::EvaluateCost(this->mDeclEffect, this->mSegments.list[segment].mActiveCount, segment);
    result = v2->mCost;
  }
  return result;
}
void __thiscall rvBSE::DisplayDebugInfo(rvBSE *this, renderEffect_s *parms, viewDef_s *view, idBounds *bounds)
{
  int v4; // eax
  rvBSE *v5; // edi
  int (*v6)(void); // eax
  int v7; // eax
  const char *v8; // eax
  float v9; // ecx
  double v10; // st6
  int v11; // ebx
  double v12; // st7
  double v13; // st7
  float *v14; // esi
  float *v15; // edi
  float v16; // xmm1_4
  float v17; // xmm3_4
  float v18; // xmm5_4
  float *v19; // esi
  float *v20; // edi
  float v21; // xmm1_4
  float v22; // xmm3_4
  float v23; // xmm5_4
  bool v24; // sf
  unsigned __int8 v25; // of
  signed int v26; // ecx
  idVec3 transformed; // [esp+58h] [ebp-110h]
  idVec3 v; // [esp+64h] [ebp-104h]
  idBounds modelBounds; // [esp+70h] [ebp-F8h]
  idBounds globalBounds; // [esp+88h] [ebp-E0h]
  float v31; // [esp+A0h] [ebp-C8h]
  float *v32; // [esp+A4h] [ebp-C4h]
  unsigned int v33; // [esp+A8h] [ebp-C0h]
  float *v34; // [esp+ACh] [ebp-BCh]
  unsigned int v35; // [esp+B0h] [ebp-B8h]
  float *v36; // [esp+B4h] [ebp-B4h]
  rvBSE *v37; // [esp+B8h] [ebp-B0h]
  idStr work; // [esp+BCh] [ebp-ACh]
  float v39; // [esp+E0h] [ebp-88h]
  float v40; // [esp+E4h] [ebp-84h]
  float v41; // [esp+E8h] [ebp-80h]
  float v42; // [esp+ECh] [ebp-7Ch]
  float v43; // [esp+F0h] [ebp-78h]
  float v44; // [esp+F4h] [ebp-74h]
  char v45; // [esp+F8h] [ebp-70h]
  float modelMatrix[16]; // [esp+118h] [ebp-50h]
  int v47; // [esp+164h] [ebp-4h]

  v4 = bse_debug.internalVar->integerValue;
  v5 = this;
  v37 = this;
  if ( v4 )
  {
    work.data = 0;
    *(_DWORD *)work.baseBuffer = -20;
    work.alloced = (int)&work.baseBuffer[4];
    work.baseBuffer[4] = 0;
    v6 = *(int (**)(void))this->mDeclEffect->base->vfptr->gap4;
    v47 = 0;
    v7 = v6();
    v8 = va("(%g) (%g) (%g)\n%s", v5->mCost, v5->mDeclEffect->mSize, parms->shaderParms[6], v7);
    idStr::operator=((idStr *)((char *)&work + 4), v8);
    ((void (__stdcall *)(idVec3 *, idMat3 *, _DWORD))view->renderWorld->vfptr->DebugAxis)(
      &parms->origin,
      &parms->axis,
      0);
    ((void (__stdcall *)(int, idVec3 *, _DWORD, idVec4 *, idMat3 *, signed int, _DWORD))view->renderWorld->vfptr->DrawTextA)(
      work.alloced,
      &parms->origin,
      0.30000001,
      &colorCyan,
      &view->renderView.viewaxis,
      1,
      0);
    v47 = -1;
    idStr::FreeData((idStr *)((char *)&work + 4));
  }
  if ( bse_showBounds.internalVar->integerValue )
  {
    transformed.y = bounds->b[1].x + bounds->b[0].x;
    transformed.z = bounds->b[1].y + bounds->b[0].y;
    v.x = bounds->b[1].z + bounds->b[0].z;
    v.y = transformed.y * 0.5;
    v.z = transformed.z * 0.5;
    modelBounds.b[0].x = 0.5 * v.x;
    transformed.y = bounds->b[1].x - v.y;
    transformed.z = bounds->b[1].y - v.z;
    v.x = bounds->b[1].z - modelBounds.b[0].x;
    v42 = transformed.y;
    v43 = transformed.z;
    v44 = v.x;
    transformed.y = parms->axis.mat[1].x * v.z + parms->axis.mat[0].x * v.y + parms->axis.mat[2].x * modelBounds.b[0].x;
    transformed.z = parms->axis.mat[1].y * v.z + parms->axis.mat[0].y * v.y + parms->axis.mat[2].y * modelBounds.b[0].x;
    v.x = v.y * parms->axis.mat[0].z + v.z * parms->axis.mat[1].z + modelBounds.b[0].x * parms->axis.mat[2].z;
    v.y = transformed.y + parms->origin.x;
    v.z = parms->origin.y + transformed.z;
    modelBounds.b[0].x = parms->origin.z + v.x;
    v39 = v.y;
    v40 = v.z;
    v41 = modelBounds.b[0].x;
    qmemcpy(&v45, &parms->axis, 0x24u);
    ((void (__stdcall *)(idVec4 *, float *, _DWORD))view->renderWorld->vfptr->DebugBox)(&colorBlue, &v39, 0);
    (*(void (__stdcall **)(idMat3 *, idVec3 *, float *))&renderUtilities->vfptr->gap4[4])(
      &parms->axis,
      &parms->origin,
      &modelMatrix[1]);
    globalBounds.b[1].x = 1.0e30;
    globalBounds.b[0].z = 1.0e30;
    work.len = (int)&globalBounds.b[0].y;
    globalBounds.b[0].y = 1.0e30;
    v9 = 0.0;
    v36 = &transformed.y;
    transformed.x = -1.0e30;
    v34 = &transformed.y;
    v10 = transformed.x;
    transformed.x = 0.0;
    v31 = v10;
    v32 = &modelBounds.b[0].y;
    globalBounds.b[1].z = v31;
    globalBounds.b[1].y = v31;
    modelBounds.b[1].x = 1.0e30;
    modelBounds.b[0].z = 1.0e30;
    modelBounds.b[0].y = 1.0e30;
    globalBounds.b[0].x = v10;
    modelBounds.b[1].z = globalBounds.b[0].x;
    modelBounds.b[1].y = globalBounds.b[0].x;
    while ( 1 )
    {
      v11 = LOBYTE(v9) & 1;
      v.y = *((float *)&v37->vfptr + 3 * v11 + 93);
      v12 = v37->mCurrentLocalBounds.b[(SLODWORD(v9) >> 1) & 1].y;
      v33 = 12 * ((SLODWORD(v9) >> 1) & 1);
      v.z = v12;
      v13 = v37->mCurrentLocalBounds.b[(SLODWORD(v9) >> 2) & 1].z;
      v35 = 12 * ((SLODWORD(v9) >> 2) & 1);
      modelBounds.b[0].x = v13;
      (*(void (__stdcall **)(float *, float *, float *))renderUtilities->vfptr->gap4)(
        &modelMatrix[1],
        &v.y,
        &transformed.y);
      v14 = (float *)work.len;
      v15 = v36;
      v16 = *v36;
      *(float *)work.len = fminf(*(float *)work.len, *v36);
      v17 = v15[1];
      v14[1] = fminf(v14[1], v17);
      v18 = v15[2];
      v14[2] = fminf(v14[2], v18);
      v14[3] = fmaxf(v16, v14[3]);
      v14[4] = fmaxf(v17, v14[4]);
      v14[5] = fmaxf(v18, v14[5]);
      v.y = bounds->b[v11].x;
      v.z = bounds->b[v33 / 0xC].y;
      modelBounds.b[0].x = bounds->b[v35 / 0xC].z;
      (*(void (__stdcall **)(float *, float *, float *))renderUtilities->vfptr->gap4)(
        &modelMatrix[1],
        &v.y,
        &transformed.y);
      v19 = v32;
      v20 = v34;
      v21 = *v34;
      *v32 = fminf(*v32, *v34);
      v22 = v20[1];
      v19[1] = fminf(v19[1], v22);
      v23 = v20[2];
      v19[2] = fminf(v19[2], v23);
      v19[3] = fmaxf(v21, v19[3]);
      v19[4] = fmaxf(v22, v19[4]);
      v19[5] = fmaxf(v23, v19[5]);
      v25 = __OFSUB__(LODWORD(transformed.x) + 1, 8);
      v24 = LODWORD(transformed.x)++ - 7 < 0;
      if ( !(v24 ^ v25) )
        break;
      v9 = transformed.x;
    }
    ((void (__stdcall *)(idVec4 *, float *, idVec3 *, idMat3 *, _DWORD))view->renderWorld->vfptr->DebugBounds)(
      &colorGreen,
      &globalBounds.b[0].y,
      &vec3_origin,
      &mat3_identity,
      0);
    v26 = 0;
    while ( *(float *)((char *)&globalBounds.b[1].y + v26) >= (double)*(float *)((char *)&modelBounds.b[1].y + v26)
         && *(float *)((char *)&globalBounds.b[0].y + v26) <= (double)*(float *)((char *)&modelBounds.b[0].y + v26) )
    {
      v26 += 4;
      if ( v26 >= 12 )
        return;
    }
    ((void (__stdcall *)(idVec4 *, float *, idVec3 *, idMat3 *, _DWORD))view->renderWorld->vfptr->DebugBounds)(
      &colorRed,
      &modelBounds.b[0].y,
      &vec3_origin,
      &mat3_identity,
      0);
  }
}
void __thiscall rvBSE::InitModel(rvBSE *this, idRenderModel *model)
{
  rvBSE *v2; // esi
  int v3; // edi
  int v4; // ebx

  v2 = this;
  v3 = 0;
  if ( this->mSegments.num > 0 )
  {
    v4 = 0;
    do
    {
      rvSegment::AllocateSurface(&v2->mSegments.list[v4], v2, model);
      ++v3;
      ++v4;
    }
    while ( v3 < v2->mSegments.num );
  }
}
idRenderModel *__userpurge rvBSE::Render@<eax>(rvBSE *this@<ecx>, int a2@<ebp>, int a3@<edi>, int a4@<esi>, idRenderModel *model, renderEffect_s *owner, viewDef_s *view, int a8, renderEffect_s *a9, float *a10)
{
  rvBSE *v10; // ebx
  idRenderModel *v12; // ebp
  float *v13; // eax
  double v14; // st7
  int v15; // esi
  double v16; // st7
  double v17; // st6
  signed int v18; // esi
  double v19; // st5
  float *v20; // ecx
  char *v21; // edi
  float v22; // ST2C_4
  float v23; // ST30_4
  float v24; // ST34_4
  int v25; // [esp+Ch] [ebp-50h]
  int v26; // [esp+10h] [ebp-4Ch]
  int v27; // [esp+14h] [ebp-48h]
  int i; // [esp+18h] [ebp-44h]
  int j; // [esp+28h] [ebp-34h]
  float v30; // [esp+38h] [ebp-24h]
  float v31; // [esp+3Ch] [ebp-20h]
  idBounds b; // [esp+40h] [ebp-1Ch]
  float v33; // [esp+58h] [ebp-4h]
  float retaddr; // [esp+5Ch] [ebp+0h]
  float v35; // [esp+6Ch] [ebp+10h]
  float v36; // [esp+6Ch] [ebp+10h]

  v10 = this;
  if ( !bse_render.internalVar->integerValue )
    return 0;
  v27 = a2;
  v12 = model;
  v26 = a4;
  v25 = a3;
  if ( *(float *)&model == 0.0 )
  {
    v12 = (idRenderModel *)((int (__cdecl *)(int, int, int))renderUtilities->vfptr->CreateBSERenderModel)(a3, a4, v27);
    rvBSE::InitModel(v10, v12);
  }
  ((void (__thiscall *)(idRenderModel *, int, int, int))v12->vfptr->FreeVertexCache)(v12, v25, v26, v27);
  v13 = (float *)v12->vfptr->Bounds(v12, (idBounds *)&v30, 0);
  v13[2] = 1.0e30;
  v13[1] = 1.0e30;
  *v13 = 1.0e30;
  v35 = -1.0e30;
  v13[5] = v35;
  v13[4] = v35;
  v13[3] = v35;
  qmemcpy(&v10->mViewAxis, a10 + 84, sizeof(v10->mViewAxis));
  v10->mViewOrg.x = a10[81];
  v10->mViewOrg.y = a10[82];
  v14 = a10[83];
  v10->mViewOrg.z = a10[83];
  if ( ((unsigned __int8 (*)(void))bse->vfptr->IsTimeLocked)() )
    ((void (*)(void))bse->vfptr->GetLockedTime)();
  else
    v14 = a10[122];
  v15 = 0;
  v36 = v14;
  for ( j = 0; j < v10->mSegments.num; ++j )
  {
    rvSegment::ClearSurface(&v10->mSegments.list[v15], (int)v12, v15 * 80, v10, v12, i);
    if ( rvSegment::Active(&v10->mSegments.list[v15]) )
    {
      rvSegment::Render(&v10->mSegments.list[v15], v10, a9, v12, v36);
      rvSegment::RenderTrail(&v10->mSegments.list[v15], v10, a9, v12, v36);
    }
    ++v15;
  }
  (*(void (__thiscall **)(idRenderModel *, signed int, signed int, _DWORD))&v12->vfptr->gap4[16])(v12, 1, 1, 0);
  v12->vfptr->Bounds(v12, (idBounds *)((char *)&b + 16), 0);
  v16 = b.b[1].y;
  v10->mLastRenderBounds.b[0].x = b.b[1].y;
  v17 = b.b[1].z;
  v10->mLastRenderBounds.b[0].y = b.b[1].z;
  v18 = 0;
  v19 = v33;
  v20 = (float *)&v10->mGrownRenderBounds;
  v10->mLastRenderBounds.b[0].z = v33;
  v21 = (char *)((char *)&b.b[1].y - (char *)&v10->mGrownRenderBounds);
  v10->mLastRenderBounds.b[1].x = retaddr;
  v10->mLastRenderBounds.b[1].y = *(float *)&model;
  v10->mLastRenderBounds.b[1].z = *(float *)&owner;
  while ( v20[3] >= (double)*(float *)((char *)v20 + (_DWORD)v21 + 12)
       && *v20 <= (double)*(float *)((char *)v20 + (_DWORD)v21) )
  {
    ++v18;
    ++v20;
    if ( v18 >= 3 )
      goto LABEL_18;
  }
  v30 = retaddr + 20.0;
  v31 = *(float *)&model + 20.0;
  b.b[0].x = *(float *)&owner + 20.0;
  v22 = v16 - 20.0;
  v23 = v17 - 20.0;
  v24 = v19 - 20.0;
  v10->mGrownRenderBounds.b[0].x = v22;
  v10->mGrownRenderBounds.b[0].y = v23;
  v10->mGrownRenderBounds.b[0].z = v24;
  v10->mGrownRenderBounds.b[1].x = v30;
  v10->mGrownRenderBounds.b[1].y = v31;
  v10->mGrownRenderBounds.b[1].z = b.b[0].x;
  v10->mForcePush = 1;
LABEL_18:
  v10->vfptr->DisplayDebugInfo(v10, a9, (viewDef_s *)a10, (idBounds *)((char *)&b + 16));
  return v12;
}
void __thiscall rvBSE::Destroy(rvBSE *this)
{
  rvBSE *v1; // esi
  rvSegment *v2; // eax
  rvParticle **v3; // edi

  v1 = this;
  v2 = this->mSegments.list;
  if ( v2 )
  {
    v3 = &v2[-1].mParticles;
    `eh vector destructor iterator'(
      v2,
      0x50u,
      (int)v2[-1].mParticles,
      (void (__thiscall *)(void *))rvSegment::~rvSegment);
    operator delete(v3);
  }
  v1->mSegments.list = 0;
  v1->mSegments.num = 0;
  v1->mSegments.size = 0;
  if ( v1->mReferenceSound )
    ((void (__stdcall *)(_DWORD))v1->mReferenceSound->vfptr->Free)(0);
}
void __thiscall rvBSE::UpdateSegments(rvBSE *this, float time)
{
  rvBSE *v2; // ebp
  int v3; // ebx
  rvSegment *v4; // eax
  rvParticle **v5; // esi
  bool v6; // sf
  unsigned __int8 v7; // of
  int v8; // ecx
  int *v9; // eax
  int v10; // esi
  rvSegment *v11; // ecx
  int v12; // edx
  int v13; // eax
  rvParticle **v14; // esi
  int v15; // esi
  int v16; // edi
  int v17; // esi
  int v18; // edi
  int v19; // esi
  int v20; // edi
  rvSegment *ptr; // [esp+14h] [ebp-14h]

  v2 = this;
  v3 = this->mDeclEffect->mSegmentTemplates.num;
  if ( v3 > 0 )
  {
    if ( v3 != this->mSegments.size )
    {
      v7 = __OFSUB__(v3, this->mSegments.num);
      v6 = v3 - this->mSegments.num < 0;
      ptr = this->mSegments.list;
      this->mSegments.size = v3;
      if ( v6 ^ v7 )
        this->mSegments.num = v3;
      v8 = 80 * v3 | -(80 * (unsigned __int64)(unsigned int)v3 >> 32 != 0);
      v9 = (int *)operator new((v8 + 4) | -__CFADD__(v8, 4));
      if ( v9 )
      {
        v10 = (int)(v9 + 1);
        *v9 = v3;
        `eh vector constructor iterator'(
          v9 + 1,
          0x50u,
          v3,
          (void (__thiscall *)(void *))rvSegment::rvSegment,
          (void (__thiscall *)(void *))rvSegment::~rvSegment);
      }
      else
      {
        v10 = 0;
      }
      v11 = ptr;
      v2->mSegments.list = (rvSegment *)v10;
      if ( ptr )
      {
        v12 = 0;
        if ( v2->mSegments.num > 0 )
        {
          v13 = 0;
          do
          {
            qmemcpy(&v2->mSegments.list[v13], &v11[v13], sizeof(v2->mSegments.list[v13]));
            v11 = ptr;
            ++v12;
            ++v13;
          }
          while ( v12 < v2->mSegments.num );
        }
        v14 = &v11[-1].mParticles;
        `eh vector destructor iterator'(
          v11,
          0x50u,
          (int)v11[-1].mParticles,
          (void (__thiscall *)(void *))rvSegment::~rvSegment);
        operator delete(v14);
      }
    }
  }
  else
  {
    v4 = this->mSegments.list;
    if ( v4 )
    {
      v5 = &v4[-1].mParticles;
      `eh vector destructor iterator'(
        v4,
        0x50u,
        (int)v4[-1].mParticles,
        (void (__thiscall *)(void *))rvSegment::~rvSegment);
      operator delete(v5);
    }
    v2->mSegments.list = 0;
    v2->mSegments.num = 0;
    v2->mSegments.size = 0;
  }
  v15 = 0;
  v2->mSegments.num = v3;
  if ( v3 > 0 )
  {
    v16 = 0;
    do
    {
      rvSegment::Init(&v2->mSegments.list[v16], v2, v2->mDeclEffect, v15++, time);
      ++v16;
    }
    while ( v15 < v2->mSegments.num );
  }
  v17 = 0;
  if ( v2->mSegments.num > 0 )
  {
    v18 = 0;
    do
    {
      rvSegment::CalcCounts(&v2->mSegments.list[v18], v2, time);
      ++v17;
      ++v18;
    }
    while ( v17 < v2->mSegments.num );
  }
  v19 = 0;
  if ( v2->mSegments.num > 0 )
  {
    v20 = 0;
    do
    {
      rvSegment::InitParticles(&v2->mSegments.list[v20], v2);
      ++v19;
      ++v20;
    }
    while ( v19 < v2->mSegments.num );
  }
}
void __thiscall rvBSE::Init(rvBSE *this, rvDeclEffect *declEffect, renderEffect_s *parms, float time)
{
  renderEffect_s *v4; // edx
  rvBSE *v5; // ebp
  double v6; // st6
  signed int v7; // esi
  float *v8; // ecx
  int v9; // eax
  double v10; // st3
  double v11; // st5
  float v12; // ST28_4
  float v13; // ST2C_4
  float *v14; // esi
  float v15; // ST30_4
  int v16; // eax
  signed int v17; // ecx
  float v18; // ST1C_4
  float v19; // ST20_4
  float v20; // ST24_4
  double v21; // st5
  float v22; // [esp+30h] [ebp-18h]
  float v23; // [esp+34h] [ebp-14h]
  float v24; // [esp+38h] [ebp-10h]
  float v25; // [esp+3Ch] [ebp-Ch]
  float v26; // [esp+40h] [ebp-8h]
  float v27; // [esp+44h] [ebp-4h]
  float parmsa; // [esp+50h] [ebp+8h]

  v4 = parms;
  v5 = this;
  this->mStartTime = time;
  this->mDeclEffect = declEffect;
  this->mLastTime = time;
  v6 = 0.0;
  this->mFlags = 0;
  this->mDuration = 0.0;
  this->mAttenuation = 1.0;
  v7 = 2;
  this->mCost = 0.0;
  this->mFlags = parms->loop != 0;
  this->mCurrentLocalBounds.b[1].z = 0.0;
  this->mCurrentLocalBounds.b[1].y = 0.0;
  v8 = &this->mLastRenderBounds.b[0].z;
  v5->mCurrentLocalBounds.b[1].x = 0.0;
  v5->mCurrentLocalBounds.b[0].z = 0.0;
  v5->mCurrentLocalBounds.b[0].y = 0.0;
  v5->mCurrentLocalBounds.b[0].x = 0.0;
  v9 = (int)&v5->mCurrentLocalBounds.b[0].z;
  v10 = declEffect->mSize;
  v5->mCurrentLocalBounds.b[0].x = v5->mCurrentLocalBounds.b[0].x - v10;
  v5->mCurrentLocalBounds.b[0].y = v5->mCurrentLocalBounds.b[0].y - v10;
  v5->mCurrentLocalBounds.b[0].z = v5->mCurrentLocalBounds.b[0].z - v10;
  v5->mCurrentLocalBounds.b[1].x = v5->mCurrentLocalBounds.b[1].x + v10;
  v5->mCurrentLocalBounds.b[1].y = v10 + v5->mCurrentLocalBounds.b[1].y;
  v5->mCurrentLocalBounds.b[1].z = v10 + v5->mCurrentLocalBounds.b[1].z;
  do
  {
    v11 = *(float *)(v9 - 8);
    v9 += 12;
    *(v8 - 2) = v11;
    v8 += 3;
    --v7;
    *(v8 - 4) = *(float *)(v9 - 16);
    *(v8 - 3) = *(float *)(v9 - 12);
  }
  while ( v7 );
  v5->mGrownRenderBounds.b[0].z = 1.0e30;
  v5->mGrownRenderBounds.b[0].y = 1.0e30;
  v5->mGrownRenderBounds.b[0].x = 1.0e30;
  parmsa = -1.0e30;
  v5->mGrownRenderBounds.b[1].z = parmsa;
  v5->mGrownRenderBounds.b[1].y = parmsa;
  v5->mGrownRenderBounds.b[1].x = parmsa;
  v5->mForcePush = 0;
  v5->mOriginalOrigin.x = v4->origin.x;
  v5->mOriginalOrigin.y = v4->origin.y;
  v5->mOriginalOrigin.z = v4->origin.z;
  v5->mOriginalEndOrigin.z = 0.0;
  v5->mOriginalEndOrigin.y = 0.0;
  v5->mOriginalEndOrigin.x = 0.0;
  qmemcpy(&v5->mOriginalAxis, &v4->axis, sizeof(v5->mOriginalAxis));
  v12 = v5->mOriginalOrigin.x + v5->mCurrentLocalBounds.b[1].x;
  v13 = v5->mCurrentLocalBounds.b[1].y + v5->mOriginalOrigin.y;
  v14 = &v22;
  v15 = v5->mCurrentLocalBounds.b[1].z + v5->mOriginalOrigin.z;
  v16 = (int)&v5->mCurrentWorldBounds.b[0].y;
  v17 = 2;
  v18 = v5->mOriginalOrigin.x + v5->mCurrentLocalBounds.b[0].x;
  v19 = v5->mCurrentLocalBounds.b[0].y + v5->mOriginalOrigin.y;
  v20 = v5->mCurrentLocalBounds.b[0].z + v5->mOriginalOrigin.z;
  v22 = v18;
  v23 = v19;
  v24 = v20;
  v25 = v12;
  v26 = v13;
  v27 = v15;
  do
  {
    v21 = *v14;
    v14 += 3;
    *(float *)(v16 - 4) = v21;
    v16 += 12;
    --v17;
    *(float *)(v16 - 12) = *(float *)((char *)&v22 - (char *)&v5->mCurrentWorldBounds + v16 - 12);
    *(float *)(v16 - 8) = *(float *)((char *)&v23 - (char *)&v5->mCurrentWorldBounds + v16 - 12);
  }
  while ( v17 );
  if ( v4->hasEndOrigin )
  {
    v5->mFlags |= 2u;
    v5->mOriginalEndOrigin.x = v4->endOrigin.x;
    v5->mOriginalEndOrigin.y = v4->endOrigin.y;
    v5->mOriginalEndOrigin.z = v4->endOrigin.z;
    v5->mCurrentEndOrigin.x = v4->endOrigin.x;
    v5->mCurrentEndOrigin.y = v4->endOrigin.y;
    v5->mCurrentEndOrigin.z = v4->endOrigin.z;
  }
  v5->mCurrentTime = time;
  v5->mCurrentOrigin.x = v5->mOriginalOrigin.x;
  v5->mCurrentOrigin.y = v5->mOriginalOrigin.y;
  v5->mCurrentOrigin.z = v5->mOriginalOrigin.z;
  v5->mCurrentVelocity.z = *(float *)&v6;
  v5->mCurrentVelocity.y = *(float *)&v6;
  v5->mCurrentVelocity.x = *(float *)&v6;
  rvBSE::UpdateFromOwner(v5, v4, time, 1);
  v5->mReferenceSound = 0;
  if ( declEffect->mFlags & 1 )
    v5->mReferenceSound = (idSoundEmitter *)((int (*)(void))session->sw->vfptr->AllocSoundEmitter)();
  rvBSE::UpdateSegments(v5, time);
  v5->mOriginDistanceToCamera = 0.0;
  v5->mShortestDistanceToCamera = 0.0;
}
int dynamic_initializer_for__singletonLock___96()
{
  sdLock::sdLock(&singletonLock_96);
  return atexit(dynamic_atexit_destructor_for__singletonLock___96);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___96()
{
  sdLock::~sdLock(&singletonLock_96);
}
