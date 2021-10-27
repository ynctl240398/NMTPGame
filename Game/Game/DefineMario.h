#pragma once
//mario prop
#define MARIO_GRAVITY 0.0005f

//state syntax 1 + 9xx
#define STATE_MARIO_IDLE 1901
#define STATE_MARIO_WALK 1902
#define STATE_MARIO_JUMP 1903
#define STATE_MARIO_SIT 1904
#define STATE_MARIO_DIE 1905

//small syntax 1 + 0xx
#define ID_SMALL_MARIO_ANI_IDLE 1000
#define ID_SMALL_MARIO_ANI_WALK 1001

//big syntax 1 + 1xx
#define ID_BIG_MARIO_ANI_IDLE 1100
#define ID_BIG_MARIO_ANI_WALK 1101

//super syntax 1 + 2xx
#define ID_SUPER_MARIO_ANI_IDLE 1200
#define ID_SUPER_MARIO_ANI_WALK 1201

//level
#define LEVEL_SMALL "small"
#define LEVEL_BIG "big"
#define LEVEL_SUPER "supper"

//direction
#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 1