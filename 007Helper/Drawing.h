#pragma once
#include "007Helper.h"
#include "GameDef.h"


class Drawing
{

public:
	void _fastcall FillArea(float Pos_x, float Pos_y, float Width, float Height, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha);
	void _fastcall Box(float Pos_x, float Pos_y, float Width, float Height, float LineWidth, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha);
	void	DrawCircle(float position[2], float points, float radius, int r, int g, int b, int a);
	void _fastcall Circle(float Pos_x, float Pos_y, float Radius, int Amount, float LineWidth, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha);
	
	void DrawLiner(int x1, int y1, int x2, int y2, int lw, ColorEntry *pColor);
	void DrawVectorLine(float *flSrc, float *flDestination, int lw, ColorEntry *pColor);
	void Draw3DBox3(cl_entity_s *pEnt, ColorEntry *pColor, Vector origin);
	void Draw3DBox2(cl_entity_s *pEnt, ColorEntry *pColor, Vector origin);
	void Draw3DBox(cl_entity_s *pEnt, ColorEntry *pColor, Vector origin);
	void DrawBox(int x, int y, int r, int g, int b, int alpha, int radius, int thickness);
	void Draw3DBox(cl_entity_s* pEnt, ColorEntry* pColor);
	


	void blackBorder(int x, int y, int w, int h);
	void Print(int x, int y, int r, int g, int b, int a, BYTE flags, char *strText);


};






extern Drawing g_Drawing;

class Font
{
public:
	Font();
	void _fastcall InitText(char* Font, int Height, int Width);
	void  Print(float Pos_x, float Pos_y, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, BYTE Flags, char* String, ...);

	void  Print2(float Pos_x, float Pos_y, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, BYTE Flags, fmt::string_view format, fmt::format_args args);

	template <typename S, typename... Args>
	void FmtPrint(float Pos_x, float Pos_y, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, BYTE Flags, const S& format, Args&&...args)
	{
		Print2(Pos_x, Pos_y, Red, Green, Blue, Alpha, Flags,format,
			fmt::make_args_checked<Args...>(format, args...));
	}

	void  DrawString(int x, int y, int r, int g, int b, const char* fmt, ...);

	template <typename S, typename... Args>
	void  FmtDrawString(int x, int y, int r, int g, int b, const S& format, Args&&...args)
	{
		DrawString2(x, y,r,g,b, format, fmt::make_args_checked<Args...>(format, args...));
	}
	void  DrawString2(int x, int y, int r, int g, int b, fmt::string_view format, fmt::format_args args);

	
	template <typename S, typename... Args>
	void  FmtDrawHudString(int x, int y, int r, int g, int b, int holdtime, const S& format, Args&&...args)
	{
		DrawHunString2(x, y, r, g, b, holdtime, format, fmt::make_args_checked<Args...>(format, args...));
	}

	void  DrawHunString2(int x, int y, int r, int g, int b, int holdtime, fmt::string_view format, fmt::format_args args);

private:
	GLuint g_FontListID;
	int cHeight, cWidth[255];
	client_textmessage_t* m_HudText;
	pfnUserMsgHook        m_HudTextArgs;
	inline void Render(float Pos_x, float Pos_y, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, char *String);
	pfnUserMsgHook GetpfnUserMsg(const char* Msg);
	
	PackWriter*  m_pHudTextArgsPack;

};



extern Font g_Font, g_Verdana;
