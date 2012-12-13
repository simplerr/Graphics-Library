#include "LuaWrapper.h"
#include "lua_Particle.h"

LuaWrapper::LuaWrapper(string luaScript)
{
	mScriptName = luaScript;
	mLuaState = lua_open();

	luaL_openlibs(mLuaState);
	tolua_open(mLuaState);

	tolua_Particle_open(mLuaState);
	luaL_dofile(mLuaState, mScriptName.c_str());
}

LuaWrapper::~LuaWrapper()
{
	lua_close(mLuaState);
}

float LuaWrapper::GetNumber(string function)
{
	lua_getglobal(mLuaState, function.c_str());
	lua_pcall(mLuaState, 0, 1, 0);
	return lua_tonumber(mLuaState, -1);
}

string LuaWrapper::GetString(string function)
{
	lua_getglobal(mLuaState, function.c_str());
	lua_pcall(mLuaState, 0, 1, 0);
	return string(lua_tostring(mLuaState, -1));
}

lua_State* LuaWrapper::GetLua()
{
	return mLuaState;
}