char __thiscall rvParticle::GetEvaluationTime(rvParticle *this, float time, float *evalTime, bool infinite)
{
  double v4; // st7

  v4 = time;
  *evalTime = time - this->mStartTime;
  if ( this->mEndTime - 0.002000000094994903 <= time )
    *evalTime = this->mEndTime - this->mStartTime - 0.002000000094994903;
  if ( infinite )
    return 1;
  if ( this->mStartTime - 0.002000000094994903 >= v4 || this->mEndTime <= v4 )
    return 0;
  return 1;
}
int __thiscall rvParticle::HandleTint(rvParticle *this, rvBSE *effect, idVec4 *colour, float alpha)
{
  int v4; // edx
  double v5; // st7
  idVec4 tint; // [esp+0h] [ebp-14h]
  float v8; // [esp+10h] [ebp-4h]

  v4 = this->mFlags;
  if ( v4 & 0x8000 )
  {
    tint.y = colour->x * colour->w * alpha * effect->mTint.x;
    tint.z = colour->y * colour->w * alpha * effect->mTint.y;
    tint.w = colour->z * colour->w * alpha * effect->mTint.z;
    v5 = alpha * (effect->mTint.w * colour->w);
  }
  else
  {
    tint.y = effect->mTint.x * colour->x;
    tint.z = effect->mTint.y * colour->y;
    tint.w = effect->mTint.z * colour->z;
    v5 = effect->mTint.w * colour->w * alpha;
  }
  v8 = v5;
  if ( v4 & 0x1000000 )
  {
    tint.y = effect->mMaterialColor.x * tint.y;
    tint.z = effect->mMaterialColor.y * tint.z;
    tint.w = effect->mMaterialColor.z * tint.w;
  }
  tint.y = effect->mBrightness * tint.y;
  tint.z = effect->mBrightness * tint.z;
  tint.w = effect->mBrightness * tint.w;
  return sdColor4::PackColor((idVec4 *)((char *)&tint + 4));
}
void __stdcall rvParticle::Attenuate(float atten, rvParticleParms *parms, rvEnvParms1Particle *result)
{
  char v3; // al

  v3 = parms->mFlags;
  if ( v3 & 0x60 )
  {
    if ( v3 & 0x40 )
      atten = 1.0 - atten;
    result->mStart = result->mStart * atten;
    result->mEnd = atten * result->mEnd;
  }
}
void __thiscall rvParticle::RenderQuadTrail(rvParticle *this, rvBSE *effect, srfTriangles_t *tri, idVec3 offset, float fraction, idVec4 *colour, idVec3 *pos, bool first)
{
  rvParticle *v8; // ebp
  int v9; // eax
  idDrawVert *v10; // esi
  float v11; // ST1C_4
  float v12; // ST20_4
  float v13; // ST24_4
  signed int v14; // eax
  double v15; // st7
  int tint; // [esp+24h] [ebp+4h]
  float tcoord; // [esp+3Ch] [ebp+1Ch]

  v8 = this;
  v9 = rvParticle::HandleTint(this, effect, colour, 1.0);
  v10 = &tri->verts[tri->numVerts];
  tint = v9;
  v11 = pos->x + offset.x;
  v12 = pos->y + offset.y;
  v13 = pos->z + offset.z;
  v10->xyz.x = v11;
  v10->xyz.y = v12;
  v10->xyz.z = v13;
  tcoord = (double)v8->mTrailRepeat * (fraction * 0.009999999776482582);
  v10->_st[0] = (signed int)(0.0 * 4096.0);
  v14 = (signed int)(4096.0 * tcoord);
  v10->_st[1] = v14;
  *(_DWORD *)v10->color = tint;
  ++v10;
  offset.x = pos->x - offset.x;
  offset.y = pos->y - offset.y;
  v15 = pos->z - offset.z;
  v10->_st[0] = 40;
  v10->_st[1] = v14;
  *(_DWORD *)v10->color = tint;
  offset.z = v15;
  v10->xyz = offset;
  if ( !first )
  {
    tri->indexes[tri->numIndexes] = tri->numVerts;
    tri->indexes[tri->numIndexes + 1] = LOWORD(tri->numVerts) - 1;
    tri->indexes[tri->numIndexes + 2] = LOWORD(tri->numVerts) - 2;
    tri->indexes[tri->numIndexes + 3] = LOWORD(tri->numVerts) - 1;
    tri->indexes[tri->numIndexes + 4] = tri->numVerts;
    tri->indexes[tri->numIndexes + 5] = LOWORD(tri->numVerts) + 1;
    tri->numIndexes += 6;
  }
  tri->numVerts += 2;
}
void __thiscall rvParticle::RenderMotion(rvParticle *this, rvBSE *effect, rvParticleTemplate *pt, srfTriangles_t *tri, renderEffect_s *owner, float time, float trailScale)
{
  rvParticle *v7; // esi
  double v8; // st7
  rvParticleTemplate *v9; // ebp
  rvEnvParms *v10; // edi
  float v11; // ST34_4
  float oneOverDuration; // ST34_4
  rvBSE *v13; // ebx
  float *v14; // eax
  float v15; // ST54_4
  float v16; // ST58_4
  float v17; // ST5C_4
  double v18; // st7
  double v19; // st6
  float v20; // ST54_4
  float v21; // ST58_4
  float v22; // ST5C_4
  float v23; // ST68_4
  double v24; // st6
  float v25; // ST6C_4
  float v26; // ST70_4
  double v27; // st7
  float v28; // ST6C_4
  float v29; // ST70_4
  int v30; // edi
  double v31; // st7
  idVec3 v32; // ST08_12
  idVec3 v33; // ST08_12
  float delta; // [esp+30h] [ebp-6Ch]
  float v35; // [esp+34h] [ebp-68h]
  float offset; // [esp+4Ch] [ebp-50h]
  float offset_4; // [esp+50h] [ebp-4Ch]
  float offset_4a; // [esp+50h] [ebp-4Ch]
  float offset_8; // [esp+54h] [ebp-48h]
  __int64 offset_8a; // [esp+54h] [ebp-48h]
  float v41; // [esp+58h] [ebp-44h]
  float v42; // [esp+5Ch] [ebp-40h]
  float v43; // [esp+60h] [ebp-3Ch]
  idVec3 position; // [esp+70h] [ebp-2Ch]
  idVec3 size; // [esp+7Ch] [ebp-20h]
  idVec4 colour; // [esp+88h] [ebp-14h]
  float dest; // [esp+98h] [ebp-4h]
  rvBSE *effecta; // [esp+A0h] [ebp+4h]
  float ib; // [esp+A4h] [ebp+8h]
  float ic; // [esp+A4h] [ebp+8h]
  float id; // [esp+A4h] [ebp+8h]
  float ie; // [esp+A4h] [ebp+8h]
  float ig; // [esp+A4h] [ebp+8h]
  int i; // [esp+A4h] [ebp+8h]
  float ih; // [esp+A4h] [ebp+8h]
  float ii; // [esp+A4h] [ebp+8h]
  int ia; // [esp+A4h] [ebp+8h]
  float ij; // [esp+A4h] [ebp+8h]
  float ik; // [esp+A4h] [ebp+8h]
  float il; // [esp+A4h] [ebp+8h]

  v7 = this;
  if ( this->mTrailCount )
  {
    v8 = time;
    delta = time - this->mTrailTime;
    if ( this->mStartTime > (double)delta )
      delta = this->mStartTime;
    v35 = v8 - delta;
    if ( v35 > 0.0020000001 )
    {
      v9 = pt;
      v10 = pt->mpFadeEnvelope;
      v11 = this->mEndTime - this->mStartTime;
      oneOverDuration = 1.0 / v11;
      ib = v8 - this->mStartTime;
      rvEnvParms::Evaluate(v9->mpTintEnvelope, &this->mTintEnv, ib, oneOverDuration, &colour.y);
      rvEnvParms::Evaluate(v10, &v7->mFadeEnv, ib, oneOverDuration, &dest);
      ic = time - v7->mStartTime;
      (*(void (__thiscall **)(rvParticle *, rvEnvParms *, _DWORD, _DWORD, float *))&v7->vfptr->gap4[8])(
        v7,
        v9->mpSizeEnvelope,
        LODWORD(ic),
        LODWORD(oneOverDuration),
        &size.y);
      v13 = effect;
      id = time - v7->mMotionStartTime;
      rvParticle::EvaluatePosition(v7, effect, v9, (idVec3 *)((char *)&position + 4), id);
      v14 = (float *)owner;
      offset = dest;
      v15 = effect->mViewOrg.x - owner->origin.x;
      v16 = effect->mViewOrg.y - owner->origin.y;
      v17 = effect->mViewOrg.z - owner->origin.z;
      v18 = v16;
      v19 = v15;
      v20 = v14[7] * v15 + v16 * v14[8] + v17 * owner->axis.mat[0].z;
      v21 = v16 * v14[11] + v14[10] * v19 + v17 * owner->axis.mat[1].z;
      v22 = v17 * owner->axis.mat[2].z + v19 * v14[13] + v18 * v14[14];
      v42 = v20 - v7->mInitPos.x;
      v43 = v21 - v7->mInitPos.y;
      v23 = v22 - v7->mInitPos.z;
      v24 = v23;
      v25 = v7->mVelocity.y * v23 - v7->mVelocity.z * v43;
      v26 = v7->mVelocity.z * v42 - v7->mVelocity.x * v23;
      position.x = v7->mVelocity.x * v43 - v7->mVelocity.y * v42;
      offset_4 = v25;
      offset_8 = v26;
      v41 = position.x;
      ie = position.x * position.x + v26 * v26 + v25 * v25;
      if ( ie == 0.0 )
      {
        v27 = v43 * 0.0;
        v28 = v24 - v27;
        v29 = v42 * 0.0 - 0.0 * v24;
        position.x = v27 - v42;
        offset_4 = v28;
        offset_8 = v29;
        v41 = position.x;
      }
      ig = offset_8 * offset_8 + offset_4 * offset_4 + v41 * v41;
      *(float *)&i = sqrt(ig);
      if ( *(float *)&i >= 0.00000011920929 )
      {
        ih = 1.0 / *(float *)&i;
        offset_4 = offset_4 * ih;
        offset_8 = offset_8 * ih;
        v41 = ih * v41;
      }
      v30 = 0;
      effecta = (rvBSE *)v7->mTrailCount;
      ii = size.y * trailScale;
      v31 = ii;
      ia = 0;
      offset_4a = v31 * offset_4;
      *(float *)&offset_8a = offset_8 * v31;
      for ( *((float *)&offset_8a + 1) = v31 * v41; v30 < v7->mTrailCount; effecta = (rvBSE *)v7->mTrailCount )
      {
        ij = (double)ia / (double)(signed int)effecta;
        dest = (1.0 - ij) * offset;
        v32.x = offset_4a;
        *(_QWORD *)&v32.y = offset_8a;
        rvParticle::RenderQuadTrail(
          v7,
          v13,
          tri,
          v32,
          ij,
          (idVec4 *)((char *)&colour + 4),
          (idVec3 *)((char *)&position + 4),
          v30 == 0);
        ik = time - ij * v35;
        il = ik - v7->mMotionStartTime;
        rvParticle::EvaluatePosition(v7, v13, v9, (idVec3 *)((char *)&position + 4), il);
        ia = ++v30;
      }
      if ( v7->mTrailCount )
      {
        dest = 0.0;
        v33.x = offset_4a;
        *(_QWORD *)&v33.y = offset_8a;
        rvParticle::RenderQuadTrail(
          v7,
          v13,
          tri,
          v33,
          1.0,
          (idVec4 *)((char *)&colour + 4),
          (idVec3 *)((char *)&position + 4),
          0);
      }
    }
  }
}
char __thiscall rvSpriteParticle::Render(rvSpriteParticle *this, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time, float override)
{
  double v7; // st7
  rvSpriteParticle *v8; // ebx
  rvEnvParms *v9; // edi
  srfTriangles_t *v10; // edi
  idDrawVert *v11; // esi
  int v12; // ebp
  double v13; // st6
  double v14; // st5
  double v15; // st7
  double v16; // st7
  double v17; // st6
  double v18; // st5
  double v19; // st4
  double v20; // st3
  double v21; // st1
  double v22; // st2
  double v23; // st1
  double v24; // st1
  double v25; // rt0
  double v26; // st3
  float rotation; // [esp+34h] [ebp-6Ch]
  float s; // [esp+38h] [ebp-68h]
  float evalTime; // [esp+3Ch] [ebp-64h]
  float ooduration; // [esp+40h] [ebp-60h]
  idVec2 size; // [esp+44h] [ebp-5Ch]
  idVec3 position; // [esp+4Ch] [ebp-54h]
  float v34; // [esp+58h] [ebp-48h]
  float v35; // [esp+5Ch] [ebp-44h]
  float v36; // [esp+60h] [ebp-40h]
  float v37; // [esp+64h] [ebp-3Ch]
  float v38; // [esp+68h] [ebp-38h]
  float v39; // [esp+6Ch] [ebp-34h]
  float v40; // [esp+70h] [ebp-30h]
  float v41; // [esp+74h] [ebp-2Ch]
  float v42; // [esp+78h] [ebp-28h]
  float v43; // [esp+7Ch] [ebp-24h]
  float v44; // [esp+80h] [ebp-20h]
  float v45; // [esp+84h] [ebp-1Ch]
  float v46; // [esp+88h] [ebp-18h]
  idVec4 tint; // [esp+8Ch] [ebp-14h]
  float dest; // [esp+9Ch] [ebp-4h]
  char effecta; // [esp+A4h] [ebp+4h]
  char pta; // [esp+A8h] [ebp+8h]
  int texOfs; // [esp+B0h] [ebp+10h]
  float timea; // [esp+B4h] [ebp+14h]
  int texScaleOfs; // [esp+B8h] [ebp+18h]

  v7 = time;
  v8 = this;
  ooduration = time - this->mStartTime;
  if ( this->mEndTime - 0.002000000094994903 <= time )
    ooduration = this->mEndTime - this->mStartTime - 0.002000000094994903;
  if ( this->mStartTime - 0.002000000094994903 >= v7 || this->mEndTime <= v7 )
    return 0;
  size.x = this->mEndTime - this->mStartTime;
  v9 = pt->mpFadeEnvelope;
  size.x = 1.0 / size.x;
  rvEnvParms::Evaluate(pt->mpTintEnvelope, &this->mTintEnv, ooduration, size.x, &tint.y);
  rvEnvParms::Evaluate(v9, &v8->mFadeEnv, ooduration, size.x, &dest);
  (*(void (__thiscall **)(rvSpriteParticle *, rvEnvParms *, _DWORD, _DWORD, float *))&v8->vfptr->gap4[8])(
    v8,
    pt->mpSizeEnvelope,
    LODWORD(ooduration),
    LODWORD(size.x),
    &size.y);
  (*(void (__thiscall **)(rvSpriteParticle *, rvEnvParms *, _DWORD, _DWORD, float *))&v8->vfptr->gap4[12])(
    v8,
    pt->mpRotateEnvelope,
    LODWORD(ooduration),
    LODWORD(size.x),
    &s);
  timea = time - v8->mMotionStartTime;
  rvParticle::EvaluatePosition((rvParticle *)&v8->vfptr, effect, pt, (idVec3 *)((char *)&position + 4), timea);
  v10 = tri;
  v11 = &tri->verts[tri->numVerts];
  v12 = rvParticle::HandleTint((rvParticle *)&v8->vfptr, effect, (idVec4 *)((char *)&tint + 4), override);
  rotation = cos(s);
  evalTime = sin(s);
  v38 = view->mat[2].x * evalTime;
  v39 = view->mat[2].y * evalTime;
  v40 = view->mat[2].z * evalTime;
  v35 = view->mat[1].x * rotation;
  v36 = view->mat[1].y * rotation;
  v37 = view->mat[1].z * rotation;
  v41 = v35 - v38;
  v42 = v36 - v39;
  v43 = v37 - v40;
  v35 = view->mat[2].x * rotation;
  v36 = view->mat[2].y * rotation;
  v37 = rotation * view->mat[2].z;
  v38 = view->mat[1].x * evalTime;
  v39 = view->mat[1].y * evalTime;
  v40 = evalTime * view->mat[1].z;
  v44 = v38 + v35;
  v45 = v39 + v36;
  v46 = v40 + v37;
  v13 = v41;
  v38 = size.y * v41;
  v14 = v42;
  v39 = size.y * v42;
  v15 = v43;
  v40 = size.y * v43;
  v41 = position.x * v44;
  v42 = position.x * v45;
  v43 = position.x * v46;
  texScaleOfs = (unsigned __int16)(signed int)((v8->mTextureScale + v8->mTextureOffset) * 0.009999999776482582 * 4096.0);
  texOfs = (unsigned __int16)(signed int)(4096.0 * (0.009999999776482582 * v8->mTextureOffset));
  v44 = v13 * 127.0 + 128.0;
  effecta = (signed int)fminf(fmaxf(v44, 0.0), 255.0);
  v44 = v14 * 127.0 + 128.0;
  LOBYTE(timea) = (signed int)fminf(fmaxf(v44, 0.0), 255.0);
  v44 = v15 * 127.0 + 128.0;
  pta = (signed int)fminf(fmaxf(v44, 0.0), 255.0);
  v16 = v41;
  v44 = position.y - v41;
  v17 = v42;
  v45 = position.z - v42;
  v18 = v43;
  v46 = v34 - v43;
  v19 = v38;
  v41 = v44 - v38;
  v20 = v39;
  v42 = v45 - v39;
  v21 = v46 - v40;
  v22 = v40;
  v11->_st[0] = texScaleOfs;
  v11->_st[1] = 40;
  v43 = v21;
  v11->xyz.x = v41;
  v11->xyz.y = v42;
  v11->xyz.z = v43;
  v11->_normal[0] = (signed int)position.y;
  v11->_normal[1] = (signed int)position.z;
  v11->_tangent[0] = (signed int)v34;
  v11->_signs[1] = LOBYTE(timea);
  v11->_signs[2] = pta;
  v11->_signs[0] = effecta;
  *(_DWORD *)v11->color = v12;
  ++v11;
  v44 = position.y - v16;
  v45 = position.z - v17;
  v23 = v34;
  v11->_st[0] = texOfs;
  v11->_st[1] = 40;
  v46 = v23 - v18;
  v41 = v44 + v19;
  v42 = v45 + v20;
  v43 = v46 + v22;
  v11->xyz.x = v41;
  v11->xyz.y = v42;
  v11->xyz.z = v43;
  v11->_normal[0] = (signed int)position.y;
  v11->_normal[1] = (signed int)position.z;
  v11->_tangent[0] = (signed int)v34;
  v11->_signs[0] = effecta;
  v11->_signs[1] = LOBYTE(timea);
  v11->_signs[2] = pta;
  *(_DWORD *)v11->color = v12;
  ++v11;
  v44 = position.y + v16;
  v45 = position.z + v17;
  v46 = v34 + v18;
  v24 = v44 + v19;
  *(_DWORD *)v11->_st = (unsigned __int16)texOfs;
  v41 = v24;
  v42 = v45 + v20;
  v43 = v46 + v22;
  v11->xyz.x = v41;
  v11->xyz.y = v42;
  v11->xyz.z = v43;
  v11->_normal[0] = (signed int)position.y;
  v11->_normal[1] = (signed int)position.z;
  v11->_tangent[0] = (signed int)v34;
  v11->_signs[1] = LOBYTE(timea);
  v11->_signs[2] = pta;
  v11->_signs[0] = effecta;
  *(_DWORD *)v11->color = v12;
  ++v11;
  v44 = v16 + position.y;
  v25 = v20;
  v45 = v17 + position.z;
  v26 = v34;
  v11->_st[0] = texScaleOfs;
  v11->_st[1] = 0;
  v46 = v18 + v26;
  v41 = v44 - v19;
  v42 = v45 - v25;
  v43 = v46 - v22;
  v11->xyz.x = v41;
  v11->xyz.y = v42;
  v11->xyz.z = v43;
  v11->_normal[0] = (signed int)position.y;
  v11->_normal[1] = (signed int)position.z;
  v11->_tangent[0] = (signed int)v34;
  v11->_signs[0] = effecta;
  v11->_signs[1] = LOBYTE(timea);
  v11->_signs[2] = pta;
  *(_DWORD *)v11->color = v12;
  v10->indexes[v10->numIndexes] = v10->numVerts;
  v10->indexes[v10->numIndexes + 1] = LOWORD(v10->numVerts) + 1;
  v10->indexes[v10->numIndexes + 2] = LOWORD(v10->numVerts) + 2;
  v10->indexes[v10->numIndexes + 3] = v10->numVerts;
  v10->indexes[v10->numIndexes + 4] = LOWORD(v10->numVerts) + 2;
  v10->indexes[v10->numIndexes + 5] = LOWORD(v10->numVerts) + 3;
  v10->numVerts += 4;
  v10->numIndexes += 6;
  return 1;
}
char __thiscall rvLineParticle::Render(rvLineParticle *this, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time, float override)
{
  double v7; // st7
  rvLineParticle *v8; // ebx
  float v9; // ST34_4
  rvEnvParms *v10; // edi
  float oneOverDuration; // ST34_4
  float v12; // ST34_4
  int v13; // eax
  float v14; // ST34_4
  float v15; // ST34_4
  float v16; // ST34_4
  float v17; // ST34_4
  float v19; // ST34_4
  float v20; // ST34_4
  float v21; // ST34_4
  float v22; // ST34_4
  float v23; // ST34_4
  float v24; // ST38_4
  float v25; // ST38_4
  idDrawVert *v26; // esi
  int v27; // ebp
  double v28; // st6
  double v29; // st7
  double v30; // st5
  float v31; // ST38_4
  float v32; // ST38_4
  double v33; // st7
  double v34; // st6
  double v35; // st5
  float v36; // ST38_4
  int v37; // esi
  float v38; // ST38_4
  double v39; // st2
  double v40; // st1
  float v41; // ST38_4
  float v42; // ST38_4
  float v43; // [esp+34h] [ebp-8Ch]
  float v44; // [esp+34h] [ebp-8Ch]
  float v45; // [esp+34h] [ebp-8Ch]
  float maxl; // [esp+38h] [ebp-88h]
  float maxla; // [esp+38h] [ebp-88h]
  idVec3 len; // [esp+3Ch] [ebp-84h]
  idVec3 velocity; // [esp+48h] [ebp-78h]
  idVec3 position; // [esp+54h] [ebp-6Ch]
  float v51; // [esp+60h] [ebp-60h]
  float v52; // [esp+64h] [ebp-5Ch]
  float v53; // [esp+68h] [ebp-58h]
  idVec3 offset; // [esp+6Ch] [ebp-54h]
  float v55; // [esp+78h] [ebp-48h]
  float v56; // [esp+7Ch] [ebp-44h]
  float v57; // [esp+80h] [ebp-40h]
  float v58; // [esp+84h] [ebp-3Ch]
  float v59; // [esp+88h] [ebp-38h]
  float v60; // [esp+8Ch] [ebp-34h]
  float v61; // [esp+90h] [ebp-30h]
  float v62; // [esp+94h] [ebp-2Ch]
  float v63; // [esp+98h] [ebp-28h]
  float v64; // [esp+9Ch] [ebp-24h]
  float v65; // [esp+A0h] [ebp-20h]
  float v66; // [esp+A4h] [ebp-1Ch]
  float size; // [esp+A8h] [ebp-18h]
  idVec4 tint; // [esp+ACh] [ebp-14h]
  float dest; // [esp+BCh] [ebp-4h]

  v7 = time;
  v8 = this;
  len.x = time - this->mStartTime;
  if ( this->mEndTime - 0.002000000094994903 <= time )
    len.x = this->mEndTime - this->mStartTime - 0.002000000094994903;
  if ( this->mStartTime - 0.002000000094994903 >= v7 || this->mEndTime <= v7 )
    return 0;
  v9 = this->mEndTime - this->mStartTime;
  v10 = pt->mpFadeEnvelope;
  oneOverDuration = 1.0 / v9;
  rvEnvParms::Evaluate(pt->mpTintEnvelope, &this->mTintEnv, len.x, oneOverDuration, &tint.y);
  rvEnvParms::Evaluate(v10, &v8->mFadeEnv, len.x, oneOverDuration, &dest);
  (*(void (__thiscall **)(rvLineParticle *, rvEnvParms *, _DWORD, _DWORD, idVec4 *))&v8->vfptr->gap4[8])(
    v8,
    pt->mpSizeEnvelope,
    LODWORD(len.x),
    LODWORD(oneOverDuration),
    &tint);
  ((void (__thiscall *)(rvLineParticle *, rvEnvParms *, _DWORD, _DWORD, float *))v8->vfptr->EvaluateLength)(
    v8,
    pt->mpLengthEnvelope,
    LODWORD(len.x),
    LODWORD(oneOverDuration),
    &len.y);
  v12 = time - v8->mMotionStartTime;
  rvParticle::EvaluatePosition((rvParticle *)&v8->vfptr, effect, pt, (idVec3 *)((char *)&position + 4), v12);
  v13 = v8->mFlags;
  if ( !(v13 & 2) )
  {
    v59 = v8->mInitAxis.mat[0].x * effect->mCurrentAxis.mat[0].x
        + v8->mInitAxis.mat[0].y * effect->mCurrentAxis.mat[0].y
        + v8->mInitAxis.mat[0].z * effect->mCurrentAxis.mat[0].z;
    v60 = v8->mInitAxis.mat[0].x * effect->mCurrentAxis.mat[1].x
        + v8->mInitAxis.mat[0].y * effect->mCurrentAxis.mat[1].y
        + effect->mCurrentAxis.mat[1].z * v8->mInitAxis.mat[0].z;
    v61 = v8->mInitAxis.mat[0].x * effect->mCurrentAxis.mat[2].x
        + v8->mInitAxis.mat[0].y * effect->mCurrentAxis.mat[2].y
        + v8->mInitAxis.mat[0].z * effect->mCurrentAxis.mat[2].z;
    v62 = effect->mCurrentAxis.mat[0].x * v8->mInitAxis.mat[1].x
        + v8->mInitAxis.mat[1].y * effect->mCurrentAxis.mat[0].y
        + v8->mInitAxis.mat[1].z * effect->mCurrentAxis.mat[0].z;
    v63 = v8->mInitAxis.mat[1].y * effect->mCurrentAxis.mat[1].y
        + v8->mInitAxis.mat[1].x * effect->mCurrentAxis.mat[1].x
        + v8->mInitAxis.mat[1].z * effect->mCurrentAxis.mat[1].z;
    v64 = effect->mCurrentAxis.mat[2].x * v8->mInitAxis.mat[1].x
        + v8->mInitAxis.mat[1].y * effect->mCurrentAxis.mat[2].y
        + v8->mInitAxis.mat[1].z * effect->mCurrentAxis.mat[2].z;
    v65 = v8->mInitAxis.mat[2].x * effect->mCurrentAxis.mat[0].x
        + effect->mCurrentAxis.mat[0].y * v8->mInitAxis.mat[2].y
        + v8->mInitAxis.mat[2].z * effect->mCurrentAxis.mat[0].z;
    v66 = v8->mInitAxis.mat[2].x * effect->mCurrentAxis.mat[1].x
        + effect->mCurrentAxis.mat[1].y * v8->mInitAxis.mat[2].y
        + effect->mCurrentAxis.mat[1].z * v8->mInitAxis.mat[2].z;
    size = v8->mInitAxis.mat[2].y * effect->mCurrentAxis.mat[2].y
         + v8->mInitAxis.mat[2].x * effect->mCurrentAxis.mat[2].x
         + v8->mInitAxis.mat[2].z * effect->mCurrentAxis.mat[2].z;
    v14 = v65 * velocity.x + v62 * len.z + v59 * len.y;
    len.x = v60 * len.y + v63 * len.z + v66 * velocity.x;
    velocity.x = len.z * v64 + len.y * v61 + velocity.x * size;
    len.y = v14;
    len.z = len.x;
  }
  if ( v13 & 0x10000 )
  {
    v15 = time - v8->mMotionStartTime;
    rvParticle::EvaluateVelocity((rvParticle *)&v8->vfptr, effect, (idVec3 *)((char *)&velocity + 4), v15);
    v16 = velocity.z * velocity.z + velocity.y * velocity.y + position.x * position.x;
    v43 = sqrt(v16);
    if ( v43 >= 0.00000011920929 )
    {
      v17 = 1.0 / v43;
      velocity.y = v17 * velocity.y;
      velocity.z = velocity.z * v17;
      position.x = v17 * position.x;
    }
    v19 = velocity.x * velocity.x + len.y * len.y + len.z * len.z;
    v20 = sqrt(v19);
    v56 = v20 * velocity.y;
    v57 = velocity.z * v20;
    v58 = v20 * position.x;
    len.y = v56;
    len.z = v57;
    velocity.x = v58;
  }
  if ( ((unsigned int)effect->mFlags >> 1) & 1 )
  {
    v56 = effect->mCurrentEndOrigin.x - effect->mCurrentOrigin.x;
    v57 = effect->mCurrentEndOrigin.y - effect->mCurrentOrigin.y;
    v58 = effect->mCurrentEndOrigin.z - effect->mCurrentOrigin.z;
    v21 = v57 * v57 + v56 * v56 + v58 * v58;
    v22 = sqrt(v21);
    len.x = v22;
    v23 = position.z * position.z + position.y * position.y + v51 * v51;
    v44 = sqrt(v23);
    if ( len.x < (double)v44 )
      return 0;
    v24 = velocity.x * velocity.x + len.y * len.y + len.z * len.z;
    maxl = sqrt(v24);
    if ( len.x < maxl + v44 )
    {
      v45 = len.x - v44;
      if ( maxl >= 0.00000011920929 )
      {
        v25 = 1.0 / maxl;
        len.y = v25 * len.y;
        len.z = len.z * v25;
        velocity.x = v25 * velocity.x;
      }
      len.y = len.y * v45;
      len.z = len.z * v45;
      velocity.x = v45 * velocity.x;
    }
  }
  v26 = &tri->verts[tri->numVerts];
  v27 = rvParticle::HandleTint((rvParticle *)&v8->vfptr, effect, (idVec4 *)((char *)&tint + 4), override);
  v28 = len.y;
  v56 = position.y + len.y;
  v57 = position.z + len.z;
  v58 = v51 + velocity.x;
  velocity.y = len.y * 0.5;
  velocity.z = len.z * 0.5;
  position.x = 0.5 * velocity.x;
  v29 = velocity.x;
  offset.y = position.y + velocity.y;
  offset.z = position.z + velocity.z;
  v30 = len.z;
  v55 = v51 + position.x;
  len.y = view->mat[0].x - offset.y;
  len.z = view->mat[0].y - offset.z;
  velocity.x = view->mat[0].z - v55;
  velocity.y = velocity.x * v30 - len.z * v29;
  velocity.z = v29 * len.y - velocity.x * v28;
  position.x = v28 * len.z - len.y * v30;
  offset.y = velocity.y;
  offset.z = velocity.z;
  v55 = position.x;
  v31 = velocity.y * velocity.y + velocity.z * velocity.z + position.x * position.x;
  maxla = sqrt(v31);
  if ( maxla >= 0.00000011920929 )
  {
    v32 = 1.0 / maxla;
    offset.y = velocity.y * v32;
    offset.z = velocity.z * v32;
    v55 = v32 * position.x;
  }
  offset.y = offset.y * tint.x;
  offset.z = offset.z * tint.x;
  v55 = tint.x * v55;
  v33 = offset.y;
  v52 = offset.y + position.y;
  v34 = offset.z;
  v53 = offset.z + position.z;
  v35 = v55;
  offset.x = v55 + v51;
  v26->xyz.x = v52;
  v26->xyz.y = v53;
  v26->xyz.z = offset.x;
  v36 = v8->mTextureOffset * 0.009999999776482582;
  v26->_st[0] = (signed int)(v36 * 4096.0);
  v26->_st[1] = (signed int)(0.0 * 4096.0);
  v26->_normal[0] = (signed int)position.y;
  v26->_normal[1] = (signed int)position.z;
  v26->_tangent[0] = (signed int)v51;
  *(_DWORD *)v26->color = v27;
  v37 = (int)&v26[1];
  v52 = position.y - v33;
  v53 = position.z - v34;
  offset.x = v51 - v35;
  *(float *)v37 = v52;
  *(float *)(v37 + 4) = v53;
  *(float *)(v37 + 8) = offset.x;
  v38 = v8->mTextureOffset * 0.009999999776482582;
  *(_WORD *)(v37 + 28) = (signed int)(v38 * 4096.0);
  *(_WORD *)(v37 + 30) = 40;
  *(_WORD *)(v37 + 16) = (signed int)position.y;
  *(_WORD *)(v37 + 18) = (signed int)position.z;
  v39 = v56;
  v40 = v56;
  *(_WORD *)(v37 + 20) = (signed int)v51;
  *(_DWORD *)(v37 + 12) = v27;
  v37 += 32;
  v52 = v40 - v33;
  v53 = v57 - v34;
  offset.x = v58 - v35;
  *(float *)v37 = v52;
  *(float *)(v37 + 4) = v53;
  *(float *)(v37 + 8) = offset.x;
  v41 = (v8->mTextureOffset + v8->mTextureScale) * 0.009999999776482582;
  *(_WORD *)(v37 + 28) = (signed int)(v41 * 4096.0);
  *(_WORD *)(v37 + 30) = 40;
  *(_WORD *)(v37 + 16) = (signed int)position.y;
  *(_WORD *)(v37 + 18) = (signed int)position.z;
  *(_WORD *)(v37 + 20) = (signed int)v51;
  *(_DWORD *)(v37 + 12) = v27;
  v52 = v33 + v39;
  v37 += 32;
  v53 = v34 + v57;
  offset.x = v35 + v58;
  *(float *)v37 = v52;
  *(float *)(v37 + 4) = v53;
  *(float *)(v37 + 8) = offset.x;
  v42 = 0.009999999776482582 * (v8->mTextureOffset + v8->mTextureScale);
  *(_WORD *)(v37 + 28) = (signed int)(4096.0 * v42);
  *(_WORD *)(v37 + 30) = (signed int)(0.0 * 4096.0);
  *(_WORD *)(v37 + 16) = (signed int)position.y;
  *(_WORD *)(v37 + 18) = (signed int)position.z;
  *(_WORD *)(v37 + 20) = (signed int)v51;
  *(_DWORD *)(v37 + 12) = v27;
  tri->indexes[tri->numIndexes] = tri->numVerts;
  tri->indexes[tri->numIndexes + 1] = LOWORD(tri->numVerts) + 1;
  tri->indexes[tri->numIndexes + 2] = LOWORD(tri->numVerts) + 2;
  tri->indexes[tri->numIndexes + 3] = tri->numVerts;
  tri->indexes[tri->numIndexes + 4] = LOWORD(tri->numVerts) + 2;
  tri->indexes[tri->numIndexes + 5] = LOWORD(tri->numVerts) + 3;
  tri->numVerts += 4;
  tri->numIndexes += 6;
  return 1;
}
char __thiscall rvOrientedParticle::Render(rvOrientedParticle *this, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time, float override)
{
  double v7; // st7
  rvOrientedParticle *v8; // ebx
  rvEnvParms *v9; // edi
  float v10; // ST34_4
  float v11; // ST34_4
  srfTriangles_t *v12; // edi
  double v13; // st6
  idDrawVert *v14; // esi
  double v15; // st3
  int v16; // ebx
  double v17; // st7
  double v18; // st6
  double v19; // st5
  signed int v20; // eax
  double v21; // st4
  double v22; // st3
  double v23; // st2
  double v24; // st1
  double v25; // rt1
  double v26; // st3
  double v27; // st7
  float ooduration; // [esp+34h] [ebp-9Ch]
  idVec3 position; // [esp+38h] [ebp-98h]
  float v31; // [esp+44h] [ebp-8Ch]
  float v32; // [esp+48h] [ebp-88h]
  float v33; // [esp+4Ch] [ebp-84h]
  float v34; // [esp+50h] [ebp-80h]
  rvAngles angles; // [esp+54h] [ebp-7Ch]
  float v36; // [esp+60h] [ebp-70h]
  float v37; // [esp+64h] [ebp-6Ch]
  float v38; // [esp+68h] [ebp-68h]
  idVec2 size; // [esp+6Ch] [ebp-64h]
  float v40; // [esp+74h] [ebp-5Ch]
  float v41; // [esp+78h] [ebp-58h]
  float v42; // [esp+7Ch] [ebp-54h]
  float v43; // [esp+80h] [ebp-50h]
  float v44; // [esp+84h] [ebp-4Ch]
  float v45; // [esp+88h] [ebp-48h]
  idVec3 rotation; // [esp+8Ch] [ebp-44h]
  idVec4 tint; // [esp+98h] [ebp-38h]
  idMat3 transform; // [esp+A8h] [ebp-28h]
  float v49; // [esp+CCh] [ebp-4h]

  v7 = time;
  v8 = this;
  ooduration = time - this->mStartTime;
  if ( this->mEndTime - 0.002000000094994903 <= time )
    ooduration = this->mEndTime - this->mStartTime - 0.002000000094994903;
  if ( this->mStartTime - 0.002000000094994903 >= v7 || this->mEndTime <= v7 )
    return 0;
  position.x = this->mEndTime - this->mStartTime;
  v9 = pt->mpFadeEnvelope;
  position.x = 1.0 / position.x;
  rvEnvParms::Evaluate(pt->mpTintEnvelope, &this->mTintEnv, ooduration, position.x, &tint.y);
  rvEnvParms::Evaluate(v9, &v8->mFadeEnv, ooduration, position.x, (float *)&transform);
  (*(void (__thiscall **)(rvOrientedParticle *, rvEnvParms *, float, _DWORD, float *))&v8->vfptr->gap4[8])(
    v8,
    pt->mpSizeEnvelope,
    COERCE_FLOAT(LODWORD(ooduration)),
    LODWORD(position.x),
    &size.y);
  (*(void (__thiscall **)(rvOrientedParticle *, rvEnvParms *, float, _DWORD, float *))&v8->vfptr->gap4[12])(
    v8,
    pt->mpRotateEnvelope,
    COERCE_FLOAT(LODWORD(ooduration)),
    LODWORD(position.x),
    &rotation.y);
  v10 = time - v8->mMotionStartTime;
  rvParticle::EvaluatePosition((rvParticle *)&v8->vfptr, effect, pt, (idVec3 *)((char *)&position + 4), v10);
  angles.yaw = rotation.y;
  v36 = tint.x;
  v34 = cos(rotation.z);
  v11 = sin(rotation.z);
  v32 = cos(rotation.y);
  v33 = sin(rotation.y);
  size.x = tint.x;
  v38 = COERCE_FLOAT(&position);
  v37 = COERCE_FLOAT(&angles);
  position.x = cos(tint.x);
  angles.pitch = sin(tint.x);
  v12 = tri;
  v13 = angles.pitch * v33;
  v14 = &tri->verts[tri->numVerts];
  transform.mat[1].y = v34 * v13 - position.x * v11;
  transform.mat[1].z = v13 * v11 + position.x * v34;
  transform.mat[2].x = angles.pitch * v32;
  v15 = position.x * v33;
  transform.mat[2].y = v34 * v15 - -v11 * angles.pitch;
  transform.mat[2].z = v11 * v15 - v34 * angles.pitch;
  v49 = position.x * v32;
  v16 = rvParticle::HandleTint((rvParticle *)&v8->vfptr, effect, (idVec4 *)((char *)&tint + 4), override);
  v41 = -transform.mat[1].y;
  v42 = -transform.mat[1].z;
  v43 = -transform.mat[2].x;
  v44 = size.y * v41;
  v45 = size.y * v42;
  rotation.x = size.y * v43;
  angles.yaw = transform.mat[2].y * v40;
  angles.roll = transform.mat[2].z * v40;
  v36 = v40 * v49;
  v17 = v44;
  v41 = position.y - v44;
  v18 = v45;
  v42 = position.z - v45;
  v19 = rotation.x;
  v43 = v31 - rotation.x;
  v14->xyz.x = v41;
  v14->xyz.y = v42;
  v14->xyz.z = v43;
  v20 = (signed int)(0.0 * 4096.0);
  v14->_st[0] = v20;
  v14->_st[1] = v20;
  v14->_normal[0] = (signed int)position.y;
  v14->_normal[1] = (signed int)position.z;
  v14->_tangent[0] = (signed int)v31;
  *(_DWORD *)v14->color = v16;
  ++v14;
  v21 = angles.yaw;
  v44 = position.y - angles.yaw;
  v22 = angles.roll;
  v45 = position.z - angles.roll;
  v23 = v36;
  rotation.x = v31 - v36;
  v14->xyz.x = v44;
  v14->xyz.y = v45;
  v14->_st[0] = 40;
  v24 = rotation.x;
  v14->_st[1] = v20;
  v14->xyz.z = v24;
  v14->_normal[0] = (signed int)position.y;
  v14->_normal[1] = (signed int)position.z;
  v14->_tangent[0] = (signed int)v31;
  *(_DWORD *)v14->color = v16;
  ++v14;
  angles.yaw = v17 + position.y;
  v25 = v22;
  angles.roll = v18 + position.z;
  v26 = v31;
  v14->_st[0] = 40;
  v14->_st[1] = 40;
  v36 = v19 + v26;
  v14->xyz.x = angles.yaw;
  v14->xyz.y = angles.roll;
  v14->xyz.z = v36;
  v14->_normal[0] = (signed int)position.y;
  v14->_normal[1] = (signed int)position.z;
  v14->_tangent[0] = (signed int)v31;
  *(_DWORD *)v14->color = v16;
  ++v14;
  angles.yaw = v21 + position.y;
  angles.roll = v25 + position.z;
  v27 = v23 + v31;
  v14->_st[0] = v20;
  v14->_st[1] = 40;
  v36 = v27;
  v14->xyz.x = angles.yaw;
  v14->xyz.y = angles.roll;
  v14->xyz.z = v36;
  v14->_normal[0] = (signed int)position.y;
  v14->_normal[1] = (signed int)position.z;
  v14->_tangent[0] = (signed int)v31;
  *(_DWORD *)v14->color = v16;
  v12->indexes[v12->numIndexes] = v12->numVerts;
  v12->indexes[v12->numIndexes + 1] = LOWORD(v12->numVerts) + 1;
  v12->indexes[v12->numIndexes + 2] = LOWORD(v12->numVerts) + 2;
  v12->indexes[v12->numIndexes + 3] = v12->numVerts;
  v12->indexes[v12->numIndexes + 4] = LOWORD(v12->numVerts) + 2;
  v12->indexes[v12->numIndexes + 5] = LOWORD(v12->numVerts) + 3;
  v12->numVerts += 4;
  v12->numIndexes += 6;
  return 1;
}
// local variable allocation has failed, the output may be wrong!
char __userpurge rvModelParticle::Render@<al>(rvModelParticle *this@<ecx>, int a2@<edi>, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time, float override, float alpha)
{
  double v9; // st7
  rvModelParticle *v10; // esi
  rvEnvParms *v11; // ebx
  double v12; // st3
  float *v13; // eax
  signed int v14; // edx
  signed int v15; // ecx
  int v16; // eax
  int v17; // edi
  int v18; // esi
  idDrawVert *v19; // ebp
  idDrawVert *v20; // esi
  double v21; // st7
  double v22; // st6
  int v23; // ecx
  int i; // eax
  float evalTime_4; // [esp+30h] [ebp-B4h]
  int evalTime_4a; // [esp+30h] [ebp-B4h]
  float oneOverDuration; // [esp+34h] [ebp-B0h]
  float v29; // [esp+38h] [ebp-ACh]
  float v30; // [esp+3Ch] [ebp-A8h]
  float v31; // [esp+40h] [ebp-A4h]
  float v32; // [esp+44h] [ebp-A0h]
  float v33; // [esp+48h] [ebp-9Ch]
  float v34; // [esp+4Ch] [ebp-98h]
  double v35; // [esp+50h] [ebp-94h] OVERLAPPED
  idVec3 position; // [esp+54h] [ebp-90h]
  idVec3 rot; // [esp+60h] [ebp-84h]
  idVec3 size; // [esp+6Ch] [ebp-78h]
  rvAngles rotation; // [esp+78h] [ebp-6Ch]
  idVec4 tint; // [esp+84h] [ebp-60h]
  idMat3 transform; // [esp+94h] [ebp-50h]
  unsigned int t; // [esp+B8h] [ebp-2Ch]
  float v43; // [esp+BCh] [ebp-28h]
  float v44; // [esp+C0h] [ebp-24h]
  float v45; // [esp+C4h] [ebp-20h]
  float v46; // [esp+C8h] [ebp-1Ch]
  float v47; // [esp+CCh] [ebp-18h]
  float v48; // [esp+D0h] [ebp-14h]
  float v49; // [esp+D4h] [ebp-10h]
  float v50; // [esp+D8h] [ebp-Ch]
  float v51; // [esp+DCh] [ebp-8h]
  float v52; // [esp+E0h] [ebp-4h]
  float retaddr; // [esp+E4h] [ebp+0h]

  v9 = time;
  v10 = this;
  evalTime_4 = time - this->mStartTime;
  if ( this->mEndTime - 0.002000000094994903 <= time )
    evalTime_4 = this->mEndTime - this->mStartTime - 0.002000000094994903;
  if ( this->mStartTime - 0.002000000094994903 >= v9 || this->mEndTime <= v9 || !this->mModel )
    return 0;
  v11 = pt->mpFadeEnvelope;
  oneOverDuration = this->mEndTime - this->mStartTime;
  oneOverDuration = 1.0 / oneOverDuration;
  rvEnvParms::Evaluate(pt->mpTintEnvelope, &this->mTintEnv, evalTime_4, oneOverDuration, &tint.y);
  rvEnvParms::Evaluate(v11, &v10->mFadeEnv, evalTime_4, oneOverDuration, (float *)&transform);
  (*(void (__thiscall **)(rvModelParticle *, rvEnvParms *, float, _DWORD, float *, int))&v10->vfptr->gap4[8])(
    v10,
    pt->mpSizeEnvelope,
    COERCE_FLOAT(LODWORD(evalTime_4)),
    LODWORD(oneOverDuration),
    &size.y,
    a2);
  (*(void (__thiscall **)(rvModelParticle *, rvEnvParms *, _DWORD, _DWORD, float *))&v10->vfptr->gap4[12])(
    v10,
    pt->mpRotateEnvelope,
    LODWORD(oneOverDuration),
    LODWORD(v29),
    &rot.z);
  oneOverDuration = override - v10->mMotionStartTime;
  rvParticle::EvaluatePosition(
    (rvParticle *)&v10->vfptr,
    (rvBSE *)pt,
    pt,
    (idVec3 *)((char *)&position + 8),
    oneOverDuration);
  v44 = COERCE_FLOAT(rvParticle::HandleTint((rvParticle *)&v10->vfptr, (rvBSE *)pt, (idVec4 *)((char *)&tint + 8), alpha));
  rotation.roll = rot.z;
  tint.y = size.y;
  v33 = cos(size.x);
  v31 = sin(size.x);
  v32 = cos(rot.z);
  v30 = sin(rot.z);
  position.y = size.y;
  v34 = COERCE_FLOAT(&oneOverDuration);
  oneOverDuration = cos(size.y);
  v29 = sin(size.y);
  transform.mat[0].z = v32 * v33;
  transform.mat[1].x = v32 * v31;
  transform.mat[1].y = -v30;
  v12 = v30 * v29;
  transform.mat[1].z = v12 * v33 - oneOverDuration * v31;
  transform.mat[2].x = oneOverDuration * v33 + v31 * v12;
  transform.mat[2].y = v29 * v32;
  v35 = oneOverDuration * v30;
  transform.mat[2].z = v35 * v33 - -v31 * v29;
  *(float *)&t = v31 * v35 - v33 * v29;
  v43 = v32 * oneOverDuration;
  if ( !(v10->mFlags & 2) )
  {
    v45 = v10->mInitAxis.mat[0].y * *(float *)&pt[1].mEntityDefName.baseBuffer[12]
        + *(float *)&pt[1].mEntityDefName.baseBuffer[8] * v10->mInitAxis.mat[0].x
        + v10->mInitAxis.mat[0].z * *(float *)&pt[1].mEntityDefName.baseBuffer[16];
    v46 = pt[1].mGravity.x * v10->mInitAxis.mat[0].x
        + v10->mInitAxis.mat[0].y * pt[1].mGravity.y
        + pt[1].mDuration.x * v10->mInitAxis.mat[0].z;
    v47 = pt[1].mCentre.x * v10->mInitAxis.mat[0].y
        + pt[1].mDuration.y * v10->mInitAxis.mat[0].x
        + v10->mInitAxis.mat[0].z * pt[1].mCentre.y;
    v48 = *(float *)&pt[1].mEntityDefName.baseBuffer[8] * v10->mInitAxis.mat[1].x
        + v10->mInitAxis.mat[1].y * *(float *)&pt[1].mEntityDefName.baseBuffer[12]
        + v10->mInitAxis.mat[1].z * *(float *)&pt[1].mEntityDefName.baseBuffer[16];
    v49 = v10->mInitAxis.mat[1].x * pt[1].mGravity.x
        + v10->mInitAxis.mat[1].y * pt[1].mGravity.y
        + v10->mInitAxis.mat[1].z * pt[1].mDuration.x;
    v50 = pt[1].mCentre.x * v10->mInitAxis.mat[1].y
        + pt[1].mDuration.y * v10->mInitAxis.mat[1].x
        + v10->mInitAxis.mat[1].z * pt[1].mCentre.y;
    v51 = v10->mInitAxis.mat[2].y * *(float *)&pt[1].mEntityDefName.baseBuffer[12]
        + v10->mInitAxis.mat[2].x * *(float *)&pt[1].mEntityDefName.baseBuffer[8]
        + v10->mInitAxis.mat[2].z * *(float *)&pt[1].mEntityDefName.baseBuffer[16];
    v52 = v10->mInitAxis.mat[2].y * pt[1].mGravity.y
        + v10->mInitAxis.mat[2].x * pt[1].mGravity.x
        + pt[1].mDuration.x * v10->mInitAxis.mat[2].z;
    retaddr = v10->mInitAxis.mat[2].y * pt[1].mCentre.x
            + v10->mInitAxis.mat[2].x * pt[1].mDuration.y
            + v10->mInitAxis.mat[2].z * pt[1].mCentre.y;
    v13 = &transform.mat[1].y;
    v14 = 3;
    do
    {
      v15 = 0;
      do
      {
        v15 += 4;
        *(float *)((char *)&rotation.yaw + v15) = *(v13 - 2) * *(float *)((char *)&v44 + v15)
                                                + *(float *)((char *)&v47 + v15) * *(v13 - 1)
                                                + *(float *)((char *)&v50 + v15) * *v13;
      }
      while ( v15 < 12 );
      v13 += 3;
      --v14;
      *(v13 - 5) = rotation.roll;
      *(v13 - 4) = tint.x;
      *(v13 - 3) = tint.y;
    }
    while ( v14 );
  }
  v16 = (int)v10->mModel->vfptr->Surface(v10->mModel, 0);
  v17 = v16;
  if ( v16 )
  {
    v18 = tri->numVerts;
    v19 = *(idDrawVert **)(*(_DWORD *)(v16 + 8) + 52);
    tri->texCoordScale = 1.0;
    v20 = &tri->verts[v18];
    evalTime_4a = 0;
    if ( *(_DWORD *)(*(_DWORD *)(v16 + 8) + 48) > 0 )
    {
      do
      {
        idDrawVert::operator=(v20, v19);
        v21 = transform.mat[1].y * v20->xyz.y;
        v22 = v20->xyz.x;
        *(float *)v20->color = v43;
        ++v19;
        ++v20;
        ++evalTime_4a;
        v34 = v21 + v22 * transform.mat[0].y + transform.mat[2].y * v20[-1].xyz.z;
        v33 = transform.mat[1].z * v20[-1].xyz.y
            + v20[-1].xyz.x * transform.mat[0].z
            + transform.mat[2].z * v20[-1].xyz.z;
        v20[-1].xyz.z = transform.mat[2].x * v20[-1].xyz.y
                      + v20[-1].xyz.x * transform.mat[1].x
                      + *(float *)&t * v20[-1].xyz.z;
        v20[-1].xyz.x = v34;
        v20[-1].xyz.y = v33;
        v20[-1].xyz.x = v20[-1].xyz.x * size.y;
        v20[-1].xyz.y = size.z * v20[-1].xyz.y;
        v20[-1].xyz.z = rotation.pitch * v20[-1].xyz.z;
        v20[-1].xyz.x = v20[-1].xyz.x + position.y;
        v20[-1].xyz.y = position.z + v20[-1].xyz.y;
        v20[-1].xyz.z = rot.x + v20[-1].xyz.z;
      }
      while ( evalTime_4a < *(_DWORD *)(*(_DWORD *)(v17 + 8) + 48) );
    }
    v23 = *(_DWORD *)(v17 + 8);
    for ( i = 0; i < *(_DWORD *)(v23 + 56); ++i )
    {
      tri->indexes[i + tri->numIndexes] = LOWORD(tri->numVerts) + *(_WORD *)(*(_DWORD *)(v23 + 60) + 2 * i);
      v23 = *(_DWORD *)(v17 + 8);
    }
    tri->numVerts += *(_DWORD *)(*(_DWORD *)(v17 + 8) + 48);
    tri->numIndexes += *(_DWORD *)(*(_DWORD *)(v17 + 8) + 56);
  }
  return 1;
}
char __thiscall rvLinkedParticle::Render(rvLinkedParticle *this, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time, float override)
{
  double v7; // st7
  rvLinkedParticle *v8; // edi
  float v9; // ST28_4
  rvEnvParms *v10; // ebx
  float oneOverDuration; // ST28_4
  unsigned int v12; // ecx
  float v13; // ST38_4
  float v14; // ST2C_4
  float left; // ST34_4
  float v16; // ST24_4
  float v17; // ST30_4
  float v18; // ST28_4
  float v19; // ST38_4
  float v20; // ST3C_4
  double v21; // st3
  double v22; // st5
  float v23; // ST38_4
  float v24; // ST3C_4
  bool v25; // zf
  idDrawVert *v26; // esi
  double v27; // st3
  __int16 v28; // bp
  double v29; // st2
  bool v30; // c0
  char v31; // al
  double v33; // st1
  double v34; // st2
  signed int v35; // eax
  double v36; // st2
  bool v37; // c0
  bool v38; // c3
  char v39; // al
  int v40; // esi
  bool v41; // c0
  char v42; // al
  char v43; // al
  float ooduration; // [esp+24h] [ebp-68h]
  float left_4; // [esp+38h] [ebp-54h]
  float left_8; // [esp+3Ch] [ebp-50h]
  idVec2 size; // [esp+40h] [ebp-4Ch]
  idVec3 position; // [esp+48h] [ebp-44h]
  idVec3 dir; // [esp+54h] [ebp-38h]
  idVec3 normal; // [esp+60h] [ebp-2Ch]
  idVec3 tangent; // [esp+6Ch] [ebp-20h]
  idVec4 tint; // [esp+78h] [ebp-14h]
  float dest; // [esp+88h] [ebp-4h]
  int t; // [esp+90h] [ebp+4h]
  float ptb; // [esp+94h] [ebp+8h]
  rvParticleTemplate *ptc; // [esp+94h] [ebp+8h]
  rvParticleTemplate *ptd; // [esp+94h] [ebp+8h]
  rvParticleTemplate *pte; // [esp+94h] [ebp+8h]
  __int16 pta; // [esp+94h] [ebp+8h]
  float viewa; // [esp+98h] [ebp+Ch]
  float timeb; // [esp+A0h] [ebp+14h]
  float timec; // [esp+A0h] [ebp+14h]
  float timed; // [esp+A0h] [ebp+14h]
  float timee; // [esp+A0h] [ebp+14h]
  float timef; // [esp+A0h] [ebp+14h]
  float timeg; // [esp+A0h] [ebp+14h]
  float timeh; // [esp+A0h] [ebp+14h]
  float timei; // [esp+A0h] [ebp+14h]
  float timej; // [esp+A0h] [ebp+14h]
  float timek; // [esp+A0h] [ebp+14h]
  float timel; // [esp+A0h] [ebp+14h]
  float timem; // [esp+A0h] [ebp+14h]
  __int16 timea; // [esp+A0h] [ebp+14h]
  __int16 overridea; // [esp+A4h] [ebp+18h]

  v7 = time;
  v8 = this;
  ooduration = time - this->mStartTime;
  if ( this->mEndTime - 0.002000000094994903 <= time )
    ooduration = this->mEndTime - this->mStartTime - 0.002000000094994903;
  if ( this->mStartTime - 0.002000000094994903 >= v7 || this->mEndTime <= v7 )
    return 0;
  v9 = this->mEndTime - this->mStartTime;
  v10 = pt->mpFadeEnvelope;
  oneOverDuration = 1.0 / v9;
  rvEnvParms::Evaluate(pt->mpTintEnvelope, &this->mTintEnv, ooduration, oneOverDuration, &tint.y);
  rvEnvParms::Evaluate(v10, &v8->mFadeEnv, ooduration, oneOverDuration, &dest);
  (*(void (__thiscall **)(rvLinkedParticle *, rvEnvParms *, float, _DWORD, float *))&v8->vfptr->gap4[8])(
    v8,
    pt->mpSizeEnvelope,
    COERCE_FLOAT(LODWORD(ooduration)),
    LODWORD(oneOverDuration),
    &size.y);
  timeb = time - v8->mMotionStartTime;
  rvParticle::EvaluatePosition((rvParticle *)&v8->vfptr, effect, pt, (idVec3 *)((char *)&position + 4), timeb);
  v12 = v8->mFlags;
  if ( ((unsigned int)v8->mFlags >> 22) & 1 )
  {
    timec = effect->mLightningAxis.mat[2].x * dir.x
          + effect->mLightningAxis.mat[0].x * position.y
          + effect->mLightningAxis.mat[1].x * position.z;
    ptb = effect->mLightningAxis.mat[1].y * position.z
        + effect->mLightningAxis.mat[0].y * position.y
        + effect->mLightningAxis.mat[2].y * dir.x;
    dir.x = position.y * effect->mLightningAxis.mat[0].z
          + position.z * effect->mLightningAxis.mat[1].z
          + dir.x * effect->mLightningAxis.mat[2].z;
    position.y = timec;
    position.z = ptb;
  }
  v13 = effect->mCurrentAxis.mat[2].z;
  v14 = effect->mCurrentAxis.mat[1].z;
  left = effect->mCurrentAxis.mat[2].y;
  v16 = effect->mCurrentAxis.mat[1].y;
  v17 = effect->mCurrentAxis.mat[2].x;
  v18 = effect->mCurrentAxis.mat[1].x;
  dir.y = v8->mInitAxis.mat[0].x * effect->mCurrentAxis.mat[0].x
        + v8->mInitAxis.mat[0].y * effect->mCurrentAxis.mat[0].y
        + v8->mInitAxis.mat[0].z * effect->mCurrentAxis.mat[0].z;
  dir.z = v8->mInitAxis.mat[0].x * v18 + v8->mInitAxis.mat[0].y * v16 + v8->mInitAxis.mat[0].z * v14;
  normal.x = v8->mInitAxis.mat[0].x * v17 + v8->mInitAxis.mat[0].y * left + v8->mInitAxis.mat[0].z * v13;
  timed = normal.x * normal.x + dir.y * dir.y + dir.z * dir.z;
  ptc = (rvParticleTemplate *)(1597463007 - (SLODWORD(timed) >> 1));
  timee = timed * 0.5;
  timef = *(float *)&ptc * (1.5 - *(float *)&ptc * *(float *)&ptc * timee);
  dir.y = dir.y * timef;
  dir.z = dir.z * timef;
  normal.x = timef * normal.x;
  v19 = view->mat[0].x - position.y;
  v20 = view->mat[0].y - position.z;
  size.x = view->mat[0].z - dir.x;
  v21 = dir.z;
  normal.y = size.x * dir.z - v20 * normal.x;
  v22 = dir.y;
  normal.z = normal.x * v19 - size.x * dir.y;
  tangent.x = v20 * dir.y - dir.z * v19;
  timeg = normal.y * normal.y + normal.z * normal.z + tangent.x * tangent.x;
  ptd = (rvParticleTemplate *)(1597463007 - (SLODWORD(timeg) >> 1));
  timeh = timeg * 0.5;
  timei = (1.5 - *(float *)&ptd * *(float *)&ptd * timeh) * *(float *)&ptd;
  v23 = normal.y * timei;
  v24 = normal.z * timei;
  size.x = timei * tangent.x;
  normal.y = -v23;
  normal.z = -v24;
  tangent.x = -size.x;
  dir.y = tangent.x * dir.z - normal.z * normal.x;
  dir.z = normal.x * normal.y - tangent.x * v22;
  normal.x = v22 * normal.z - normal.y * v21;
  timej = dir.y * dir.y + dir.z * dir.z + normal.x * normal.x;
  pte = (rvParticleTemplate *)(1597463007 - (SLODWORD(timej) >> 1));
  timek = 0.5 * timej;
  timel = *(float *)&pte * (1.5 - *(float *)&pte * *(float *)&pte * timek);
  normal.y = dir.y * timel;
  v25 = v8->mNext == 0;
  normal.z = dir.z * timel;
  tangent.y = v23;
  tangent.x = timel * normal.x;
  tangent.z = v24;
  tint.x = size.x;
  left_4 = size.y * v23;
  left_8 = size.y * v24;
  size.x = size.y * size.x;
  if ( v25 && (v12 >> 23) & 1 )
    override = 0.0;
  v26 = &tri->verts[tri->numVerts];
  t = rvParticle::HandleTint((rvParticle *)&v8->vfptr, effect, (idVec4 *)((char *)&tint + 4), override);
  dir.y = position.y - left_4;
  dir.z = position.z - left_8;
  normal.x = dir.x - size.x;
  v26->xyz.x = dir.y;
  v26->xyz.y = dir.z;
  v26->xyz.z = normal.x;
  timem = v8->mTextureScale * v8->mFraction * 0.009999999776482582;
  v26->_st[0] = (signed int)(timem * 4096.0);
  v27 = normal.y;
  v26->_st[1] = (signed int)(0.0 * 4096.0);
  v28 = (signed int)(v27 * 32767.0);
  v29 = normal.z * 32767.0;
  v26->_normal[0] = v28;
  v30 = 0.0 < tangent.x;
  timea = (signed int)v29;
  v26->_normal[1] = timea;
  if ( v30 )
    v31 = 2;
  else
    v31 = 0;
  v33 = tangent.y;
  v26->_signs[0] = v31;
  overridea = (signed int)(v33 * 32767.0);
  v34 = 32767.0 * tangent.z;
  v26->_tangent[0] = overridea;
  v35 = (signed int)v34;
  v36 = tint.x;
  v37 = tint.x < 0.0;
  v38 = tint.x == 0.0;
  pta = v35;
  v26->_tangent[1] = v35;
  if ( v37 || v38 )
    v39 = 0;
  else
    v39 = 2;
  v26->_signs[1] = v39;
  *(_DWORD *)v26->color = t;
  v40 = (int)&v26[1];
  tangent.y = left_4 + position.y;
  tangent.z = left_8 + position.z;
  tint.x = size.x + dir.x;
  *(float *)v40 = tangent.y;
  *(float *)(v40 + 4) = tangent.z;
  *(float *)(v40 + 8) = tint.x;
  viewa = v8->mTextureScale * v8->mFraction * 0.009999999776482582;
  v41 = 0.0 < tangent.x;
  *(_WORD *)(v40 + 28) = (signed int)(4096.0 * viewa);
  *(_WORD *)(v40 + 18) = timea;
  *(_WORD *)(v40 + 30) = 40;
  *(_WORD *)(v40 + 16) = v28;
  if ( v41 )
    v42 = 2;
  else
    v42 = 0;
  *(_BYTE *)(v40 + 24) = v42;
  *(_WORD *)(v40 + 20) = overridea;
  *(_WORD *)(v40 + 22) = pta;
  if ( v36 <= 0.0 )
    v43 = 0;
  else
    v43 = 2;
  *(_BYTE *)(v40 + 25) = v43;
  *(_DWORD *)(v40 + 12) = t;
  if ( tri->numVerts > 0 )
  {
    tri->indexes[tri->numIndexes] = LOWORD(tri->numVerts) - 2;
    tri->indexes[tri->numIndexes + 1] = LOWORD(tri->numVerts) - 1;
    tri->indexes[tri->numIndexes + 2] = tri->numVerts;
    tri->indexes[tri->numIndexes + 3] = LOWORD(tri->numVerts) - 1;
    tri->indexes[tri->numIndexes + 4] = LOWORD(tri->numVerts) + 1;
    tri->indexes[tri->numIndexes + 5] = tri->numVerts;
    tri->numIndexes += 6;
  }
  tri->numVerts += 2;
  return 1;
}
void __stdcall rvParticle::Attenuate(float atten, rvParticleParms *parms, rvEnvParms2Particle *result)
{
  char v3; // al

  v3 = parms->mFlags;
  if ( v3 & 0x60 )
  {
    if ( v3 & 0x40 )
    {
      atten = 1.0 - atten;
      result->mStart.x = result->mStart.x * atten;
      result->mStart.y = atten * result->mStart.y;
      result->mEnd.x = atten * result->mEnd.x;
    }
    else
    {
      result->mStart.x = result->mStart.x * atten;
      result->mStart.y = atten * result->mStart.y;
      result->mEnd.x = result->mEnd.x * atten;
    }
    result->mEnd.y = atten * result->mEnd.y;
  }
}
void __stdcall rvParticle::Attenuate(float atten, rvParticleParms *parms, rvEnvParms3Particle *result)
{
  char v3; // al

  v3 = parms->mFlags;
  if ( v3 & 0x60 )
  {
    if ( v3 & 0x40 )
    {
      atten = 1.0 - atten;
      result->mStart.x = result->mStart.x * atten;
      result->mStart.y = atten * result->mStart.y;
      result->mStart.z = result->mStart.z * atten;
      result->mEnd.x = atten * result->mEnd.x;
    }
    else
    {
      result->mStart.x = result->mStart.x * atten;
      result->mStart.y = atten * result->mStart.y;
      result->mStart.z = result->mStart.z * atten;
      result->mEnd.x = result->mEnd.x * atten;
    }
    result->mEnd.y = result->mEnd.y * atten;
    result->mEnd.z = atten * result->mEnd.z;
  }
}
char __thiscall sdOrientedLinkedParticle::Render(sdOrientedLinkedParticle *this, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time, float override)
{
  double v7; // st7
  sdOrientedLinkedParticle *v8; // ebp
  float v9; // ST30_4
  rvEnvParms *v10; // edi
  float oneOverDuration; // ST44_4
  float v12; // ST30_4
  unsigned int v13; // eax
  float v14; // ST30_4
  float v15; // ST24_4
  double v16; // st7
  float v17; // ST48_4
  float v18; // ST4C_4
  float v19; // ST50_4
  float v20; // ST30_4
  float v21; // ST24_4
  float v22; // ST50_4
  idDrawVert *v23; // esi
  float v24; // ST38_4
  float v25; // ST28_4
  float v26; // ST3C_4
  float v27; // ST58_4
  float v28; // ST34_4
  float v29; // ST2C_4
  float v30; // ST68_4
  float v31; // ST6C_4
  float v32; // ST24_4
  double v33; // st7
  double v34; // st6
  double v35; // st7
  signed int v36; // eax
  double v37; // st6
  signed int v38; // eax
  double v39; // st6
  bool v40; // c0
  double v41; // st5
  __int16 v42; // di
  double v43; // st5
  bool v44; // c0
  __int16 v45; // cx
  double v46; // st5
  float v47; // ST24_4
  float v49; // ST24_4
  double v50; // rtt
  double v51; // st5
  double v52; // st7
  float v53; // ST5C_4
  float v54; // ST60_4
  float v55; // ST64_4
  char v56; // al
  int v57; // esi
  float v58; // ST5C_4
  float v59; // ST60_4
  float v60; // ST64_4
  float v61; // ST28_4
  bool v62; // c0
  bool v63; // c0
  double v64; // st7
  double v65; // st6
  double v66; // rtt
  double v67; // st7
  double v68; // rt0
  double v69; // st6
  char v70; // al
  float v71; // [esp+24h] [ebp-9Ch]
  float v72; // [esp+24h] [ebp-9Ch]
  float v73; // [esp+24h] [ebp-9Ch]
  float v74; // [esp+28h] [ebp-98h]
  float v75; // [esp+28h] [ebp-98h]
  float v76; // [esp+28h] [ebp-98h]
  float v77; // [esp+28h] [ebp-98h]
  float t; // [esp+2Ch] [ebp-94h]
  float ta; // [esp+2Ch] [ebp-94h]
  float tb; // [esp+2Ch] [ebp-94h]
  int v81; // [esp+30h] [ebp-90h]
  unsigned __int8 evalTime; // [esp+34h] [ebp-8Ch]
  unsigned __int8 evalTimea; // [esp+34h] [ebp-8Ch]
  float v84; // [esp+38h] [ebp-88h]
  unsigned __int8 v85; // [esp+3Ch] [ebp-84h]
  unsigned __int8 v86; // [esp+3Ch] [ebp-84h]
  __int16 ooduration_2; // [esp+42h] [ebp-7Eh]
  __int16 left; // [esp+44h] [ebp-7Ch]
  float left_4; // [esp+48h] [ebp-78h]
  float left_8; // [esp+4Ch] [ebp-74h]
  float v91; // [esp+50h] [ebp-70h]
  float v92; // [esp+54h] [ebp-6Ch]
  float v93; // [esp+58h] [ebp-68h]
  float v94; // [esp+60h] [ebp-60h]
  float v95; // [esp+64h] [ebp-5Ch]
  idVec3 position; // [esp+70h] [ebp-50h]
  idVec3 tangent; // [esp+7Ch] [ebp-44h]
  idVec3 bitangent; // [esp+88h] [ebp-38h]
  idVec3 normal; // [esp+94h] [ebp-2Ch]
  idVec2 size; // [esp+A0h] [ebp-20h]
  float v101; // [esp+A8h] [ebp-18h]
  idVec4 tint; // [esp+ACh] [ebp-14h]
  float dest; // [esp+BCh] [ebp-4h]

  v7 = time;
  v8 = this;
  v84 = time - this->mStartTime;
  if ( this->mEndTime - 0.002000000094994903 <= time )
    v84 = this->mEndTime - this->mStartTime - 0.002000000094994903;
  if ( this->mStartTime - 0.002000000094994903 >= v7 || this->mEndTime <= v7 )
    return 0;
  v9 = this->mEndTime - this->mStartTime;
  v10 = pt->mpFadeEnvelope;
  oneOverDuration = 1.0 / v9;
  rvEnvParms::Evaluate(pt->mpTintEnvelope, &this->mTintEnv, v84, oneOverDuration, &tint.y);
  rvEnvParms::Evaluate(v10, &v8->mFadeEnv, v84, oneOverDuration, &dest);
  (*(void (__thiscall **)(sdOrientedLinkedParticle *, rvEnvParms *, float, _DWORD, float *))&v8->vfptr->gap4[8])(
    v8,
    pt->mpSizeEnvelope,
    COERCE_FLOAT(LODWORD(v84)),
    LODWORD(oneOverDuration),
    &size.y);
  v12 = time - v8->mMotionStartTime;
  rvParticle::EvaluatePosition((rvParticle *)&v8->vfptr, effect, pt, (idVec3 *)((char *)&position + 4), v12);
  v13 = v8->mFlags;
  if ( ((unsigned int)v8->mFlags >> 22) & 1 )
  {
    v14 = effect->mLightningAxis.mat[2].x * tangent.x
        + effect->mLightningAxis.mat[0].x * position.y
        + effect->mLightningAxis.mat[1].x * position.z;
    v15 = effect->mLightningAxis.mat[1].y * position.z
        + effect->mLightningAxis.mat[0].y * position.y
        + effect->mLightningAxis.mat[2].y * tangent.x;
    tangent.x = position.y * effect->mLightningAxis.mat[0].z
              + position.z * effect->mLightningAxis.mat[1].z
              + tangent.x * effect->mLightningAxis.mat[2].z;
    position.y = v14;
    position.z = v15;
  }
  if ( !v8->mNext && (v13 >> 23) & 1 )
    override = 0.0;
  v16 = v8->mInitAxis.mat[0].z * 0.0;
  v17 = v8->mInitAxis.mat[0].y - v16;
  v18 = v16 - v8->mInitAxis.mat[0].x;
  v19 = v8->mInitAxis.mat[0].x * 0.0 - v8->mInitAxis.mat[0].y * 0.0;
  v20 = effect->mCurrentAxis.mat[0].x * v17 + v18 * effect->mCurrentAxis.mat[0].y + v19 * effect->mCurrentAxis.mat[0].z;
  v21 = v18 * effect->mCurrentAxis.mat[1].y + effect->mCurrentAxis.mat[1].x * v17 + v19 * effect->mCurrentAxis.mat[1].z;
  v22 = v19 * effect->mCurrentAxis.mat[2].z + v17 * effect->mCurrentAxis.mat[2].x + v18 * effect->mCurrentAxis.mat[2].y;
  normal.x = v22;
  bitangent.y = v20;
  bitangent.z = v21;
  left_4 = v20 * size.y;
  v23 = &tri->verts[tri->numVerts];
  left_8 = v21 * size.y;
  v91 = size.y * v22;
  v24 = effect->mCurrentAxis.mat[2].z;
  v25 = effect->mCurrentAxis.mat[1].z;
  v26 = effect->mCurrentAxis.mat[2].y;
  v27 = effect->mCurrentAxis.mat[1].y;
  v28 = effect->mCurrentAxis.mat[2].x;
  v29 = effect->mCurrentAxis.mat[1].x;
  normal.y = effect->mCurrentAxis.mat[0].x * 0.0 + effect->mCurrentAxis.mat[0].y * 0.0 + effect->mCurrentAxis.mat[0].z;
  normal.z = v29 * 0.0 + v27 * 0.0 + v25;
  size.x = 0.0 * v26 + v28 * 0.0 + v24;
  tangent.y = v21 * size.x - normal.x * normal.z;
  tangent.z = normal.x * normal.y - size.x * v20;
  bitangent.x = normal.z * v20 - v21 * normal.y;
  v81 = rvParticle::HandleTint((rvParticle *)&v8->vfptr, effect, (idVec4 *)((char *)&tint + 4), override);
  v30 = position.y - left_4;
  v31 = position.z - left_8;
  position.x = tangent.x - v91;
  v23->xyz.x = v30;
  v23->xyz.y = v31;
  v23->xyz.z = position.x;
  v32 = v8->mTextureScale * v8->mFraction * 0.009999999776482582;
  v23->_st[0] = (signed int)(v32 * 4096.0);
  v33 = normal.y;
  v23->_st[1] = (signed int)(4096.0 * 0.0);
  v34 = v33 * 32767.0;
  v35 = 32767.0;
  v36 = (signed int)v34;
  v37 = normal.z * 32767.0;
  left = v36;
  v23->_normal[0] = v36;
  v38 = (signed int)v37;
  v39 = 0.0;
  v40 = 0.0 < size.x;
  ooduration_2 = v38;
  v23->_normal[1] = v38;
  evalTime = 2;
  if ( !v40 )
    evalTime = 0;
  v41 = tangent.y * 32767.0;
  v23->_signs[0] = evalTime;
  v42 = (signed int)v41;
  v43 = tangent.z * 32767.0;
  v23->_tangent[0] = v42;
  v44 = 0.0 < bitangent.x;
  v45 = (signed int)v43;
  v23->_tangent[1] = v45;
  v85 = 2;
  if ( !v44 )
    v85 = 0;
  v23->_signs[1] = v85;
  v46 = 1.0;
  v71 = 1.0 - (double)(v42 * (signed int)v42 + v45 * (signed int)v45) / 1073676288.0;
  if ( v71 <= 0.0 )
  {
    v74 = 0.0;
  }
  else
  {
    v47 = sqrt(v71);
    v74 = v47;
    v35 = 32767.0;
    v39 = 0.0;
    v46 = 1.0;
  }
  v93 = (double)v42 / v35;
  size.y = v93;
  t = (double)v45 / v35;
  v101 = t;
  tint.x = ((double)v85 - v46) * v74;
  v72 = v46 - (double)(left * (signed int)left + (signed __int16)v38 * (signed int)(signed __int16)v38) / 1073676288.0;
  if ( v72 <= v39 )
  {
    v75 = v39;
  }
  else
  {
    v49 = sqrt(v72);
    v75 = v49;
    v35 = 32767.0;
    v39 = 0.0;
    v46 = 1.0;
  }
  v92 = (double)left / v35;
  v50 = v46;
  v51 = (double)(signed __int16)v38 / v35;
  v52 = v50;
  v73 = v51;
  position.x = ((double)evalTime - v50) * v75;
  v53 = v73 * tint.x - position.x * v101;
  v54 = position.x * size.y - tint.x * v92;
  v55 = v92 * v101 - v73 * size.y;
  if ( bitangent.z * v54 + v53 * bitangent.y + normal.x * v55 <= 0.0 )
    v56 = 0;
  else
    v56 = 2;
  v23->_signs[2] = v56;
  *(_DWORD *)v23->color = v81;
  v57 = (int)&v23[1];
  v58 = left_4 + position.y;
  v59 = left_8 + position.z;
  v60 = v91 + tangent.x;
  *(float *)v57 = v58;
  *(float *)(v57 + 4) = v59;
  *(float *)(v57 + 8) = v60;
  v61 = v8->mTextureScale * v8->mFraction * 0.009999999776482582;
  v62 = v39 < size.x;
  *(_WORD *)(v57 + 28) = (signed int)(v61 * 4096.0);
  *(_WORD *)(v57 + 18) = ooduration_2;
  *(_WORD *)(v57 + 30) = 40;
  *(_WORD *)(v57 + 16) = left;
  v86 = 2;
  if ( !v62 )
    v86 = 0;
  v63 = v39 < bitangent.x;
  *(_BYTE *)(v57 + 24) = v86;
  *(_WORD *)(v57 + 20) = v42;
  *(_WORD *)(v57 + 22) = v45;
  evalTimea = 2;
  if ( !v63 )
    evalTimea = 0;
  *(_BYTE *)(v57 + 25) = evalTimea;
  v76 = v52 - (double)(v42 * (signed int)v42 + v45 * (signed int)v45) / 1073676288.0;
  if ( v76 <= v39 )
  {
    v77 = v39;
    v66 = v39;
    v65 = v52;
    v64 = v66;
  }
  else
  {
    v77 = sqrt(v76);
    v64 = 0.0;
    v65 = 1.0;
  }
  v94 = t;
  v95 = ((double)evalTimea - v65) * v77;
  ta = v65 - (double)(left * (signed int)left + ooduration_2 * (signed int)ooduration_2) / 1073676288.0;
  if ( ta <= v64 )
  {
    v68 = v65;
    v69 = v64;
    v67 = v68;
    tb = v69;
  }
  else
  {
    tb = sqrt(ta);
    v67 = 1.0;
  }
  position.x = ((double)v86 - v67) * tb;
  tangent.y = v73 * v95 - position.x * v94;
  tangent.z = position.x * v93 - v95 * v92;
  bitangent.x = v94 * v92 - v73 * v93;
  if ( bitangent.z * tangent.z + tangent.y * bitangent.y + normal.x * bitangent.x <= 0.0 )
    v70 = 0;
  else
    v70 = 2;
  *(_BYTE *)(v57 + 26) = v70;
  *(_DWORD *)(v57 + 12) = v81;
  if ( tri->numVerts > 0 )
  {
    tri->indexes[tri->numIndexes] = LOWORD(tri->numVerts) - 2;
    tri->indexes[tri->numIndexes + 1] = LOWORD(tri->numVerts) - 1;
    tri->indexes[tri->numIndexes + 2] = tri->numVerts;
    tri->indexes[tri->numIndexes + 3] = LOWORD(tri->numVerts) - 1;
    tri->indexes[tri->numIndexes + 4] = LOWORD(tri->numVerts) + 1;
    tri->indexes[tri->numIndexes + 5] = tri->numVerts;
    tri->numIndexes += 6;
  }
  tri->numVerts += 2;
  return 1;
}
int dynamic_initializer_for__singletonLock___116()
{
  sdLock::sdLock(&singletonLock_116);
  return atexit(dynamic_atexit_destructor_for__singletonLock___116);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___116()
{
  sdLock::~sdLock(&singletonLock_116);
}
