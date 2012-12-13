#pragma once
#include <string>

extern "C" {
#include "tolua++.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

using namespace std;

class LuaWrapper
{
public:
	LuaWrapper(string luaScript);
	~LuaWrapper();

	float GetNumber(string function);
	string GetString(string function);
	lua_State* GetLua();
private:
	lua_State* mLuaState;
	string mScriptName;
};