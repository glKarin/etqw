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

		if (planeList[node->planeNum].Side(origin) == PLANESIDE_BACK) {
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
	// TODO
	common->Error("Disable idAASFileLocal::PushPointIntoAreaNum(%d)", areaNum);
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

bool idAASFileLocal::PushPointIntoArea( int areaNum, idVec3 &point ) const {
  bool v4; // bl
  const aasArea_t *v5; // esi
  int v6; // ecx
  unsigned int v7; // edx
  const aasEdge_t *list; // eax
  bool v9; // zf
  const aasEdge_t *v10; // eax
  const idVec3 *v11; // edx
  const float *p_x; // esi
  const float *v13; // ebx
  double v14; // st7
  double v15; // st7
  long double v16; // st6
  long double v17; // st5
  long double v18; // st4
  long double v19; // st5
  long double v20; // st4
  double v21; // st3
  double v22; // st3
  long double v23; // st4
  long double v24; // st6
  long double v25; // rt2
  long double v26; // rtt
  double v27; // st3
  bool result; // al
  float v29; // [esp+8h] [ebp-98h]
  float v30; // [esp+8h] [ebp-98h]
  float v31; // [esp+8h] [ebp-98h]
  float v32; // [esp+8h] [ebp-98h]
  float v33; // [esp+8h] [ebp-98h]
  float v34; // [esp+8h] [ebp-98h]
  char v35; // [esp+Fh] [ebp-91h]
  float v36; // [esp+10h] [ebp-90h]
  float v37; // [esp+10h] [ebp-90h]
  float v38; // [esp+10h] [ebp-90h]
  float v39; // [esp+10h] [ebp-90h]
  float v40; // [esp+10h] [ebp-90h]
  float v41; // [esp+10h] [ebp-90h]
  float v42; // [esp+10h] [ebp-90h]
  char v43; // [esp+17h] [ebp-89h]
  float v44; // [esp+18h] [ebp-88h]
  float v45; // [esp+1Ch] [ebp-84h]
  float v46; // [esp+1Ch] [ebp-84h]
  float v47; // [esp+1Ch] [ebp-84h]
  float v48; // [esp+20h] [ebp-80h]
  float v49; // [esp+20h] [ebp-80h]
  float v50; // [esp+20h] [ebp-80h]
  float v51; // [esp+20h] [ebp-80h]
  float v52; // [esp+24h] [ebp-7Ch]
  int v53; // [esp+28h] [ebp-78h]
  const aasArea_t *v56; // [esp+38h] [ebp-68h]
  float v57; // [esp+3Ch] [ebp-64h]
  float v58; // [esp+40h] [ebp-60h]
  float v59; // [esp+44h] [ebp-5Ch]
  float v60; // [esp+48h] [ebp-58h]
  float v61; // [esp+4Ch] [ebp-54h]
  float v62; // [esp+50h] [ebp-50h]
  float v63; // [esp+54h] [ebp-4Ch]
  float v64; // [esp+58h] [ebp-48h]
  float v65; // [esp+5Ch] [ebp-44h]
  float v66; // [esp+60h] [ebp-40h]
  float v67; // [esp+64h] [ebp-3Ch]
  float v68; // [esp+68h] [ebp-38h]
  float v69; // [esp+6Ch] [ebp-34h]
  float v70; // [esp+70h] [ebp-30h]
  float v71; // [esp+74h] [ebp-2Ch]
  float v72; // [esp+78h] [ebp-28h]
  float v73; // [esp+7Ch] [ebp-24h]
  float v74; // [esp+80h] [ebp-20h]
  float v75; // [esp+84h] [ebp-1Ch]
  float v78; // [esp+94h] [ebp-Ch]
  float v79; // [esp+98h] [ebp-8h]
  float v80; // [esp+9Ch] [ebp-4h]
  idVec3 v81; // 0:^34.12
  idVec3 v82; // 0:^90.12

  v57 = 1.0e30;
  v82 = point;
  v81 = point;
  v4 = false;
  v5 = &this->areas[areaNum];
  v35 = 0;
  v43 = 0;
  v56 = v5;
  v53 = 0;
  if ( v5->numEdges <= 0 )
    return v4;
  do
  {
    v6 = this->edgeIndex[v53 + v5->firstEdge];
    v7 = abs(v6);
    list = this->edges.Ptr();
    v9 = (list[v7].flags & 0x40) == 0;
    v10 = &list[v7];
    if ( !v9 )
      goto LABEL_53;
    v11 = this->vertices.Ptr();
    p_x = &v11[v10->vertexNum[(unsigned int)v6 >> 31]].x;
    v78 = *p_x - v82.x;
    v13 = &v11[v10->vertexNum[v6 >= 0]].x;
    v79 = p_x[1] - v82.y;
    v80 = p_x[2] - v82.z;
    v29 = v79 * v79 + v78 * v78 + v80 * v80;
    if ( v57 > (double)v29 )
    {
      v57 = v79 * v79 + v78 * v78 + v80 * v80;
      v81.x = *p_x;
      v81.y = p_x[1];
      v81.z = p_x[2];
    }
    v61 = *v13 - *p_x;
    v62 = v13[1] - p_x[1];
    v63 = v13[2] - p_x[2];
    v58 = v62 * this->settings.invGravityDir.z - this->settings.invGravityDir.y * v63;
    v59 = v63 * this->settings.invGravityDir.x - this->settings.invGravityDir.z * v61;
    v60 = v61 * this->settings.invGravityDir.y - v62 * this->settings.invGravityDir.x;
    v44 = v58;
    v45 = v59;
    v48 = v60;
    v30 = v60 * v60 + v59 * v59 + v58 * v58;
    v31 = sqrt(v30);
    v14 = v31;
    if ( v31 >= 0.00000011920929 )
    {
      v33 = 1.0 / v14;
      v44 = v33 * v58;
      v45 = v59 * v33;
      v48 = v33 * v60;
      v32 = v14;
      v15 = 0.0;
    }
    else
    {
      v15 = 0.0;
      v32 = 0.0;
    }
    v16 = v44;
    v17 = v48;
    v18 = v45;
    if ( v44 == v15 )
    {
      if ( v45 == v15 )
      {
        v19 = v45;
        v20 = v48;
        if ( v48 <= v15 )
          v21 = -1.0;
        else
          v21 = 1.0;
        if ( v21 != v20 )
        {
          v49 = v21;
          v20 = v49;
        }
        goto LABEL_44;
      }
      if ( v15 == v17 )
      {
        if ( v18 <= v15 )
          v22 = -1.0;
        else
          v22 = 1.0;
        if ( v22 != v18 )
        {
          v46 = v22;
          v18 = v46;
        }
        goto LABEL_43;
      }
    }
    else if ( v45 == v15 && v15 == v17 )
    {
      v23 = v44;
      v24 = v45;
      if ( v44 <= v15 )
      {
        if ( -1.0 != v23 )
          v23 = (float)-1.0;
      }
      else if ( 1.0 != v23 )
      {
        v23 = (float)1.0;
      }
      goto LABEL_42;
    }
    v36 = fabs(v16);
    if ( 1.0 == v36 )
    {
      if ( v15 == v18 && v15 == v17 )
        goto LABEL_43;
      v50 = v15;
      v17 = v50;
      v24 = v50;
      v23 = v44;
LABEL_42:
      v25 = v23;
      v18 = v24;
      v16 = v25;
LABEL_43:
      v26 = v18;
      v20 = v17;
      v19 = v26;
      goto LABEL_44;
    }
    v37 = fabs(v18);
    if ( v37 == 1.0 )
    {
      if ( v15 != v16 || v15 != v17 )
      {
        v51 = v15;
        v17 = v51;
        v16 = v51;
        v18 = v45;
      }
      goto LABEL_43;
    }
    v38 = fabs(v17);
    if ( v38 != 1.0 || v15 == v16 && v15 == v18 )
      goto LABEL_43;
    v47 = v15;
    v19 = v47;
    v16 = v47;
    v20 = v48;
LABEL_44:
    if ( v15 != v32 )
    {
      v39 = v19 * v13[1] + *v13 * v16 + v13[2] * v20;
      v52 = -v39;
    }
    v34 = point.x * v16 + point.y * v19 + v20 * point.z + v52;
    v27 = v34;
    if ( v34 < v15 )
    {
      v35 = 1;
      v70 = v16 * v27;
      v71 = v19 * v27;
      v72 = v27 * v20;
      point.x = point.x - v70;
      point.y = point.y - v71;
      point.z = point.z - v72;
      v34 = v15;
    }
    v40 = fabs(v34);
    if ( v40 < 0.1000000014901161 )
    {
      v67 = *v13 - *p_x;
      v68 = v13[1] - p_x[1];
      v69 = v13[2] - p_x[2];
      v64 = point.x - *p_x;
      v65 = point.y - p_x[1];
      v66 = point.z - p_x[2];
      v41 = v66 * v69 + v64 * v67 + v65 * v68;
      if ( v41 >= v15 )
      {
        v73 = point.x - *v13;
        v74 = point.y - v13[1];
        v75 = point.z - v13[2];
        v42 = v67 * v73 + v68 * v74 + v69 * v75;
        if ( v15 >= v42 )
          v43 = 1;
      }
    }
    v4 = v35;
    v5 = v56;
LABEL_53:
    ++v53;
  }
  while ( v53 < v5->numEdges );
  if ( !v4 )
    return v4;
  result = v4;
  if ( !v43 )
    point = v81;
  return result;
}


typedef unsigned int _DWORD;
typedef unsigned short _WORD;
typedef unsigned char _BYTE;
#define LODWORD(x) ((DWORD)(*(uint64_t *)&(x)))

bool idAASFileLocal::TraceHeight( aasTraceHeight_t &trace, const idVec3 &start, const idVec3 &end ) const {
  float *v4; // edx
  double y; // st7
  int v6; // eax
  const aasNode_t *list; // esi
  int v8; // eax
  bool v9; // zf
  const aasNode_t *v10; // esi
  int numPoints; // eax
  idVec3 *points; // ebx
  int v13; // eax
  idVec3 *v14; // eax
  int planeNum; // ebx
  const idPlane *v16; // eax
  double v17; // st6
  double v18; // st7
  double v19; // st5
  double v20; // st4
  double v21; // st2
  int v22; // eax
  int v23; // ebp
  double v24; // st1
  double v25; // st2
  double v26; // st2
  double v27; // st1
  float v29; // [esp+10h] [ebp-1040h]
  float v30; // [esp+10h] [ebp-1040h]
  float v31; // [esp+10h] [ebp-1040h]
  float v32; // [esp+14h] [ebp-103Ch]
  float v33; // [esp+18h] [ebp-1038h]
  float v34; // [esp+1Ch] [ebp-1034h]
  float v35; // [esp+20h] [ebp-1030h]
  float v36; // [esp+24h] [ebp-102Ch]
  float v37; // [esp+28h] [ebp-1028h]
  float v38; // [esp+2Ch] [ebp-1024h]
  float v39; // [esp+30h] [ebp-1020h]
  float v40; // [esp+34h] [ebp-101Ch]
  float v41; // [esp+38h] [ebp-1018h]
  float v42; // [esp+3Ch] [ebp-1014h]
  float v43; // [esp+40h] [ebp-1010h]
  float v44; // [esp+44h] [ebp-100Ch]
  float v45; // [esp+48h] [ebp-1008h]
  float v46; // [esp+4Ch] [ebp-1004h]
  float v47[1024]; // [esp+50h] [ebp-1000h] BYREF
  float *retaddr = (v47 + 1024) - 32; // [esp+1050h] [ebp+0h] BYREF // aasTraceStack_t

  trace.numPoints = 0;
  v47[0] = start.x;
  v4 = v47;
  y = start.y;
  v47[6] = 0.0;
  v47[1] = y;
  *(int *)&v47[7] = 1; // LODWORD(v47[7]) = 1;
  v47[2] = start.z;
  v47[3] = end.x;
  v47[4] = end.y;
  v47[5] = end.z;
  while ( 1 )
  {
    v6 = *((_DWORD *)v4 + 7);
    if ( !v6 )
      goto LABEL_23;
    list = this->nodes.Ptr();
    v8 = v6;
    v9 = (list[v8].flags & AAS_NODE_FLAG_COLUMN_HEIGHT) == 0; // 2
    v10 = &list[v8];
    if ( !v9 )
    {
      numPoints = trace.numPoints;
      if ( numPoints < trace.maxPoints )
      {
        points = trace.points;
        v13 = numPoints;
        points[v13].x = *v4;
        v14 = &points[v13];
        v14->y = v4[1];
        v14->z = v4[2];
        trace.points[trace.numPoints++].z = (float)((v10->flags >> 2) - 0x2000);
      }
      goto LABEL_23;
    }
    planeNum = v10->planeNum;
    v44 = *v4;
    v45 = v4[1];
    v16 = &this->planeList[planeNum];
    v46 = v4[2];
    v32 = v4[3];
    v33 = v4[4];
    v34 = v4[5];
    v17 = v44;
    v18 = v45;
    v19 = v46;
    v29 = v16->ToFloatPtr()[2] * v46 + v16->ToFloatPtr()[1] * v45 + v16->ToFloatPtr()[0] * v44 + v16->ToFloatPtr()[3];
    v20 = v29;
    v30 = v16->ToFloatPtr()[0] * v32 + v16->ToFloatPtr()[1] * v33 + v16->ToFloatPtr()[2] * v34 + v16->ToFloatPtr()[3];
    v21 = v30;
    if ( v20 >= -0.1000000014901161 && v21 >= -0.1000000014901161 )
    {
      v22 = v10->children[0];
      goto LABEL_22;
    }
    if ( v20 < 0.1000000014901161 && v21 < 0.1000000014901161 )
    {
      v22 = v10->children[1];
      goto LABEL_22;
    }
    v23 = *((_DWORD *)v4 + 6);
    v24 = v20 >= 0.0 ? v20 - 0.125 : v20 + 0.125;
    v25 = v24 / (v20 - v21);
    if ( v25 >= 0.0 )
    {
      if ( v25 <= 1.0 )
      {
        v27 = v25;
        v26 = 0.0;
      }
      else
      {
        v26 = 0.0;
        v27 = 0.9990000128746033;
      }
    }
    else
    {
      v26 = 0.0;
      v27 = 0.001000000047497451;
    }
    v35 = v32 - v17;
    v36 = v33 - v18;
    v37 = v34 - v19;
    v31 = v27;
    v38 = v35 * v31;
    v39 = v36 * v31;
    v40 = v31 * v37;
    v41 = v38 + v17;
    v42 = v39 + v18;
    v43 = v40 + v19;
    *((_DWORD *)v4 + 6) = planeNum;
    *v4 = v41;
    v4[1] = v42;
    v4 += 8;
    *(v4 - 6) = v43;
    *(v4 - 1) = *(float *)&v10->children[v26 <= v20];
    if ( v4 >= (float *)&retaddr )
      break;
    v22 = v10->children[v26 > v20];
    *v4 = v44;
    v4[1] = v45;
    v4[2] = v19;
    v4[3] = v41;
    v4[4] = v42;
    v4[5] = v43;
    *((_DWORD *)v4 + 6) = v23;
LABEL_22:
    *((_DWORD *)v4 + 7) = v22;
    v4 += 8;
    if ( v4 >= (float *)&retaddr )
      break;
LABEL_23:
    v4 -= 8;
    if ( v4 < v47 )
      return true;
  }
  common->Warning("idAASFileLocal::Trace: stack overflow");
  return false;
}

bool idAASFileLocal::TraceFloor( aasTraceFloor_t &trace, const idVec3 &start, int startAreaNum, const idVec3 &end, int endAreaNum, int travelFlags ) const
{
  struct aasTraceFloor_t *v7; // ebp
  long double a; // st6
  double v10; // st5
  long double c; // st3
  double b; // st2
  long double v13; // st7
  long double v14; // st5
  long double v15; // st7
  long double v16; // st5
  long double v17; // st4
  long double v18; // st6
  long double v19; // rt0
  long double v20; // st4
  long double v21; // st6
  long double v22; // st5
  long double v23; // st4
  long double v24; // st5
  long double v25; // st7
  double v26; // st4
  long double v27; // st7
  double v28; // st4
  double v29; // st4
  long double v30; // st6
  long double v31; // rtt
  long double v32; // st4
  long double v33; // st3
  double x; // st7
  int i; // eax
  int v36; // edx
  int granularity; // eax
  bool v38; // cc
  int num; // ebx
  int size; // eax
  int v41; // eax
  int *list; // edi
  int j; // eax
  double v44; // st7
  int v45; // edi
  int v46; // eax
  double v47; // st4
	const aasReachability_t *v48;
  //float v48; // ecx
  double v49; // st5
  double v50; // st7
  int v51; // ebx
  const aasArea_t *v52; // eax
  int v53; // eax
  int v54; // ebp
  int v55; // eax
  int v56; // eax
  int *v57; // edi
  int k; // eax
  double v59; // st7
  double v60; // st5
  int v61; // ebp
  int m; // eax
  int v63; // edx
  double v64; // st7
  double v65; // st7
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
  float v95; // [esp+10h] [ebp-7Ch]
  float v96; // [esp+10h] [ebp-7Ch]
  float v97; // [esp+10h] [ebp-7Ch]
  float v98; // [esp+10h] [ebp-7Ch]
  float v99; // [esp+14h] [ebp-78h]
  float v100; // [esp+14h] [ebp-78h]
  float v101; // [esp+14h] [ebp-78h]
  float v102; // [esp+14h] [ebp-78h]
  float v103; // [esp+14h] [ebp-78h]
  float v104; // [esp+18h] [ebp-74h]
  float v105; // [esp+18h] [ebp-74h]
  float v106; // [esp+18h] [ebp-74h]
  float v107; // [esp+18h] [ebp-74h]
  float v108; // [esp+18h] [ebp-74h]
  idPlane v109; // [esp+1Ch] [ebp-70h] BYREF
  int v110; // [esp+2Ch] [ebp-60h]
  idPlane v111; // [esp+30h] [ebp-5Ch] BYREF
  float v112; // [esp+40h] [ebp-4Ch]
	idVec3 v113;
  //float v113; // [esp+44h] [ebp-48h]
  //float v114; // [esp+48h] [ebp-44h]
  //float v115; // [esp+4Ch] [ebp-40h]
  int v116; // [esp+50h] [ebp-3Ch]
	floorEdgeSplitPoint_t v117;
  //float v117; // [esp+54h] [ebp-38h] BYREF
  //float y; // [esp+58h] [ebp-34h]
  //float z; // [esp+5Ch] [ebp-30h]
  //float v120; // [esp+60h] [ebp-2Ch]
  //int v121; // [esp+64h] [ebp-28h]
	floorEdgeSplitPoint_t v122;
  //float v122; // [esp+68h] [ebp-24h] BYREF
  //float v123; // [esp+6Ch] [ebp-20h]
  //float v124; // [esp+70h] [ebp-1Ch]
  //float v125; // [esp+74h] [ebp-18h]
  //int v126; // [esp+78h] [ebp-14h]
  float v127; // [esp+88h] [ebp-4h]

	idList<aasArea_t> &areas = (idList<aasArea_t> &)this->areas;
	idList<int> &floorIndex = (idList<int> &)this->floorIndex;

  v7 = &trace;
  trace.fraction = 0.0;
  trace.endpos = start;
  trace.lastAreaNum = startAreaNum;
  trace.lastEdgeNum = 0;
  v113 = end - start;
	idVec3 normal = v113.Cross(this->settings.gravityDir);
	v111.SetNormal(normal);
	v111.Normalize();
  a = v111[0];
  v10 = -1.0;
  c = v111[2];
  b = v111[1];
  if ( v111[0] != 0.0 )
  {
    if ( v111[1] == 0.0 && 0.0 == c )
    {
      v15 = v111[1];
      if ( a <= 0.0 )
      {
        v17 = v111[2];
        if ( -1.0 == a )
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
        if ( 1.0 == a )
          goto LABEL_23;
        v10 = 1.0;
        v18 = v111[2];
      }
      v111[0] = v10;
      goto LABEL_22;
    }
LABEL_28:
    v14 = v111[1];
    v79 = fabs(a);
    if ( v79 == 1.0 )
    {
      v20 = v111[2];
      if ( 0.0 != v14 || 0.0 != v20 )
      {
        v111[2] = 0.0;
        v111[1] = 0.0;
        v13 = (float)0.0;
        v14 = v13;
        goto LABEL_24;
      }
      goto LABEL_44;
    }
    v80 = fabs(v14);
    if ( v80 != 1.0 )
    {
      v81 = fabs(c);
      v20 = v111[2];
      if ( v81 == 1.0 && (0.0 != a || 0.0 != v14) )
      {
        v13 = v111[2];
        v111[1] = 0.0;
        v111[0] = 0.0;
        v14 = (float)0.0;
        a = v14;
        goto LABEL_24;
      }
      goto LABEL_44;
    }
    v20 = v111[2];
    if ( 0.0 == a && 0.0 == v20 )
    {
LABEL_44:
      v13 = v20;
      goto LABEL_24;
    }
    v15 = v111[1];
    v111[2] = 0.0;
    v111[0] = 0.0;
    v18 = (float)0.0;
LABEL_22:
    v16 = v18;
    a = v111[0];
    goto LABEL_23;
  }
  if ( v111[1] != 0.0 )
  {
    if ( 0.0 == c )
    {
      v15 = v111[1];
      if ( b > 0.0 )
      {
        v16 = v111[2];
        if ( 1.0 != b )
        {
          v13 = v111[2];
          v111[1] = 1.0;
          v14 = (float)1.0;
          goto LABEL_24;
        }
        goto LABEL_23;
      }
      v17 = v111[2];
      if ( -1.0 != b )
      {
        v13 = v111[2];
        v111[1] = -1.0;
        v14 = (float)-1.0;
        goto LABEL_24;
      }
      goto LABEL_43;
    }
    goto LABEL_28;
  }
  v13 = v111[2];
  if ( v111[2] <= 0.0 )
  {
    if ( -1.0 != v13 )
    {
      v15 = v111[1];
      v111[2] = -1.0;
      v16 = (float)-1.0;
      goto LABEL_23;
    }
    v14 = v111[1];
  }
  else
  {
    v14 = v111[1];
    if ( 1.0 != v13 )
    {
      v15 = v111[1];
      v111[2] = 1.0;
      v16 = (float)1.0;
LABEL_23:
      v19 = v16;
      v14 = v15;
      v13 = v19;
    }
  }
LABEL_24:
  v75 = v14 * start.y + start.x * a + v13 * start.z;
  v111[3] = -v75;
  v95 = v13 * this->settings.gravityDir.y - this->settings.gravityDir.z * v14;
  v100 = this->settings.gravityDir.z * a - v13 * this->settings.gravityDir.x;
  v105 = v14 * this->settings.gravityDir.x - a * this->settings.gravityDir.y;
  v109[0] = v95;
  v109[1] = v100;
  v109[2] = v105;
  v76 = v105 * v105 + v100 * v100 + v95 * v95;
  v77 = sqrt(v76);
  if ( v77 >= 0.00000011920929 )
  {
    v78 = 1.0 / v77;
    v109[0] = v78 * v95;
    v109[1] = v100 * v78;
    v109[2] = v78 * v105;
  }
  v21 = v109[0];
  v22 = v109[2];
  v23 = v109[1];
  if ( v109[0] != 0.0 )
  {
    if ( v109[1] == 0.0 && 0.0 == v22 )
    {
      v27 = v109[1];
      if ( v21 <= 0.0 )
        v29 = -1.0;
      else
        v29 = 1.0;
      if ( v29 == v21 )
        goto LABEL_66;
      v30 = v109[2];
      v109[0] = v29;
      goto LABEL_65;
    }
LABEL_77:
    v84 = fabs(v21);
    if ( 1.0 == v84 )
    {
      if ( 0.0 != v23 || 0.0 != v22 )
      {
        v109[2] = 0.0;
        v109[1] = 0.0;
        v25 = (float)0.0;
        v24 = v25;
        goto LABEL_67;
      }
      goto LABEL_91;
    }
    v85 = fabs(v23);
    if ( v85 == 1.0 )
    {
      if ( 0.0 != v21 || 0.0 != v22 )
      {
        v27 = v109[1];
        v109[2] = 0.0;
        v109[0] = 0.0;
        v30 = (float)0.0;
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
      if ( v86 == 1.0 && (0.0 != v21 || 0.0 != v23) )
      {
        v25 = v109[2];
        v109[1] = 0.0;
        v109[0] = 0.0;
        v24 = (float)0.0;
        v21 = v24;
        goto LABEL_67;
      }
    }
LABEL_91:
    v27 = v109[1];
    goto LABEL_66;
  }
  if ( v109[1] == 0.0 )
  {
    v24 = v109[1];
    v25 = v109[2];
    if ( v109[2] <= 0.0 )
      v26 = -1.0;
    else
      v26 = 1.0;
    if ( v26 == v25 )
      goto LABEL_67;
    v27 = v109[1];
    v109[2] = v26;
    v22 = v109[2];
    goto LABEL_66;
  }
  if ( 0.0 != v22 )
    goto LABEL_77;
  v27 = v109[1];
  if ( v23 <= 0.0 )
    v28 = -1.0;
  else
    v28 = 1.0;
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
  v83 = v25 * end.z + v21 * end.x + v24 * end.y;
  v127 = -v83;
  if ( !idAASFileLocal::GetFloorEdgeSplitPoints(
          (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v122,
          (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v117,
          startAreaNum,
          &v111,
          &v109) )
  {
    x = start.x;
    v117.edgeIndex = 0;
    v117.point[0] = x;
    v122.edgeIndex = 0;
    v117.point[1] = start.y;
    v117.point[2] = start.z;
    v122.point = v117.point;
    v117.distance = 0.0;
    v122.distance = 0.0;
  }
  for ( i = 0; i < this->floorIndex.Num(); ++i )
  {
    v36 = this->floorIndex[i];
    areas[v36].flags &= ~0x8000u;
  }
  floorIndex.SetNum(0);
  while ( 1 )
  {
    v44 = v117.point[0];
    v45 = v116;
    floorIndex.Append(v116);
    areas[v45].flags |= 0x8000u;
    v7->endpos.x = v117.point[0];
    v7->endpos.y = v117.point[1];
    v46 = v117.edgeIndex;
    v7->endpos.z = v117.point[2];
    v7->lastEdgeNum = v46;
    if ( v45 == endAreaNum )
      break;
    v112 = v7->endpos.z * v109[2] + v7->endpos.y * v109[1] + v109[0] * v7->endpos.x + v127;
    if ( v112 > 0.1000000014901161 )
      break;
    v47 = v7->endpos.y;
    v48 = this->areas[v45].reach;
    v49 = v109[0] * v7->endpos.x;
    v110 = this->floorIndex.Num();
    v112 = v109[2] * v7->endpos.z + v109[1] * v47 + v49;
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
      if ( ( v48->travelFlags & (unsigned __int16)~travelFlags) == 0 )
      {
        v51 = v48->travelFlags;
        v52 = &this->areas[v51];
        if ( (v52->travelFlags & (unsigned __int16)~travelFlags) == 0 && (v52->flags & 0x8000u) == 0 )
        {
          floorIndex.Append(v51);
          areas[v51].flags |= 0x8000u;
          idAASFileLocal::GetFloorEdgeSplitPoints(
            (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v122,
            (struct idAASFileLocal::floorEdgeSplitPoint_t *)&v117,
            _v112->travelFlags,
            &v111,
            &v109);
          if ( v122.distance < 1.0e30 && v117.distance >= 0.1000000014901161 )
          {
            v96 = trace.endpos.x - v122.point[0];
            v101 = trace.endpos.y - v122.point[1];
            v106 = trace.endpos.z - v122.point[2];
            v87 = fabs(v96);
            if ( v87 < 0.00000011920929 )
              v96 = 0.0;
            v88 = fabs(v101);
            if ( v88 < 0.00000011920929 )
              v101 = 0.0;
            v89 = fabs(v106);
            v59 = v106;
            if ( v89 < 0.00000011920929 )
              v59 = (float)0.0;
            v90 = this->settings.gravityDir.y * v101
                + v96 * this->settings.gravityDir.x
                + this->settings.gravityDir.z * v59;
            v113 = v90 * this->settings.gravityDir;
            v91 = v113.LengthSqr();
            v60 = v91;
            v92 = this->settings.maxStepHeight * this->settings.maxStepHeight;
            if ( v92 >= v60 )
            {
              v97 = v96 - v113.x;
              v102 = v101 - v113.y;
              v107 = v59 - v113.z;
              v93 = v107 * v107 + v102 * v102 + v97 * v97;
              if ( v93 <= 0.040000003 )
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
      areas[v63].flags &= ~0x8000u;
    }
    v68 = _v112;
    floorIndex.SetNum(v61);
    v7 = &trace;
    v116 = v68->toAreaNum;
  }
  v7->endpos = end;
  v65 = 1.0;
LABEL_159:
  v7->lastAreaNum = v45;
  v7->fraction = v65;
  for ( ii = 0; ii < this->floorIndex.Num(); ++ii )
  {
    v70 = this->floorIndex[ii];
    areas[v70].flags &= ~0x8000u;
  }
  floorIndex.Clear();
  return true;
}

bool idAASFileLocal::SplitFloorWinding(
		int a2, // areaNum
		const idPlane *a3,
		float *a4,
		int *a5) const
{
	const aasArea_t *list; // eax
	int v6; // ebp
	bool v7; // cc
	const aasArea_t *v8; // eax
	float *v9; // edi
	int v10; // esi
	const idVec3 *v11; // eax
	int v12; // eax
	int v14; // [esp+4h] [ebp-8h]
	int v15; // [esp+8h] [ebp-4h]
	const aasArea_t *v16; // [esp+10h] [ebp+4h]

	list = this->areas.Ptr();
	v6 = 0;
	v7 = list[a2].numEdges <= 0;
	v8 = &list[a2];
	v15 = 1;
	v14 = 0;
	v16 = v8;
	if ( !v7 )
	{
		v9 = a4;
		do
		{
			v10 = this->edgeIndex[v6 + v8->firstEdge];
			v11 = &this->vertices[this->edges[abs(v10)].vertexNum[v10 >> 31]];
			++v6;
			float d = a3->Distance(*v11);
			*v9++ = d; // a4 += 1
			v12 = FLOATSIGNBITSET(d); // check distance less than 0; v12 = 1 if distance < 0 // *((_DWORD *)v9 - 1) >> 31;
			v15 &= v12;
			v14 |= v12;
			*a5++ = v12; // a5 += 1 // *(_DWORD *)((char *)v9 + ((char *)a5 - (char *)a4) - 4) = v12; // a5 - a4 == align16( (1+numEdges) * sizeof(int/float) )
			v8 = v16;
			//Sys_Printf("EEE %d/%d: %f %d | %d %d\n", v6-1,v8->numEdges, d,v12,v15, v14);
		}
		while ( v6 < v16->numEdges );
	}
	a4[v8->numEdges] = *a4;
	a5[v8->numEdges] = *a5;
	return v15 != v14; // all distance >= 0; v14 == v15 if has distance < 0
}

//----- (005EC1D0) --------------------------------------------------------
bool idAASFileLocal::GetFloorEdgeSplitPoints(
        idAASFileLocal::floorEdgeSplitPoint_t *a2,
        idAASFileLocal::floorEdgeSplitPoint_t *a3,
        int a4, // areaNum
        const idPlane *a5,
        const idPlane *a6) const
{
  float *v8; // esp
  int *v9; // esp
  bool result; // al
  int v11; // eax
  float *v12; // ebx
  //idPlane *v12; // ebx
  int v13; // ecx
  int v14; // ecx // edgeIndex
  const aasEdge_t *v15; // edx
  const idVec3 *v16; // eax
  const idVec3 *v17; // edx
  idVec3 v19;
  //double v18; // st6
  //double v19; // st7
  //double v20; // st5
  int v21; // [esp+0h] [ebp-3Ch] BYREF
  _BYTE v22[8]; // [esp+4h] [ebp-38h] BYREF
  idVec3 v23;
  //float v23; // [esp+Ch] [ebp-30h]
  //float v24; // [esp+10h] [ebp-2Ch]
  //float v25; // [esp+14h] [ebp-28h]
  idVec3 v26;
  //float v26; // [esp+18h] [ebp-24h]
  //float v27; // [esp+1Ch] [ebp-20h]
  //float v28; // [esp+20h] [ebp-1Ch]
  idVec3 v29; // edgeVec
  //float v29; // [esp+24h] [ebp-18h]
  //float v30; // [esp+28h] [ebp-14h]
  //float v31; // [esp+2Ch] [ebp-10h]
  int i; // [esp+30h] [ebp-Ch]
  int *v33; // [esp+34h] [ebp-8h]
  const aasArea_t *v34; // [esp+38h] [ebp-4h]
  int v35; // [esp+4Ch] [ebp+10h]
  int v36; // [esp+4Ch] [ebp+10h]
  float *v37; // [esp+50h] [ebp+14h]
  //idPlane *v37; // [esp+50h] [ebp+14h]
  float v38; // [esp+50h] [ebp+14h]
  float v39; // [esp+50h] [ebp+14h]

  a2->point[0] = 0.0;
  a2->point[1] = 0.0;
  a2->point[2] = 0.0;
  a2->edgeIndex = 0;
  a2->distance = 1.0e30;
  a3->point[0] = 0.0;
  a3->point[1] = 0.0;
  a3->point[2] = 0.0;
  a3->edgeIndex = 0;
  a3->distance = -1.0e30;
  v34 = &this->areas[a4];
  v35 = 4 * (v34->numEdges + 1) + 15;
  v8 = (float *)alloca(v35);
  v9 = (int *)alloca(v35);
  v33 = &v21;
  result = idAASFileLocal::SplitFloorWinding(a4, a5, v8, v9); // (this, a4, a5, (float *)&v21, &v21);
  if ( result ) // all distance >= 0
  {
    v11 = 0;
    v36 = 0;
    if ( v34->numEdges > 0 )
    {
      // v12 = (idPlane *)v22;
      // v13 = (char *)v33 - (char *)&v21; // == sizeof(v8/v9)
      // v37 = (idPlane *)v22;
v33 = v9;
v12 = v8;
int *_v12 = v9;
v37 = v8;
      for ( ; ; /*i = (char *)v33 - (char *)&v21; ; v13 = i*/ )
      {
        if ( v33[v11] != *_v12 /* *(_DWORD *)((char *)&v12->ToFloatPtr()[0] + v13)*/ )
        {
          v14 = this->edgeIndex[v11 + v34->firstEdge];
          v15 = &this->edges[abs(v14)];
          v16 = &this->vertices[v15->vertexNum[(unsigned int)v14 >> 31]];
          v17 = &this->vertices[v15->vertexNum[v14 >= 0]];
          v12 = v37;
          v29 = *v17 - *v16;
        	float _v37 = *(v37 - 1);
          v38 = _v37 / (_v37 - *v37);
          v26 = v29 * v38;
          v23 = *v16 + v26;
          v19 = v23;
          v39 = a6->Distance(v23);
          if ( a2->distance > v39 )
          {
            a2->distance = v39;
            a2->edgeIndex = v14;
            a2->point = v19;
          }
          v11 = v36;
          if ( a3->distance < v39 )
          {
            a3->distance = v39;
            a3->edgeIndex = v14;
            a3->point = v19;
          }
        }
        ++v11;
        v12++;
      	_v12++;
        v36 = v11;
        v37 = v12;
        if ( v11 >= v34->numEdges )
          break;
      }
    }
    return true;
  }
  return result;
}

#endif
