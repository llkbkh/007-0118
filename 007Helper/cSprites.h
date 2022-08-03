//==============================================================================
/*
		drawing sprites from multiple-sprite files by name - by tabris
																			  */
//==============================================================================
#pragma once
#include "007Helper.h"



#define MAX_SPRITES 200
//==============================================================================
//==============================================================================
typedef struct 
{ 
    char szName[256]; 
    char szSprite[256]; 
    int hspr; 
    wrect_t rc; 
    int iRes; 
} spriteinfo_t; 
//==============================================================================
typedef struct 
{ 
    int iSpritesLoaded; 
    spriteinfo_t spriteinfo[0x1000];
} spritedata_t;
//==============================================================================
extern spritedata_t spritedata;

class Sprite
{
public:
	int GetSpriteIndex(char *szName);
	client_sprite_t* _cdecl hookpfnSPR_GetList(char *psz, int *piCount);
	BOOL SpriteInit();
	void DrawNumber(char * Number,int x ,int y,int r ,int g , int b ,int Type);
	wrect_t& GetRc(int Index);
	HSPRITE  GethSpr(int Index);
private:

};

extern Sprite g_Sprite;

