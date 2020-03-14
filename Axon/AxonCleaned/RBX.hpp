#pragma once
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include "RetCheck.hpp"
extern "C" {
#include "Lua5.1\lua.h"
#include "Lua5.1\lua.hpp"
#include "Lua5.1\lualib.h"
#include "Lua5.1\lauxlib.h"
#include "Lua5.1\luaconf.h"
#include "Lua5.1\llimits.h"
}


#define x(x) (x - 4194304 + (DWORD)GetModuleHandleA(0))
#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)

DWORD rbx_state;
lua_State* vl_state;
std::vector<int> int3breakpoints;


/*
*
*Buy Sentinel, Use Fluxus: https://discord.gg/pATH72y
*Cleaned up by ShowerHead
*Everything you need to update is in this file.
*
*/


#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 2
#define R_LUA_TNUMBER 3
#define R_LUA_TBOOLEAN 1
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 8
#define R_LUA_TFUNCTION 5
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 6
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10

DWORD identity_offset = 128;

#define LuaState(v2) *(DWORD*)(v2 + 172) - (v2 + 172);

DWORD ScriptContext = x(0x1C49D48);

DWORD RBX_GetField = unprotect(x(0x7C80A0));
#define RBX_GetFieldC __cdecl

DWORD RBX_TolString = unprotect(x(0x7CA9D0));
#define RBX_TolStringC __cdecl

DWORD RBX_Settop = unprotect(x(0x7CA790));
#define RBX_SettopC __stdcall

DWORD RBX_ToBoolean = x(0x7CA910);
#define RBX_ToBooleanC __cdecl

DWORD RBX_PushValue = unprotect(x(0x7C9730));
#define RBX_PushValueC __fastcall

DWORD RBX_PushNumber = unprotect(x(0x7C9550));
#define RBX_PushNumberC __stdcall

DWORD RBX_PushString = x(0x7C95E0);
#define RBX_PushStringC __fastcall

DWORD RBX_PCall = unprotect(x(0x7C8FB0));
#define RBX_PCallC __cdecl

DWORD RBX_PushNil = unprotect(x(0x7C94E0));
#define RBX_PushNilC __cdecl

DWORD RBX_Next = unprotect(x(0x7C8D10));
#define RBX_NextC __cdecl

DWORD RBX_PushBoolean = unprotect(x(0x7C9080));
#define RBX_PushBooleanC __cdecl

DWORD RBX_ToNumber = unprotect(x(0x7CAB00));
#define RBX_ToNumberC __cdecl

DWORD RBX_PushCClosure = unprotect(x(0x7C9100));
#define RBX_PushCClosureC __stdcall

DWORD RBX_CreateTable = unprotect(x(0x7C7C70));
#define RBX_CreateTableC __cdecl

DWORD RBX_NewThread = unprotect(x(0x7C8AB0));
#define RBX_NewThreadC __cdecl

DWORD RBX_NewUserData = unprotect(x(0x7C8BF0));
#define RBX_NewUserDataC __cdecl

DWORD RBX_RawGeti = unprotect(x(0x7C99E0));
#define RBX_RawGetiC __cdecl

DWORD RBX_GetMetaTable = unprotect(x(0x7C8430));
#define RBX_GetMetaTableC __fastcall

DWORD RBX_ToUserData = unprotect(x(0x7CAC40));
#define RBX_ToUserDataC __cdecl

DWORD RBX_Type = x(0x7CACA0);
#define RBX_TypeC __cdecl

DWORD RBX_LuaL_Ref = unprotect(x(0x7C3180));
#define RBX_LuaL_RefC __cdecl

DWORD RBX_Settable = unprotect(x(0x7CA700));
#define RBX_SettableC __cdecl

DWORD RBX_PushLightUD = unprotect(x(0x7C93C0));
#define RBX_PushLightUDC __cdecl

DWORD RBX_Gettop = x(0x7C86A0);
#define RBX_GettopC __cdecl

typedef char(RBX_GetFieldC* rgetfield)(int a1, int a2, const char* a3);
rgetfield r_lua_getfield = (rgetfield)RBX_GetField;//up8

typedef char* (RBX_TolStringC* rtolstring)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)RBX_TolString;

typedef void(RBX_SettopC* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)RBX_Settop;//up8

typedef bool(RBX_ToBooleanC* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)RBX_ToBoolean;//up8

typedef void(RBX_PushValueC* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)RBX_PushValue;//up8

typedef double(RBX_PushNumberC* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)RBX_PushNumber;//up8

typedef void(RBX_PushStringC* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)RBX_PushString;//up8

typedef int(RBX_PCallC* rpnigger)(DWORD, int, int, int);
rpnigger r_lua_pcall = (rpnigger)RBX_PCall;

typedef void(RBX_PushNilC* pushnil)(DWORD);
pushnil r_lua_pushnil = (pushnil)RBX_PushNil;//up8

typedef DWORD(RBX_NextC* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)RBX_Next;//up8

typedef int(RBX_PushBooleanC* rboolean)(unsigned int, int);
rboolean r_lua_pushboolean = (rboolean)RBX_PushBoolean;//up8

typedef double(RBX_ToNumberC* rtnumber)(int a1, signed int a2, DWORD* a3);
rtnumber r_lua_tonumber = (rtnumber)RBX_ToNumber;//up8

typedef void(RBX_PushCClosureC* rpushcclosure)(DWORD, DWORD, DWORD, DWORD);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)RBX_PushCClosure;//up8

typedef void(RBX_CreateTableC* rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)RBX_CreateTable;//up8

typedef DWORD(RBX_NewThreadC* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)RBX_NewThread;//upd8

typedef int(RBX_NewUserDataC* rnewusernigger)(int a1, unsigned int a2, char a3);
rnewusernigger r_lua_newuserdata = (rnewusernigger)RBX_NewUserData;

typedef void(RBX_RawGetiC* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)RBX_RawGeti;//up8

typedef void* (RBX_GetMetaTableC* rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)RBX_GetMetaTable;//upd8

typedef int(RBX_ToUserDataC* rtonigger)(DWORD, int);
rtonigger r_lua_touserdata = (rtonigger)RBX_ToUserData;

typedef DWORD(RBX_TypeC* rtype)(DWORD, int);
rtype r_lua_type = (rtype)RBX_Type;//up8

typedef DWORD(RBX_LuaL_RefC* rref)(DWORD, DWORD);
rref r_luaL_ref = (rref)RBX_LuaL_Ref;//upd8

typedef void* (RBX_SettableC* rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)RBX_Settable;//upd8

typedef int(RBX_PushLightUDC* rpushlightnigger)(int a1, int a2);
rpushlightnigger r_lua_pushlightuserdata = (rpushlightnigger)RBX_PushLightUD;

typedef int(RBX_GettopC* rgetnigger)(int a1);
rgetnigger r_lua_gettop = (rgetnigger)RBX_Gettop;

void SetLevel(DWORD RobloxState, int Level) {
	int v3 = *(DWORD*)(RobloxState + identity_offset);
	*(DWORD*)(v3 + 24) = Level;
}