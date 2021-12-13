#pragma once
//mario prop
#define MARIO_WALKING_SPEED						0.1f
#define MARIO_RUNNING_SPEED						0.2f

#define MARIO_ACCEL_WALK_X						0.0005f
#define MARIO_ACCEL_RUN_X						0.0006f

#define MARIO_JUMP_SPEED_Y						0.8f
#define MARIO_JUMP_RUN_SPEED_Y					0.9f

#define MARIO_GRAVITY							0.002f

#define MARIO_BIG_BBOX_WIDTH					19
#define MARIO_BIG_BBOX_HEIGHT					28
#define MARIO_BIG_SITTING_BBOX_WIDTH			19
#define MARIO_BIG_SITTING_BBOX_HEIGHT			18

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2)

#define MARIO_SMALL_BBOX_WIDTH					16
#define MARIO_SMALL_BBOX_HEIGHT					15

#define ZONE 5
#define MARIO_JUMP_DEFLECT_SPEED				0.5f
#define MARIO_RUN_DEFLECT_SPEED					0.5f

#define MARIO_UNTOUCHABLE_TIME					2500

//state syntax 1 + 9xx
#define STATE_MARIO_IDLE						1901
#define STATE_MARIO_WALK						1902
#define STATE_MARIO_RUN							1903
#define STATE_MARIO_JUMP						1904
#define STATE_MARIO_SIT							1905
#define STATE_MARIO_DIE							1906
#define STATE_MARIO_RELEASE_JUMP				1907
#define STATE_MARIO_RELEASE_SIT					1908
#define STATE_MARIO_KICK						1909

//syntax x
#define ID_ANI_MARIO_IDLE						0
#define ID_ANI_MARIO_WALK						1
#define ID_ANI_MARIO_JUMP_WALK					2
#define ID_ANI_MARIO_RUN						3
#define ID_ANI_MARIO_SIT						4
#define ID_ANI_MARIO_BRACE						5
#define ID_ANI_MARIO_DIE						6
#define ID_ANI_MARIO_JUMP_RUN					7
#define ID_ANI_MARIO_KICK						8

//small syntax 1 + 0xx
//big syntax 1 + 1xx
//supper syntax 1 + 2xx

//level syntax 1 + 81x
#define LEVEL_SMALL								1810
#define LEVEL_BIG								1811
#define LEVEL_SUPER								1812
#define LEVEL_FIRER								1813

//direction
#define DIRECTION_LEFT							1
#define DIRECTION_RIGHT							-1