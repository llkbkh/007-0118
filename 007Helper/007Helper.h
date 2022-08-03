#pragma once
#include <ACEConnect.h>
#pragma comment(lib,"RcfDll.lib")
#pragma comment(lib,"imm32.lib")

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <thread>
#include <PackWrite.h>
#include <PackReader.h>

#include "GameSDK.h"
#include "CheatStruct.h"

#include "GameVar.h"

#include "IGameClient.h"
#include "IGameEngineMSG.h"

#include "Vector.h"
#include "QAngle.h"

#pragma warning(disable: 4129)

#if ANTI_DEBUG
#define ODG_EXPORT			EXPORT		//µ÷ÊÔÓÃµÄ

#else

#define ODG_EXPORT			

#endif


bool WorldToScreen(float in[3], float out[2]);