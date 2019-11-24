
set(SRC
src/Body.cpp
src/Box.cpp
src/Capsule.cpp
src/CharacterController.cpp
src/Compound.cpp
src/Heightfield.cpp
src/Physics.cpp
src/PhysicsProfiler.cpp
src/Plane.cpp
src/Sphere.cpp
src/Shape.cpp
src/Trimesh.cpp
)

include_directories(include ../../include)

add_library(PeakPhysics SHARED ${SRC})
target_link_libraries(PeakPhysics PeakEngine freeimageplus)

install(DIRECTORY include/peakphysics/ DESTINATION include/peakphysics FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakPhysics.hpp DESTINATION include)
install(TARGETS PeakPhysics LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)
