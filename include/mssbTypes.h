#ifndef __MSSB_TYPES_H_
#define __MSSB_TYPES_H_

#include "types.h"

#define ARRAY_COUNT(x) (sizeof(x) / (*(x)))
#define OFFSET_OF(structName, field) (size_t)(&(((structName*)NULL)->field))

// used to pad structs when there are unused variables/still filling out the struct
#define artificial_padding(lastOffset, currentOffset, typeOfLastMember) \
u8 __##currentOffset##padding[(currentOffset) - (lastOffset) - sizeof(typeOfLastMember)]

#define S8_MAX (0x7F)
#define S8_MIN (-0x80)
#define U8_MAX (0xFF)
#define U8_MIN (0)

#define S16_MAX (0x7FFF)
#define S16_MIN (-0x8000)
#define U16_MAX (0xFFFF)
#define U16_MIN (0)

#define S32_MAX (0x7FFFFFFF)
#define S32_MIN (-0x80000000)
#define U32_MAX (0xFFFFFFFF)
#define U32_MIN (0)

// Enum macro to help debug types
#define E(storage, enumType) storage

typedef enum _CHAR_ID {
    /* -1   -1 */ CHAR_ID_NONE = -1,
    /* 0  0x00 */ CHAR_ID_MARIO,            
    /* 1  0x01 */ CHAR_ID_LUIGI,            
    /* 2  0x02 */ CHAR_ID_DK,               
    /* 3  0x03 */ CHAR_ID_DIDDY,            
    /* 4  0x04 */ CHAR_ID_PEACH,            
    /* 5  0x05 */ CHAR_ID_DAISY,            
    /* 6  0x06 */ CHAR_ID_YOSHI,            
    /* 7  0x07 */ CHAR_ID_BABYMARIO,        
    /* 8  0x08 */ CHAR_ID_BABYLUIGI,        
    /* 9  0x09 */ CHAR_ID_BOWSER,           
    /* 10 0x0A */ CHAR_ID_WARIO,            
    /* 11 0x0B */ CHAR_ID_WALUIGI,          
    /* 12 0x0C */ CHAR_ID_KOOPA_GREEN,      
    /* 13 0x0D */ CHAR_ID_TOAD_RED,         
    /* 14 0x0E */ CHAR_ID_BOO,              
    /* 15 0x0F */ CHAR_ID_TOADETTE,         
    /* 16 0x10 */ CHAR_ID_SHYGUY_RED,       
    /* 17 0x11 */ CHAR_ID_BIRDO,            
    /* 18 0x12 */ CHAR_ID_MONTY,            
    /* 19 0x13 */ CHAR_ID_BOWSERJR,         
    /* 20 0x14 */ CHAR_ID_PARATROOPA_RED,   
    /* 21 0x15 */ CHAR_ID_PIANTA_BLUE,      
    /* 22 0x16 */ CHAR_ID_PIANTA_RED,       
    /* 23 0x17 */ CHAR_ID_PIANTA_YELLOW,    
    /* 24 0x18 */ CHAR_ID_NOKI_BLUE,        
    /* 25 0x19 */ CHAR_ID_NOKI_RED,         
    /* 26 0x1A */ CHAR_ID_NOKI_GREEN,       
    /* 27 0x1B */ CHAR_ID_BRO_HAMMER,       
    /* 28 0x1C */ CHAR_ID_TOADSWORTH,       
    /* 29 0x1D */ CHAR_ID_TOAD_BLUE,        
    /* 30 0x1E */ CHAR_ID_TOAD_YELLOW,      
    /* 31 0x1F */ CHAR_ID_TOAD_GREEN,       
    /* 32 0x20 */ CHAR_ID_TOAD_PURPLE,      
    /* 33 0x21 */ CHAR_ID_MAGIKOOPA_BLUE,   
    /* 34 0x22 */ CHAR_ID_MAGIKOOPA_RED,    
    /* 35 0x23 */ CHAR_ID_MAGIKOOPA_GREEN,  
    /* 36 0x24 */ CHAR_ID_MAGIKOOPA_YELLOW, 
    /* 37 0x25 */ CHAR_ID_KINGBOO,          
    /* 38 0x26 */ CHAR_ID_PETEY,            
    /* 39 0x27 */ CHAR_ID_DIXIE,            
    /* 40 0x28 */ CHAR_ID_GOOMBA,           
    /* 41 0x29 */ CHAR_ID_PARAGOOMBA,       
    /* 42 0x2A */ CHAR_ID_KOOPA_RED,        
    /* 43 0x2B */ CHAR_ID_PARATROOPA_GREEN, 
    /* 44 0x2C */ CHAR_ID_SHYGUY_BLUE,      
    /* 45 0x2D */ CHAR_ID_SHYGUY_YELLOW,    
    /* 46 0x2E */ CHAR_ID_SHYGUY_GREEN,     
    /* 47 0x2F */ CHAR_ID_SHYGUY_BLACK,     
    /* 48 0x30 */ CHAR_ID_DRYBONES_GRAY,    
    /* 49 0x31 */ CHAR_ID_DRYBONES_GREEN,   
    /* 50 0x32 */ CHAR_ID_DRYBONES_RED,     
    /* 51 0x33 */ CHAR_ID_DRYBONES_BLUE,    
    /* 52 0x34 */ CHAR_ID_BRO_FIRE,         
    /* 53 0x35 */ CHAR_ID_BRO_BOOMERANG,    
    /* 54 0x36 */ NUM_CHOOSABLE_CHARACTERS,
    /* 53 0x35 */ CHAR_ID_MAX = NUM_CHOOSABLE_CHARACTERS - 1,
    /*  0  0x0 */ CHAR_ID_MIN = CHAR_ID_NONE + 1
} CHAR_ID;

typedef enum _BATTING_HAND {
    /* 0x0 */ BATTING_HAND_RIGHT,
    /* 0x1 */ BATTING_HAND_LEFT,
} BATTING_HAND;

typedef enum _REGULAR_STAR_SWING {
    /* 0x0 */ REGULAR_STAR_SWING_NONE,
    /* 0x1 */ REGULAR_STAR_SWING_POPFLY,
    /* 0x2 */ REGULAR_STAR_SWING_GROUNDER,
    /* 0x3 */ REGULAR_STAR_SWING_LINEDRIVE,
} REGULAR_STAR_SWING;

typedef enum _CHARACTER_CLASS {
    /* 0x0 */ CHARACTER_CLASS_BALANCED,
    /* 0x1 */ CHARACTER_CLASS_POWER,
    /* 0x2 */ CHARACTER_CLASS_SPEED,
    /* 0x3 */ CHARACTER_CLASS_TECHNIQUE,
} CHARACTER_CLASS;

typedef enum _BAT_CONTACT_TYPE {
    /* 0x0 */ BAT_CONTACT_TYPE_SLAP,
    /* 0x1 */ BAT_CONTACT_TYPE_CHARGE,
    /* 0x2 */ BAT_CONTACT_TYPE_STAR,
    /* 0x3 */ BAT_CONTACT_TYPE_BUNT,
} BAT_CONTACT_TYPE;

typedef enum _HIT_CONTACT_TYPE {
    /*  -1 */ HIT_CONTACT_TYPE_NONE = -1,
    /* 0x0 */ HIT_CONTACT_TYPE_RIGHT_SOUR,
    /* 0x1 */ HIT_CONTACT_TYPE_RIGHT_NICE,
    /* 0x2 */ HIT_CONTACT_TYPE_PERFECT,
    /* 0x3 */ HIT_CONTACT_TYPE_LEFT_NICE,
    /* 0x4 */ HIT_CONTACT_TYPE_LEFT_SOUR,
} HIT_CONTACT_TYPE;

typedef enum _HIT_TYPE {
    /*   -1 */ HIT_TYPE_SLAP_NONE = -1,
    /* 0x00 */ HIT_TYPE_SLAP_SOUR = HIT_TYPE_SLAP_NONE,
    /* 0x01 */ HIT_TYPE_SLAP_NICE,
    /* 0x02 */ HIT_TYPE_SLAP_PERFECT,
    /* 0x03 */ HIT_TYPE_CHARGE,
    /* 0x03 */ HIT_TYPE_CHARGE_SOUR = HIT_TYPE_CHARGE,
    /* 0x04 */ HIT_TYPE_CHARGE_NICE,
    /* 0x05 */ HIT_TYPE_CHARGE_PERFECT,
    /* 0x06 */ HIT_TYPE_CHANGE_UP,
    /* 0x06 */ HIT_TYPE_CHANGE_UP_SOUR = HIT_TYPE_CHANGE_UP,
    /* 0x07 */ HIT_TYPE_CHANGE_UP_NICE,
    /* 0x08 */ HIT_TYPE_CHANGE_UP_PERFECT,
    /* 0x09 */ HIT_TYPE_CHANGE_UP_CHARGE,
    /* 0x09 */ HIT_TYPE_CHANGE_UP_CHARGE_SOUR = HIT_TYPE_CHANGE_UP_CHARGE,
    /* 0x0A */ HIT_TYPE_CHANGE_UP_CHARGE_NICE,
    /* 0x0B */ HIT_TYPE_CHANGE_UP_CHARGE_PERFECT,
    /* 0x0C */ HIT_TYPE_PERFECT_PITCH,
    /* 0x0C */ HIT_TYPE_PERFECT_PITCH_SOUR = HIT_TYPE_PERFECT_PITCH,
    /* 0x0D */ HIT_TYPE_PERFECT_PITCH_NICE,
    /* 0x0E */ HIT_TYPE_PERFECT_PITCH_PERFECT,
    /* 0x0F */ HIT_TYPE_PERFECT_PITCH_CHARGE,
    /* 0x0F */ HIT_TYPE_PERFECT_PITCH_CHARGE_SOUR = HIT_TYPE_PERFECT_PITCH_CHARGE,
    /* 0x10 */ HIT_TYPE_PERFECT_PITCH_CHARGE_NICE,
    /* 0x11 */ HIT_TYPE_PERFECT_PITCH_CHARGE_PERFECT,
} HIT_TYPE;

typedef enum _BUNT_STATUS {
    /* 0x0 */ BUNT_STATUS_NONE,
    /* 0x1 */ BUNT_STATUS_STARTING,
    /* 0x2 */ BUNT_STATUS_SHOWING,
    /* 0x3 */ BUNT_STATUS_STRIKE,
    /* 0x4 */ BUNT_STATUS_LATE,
    /* 0x5 */ BUNT_STATUS_RETREATING,
    /* 0x6 */ BUNT_STATUS_6,
    /* 0x7 */ BUNT_STATUS_7,
} BUNT_STATUS;

typedef enum _HIT_TRAJECTORY {
    /* 0x0 */ HIT_TRAJECTORY_0,     // no hit
    /* 0x2 */ HIT_TRAJECTORY_2 = 2, // grounder
    /* 0x3 */ HIT_TRAJECTORY_3,     // line drive
    /* 0x4 */ HIT_TRAJECTORY_4,     // fly ball?/bOD_notVeryFoul
    /* 0x5 */ HIT_TRAJECTORY_5,     // bunt
    /* 0x6 */ HIT_TRAJECTORY_6,     // fair bund? bOD bunt / bB bunt
} HIT_TRAJECTORY;

typedef enum _DID_SWING_TYPE {
    /* 0x0 */ DID_SWING_TYPE_NONE,
    /* 0x1 */ DID_SWING_TYPE_SWING,
    /* 0x2 */ DID_SWING_TYPE_BUNT,
} DID_SWING_TYPE;

typedef enum _CHARGE_SWING_STAGE {
    /* 0x0 */ CHARGE_SWING_STAGE_NONE,
    /* 0x1 */ CHARGE_SWING_STAGE_CHARGEUP,
    /* 0x2 */ CHARGE_SWING_STAGE_SWING,
    /* 0x3 */ CHARGE_SWING_STAGE_RELEASE,
} CHARGE_SWING_STAGE;

typedef enum _CAPTAIN_STAR_TYPE {
    /* 0x0 */ CAPTAIN_STAR_TYPE_NONE,
    /* 0x1 */ CAPTAIN_STAR_TYPE_MARIO,
    /* 0x2 */ CAPTAIN_STAR_TYPE_LUIGI,
    /* 0x3 */ CAPTAIN_STAR_TYPE_WARIO,
    /* 0x4 */ CAPTAIN_STAR_TYPE_WALUIGI,
    /* 0x5 */ CAPTAIN_STAR_TYPE_DK,
    /* 0x6 */ CAPTAIN_STAR_TYPE_DIDDY,
    /* 0x7 */ CAPTAIN_STAR_TYPE_BOWSER,
    /* 0x8 */ CAPTAIN_STAR_TYPE_BOWSERJR,
    /* 0x9 */ CAPTAIN_STAR_TYPE_YOSHI,
    /* 0xA */ CAPTAIN_STAR_TYPE_BIRDO,
    /* 0xB */ CAPTAIN_STAR_TYPE_PEACH,
    /* 0xC */ CAPTAIN_STAR_TYPE_DAISY,
} CAPTAIN_STAR_TYPE;

typedef enum _STICK_SIDEWAYS_INPUT {
    /* 0x0 */ STICK_SIDEWAYS_INPUT_NONE,
    /* 0x1 */ STICK_SIDEWAYS_INPUT_TOWARDS,
    /* 0x2 */ STICK_SIDEWAYS_INPUT_AWAY,
} STICK_SIDEWAYS_INPUT;

typedef enum _BALL_HITTABLE {
    /* 0x0 */ BALL_HITTABLE_NONE,
    /* 0x0 */ BALL_HITTABLE_IN_AIR = BALL_HITTABLE_NONE,
    /* 0x1 */ BALL_HITTABLE_HITTABLE,
    /* 0x2 */ BALL_HITTABLE_UNHITTABLE,
} BALL_HITTABLE;

typedef enum _FORCE_OUT_TYPE {
    /* 0x0 */ FORCE_OUT_TYPE_NOT_FORCED_TO_ADVANCE,
    /* 0x1 */ FORCE_OUT_TYPE_FORCED_TO_ADVANCE,
    /* 0x2 */ FORCE_OUT_TYPE_OUT_ON_FORCE,
} FORCE_OUT_TYPE;

typedef enum _RUNNER_STATUS {
    /* 0x0 */ RUNNER_STATUS_NONE,
    /* 0x1 */ RUNNER_STATUS_ON_FIELD,
    /* 0x2 */ RUNNER_STATUS_OUT_DURING_PLAY,
    /* 0x3 */ RUNNER_STATUS_SCORED_DURING_PLAY,
    /* 0x4 */ RUNNER_STATUS_SCORED_DEAD_BALL, // (walkoff or other)/batterWhilePickoff
    /* 0x5 */ RUNNER_STATUS_WALK_WHILE_STEALING,
} RUNNER_STATUS;

typedef enum _TAG_UP_TYPE { TAG_UP_TYPE_NONE, TAG_UP_TYPE_IN_AIR, TAG_UP_TYPE_TAGGED } TAG_UP_TYPE;

typedef enum _PREVENT_PASSING_TYPE {
    /* 0x0 */PREVENT_PASSING_TYPE_NONE = 0,
    /* 0x1 */PREVENT_PASSING_TYPE_PREVENT_FORWARD = 1,
    /* 0x2 */PREVENT_PASSING_TYPE_PREVENT_BACKWARD = 2,
    /* 0x4 */PREVENT_PASSING_TYPE_PREVENT_CROSSINGPLATE = 4,
    /* 0x8 */PREVENT_PASSING_TYPE_TAG_UP = 8, // unsure
} PREVENT_PASSING_TYPE;

typedef enum _BALL_RESULT_TYPE {
    /*  -1 */ BALL_RESULT_TYPE_FOUL = -1,
    /* 0x0 */ BALL_RESULT_TYPE_IN_AIR,
    /* 0x1 */ BALL_RESULT_TYPE_LANDED,
    /* 0x2 */ BALL_RESULT_TYPE_FIELDED,
    /* 0x3 */ BALL_RESULT_TYPE_CAUGHT
} BALL_RESULT_TYPE;

typedef s16 frame_t;
typedef s16 sAng;

typedef enum _GAME_TYPE {
    GAME_TYPE_EXHIBITION_GAME = 0,
    GAME_TYPE_PRACTICE = 2,
    GAME_TYPE_DEMO = 4,
    GAME_TYPE_CHALLENGE = 5,
    GAME_TYPE_TOY_FIELD = 6,
    GAME_TYPE_MINIGAMES = 7,
} GAME_TYPE;

typedef enum _STADIUM_ID {
    STADIUM_ID_MARIO_STADIUM,
    STADIUM_ID_BOWSERS_CASTLE,
    STADIUM_ID_WARIO_PALACE,
    STADIUM_ID_YOHSI_PARK,
    STADIUM_ID_PEACH_GARDEN,
    STADIUM_ID_DK_JUNGLE,
    STADIUM_ID_TOY_FIELD,
} STADIUM_ID;

#ifndef __cplusplus
typedef u8 bool;
#define true TRUE
#define false FALSE
#endif

#endif // !__MSSB_TYPES_H_
