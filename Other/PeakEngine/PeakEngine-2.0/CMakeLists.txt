
cmake_minimum_required(VERSION 2.6)
project(PeakEngine)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

set(ENGINE_SRC
src/core/Client.cpp
src/core/Game.cpp
src/core/Engine.cpp
src/core/Server.cpp
src/entity/ClientEntity.cpp
src/entity/Entity.cpp
src/entity/EntityManager.cpp
src/entity/EntityMap.cpp
src/entity/FloatProperty.cpp
src/entity/Property.cpp
src/entity/QuaternionProperty.cpp
src/entity/ServerEntity.cpp
src/entity/Vector2FProperty.cpp
src/entity/Vector3FProperty.cpp
src/entity/IntProperty.cpp
src/network/LocalConnection.cpp
src/network/NetworkConnection.cpp
src/network/NetworkClient.cpp
src/network/NetworkHost.cpp
src/support/Buffer.cpp
src/support/ConditionVariable.cpp
src/support/Mutex.cpp
src/support/OS.cpp
src/support/ScopedLock.cpp
src/support/Thread.cpp
)

option(PEAKENGINE_EMULATE_NETWORK "Emulate lag/packet loss in LocalConnection" OFF)
if(PEAKENGINE_EMULATE_NETWORK)
	add_definitions(-DEMULATE_NETWORK)
endif(PEAKENGINE_EMULATE_NETWORK)

include_directories(include)

add_library(PeakEngine SHARED ${ENGINE_SRC})
if(WIN32)
	target_link_libraries(PeakEngine enet ws2_32 winmm pthread)
else(WIN32)
	target_link_libraries(PeakEngine enet pthread)
endif(WIN32)

install(DIRECTORY include/peakengine/ DESTINATION include/peakengine FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakEngine.hpp DESTINATION include)
install(TARGETS PeakEngine LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)

# Plugins

add_subdirectory(plugins/graphics)
add_subdirectory(plugins/physics)
