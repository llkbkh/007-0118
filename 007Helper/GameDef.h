#pragma once

//***************************** CSO ¼ü´úÂë
#define K_1								49
#define K_2								50
#define K_3								51
#define K_4								52
#define K_5								53
#define K_6								54
#define K_7								55
#define K_8								56
#define K_9								57
#define K_0								48
#define K_Minus							45
#define K_TAB							9

#define K_C								99
#define K_E								101
#define K_F								102
#define K_K								107
#define K_L								108

#define K_P								112
#define K_R								114
#define K_T								116

#define K_X								120
#define K_Z								122

#define K_N_9							162
#define K_N_8							161
#define K_N_7							160
#define K_N_6							165
#define K_N_5							164
#define K_N_4							163
#define K_N_3							168
#define K_N_2							167
#define K_N_1							166
#define K_N_0							170

#define M_PI					3.14159265358979323846
#define POW(x) ((x)*(x))
#define PLAYER_FATAL_FALL_SPEED		980
#define PLAYER_MAX_SAFE_FALL_SPEED	504.8
#define DAMAGE_FOR_FALL_SPEED	(float) 100 / ( PLAYER_FATAL_FALL_SPEED - PLAYER_MAX_SAFE_FALL_SPEED )

#define VectorLength(a) sqrt(POW((a)[0])+POW((a)[1])+POW((a)[2]))
#define VectorLengthSquared(v) ((v)[0]*(v)[0]+(v)[1]*(v)[1]+(v)[2]*(v)[2])
#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}
#define VectorDistance(a,b) sqrt(POW((a)[0]-(b)[0])+POW((a)[1]-(b)[1])+POW((a)[2]-(b)[2]))

#define		MAX_PALYER		35	
#define		PLAYER_OFFSET	0x28

#define FL_NONE					0
#define FL_OUTLINE				1
#define FL_CENTER				2

enum class GameVer
{
	KR220224    = 388,
	KR220324	= 389,
	KR220421    = 391,
	KR220519    = 392,

	KRDefault	= KR220421,
};