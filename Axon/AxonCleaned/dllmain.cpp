#include "Wrapper.hpp"

DWORD WINAPI input(PVOID lvpParameter)
{
	std::string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Axon"),//change ur pipe skid
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			luaL_dostring(vl_state, WholeScript.c_str()); //magic :TM:
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

namespace Scanner {
	//imagine not using datamodel, couldn't be me
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; *mask; ++aob, ++mask, ++location) {
			__try {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	}
	DWORD find_Pattern(BYTE* pattern, char* mask, BYTE protection = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
		SYSTEM_INFO SI = { 0 };
		GetSystemInfo(&SI);
		DWORD start = (DWORD)SI.lpMinimumApplicationAddress;
		DWORD end = (DWORD)SI.lpMaximumApplicationAddress;
		MEMORY_BASIC_INFORMATION mbi;
		while (start < end && VirtualQuery((void*)start, &mbi, sizeof(mbi))) {
			if ((mbi.State & MEM_COMMIT) && (mbi.Protect & protection) && !(mbi.Protect & PAGE_GUARD)) {
				for (DWORD i = (DWORD)mbi.BaseAddress; i < (DWORD)mbi.BaseAddress + mbi.RegionSize; ++i) {
					if (compare((BYTE*)i, pattern, mask)) {
						return i;
					}
				}
			}
			start += mbi.RegionSize;
		}
		return 0;
	}
	int Scan(DWORD mode, char* content, char* mask) {
		return find_Pattern((BYTE*)content, mask, mode);
	}
}

static int UserDataGC(lua_State* Thread) {
	void* UD = lua_touserdata(Thread, 1);
	if (rbx_state) {

		r_lua_rawgeti(rbx_state, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(rbx_state, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
		}
	}
	return 0;
}

void CreateConsole(const char* hihi) {
	DWORD consoleOldProtect = 0;
	VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &consoleOldProtect);
	*(UINT*)FreeConsole = 0xC2;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	DWORD cons = (WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU);
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, cons);
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	SetConsoleTitleA(hihi);
}

void EntryPoint() {
	CreateConsole("Axon Cleaned | ShowerHead");
	std::cout << "[Axon]: Scanning...";
	rbx_state = LuaState(Scanner::Scan(PAGE_READWRITE, (char*)&ScriptContext, (char*)"xxxx")); //credits to asdf for scanner
	vl_state = lua_open();
	luaL_openlibs(vl_state);
	VehHandlerpush();
	luaL_newmetatable(vl_state, "garbagecollector");
	lua_pushcfunction(vl_state, UserDataGC);
	lua_setfield(vl_state, -2, "__gc");
	lua_pushvalue(vl_state, -1);
	lua_setfield(vl_state, -2, "__index");
	std::cout << "Completed!\r\n";
	std::cout << "[Axon]: Wrapping...";
	WrapGlobals(rbx_state, vl_state);
	lua_newtable(vl_state);
	lua_setglobal(vl_state, "_G");
	std::cout << "Completed!\r\n";
	std::cout << "[Axon]: Setting Context...";
	SetLevel(rbx_state, 6);
	std::cout << "Completed!\r\n";
	std::cout << "[Axon]: Ready for script execution.";
	luaL_dostring(vl_state, "game.StarterGui:SetCore('SendNotification', {Duration = 4; Title='Axon'; Text='Axon Injected!'})");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EntryPoint, NULL, NULL, NULL);
	}
	return TRUE;
}