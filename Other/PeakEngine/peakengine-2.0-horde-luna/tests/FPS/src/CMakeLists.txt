
set (ENGINE_LUA_PKG ${CMAKE_CURRENT_SOURCE_DIR}/arizona.pkg)
set (ENGINE_LUA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lua_plugin.cpp)

set (TOLUA "tolua++")
set (TOLUA_ARGS -n plugin -o ${CMAKE_CURRENT_SOURCE_DIR}/lua_plugin.cpp ${ENGINE_LUA_PKG})
add_custom_command (OUTPUT ${ENGINE_LUA_SRC}
	COMMAND ${TOLUA} ARGS ${TOLUA_ARGS})

set (LIBRARY_OUTPUT_PATH ../data/plugins)
include_directories(../../../engine/include ../../../engine/src /usr/local/include/)
add_library(Arizona SHARED Plugin.cpp lua_src.cpp Player.cpp ${ENGINE_LUA_SRC})

