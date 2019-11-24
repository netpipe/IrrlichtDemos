
set(SRC
src/Graphics.cpp
src/Loadable.cpp
src/menu/Menu.cpp
src/menu/MenuButton.cpp
src/menu/MenuElement.cpp
src/menu/MenuImage.cpp
src/menu/Sprite.cpp
src/menu/SpriteLayer.cpp
src/scene/CameraSceneNode.cpp
src/scene/GroupSceneNode.cpp
src/scene/ModelSceneNode.cpp
src/scene/RootSceneNode.cpp
src/scene/SceneNode.cpp
src/scene/TerrainSceneNode.cpp
)

include_directories(include ../../include)

add_library(PeakGraphics SHARED ${SRC})
target_link_libraries(PeakGraphics PeakEngine Lightfeather)

install(DIRECTORY include/peakgraphics/ DESTINATION include/peakgraphics FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakGraphics.hpp DESTINATION include)
install(TARGETS PeakGraphics LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)
