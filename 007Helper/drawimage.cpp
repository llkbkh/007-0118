#include "drawimage.h"

#include"cSprites.h"

DrawImageItem g_rgDrawImage[MAX_DRAWIMAGE];
float g_flTime, g_fOldTime, g_flTimeDelta;

void DrawImageInitialize(void)
{
	memset(g_rgDrawImage, 0, sizeof(g_rgDrawImage));
}

void DrawImageAdd(DrawImageItem rgTempDrawImage)
{
	if (rgTempDrawImage.iChanne <= 0 || rgTempDrawImage.iChanne >= MAX_DRAWIMAGE)
	{
		for (int i = 0; i < MAX_DRAWIMAGE; i++)
		{
			if (!g_rgDrawImage[i].flEndDisplayTime)
			{
				g_rgDrawImage[i] = rgTempDrawImage;
				break;
			}
		}
	}
	else
		g_rgDrawImage[rgTempDrawImage.iChanne] = rgTempDrawImage;
}
void ScaleColors(int& r, int& g, int& b, int a)
{
	float x = (float)a / 255;
	r = (int)(r * x);
	g = (int)(g * x);
	b = (int)(b * x);
}
void DrawImageRedraw()
{
	for (int i = 0; i < MAX_DRAWIMAGE; i++)
	{
		if (!g_rgDrawImage[i].flEndDisplayTime)
			continue;

		if (g_flTime >= g_rgDrawImage[i].flEndDisplayTime)
		{
			g_rgDrawImage[i].flEndDisplayTime = 0.0;
			continue;
		}

		int r, g, b, a;
		r = g_rgDrawImage[i].color.r;
		g = g_rgDrawImage[i].color.g;
		b = g_rgDrawImage[i].color.b;

		switch (g_rgDrawImage[i].iMode)
		{
			case 1:
			{
				float flHasDisplayTime = g_flTime - g_rgDrawImage[i].flStartDisplayTime;
				float flNeedDisplayTime = g_rgDrawImage[i].flEndDisplayTime - g_rgDrawImage[i].flStartDisplayTime;
				a = int(flHasDisplayTime / flNeedDisplayTime * 255);
				ScaleColors(r, g, b, a);
				break;
			}
			case 2:
			{
				float flHasDisplayTime = g_flTime - g_rgDrawImage[i].flStartDisplayTime;
				float flNeedDisplayTime = g_rgDrawImage[i].flEndDisplayTime - g_rgDrawImage[i].flStartDisplayTime;
				a = 255 - int(flHasDisplayTime / flNeedDisplayTime * 255);
				ScaleColors(r, g, b, a);
				break;
			}
			case 3:
			{
				float flHasDisplayTime = g_flTime - g_rgDrawImage[i].flStartDisplayTime;
				float flNeedDisplayTime = g_rgDrawImage[i].flEndDisplayTime - g_rgDrawImage[i].flStartDisplayTime;
				a = int(flHasDisplayTime / flNeedDisplayTime * 510);
				if (a > 255)
					a = 255 - (a - 255);
				ScaleColors(r, g, b, a);
				break;
			}
		}
		
		if(g_rgDrawImage[i].iCheck>0)
		{
			g_rgDrawImage[i].hl_hSprites = g_Sprite.GethSpr(g_rgDrawImage[i].iSprIndex);
		}
		

		Engine::g_Engine.pfnSPR_Set(g_rgDrawImage[i].hl_hSprites, r, g, b);
		
		if (!g_rgDrawImage[i].iCenter)
		{
			switch (g_rgDrawImage[i].iFunction)
			{
				case 0:
				{
					if(g_rgDrawImage[i].iCheck>0)
					{
						if(g_rgDrawImage[i].iLength>-1)
						{
							wrect_t *TempRect;
							TempRect = &g_Sprite.GetRc (g_rgDrawImage[i].iSprIndex);
							TempRect->right = g_rgDrawImage[i].iLength;
							Engine::g_Engine.pfnSPR_Draw(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y,TempRect);
						}
						else
						{
							Engine::g_Engine.pfnSPR_Draw(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y,&g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex));
						}
					}
					else
					{
						Engine::g_Engine.pfnSPR_Draw(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y, NULL);
					}
					break;
				}
				case 1:
				{
					if(g_rgDrawImage[i].iCheck>0)
					{
						if(g_rgDrawImage[i].iLength>-1)
						{
							wrect_t *TempRect;
							TempRect = &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex);
							TempRect->right = g_rgDrawImage[i].iLength;
							Engine::g_Engine.pfnSPR_DrawAdditive(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y, TempRect);
						}
						else
						{
							Engine::g_Engine.pfnSPR_DrawAdditive(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y, &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex));
						}
					}
					else
					{
						Engine::g_Engine.pfnSPR_DrawAdditive(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y, NULL);
					}
					break;
				}
				case 2:
				{
					if(g_rgDrawImage[i].iCheck>0)
					{
						if(g_rgDrawImage[i].iLength>-1)
						{
							wrect_t *TempRect;
							TempRect = &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex);
							TempRect->right = g_rgDrawImage[i].iLength;
							Engine::g_Engine.pfnSPR_DrawHoles(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y, TempRect);
						}
						else
						{
							Engine::g_Engine.pfnSPR_DrawHoles(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y, &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex));
						}
					}
					else
					{
						Engine::g_Engine.pfnSPR_DrawHoles(0, g_rgDrawImage[i].x, g_rgDrawImage[i].y, NULL);
					}
					break;
				}
			}
		}
		else
		{
			switch (g_rgDrawImage[i].iFunction)
			{
				case 0:
				{
					if(g_rgDrawImage[i].iCheck>0)
					{
						if(g_rgDrawImage[i].iLength>-1)
						{
							wrect_t *TempRect;
							TempRect = &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex);
							TempRect->right = g_rgDrawImage[i].iLength;
							Engine::g_Engine.pfnSPR_Draw(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, TempRect);
						}
						else
						{
							Engine::g_Engine.pfnSPR_Draw(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex));
						}
					}
					else
					{
						Engine::g_Engine.pfnSPR_Draw(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, NULL);
					}
					break;
				}
				case 1:
				{
					if(g_rgDrawImage[i].iCheck>0)
					{
						if(g_rgDrawImage[i].iLength>-1)
						{
							wrect_t *TempRect;
							TempRect = &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex);
							TempRect->right = g_rgDrawImage[i].iLength;
							Engine::g_Engine.pfnSPR_DrawAdditive(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, TempRect);
						}
						else
						{
							Engine::g_Engine.pfnSPR_DrawAdditive(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex));
						}
					}
					else
					{
						Engine::g_Engine.pfnSPR_DrawAdditive(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, NULL);
					}
					break;
				}
				case 2:
				{
					if(g_rgDrawImage[i].iCheck>0)
					{
						if(g_rgDrawImage[i].iLength>-1)
						{
							wrect_t *TempRect;
							TempRect = &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex);
							TempRect->right = g_rgDrawImage[i].iLength;
							Engine::g_Engine.pfnSPR_DrawHoles(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, TempRect);
						}
						else
						{
							Engine::g_Engine.pfnSPR_DrawHoles(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, &g_Sprite.GetRc(g_rgDrawImage[i].iSprIndex));
						}
					}
					else
					{
						Engine::g_Engine.pfnSPR_DrawHoles(0, g_rgDrawImage[i].x - Engine::g_Engine.pfnSPR_Width(g_rgDrawImage[i].hl_hSprites, 0) / 2, g_rgDrawImage[i].y - Engine::g_Engine.pfnSPR_Height(g_rgDrawImage[i].hl_hSprites, 0) / 2, NULL);
					}
					break;
				}
			}
		}
	}
}