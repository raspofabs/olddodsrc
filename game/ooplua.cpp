#include "ooplua.h"
#include "core/util.h"
#include "lua/lua.hpp"

LuaScript::LuaScript() {
	lua = luaL_newstate();
}
LuaScript::~LuaScript() {
	lua_close( lua );
	lua = 0;
}

int LuaScript::LoadScript( const char *filename ) {
	if( FILE *fp = fopen(filename, "r" ) ) {
		fseek( fp, 0, SEEK_END );
		size_t length = ftell( fp );
		rewind( fp );
		char* buffer = new char[ length + 1 ];
		size_t charcount = fread( buffer, 1, length, fp );
		fclose( fp );
		if( length != charcount ) {
			Log(1, "Error loading string. Only %i bytes read.\n", charcount );
			delete [] buffer;
			return 1;
		}

		buffer[length] = 0;
		Log(4, "Script:\n<<START>>\n%s\n<<END>>\n", buffer );

		Log(3,"Lua initialised with script %i bytes long\n", length );
		luaL_loadstring( lua, buffer );
		int r = lua_pcall(lua,0, LUA_MULTRET, 0);
		delete [] buffer;
		if( r ) {
			Log(3,"Lua error [%s]\n", lua_tostring(lua,-1) );
			return 1;
		}
		return 0;
	} else {
		Log(1,"No config file.\n" );
		return 1;
	}
}
float LuaScript::GetGlobalNumber( const char *identifier, float default_value ) {
	double val1 = default_value;
	lua_getglobal(lua,identifier);
	if( !lua_isnil(lua,1) ) {
		val1 = lua_tonumber(lua, 1);
	} else {
		Log(3,"Can't find [%s]\n", identifier );
	}
	lua_pop(lua,1);
	return val1;
}

