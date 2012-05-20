PREFIX ?= /usr/local
MEDIA_PREFIX ?= /share/pickle

SOURCES   := $(wildcard *.cpp)
OBJECTS   := $(subst .cpp,.o,$(SOURCES))
DEPS      := $(subst .o,.d,$(OBJECTS))
BINARY    := pickle
UNAME     := $(shell uname)
CXXFLAGS  += -DPREFIX=\"$(PREFIX)\" -DMEDIA_PREFIX=\"$(MEDIA_PREFIX)\" -ggdb3 -Wall `sdl-config --cflags`
LDLIBS    := -lstdc++ -lSDL_image -lSDL_ttf -lGL `sdl-config --libs`

ifeq ($(UNAME),Darwin)
	LDLIBS += -L/System/Library/Frameworks/OpenGL.framework/Libraries/
endif

.PHONY: all clean

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) $(LDLIBS) -o $(BINARY)

clean:
	rm -f $(BINARY) $(OBJECTS) $(DEPS)
