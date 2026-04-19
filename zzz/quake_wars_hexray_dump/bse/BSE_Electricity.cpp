int __thiscall rvElectricityParticle::Update(rvElectricityParticle *this, rvParticleTemplate *pt, float time)
{
  rvElectricityParticle *v3; // esi
  int result; // eax
  idVec3 length; // [esp+1Ch] [ebp-10h]
  float v6; // [esp+28h] [ebp-4h]
  float timea; // [esp+34h] [ebp+8h]
  float timeb; // [esp+34h] [ebp+8h]
  float timec; // [esp+34h] [ebp+8h]
  float timed; // [esp+34h] [ebp+8h]
  float timee; // [esp+34h] [ebp+8h]

  v3 = this;
  length.x = this->mEndTime - this->mStartTime;
  length.x = 1.0 / length.x;
  timea = time - this->mStartTime;
  ((void (__thiscall *)(rvElectricityParticle *, rvEnvParms *, _DWORD, _DWORD, float *))this->vfptr->EvaluateLength)(
    this,
    pt->mpLengthEnvelope,
    LODWORD(timea),
    LODWORD(length.x),
    &length.y);
  timeb = length.y * length.y + length.z * length.z + v6 * v6;
  timec = sqrt(timeb);
  timed = timec * 0.0625;
  timee = _ceil(timed);
  result = (signed int)timee;
  if ( result >= 3 )
  {
    if ( result > 50 )
      result = 50;
    v3->mNumBolts = result;
  }
  else
  {
    result = 3;
    v3->mNumBolts = 3;
  }
  return result;
}
void __thiscall rvElectricityParticle::SetupElectricity(rvElectricityParticle *this, rvParticleTemplate *pt)
{
  rvElectricityParticle *v2; // esi
  rvElectricityInfo *v3; // eax

  v2 = this;
  this->mNumForks = pt->mElecInfo->mNumForks;
  this->mSeed = rvRandom::Init();
  v3 = pt->mElecInfo;
  v2->mForkSizeMins.x = v3->mForkSizeMins.x;
  v2->mForkSizeMins.y = v3->mForkSizeMins.y;
  v2->mForkSizeMins.z = v3->mForkSizeMins.z;
  v2->mForkSizeMaxs.x = v3->mForkSizeMaxs.x;
  v2->mForkSizeMaxs.y = v3->mForkSizeMaxs.y;
  v2->mForkSizeMaxs.z = v3->mForkSizeMaxs.z;
  v2->mJitterSize.x = v3->mJitterSize.x;
  v2->mJitterSize.y = v3->mJitterSize.y;
  v2->mJitterSize.z = v3->mJitterSize.z;
  v2->mLastJitter = 0.0;
  v2->mJitterRate = pt->mElecInfo->mJitterRate;
  v2->mJitterTable = pt->mElecInfo->mJitterTable;
}
void __thiscall rvElectricityParticle::RenderLineSegment(rvElectricityParticle *this, rvBSE *effect, SElecWork *work, idVec3 start, float startFraction)
{
  SElecWork *v5; // esi
  idDrawVert *v6; // edi
  int v7; // ebx
  float v8; // ST18_4
  float v9; // ST1C_4
  float v10; // ST20_4
  float v11; // ST24_4
  float v12; // ST28_4
  float v13; // ST2C_4
  int v14; // edi
  float offset_4; // [esp+10h] [ebp-18h]
  float offset_8; // [esp+14h] [ebp-14h]
  float v17; // [esp+18h] [ebp-10h]
  float v18; // [esp+1Ch] [ebp-Ch]
  float v19; // [esp+20h] [ebp-8h]
  float v20; // [esp+24h] [ebp-4h]
  float workb; // [esp+30h] [ebp+8h]
  float worka; // [esp+30h] [ebp+8h]
  float workc; // [esp+30h] [ebp+8h]
  float workd; // [esp+30h] [ebp+8h]
  float worke; // [esp+30h] [ebp+8h]
  float startFractiona; // [esp+40h] [ebp+18h]

  v5 = work;
  v6 = &work->tri->verts[work->tri->numVerts];
  v7 = rvParticle::HandleTint((rvParticle *)&this->vfptr, effect, &work->tint, work->alpha);
  v18 = v5->viewPos.z * v5->length.y - v5->viewPos.y * v5->length.z;
  v19 = v5->viewPos.x * v5->length.z - v5->length.x * v5->viewPos.z;
  v20 = v5->length.x * v5->viewPos.y - v5->viewPos.x * v5->length.y;
  offset_4 = v18;
  offset_8 = v19;
  v17 = v20;
  workb = v20 * v20 + v19 * v19 + v18 * v18;
  worka = sqrt(workb);
  if ( worka >= 0.00000011920929 )
  {
    workc = 1.0 / worka;
    offset_4 = workc * v18;
    offset_8 = v19 * workc;
    v17 = workc * v20;
  }
  workd = v5->size;
  v8 = workd * offset_4;
  v9 = offset_8 * workd;
  v10 = workd * v17;
  v11 = start.x + v8;
  v12 = start.y + v9;
  v13 = start.z + v10;
  v6->xyz.x = v11;
  v6->xyz.y = v12;
  v6->xyz.z = v13;
  worke = v5->step * startFraction + v5->fraction;
  v6->_st[0] = (signed int)(worke * 4096.0);
  v6->_st[1] = (signed int)(0.0 * 4096.0);
  *(_DWORD *)v6->color = v7;
  v14 = (int)&v6[1];
  start.x = start.x - v8;
  start.y = start.y - v9;
  start.z = start.z - v10;
  *(idVec3 *)v14 = start;
  startFractiona = v5->step * startFraction + v5->fraction;
  *(_DWORD *)(v14 + 12) = v7;
  *(_WORD *)(v14 + 28) = (signed int)(4096.0 * startFractiona);
  *(_WORD *)(v14 + 30) = 4096;
  v5->tri->numVerts += 2;
}
void __thiscall rvElectricityParticle::ApplyShape(rvElectricityParticle *this, rvBSE *effect, SElecWork *work, idVec3 start, idVec3 end, int count, float startFraction, float endFraction)
{
  float v8; // ebx
  float v9; // ebp
  rvElectricityParticle *v10; // edi
  int v11; // esi
  float v12; // ST3C_4
  float max; // ST28_4
  float min; // ST3C_4
  float v15; // ST3C_4
  float v16; // ST28_4
  float v17; // ST3C_4
  float v18; // ST3C_4
  float v19; // ST3C_4
  double v20; // st7
  double v21; // st6
  float v22; // ST3C_4
  float v23; // ST44_4
  float v24; // ST7C_4
  float v25; // ST80_4
  float shapea; // ST84_4
  float v27; // ST3C_4
  float v28; // STF4_4
  float v29; // STF8_4
  float v30; // STFC_4
  float v31; // STC4_4
  float v32; // STC8_4
  float v33; // STCC_4
  float v34; // ST94_4
  float v35; // ST98_4
  float v36; // ST9C_4
  double v37; // st3
  float v38; // ST40_4
  float v39; // STDC_4
  float v40; // STE0_4
  float v41; // STE4_4
  float point1_4; // ST48_4
  float point1_8; // ST4C_4
  float point2; // ST50_4
  float v45; // ST40_4
  float v46; // STA0_4
  float v47; // STA4_4
  float left; // STA8_4
  float v49; // ST48_4
  __int64 v50; // ST4C_8
  float v51; // ST40_4
  float v52; // STD0_4
  float v53; // STD4_4
  float v54; // STD8_4
  float v55; // STB8_4
  float v56; // STBC_4
  float v57; // STC0_4
  float v58; // ST100_4
  float v59; // ST104_4
  float v60; // ST108_4
  float v61; // ST40_4
  float v62; // STE8_4
  float v63; // STEC_4
  float v64; // STF0_4
  float point2_4; // ST54_4
  float point2_8; // ST58_4
  float forward; // ST5C_4
  float v68; // ST40_4
  float v69; // ST10C_4
  float v70; // ST110_4
  float shapeb; // ST114_4
  float v72; // ST54_4
  __int64 v73; // ST58_8
  float v74; // ST44_4
  idVec3 v75; // ST14_12
  idVec3 v76; // ST08_12
  float v77; // ST3C_4
  idVec3 v78; // ST14_12
  idVec3 v79; // ST08_12
  bool v80; // zf
  idVec3 v81; // ST1C_12
  float length; // [esp+18h] [ebp-F8h]
  float v83; // [esp+1Ch] [ebp-F4h]
  float point1; // [esp+20h] [ebp-F0h]
  float forward_4; // [esp+3Ch] [ebp-D4h]
  float forward_8; // [esp+40h] [ebp-D0h]
  float v87; // [esp+44h] [ebp-CCh]
  float v88; // [esp+48h] [ebp-C8h]
  float v89; // [esp+4Ch] [ebp-C4h]
  float v90; // [esp+50h] [ebp-C0h]
  int v91; // [esp+54h] [ebp-BCh]
  float shapea_4; // [esp+64h] [ebp-ACh]
  float shapea_8; // [esp+68h] [ebp-A8h]
  float v94; // [esp+6Ch] [ebp-A4h]
  float left_4; // [esp+88h] [ebp-88h]
  float left_8; // [esp+8Ch] [ebp-84h]
  float shapeb_4; // [esp+F4h] [ebp-1Ch]
  float shapeb_8; // [esp+F8h] [ebp-18h]
  float v99; // [esp+FCh] [ebp-14h]

  v8 = start.z;
  v9 = start.y;
  v10 = this;
  if ( count >= 1 )
  {
    v11 = count - 1;
    v91 = count;
    do
    {
      shapea_4 = rvRandom::flrand(0.56, 0.75999999);
      shapea_8 = rvRandom::flrand(0.050000001, 0.090000004);
      v94 = rvRandom::flrand(0.050000001, 0.090000004);
      shapeb_4 = rvRandom::flrand(0.23, 0.43000001);
      v12 = 0.01999999955296516 - shapea_8;
      max = v12;
      min = -shapea_8 - 0.01999999955296516;
      shapeb_8 = rvRandom::flrand(min, max);
      v15 = 0.01999999955296516 - v94;
      v16 = v15;
      v17 = -v94 - 0.01999999955296516;
      v99 = rvRandom::flrand(v17, v16);
      v88 = end.x - start.x;
      v89 = end.y - start.y;
      v90 = end.z - start.z;
      forward_4 = v88;
      forward_8 = v89;
      v87 = v90;
      v18 = v88 * v88 + v89 * v89 + v90 * v90;
      length = sqrt(v18);
      v83 = 0.699999988079071 * length;
      if ( length >= 0.00000011920929 )
      {
        v19 = 1.0 / length;
        forward_4 = v19 * v88;
        forward_8 = v89 * v19;
        v87 = v19 * v90;
      }
      v20 = forward_4;
      v21 = forward_8;
      point1 = forward_4 * forward_4 + forward_8 * forward_8;
      if ( point1 == 0.0 )
      {
        left_4 = 1.0;
        left_8 = 0.0;
      }
      else
      {
        v22 = sqrt(point1);
        v23 = 1.0 / v22;
        left_4 = -forward_8 * v23;
        left_8 = v23 * forward_4;
        v21 = forward_8;
        v20 = forward_4;
      }
      v24 = left_8 * v87 - v21 * 0.0;
      v25 = v20 * 0.0 - v87 * left_4;
      shapea = v21 * left_4 - v20 * left_8;
      v27 = 1.0 - shapea_4;
      v28 = v27 * end.x;
      v29 = end.y * v27;
      v30 = v27 * end.z;
      v31 = start.x * shapea_4;
      v32 = start.y * shapea_4;
      v33 = shapea_4 * start.z;
      v34 = v31 + v28;
      v35 = v32 + v29;
      v36 = v33 + v30;
      v37 = v83;
      v38 = shapea_8 * v83;
      v39 = v38 * left_4;
      v40 = left_8 * v38;
      v41 = v38 * 0.0;
      point1_4 = v39 + v34;
      point1_8 = v40 + v35;
      point2 = v41 + v36;
      v45 = v94 * v37;
      v46 = v45 * v24;
      v47 = v25 * v45;
      left = v45 * shapea;
      v49 = v46 + point1_4;
      *(float *)&v50 = v47 + point1_8;
      *((float *)&v50 + 1) = left + point2;
      v51 = 1.0 - shapeb_4;
      v52 = end.x * v51;
      v53 = end.y * v51;
      v54 = v51 * end.z;
      v55 = start.x * shapeb_4;
      v56 = start.y * shapeb_4;
      v57 = shapeb_4 * start.z;
      v58 = v55 + v52;
      v59 = v56 + v53;
      v60 = v57 + v54;
      v61 = shapeb_8 * v37;
      v62 = left_4 * v61;
      v63 = left_8 * v61;
      v64 = 0.0 * v61;
      point2_4 = v62 + v58;
      point2_8 = v63 + v59;
      forward = v64 + v60;
      v68 = v37 * v99;
      v69 = v68 * v24;
      v70 = v25 * v68;
      shapeb = v68 * shapea;
      v72 = v69 + point2_4;
      *(float *)&v73 = v70 + point2_8;
      *((float *)&v73 + 1) = shapeb + forward;
      v74 = 0.6666666865348816 * startFraction + 0.3333333432674408 * endFraction;
      v75.x = v49;
      *(_QWORD *)&v75.y = v50;
      v76.x = start.x;
      *(_QWORD *)&v76.y = __PAIR__(LODWORD(v8), LODWORD(v9));
      rvElectricityParticle::ApplyShape(v10, effect, work, v76, v75, v11, startFraction, v74);
      v77 = startFraction * 0.3333333432674408 + endFraction * 0.6666666865348816;
      v78.x = v72;
      *(_QWORD *)&v78.y = v73;
      v79.x = v49;
      *(_QWORD *)&v79.y = v50;
      rvElectricityParticle::ApplyShape(v10, effect, work, v79, v78, v11, v74, v77);
      start.x = v72;
      startFraction = v77;
      v8 = *((float *)&v73 + 1);
      LODWORD(v9) = v73;
      --v11;
      v80 = v91-- == 1;
      *(_QWORD *)&start.y = v73;
    }
    while ( !v80 );
  }
  v81.x = start.x;
  *(_QWORD *)&v81.y = __PAIR__(LODWORD(v8), LODWORD(v9));
  rvElectricityParticle::RenderLineSegment(v10, effect, work, v81, startFraction);
}
void __thiscall rvElectricityParticle::RenderBranch(rvElectricityParticle *this, rvBSE *effect, SElecWork *work, idVec3 start, idVec3 end)
{
  rvElectricityParticle *v5; // edi
  float v6; // ST40_4
  float v7; // ST40_4
  double v8; // st7
  float v9; // ST40_4
  float v10; // ST3C_4
  srfTriangles_t *v11; // eax
  int v12; // ebp
  double v13; // st4
  float *v14; // ecx
  float v15; // ST98_4
  float v16; // ST9C_4
  float v17; // STA0_4
  float min; // ST24_4
  float v19; // ST48_4
  float v20; // STA4_4
  float v21; // STA8_4
  float v22; // STAC_4
  float v23; // ST24_4
  float v24; // ST48_4
  float v25; // STBC_4
  float v26; // STC0_4
  float v27; // STC4_4
  float pos_4; // ST5C_4
  float pos_8; // ST60_4
  float old; // ST64_4
  float v31; // ST24_4
  float v32; // ST48_4
  float v33; // ST8C_4
  float v34; // ST90_4
  float v35; // ST94_4
  float v36; // ST5C_4
  float v37; // ST60_4
  float v38; // ST64_4
  float v39; // STB0_4
  float v40; // STB4_4
  float v41; // STB8_4
  float v42; // STE4_4
  float v43; // STE8_4
  float v44; // STCC_4
  float v45; // STD0_4
  idVec3 v46; // ST14_12
  idVec3 v47; // ST08_12
  float *v48; // eax
  idVec3 v49; // ST1C_12
  srfTriangles_t *v50; // ecx
  int v51; // eax
  srfTriangles_t *v52; // edx
  int v53; // eax
  int count; // [esp+1Ch] [ebp-B0h]
  int counta; // [esp+1Ch] [ebp-B0h]
  int countb; // [esp+1Ch] [ebp-B0h]
  float countc; // [esp+1Ch] [ebp-B0h]
  float v58; // [esp+20h] [ebp-ACh]
  int v59; // [esp+20h] [ebp-ACh]
  char weighting_3; // [esp+27h] [ebp-A5h]
  int offset; // [esp+2Ch] [ebp-A0h]
  float offset_4; // [esp+30h] [ebp-9Ch]
  float offset_8; // [esp+34h] [ebp-98h]
  float pos; // [esp+38h] [ebp-94h]
  float old_4; // [esp+48h] [ebp-84h]
  __int64 old_8; // [esp+4Ch] [ebp-80h]
  float current_4; // [esp+54h] [ebp-78h]
  __int64 current_8; // [esp+58h] [ebp-74h]
  float left_4; // [esp+60h] [ebp-6Ch]
  float left_8; // [esp+64h] [ebp-68h]
  float v71; // [esp+A8h] [ebp-24h]
  float v72; // [esp+B4h] [ebp-18h]
  float v73; // [esp+B8h] [ebp-14h]
  float v74; // [esp+BCh] [ebp-10h]
  float v75; // [esp+C0h] [ebp-Ch]

  v5 = this;
  v6 = work->forward.y * work->forward.y + work->forward.x * work->forward.x + work->forward.z * work->forward.z;
  v58 = sqrt(v6);
  if ( v58 >= 0.00000011920929 )
  {
    v7 = 1.0 / v58;
    work->forward.x = work->forward.x * v7;
    work->forward.y = work->forward.y * v7;
    work->forward.z = v7 * work->forward.z;
  }
  *(float *)&count = work->forward.y * work->forward.y + work->forward.x * work->forward.x;
  v8 = 0.0;
  if ( *(float *)&count == 0.0 )
  {
    left_4 = 1.0;
    left_8 = 0.0;
  }
  else
  {
    v9 = sqrt(*(float *)&count);
    v10 = 1.0 / v9;
    left_4 = -work->forward.y * v10;
    left_8 = v10 * work->forward.x;
    v8 = 0.0;
  }
  v11 = work->tri;
  v12 = v11->numVerts;
  weighting_3 = 1;
  v59 = 0;
  counta = v11->numAllocedVerts - v12;
  v72 = work->forward.z * left_8 - work->forward.y * 0.0;
  v73 = 0.0 * work->forward.x - work->forward.z * left_4;
  v74 = left_4 * work->forward.y - left_8 * work->forward.x;
  old_4 = start.x;
  *(float *)&old_8 = start.y;
  *((float *)&old_8 + 1) = start.z;
  pos = v8;
  offset_8 = v8;
  offset_4 = v8;
  if ( counta > 40 )
  {
    *(float *)&countb = 40.0 / (double)counta;
    v13 = work->step;
    if ( *(float *)&countb >= v13 )
      v13 = *(float *)&countb;
    countc = v13;
    v14 = (float *)work->coords;
    work->step = countc;
    while ( 1 )
    {
      ++v59;
      *v14 = old_4;
      offset = (int)(v14 + 3);
      v14[1] = *(float *)&old_8;
      v14[2] = *((float *)&old_8 + 1);
      if ( 1.0 - work->step * 0.5 <= countc )
      {
        weighting_3 = 0;
        countc = 1.0;
      }
      min = -v5->mJitterSize.x;
      v19 = rvRandom::flrand(min, v5->mJitterSize.x);
      v20 = work->forward.x * v19;
      v21 = work->forward.y * v19;
      v22 = v19 * work->forward.z;
      v23 = -v5->mJitterSize.y;
      v24 = rvRandom::flrand(v23, v5->mJitterSize.y);
      v25 = v24 * left_4;
      v26 = left_8 * v24;
      v27 = v24 * 0.0;
      pos_4 = v25 + v20;
      pos_8 = v26 + v21;
      old = v27 + v22;
      v31 = -v5->mJitterSize.z;
      v32 = rvRandom::flrand(v31, v5->mJitterSize.z);
      v33 = v32 * v72;
      v34 = v32 * v73;
      v35 = v32 * v74;
      v36 = v33 + pos_4;
      v37 = v34 + pos_8;
      v38 = v35 + old;
      offset_4 = v36 + offset_4;
      offset_8 = v37 + offset_8;
      pos = v38 + pos;
      ((void (__stdcall *)(float))v5->mJitterTable->vfptr[1].__vecDelDtor)(COERCE_FLOAT(LODWORD(countc)));
      v15 = end.x - start.x;
      v39 = v15 * countc;
      v16 = end.y - start.y;
      v40 = v16 * countc;
      v17 = end.z - start.z;
      v41 = v17 * countc;
      v75 = v39 + start.x;
      v42 = v40 + start.y;
      v43 = v41 + start.z;
      v71 = offset_4 * countc;
      v44 = offset_8 * countc;
      v45 = countc * pos;
      current_4 = v71 + v75;
      *(float *)&current_8 = v44 + v42;
      *((float *)&current_8 + 1) = v45 + v43;
      work->fraction = countc - work->step;
      v46.x = current_4;
      *(_QWORD *)&v46.y = current_8;
      v47.x = old_4;
      *(_QWORD *)&v47.y = old_8;
      rvElectricityParticle::ApplyShape(v5, effect, work, v47, v46, 2, 0.0, 1.0);
      old_4 = current_4;
      *(float *)&old_8 = *(float *)&current_8;
      *((float *)&old_8 + 1) = *((float *)&current_8 + 1);
      countc = work->step + countc;
      if ( !weighting_3 )
        break;
      v14 = (float *)offset;
    }
    if ( v12 != work->tri->numVerts )
    {
      v48 = &work->fraction + 3 * v59 + 18;
      *v48 = current_4;
      v48[1] = *(float *)&current_8;
      v48[2] = *((float *)&current_8 + 1);
      v49.x = v71 + v75;
      *(_QWORD *)&v49.y = current_8;
      rvElectricityParticle::RenderLineSegment(v5, effect, work, v49, 1.0);
      v50 = work->tri;
      v51 = v50->numIndexes;
      if ( v12 < v50->numVerts - 2 )
      {
        do
        {
          v52 = work->tri;
          if ( v52->numIndexes + 6 >= v52->numAllocedIndices )
            break;
          v52->indexes[v51] = v12;
          v53 = v51 + 1;
          work->tri->indexes[v53++] = v12 + 1;
          work->tri->indexes[v53] = v12 + 3;
          work->tri->indexes[v53++ + 1] = v12;
          work->tri->indexes[v53 + 1] = v12 + 3;
          v53 += 2;
          work->tri->indexes[v53] = v12 + 2;
          v12 += 2;
          v51 = v53 + 1;
        }
        while ( v12 < work->tri->numVerts - 2 );
      }
      work->tri->numIndexes = v51;
    }
  }
}
char __thiscall rvElectricityParticle::Render(rvElectricityParticle *this, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time, float override)
{
  rvElectricityParticle *v7; // esi
  rvEnvParms *v9; // ebx
  float v10; // ST6C_4
  float oneOverDuration; // ST74_4
  float v12; // ST6C_4
  float v13; // ST6C_4
  int v14; // eax
  float v15; // ST6C_4
  float v16; // ST70_4
  float v17; // ST6C_4
  float v18; // ST6C_4
  float v19; // ST6C_4
  float v20; // ST6C_4
  float v21; // ST6C_4
  float v22; // ST78_4
  float v23; // ST7C_4
  float point_8; // ST80_4
  float v25; // ST6C_4
  unsigned int v26; // eax
  double v27; // st6
  double v28; // st7
  float v29; // ST78_4
  idVec3 v30; // ST1C_12
  float v31; // ST7C_4
  float v32; // ST80_4
  idVec3 v33; // ST10_12
  float *j; // edi
  signed int v35; // eax
  unsigned int v36; // eax
  double v37; // st7
  idVec3 *v38; // eax
  float *v39; // edi
  signed int v40; // eax
  double v41; // st7
  double v42; // st7
  double v43; // st7
  float v44; // ST70_4
  float v45; // ST70_4
  double v46; // st7
  float v47; // ST70_4
  float v48; // ST70_4
  signed int v49; // eax
  idVec3 v50; // ST1C_12
  idVec3 v51; // ST10_12
  float length; // [esp+6Ch] [ebp-3FCh]
  float lengtha; // [esp+6Ch] [ebp-3FCh]
  signed int forkLength; // [esp+70h] [ebp-3F8h]
  int i; // [esp+74h] [ebp-3F4h]
  int ia; // [esp+74h] [ebp-3F4h]
  float point; // [esp+78h] [ebp-3F0h]
  __int64 point_4; // [esp+7Ch] [ebp-3ECh]
  idVec3 velocity; // [esp+84h] [ebp-3E4h]
  float evalTime; // [esp+90h] [ebp-3D8h]
  idVec3 position; // [esp+94h] [ebp-3D4h]
  float v62; // [esp+A0h] [ebp-3C8h]
  float v63; // [esp+A4h] [ebp-3C4h]
  float v64; // [esp+A8h] [ebp-3C0h]
  float v65; // [esp+ACh] [ebp-3BCh]
  float v66; // [esp+B0h] [ebp-3B8h]
  float v67; // [esp+B4h] [ebp-3B4h]
  float v68; // [esp+B8h] [ebp-3B0h]
  float v69; // [esp+BCh] [ebp-3ACh]
  float v70; // [esp+C0h] [ebp-3A8h]
  float v71; // [esp+C4h] [ebp-3A4h]
  float v72; // [esp+C8h] [ebp-3A0h]
  float v73; // [esp+CCh] [ebp-39Ch]
  float v74; // [esp+D0h] [ebp-398h]
  float v75; // [esp+D4h] [ebp-394h]
  float v76; // [esp+D8h] [ebp-390h]
  float v77; // [esp+DCh] [ebp-38Ch]
  float v78; // [esp+E0h] [ebp-388h]
  float v79; // [esp+E4h] [ebp-384h]
  float v80; // [esp+E8h] [ebp-380h]
  float v81; // [esp+ECh] [ebp-37Ch]
  float v82; // [esp+F0h] [ebp-378h]
  unsigned int seed; // [esp+F4h] [ebp-374h]
  SElecWork work; // [esp+F8h] [ebp-370h]
  idVec3 forkPoints[16]; // [esp+3A8h] [ebp-C0h]

  v7 = this;
  if ( !rvParticle::GetEvaluationTime((rvParticle *)&this->vfptr, time, &evalTime, 0) )
    return 0;
  v9 = pt->mpFadeEnvelope;
  v10 = v7->mEndTime - v7->mStartTime;
  oneOverDuration = 1.0 / v10;
  v12 = evalTime;
  rvEnvParms::Evaluate(pt->mpTintEnvelope, &v7->mTintEnv, evalTime, oneOverDuration, &work.tint.x);
  rvEnvParms::Evaluate(v9, &v7->mFadeEnv, v12, oneOverDuration, &work.tint.w);
  (*(void (__thiscall **)(rvElectricityParticle *, rvEnvParms *, _DWORD, _DWORD, float *))&v7->vfptr->gap4[8])(
    v7,
    pt->mpSizeEnvelope,
    LODWORD(evalTime),
    LODWORD(oneOverDuration),
    &work.size);
  ((void (__thiscall *)(rvElectricityParticle *, rvEnvParms *, _DWORD, _DWORD, idVec3 *))v7->vfptr->EvaluateLength)(
    v7,
    pt->mpLengthEnvelope,
    LODWORD(evalTime),
    LODWORD(oneOverDuration),
    &work.length);
  v13 = time - v7->mMotionStartTime;
  rvParticle::EvaluatePosition((rvParticle *)&v7->vfptr, effect, pt, &position, v13);
  v14 = v7->mFlags;
  if ( !(v14 & 2) )
  {
    v71 = effect->mCurrentAxis.mat[0].y * v7->mInitAxis.mat[0].y
        + v7->mInitAxis.mat[0].x * effect->mCurrentAxis.mat[0].x
        + effect->mCurrentAxis.mat[0].z * v7->mInitAxis.mat[0].z;
    v72 = v7->mInitAxis.mat[0].x * effect->mCurrentAxis.mat[1].x
        + effect->mCurrentAxis.mat[1].y * v7->mInitAxis.mat[0].y
        + v7->mInitAxis.mat[0].z * effect->mCurrentAxis.mat[1].z;
    v73 = v7->mInitAxis.mat[0].x * effect->mCurrentAxis.mat[2].x
        + effect->mCurrentAxis.mat[2].y * v7->mInitAxis.mat[0].y
        + effect->mCurrentAxis.mat[2].z * v7->mInitAxis.mat[0].z;
    v74 = effect->mCurrentAxis.mat[0].y * v7->mInitAxis.mat[1].y
        + v7->mInitAxis.mat[1].x * effect->mCurrentAxis.mat[0].x
        + effect->mCurrentAxis.mat[0].z * v7->mInitAxis.mat[1].z;
    v75 = effect->mCurrentAxis.mat[1].y * v7->mInitAxis.mat[1].y
        + effect->mCurrentAxis.mat[1].x * v7->mInitAxis.mat[1].x
        + effect->mCurrentAxis.mat[1].z * v7->mInitAxis.mat[1].z;
    v76 = v7->mInitAxis.mat[1].x * effect->mCurrentAxis.mat[2].x
        + effect->mCurrentAxis.mat[2].y * v7->mInitAxis.mat[1].y
        + effect->mCurrentAxis.mat[2].z * v7->mInitAxis.mat[1].z;
    v77 = effect->mCurrentAxis.mat[0].x * v7->mInitAxis.mat[2].x
        + v7->mInitAxis.mat[2].y * effect->mCurrentAxis.mat[0].y
        + effect->mCurrentAxis.mat[0].z * v7->mInitAxis.mat[2].z;
    v78 = effect->mCurrentAxis.mat[1].x * v7->mInitAxis.mat[2].x
        + v7->mInitAxis.mat[2].y * effect->mCurrentAxis.mat[1].y
        + v7->mInitAxis.mat[2].z * effect->mCurrentAxis.mat[1].z;
    v79 = effect->mCurrentAxis.mat[2].y * v7->mInitAxis.mat[2].y
        + effect->mCurrentAxis.mat[2].x * v7->mInitAxis.mat[2].x
        + effect->mCurrentAxis.mat[2].z * v7->mInitAxis.mat[2].z;
    v15 = v77 * work.length.z + v74 * work.length.y + v71 * work.length.x;
    v16 = v72 * work.length.x + v75 * work.length.y + v78 * work.length.z;
    work.length.z = work.length.y * v76 + work.length.x * v73 + work.length.z * v79;
    work.length.x = v15;
    work.length.y = v16;
  }
  if ( v14 & 0x10000 )
  {
    v17 = time - v7->mMotionStartTime;
    rvParticle::EvaluateVelocity((rvParticle *)&v7->vfptr, effect, &velocity, v17);
    v18 = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z;
    length = sqrt(v18);
    if ( length >= 0.00000011920929 )
    {
      v19 = 1.0 / length;
      velocity.x = v19 * velocity.x;
      velocity.y = velocity.y * v19;
      velocity.z = v19 * velocity.z;
    }
    v20 = work.length.x * work.length.x + work.length.y * work.length.y + work.length.z * work.length.z;
    v21 = sqrt(v20);
    v22 = v21 * velocity.x;
    v23 = velocity.y * v21;
    point_8 = v21 * velocity.z;
    work.length.x = v22;
    work.length.y = v23;
    work.length.z = point_8;
  }
  v25 = work.length.x * work.length.x + work.length.y * work.length.y + work.length.z * work.length.z;
  lengtha = sqrt(v25);
  if ( lengtha < 0.1000000014901161 )
    return 0;
  v26 = rvRandom::irand(0, 0x7FFF);
  v27 = v7->mJitterRate;
  seed = v26;
  if ( v27 + v7->mLastJitter <= time )
  {
    v7->mLastJitter = time;
    v7->mSeed = rvRandom::Init();
  }
  rvRandom::mSeed = v7->mSeed;
  work.viewPos.x = view->mat[0].x;
  v28 = view->mat[0].y;
  work.tri = tri;
  work.viewPos.y = v28;
  work.viewPos.z = view->mat[0].z;
  work.alpha = override;
  work.forward.x = work.length.x;
  work.forward.y = work.length.y;
  work.forward.z = work.length.z;
  work.step = v7->mTextureScale / (double)v7->mNumBolts;
  v29 = work.length.x + position.x;
  v30.x = v29;
  v31 = work.length.y + position.y;
  v30.y = v31;
  v32 = work.length.z + position.z;
  v30.z = v32;
  v33.x = position.x;
  *(_QWORD *)&v33.y = *(_QWORD *)&position.y;
  rvElectricityParticle::RenderBranch(v7, effect, &work, v33, v30);
  i = 0;
  for ( j = &forkPoints[0].z; ; *(j - 3) = v38->z )
  {
    v35 = v7->mNumForks;
    if ( v35 >= 16 )
      v35 = 16;
    if ( i >= v35 )
      break;
    ++i;
    v36 = rvRandom::irand(1, v7->mNumBolts - 1);
    v37 = work.coords[v36].x;
    v38 = &work.coords[v36];
    *(j - 2) = v37;
    j += 3;
    *(j - 4) = v38->y;
  }
  ia = 0;
  v39 = &forkPoints[0].z;
  while ( 1 )
  {
    v40 = v7->mNumForks;
    if ( v40 >= 16 )
      v40 = 16;
    if ( ia >= v40 )
      break;
    v68 = position.x + *(v39 - 2);
    v69 = position.y + *(v39 - 1);
    v70 = *v39 + position.z;
    v62 = v68 + work.length.x;
    v63 = v69 + work.length.y;
    v64 = v70 + work.length.z;
    v80 = v62 * 0.5;
    v81 = v63 * 0.5;
    v82 = 0.5 * v64;
    v41 = rvRandom::flrand(v7->mForkSizeMins.x, v7->mForkSizeMaxs.x);
    point = v41 + v80;
    v42 = rvRandom::flrand(v7->mForkSizeMins.y, v7->mForkSizeMaxs.y);
    *(float *)&point_4 = v42 + v81;
    v43 = rvRandom::flrand(v7->mForkSizeMins.z, v7->mForkSizeMaxs.z);
    *((float *)&point_4 + 1) = v43 + v82;
    v65 = point - *(v39 - 2);
    v66 = *(float *)&point_4 - *(v39 - 1);
    v67 = *((float *)&point_4 + 1) - *v39;
    work.forward.x = v65;
    work.forward.y = v66;
    work.forward.z = v67;
    v44 = v67 * v67 + v66 * v66 + v65 * v65;
    v45 = sqrt(v44);
    v46 = v45;
    if ( v45 <= 1.0 || lengtha <= v46 )
    {
      ++ia;
      v39 += 3;
    }
    else
    {
      v47 = v46 * 0.0625;
      v48 = _ceil(v47);
      v49 = (signed int)v48;
      forkLength = (signed int)v48;
      if ( v49 >= 3 )
      {
        if ( v49 > 50 )
          forkLength = 50;
      }
      else
      {
        forkLength = 3;
      }
      v50.x = point;
      *(_QWORD *)&v50.y = point_4;
      *(_QWORD *)&v51.x = *((_QWORD *)v39 - 1);
      v51.z = *v39;
      work.step = 1.0 / (double)forkLength;
      rvElectricityParticle::RenderBranch(v7, effect, &work, v51, v50);
      ++ia;
      v39 += 3;
    }
  }
  rvRandom::mSeed = seed;
  return 1;
}
int dynamic_initializer_for__singletonLock___118()
{
  sdLock::sdLock(&singletonLock_118);
  return atexit(dynamic_atexit_destructor_for__singletonLock___118);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___118()
{
  sdLock::~sdLock(&singletonLock_118);
}
