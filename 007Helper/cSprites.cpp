//==============================================================================
/*
		drawing sprites from multiple-sprite files by name - by tabris
																			  */
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "cSprites.h"
spritedata_t spritedata;
#pragma warning(disable:4786)
#pragma warning(disable:4800)
#pragma warning(disable:4244)
#pragma warning(disable:4101)
#pragma warning(disable:4715)
#pragma warning(disable:4305)
Sprite g_Sprite;
//==============================================================================
void LoadSprites(client_sprite_t *pList, int iCount)
{
    int i;
    client_sprite_t *p;


    spritedata.iSpritesLoaded = 0;

    if(!pList)
    {
        return;
    }

    p = pList;

    i = iCount;

    while(i-- && p->szName != NULL && spritedata.iSpritesLoaded < 0x1000)
    {

		strcpy(spritedata.spriteinfo[spritedata.iSpritesLoaded].szName, p->szName);
		strcpy(spritedata.spriteinfo[spritedata.iSpritesLoaded].szSprite, p->szSprite);
	

		std::string Text =  fmt::format(xorstr_("sprites/{}.spr"), p->szSprite);

        spritedata.spriteinfo[spritedata.iSpritesLoaded].hspr = (int)Engine::g_Engine.pfnSPR_LoadEx(Text.data());

        memcpy(&(spritedata.spriteinfo[spritedata.iSpritesLoaded].rc), &(p->rc), sizeof(p->rc));
        spritedata.spriteinfo[spritedata.iSpritesLoaded].iRes = p->iRes;

        p++;
        spritedata.iSpritesLoaded++;

    }

	ACEDebugLog("[{}] iSpritesLoaded:{}",__FUNCTION__, spritedata.iSpritesLoaded);
}

HSPRITE Sprite::GethSpr(int Index)
{
	return (HSPRITE)spritedata.spriteinfo[Index].hspr;
}

BOOL Sprite::SpriteInit()
{
	spritedata.iSpritesLoaded = 0;
	int o = 1; // any value?
	hookpfnSPR_GetList(xorstr_("sprites/weapon_knife.txt"), &o);
	return TRUE;
}



//==============================================================================
client_sprite_t* _cdecl Sprite::hookpfnSPR_GetList(char *psz, int *piCount)
{
    client_sprite_t *ret;

    ret = Engine::g_Engine.pfnSPR_GetList(psz, piCount);
	client_sprite_t *pList;
    if(!strcmp(psz, xorstr_("sprites/weapon_knife.txt")))
    {
        int iCount;
        pList = Engine::g_Engine.pfnSPR_GetList(xorstr_("sprites/hud.txt"), &iCount);

		ACEDebugLog("[{}] WeaponHun iCount:{}",__FUNCTION__,iCount);

		//zbt_supplybox.spr
		if(pList)
		{
            LoadSprites(pList, iCount);
		}
    }
    return ret;
}
//==============================================================================

wrect_t& Sprite::GetRc(int Index)
{
	return spritedata.spriteinfo[Index].rc;
}
int Sprite::GetSpriteIndex(char *szName)
{
    int i;

    if(szName[0] == 0x00)
        return -1;

    for(i = 0; i < spritedata.iSpritesLoaded; i++)
    {
        if(spritedata.spriteinfo[i].iRes == 640)
        {
            if(strcmp(spritedata.spriteinfo[i].szName, szName)==0)
            {
                return i;
            }
        }
    }
	return -1;
}


void Sprite::DrawNumber(char* Number,int x,int y, int r, int g, int b, int Type)
{
	ULONG nLenght	= strlen(Number);

	for (int i = 0; i < nLenght; i++)
	{
		//取当前字符
		char Ascii = Number[i];

		std::string Text = fmt::format(xorstr_("dollarNum_{}_new"), Ascii - 0x30);

		int index = g_Sprite.GetSpriteIndex(Text.data());
		if (index != -1)
		{
			Engine::g_Engine.pfnSPR_Set((HSPRITE)spritedata.spriteinfo[index].hspr, r, g, b); // Get sprite ready
			if (Type == 0)
			{
				Engine::g_Engine.pfnSPR_DrawHoles(0, x + 15 * i, y, &(spritedata.spriteinfo[index].rc)); // Draw sprite
			}				
			else
			{
				Engine::g_Engine.pfnSPR_DrawAdditive(0, x + 15 * i, y, &(spritedata.spriteinfo[index].rc)); // Draw sprite
			}
		}
	}

}
/*
bunkerbuster_cooltime0		数字
dollarNum_0_new

*/
//==============================================================================
// End.
//==============================================================================