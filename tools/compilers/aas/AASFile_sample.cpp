/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "../../../idlib/precompiled.h"
#pragma hdrstop

#include "AASFile.h"
#include "AASFile_local.h"


//===============================================================
//
//	Environment Sampling
//
//===============================================================

/*
================
idAASFileLocal::EdgeCenter
================
*/
idVec3 idAASFileLocal::EdgeCenter(int edgeNum) const
{
	const aasEdge_t *edge;
	edge = &edges[edgeNum];
	return (vertices[edge->vertexNum[0]] + vertices[edge->vertexNum[1]]) * 0.5f;
}

/*
================
idAASFileLocal::FaceCenter
================
*/
idVec3 idAASFileLocal::FaceCenter(int faceNum) const
{
	int i, edgeNum;
	const aasFace_t *face;
	const aasEdge_t *edge;
	idVec3 center;

	center = vec3_origin;

#ifdef _SPLASHDAMAGE
	edgeNum = faceNum;
	edge = &edges[ abs(edgeNum)];
	center += vertices[ edge->vertexNum[ INTSIGNBITSET(edgeNum)] ];
#else
	face = &faces[faceNum];

	if (face->numEdges > 0) {
		for (i = 0; i < face->numEdges; i++) {
			edgeNum = edgeIndex[ face->firstEdge + i ];
			edge = &edges[ abs(edgeNum)];
			center += vertices[ edge->vertexNum[ INTSIGNBITSET(edgeNum)] ];
		}

		center /= face->numEdges;
	}
#endif

	return center;
}

/*
================
idAASFileLocal::AreaCenter
================
*/
idVec3 idAASFileLocal::AreaCenter(int areaNum) const
{
	int i, faceNum;
	const aasArea_t *area;
	idVec3 center;

	center = vec3_origin;

	area = &areas[areaNum];

#ifdef _SPLASHDAMAGE
	if (area->numEdges > 0) {
		for (i = 0; i < area->numEdges; i++) {
			int edgeNum = edgeIndex[area->firstEdge + i];
			center += EdgeCenter(abs(edgeNum));
		}

		center /= area->numEdges;
	}
#else
	if (area->numFaces > 0) {
		for (i = 0; i < area->numFaces; i++) {
			faceNum = faceIndex[area->firstFace + i];
			center += FaceCenter(abs(faceNum));
		}

		center /= area->numFaces;
	}
#endif

	return center;
}

/*
============
idAASFileLocal::AreaReachableGoal
============
*/
idVec3 idAASFileLocal::AreaReachableGoal(int areaNum) const
{
	int i, faceNum, numFaces;
	const aasArea_t *area;
	idVec3 center;
	idVec3 start, end;
	aasTrace_t trace;

	area = &areas[areaNum];

	if (!(area->flags & (AREA_REACHABLE_WALK|AREA_REACHABLE_FLY)) || (area->flags & AREA_LIQUID)) {
		return AreaCenter(areaNum);
	}

	center = vec3_origin;

#ifdef _SPLASHDAMAGE
	int _numEdges = 0;

	for (i = 0; i < area->numEdges; i++) {
		int edgeNum = edgeIndex[area->firstEdge + i];

		if (!(edges[abs(edgeNum)].flags & FACE_FLOOR)) {
			continue;
		}

		center += FaceCenter(abs(edgeNum));
		_numEdges++;
	}

	if (_numEdges > 0) {
		center /= _numEdges;
	}
#else
	numFaces = 0;

	for (i = 0; i < area->numFaces; i++) {
		faceNum = faceIndex[area->firstFace + i];

		if (!(faces[abs(faceNum)].flags & FACE_FLOOR)) {
			continue;
		}

		center += FaceCenter(abs(faceNum));
		numFaces++;
	}

	if (numFaces > 0) {
		center /= numFaces;
	}
#endif

	center[2] += 1.0f;
	end = center;
	end[2] -= 1024;
	Trace(trace, center, end);

	return trace.endpos;
}

/*
================
idAASFileLocal::EdgeBounds
================
*/
idBounds idAASFileLocal::EdgeBounds(int edgeNum) const
{
	const aasEdge_t *edge;
	idBounds bounds;

	edge = &edges[ abs(edgeNum)];
	bounds[0] = bounds[1] = vertices[ edge->vertexNum[0] ];
	bounds += vertices[ edge->vertexNum[1] ];
	return bounds;
}

/*
================
idAASFileLocal::FaceBounds
================
*/
idBounds idAASFileLocal::FaceBounds(int faceNum) const
{
	int i, edgeNum;
	const aasFace_t *face;
	const aasEdge_t *edge;
	idBounds bounds;

#ifdef _SPLASHDAMAGE
	edgeNum = faceNum;
	bounds.Clear();

	edge = &edges[ abs(edgeNum)];
	bounds.AddPoint(vertices[ edge->vertexNum[ INTSIGNBITSET(edgeNum)] ]);
#else
	face = &faces[faceNum];
	bounds.Clear();

	for (i = 0; i < face->numEdges; i++) {
		edgeNum = edgeIndex[ face->firstEdge + i ];
		edge = &edges[ abs(edgeNum)];
		bounds.AddPoint(vertices[ edge->vertexNum[ INTSIGNBITSET(edgeNum)] ]);
	}
#endif

	return bounds;
}

/*
================
idAASFileLocal::AreaBounds
================
*/
idBounds idAASFileLocal::AreaBounds(int areaNum) const
{
	int i, faceNum;
	const aasArea_t *area;
	idBounds bounds;

	area = &areas[areaNum];
	bounds.Clear();

#ifdef _SPLASHDAMAGE
	for (i = 0; i < area->numEdges; i++) {
		int edgeNum = edgeIndex[area->firstEdge + i];
		bounds += EdgeBounds(abs(edgeNum));
	}
#else
	for (i = 0; i < area->numFaces; i++) {
		faceNum = faceIndex[area->firstFace + i];
		bounds += FaceBounds(abs(faceNum));
	}
#endif

	return bounds;
}

/*
============
idAASFileLocal::PointAreaNum
============
*/
int idAASFileLocal::PointAreaNum(const idVec3 &origin) const
{
	int nodeNum;
	const aasNode_t *node;

	nodeNum = 1;

	do {
		node = &nodes[nodeNum];

#ifdef _SPLASHDAMAGE
		if (planeList[node->planeNum].Distance(origin) <= 0.0f) 
#else
		if (planeList[node->planeNum].Side(origin) == PLANESIDE_BACK) 
#endif
		{
			nodeNum = node->children[1];
		} else {
			nodeNum = node->children[0];
		}

		if (nodeNum < 0) {
			return -nodeNum;
		}
	} while (nodeNum);

	return 0;
}

/*
============
idAASFileLocal::PointReachableAreaNum
============
*/
int idAASFileLocal::PointReachableAreaNum(const idVec3 &origin, const idBounds &searchBounds, const int areaFlags, const int excludeTravelFlags) const
{
#ifdef _SPLASHDAMAGE
	int result; // eax
	int v7; // ebx
	idVec3 v8;
	idVec3 v11; // [esp+14h] [ebp-44h] BYREF
	idBounds v12; // [esp+20h] [ebp-38h] BYREF
	idAASFileLocal::bestReachableArea_t v13; // [esp+38h] [ebp-20h] BYREF
	float maxStepHeight; // [esp+64h] [ebp+Ch]

	v13.v0 = this->settings.boundingBox[1].z - this->settings.boundingBox[0].z;
	v13.excludeTravelFlags = excludeTravelFlags;
	v13.v1 = 24.0f;
	v13.distance1 = 1.0e30f;
	v13.areaFlags = areaFlags;
	v13.distance2 = 1.0e30f;
	v13.areaNum1 = 0;
	v13.areaNum2 = 0;
	idAASFileLocal::PointBestReachableAreaNum(&origin, &v13);
	if ( !v13.areaNum1 )
	{
		maxStepHeight = this->settings.maxStepHeight;
		v8 = this->settings.invGravityDir * maxStepHeight;
		v11 = origin + v8;
		idAASFileLocal::PointBestReachableAreaNum(&v11, &v13);
	}
	if ( v13.v0 > v13.distance1 )
		return v13.areaNum1;
	v7 = 0;
	v12[0].x = origin.x - 4.0f;
	v12[0].y = origin.y - 4.0f;
	v12[1].x = origin.x + 4.0f;
	v12[1].y = origin.y + 4.0f;
	v12[1].z = origin.z;
	v12[0].z = v12[1].z;
	while ( 1 )
	{
		result = v13.areaNum2;
		if ( v13.areaNum2 )
			break;
		idAASFileLocal::BoundsBestReachableAreaNum(&v12, &origin, 1, 0, &v13);
		++v7;
		v12.ExpandSelf(4.0f);
		if ( v7 >= 4 )
		{
			result = v13.areaNum2;
			if ( !v13.areaNum2 )
				return v13.areaNum1;
			return result;
		}
	}
	return result;
#else
	int areaList[32], areaNum, i;
	idVec3 start, end, pointList[32];
	aasTrace_t trace;
	idBounds bounds;
	float frac;

	start = origin;

	trace.areas = areaList;
	trace.points = pointList;
	trace.maxAreas = sizeof(areaList) / sizeof(int);
	trace.getOutOfSolid = true;

	areaNum = PointAreaNum(start);

	if (areaNum) {
		if ((areas[areaNum].flags & areaFlags) && ((areas[areaNum].travelFlags & excludeTravelFlags) == 0)) {
			return areaNum;
		}
	} else {
		// trace up
		end = start;
		end[2] += 32.0f;
		Trace(trace, start, end);

		if (trace.numAreas >= 1) {
			if ((areas[0].flags & areaFlags) && ((areas[0].travelFlags & excludeTravelFlags) == 0)) {
				return areaList[0];
			}

			start = pointList[0];
			start[2] += 1.0f;
		}
	}

	// trace down
	end = start;
	end[2] -= 32.0f;
	Trace(trace, start, end);

	if (trace.lastAreaNum) {
		if ((areas[trace.lastAreaNum].flags & areaFlags) && ((areas[trace.lastAreaNum].travelFlags & excludeTravelFlags) == 0)) {
			return trace.lastAreaNum;
		}

		start = trace.endpos;
	}

	// expand bounds until an area is found
	for (i = 1; i <= 12; i++) {
		frac = i * (1.0f / 12.0f);
		bounds[0] = origin + searchBounds[0] * frac;
		bounds[1] = origin + searchBounds[1] * frac;
		areaNum = BoundsReachableAreaNum(bounds, areaFlags, excludeTravelFlags);

		if (areaNum && (areas[areaNum].flags & areaFlags) && ((areas[areaNum].travelFlags & excludeTravelFlags) == 0)) {
			return areaNum;
		}
	}

	return 0;
#endif
}

/*
============
idAASFileLocal::BoundsReachableAreaNum_r
============
*/
int idAASFileLocal::BoundsReachableAreaNum_r(int nodeNum, const idBounds &bounds, const int areaFlags, const int excludeTravelFlags) const
{
	int res;
	const aasNode_t *node;

	while (nodeNum) {
		if (nodeNum < 0) {
			if ((areas[-nodeNum].flags & areaFlags) && ((areas[-nodeNum].travelFlags & excludeTravelFlags) == 0)) {
				return -nodeNum;
			}

			return 0;
		}

		node = &nodes[nodeNum];
		res = bounds.PlaneSide(planeList[node->planeNum]);

		if (res == PLANESIDE_BACK) {
			nodeNum = node->children[1];
		} else if (res == PLANESIDE_FRONT) {
			nodeNum = node->children[0];
		} else {
			nodeNum = BoundsReachableAreaNum_r(node->children[1], bounds, areaFlags, excludeTravelFlags);

			if (nodeNum) {
				return nodeNum;
			}

			nodeNum = node->children[0];
		}
	}

	return 0;
}

/*
============
idAASFileLocal::BoundsReachableAreaNum
============
*/
int idAASFileLocal::BoundsReachableAreaNum(const idBounds &bounds, const int areaFlags, const int excludeTravelFlags) const
{

	return BoundsReachableAreaNum_r(1, bounds, areaFlags, excludeTravelFlags);
}

/*
============
idAASFileLocal::PushPointIntoAreaNum
============
*/
void idAASFileLocal::PushPointIntoAreaNum(int areaNum, idVec3 &point) const
{
#ifdef _SPLASHDAMAGE //karin: removed in ETQW game
	PushPointIntoArea(areaNum, point);
	//common->Error("Disable idAASFileLocal::PushPointIntoAreaNum(%d)", areaNum);
#else
	int i, faceNum;
	const aasArea_t *area;
	const aasFace_t *face;

	area = &areas[areaNum];

	// push the point to the right side of all area face planes
	for (i = 0; i < area->numFaces; i++) {
		faceNum = faceIndex[area->firstFace + i];
		face = &faces[abs(faceNum)];

		const idPlane &plane = planeList[face->planeNum ^ INTSIGNBITSET(faceNum)];
		float dist = plane.Distance(point);

		// project the point onto the face plane if it is on the wrong side
		if (dist < 0.0f) {
			point -= dist * plane.Normal();
		}
	}
#endif
}

/*
============
idAASFileLocal::Trace
============
*/
#define TRACEPLANE_EPSILON		0.125f

typedef struct aasTraceStack_s {
	idVec3			start;
	idVec3			end;
	int				planeNum;
	int				nodeNum;
} aasTraceStack_t;

bool idAASFileLocal::Trace(aasTrace_t &trace, const idVec3 &start, const idVec3 &end) const
{
#ifdef _SPLASHDAMAGExxx
	return Trace(&trace, &start, &end);
#else
	int side, nodeNum, tmpPlaneNum;
	double front, back, frac;
	idVec3 cur_start, cur_end, cur_mid, v1, v2;
	aasTraceStack_t tracestack[MAX_AAS_TREE_DEPTH];
	aasTraceStack_t *tstack_p;
	const aasNode_t *node;
	const idPlane *plane;

	trace.numAreas = 0;
	trace.lastAreaNum = 0;
	trace.blockingAreaNum = 0;

	tstack_p = tracestack;
	tstack_p->start = start;
	tstack_p->end = end;
	tstack_p->planeNum = 0;
	tstack_p->nodeNum = 1;		//start with the root of the tree
	tstack_p++;

	while (1) {

		tstack_p--;

		// if the trace stack is empty
		if (tstack_p < tracestack) {
			if (!trace.lastAreaNum) {
				// completely in solid
				trace.fraction = 0.0f;
				trace.endpos = start;
			} else {
				// nothing was hit
				trace.fraction = 1.0f;
				trace.endpos = end;
			}

			trace.planeNum = 0;
			return false;
		}

		// number of the current node to test the line against
		nodeNum = tstack_p->nodeNum;

		// if it is an area
		if (nodeNum < 0) {
			// if can't enter the area
			if ((areas[-nodeNum].flags & trace.flags) || (areas[-nodeNum].travelFlags & trace.travelFlags)) {
				if (!trace.lastAreaNum) {
					trace.fraction = 0.0f;
					v1 = vec3_origin;
				} else {
					v1 = end - start;
					v2 = tstack_p->start - start;
					trace.fraction = v2.Length() / v1.Length();
				}

				trace.endpos = tstack_p->start;
				trace.blockingAreaNum = -nodeNum;
				trace.planeNum = tstack_p->planeNum;
				// always take the plane with normal facing towards the trace start
				plane = &planeList[trace.planeNum];

				if (v1 * plane->Normal() > 0.0f) {
					trace.planeNum ^= 1;
				}

				return true;
			}

			trace.lastAreaNum = -nodeNum;

			if (trace.numAreas < trace.maxAreas) {
				if (trace.areas) {
					trace.areas[trace.numAreas] = -nodeNum;
				}

				if (trace.points) {
					trace.points[trace.numAreas] = tstack_p->start;
				}

				trace.numAreas++;
			}

			continue;
		}

		// if it is a solid leaf
		if (!nodeNum) {
			if (!trace.lastAreaNum) {
				trace.fraction = 0.0f;
				v1 = vec3_origin;
			} else {
				v1 = end - start;
				v2 = tstack_p->start - start;
				trace.fraction = v2.Length() / v1.Length();
			}

			trace.endpos = tstack_p->start;
			trace.blockingAreaNum = 0;	// hit solid leaf
			trace.planeNum = tstack_p->planeNum;
			// always take the plane with normal facing towards the trace start
			plane = &planeList[trace.planeNum];

			if (v1 * plane->Normal() > 0.0f) {
				trace.planeNum ^= 1;
			}

			if (!trace.lastAreaNum && trace.getOutOfSolid) {
				continue;
			} else {
				return true;
			}
		}

		// the node to test against
		node = &nodes[nodeNum];
		// start point of current line to test against node
		cur_start = tstack_p->start;
		// end point of the current line to test against node
		cur_end = tstack_p->end;
		// the current node plane
		plane = &planeList[node->planeNum];

		front = plane->Distance(cur_start);
		back = plane->Distance(cur_end);

		// if the whole to be traced line is totally at the front of this node
		// only go down the tree with the front child
		if (front >= -ON_EPSILON && back >= -ON_EPSILON) {
			// keep the current start and end point on the stack and go down the tree with the front child
			tstack_p->nodeNum = node->children[0];
			tstack_p++;

			if (tstack_p >= &tracestack[MAX_AAS_TREE_DEPTH]) {
				common->Error("idAASFileLocal::Trace: stack overflow\n");
				return false;
			}
		}
		// if the whole to be traced line is totally at the back of this node
		// only go down the tree with the back child
		else if (front < ON_EPSILON && back < ON_EPSILON) {
			// keep the current start and end point on the stack and go down the tree with the back child
			tstack_p->nodeNum = node->children[1];
			tstack_p++;

			if (tstack_p >= &tracestack[MAX_AAS_TREE_DEPTH]) {
				common->Error("idAASFileLocal::Trace: stack overflow\n");
				return false;
			}
		}
		// go down the tree both at the front and back of the node
		else {
			tmpPlaneNum = tstack_p->planeNum;

			// calculate the hit point with the node plane
			// put the cross point TRACEPLANE_EPSILON on the near side
			if (front < 0) {
				frac = (front + TRACEPLANE_EPSILON) / (front - back);
			} else {
				frac = (front - TRACEPLANE_EPSILON) / (front - back);
			}

			if (frac < 0) {
				frac = 0.001f; //0
			} else if (frac > 1) {
				frac = 0.999f; //1
			}

			cur_mid = cur_start + (cur_end - cur_start) * frac;

			// side the front part of the line is on
			side = front < 0;

			// first put the end part of the line on the stack (back side)
			tstack_p->start = cur_mid;
			tstack_p->planeNum = node->planeNum;
			tstack_p->nodeNum = node->children[!side];
			tstack_p++;

			if (tstack_p >= &tracestack[MAX_AAS_TREE_DEPTH]) {
				common->Error("idAASFileLocal::Trace: stack overflow\n");
				return false;
			}

			// now put the part near the start of the line on the stack so we will
			// continue with that part first.
			tstack_p->start = cur_start;
			tstack_p->end = cur_mid;
			tstack_p->planeNum = tmpPlaneNum;
			tstack_p->nodeNum = node->children[side];
			tstack_p++;

			if (tstack_p >= &tracestack[MAX_AAS_TREE_DEPTH]) {
				common->Error("idAASFileLocal::Trace: stack overflow\n");
				return false;
			}
		}
	}

	return false;
#endif
}

/*
============
idAASLocal::AreaContentsTravelFlags
============
*/
int idAASFileLocal::AreaContentsTravelFlags(int areaNum) const
{
	if (areas[areaNum].contents & AREACONTENTS_WATER) {
		return TFL_WATER;
	}

	return TFL_AIR;
}

/*
============
idAASFileLocal::MaxTreeDepth_r
============
*/
void idAASFileLocal::MaxTreeDepth_r(int nodeNum, int &depth, int &maxDepth) const
{
	const aasNode_t *node;

	if (nodeNum <= 0) {
		return;
	}

	depth++;

	if (depth > maxDepth) {
		maxDepth = depth;
	}

	node = &nodes[nodeNum];
	MaxTreeDepth_r(node->children[0], depth, maxDepth);
	MaxTreeDepth_r(node->children[1], depth, maxDepth);

	depth--;
}

/*
============
idAASFileLocal::MaxTreeDepth
============
*/
int idAASFileLocal::MaxTreeDepth(void) const
{
	int depth, maxDepth;

	depth = maxDepth = 0;
	MaxTreeDepth_r(1, depth, maxDepth);
	return maxDepth;
}

#ifdef _SPLASHDAMAGE

typedef unsigned int _DWORD;
#define __int16 short
typedef unsigned short _WORD;
typedef unsigned char _BYTE;
#define LODWORD(x) ((DWORD)(*(uint64_t *)&(x)))

bool idAASFileLocal::PushPointIntoArea( int areaNum, idVec3 &point ) const {
  bool ret_v4; // bl
  const aasArea_t *area_v5; // esi
  int edgeIndex_v6; // ecx
  unsigned int edgeIndexAbs_v7; // edx
  const aasEdge_t *edge_list; // eax
  bool b_v9; // zf
  const aasEdge_t *edge_v10; // eax
  const idVec3 *vertex_v11; // edx
  const idVec3 *edgeVertexLT0_p_x; // esi
  const idVec3 *edgeVertexGE0_v13; // ebx
  float rightLength_v14; // st7
  //float v15; // st7
  float normalX_v16; // st6
  float normalZ_v17; // st5
  float normalY_v18; // st4
  float normalY_v19; // st5
  float normalZ_v20; // st4
  float normalZ_v21; // st3
  float v22; // st3
  float normalX_v23; // st4
  float normalY_v24; // st6
  float normalX_v25; // rt2
  float normalY_v26; // rtt
  float v27; // st3
  bool result; // al
  float v29; // [esp+8h] [ebp-98h]
  float rightLengthSqr_v30; // [esp+8h] [ebp-98h]
  float rightLength_v31; // [esp+8h] [ebp-98h]
  float v32; // [esp+8h] [ebp-98h]
  float rightLengthInv_v33; // [esp+8h] [ebp-98h]
  float dist_v34; // [esp+8h] [ebp-98h]
  char b_v35; // [esp+Fh] [ebp-91h]
  float normalX_v36; // [esp+10h] [ebp-90h]
  float normalY_v37; // [esp+10h] [ebp-90h]
  float normalZ_v38; // [esp+10h] [ebp-90h]
  float v39; // [esp+10h] [ebp-90h]
  float dist_v40; // [esp+10h] [ebp-90h]
  float dist_v41; // [esp+10h] [ebp-90h]
  float dist_v42; // [esp+10h] [ebp-90h]
  char v43; // [esp+17h] [ebp-89h]
	idVec3 rightNormalized_v44; // v44 v45 v48
  //float v44; // [esp+18h] [ebp-88h]
  //float v45; // [esp+1Ch] [ebp-84h]
  float normalY_v46; // [esp+1Ch] [ebp-84h]
  //float v48; // [esp+20h] [ebp-80h]
  float normalZ_v49; // [esp+20h] [ebp-80h]
  float v52 = 0.0f; // [esp+24h] [ebp-7Ch]
  int edgeIndexNum_v53; // [esp+28h] [ebp-78h]
  const aasArea_t *v56; // [esp+38h] [ebp-68h]
  float maxDistance_v57; // [esp+3Ch] [ebp-64h]
	idVec3 right_v58; // v58 v59 v60
	idVec3 edgeLine_v61; // v61 v62 v63
	idVec3 vertexLT0ToPoint_v64; // v64 v65 v66
	idVec3 vertexLT0ToGE0_v67; // v67 v68 v69
	idVec3 v70; // v70 v71 v72
	idVec3 vertexGE0ToPoint_v73; // v73 v74 v75
	idVec3 v78; // v78 v79 v80
  idVec3 lastPoint_v81; // 0:^34.12
  idVec3 originPoint_v82; // 0:^90.12

	idVec3 v16_19_20;

  maxDistance_v57 = 1.0e30f;
  originPoint_v82 = point;
  lastPoint_v81 = point;
  ret_v4 = false;
  area_v5 = &this->areas[areaNum];
  b_v35 = 0;
  v43 = 0;
  v56 = area_v5;
  edgeIndexNum_v53 = 0;
  if ( area_v5->numEdges <= 0 )
    return ret_v4;
  do
  {
    edgeIndex_v6 = this->edgeIndex[edgeIndexNum_v53 + area_v5->firstEdge];
    edgeIndexAbs_v7 = abs(edgeIndex_v6);
    edge_list = this->edges.Ptr();
    b_v9 = (edge_list[edgeIndexAbs_v7].flags & AAS_EDGE_VERTICAL/* 0x40 */) == 0;
    edge_v10 = &edge_list[edgeIndexAbs_v7];
    if ( !b_v9 )
      goto LABEL_53;
    vertex_v11 = this->vertices.Ptr();
    edgeVertexLT0_p_x = &vertex_v11[edge_v10->vertexNum[(unsigned int)edgeIndex_v6 >> 31]];
    v78 = *edgeVertexLT0_p_x - originPoint_v82;
    edgeVertexGE0_v13 = &vertex_v11[edge_v10->vertexNum[edgeIndex_v6 >= 0]];
    v29 = v78.LengthSqr();
    if ( maxDistance_v57 > v29 )
    {
      maxDistance_v57 = v29;
      lastPoint_v81 = *edgeVertexLT0_p_x;
    }
    edgeLine_v61 = *edgeVertexGE0_v13 - *edgeVertexLT0_p_x;
  	right_v58 = edgeLine_v61.Cross(this->settings.invGravityDir);
    rightNormalized_v44 = right_v58;
    rightLengthSqr_v30 = right_v58.LengthSqr();
    rightLength_v31 = sqrt(rightLengthSqr_v30);
    rightLength_v14 = rightLength_v31;
    if ( rightLength_v31 >= 0.0000001f )
    {
      rightLengthInv_v33 = 1.0f / rightLength_v14;
      rightNormalized_v44 = rightLengthInv_v33 * right_v58;
      v32 = rightLength_v14;
      //v15 = 0.0f;
    }
    else
    {
      //v15 = 0.0f;
      v32 = 0.0f;
    }
  	v16_19_20 = rightNormalized_v44;
#if 0
    normalX_v16 = rightNormalized_v44.x;
    normalZ_v17 = rightNormalized_v44.z;
    normalY_v18 = rightNormalized_v44.y;
    if ( rightNormalized_v44.x == 0.0f ) // by x-axis
    {
      if ( rightNormalized_v44.y == 0.0f ) // if y == 0 pure up/down
      {
        normalY_v19 = rightNormalized_v44.y; // 0.0
        normalZ_v20 = rightNormalized_v44.z;
        if ( rightNormalized_v44.z <= 0.0f )
          normalZ_v21 = -1.0f;
        else
          normalZ_v21 = 1.0f;
        if ( normalZ_v21 != normalZ_v20 )
        {
          normalZ_v49 = normalZ_v21;
          normalZ_v20 = normalZ_v49; // -1.0 / 1.0
        }
        goto LABEL_44;
      }
      if ( 0.0f == normalZ_v17 ) // if z == 0 pure forward/backward
      {
        if ( normalY_v18 <= 0.0f )
          normalY_v22 = -1.0f;
        else
          normalY_v22 = 1.0f;
        if ( normalY_v22 != normalY_v18 )
        {
          normalY_v46 = normalY_v22;
          normalY_v18 = normalY_v46;
        }
        goto LABEL_43;
      }
    }
    else if ( rightNormalized_v44.y == 0.0f && 0.0f == normalZ_v17 ) // if y == 0 && z == 0 pure left/right
    {
      normalX_v23 = rightNormalized_v44.x;
      normalY_v24 = rightNormalized_v44.y;
      if ( rightNormalized_v44.x <= 0.0f )
      {
        if ( -1.0f != normalX_v23 )
          normalX_v23 = -1.0f;
      }
      else if ( 1.0f != normalX_v23 )
      {
        normalX_v23 = 1.0f;
      }
      goto LABEL_42;
    }
    normalX_v36 = fabs(normalX_v16);
    if ( 1.0f == normalX_v36 )
    {
      if ( 0.0f == normalY_v18 && 0.0f == normalZ_v17 )
        goto LABEL_43;
      normalZ_v17 = 0.0f; // v50
      normalY_v24 = 0.0f; // v50
      normalX_v23 = rightNormalized_v44.x;
LABEL_42:
      normalX_v25 = normalX_v23;
      normalY_v18 = normalY_v24;
      normalX_v16 = normalX_v25;
LABEL_43:
      normalY_v26 = normalY_v18;
      normalZ_v20 = normalZ_v17;
      normalY_v19 = normalY_v26;
      goto LABEL_44;
    }
    normalY_v37 = fabs(normalY_v18);
    if ( normalY_v37 == 1.0f )
    {
      if ( 0.0f != normalX_v16 || 0.0f != normalZ_v17 )
      {
        normalZ_v17 = 0.0f; // v51
        normalX_v16 = 0.0f; // v51
        normalY_v18 = rightNormalized_v44.y;
      }
      goto LABEL_43;
    }
    normalZ_v38 = fabs(normalZ_v17);
    if ( normalZ_v38 != 1.0f || 0.0f == normalX_v16 && 0.0f == normalY_v18 )
      goto LABEL_43;
    normalY_v19 = 0.0f; // v47
    normalX_v16 = 0.0f; // v47
    normalZ_v20 = rightNormalized_v44.z;
LABEL_44:
#endif
  	//v16_19_20.Set(normalX_v16, normalY_v19, normalZ_v20);
	v16_19_20.FixDegenerateNormal();
    if ( 0.0f != v32 )
    {
      v39 = v16_19_20 * *edgeVertexGE0_v13;
      v52 = -v39;
    }
    dist_v34 = point * v16_19_20 + v52;
    v27 = dist_v34;
    if ( dist_v34 < 0.0f )
    {
      b_v35 = 1;
      v70 = v16_19_20 * v27;
      point = point - v70;
      dist_v34 = 0.0f;
    }
    dist_v40 = fabs(dist_v34);
    if ( dist_v40 < 0.1f )
    {
      vertexLT0ToGE0_v67 = *edgeVertexGE0_v13 - *edgeVertexLT0_p_x;
      vertexLT0ToPoint_v64 = point - *edgeVertexLT0_p_x;
      dist_v41 = vertexLT0ToPoint_v64 * vertexLT0ToGE0_v67;
      if ( dist_v41 >= 0.0f )
      {
        vertexGE0ToPoint_v73 = point - *edgeVertexGE0_v13;
        dist_v42 = vertexLT0ToGE0_v67 * vertexGE0ToPoint_v73;
        if ( 0.0f >= dist_v42 )
          v43 = 1;
      }
    }
    ret_v4 = b_v35;
    area_v5 = v56;
LABEL_53:
    ++edgeIndexNum_v53;
  }
  while ( edgeIndexNum_v53 < area_v5->numEdges );
  if ( !ret_v4 )
    return ret_v4;
  result = ret_v4;
  if ( !v43 )
    point = lastPoint_v81;
  return result;
}

bool idAASFileLocal::TraceHeight( aasTraceHeight_t &trace, const idVec3 &start, const idVec3 &end ) const {
  aasTraceStack_t *tstack_p_v4; // edx
  int nodeNum_v6; // eax
  const aasNode_t *list; // esi
  int v8; // eax
  bool v9; // zf
  const aasNode_t *node_v10; // esi
  int numPoints; // eax
  idVec3 *points; // ebx
  int v13; // eax
  idVec3 *v14; // eax
  int planeNum; // ebx
  const idPlane *plane_v16; // eax
  float front_v20; // st4
  float back_v21; // st2
  int nodeNum_v22; // eax
  int tmpPlaneNum_v23; // ebp
  float v24; // st1
  double frac_v25; // st2
  float zero_v26; // st2
  double frac2_v27; // st1
  double front_v29; // [esp+10h] [ebp-1040h]
  double back_v30; // [esp+10h] [ebp-1040h]
  float v31; // [esp+10h] [ebp-1040h]
  idVec3 cur_end_v32; // v32 v33 v34
  idVec3 v35; // v35 v36 v37
  idVec3 v38; // v38 v39 v40
  idVec3 cur_mid_v41; // v41 v42 v43
  idVec3 cur_start_v44; // v44 v45 v46
  aasTraceStack_t tracestack_v47[MAX_AAS_TREE_DEPTH];
  // float v47[1024]; // [esp+50h] [ebp-1000h] BYREF
  aasTraceStack_t *retaddr = &tracestack_v47[MAX_AAS_TREE_DEPTH]; // (v47 + 1024) - 32; // [esp+1050h] [ebp+0h] BYREF // aasTraceStack_t

	idVec3 cur_start_v17; // v17 v18 v19
	tstack_p_v4 = tracestack_v47;
  trace.numPoints = 0;
  tstack_p_v4->start = start;
  tstack_p_v4->planeNum = 0;
  tstack_p_v4->nodeNum = 1; // LODWORD(v47[7]) = 1;
	tstack_p_v4->end = end;
  while ( 1 )
  {
    nodeNum_v6 = tstack_p_v4->nodeNum;
    if ( !nodeNum_v6 )
      goto LABEL_23;
    list = this->nodes.Ptr();
    v8 = nodeNum_v6;
    v9 = (list[v8].flags & AAS_NODE_FLAG_COLUMN_HEIGHT) == 0; // 2
    node_v10 = &list[v8];
    if ( !v9 )
    {
      numPoints = trace.numPoints;
      if ( numPoints < trace.maxPoints )
      {
        points = trace.points;
        v13 = numPoints;
        points[v13] = tstack_p_v4->start;
        v14 = &points[v13];
        trace.points[trace.numPoints++].z = (float)((node_v10->flags >> AAS_NODE_FLAG_COLUMN_HEIGHT/* 2 */) - AAS_NODE_FLAG_COLUMN_HEIGHT_OFFSET/* 0x2000 */);
      }
      goto LABEL_23;
    }
    planeNum = node_v10->planeNum;
    cur_start_v44 = tstack_p_v4->start;
    plane_v16 = &this->planeList[planeNum];
    cur_end_v32 = tstack_p_v4->end;
    cur_start_v17 = cur_start_v44;
    front_v29 = plane_v16->Distance(cur_start_v44);
    front_v20 = front_v29;
    back_v30 = plane_v16->Distance(cur_end_v32);
    back_v21 = back_v30;
    if ( front_v20 >= -ON_EPSILON && back_v21 >= -ON_EPSILON )
    {
      nodeNum_v22 = node_v10->children[0];
      goto LABEL_22;
    }
    if ( front_v20 < ON_EPSILON && back_v21 < ON_EPSILON )
    {
      nodeNum_v22 = node_v10->children[1];
      goto LABEL_22;
    }
    tmpPlaneNum_v23 = tstack_p_v4->planeNum;
    v24 = front_v20 >= 0.0f ? front_v20 - TRACEPLANE_EPSILON : (front_v20 + TRACEPLANE_EPSILON);
    frac_v25 = v24 / (front_v20 - back_v21);
        zero_v26 = 0.0f;
    if ( frac_v25 >= 0.0f )
    {
      if ( frac_v25 <= 1.0f )
      {
        frac2_v27 = frac_v25;
      }
      else
      {
        frac2_v27 = 0.999f;
      }
    }
    else
    {
      frac2_v27 = 0.001f;
    }
    v35 = cur_end_v32 - cur_start_v17;
    v31 = frac2_v27;
    v38 = v35 * v31;
    cur_mid_v41 = v38 + cur_start_v17;
    tstack_p_v4->planeNum = planeNum;
    tstack_p_v4->start = cur_mid_v41;
    tstack_p_v4->nodeNum = node_v10->children[zero_v26 <= front_v20];
    tstack_p_v4++;
    if ( tstack_p_v4 >= retaddr )
      break;
    nodeNum_v22 = node_v10->children[zero_v26 > front_v20];
    tstack_p_v4->start = cur_start_v44;
    tstack_p_v4->end = cur_mid_v41;
    tstack_p_v4->planeNum = tmpPlaneNum_v23;
LABEL_22:
    tstack_p_v4->nodeNum = nodeNum_v22;
    tstack_p_v4++;
    if ( tstack_p_v4 >= retaddr )
      break;
LABEL_23:
    tstack_p_v4--;
    if ( tstack_p_v4 < tracestack_v47 )
      return true;
  }
  common->Warning("idAASFileLocal::Trace: stack overflow");
  return false;
}


#if 0
/**
 * SplitFloorWinding - 分割地板多边形
 *
 * @param areaNum      - 区域编号
 * @param plane        - 分割平面
 * @param dists        - 输出：各顶点到平面的距离
 * @param sides        - 输出：各顶点在平面的哪一侧
 * @return             - 如果多边形被分割返回 true，否则返回 false
 *
 * 功能：计算地板区域的所有顶点到指定平面的距离，并确定每个顶点在平面的哪一侧。
 *       如果所有顶点都在平面的同一侧，则多边形不被分割。
 */
bool idAASFileLocal::SplitFloorWinding(int areaNum, const idPlane* plane,
                                         float* dists, int* sides) const {
    const aasArea_t* area = &areas[areaNum];

    // 如果区域没有边，直接返回 false
    if (area->numEdges <= 0) {
        return false;
    }

    int allFront = 1;  // 所有顶点都在正面
    int allBack = 0;   // 所有顶点都在背面
    int edgeCount = 0;

    // 遍历区域的所有边
    for (int i = 0; i < area->numEdges; i++) {
        // 获取边索引（可能为负数，表示边的方向）
        int edgeIndex = this->edgeIndex[i + area->firstEdge];
        bool reverse = edgeIndex < 0;

        // 获取边的起点顶点（根据边方向）
        const aasEdge_t* edge = &edges[abs(edgeIndex)];
        const idVec3* vertex = &vertices[edge->vertexNum[reverse ? 1 : 0]];

        // 计算顶点到平面的距离
        // 平面方程：ax + by + cz + d = 0
        dists[i] = plane->operator[](0) * vertex->x + plane->operator[](1) * vertex->y +
                   plane->operator[](2) * vertex->z + plane->operator[](3);

        // 判断顶点在平面的哪一侧
        // 负数表示背面，非负数表示正面
        int side = dists[i] < 0.0f ? 1 : 0;

        sides[i] = side;
        allFront &= side;   // 如果有任何顶点不在正面，allFront 变为 0
        allBack |= side;    // 如果有任何顶点在背面，allBack 变为 1
    }

    // 闭合多边形（最后一个顶点与第一个顶点相同）
    dists[area->numEdges] = dists[0];
    sides[area->numEdges] = sides[0];

    // 如果 allFront != allBack，说明多边形被平面分割
    return allFront != allBack;
}

/**
 * GetFloorEdgeSplitPoints - 获取地板边的分割点
 *
 * @param minSplit     - 输出：距离参考平面最近的分割点
 * @param maxSplit     - 输出：距离参考平面最远的分割点
 * @param areaNum      - 区域编号
 * @param splitPlane   - 分割平面
 * @param refPlane     - 参考平面（用于计算距离）
 * @return             - 如果找到分割点返回 true，否则返回 false
 *
 * 功能：找到分割平面与地板区域边界的交点中，距离参考平面最近和最远的点。
 *       这用于在导航网格中找到合适的路径点。
 */
bool idAASFileLocal::GetFloorEdgeSplitPoints(floorEdgeSplitPoint_t* minSplit,
                                              floorEdgeSplitPoint_t* maxSplit,
                                              int areaNum, const idPlane* splitPlane,
                                              const idPlane* refPlane) const {
    // 初始化分割点
    minSplit->point.Zero();
    minSplit->edgeIndex = 0;
    minSplit->distance = 1.0e30f;  // 初始化为很大的值

    maxSplit->point.Zero();
    maxSplit->edgeIndex = 0;
    maxSplit->distance = -1.0e30f; // 初始化为很小的值

    const aasArea_t* area = &areas[areaNum];

    // 临时缓冲区大小
    int bufferSize = area->numEdges * 4 + 19;

    // 分配临时缓冲区用于存储距离和边信息
    float* dists = (float*)_alloca(bufferSize);
    int* sides = (int*)_alloca(bufferSize);

    // 首先尝试分割地板多边形
    bool wasSplit = SplitFloorWinding(areaNum, splitPlane, dists, sides);

    if (wasSplit) {
        // 遍历所有边，找到穿越平面的边
        for (int i = 0; i < area->numEdges; i++) {
            // 检查当前边的两个顶点是否在平面的不同侧
            if (sides[i] != sides[i + 1]) {
                // 获取边索引
                int edgeIdx = edgeIndex[i + area->firstEdge];
                bool reverse = edgeIdx < 0;

                // 获取边的两个顶点
                const aasEdge_t* edge = &edges[abs(edgeIdx)];
                const idVec3* v1 = &vertices[edge->vertexNum[reverse ? 1 : 0]];
                const idVec3* v2 = &vertices[edge->vertexNum[reverse ? 0 : 1]];

                // 计算边的方向向量
                idVec3 edgeDir = *v2 - *v1;

                // 计算分割比例（使用线性插值）
                // dists[i] 是 v1 到平面的距离，dists[i+1] 是 v2 到平面的距离
                float t = dists[i] / (dists[i] - dists[i + 1]);

                // 计算分割点坐标
                idVec3 splitPoint = *v1 + edgeDir * t;

                // 计算分割点到参考平面的距离
                float refDist = refPlane->operator[](0) * splitPoint.x +
                               refPlane->operator[](1) * splitPoint.y +
                               refPlane->operator[](2) * splitPoint.z +
                               refPlane->operator[](3);

                // 更新最近分割点
                if (refDist < minSplit->distance) {
                    minSplit->distance = refDist;
                    minSplit->edgeIndex = edgeIdx;
                    minSplit->point = splitPoint;
                }

                // 更新最远分割点
                if (refDist > maxSplit->distance) {
                    maxSplit->distance = refDist;
                    maxSplit->edgeIndex = edgeIdx;
                    maxSplit->point = splitPoint;
                }
            }
        }

        return true;
    }

    return wasSplit;
}
#endif

#if 0
/**
 * TraceFloor - 地板追踪函数
 *
 * @param trace         - 输出：追踪结果
 * @param start         - 起始位置
 * @param startAreaNum  - 起始区域编号
 * @param end           - 目标位置
 * @param endAreaNum    - 目标区域编号
 * @param travelFlags   - 通行标志（过滤可达性）
 * @return              - 如果追踪成功返回 true，否则返回 false
 *
 * 功能：在导航网格中沿着地板追踪一个点从起始位置到目标位置的移动。
 *       使用两个平面（垂直于重力方向和运动方向）来约束追踪路径。
 *       通过可达性链表遍历相邻区域，处理台阶高度限制。
 */
bool idAASFileLocal::TraceFloor(aasTraceFloor_t& trace, const idVec3& start,
                                  int startAreaNum, const idVec3& end,
                                  int endAreaNum, int travelFlags) const {
    // 初始化追踪结果
    trace.fraction = 0.0f;
    trace.endpos = start;
    trace.lastAreaNum = startAreaNum;
    trace.lastEdgeNum = 0;

	idList<aasArea_t> &areas = (idList<aasArea_t> &)this->areas;
	idList<int> &floorIndex = (idList<int> &)this->floorIndex;
    // 计算运动方向向量
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float dz = end.z - start.z;

    // 计算垂直于重力方向和运动方向的平面（分割平面）
    idPlane splitPlane;
    splitPlane[0] = dy * settings.gravityDir.z - dz * settings.gravityDir.y;
    splitPlane[1] = dz * settings.gravityDir.x - dx * settings.gravityDir.z;
    splitPlane[2] = dx * settings.gravityDir.y - dy * settings.gravityDir.x;

    // 归一化平面法向量
    float lenSq = splitPlane[0] * splitPlane[0] +
                  splitPlane[1] * splitPlane[1] +
                  splitPlane[2] * splitPlane[2];
    float len = sqrt(lenSq);
    if (len >= 1e-7f) {
        float invLen = 1.0f / len;
        splitPlane[0] *= invLen;
        splitPlane[1] *= invLen;
        splitPlane[2] *= invLen;
    }

    // 简化平面法向量为轴向对齐（使用 idlib 的标准函数）
    splitPlane.FixDegenerateNormal();

    // 设置平面的 d 值（平面经过起始点）
    splitPlane[3] = -(splitPlane[0] * start.x +
                     splitPlane[1] * start.y +
                     splitPlane[2] * start.z);

    // 计算参考平面（垂直于分割平面和重力方向）
    idPlane refPlane;
    refPlane[0] = splitPlane[2] * settings.gravityDir.y - settings.gravityDir.z * splitPlane[1];
    refPlane[1] = settings.gravityDir.z * splitPlane[0] - splitPlane[2] * settings.gravityDir.x;
    refPlane[2] = splitPlane[1] * settings.gravityDir.x - splitPlane[0] * settings.gravityDir.y;

    // 归一化参考平面法向量
    lenSq = refPlane[0] * refPlane[0] +
            refPlane[1] * refPlane[1] +
            refPlane[2] * refPlane[2];
    len = sqrt(lenSq);
    if (len >= 1e-7f) {
        float invLen = 1.0f / len;
        refPlane[0] *= invLen;
        refPlane[1] *= invLen;
        refPlane[2] *= invLen;
    }

    // 简化参考平面法向量为轴向对齐（使用 idlib 的标准函数）
    refPlane.FixDegenerateNormal();

    // 设置参考平面的 d 值（平面经过起始点）
    refPlane[3] = -(refPlane[0] * start.x +
                   refPlane[1] * start.y +
                   refPlane[2] * start.z);

    // 计算目标点到参考平面的距离（用于判断是否到达目标）
    float endDist = -(refPlane[0] * end.x + refPlane[1] * end.y +
                    refPlane[2] * end.z + refPlane[3]);

    // 声明分割点变量
    floorEdgeSplitPoint_t minSplit, maxSplit;

    // 获取起始区域的分割点
    bool hasSplit = GetFloorEdgeSplitPoints(&minSplit, &maxSplit, startAreaNum,
                                             &splitPlane, &refPlane);

    if (!hasSplit) {
        // 如果区域没有被分割，使用起始点作为分割点
        minSplit.point = start;
        minSplit.edgeIndex = 0;
        minSplit.distance = 0.0f;
        maxSplit = minSplit;
    }

    // 清除之前的搜索标记
    for (int i = 0; i < floorIndex.Num(); ++i) {
        areas[floorIndex[i]].flags &= ~0x8000u;
    }
    floorIndex.Clear();

    // 追踪主循环
    int currentAreaNum = startAreaNum;

    while (true) {
        // 将当前区域加入搜索列表
        floorIndex.Append(currentAreaNum);
        areas[currentAreaNum].flags |= 0x8000u;

        // 更新追踪结果位置
        trace.endpos = maxSplit.point;
        trace.lastEdgeNum = maxSplit.edgeIndex;

        // 检查是否到达目标区域
        if (currentAreaNum == endAreaNum) {
            break;
        }

        // 检查是否超过目标距离
        float currentDist = refPlane[0] * trace.endpos.x +
                           refPlane[1] * trace.endpos.y +
                           refPlane[2] * trace.endpos.z + endDist;
        if (currentDist > 0.1f) {
            break;
        }

        currentDist = refPlane[0] * trace.endpos.x +
                           refPlane[1] * trace.endpos.y +
                           refPlane[2] * trace.endpos.z;
		refPlane[3] = -currentDist;

        // 遍历当前区域的可达性
        aasReachability_t* reach = (aasReachability_t*)areas[currentAreaNum].reach;
        int searchStart = floorIndex.Num();
        bool foundNextArea = false;

        while (reach != nullptr) {
            // 检查通行标志是否匹配
            if ((reach->travelFlags & ~travelFlags) == 0) {
                int nextAreaNum = reach->toAreaNum;
                aasArea_t* nextArea = &areas[nextAreaNum];

                // 检查目标区域的通行标志和是否已访问
                if ((nextArea->travelFlags & ~travelFlags) == 0 &&
                    (nextArea->flags & 0x8000u) == 0) {
                    // 获取下一个区域的分割点
                    floorIndex.Append(nextAreaNum);
                    nextArea->flags |= 0x8000u;

                    hasSplit = GetFloorEdgeSplitPoints(&minSplit, &maxSplit,
                                                       nextAreaNum, &splitPlane, &refPlane);

                    // 检查分割点是否有效
                    if (minSplit.distance < 1.0e30f && maxSplit.distance >= 0.1f) {
                        // 计算步长
                        float stepX = trace.endpos.x - minSplit.point.x;
                        float stepY = trace.endpos.y - minSplit.point.y;
                        float stepZ = trace.endpos.z - minSplit.point.z;

                        // 微小值处理
                        if (fabs(stepX) < 1e-7f) stepX = 0.0f;
                        if (fabs(stepY) < 1e-7f) stepY = 0.0f;
                        if (fabs(stepZ) < 1e-7f) stepZ = 0.0f;

                        // 计算沿重力方向的分量
                        float gravityDot = stepX * settings.gravityDir.x +
                                          stepY * settings.gravityDir.y +
                                          stepZ * settings.gravityDir.z;

                        float gravityX = gravityDot * settings.gravityDir.x;
                        float gravityY = gravityDot * settings.gravityDir.y;
                        float gravityZ = gravityDot * settings.gravityDir.z;

                        // 计算重力分量的长度平方
                        float gravityLenSq = gravityX * gravityX +
                                             gravityY * gravityY +
                                             gravityZ * gravityZ;

                        // 检查是否超过最大台阶高度
                        float maxStepSq = settings.maxStepHeight * settings.maxStepHeight;
                        if (maxStepSq >= gravityLenSq) {
                            // 计算水平分量
                            float horzX = stepX - gravityX;
                            float horzY = stepY - gravityY;
                            float horzZ = stepZ - gravityZ;
                            float horzLenSq = horzX * horzX +
                                              horzY * horzY +
                                              horzZ * horzZ;

                            // 检查水平距离是否在允许范围内
                            if (horzLenSq <= 0.04f) {
                                // 找到可行的下一步
                                currentAreaNum = nextAreaNum;
                                foundNextArea = true;
                                break;
                            }
                        }
                    }
                }
            }

            // 移动到下一个可达性
            reach = reach->next;
        }

        if (!foundNextArea) {
            // 无法继续追踪，计算完成比例
            float distToStartSq = (trace.endpos.x - start.x) * (trace.endpos.x - start.x) +
                                 (trace.endpos.y - start.y) * (trace.endpos.y - start.y) +
                                 (trace.endpos.z - start.z) * (trace.endpos.z - start.z);
            float totalDistSq = dx * dx + dy * dy + dz * dz;

            trace.fraction = sqrt(distToStartSq / totalDistSq);
            break;
        }
    }

    // 如果到达了目标区域，设置完成比例为 1.0
    if (currentAreaNum == endAreaNum) {
        trace.endpos = end;
        trace.fraction = 1.0f;
    }

    // 更新最后到达的区域
    trace.lastAreaNum = currentAreaNum;

    // 清除搜索标记
    for (int i = 0; i < floorIndex.Num(); ++i) {
        areas[floorIndex[i]].flags &= ~0x8000u;
    }
    floorIndex.Clear();

    return true;
}
#else
bool aaa;
bool idAASFileLocal::TraceFloor( aasTraceFloor_t &trace, const idVec3 &start, int startAreaNum, const idVec3 &end, int endAreaNum, int travelFlags ) const
{
  struct aasTraceFloor_t *v7; // ebp
  float a; // st6
  float v10; // st5
  float c; // st3
  float b; // st2
  float v13; // st7
  float v14; // st5
  float v15; // st7
  float v16; // st5
  float v17; // st4
  float v18; // st6
  float v19; // rt0
  float v20; // st4
  float v21; // st6
  float v22; // st5
  float v23; // st4
  float v24; // st5
  float v25; // st7
  float v26; // st4
  float v27; // st7
  float v28; // st4
  float v29; // st4
  float v30; // st6
  float v31; // rtt
  float v32; // st4
  float v33; // st3
  float x; // st7
  int i; // eax
  int v36; // edx
  int granularity; // eax
  bool v38; // cc
  int num; // ebx
  int size; // eax
  int v41; // eax
  int *list; // edi
  int j; // eax
  float v44; // st7
  int v45; // edi
  int v46; // eax
  float v47; // st4
	const aasReachability_t *v48;
  //float v48; // ecx
  float v49; // st5
  float v50; // st7
  int v51; // ebx
  const aasArea_t *v52; // eax
  int v53; // eax
  int v54; // ebp
  int v55; // eax
  int v56; // eax
  int *v57; // edi
  int k; // eax
  float v59; // st7
  float v60; // st5
  int v61; // ebp
  int m; // eax
  int v63; // edx
  float v64; // st7
  float v65; // st7
  int *v66; // edi
  int n; // eax
  //float v68; // eax
  const aasReachability_t *v68; // eax
  int ii; // eax
  int v70; // edx
  float v72; // [esp+8h] [ebp-84h]
  float v73; // [esp+8h] [ebp-84h]
  float v74; // [esp+8h] [ebp-84h]
  float v75; // [esp+8h] [ebp-84h]
  float v76; // [esp+8h] [ebp-84h]
  float v77; // [esp+8h] [ebp-84h]
  float v78; // [esp+8h] [ebp-84h]
  float v79; // [esp+8h] [ebp-84h]
  float v80; // [esp+8h] [ebp-84h]
  float v81; // [esp+8h] [ebp-84h]
  float v82; // [esp+8h] [ebp-84h]
  float v83; // [esp+8h] [ebp-84h]
  float v84; // [esp+8h] [ebp-84h]
  float v85; // [esp+8h] [ebp-84h]
  float v86; // [esp+8h] [ebp-84h]
  float v87; // [esp+Ch] [ebp-80h]
  float v88; // [esp+Ch] [ebp-80h]
  float v89; // [esp+Ch] [ebp-80h]
  float v90; // [esp+Ch] [ebp-80h]
  float v91; // [esp+Ch] [ebp-80h]
  float v92; // [esp+Ch] [ebp-80h]
  float v93; // [esp+Ch] [ebp-80h]
  float v94; // [esp+10h] [ebp-7Ch]
	idVec3 v95; // v95 v100 v105
  //float v95; // [esp+10h] [ebp-7Ch]
	idVec3 v96; // v96 v101 v106
  //float v96; // [esp+10h] [ebp-7Ch]
	idVec3 v97; // v97 v102 v107
  //float v97; // [esp+10h] [ebp-7Ch]
  float v98; // [esp+10h] [ebp-7Ch]
  float v99; // [esp+14h] [ebp-78h]
  float v103; // [esp+14h] [ebp-78h]
  float v104; // [esp+18h] [ebp-74h]
  float v108; // [esp+18h] [ebp-74h]
  idPlane v109; // [esp+1Ch] [ebp-70h] BYREF // point to forward
  int v110; // [esp+2Ch] [ebp-60h]
  idPlane v111; // [esp+30h] [ebp-5Ch] BYREF // point to left
  float v112; // [esp+40h] [ebp-4Ch]
	idVec3 v113; // v113 v114 v115
  int v116; // [esp+50h] [ebp-3Ch]
	floorEdgeSplitPoint_t v117; // v117 y z v120 v121
	floorEdgeSplitPoint_t v122; // v122 v123 v124 v125 v126
  float v127; // [esp+88h] [ebp-4h]

	idList<aasArea_t> &areas = (idList<aasArea_t> &)this->areas;
	idList<int> &floorIndex = (idList<int> &)this->floorIndex;

  v7 = &trace;
  trace.fraction = 0.0f;
  trace.endpos = start;
  trace.lastAreaNum = startAreaNum;
  trace.lastEdgeNum = 0;
  v113 = end - start;
	idVec3 normal = v113.Cross(this->settings.gravityDir); // left
	v111.SetNormal(normal);
	v111.Normalize(true);
  a = v111[0];
  v10 = -1.0f;
  c = v111[2];
  b = v111[1];
#if 0
  if ( v111[0] != 0.0f )
  {
    if ( v111[1] == 0.0f && 0.0f == c )
    {
      v15 = v111[1];
      if ( a <= 0.0f )
      {
        v17 = v111[2];
        if ( -1.0f == a )
        {
LABEL_43:
          v16 = v17;
          goto LABEL_23;
        }
        v18 = v111[2];
      }
      else
      {
        v16 = v111[2];
        if ( 1.0f == a )
          goto LABEL_23;
        v10 = 1.0f;
        v18 = v111[2];
      }
      v111[0] = v10;
      goto LABEL_22;
    }
LABEL_28:
    v14 = v111[1];
    v79 = fabs(a);
    if ( v79 == 1.0f )
    {
      v20 = v111[2];
      if ( 0.0f != v14 || 0.0f != v20 )
      {
        v111[2] = 0.0f;
        v111[1] = 0.0f;
        v13 = 0.0f;
        v14 = v13;
        goto LABEL_24;
      }
      goto LABEL_44;
    }
    v80 = fabs(v14);
    if ( v80 != 1.0f )
    {
      v81 = fabs(c);
      v20 = v111[2];
      if ( v81 == 1.0f && (0.0f != a || 0.0f != v14) )
      {
        v13 = v111[2];
        v111[1] = 0.0f;
        v111[0] = 0.0f;
        v14 = 0.0f;
        a = v14;
        goto LABEL_24;
      }
      goto LABEL_44;
    }
    v20 = v111[2];
    if ( 0.0f == a && 0.0f == v20 )
    {
LABEL_44:
      v13 = v20;
      goto LABEL_24;
    }
    v15 = v111[1];
    v111[2] = 0.0f;
    v111[0] = 0.0f;
    v18 = 0.0f;
LABEL_22:
    v16 = v18;
    a = v111[0];
    goto LABEL_23;
  }
  if ( v111[1] != 0.0f )
  {
    if ( 0.0f == c )
    {
      v15 = v111[1];
      if ( b > 0.0f )
      {
        v16 = v111[2];
        if ( 1.0f != b )
        {
          v13 = v111[2];
          v111[1] = 1.0f;
          v14 = 1.0f;
          goto LABEL_24;
        }
        goto LABEL_23;
      }
      v17 = v111[2];
      if ( -1.0f != b )
      {
        v13 = v111[2];
        v111[1] = -1.0f;
        v14 = -1.0f;
        goto LABEL_24;
      }
      goto LABEL_43;
    }
    goto LABEL_28;
  }
  v13 = v111[2];
  if ( v111[2] <= 0.0f )
  {
    if ( -1.0f != v13 )
    {
      v15 = v111[1];
      v111[2] = -1.0f;
      v16 = -1.0f;
      goto LABEL_23;
    }
    v14 = v111[1];
  }
  else
  {
    v14 = v111[1];
    if ( 1.0f != v13 )
    {
      v15 = v111[1];
      v111[2] = 1.0f;
      v16 = 1.0f;
LABEL_23:
      v19 = v16;
      v14 = v15;
      v13 = v19;
    }
  }
LABEL_24:
  v75 = v14 * start.y + start.x * a + v13 * start.z;
  v111[3] = -v75;
  v95.x = v13 * this->settings.gravityDir.y - this->settings.gravityDir.z * v14;
  v95.y = this->settings.gravityDir.z * a - v13 * this->settings.gravityDir.x;
  v95.z = v14 * this->settings.gravityDir.x - a * this->settings.gravityDir.y;
#else
  v14 = v111[1];
  v13 = v111[2];
  a = v111[0];
  v111.FitThroughPoint(start);
  v95 = this->settings.gravityDir.Cross(v111.Normal()); // point to start
#endif
  v109.SetNormal(v95);
	v109.Normalize(true);
#if 0
  v21 = v109[0];
  v22 = v109[2];
  v23 = v109[1];
  if ( v109[0] != 0.0f )
  {
    if ( v109[1] == 0.0f && 0.0f == v22 )
    {
      v27 = v109[1];
      if ( v21 <= 0.0f )
        v29 = -1.0f;
      else
        v29 = 1.0f;
      if ( v29 == v21 )
        goto LABEL_66;
      v30 = v109[2];
      v109[0] = v29;
      goto LABEL_65;
    }
LABEL_77:
    v84 = fabs(v21);
    if ( 1.0f == v84 )
    {
      if ( 0.0f != v23 || 0.0f != v22 )
      {
        v109[2] = 0.0f;
        v109[1] = 0.0f;
        v25 = 0.0f;
        v24 = v25;
        goto LABEL_67;
      }
      goto LABEL_91;
    }
    v85 = fabs(v23);
    if ( v85 == 1.0f )
    {
      if ( 0.0f != v21 || 0.0f != v22 )
      {
        v27 = v109[1];
        v109[2] = 0.0f;
        v109[0] = 0.0f;
        v30 = 0.0f;
LABEL_65:
        v22 = v30;
        v21 = v109[0];
LABEL_66:
        v31 = v22;
        v24 = v27;
        v25 = v31;
        goto LABEL_67;
      }
    }
    else
    {
      v86 = fabs(v22);
      if ( v86 == 1.0f && (0.0f != v21 || 0.0f != v23) )
      {
        v25 = v109[2];
        v109[1] = 0.0f;
        v109[0] = 0.0f;
        v24 = 0.0f;
        v21 = v24;
        goto LABEL_67;
      }
    }
LABEL_91:
    v27 = v109[1];
    goto LABEL_66;
  }
  if ( v109[1] == 0.0f )
  {
    v24 = v109[1];
    v25 = v109[2];
    if ( v109[2] <= 0.0f )
      v26 = -1.0f;
    else
      v26 = 1.0f;
    if ( v26 == v25 )
      goto LABEL_67;
    v27 = v109[1];
    v109[2] = v26;
    v22 = v109[2];
    goto LABEL_66;
  }
  if ( 0.0f != v22 )
    goto LABEL_77;
  v27 = v109[1];
  if ( v23 <= 0.0f )
    v28 = -1.0f;
  else
    v28 = 1.0f;
  if ( v28 == v27 )
    goto LABEL_66;
  v25 = v109[2];
  v109[1] = v28;
  v24 = v109[1];
LABEL_67:
  v32 = v24 * start.y;
  v33 = start.x * v21;
  v116 = startAreaNum;
  v82 = v32 + v33 + v25 * start.z;
  v109[3] = -v82;
#else
  v24 = v109[1];
  v21 = v109[0];
  v25 = v109[2];
  v116 = startAreaNum;
  v109.FitThroughPoint(start);
#endif
  v83 = v25 * end.z + v21 * end.x + v24 * end.y;
  v127 = -v83;
			ID_IF_DBG()
				aaa=1;
  if ( !idAASFileLocal::GetFloorEdgeSplitPoints(
          (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v122,
          (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v117,
          startAreaNum,
          &v111,
          &v109) )
  {
    x = start.x;
    v117.edgeIndex = 0;
    v117.point = start;
    v122.edgeIndex = 0;
    v122.point = v117.point;
    v117.distance = 0.0f;
    v122.distance = 0.0f;
  }
  else
  {
			ID_IF_DBG()
			{
				//printf("Left: %s|Forward: %s\n", v111.Normal().ToString(), v109.Normal().ToString());
				session->rw->DebugArrow(colorBlue, start, start+v111.Normal() * 1000, 10);
				session->rw->DebugArrow(colorOrange, start, start+v109.Normal() * 1000, 10);
				//session->rw->DebugAxis(v109.Normal() * -v109[3], v109.Normal().ToAngles().ToMat3());
			session->rw->DrawText( va( "A: %d", startAreaNum ), v122.point, 1.0f, colorRed, mat3_identity );
			session->rw->DebugCircle(colorRed, v122.point, idVec3( 0, 0, 1 ), 30.0f, 8);
			session->rw->DrawText( va( "B: %d", startAreaNum ), v117.point, 1.0f, colorRed, mat3_identity );
			session->rw->DebugCircle(colorRed, v117.point, idVec3( 0, 0, 1 ), 30.0f, 8);
			session->rw->DebugArrow( colorRed, v122.point, v117.point, 5 );
			}
  }
  // clear last search area bit
  for ( i = 0; i < this->floorIndex.Num(); ++i )
  {
    v36 = this->floorIndex[i];
    areas[v36].flags &= ~AAS_AREA_FLOOD_VISITED /* 0x8000u */;
  }
  floorIndex.Clear();
  while ( 1 )
  {
    v44 = v117.point[0];
    v45 = v116;
    floorIndex.Append(v116);
    areas[v45].flags |= AAS_AREA_FLOOD_VISITED /* 0x8000u */;
    v7->endpos = v117.point;
    v46 = v117.edgeIndex;
    v7->lastEdgeNum = v46;
    if ( v45 == endAreaNum )
      break;
    v112 = v109.Normal() * v7->endpos + v127;
    if ( v112 > 0.1f )
      break;
    v47 = v7->endpos.y;
    v48 = this->areas[v45].reach;
    v49 = v109[0] * v7->endpos.x;
    v110 = this->floorIndex.Num();
  	v112 = v109.Normal() * v7->endpos;
    v50 = v112;
    const aasReachability_t *_v112 = v48;
    v109[3] = -v50;
    if ( v48 == NULL )
    {
LABEL_150:
    	v113 = v7->endpos - start;
    	idVec3 _v98 = end - start;
      float _v110 = v113.LengthSqr();
      v64 = _v110;
      _v110 = _v98.LengthSqr();
      _v110 = v64 / _v110;
      _v110 = sqrt(_v110);
      v65 = _v110;
      goto LABEL_159;
    }
    while ( 1 )
    {
      if ( ( v48->travelFlags & ~travelFlags) == 0 )
      {
        v51 = v48->toAreaNum; // + 6
        v52 = &this->areas[v51];
        if ( (v52->travelFlags & ~travelFlags) == 0 && (v52->flags & AAS_AREA_FLOOD_VISITED /* 0x8000u */) == 0 )
        {
          floorIndex.Append(v51);
          areas[v51].flags |= AAS_AREA_FLOOD_VISITED/* 0x8000u */;
          idAASFileLocal::GetFloorEdgeSplitPoints(
            (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v122,
            (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v117,
            _v112->toAreaNum,
            &v111,
            &v109);
          if ( v122.distance < 1.0e30f && v117.distance >= 0.1f )
          {
            v96 = trace.endpos - v122.point;
            v87 = fabs(v96.x);
            if ( v87 < 0.0000001f )
              v96.x = 0.0f;
            v88 = fabs(v96.y);
            if ( v88 < 0.0000001f )
              v96.y = 0.0f;
            v89 = fabs(v96.z);
            v59 = v96.z;
            if ( v89 < 0.0000001f ) {
            	v59 = 0.0f;
            	v96.z = v59;
            }
#if 0
			ID_IF_DBG()
			{
			session->rw->DrawText( va( "X: %d", _v112->toAreaNum ), v122.point, 1.0f, colorBlue, mat3_identity );
			session->rw->DebugCircle(colorBlue, v122.point, idVec3( 0, 0, 1 ), 35.0f, 8);
			session->rw->DrawText( va( "Y: %d", _v112->toAreaNum ), v117.point, 1.0f, colorBlue, mat3_identity );
			session->rw->DebugCircle(colorBlue, v117.point, idVec3( 0, 0, 1 ), 35.0f, 8);
			session->rw->DebugArrow( colorBlue, v122.point, v122.point+v96, 5 );
			}
#endif
            v90 = this->settings.gravityDir * v96;
            v113 = v90 * this->settings.gravityDir;
            v91 = v113.LengthSqr();
            v60 = v91;
            v92 = this->settings.maxStepHeight * this->settings.maxStepHeight;
            if ( v92 >= v60 )
            {
              v97 = v96 - v113;
              v93 = v97.LengthSqr();
              if ( v93 <= 0.04f )
                break;
            }
          }
        }
      }
      _v112 = _v112->next;
      v48 = _v112;
      if ( _v112 == NULL )
      {
        v45 = v116;
        v7 = &trace;
        goto LABEL_150;
      }
    }
    v61 = v110;
    for ( m = v110; m < this->floorIndex.Num(); ++m )
    {
      v63 = this->floorIndex[m];
      areas[v63].flags &= ~AAS_AREA_FLOOD_VISITED/* 0x8000u */;
    }
    v68 = _v112;
    floorIndex.SetNum(v61);
    v7 = &trace;
    v116 = v68->toAreaNum;
  }
  v7->endpos = end;
  v65 = 1.0f;
LABEL_159:
  v7->lastAreaNum = v45;
  v7->fraction = v65;
  for ( ii = 0; ii < this->floorIndex.Num(); ++ii )
  {
    v70 = this->floorIndex[ii];
    areas[v70].flags &= ~AAS_AREA_FLOOD_VISITED/* 0x8000u */;
  }
  floorIndex.Clear();
  return true;
}
#endif

#if 1
bool idAASFileLocal::SplitFloorWinding(
		int areaNum_a2,
		const idPlane *plane_a3,
		float *retDists_a4,
		int *retSigns_a5) const
{
	const aasArea_t *area_list; // eax
	int i_v6; // ebp
	bool noEdges_v7; // cc
	const aasArea_t *area_v8; // eax
	float *distPtr_v9; // edi
	int edgeIndex_v10; // esi
	const idVec3 *vertex_v11; // eax
	int sign_v12; // eax
	int v14; // [esp+4h] [ebp-8h]
	int v15; // [esp+8h] [ebp-4h]
	const aasArea_t *area_v16; // [esp+10h] [ebp+4h]
	int *signPtr = retSigns_a5;

	area_list = this->areas.Ptr();
	i_v6 = 0;
	noEdges_v7 = area_list[areaNum_a2].numEdges <= 0;
	area_v8 = &area_list[areaNum_a2];
	v15 = 1;
	v14 = 0;
	area_v16 = area_v8;
	if ( !noEdges_v7 )
	{
		distPtr_v9 = retDists_a4;
		do
		{
			edgeIndex_v10 = this->edgeIndex[i_v6 + area_v8->firstEdge];
			vertex_v11 = &this->vertices[this->edges[abs(edgeIndex_v10)].vertexNum[edgeIndex_v10 >> 31]];
			/*
			if(ID_IS_DBG() && aaa)
			{
			session->rw->DrawText( va( "%d / %d", i_v6 ,area_v16->numEdges), *vertex_v11+idVec3(0,0,50+i_v6*5), 1.0f, colorBlue, mat3_identity );
			session->rw->DebugCircle(colorBlue, *vertex_v11, idVec3( 0, 0, 1 ), 25.0f, 3);
			}
			*/
			++i_v6;
			float d = plane_a3->Distance(*vertex_v11);
			*distPtr_v9++ = d; // retDists_a4 += 1
			sign_v12 = FLOATSIGNBITSET(d); // check distance less than 0; sign_v12 = 1 if distance < 0 // *((_DWORD *)distPtr_v9 - 1) >> 31;
			v15 &= sign_v12;
			v14 |= sign_v12;
			*signPtr++ = sign_v12; // retSigns_a5 += 1 // *(_DWORD *)((char *)distPtr_v9 + ((char *)retSigns_a5 - (char *)retDists_a4) - 4) = sign_v12; // retSigns_a5 - retDists_a4 == align16( (1+numEdges) * sizeof(int/float) )
			area_v8 = area_v16;
			//Sys_Printf("EEE %d/%d: %f %d | %d %d\n", i_v6-1,area_v8->numEdges, d,sign_v12,v15, v14);
		}
		while ( i_v6 < area_v16->numEdges );
	}
	retDists_a4[area_v8->numEdges] = *retDists_a4;
	retSigns_a5[area_v8->numEdges] = *retSigns_a5;
	return v15 != v14; // all distance >= 0; v14 == v15 if has distance < 0
}

//----- (005EC1D0) --------------------------------------------------------
bool idAASFileLocal::GetFloorEdgeSplitPoints(
        idAASFileLocal::floorEdgeSplitPoint_t *a2,
        idAASFileLocal::floorEdgeSplitPoint_t *a3,
        int areaNum_a4,
        const idPlane *a5,
        const idPlane *a6) const
{
  float *dists_v8; // esp
  int *sides_v9; // esp
  bool result; // al
  int i_v11; // eax
  float *distPtr_v12; // ebx
  int v13; // ecx
  int edgeIndex_v14; // ecx // edgeIndex
  const aasEdge_t *edge_v15; // edx
  const idVec3 *vertex0_v16; // eax
  const idVec3 *vertex1_v17; // edx
  int v21; // [esp+0h] [ebp-3Ch] BYREF
  _BYTE v22[8]; // [esp+4h] [ebp-38h] BYREF
  idVec3 projVertex_v23; // v23 v24 v25
  idVec3 projLine_v26; // v26 v27 v28
  idVec3 edgeLine_v29; // edgeVec // v29 v30 v31
  int i; // [esp+30h] [ebp-Ch]
  int *sides_v33; // [esp+34h] [ebp-8h]
  const aasArea_t *area_v34; // [esp+38h] [ebp-4h]
  int size_v35; // [esp+4Ch] [ebp+10h]
  int i_v36; // [esp+4Ch] [ebp+10h]
  float *distPtr_v37; // [esp+50h] [ebp+14h]
  float frac_v38; // [esp+50h] [ebp+14h]
  float dist_v39; // [esp+50h] [ebp+14h]

	idVec3 projVertex_v19;
  a2->point = vec3_origin;
  a2->edgeIndex = 0;
  a2->distance = 1.0e30f;
  a3->point = vec3_origin;
  a3->edgeIndex = 0;
  a3->distance = -1.0e30f;
  area_v34 = &this->areas[areaNum_a4];
  size_v35 = 4 * (area_v34->numEdges + 1) + 15;
  dists_v8 = (float *)alloca(size_v35);
  sides_v9 = (int *)alloca(size_v35);
  sides_v33 = &v21;
  result = idAASFileLocal::SplitFloorWinding(areaNum_a4, a5, dists_v8, sides_v9); // (this, areaNum_a4, a5, (float *)&v21, &v21);
  if ( result ) // all distance >= 0
  {
    i_v11 = 0;
    i_v36 = 0;
    if ( area_v34->numEdges > 0 )
    {
      // v13 = (char *)sides_v33 - (char *)&v21; // == sizeof(dists_v8/sides_v9)
		distPtr_v12 = dists_v8 + 1; // float* distance
		sides_v33 = sides_v9; // int* flags
		distPtr_v37 = dists_v8 + 1; // float* distance
		int *sidePtr_v12 = sides_v9 + 1; // int* flags
		idVec3 last=vec3_origin;
      for ( ; ; /*i = (char *)sides_v33 - (char *)&v21; ; v13 = i*/ )
      {
        //if ( sides_v33[i_v11] != *sidePtr_v12 /* *(_DWORD *)((char *)&distPtr_v12->ToFloatPtr()[0] + v13)*/ )
		if(sides_v33[i_v11] != *sidePtr_v12) // cross this edge
        {
          edgeIndex_v14 = this->edgeIndex[i_v11 + area_v34->firstEdge];
          edge_v15 = &this->edges[abs(edgeIndex_v14)];
          vertex0_v16 = &this->vertices[edge_v15->vertexNum[(unsigned int)edgeIndex_v14 >> 31]];
          vertex1_v17 = &this->vertices[edge_v15->vertexNum[edgeIndex_v14 >= 0]];
          distPtr_v12 = distPtr_v37;
          edgeLine_v29 = *vertex1_v17 - *vertex0_v16;
		  float lastDist_v37 = *(distPtr_v37 - 1);
          frac_v38 = lastDist_v37 / (lastDist_v37 - *distPtr_v37);
          projLine_v26 = edgeLine_v29 * frac_v38;
          projVertex_v23 = *vertex0_v16 + projLine_v26;
		  /*
			if(ID_IS_DBG() && aaa)
			{
			session->rw->DrawText( va( "%d", i_v11 ), projVertex_v23, 1.0f, colorGreen, mat3_identity );
			session->rw->DebugCircle(colorGreen, projVertex_v23, idVec3( 0, 0, 1 ), 25.0f, 4);
			if(!last.IsZero())
			session->rw->DebugArrow( colorGreen, last, projVertex_v23, 5 );
			session->rw->DebugLine( colorGreen, *vertex0_v16, *vertex1_v17 );
			last = projVertex_v23;
			aaa=0;
			}
			*/
          projVertex_v19 = projVertex_v23;
          dist_v39 = a6->Distance(projVertex_v23);
          if ( a2->distance > dist_v39 )
          {
            a2->distance = dist_v39;
            a2->edgeIndex = edgeIndex_v14;
            a2->point = projVertex_v19;
          }
          i_v11 = i_v36;
          if ( a3->distance < dist_v39 )
          {
            a3->distance = dist_v39;
            a3->edgeIndex = edgeIndex_v14;
            a3->point = projVertex_v19;
          }
        }
        ++i_v11;
        distPtr_v12++;
      	sidePtr_v12++;
        i_v36 = i_v11;
        distPtr_v37 = distPtr_v12;
        if ( i_v11 >= area_v34->numEdges )
          break;
      }
    }
    return true;
  }
  return result;
}
#endif

float idAASFileLocal::GetFloorDistance(
        int areaNum_a2,
        const idPlane *plane_a3,
        const idVec3 *a4,
        float a5,
        float a6) const
{
  const aasArea_t *area_v6; // edx
  float v8; // st7
  float result; // st7
  const aasEdge_t *edge_list; // ebx
  const idVec3 *vertexes_v11; // esi
  const int *edgeIndex_v12; // edi
  unsigned int edgeIndexAbs_v13; // eax
  int vertexIndex_v14; // edx
  const idVec3 *vertex1_p_x; // eax
  const idVec3 *vertex0_v16; // edx
  float v17; // st7
  float v18; // st5
  float v19; // st6
  float v20; // st2
  float v21; // [esp+4h] [ebp-40h]
	idVec3 v22; // v22 v23 v24
  idVec3 edgeLine_v25; // v25 v26 v27
  idVec3 v28; // v28 v29 v30
  idVec3 v31; // v31 v32 v33
  idVec3 v34(0.0f, 0.0f, 0.0f); // v34 v35 v36
  float dist_v37; // [esp+48h] [ebp+4h]
  float distAbs_v38; // [esp+48h] [ebp+4h]
  float v39; // [esp+48h] [ebp+4h]
  float edgeLineLengthSqr_v40; // [esp+48h] [ebp+4h]
  float v41; // [esp+48h] [ebp+4h]
  float v42; // [esp+48h] [ebp+4h]
  float v43; // [esp+48h] [ebp+4h]
  float v44; // [esp+48h] [ebp+4h]
  float v45; // [esp+48h] [ebp+4h]
  float v46; // [esp+4Ch] [ebp+8h]
  float maxDist_v47; // [esp+50h] [ebp+Ch]
  int numEdges; // [esp+54h] [ebp+10h]

	idVec3 _v18; // v18 v17 v19

  area_v6 = &this->areas[areaNum_a2];
  dist_v37 = plane_a3->Distance(*a4);
  distAbs_v38 = fabs(dist_v37);
  v8 = distAbs_v38;
  v39 = this->settings.invGravityDir * plane_a3->Normal();
  v46 = v8 / v39;
  result = v46;
  if ( a5 <= v46 )
  {
    maxDist_v47 = 1.0e30f;
    if ( area_v6->numEdges > 0 )
    {
      edge_list = this->edges.Ptr();
      vertexes_v11 = this->vertices.Ptr();
      edgeIndex_v12 = &this->edgeIndex[area_v6->firstEdge];
      numEdges = area_v6->numEdges;
      do
      {
        edgeIndexAbs_v13 = abs(*edgeIndex_v12);
        vertexIndex_v14 = edge_list[edgeIndexAbs_v13].vertexNum[0];
        vertex1_p_x = &vertexes_v11[edge_list[edgeIndexAbs_v13].vertexNum[1]];
        vertex0_v16 = &vertexes_v11[vertexIndex_v14];
        edgeLine_v25 = *vertex1_p_x - *vertex0_v16;
        _v18 = edgeLine_v25;
        edgeLineLengthSqr_v40 = edgeLine_v25.LengthSqr();
        if ( edgeLineLengthSqr_v40 >= 0.1f )
        {
          v22 = *a4 - *vertex0_v16;
          v21 = v22 * _v18;
          v41 = v21 / edgeLineLengthSqr_v40;
          if ( v41 >= 0.0f )
          {
            v20 = v41;
            if ( v41 > 1.0f )
              v20 = 1.0f;
          }
          else
          {
            v20 = 0.0f;
          }
          v28 = _v18 * v20;
          v31 = v22 - v28;
          v42 = v31.LengthSqr();
          if ( maxDist_v47 > v42 )
          {
            maxDist_v47 = v31.LengthSqr();
            v34 = v22 - v28;
          }
        }
        ++edgeIndex_v12;
        --numEdges;
      }
      while ( numEdges );
      result = v46;
    }
    v43 = a6 * a6;
    if ( v43 <= maxDist_v47 )
      return v46;
    v44 = this->settings.invGravityDir * v34;
    v45 = fabs(v44);
    if ( v45 >= result )
      return v46;
    else
      return v45;
  }
  return result;
}

void idAASFileLocal::BoundsBestReachableAreaNum(
		idBounds *bounds_a2,
		const idVec3 *origin_a3,
		int nodeNum_a4,
		const idPlane *plane_a5,
		idAASFileLocal::bestReachableArea_t *bestReachableArea_a6) const
{
	int nodeNum_v6; // esi
	bool b_v7; // sf
	const aasNode_t *node_v10; // esi
	int side_v11; // eax
	const aasArea_t *area_v12; // eax
	int areaNum_v13; // esi
	float FloorDistance; // [esp+1Ch] [ebp+4h]

	nodeNum_v6 = nodeNum_a4;
	b_v7 = nodeNum_a4 < 0;
	if ( !nodeNum_a4 )
		return;
	while ( !b_v7 )
	{
		node_v10 = &this->nodes[nodeNum_v6];
		side_v11 = bounds_a2->PlaneSide(this->planeList[node_v10->planeNum], 0.1f);
		if ( !side_v11 )
			goto LABEL_7;
		if ( side_v11 != SIDE_BACK/* 1 */ )
		{
			idAASFileLocal::BoundsBestReachableAreaNum(bounds_a2, origin_a3, node_v10->children[1], plane_a5, bestReachableArea_a6);
			LABEL_7:
				  if ( (node_v10->flags & AAS_NODE_FLAG_FLOOR_PLANE/* 1 */) != 0 )
				  	plane_a5 = &this->planeList[node_v10->planeNum];
			nodeNum_v6 = node_v10->children[0];
			goto LABEL_10;
		}
		nodeNum_v6 = node_v10->children[1];
		LABEL_10:
			b_v7 = nodeNum_v6 < 0;
		if ( !nodeNum_v6 )
			return;
	}
	area_v12 = &this->areas[-nodeNum_v6];
	if ( (area_v12->flags & bestReachableArea_a6->areaFlags) != 0 && (area_v12->travelFlags & bestReachableArea_a6->excludeTravelFlags) == 0 )
	{
		areaNum_v13 = -nodeNum_v6;
		FloorDistance = idAASFileLocal::GetFloorDistance(areaNum_v13, plane_a5, origin_a3, bestReachableArea_a6->v0, bestReachableArea_a6->v1);
		if ( bestReachableArea_a6->distance1 - bestReachableArea_a6->v0 > FloorDistance && bestReachableArea_a6->distance2 > FloorDistance )
		{
			bestReachableArea_a6->distance2 = FloorDistance;
			bestReachableArea_a6->areaNum2 = areaNum_v13;
		}
	}
}

void idAASFileLocal::PointBestReachableAreaNum(
		const idVec3 *origin_a2,
		idAASFileLocal::bestReachableArea_t *bestReachableArea_a3) const
{
	const idPlane *plane_list; // ebp
	int nodeNum_v5; // eax
	const aasNode_t *node_v6; // esi
	const idPlane *plane_p_a; // edx
	const aasArea_t *area_v8; // edx
	int areaNum_v9; // ebx
	const idPlane *plane_v10; // [esp+18h] [ebp-4h]
	float dist_v11; // [esp+20h] [ebp+4h]

	plane_list = this->planeList.Ptr();
	plane_v10 = NULL;
	nodeNum_v5 = 1;
	while ( 1 )
	{
		node_v6 = &this->nodes[nodeNum_v5];
		plane_p_a = &plane_list[node_v6->planeNum];
		dist_v11 = plane_p_a->Distance(*origin_a2);
		if ( dist_v11 <= 0.0f )
		{
			nodeNum_v5 = node_v6->children[1];
		}
		else
		{
			nodeNum_v5 = node_v6->children[0];
			if ( (node_v6->flags & AAS_NODE_FLAG_FLOOR_PLANE/* 1 */) != 0 )
				plane_v10 = &plane_list[node_v6->planeNum];
		}
		if ( nodeNum_v5 < 0 )
			break;
		if ( !nodeNum_v5 )
			return;
	}
	area_v8 = &this->areas[-nodeNum_v5]; // negative is area num
	if ( (area_v8->flags & bestReachableArea_a3->areaFlags) != 0 && (area_v8->travelFlags & bestReachableArea_a3->excludeTravelFlags) == 0 )
	{
		areaNum_v9 = -nodeNum_v5;
		bestReachableArea_a3->distance1 = idAASFileLocal::GetFloorDistance(-nodeNum_v5, plane_v10, origin_a2, bestReachableArea_a3->v0, bestReachableArea_a3->v1);
		bestReachableArea_a3->areaNum1 = areaNum_v9;
	}
}


#if 0
int idAASFileLocal::BoundsReachableAreaNum_r(
		idBounds *a2,
		int a3,
		int a4,
		int a5) const
{
	
	int v5; // eax
	bool v6; // sf
	const aasNode_t *v8; // esi
	int v9; // eax
	int result; // eax
	const aasArea_t *v11; // edi

	v5 = a3;
	v6 = a3 < 0;
	if ( !a3 )
		return 0;
	while ( !v6 )
	{
		v8 = &this->nodes[v5];
		v9 = a2->PlaneSide(this->planeList[v8->planeNum], 0.1);
		if ( v9 == 1 )
		{
			v5 = v8->children[1];
		}
		else
		{
			if ( v9 )
			{
				result = idAASFileLocal::BoundsReachableAreaNum_r(a2, v8->children[1], a4, a5);
				if ( result )
					return result;
			}
			v5 = v8->children[0];
		}
		v6 = v5 < 0;
		if ( !v5 )
			return 0;
	}
	v11 = &this->areas[-v5];
	if ( (v11->flags & (unsigned __int16)a4) != 0 && (v11->travelFlags & (unsigned __int16)a5) == 0 )
		return -v5;
	else
		return 0;
}
#endif

#if 1
bool idAASFileLocal::Trace(
        aasTrace_t *a2,
        const idVec3 *a3,
        const idVec3 *a4) const
{
  const idVec3 *v4; // edx
  const idVec3 *v5; // ecx
	aasTraceStack_t * v6;
  //float *v6; // esi
  double z; // st7
  int v8; // ebx
  const aasArea_t *v9; // eax
  int numAreas; // eax
  int v11; // ebx
  bool v12; // cc
  int *areas; // ecx
  idVec3 *points; // ecx
  idVec3 p_x; // eax
  int v16; // ebx
  const aasNode_t *v17; // ebx
  const idPlane *v18; // eax
  double v19; // st6
  double v20; // st5
  double v21; // st7
  double v22; // st4
  double v23; // st3
  double v24; // st2
  bool v25; // cf
  double v26; // st1
  double v27; // st2
  double v28; // st2
  double v29; // st1
  aasTraceStack_t *v30; // esi
  int v31; // eax
  bool result; // al
  double v33; // st7
  double v34; // st6
  double v35; // st5
  double v36; // st7
  int v37; // esi
  float v38; // [esp+Ch] [ebp-108Ch]
  float v39; // [esp+Ch] [ebp-108Ch]
  float v40; // [esp+Ch] [ebp-108Ch]
  float v41; // [esp+Ch] [ebp-108Ch]
  float v42; // [esp+Ch] [ebp-108Ch]
  float v43; // [esp+Ch] [ebp-108Ch]
  float v44; // [esp+Ch] [ebp-108Ch]
  float v45; // [esp+Ch] [ebp-108Ch]
  float v46; // [esp+Ch] [ebp-108Ch]
  float v47; // [esp+Ch] [ebp-108Ch]
  float v48; // [esp+10h] [ebp-1088h]
  float v49; // [esp+10h] [ebp-1088h]
  float v50; // [esp+10h] [ebp-1088h]
  float v51; // [esp+10h] [ebp-1088h]
  int v52; // [esp+10h] [ebp-1088h]
  float v53; // [esp+10h] [ebp-1088h]
  idVec3 v54; // v54 v57 v60
  //float v54; // [esp+14h] [ebp-1084h]
  idVec3 v55; // v55 v58 v61
  //float v55; // [esp+14h] [ebp-1084h]
  idVec3 v56; // v56 v59 v62
  //float v56; // [esp+14h] [ebp-1084h]
  //float v57; // [esp+18h] [ebp-1080h]
  //float v58; // [esp+18h] [ebp-1080h]
  //float v59; // [esp+18h] [ebp-1080h]
  //float v60; // [esp+1Ch] [ebp-107Ch]
  //float v61; // [esp+1Ch] [ebp-107Ch]
  //float v62; // [esp+1Ch] [ebp-107Ch]
  idVec3 x; // [esp+20h] [ebp-1078h] // x y 67
  //float x; // [esp+20h] [ebp-1078h]
  idVec3 v64; // [esp+20h] [ebp-1078h] // 64 66 68
  //float v64; // [esp+20h] [ebp-1078h]
  //float y; // [esp+24h] [ebp-1074h]
  //float v66; // [esp+24h] [ebp-1074h]
  //float v67; // [esp+28h] [ebp-1070h]
  //float v68; // [esp+28h] [ebp-1070h]
  float v70; // [esp+34h] [ebp-1064h]
  float v71; // [esp+38h] [ebp-1060h]
  idVec3 v72; // v72 v73 v74
  //float v72; // [esp+54h] [ebp-1044h]
  //float v73; // [esp+58h] [ebp-1040h]
  //float v74; // [esp+5Ch] [ebp-103Ch]
  idVec3 v75; // v75 v76 v77
  //float v75; // [esp+60h] [ebp-1038h]
  //float v76; // [esp+64h] [ebp-1034h]
  //float v77; // [esp+68h] [ebp-1030h]
  idVec3 v78; // v78 v79 v80
  //float v78; // [esp+6Ch] [ebp-102Ch]
  //float v79; // [esp+70h] [ebp-1028h]
  //float v80; // [esp+74h] [ebp-1024h]
  double v81; // [esp+78h] [ebp-1020h]
  double v82; // [esp+80h] [ebp-1018h]
  idVec3 v83; // v83 v84 v85
  //float v83; // [esp+8Ch] [ebp-100Ch]
  //float v84; // [esp+90h] [ebp-1008h]
  //float v85; // [esp+94h] [ebp-1004h]
  aasTraceStack_t v86[MAX_AAS_TREE_DEPTH]; // [esp+98h] [ebp-1000h] BYREF
  //float v86[1024]; // [esp+98h] [ebp-1000h] BYREF
  aasTraceStack_t *vars0 = &v86[MAX_AAS_TREE_DEPTH]; // [esp+1098h] [ebp+0h] BYREF

	v6 = &v86[0];
	idVec3 _v23; // v23 v70 v71
	idVec3 _v19; // v19 v20 v21

  v4 = a3;
  a2->numAreas = 0;
  a2->lastAreaNum = 0;
  a2->blockingAreaNum = 0;
	v6->start = *a3;
	v6->end = *a4;
	v6->planeNum = 0;
	v6->nodeNum = 1;
  v5 = a4;
  v6 = v86;
  while ( 1 )
  {
    v8 = v6->nodeNum;
    if ( v8 >= 0 )
    {
      if ( v8 )
      {
        v17 = &this->nodes[v8];
        v18 = &this->planeList[v17->planeNum];
        _v19 = v6->start;
        v40 = v18->Distance(_v19);
        v22 = v40;
        _v23 = v6->end;
        v41 = v18->Distance(v6->end);
        v24 = v41;
        if ( v22 < -ON_EPSILON || v24 < -ON_EPSILON )
        {
          if ( v22 >= ON_EPSILON || v24 >= ON_EPSILON )
          {
            v52 = v6->planeNum;
            if ( v22 >= 0.0 )
              v26 = v22 - TRACEPLANE_EPSILON;
            else
              v26 = v22 + TRACEPLANE_EPSILON;
            v27 = v26 / (v22 - v24);
            if ( v27 >= 0.0 )
            {
              if ( v27 <= 1.0 )
              {
                v29 = v27;
                v28 = 0.0;
              }
              else
              {
                v28 = 0.0;
                v29 = 0.999f;
              }
            }
            else
            {
              v28 = 0.0;
              v29 = 0.001f;
            }
            v72 = _v23 - _v19;
            v42 = v29;
            v78 = v72 * v42;
            v54 = v78 + _v19;
            v6->planeNum = v17->planeNum;
            v6->start = v54;
          	v30 = v6 + 1;
            v6->nodeNum = v17->children[v28 <= v22];
            if ( v30 >= vars0 )
            {
LABEL_50:
			  common->Warning("idAASFileLocal::Trace: stack overflow");
              return false;
            }
            v31 = v17->children[v28 > v22];
          	v6->start = _v19;
          	v6->end = v54;
            v6->planeNum = v52;
            v6->nodeNum = v31;
            v6 = v6 + 1;
            v25 = v6 < vars0;
          }
          else
          {
            v6->nodeNum = v17->children[1];
            v6 = v6 + 1;
            v25 = v6 < vars0;
          }
          if ( !v25 )
            goto LABEL_50;
        }
        else
        {
          v6->nodeNum = v17->children[0];
          v6 = v6 + 1;
          if ( v6 >= vars0 )
            goto LABEL_50;
        }
      }
      else
      {
        if ( a2->lastAreaNum )
        {
          v75 = *v5 - *v4;
          x = v75;
          v83 = v6->start - *v4;
          v49 = v83.Length();
          v38 = v49;
          v51 = v75.Length();
          v4 = a3;
          a2->fraction = v38 / v51;
        }
        else
        {
          a2->fraction = 0.0;
          x = vec3_origin;
        }
        v16 = v6->planeNum;
        a2->endpos = v6->start;
        a2->blockingAreaNum = 0;
        a2->planeNum = v16;
        v39 = x * this->planeList[v16].Normal();
        if ( v39 > 0.0 )
          a2->planeNum = v16 ^ 1;
        if ( a2->lastAreaNum || !a2->getOutOfSolid )
          return true;
      }
    }
    else
    {
      v9 = &this->areas[-v8];
      if ( (v9->flags & a2->flags) != 0 || (v9->travelFlags & a2->travelFlags) != 0 )
      {
        if ( a2->lastAreaNum )
        {
          v55 = *a4 - *v4;
        	v64 = v55;
          v56 = v6->start - *v4;
          v44 = v6->start.Length();
          v53 = v44;
          v46 = v55.Length();
          a2->fraction = v53 / v46;
        }
        else
        {
          a2->fraction = 0.0;
          v64 = vec3_origin;
        }
        a2->endpos = v6->start;
        v37 = v6->planeNum;
        a2->blockingAreaNum = -v8;
        a2->planeNum = v37;
        v47 = v64 * this->planeList[v37].Normal();
        if ( v47 > 0.0 )
          a2->planeNum = v37 ^ 1;
        return true;
      }
      numAreas = a2->numAreas;
      v11 = -v8;
      v12 = numAreas < a2->maxAreas;
      a2->lastAreaNum = v11;
      if ( v12 )
      {
        areas = a2->areas;
        if ( areas )
          areas[numAreas] = v11;
        points = a2->points;
        if ( points )
        {
          p_x = v6->start;
			points[numAreas] = p_x;
        }
        ++a2->numAreas;
      }
    }
    v6 = v6 - 1;
    if ( v6 < v86 )
      break;
    v5 = a4;
  }
  if ( a2->lastAreaNum )
  {
    a2->fraction = 1.0;
    a2->endpos = *a4;
    result = false;
  }
  else
  {
    result = false;
    a2->fraction = 0.0;
    a2->endpos = *v4;
  }
  a2->planeNum = 0;
  return result;
}
#endif

#endif
