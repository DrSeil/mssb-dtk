#ifndef __GAME_rep_1D58_H_
#define __GAME_rep_1D58_H_

#include "mssbTypes.h"

typedef struct StadiumObject StadiumObject;


typedef struct {
    u8 _pad[0x6C];
    u8 unk6C; // Offset 0x6C
} CommonBss_350E4;
extern CommonBss_350E4 lbl_3_common_bss_350E4;

void fn_3_B7FC8(void);
void fn_3_B80D0(void);
void fn_3_B8184(void);
struct StadiumObject* fn_3_B827C(void);
void fn_3_B828C(struct StadiumObject*);
void fn_3_B8298(void);
void fn_3_B8414(void);
void fn_3_B8464(void);
void fn_3_B8574(void);
void fn_3_B85A8(void);
void fn_3_B85DC(void);
s32 fn_3_B8658(f32*, f32*);
void fn_3_B867C(void);
void fn_3_B8828(void);
void fn_3_B8C08(void);
void fn_3_B902C(void);
void fn_3_B908C(void);
void fn_3_B9124(void);
int processStadiumObjectFunction(int, struct StadiumObject*, int, struct StadiumObject*);
void fn_3_B91C8(void);
void fn_3_B939C(void);
void fn_3_B93C4(void);
void fn_3_B93C8(void);
void fn_3_B93CC(void);
void fn_3_B950C(void);
void fn_3_B9510(int);
void fn_3_B9524(void);
void fn_3_B9534(void);
void fn_3_B95EC(void);
void fn_3_B97C8(struct StadiumObject*);
void fn_3_B97DC(void);
void fn_3_B98E8(void);
void fn_3_B99E4(void);
void fn_3_B9BB4(void);
void fn_3_B9D68(void);
void fn_3_B9FB8(void);

#endif // !__GAME_rep_1D58_H_
