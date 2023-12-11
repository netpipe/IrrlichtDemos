############################################
#
# PhoenixCore Examples CMake
#
############################################
cmake_minimum_required( VERSION 2.6 )

project( PhoenixCore )

############################################
# Examples
############################################

# Hello, World!
add_executable( 01HelloWorld 01\ helloworld.cpp )
target_link_libraries( 01HelloWorld PhoenixCore ${LIBRARIES} )

# Shapes
add_executable( 02Shapes 02\ shapes.cpp )
target_link_libraries(  02Shapes PhoenixCore ${LIBRARIES} )

# Textures
add_executable( 03Textures 03\ textures.cpp )
target_link_libraries( 03Textures PhoenixCore ${LIBRARIES} )

# Persistent geometry
add_executable( 04Persistence 04\ persistence.cpp )
target_link_libraries( 04Persistence PhoenixCore ${LIBRARIES} )

# Resources
add_executable( 05Resources 05\ resources.cpp )
target_link_libraries( 05Resources PhoenixCore ${LIBRARIES} )

# Geometry
add_executable( 06Geometry 06\ geometry.cpp )
target_link_libraries( 06Geometry PhoenixCore ${LIBRARIES} )

# Views
add_executable( 07Views 07\ views.cpp )
target_link_libraries( 07Views PhoenixCore ${LIBRARIES} )

# Box2d
if( BOX2D )
    add_executable( Box2d box2d.cpp )
    target_link_libraries( Box2d PhoenixCore ${LIBRARIES} ${BOX2D_LIBRARY} )
endif()