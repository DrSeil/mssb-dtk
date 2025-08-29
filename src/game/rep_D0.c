#include "game/rep_D0.h"
#include "header_rep_data.h"
#include "Dolphin/stl.h"
#include "game/UnknownHomes.h"
#include "game/rep_1D58.h"




// .text:0x00000914 size:0x158 mapped:0x8063F9A8
BALL_COLLISION_TYPE checkCollision(VecSrcDst* inVec, CollisionStruct* outCollision, int collisionCheckType,
                                   BOOL useBallCoords) {
    VecSrcDst p;
    Vec v;
    BALL_COLLISION_TYPE ret;
    if (collisionCheckType) {
        if (useBallCoords && (gameInitVariables.StadiumID == STADIUM_ID_WARIO_PALACE ||
                              gameInitVariables.StadiumID == STADIUM_ID_YOHSI_PARK ||
                              gameInitVariables.StadiumID == STADIUM_ID_DK_JUNGLE)) {
            memcpy(&p.src, &inMemBall.AtBat_Contact_BallPos, sizeof(p.src));
            memcpy(&p.dst, &inMemBall.pastCoordinates[4], sizeof(p.dst));
            p.src.y *= -1.f;
            p.dst.y *= -1.f;
        } else {
            memcpy(&p, inVec, sizeof(p));
        }
        ret = checkStatiumHazardCollisions(&p, outCollision, &v);
        if (ret) {
            if (collisionCheckType != 2 || ret & BALL_COLLISION_TYPE_FOUL) {
                ret = BALL_COLLISION_TYPE_NONE;
            } else {
                processStadiumObjectFunction();
            }
        }
    }
    if (collisionCheckType != 3) {
        ret = didCollideWithBoundingBoxes(inVec, outCollision, g_UNK_StadiumDetails.pCollisionBoxes,
                                          g_UNK_StadiumDetails.numCollisionBoxes);
    }
    return ret;
}

// .text:0x00000A6C size:0x384 mapped:0x8063FB00
BALL_COLLISION_TYPE checkStatiumHazardCollisions(VecSrcDst* inVec, CollisionStruct* outCollision, Vec* v) {
    return;
}

// .text:0x00000DF0 size:0x2F0 mapped:0x8063FE84
BALL_COLLISION_TYPE didCollideWithBoundingBoxes(VecSrcDst* inVec, CollisionStruct* outCollision, CollisionBox* boxes,
                                                int boxCount) {
    return;
}

// .text:0x000010E0 size:0x3C4 mapped:0x80640174
bool checkTriangleCollisions(TriangleCollisionStruct* collisionData, TriangleGroup* triangleGroup) {
    #define O_GROUP ((TriangleGroup*)triangleGroup)
    #define O_TRI ((CollisionTriangle*)triangleGroup)
    // r25 = ret
    // r30 = 0x44(r1) = tri_v3
    // r31 = 0x38(r1) = tri_v2
    // 0x2c(r1) = tri_v1
    // r28 = 0x20(r1) = cross
    // r29 = 0x14(r1) = dist_v3v2
    // 0x8(r1) = dist_v2v1
    // r24 = O_GROUP
    // r23 = collisionData
    Vec tri_v3;
    Vec tri_v2;
    Vec tri_v1;
    Vec cross;
    Vec dist_v3v2; 
    Vec dist_v2v1;
    Vec *pTri_v2;
    Vec *pTri_v3;
    Vec* pDist_v3v2;
    Vec* pCross;
    u32 remainingTriangles;
    u32 didVecPassTriangle;
    u32 isBackwardsTriangle;
    bool ret;
    bool isList;
    f32 d;
    ret = false;
    while (true) {
        pTri_v2 = &tri_v2;
        pTri_v3 = &tri_v3;
        pDist_v3v2 = &dist_v3v2;
        pCross = &cross;
        remainingTriangles = O_GROUP->count;
        if (remainingTriangles == 0) break;

        // this should be reassigned to r24? but they're different types?
        // i hope it's not a union
        isList = O_GROUP->isTriangleList;
        O_TRI = O_GROUP->tris;
        if (!isList) {
            // groups of 3 verts to make up a triangle
            do {
                PSMTXMultVec(collisionData->mtx1, &O_TRI[0].trianglePoint, &tri_v1);
                PSMTXMultVec(collisionData->mtx1, &O_TRI[1].trianglePoint, pTri_v2);
                PSMTXMultVec(collisionData->mtx1, &O_TRI[2].trianglePoint, pTri_v3);
                didVecPassTriangle = tri_v1.x * pTri_v2->y - pTri_v2->x * tri_v1.y >= 0;
                didVecPassTriangle &= pTri_v2->x * pTri_v3->y - pTri_v3->x * pTri_v2->y >= 0;
                didVecPassTriangle &= pTri_v3->x * tri_v1.y - tri_v1.x * pTri_v3->y >= 0;
                if (didVecPassTriangle) {
                    PSVECSubtract(pTri_v2, &tri_v1, &dist_v2v1);
                    PSVECSubtract(pTri_v3, pTri_v2, pDist_v3v2);
                    PSVECCrossProduct(&dist_v2v1, pDist_v3v2, pCross);
                    d = -PSVECDotProduct(pCross, &tri_v1) / pCross->z;
                    if (d >= 0.f && collisionData->collisionDistance > d) {
                        collisionData->collisionDistance = d;
                        ret = true;
                        collisionData->collisionType = O_TRI[2].collisionType;
                        collisionData->normal = *pCross;
                    }
                }
                O_TRI += 3;
            } while (--remainingTriangles);
        } else {
            // the first 3 verts describe a triangle, after that each new vert replaces the oldest vert to make a triangle fan
            isBackwardsTriangle = false;
            PSMTXMultVec(collisionData->mtx1, &O_TRI[0].trianglePoint, &tri_v1);
            PSMTXMultVec(collisionData->mtx1, &O_TRI[1].trianglePoint, pTri_v2);
            O_TRI += 2;
            do {
                PSMTXMultVec(collisionData->mtx1, &O_TRI->trianglePoint, pTri_v3);
                dist_v2v1.x = tri_v1.x * pTri_v2->y - pTri_v2->x * tri_v1.y;
                dist_v2v1.y = pTri_v2->x * pTri_v3->y - pTri_v3->x * pTri_v2->y;
                dist_v2v1.z = pTri_v3->x * tri_v1.y - tri_v1.x * pTri_v3->y;

                if (isBackwardsTriangle) {
                    didVecPassTriangle = dist_v2v1.x <= 0.f & dist_v2v1.y <= 0.f & dist_v2v1.z <= 0.f;
                } else {
                    didVecPassTriangle = dist_v2v1.x >= 0.f & dist_v2v1.y >= 0.f & dist_v2v1.z >= 0.f;
                }

                if (didVecPassTriangle) {
                    PSVECSubtract(pTri_v2, &tri_v1, &dist_v2v1);
                    PSVECSubtract(pTri_v3, pTri_v2, pDist_v3v2);
                    PSVECCrossProduct(&dist_v2v1, pDist_v3v2, pCross);
                    d = -PSVECDotProduct(pCross, &tri_v1) / cross.z;
                    if (d >= 0.f && collisionData->collisionDistance > d) {
                        collisionData->collisionDistance = d;
                        collisionData->collisionType = O_TRI[0].collisionType;
                        if (!isBackwardsTriangle) {
                            collisionData->normal = *pCross;
                        } else {
                            collisionData->normal.x = -pCross->x;
                            collisionData->normal.y = -pCross->y;
                            collisionData->normal.z = -pCross->z;
                        }
                        ret = true;
                    }
                }
                tri_v1 = *pTri_v2;
                *pTri_v2 = *pTri_v3;
                isBackwardsTriangle ^= 1;
                O_TRI++;
            } while (--remainingTriangles);
        }
        
    }

    return ret;
}
