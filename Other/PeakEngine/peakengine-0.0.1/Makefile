
all: engine_all editor_all bin/enginetest

clean: engine_clean editor_clean
	@-rm bin/enginetest

rebuild: clean all

run: bin/enginetest
	@bin/enginetest tests/test1
rollout: bin/enginetest
	@bin/enginetest tests/rollout
fpstest: bin/enginetest
	@bin/enginetest tests/fpstest
gunships: bin/enginetest
	@bin/enginetest tests/gunships

include engine/Makefile
include tools/editor/Makefile

bin/enginetest: tests/main.cpp $(ENGINE_OUTPUT)
	@echo -e "\033[36mCompiling \033[0m\033[1m$@.\033[0m"
	@$(CXX) $< -o $@ -L"." -Wl,-rpath,"lib" -L"lib" -lpeakengine -I"engine/include" -lenet -ltolua++ -g
	
bin/engineteststatic: tests/main.cpp $(ENGINE_OUTPUT_STATIC)
	@echo -e "\033[36mCompiling \033[0m\033[1m$@.\033[0m"
	@$(CXX) $< -o $@ -L"." -Wl,-rpath,"lib" -L"lib" lib/libpeakengine.a -I"engine/include" -lenet -ltolua++ -lode -llua -lIrrlicht -lGL -lGLU -lXext -lXxf86vm -lX11 -L"/usr/local/lib" -L"/usr/local/lib/lua51" -Wl,-rpath,"/usr/local/lib" -lalut -L"/usr/lib" -lopenal -lvorbisfile -lvorbisenc -lvorbis -logg -pg

