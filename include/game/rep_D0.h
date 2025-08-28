#ifndef __GAME_rep_D0_H_
#define __GAME_rep_D0_H_

#include "mssbTypes.h"
#include "Dolphin/vec.h"
#include "Dolphin/mtx.h"

typedef struct _VecSrcDst {
    Vec src, dst;
} VecSrcDst;

typedef struct _CollisionStruct {
    Vec position, normal;
} CollisionStruct;

typedef struct _AABB_Box {
    Vec a, b;
} AABB_Box;

typedef struct _CollisionBox {
    AABB_Box* boundingBox;
} CollisionBox;

typedef struct _UNK_StadiumCollision {
    void* _00;
    artificial_padding(0, 0x70a, void*);
    s16 numCollisionBoxes;
    CollisionBox* pCollisionBoxes;
} UNK_StadiumCollision; // size: 0x780

typedef struct _TriangleCollisionStruct {
    /*0x00*/ Mtx mtx1;
    /*0x30*/ Mtx mtx2;
    /*0x60*/ Vec normal;
    /*0x6C*/ f32 distance; // unsure
    /*0x70*/ f32 collisionDistance; // unsure
    /*0x72*/ E(u16, BALL_COLLISION_TYPE) collisionType;
} TriangleCollisionStruct; // size: 0x74

typedef struct _CollisionTriangle{
    /*0x00*/ Vec trianglePoint;
    /*0x0C*/ E(u16, BALL_COLLISION_TYPE) collisionType;
    // pad 2 bytes
} CollisionTriangle; // size:0x10

typedef struct _TriangleGroup {
    /*0x00*/ u8 _0;
    /*0x01*/ bool isTriangleList;
    /*0x02*/ s16 count;
    /*0x04*/ CollisionTriangle tris[];
} TriangleGroup; // size: at least 4, expandable

extern UNK_StadiumCollision g_UNK_StadiumDetails; 

typedef enum _BALL_COLLISION_TYPE {
    BALL_COLLISION_TYPE_NONE,
    BALL_COLLISION_TYPE_GRASS,
    BALL_COLLISION_TYPE_WALL,
    BALL_COLLISION_TYPE_STRUCTURE,
    BALL_COLLISION_TYPE_FOUL_LINE,
    BALL_COLLISION_TYPE_UNCLIMBABLE_WALL,
    BALL_COLLISION_TYPE_DIRT,
    BALL_COLLISION_TYPE_PIT_WALL,
    BALL_COLLISION_TYPE_PIT,
    BALL_COLLISION_TYPE_ROUGH_TERRAIN,
    BALL_COLLISION_TYPE_WATER,
    BALL_COLLISION_TYPE_CHOMP_HAZARD,
    BALL_COLLISION_TYPE_FOUL = 0x80,
} BALL_COLLISION_TYPE;

BALL_COLLISION_TYPE checkCollision(VecSrcDst* inVec, CollisionStruct* outCollision, int collisionCheckType, BOOL useBallCoords);
BALL_COLLISION_TYPE checkStatiumHazardCollisions(VecSrcDst* inVec, CollisionStruct* outCollision, Vec* v);
BALL_COLLISION_TYPE didCollideWithBoundingBoxes(VecSrcDst* inVec, CollisionStruct* outCollision, CollisionBox* boxes,
                                                int boxCount);
bool checkTriangleCollisions(TriangleCollisionStruct* collisionData, TriangleGroup* TriangleGroup);

#endif // !__GAME_rep_D0_H_
