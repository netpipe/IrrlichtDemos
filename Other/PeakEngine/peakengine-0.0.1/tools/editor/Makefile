

EDITOR_SRC = tools/editor/src/main.cpp \
tools/editor/src/MainWindow.cpp

EDITOR_CFLAGS = -I"." -I"include" -I"tools/editor/include" -Wall -I"/usr/local/include" -g `pkg-config gtkmm-2.4 --cflags` `pkg-config libglademm-2.4 --cflags` `pkg-config gdk-2.0 --cflags`
EDITOR_CXXFLAGS = $(EDITOR_CFLAGS)
EDITOR_LDFLAGS = -L"lib" -lpeakengine -lenet -ltolua++ -L"/usr/local/lib" -g `pkg-config gtkmm-2.4 --libs` `pkg-config libglademm-2.4 --libs` `pkg-config gthread-2.0 --libs` `pkg-config gdk-2.0 --libs`

EDITOR_OBJ = $(EDITOR_SRC:tools/editor/src/%cpp=tools/editor/obj/%o)
EDITOR_DEPS = $(EDITOR_SRC:tools/editor/src/%cpp=tools/editor/dep/%d)
EDITOR_LDFLAGS += -Wl,-rpath,"lib"

EDITOR_OUTPUT = editor

editor_all: $(EDITOR_DEPS) $(EDITOR_OUTPUT)

editor_clean:
	@echo -e "Editor: \033[31mCleaning up.\033[0m"
	-@rm -f $(EDITOR_DEPS) $(EDITOR_OBJ) $(EDITOR_OUTPUT)

editor_rebuild: editor_clean editor_all

$(EDITOR_OUTPUT): $(EDITOR_OBJ) $(ENGINE_OUTPUT)
	@echo "Linking $@."
	@$(CXX) $(EDITOR_OBJ) -o $@ $(EDITOR_LDFLAGS)

tools/editor/obj/%o: tools/editor/src/%cpp
	@echo "Compiling $<."
	@$(CXX) -c -o $@ $< $(EDITOR_CXXFLAGS)

tools/editor/obj/%o: tools/editor/src/%c
	@echo "Compiling $<."
	@$(CC) -c -o $@ $< $(EDITOR_CFLAGS)

tools/editor/dep/%d: tools/editor/src/%c
	@echo "Generating $@."
	@$(CC) -MM -MT "$(@:tools/editor/dep/%d=tools/editor/obj/%o) $@" $< $(EDITOR_CFLAGS) > $@

tools/editor/dep/%d: tools/editor/src/%cpp
	@echo "Generating $@."
	@$(CXX) -MM -MT "$(@:tools/editor/dep/%d=tools/editor/obj/%o) $@" $< $(EDITOR_CXXFLAGS) > $@

-include $(EDITOR_DEPS)

