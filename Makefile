NAME := homebrew_launcher

INCLUDES := -Isrc -Ilibs/libgui-sdl/include/gui-sdl
SOURCES :=  src libs/libgui-sdl/source
LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

CPPFILES := $(shell find $(SOURCES) -name '*.cpp')

make all:
	g++ $(INCLUDES) -o $(NAME).exe $(CPPFILES) $(LIBS) -DPC -g -std=c++17

.PHONY: clean
clean:
	rm -f $(NAME).exe