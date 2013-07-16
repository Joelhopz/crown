/*
Copyright (c) 2013 Daniele Bartolini, Michele Rossi
Copyright (c) 2012 Daniele Bartolini, Simone Boscaratto

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "lua.hpp"
#include "Types.h"
#include "Config.h"
#include "LuaStack.h"
#include "LinearAllocator.h"
#include "Thread.h"

namespace crown
{

/// LuaEnvironment is a wrapper of a subset of Lua functions and 
/// provides utilities for extending Lua
class LuaEnvironment
{

public:
	/// Constructor
							LuaEnvironment();
	/// Init Lua state and open libraries. Must be called first
	void					init();
	/// Close Lua state and shutdown LuaEnvironment
	void					shutdown();
	/// Return the __lua_State__ pointer required by each Lua function
	lua_State*				state();

	const char*				error();
	/// Load Lua chunk as buffer. @a buffer contains lua chunk, @len is length of buffer
	void					load_buffer(const char* buffer, size_t len);
	/// Load Lua chunk as file. @a file is path to lua file
	void					load_file(const char* file);
	/// Load Lua chunk as string
	void 					load_string(const char* str);
	/// Get global symbol from Lua stack. @symbol is the name of required symbol
	void					get_global_symbol(const char* symbol);
	/// Execute Lua chunk. @a args is the number of arguments required
	/// @a results is the number of results returned
	void					execute(int32_t args, int32_t results);
	/// Collect garbage generated by Lua
	void					collect_garbage();
	/// Call init function in the Lua game file
	void					game_init();
	/// Call shutdown function in the Lua game file
	void					game_shutdown();
	/// Call frame function in the Lua game file
	void					game_frame(float dt);

	/// Load a function which will be used in Lua. @a module is the name of table contenitor,
	/// @a name is the name of function in module and @func is the pointer to the function.
	/// _func_ must be a C/lua function (__int32_t function_name(lua_State* L)__)
	void					load_module_function(const char* module, const char* name, const lua_CFunction func);
	/// Load a enum's value which will be used in Lua. 
	/// @a module is the name of table contenitor, generally take  enum's name
	/// @a name is module's name that refears _value_ and @value is an unsigned integer
	void					load_module_enum(const char* module, const char* name, uint32_t value);

private:
	/// Thread used for garbage collection. It calls __collect_garbage()__
	static void*			background_thread(void* thiz);

	void					lua_error();
	// Disable copying
							LuaEnvironment(const LuaEnvironment&);
	LuaEnvironment& 		operator=(const LuaEnvironment&);

private:
	/// Required by each Lua function
	lua_State*				m_state;
	/// LuaEnvironment is used right now?
	bool					m_is_used;
	/// Thread used for garbage collection
	os::Thread 				m_thread;

	char					m_error_buffer[1024];

	char					m_tmp_buffer[1024];

	static const char* 		class_system;

	static const char*		commands_list;

	static const char*		get_cmd_by_name;

	static const char*		tmp_print_table;

	static const char*		count_all;

	// static const char*		type_name;

	// static const char*		type_count;
};


void load_int_setting(LuaEnvironment& env);
void load_float_setting(LuaEnvironment& env);
void load_string_setting(LuaEnvironment& env);

void load_vec2(LuaEnvironment& env);
void load_vec3(LuaEnvironment& env);
void load_mat4(LuaEnvironment& env);
void load_quat(LuaEnvironment& env);
void load_math(LuaEnvironment& env);
void load_mouse(LuaEnvironment& env);
void load_keyboard(LuaEnvironment& env);
void load_touch(LuaEnvironment& env);
void load_accelerometer(LuaEnvironment& env);
void load_camera(LuaEnvironment& env);
void load_device(LuaEnvironment& env);
void load_window(LuaEnvironment& env);

CE_EXPORT int32_t luaopen_libcrown(lua_State* L);

} // namespace crown