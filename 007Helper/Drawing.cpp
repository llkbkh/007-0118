#include "Drawing.h"

void ColorEntry::fill_missing()
{

	compiled = (r << 24) + (g << 16) + (b << 8) + a;
	fr = float(r) / 255.0 / 30.0;
	fg = float(g) / 255.0 / 30.0;
	fb = float(b) / 255.0 / 30.0;
	onebased_r = float(r) / 255.0;
	onebased_g = float(g) / 255.0;
	onebased_b = float(b) / 255.0;
	onebased_a = float(a) / 255.0;
}

void _fastcall Drawing::FillArea(float Pos_x, float Pos_y, float Width, float Height, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha)
{
	
	Engine::g_Engine.pfnFillRGBA(Pos_x, Pos_y, Width, Height, Red, Green, Blue, Alpha);
}
void Drawing::DrawCircle(float position[2], float points, float radius, int r, int g, int b, int a)
{

	float step = (float)M_PI * 2.0f / points;

	float start[2], end[2];

	for (float x = 0; x < (M_PI * 2.0f); x += step)
	{
		start[0] = radius * cosf(x) + position[0];
		start[1] = radius * sinf(x) + position[1];
		end[0] = radius * cosf(x + step) + position[0];
		end[1] = radius * sinf(x + step) + position[1];


		ColorEntry Color;

		Color.r = r;
		Color.g = g;
		Color.b = b;
		Color.a = a;

		DrawLiner(start[0], start[1], end[0], end[1], radius, &Color);
	}
}

void _fastcall Drawing::Circle(float Pos_x, float Pos_y, float Radius, int Amount, float LineWidth, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha)
{

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(Red, Green, Blue, Alpha);
	glLineWidth(LineWidth);
	glBegin(GL_LINE_LOOP);

	for (uint16_t i = 0; i < Amount; ++i)
	{
		float Angle = 2 * 3.1415926 * float(i) / float(Amount);

		float x = Radius * cosf(Angle);
		float y = Radius * sinf(Angle);

		glVertex2f(Pos_x + x, Pos_y + y);
	}

	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
void _fastcall Drawing::Box(float Pos_x, float Pos_y, float Width, float Height, float LineWidth, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha)
{
	
	FillArea(Pos_x, Pos_y, Width, LineWidth, Red, Green, Blue, Alpha);
	FillArea(Pos_x + Width - LineWidth, Pos_y + LineWidth, LineWidth, Height - LineWidth, Red, Green, Blue, Alpha);
	FillArea(Pos_x, Pos_y + LineWidth, LineWidth, Height - LineWidth, Red, Green, Blue, Alpha);
	FillArea(Pos_x + LineWidth, Pos_y + Height - LineWidth, Width - LineWidth * 2, LineWidth, Red, Green, Blue, Alpha);
}
void Drawing::Draw3DBox2(cl_entity_s *pEnt, ColorEntry *pColor, Vector origin)
{
	
	if (!pEnt || !pColor)
		return;

	vec3_t vF, vR, vU;

	Engine::g_Engine.pfnAngleVectors(Vector(0.0f, pEnt->angles[1], 0.0f), vF, vR, vU);

	//If you want only the hitbox of the player. simply make it only maxs & mins instead of adding numbers:P
	//3d
	float flForward = pEnt->curstate.maxs.y + 0.0f;
	float flBack = pEnt->curstate.mins.y - 0.0f;
	float flRight = pEnt->curstate.maxs.x + 0.0f;
	float flLeft = pEnt->curstate.mins.x - 0.0f;

	//3d
	vec3_t vUFLeft = origin + vU * pEnt->curstate.maxs.z + vF * flForward + vR * flLeft; // vUFLeft = Top left front
	vec3_t vUFRight = origin + vU * pEnt->curstate.maxs.z + vF * flForward + vR * flRight; // vUFRight = Top right front
	vec3_t vUBLeft = origin + vU * pEnt->curstate.maxs.z + vF * flBack + vR * flLeft; // vUBLeft = Top left back
	vec3_t vUBRight = origin + vU * pEnt->curstate.maxs.z + vF * flBack + vR * flRight; // vUBRight = Top right back
	vec3_t vBFLeft = origin + vU * pEnt->curstate.mins.z + vF * flForward + vR * flLeft; // vBFLeft = Bottom left front
	vec3_t vBFRight = origin + vU * pEnt->curstate.mins.z + vF * flForward + vR * flRight; // vBFRight = Bottom right front
	vec3_t vBBLeft = origin + vU * pEnt->curstate.mins.z + vF * flBack + vR * flLeft; // vBBLeft = Bottom left back
	vec3_t vBBRight = origin + vU * pEnt->curstate.mins.z + vF * flBack + vR * flRight; // vBBRight = Bottom right back

																						//Top Box
	DrawVectorLine(vUBLeft, vUBRight, 2, pColor); // Back left -> Back right
	DrawVectorLine(vUBRight, vUFRight, 2, pColor); // Back right -> Front right
	DrawVectorLine(vUFRight, vUFLeft, 2, pColor); // Front right -> Front left
	DrawVectorLine(vUFLeft, vUBLeft, 2, pColor); // Front left -> Back right
												 //Mid Box
	DrawVectorLine(vUBLeft, vBBLeft, 2, pColor); // Top left -> Bottom left
	DrawVectorLine(vUBRight, vBBRight, 2, pColor); // Top right -> Bottom right
	DrawVectorLine(vUFRight, vBFRight, 2, pColor); // Top right -> Bottom right
	DrawVectorLine(vUFLeft, vBFLeft, 2, pColor); // Top left -> Bottom left
												 //Bottom Box
	DrawVectorLine(vBBLeft, vBBRight, 2, pColor); // Back left -> Back right
	DrawVectorLine(vBBRight, vBFRight, 2, pColor); // Back right -> Front right
	DrawVectorLine(vBFRight, vBFLeft, 2, pColor); // Front right -> Front left
	DrawVectorLine(vBFLeft, vBBLeft, 2, pColor); // Front left -> Back right
}

void Drawing::DrawVectorLine(float *flSrc, float *flDestination, int lw, ColorEntry *pColor)
{

	vec3_t vScreenSrc, vScreenDest;

	if (!WorldToScreen(flSrc, vScreenSrc) || !WorldToScreen(flDestination, vScreenDest))
		return;
	DrawLiner((int)vScreenSrc.x, (int)vScreenSrc.y, (int)vScreenDest.x, (int)vScreenDest.y, lw, pColor);
}
void Drawing::DrawLiner(int x1, int y1, int x2, int y2, int lw, ColorEntry *pColor)
{
	
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(pColor->r, pColor->g, pColor->b, pColor->a);
	glLineWidth((float)lw);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
void Drawing::Draw3DBox(cl_entity_s *pEnt, ColorEntry *pColor)
{
	

	if (!pEnt || !pColor)
		return;

	vec3_t vF, vR, vU;

	Engine::g_Engine.pfnAngleVectors(Vector(0.0f, pEnt->angles[1], 0.0f), vF, vR, vU);

	//If you want only the hitbox of the player. simply make it only maxs & mins instead of adding numbers:P
	float flForward = pEnt->curstate.maxs.y + 15.0f;
	float flBack = pEnt->curstate.mins.y - 5.0f;
	float flRight = pEnt->curstate.maxs.x + 5.0f;
	float flLeft = pEnt->curstate.mins.x - 5.0f;

	vec3_t vUFLeft = pEnt->origin + vU * pEnt->curstate.maxs.z + vF * flForward + vR * flLeft; // vUFLeft = Top left front
	vec3_t vUFRight = pEnt->origin + vU * pEnt->curstate.maxs.z + vF * flForward + vR * flRight; // vUFRight = Top right front
	vec3_t vUBLeft = pEnt->origin + vU * pEnt->curstate.maxs.z + vF * flBack + vR * flLeft; // vUBLeft = Top left back
	vec3_t vUBRight = pEnt->origin + vU * pEnt->curstate.maxs.z + vF * flBack + vR * flRight; // vUBRight = Top right back

	vec3_t vBFLeft = pEnt->origin + vU * pEnt->curstate.mins.z + vF * flForward + vR * flLeft; // vBFLeft = Bottom left front
	vec3_t vBFRight = pEnt->origin + vU * pEnt->curstate.mins.z + vF * flForward + vR * flRight; // vBFRight = Bottom right front
	vec3_t vBBLeft = pEnt->origin + vU * pEnt->curstate.mins.z + vF * flBack + vR * flLeft; // vBBLeft = Bottom left back
	vec3_t vBBRight = pEnt->origin + vU * pEnt->curstate.mins.z + vF * flBack + vR * flRight; // vBBRight = Bottom right back

																							  //Top Box
	DrawVectorLine(vUBLeft, vUBRight, 1, pColor); // Back left -> Back right
	DrawVectorLine(vUBRight, vUFRight, 1, pColor); // Back right -> Front right
	DrawVectorLine(vUFRight, vUFLeft, 1, pColor); // Front right -> Front left
	DrawVectorLine(vUFLeft, vUBLeft, 1, pColor); // Front left -> Back right

												 //Mid Box
	DrawVectorLine(vUBLeft, vBBLeft, 1, pColor); // Top left -> Bottom left
	DrawVectorLine(vUBRight, vBBRight, 1, pColor); // Top right -> Bottom right
	DrawVectorLine(vUFRight, vBFRight, 1, pColor); // Top right -> Bottom right
	DrawVectorLine(vUFLeft, vBFLeft, 1, pColor); // Top left -> Bottom left

												 //Bottom Box
	DrawVectorLine(vBBLeft, vBBRight, 1, pColor); // Back left -> Back right
	DrawVectorLine(vBBRight, vBFRight, 1, pColor); // Back right -> Front right
	DrawVectorLine(vBFRight, vBFLeft, 1, pColor); // Front right -> Front left
	DrawVectorLine(vBFLeft, vBBLeft, 1, pColor); // Front left -> Back right
}
void Drawing::Draw3DBox3(cl_entity_s *pEnt, ColorEntry *pColor, Vector origin)
{
	
	if (!pEnt || !pColor)
		return;

	vec3_t vF, vR, vU;
	static int Count = 0;
	Count++;
	if (Count == 0)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(0.0f, pEnt->angles[1], 0.0f), vF, vR, vU);
	}
	else if (Count == 1)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-36.0f, pEnt->angles[1], 360.0f), vF, vR, vU);
	}
	else if (Count == 2)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-72.0f, pEnt->angles[1], 324.0f), vF, vR, vU);
	}
	else if (Count == 3)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-108.0f, pEnt->angles[1], 288.0f), vF, vR, vU);
	}
	else if (Count == 4)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-180.0f, pEnt->angles[1], 252.0f), vF, vR, vU);
	}
	else if (Count == 5)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-216.0f, pEnt->angles[1], 216.0f), vF, vR, vU);
	}
	else if (Count == 6)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-252.0f, pEnt->angles[1], 180.0f), vF, vR, vU);
	}
	else if (Count == 7)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-288.0f, pEnt->angles[1], 108.0f), vF, vR, vU);
	}
	else if (Count == 8)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-324.0f, pEnt->angles[1], 72.0f), vF, vR, vU);
	}
	else if (Count == 9)
	{
		Engine::g_Engine.pfnAngleVectors(Vector(-360.0f, pEnt->angles[1], 36.0f), vF, vR, vU);
		Count = 0;
	}

	//If you want only the hitbox of the player. simply make it only maxs & mins instead of adding numbers:P
	//3d
	float flForward = pEnt->curstate.maxs.y + 25.0f;
	float flBack = pEnt->curstate.mins.y - 25.0f;
	float flRight = pEnt->curstate.maxs.x + 25.0f;
	float flLeft = pEnt->curstate.mins.x - 25.0f;

	//3d
	vec3_t vUFLeft = origin + vU * pEnt->curstate.maxs.z + vF * flForward + vR * flLeft; // vUFLeft = Top left front
	vec3_t vUFRight = origin + vU * pEnt->curstate.maxs.z + vF * flForward + vR * flRight; // vUFRight = Top right front
	vec3_t vUBLeft = origin + vU * pEnt->curstate.maxs.z + vF * flBack + vR * flLeft; // vUBLeft = Top left back
	vec3_t vUBRight = origin + vU * pEnt->curstate.maxs.z + vF * flBack + vR * flRight; // vUBRight = Top right back
	vec3_t vBFLeft = origin + vU * pEnt->curstate.mins.z + vF * flForward + vR * flLeft; // vBFLeft = Bottom left front
	vec3_t vBFRight = origin + vU * pEnt->curstate.mins.z + vF * flForward + vR * flRight; // vBFRight = Bottom right front
	vec3_t vBBLeft = origin + vU * pEnt->curstate.mins.z + vF * flBack + vR * flLeft; // vBBLeft = Bottom left back
	vec3_t vBBRight = origin + vU * pEnt->curstate.mins.z + vF * flBack + vR * flRight; // vBBRight = Bottom right back

																						//Top Box
	DrawVectorLine(vUBLeft, vUBRight, 1, pColor); // Back left -> Back right
	DrawVectorLine(vUBRight, vUFRight, 1, pColor); // Back right -> Front right
	DrawVectorLine(vUFRight, vUFLeft, 1, pColor); // Front right -> Front left
	DrawVectorLine(vUFLeft, vUBLeft, 1, pColor); // Front left -> Back right
												 //Mid Box
	DrawVectorLine(vUBLeft, vBBLeft, 1, pColor); // Top left -> Bottom left
	DrawVectorLine(vUBRight, vBBRight, 1, pColor); // Top right -> Bottom right
	DrawVectorLine(vUFRight, vBFRight, 1, pColor); // Top right -> Bottom right
	DrawVectorLine(vUFLeft, vBFLeft, 1, pColor); // Top left -> Bottom left
												 //Bottom Box
	DrawVectorLine(vBBLeft, vBBRight, 1, pColor); // Back left -> Back right
	DrawVectorLine(vBBRight, vBFRight, 1, pColor); // Back right -> Front right
	DrawVectorLine(vBFRight, vBFLeft, 1, pColor); // Front right -> Front left
	DrawVectorLine(vBFLeft, vBBLeft, 1, pColor); // Front left -> Back right
}
void Drawing::Draw3DBox(cl_entity_s *pEnt, ColorEntry *pColor, Vector origin)
{


	if (!pEnt || !pColor)
		return;

	vec3_t vF, vR, vU;

	Engine::g_Engine.pfnAngleVectors(Vector(90.0f, pEnt->angles[1], 0.0f), vF, vR, vU);

	//If you want only the hitbox of the player. simply make it only maxs & mins instead of adding numbers:P

	//2d
	float flForward2 = pEnt->curstate.maxs.y + 50.0f;
	float flBack2 = pEnt->curstate.mins.y - 50.0f;
	float flRight2 = pEnt->curstate.maxs.x + 25.0f;
	float flLeft2 = pEnt->curstate.mins.x - 25.0f;

	//2d
	vec3_t vUBLeft2 = origin + vU * pEnt->curstate.maxs.z + vF * flBack2 + vR * flLeft2; // vUBLeft = Top left back
	vec3_t vUBRight2 = origin + vU * pEnt->curstate.maxs.z + vF * flBack2 + vR * flRight2; // vUBRight = Top right back
	vec3_t vUFLeft2 = origin + vU * pEnt->curstate.maxs.z + vF * flForward2 + vR * flLeft2; // vUFLeft = Top left front
	vec3_t vUFRight2 = origin + vU * pEnt->curstate.maxs.z + vF * flForward2 + vR * flRight2; // vUFRight = Top right front

																							  //Top Box2d
	DrawVectorLine(vUBLeft2, vUBRight2, 2, pColor); // Back left -> Back right
	DrawVectorLine(vUBRight2, vUFRight2, 2, pColor); // Back right -> Front right
	DrawVectorLine(vUFRight2, vUFLeft2, 2, pColor); // Front right -> Front left
	DrawVectorLine(vUFLeft2, vUBLeft2, 2, pColor); // Front left -> Back right
}
void Drawing::blackBorder(int x, int y, int w, int h)
{
	Engine::g_Engine.pfnFillRGBA(x - 1, y - 1, w + 2, 1, 0, 0, 0, 254); //top
	Engine::g_Engine.pfnFillRGBA(x - 1, y, 1, h - 1, 0, 0, 0, 254);	//left
	Engine::g_Engine.pfnFillRGBA(x + w, y, 1, h - 1, 0, 0, 0, 254);	//right
	Engine::g_Engine.pfnFillRGBA(x - 1, y + h - 1, w + 2, 1, 0, 0, 0, 254); //bottom

}
void Drawing::DrawBox(int x, int y, int r, int g, int b, int alpha, int radius, int thickness)
{

	int radius2 = radius << 1;
	Engine::g_Engine.pfnFillRGBA(x - radius + thickness, y - radius, radius2 - 2, thickness, r, g, b, alpha);
	Engine::g_Engine.pfnFillRGBA(x - radius, y - radius, thickness, radius2, r, g, b, alpha);
	Engine::g_Engine.pfnFillRGBA(x - radius, y + radius, radius2, thickness, r, g, b, alpha);
	Engine::g_Engine.pfnFillRGBA(x + radius, y - radius, thickness, radius2 + thickness, r, g, b, alpha);
}
void Drawing::Print(int x, int y, int r, int g, int b, int a, BYTE flags, char *strText)
{

	Engine::g_Engine.pfnDrawSetTextColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);

	if (flags & FL_CENTER)
	{
		int iWidth, iHeight;
		Engine::g_Engine.pfnDrawConsoleStringLen(strText, &iWidth, &iHeight);
		Engine::g_Engine.pfnDrawConsoleString(x - iWidth / 2, y, strText);
	}
	else
		Engine::g_Engine.pfnDrawConsoleString(x, y, strText);
	return;
}





void Font::Render(float Pos_x, float Pos_y, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, char *String)
{
	int i;
	for (i = 0; Pos_x < 0; ++i)
	{
		Pos_x += cWidth[String[i]];

		if (!String[i])
		{
			return;
		}
	}
	glColor4ub(Red, Green, Blue, Alpha);
	glRasterPos2f(Pos_x, Pos_y);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPushAttrib(GL_LIST_BIT);
	glListBase(g_FontListID);
	glCallLists(strlen(String) - i, GL_UNSIGNED_BYTE, String + i);
	glPopAttrib();
}
void Font::DrawString(int x, int y, int r, int g, int b, const char *fmt, ...)
{

	va_list va_alist;
	char buf[MAXBYTE];
	va_start(va_alist, fmt);
	_vsnprintf(buf, sizeof(buf), fmt, va_alist);
	va_end(va_alist);
	Engine::g_Engine.pfnDrawSetTextColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
	Engine::g_Engine.pfnDrawConsoleString(x, y, buf);
}

void Font::DrawString2(int x, int y, int r, int g, int b, fmt::string_view format, fmt::format_args args)
{
	std::string Text = vformat(format, args);

	Engine::g_Engine.pfnDrawSetTextColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);

	Engine::g_Engine.pfnDrawConsoleString(x, y, Text.data());
}
pfnUserMsgHook Font::GetpfnUserMsg(const char* Msg)
{
	PUserMsg Ptr = (PUserMsg)*(PDWORD)(Engine::g_pUserMsgBase);

	while (Ptr->next)
	{
		if (!lstrcmp(Ptr->name, Msg))
		{

			return Ptr->pfn;
		}

		Ptr = Ptr->next;
	}

	return nullptr;
}
void Font::DrawHunString2(int x, int y, int r, int g, int b, int holdtime, fmt::string_view format, fmt::format_args args)
{
	static std::string Text;

	Text = vformat(format, args);

	//ÐÞ¸ÄHunText
	if (!m_HudText || !m_HudTextArgs)
	{
		m_HudText     = Engine::g_pEngine->pfnTextMessageGet(xorstr_("tuto07"));
		m_HudTextArgs = GetpfnUserMsg(xorstr_("HudTextArgs"));
	}

	m_HudText->x = x;
	m_HudText->y = y;
	m_HudText->r1 = r;
	m_HudText->g1 = g;
	m_HudText->b1 = b;
	m_HudText->holdtime = holdtime;
	m_HudText->pMessage = Text.data();

	m_pHudTextArgsPack->Clear();
	m_pHudTextArgsPack->WriteString(xorstr_("tuto07"));

	m_HudTextArgs(xorstr_("HudTextArgs"), m_pHudTextArgsPack->GetBytesLength(), m_pHudTextArgsPack->GetBytes());
}



void Font::Print(float Pos_x, float Pos_y, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, BYTE Flags, char *String, ...)
{

	char newText[256];
	va_list ArgumentPtr;
	va_start(ArgumentPtr, String);
	vsprintf_s(newText, String, ArgumentPtr);
	va_end(ArgumentPtr);
	DrawString(Pos_x, Pos_y - 10, Red, Green, Blue, newText);
}

void Font::Print2(float Pos_x, float Pos_y, BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, BYTE Flags, fmt::string_view format, fmt::format_args args)
{
	std::string Text = vformat(format, args);

	FmtDrawString(Pos_x, Pos_y - 10, Red, Green, Blue, Text);
}

Font::Font()
{
	m_HudText     = nullptr;
	m_HudTextArgs = nullptr;
	m_pHudTextArgsPack = new PackWriter(1024);
}

void Font::InitText(char* Font, int Height, int Width)
{
	HDC hDC;
	HFONT hFont, hOldFont;
	hDC = wglGetCurrentDC();
	g_FontListID = glGenLists(256);

	hFont = CreateFontA(Height, Width, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET,
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, CLEARTYPE_NATURAL_QUALITY, FF_DONTCARE | VARIABLE_PITCH, Font);

	hOldFont = (HFONT)SelectObject(hDC, hFont);

	if (!wglUseFontBitmaps(hDC, 0, 255, g_FontListID))
	{
		wglUseFontBitmaps(hDC, 0, 255, g_FontListID);
	}

	for (BYTE i = 0; i < 255; ++i)
	{
		SIZE s;

		char Line[2] = { (char)i, 0 };

		GetTextExtentPoint(hDC, Line, 1, &s);

		cWidth[i] = s.cx;
		cHeight = s.cy;
	}

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

}
Font g_Font, g_Verdana;
Drawing g_Drawing;


