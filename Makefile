# @file Makefile
#   Makefile both for mingw on windows
#
# @copyright Copyright(c) 2024, mapaware.top
# @since 2024-10-29 12:26:10
# @date 2024-10-30 00:49:26
########################################################################
# Linux, CYGWIN_NT, MSYS_NT, ...
shuname="$(shell uname)"
OSARCH=$(shell echo $(shuname)|awk -F '-' '{ print $$1 }')

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

#--------------------------------------------------------------
# compiler
CC ?= gcc

# default build: RELEASE
# make BUILD=DEBUG
BUILD ?= RELEASE

#--------------------------------------------------------------
# source prefix dir
SRC_PREFIX := $(CURDIR)/source

# list all projects in order
PROJECTS := sqlite3 shapefile geodbapi shapetool mapaware

#--------------------------------------------------------------
.PHONY: all clean revise help

all:
	@for dir in $(PROJECTS); do \
		$(MAKE) BUILD="$(BUILD)" -C "$(SRC_PREFIX)/$$dir"; \
	done

clean:
	@for dir in $(PROJECTS); do \
		$(MAKE) -C "$(SRC_PREFIX)/$$dir" clean; \
	done
	@$(CURDIR)/clean_all.sh

revise:
	@/usr/bin/find $(SRC_PREFIX) -type f -mtime -30 \( -name '*.h' -o -name '*.c' \) | xargs -I {} sh -c "sh revise-source.sh {}"
	@/usr/bin/find $(SRC_PREFIX) -type f -mtime -30 \( -name '*.hxx' -o -name '*.cxx' \) | xargs -I {} sh -c "sh revise-source.sh {}"
	@/usr/bin/find $(SRC_PREFIX) -type f -mtime -30 \( -name '*.hpp' -o -name '*.cpp' \) | xargs -I {} sh -c "sh revise-source.sh {}"
	@/usr/bin/find $(CURDIR) \( -path ./deps -o -path ./.vscode \) -prune -o -type f -mtime -30 -name '*.sh' | xargs -I {} sh -c "sh revise-source.sh {}"
	@/usr/bin/find $(CURDIR) \( -path ./deps -o -path ./.vscode \) -prune -o -type f -mtime -30 -name 'Makefile' | xargs -I {} sh -c "sh revise-source.sh {}"
	@echo "(Ok) revise all sources done."

help:
	@echo "make projects = [$(PROJECTS)]"
	@echo "  $$ make BUILD=DEBUG  # build projects for debug"
	@echo "  $$ make clean        # clean all projects"
	@echo "  $$ make              # build projects release (default)"
	@echo "  $$ make revise       # revise source codes"
	@echo