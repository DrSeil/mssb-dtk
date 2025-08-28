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
bool checkTriangleCollisions(TriangleCollisionStruct* collisionData, TriangleGroup* TriangleGroup) {
    bool ret = false;

    

    return ret;
}
