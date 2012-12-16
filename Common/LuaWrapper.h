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

	void Reload();

	float GetTableNumber(string table, string element, int n = 0);
	string  GetTableString(string table, string element);
	float GetNumber(string function);
	string GetString(string function);
	lua_State* GetLua();
private:
	lua_State* mLuaState;
	string mScriptName;
};