
ENGINE_LUASRC = engine/src/lua_peak.cpp

ENGINE_SRC = $(ENGINE_LUASRC) \
engine/src/core/Address.cpp \
engine/src/core/Util.cpp \
engine/src/core/GameEngine.cpp \
engine/src/core/Script.cpp \
engine/src/core/SettingsManager.cpp \
engine/src/core/Buffer.cpp \
engine/src/core/Vector3D.cpp \
engine/src/core/Vector2DI.cpp \
engine/src/core/Vector2D.cpp \
engine/src/core/File.cpp \
engine/src/core/FileSystem.cpp \
engine/src/core/IniFile.cpp \
engine/src/core/Quaternion.cpp \
engine/src/core/Game.cpp \
engine/src/core/Color.cpp \
engine/src/core/Logger.cpp \
engine/src/core/InputManager.cpp \
engine/src/core/Keycode.cpp \
engine/src/core/Timer.cpp \
engine/src/core/Level.cpp \
engine/src/network/NetworkHost.cpp \
engine/src/network/BroadcastClient.cpp \
engine/src/network/BroadcastHost.cpp \
engine/src/network/Connection.cpp \
engine/src/network/NetworkClient.cpp \
engine/src/physics/Hinge2.cpp \
engine/src/physics/Body.cpp \
engine/src/physics/Hinge.cpp \
engine/src/physics/Cube.cpp \
engine/src/physics/Sphere.cpp \
engine/src/physics/Joint.cpp \
engine/src/physics/Cylinder.cpp \
engine/src/physics/Plane.cpp \
engine/src/physics/Geometry.cpp \
engine/src/physics/Ray.cpp \
engine/src/physics/World.cpp \
engine/src/physics/Heightfield.cpp \
engine/src/graphic/irr/GraphicsEngine.cpp \
engine/src/graphic/irr/SceneNode.cpp \
engine/src/graphic/irr/CubeSceneNode.cpp \
engine/src/graphic/irr/SphereSceneNode.cpp \
engine/src/graphic/irr/CameraSceneNode.cpp \
engine/src/graphic/irr/MeshSceneNode.cpp \
engine/src/graphic/irr/Image.cpp \
engine/src/graphic/irr/InputReceiver.cpp \
engine/src/graphic/irr/Terrain.cpp \
engine/src/graphic/irr/SkyBox.cpp \
engine/src/graphic/irr/SkyDome.cpp \
engine/src/graphic/Material.cpp \
engine/src/graphic/OverlayManager.cpp \
engine/src/graphic/Overlay.cpp \
engine/src/graphic/irr/RectangleOverlay.cpp \
engine/src/graphic/irr/TextureOverlay.cpp \
engine/src/graphic/irr/LabelOverlay.cpp \
engine/src/graphic/SceneNodeController.cpp \
engine/src/graphic/FollowingController.cpp \
engine/src/graphic/RotationController.cpp \
engine/src/entity/Entity.cpp \
engine/src/entity/PhysicsComponent.cpp \
engine/src/entity/GraphicsComponent.cpp \
engine/src/entity/ServerScriptComponent.cpp \
engine/src/entity/ClientScriptComponent.cpp \
engine/src/entity/EntityVariables.cpp \
engine/src/sound/SoundEngine.cpp \
engine/src/sound/Sound.cpp \
engine/src/sound/Listener.cpp \
engine/src/sound/DataSoundSource.cpp \
engine/src/sound/OggVorbisSource.cpp \
engine/src/sound/SoundFactory.cpp \
engine/src/menu/MenuManager.cpp \
engine/src/menu/Menu.cpp \
engine/src/menu/MenuElement.cpp \
engine/src/menu/Label.cpp \
engine/src/menu/Button.cpp \
engine/src/menu/CheckBox.cpp \
engine/src/menu/ListBox.cpp \
engine/src/menu/ComboBox.cpp

ENGINE_OBJ = $(ENGINE_SRC:engine/src/%cpp=engine/obj/%o)
ENGINE_DEPS = $(ENGINE_SRC:engine/src/%cpp=engine/dep/%d)

ENGINE_CFLAGS = -I"/usr/local/include/lua51" -I"/usr/local/include" -I"engine/include" -I"." -I"/usr/include/lua5.1" -fPIC -Wall -g
ENGINE_CXXFLAGS = $(ENGINE_CFLAGS) -D_USE_IRRLICHT_

ENGINE_LDFLAGS = -shared -lode `pkg-config lua5.1 --libs` `pkg-config lua --libs` -lIrrlicht -lGL -lGLU -lXext -lXxf86vm -lX11 -L"/usr/local/lib" -L"/usr/local/lib/lua51" -Wl,-rpath,"/usr/local/lib" -lalut -L"/usr/lib" -lopenal -lvorbisfile -lvorbisenc -lvorbis -logg -g

ENGINE_OUTPUT = lib/libpeakengine.so
ENGINE_OUTPUT_STATIC = lib/libpeakengine.a

engine_all: $(ENGINE_DEPS) $(ENGINE_OUTPUT)

engine_clean:
	@echo -e "Engine: \033[31mCleaning up.\033[0m"
	-@rm -f $(ENGINE_DEPS) $(ENGINE_OBJ) $(ENGINE_OUTPUT)

engine_rebuild: engine_clean engine_all

$(ENGINE_OUTPUT): $(ENGINE_OBJ)
	@echo -e "\033[36mLinking \033[0m\033[1m$@.\033[0m"
	@$(CXX) $(ENGINE_OBJ) -o $@ $(ENGINE_LDFLAGS)
$(ENGINE_OUTPUT_STATIC): $(ENGINE_OBJ)
	@echo -e "\033[36mCreating \033[0m\033[1m$@.\033[0m"
	@$(AR) -rs $@ $(ENGINE_OBJ)

engine/obj/%o: engine/src/%cpp
	@echo -e "\033[32mCompiling \033[0m\033[1m$<.\033[0m"
	@$(CXX) -c -o $@ $< $(ENGINE_CXXFLAGS)

engine/obj/%o: engine/src/%c
	@echo -e "\033[32mCompiling \033[0m\033[1m$<.\033[0m"
	@$(CC) -c -o $@ $< $(ENGINE_CFLAGS)

engine/dep/%d: engine/src/%c
	@echo -e "\033[33mGenerating \033[0m\033[1m$@.\033[0m"
	@$(CC) -MM -MT "$(@:engine/dep/%d=engine/obj/%o) $@" $< $(ENGINE_CFLAGS) > $@

engine/dep/%d: engine/src/%cpp
	@echo -e "\033[33mGenerating \033[0m\033[1m$@.\033[0m"
	@$(CXX) -MM -MT "$(@:engine/dep/%d=engine/obj/%o) $@" $< $(ENGINE_CXXFLAGS) > $@

engine/src/lua_%cpp: engine/include/%pkg engine/include/*/*.h
	@echo -e "\033[33mCreating lua interface \033[0m\033[1m$<.\033[0m"
	@tolua++ -n peak -o $@ $<

engine/obj/lua_%o: engine/src/lua_%cpp
	@echo -e "\033[32mCompiling \033[0m\033[1m$<.\033[0m"
	@$(CXX) -c -o $@ $< $(ENGINE_CXXFLAGS)

-include $(ENGINE_DEPS)

