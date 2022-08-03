#pragma once
#include "007Helper.h"
#include "GameDef.h"
#include "IGameClient.h"

class GameMenu :public IGameClient
{
private:

	inline void Add(char* Name, BYTE Red, BYTE Green, BYTE Blue, bool Space, bool Active);
	inline void Add(std::string Name, BYTE Red, BYTE Green, BYTE Blue, bool Space, bool Active);
	inline void AddSub(char* Name, float* Value, float Min, float Max, float Step, bool Float, bool Space);
	inline void AddSub(std::string Name, float* Value, float Min, float Max, float Step, bool Float, bool Space);
	inline void Clear();

	struct M_Entry
	{
		std::string Name;

		BYTE Color[3];

		bool Space, Value, Active;
	};

	struct S_Entry
	{
		std::string Name;

		float* Value, Step;
		float Min, Max;

		bool Float, Space;
	};
#define MAX_MENU_COUNT			250
#define MAX_SUB_MENU_COUNT		250

	M_Entry MenuEntry[MAX_MENU_COUNT];
	S_Entry SubmEntry[MAX_SUB_MENU_COUNT];


	int M_EntryPos, M_EntrySize;
	int S_EntryPos, S_EntrySize;

	bool SubMenu, Visible;

	void  Initialize();

	std::string GetInMenuSubItem(std::string Item);
public:
	GameMenu();
	// ≥ı ºªØ
	virtual bool vInit();

	virtual void vHUD_Redraw(float time, int intermission);

	virtual int  vHUD_Key_Event(int down, int keynum, const char* pszCurrentBinding);

	void  Draw(int Pos_x, int Pos_y);
	void  HUD_Key_Event(int keynum);

};