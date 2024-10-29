# @file Makefile
#   Makefile both for mingw on windows
#
# @copyright Copyright(c) 2024, mapaware.top
# @since 2024-10-29 13:01:28
# @date 2024-10-30 00:16:13
########################################################################
# Linux, CYGWIN_NT, MSYS_NT, ...
shuname = "$(shell uname)"
OSARCH ?= $(shell echo $(shuname)|awk -F '-' '{ print $$1 }')

# Note: cygwin is not supported! use mingw instead
ifeq ($(OSARCH), CYGWIN_NT)
	OSARCH=CYGWIN64
	$(error cygwin is not supported! use mingw instead)
endif

# mingw
ifeq ($(OSARCH), MINGW64_NT)
	OSARCH=MINGW64
else ifeq ($(OSARCH), MSYS_NT)
	OSARCH=MINGW64
else ifeq ($(OSARCH), MINGW32_NT)
	OSARCH=MINGW32
	$(error 32-bit mingw is not supported)
else
	OSARCH=LINUX64
endif

# project
PROJECT := $(notdir $(CURDIR))

VERSION_FILE := $(CURDIR)/VERSION
VERSION := $(shell cat $(VERSION_FILE))

APPNAME := $(PROJECT)
APPNAME_VER := $(APPNAME)-$(VERSION)

SDL2_MINGW64_ROOT := ../../deps/SDL2-2.30.7-mingw/x86_64-w64-mingw32

#--------------------------------------------------------------
# compiler
CC := gcc

# default build: RELEASE
# make BUILD=DEBUG
BUILD ?= RELEASE

# compile directives
CFLAGS += -std=gnu11 -D_GNU_SOURCE -fPIC -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable

# load libs: -lpthread = libpthread.so
LDFLAGS += -lm -lpthread -lcairo -lshapefile -lgeodbapi -lproj-22

ifeq ($(BUILD), DEBUG)
    # make BUILD=DEBUG
	CFLAGS += -D_DEBUG -g
else
    # default is release
	CFLAGS += -DNDEBUG -O3
endif

#--------------------------------------------------------------
INCDIRS := -I. -I.. -I/usr/local/include

# Get pathfiles for C source files like: 1.c 2.c
CSRCS := $(foreach cdir, $(CURDIR), $(wildcard $(cdir)/*.c))

# Get names of object files: '1.o 2.o'
COBJS := $(patsubst %.c, %.o, $(notdir $(CSRCS)))

LIBDIRS := -L/usr/local/lib -L/usr/local/lib64 -L/usr/local/bin \
	-L../shapefile -L../geodbapi


ifeq ($(OSARCH), MINGW64)
	CFLAGS += -Dmain=SDL_main -D__MINGW64__ -m64
	INCDIRS += -I/mingw64/include -I$(SDL2_MINGW64_ROOT)/include
	LIBDIRS += -L/mingw64/lib -L$(SDL2_MINGW64_ROOT)/lib
	LDFLAGS += -lmingw32 -lSDL2main -lSDL2 -mwindows -lws2_32
else ifeq ($(OSARCH), LINUX64)
	CFLAGS += -D__LINUX__
    LDFLAGS += -lSDL2 -lrt
else
	$(error $(OSARCH) is not supported)
endif

#--------------------------------------------------------------
.PHONY: all clean $(APPNAME_VER)

all: $(APPNAME_VER)

clean:
	@/usr/bin/find $(CURDIR) -type f -name '*.o' | xargs -I {} sh -c "rm -f {}"
	@rm -f $(APPNAME_VER) $(APPNAME)
	@echo "[clean] done: $(CURDIR)"

#--------------------------------------------------------------
# http://www.gnu.org/software/make/manual/make.html#Eval-Function
define COBJS_template =
$(basename $(notdir $(1))).o: $(1)
	$(CC) $(CFLAGS) $(CFLAGSW) $(INCDIRS) -c $(1) -o $(basename $(notdir $(1))).o
endef

$(foreach src,$(CSRCS),$(eval $(call COBJS_template,$(src))))
#--------------------------------------------------------------

$(APPNAME_VER): $(COBJS)
	@echo ">>>> $(APPNAME_VER) >>>>"
	@rm -f $@
	@rm -f $(APPNAME)
	$(CC) $(CFLAGS) $(LIBDIRS) \
		-Wl,--rpath='.:./libs:/usr/local/lib' \
		$^ -o $@ \
		$(LDFLAGS)
	@ln -s $@ $(APPNAME)
