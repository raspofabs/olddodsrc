#ifndef __OOP_LUA_H__
#define __OOP_LUA_H__

struct lua_State;

class LuaScript {
	public:
		LuaScript();
		~LuaScript();

		int LoadScript( const char *filename ); // return 0 on success
		float GetGlobalNumber( const char *identifier, float default_value );

	private:
		lua_State *lua;
};


#endif
