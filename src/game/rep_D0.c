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
    bool ret = false;
    u32 remainingTriangles;
    u32 isNegativeSlope;
    u32 isCurrentTriangleStrip;
    Vec v5; Vec *p_v5 = &v5;
    Vec v2; Vec *p_v2 = &v2;
    Vec v1; Vec *p_v1 = &v1;
    Vec cross; Vec *p_cross = &cross;
    Vec v3; Vec *p_v3 = &v3;
    Vec v4; Vec *p_v4 = &v4;
    f32 d;
    while (true) {
        CollisionTriangle* currentTri;
        remainingTriangles = triangleGroup->count;
        if (remainingTriangles == 0) break;
        
        currentTri = triangleGroup->tris;
        if (triangleGroup->isTriangleList == false) {
            do {
                PSMTXMultVec(collisionData->mtx1, &currentTri[0].trianglePoint, &v1);
                PSMTXMultVec(collisionData->mtx1, &currentTri[1].trianglePoint, p_v2);
                PSMTXMultVec(collisionData->mtx1, &currentTri[2].trianglePoint, p_v3);
                if ((v1.x * v2.y - v2.x * v1.y) >= 0.f & (v2.x * v3.y - v3.x * v2.y) >= 0.f &
                    (v3.x * v1.y - v1.x * v3.y) >= 0.f) {
                    PSVECSubtract(p_v2, &v1, &v4);
                    PSVECSubtract(p_v3, p_v2, p_v5);
                    PSVECCrossProduct(&v4, p_v5, p_cross);
                    d = -PSVECDotProduct(p_cross, &v1) / p_cross->z;
                    if (d >= 0.f && collisionData->collisionDistance > d) {
                        collisionData->collisionDistance = d;
                        ret = true;
                        collisionData->collisionType = currentTri[2].collisionType;
                        collisionData->normal = *p_cross;
                    }
                }
                triangleGroup += 3;
                // currentTri = triangleGroup->tris;
            } while (--remainingTriangles);
        } else {
            CollisionTriangle* pTris;
            isNegativeSlope = false;
            PSMTXMultVec(collisionData->mtx1, &currentTri[0].trianglePoint, &v1);
            PSMTXMultVec(collisionData->mtx1, &triangleGroup->tris[1].trianglePoint, p_v2);
            pTris = triangleGroup->tris;
            do {
                PSMTXMultVec(collisionData->mtx1, &pTris->trianglePoint, p_v3);
                p_v4->z = p_v3->x * p_v1->y - p_v1->x * p_v3->y;
                p_v4->y = p_v2->x * p_v3->y - p_v3->x * p_v2->y;
                p_v4->z = p_v1->x * p_v2->y - p_v2->x * p_v1->y;
                if (isNegativeSlope) {
                    isCurrentTriangleStrip = p_v4->z <= 0.f & p_v4->x <= 0.f & p_v4->y <= 0.f;
                } else {
                    isCurrentTriangleStrip = p_v4->z >= 0.f & p_v4->x >= 0.f & p_v4->y >= 0.f;
                }
                
                if (isCurrentTriangleStrip)
                {
                    PSVECSubtract(p_v2, &v1, &v4);
                    PSVECSubtract(p_v3, p_v2, p_v5);
                    PSVECCrossProduct(&v4, p_v5, p_cross);
                    d = -PSVECDotProduct(p_cross, &v1) / cross.z;
                    if (d >= 0.f && collisionData->collisionDistance > d) {
                        collisionData->collisionDistance = d;
                        collisionData->collisionType = currentTri[2].collisionType;
                        if (!isNegativeSlope) {
                            collisionData->normal = cross;
                        } else {
                            collisionData->normal.x = -cross.x;
                            collisionData->normal.y = -cross.y;
                            collisionData->normal.z = -cross.z;
                        }
                        ret = true;
                    }
                }
                v1 = v2;
                v2 = v3;
                remainingTriangles--;
                isNegativeSlope ^= 1;
                triangleGroup++;
            } while(remainingTriangles);
        }
    }

    return ret;
}
