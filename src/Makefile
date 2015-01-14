#
#  machina
#  Copyright (c) 2014, drmats
#  All rights reserved.
#  https://github.com/drmats/machina
#

PNAME            =  machina
PLIBS            =  batch.o shader.o gframe.o camera.o primitives.o main_loop.o machina.o main.o
GNUCPP           =  g++
CROSSCPP32       =  i686-w64-mingw32-g++
CROSSCPP64       =  x86_64-w64-mingw32-g++
GNUCOMPILEFLAGS  =  -std=c++11 -mtune=generic -O2 -Wall -Wpedantic
GNULINKLIBS      =  -lGLEW -lGL -lGLU -lSDL2 -lm
CROSSLINKLIBS    =  -lmingw32 -lstdc++ -lwinpthread -lglew32 -lopengl32 -lglu32 -lSDL2main -lSDL2 -lm
CROSSLINKFLAGS   =  -mwindows
ENVIRONMENT      =


.PHONY: linux
linux:
ifeq ($(ENVIRONMENT),)
	@$(MAKE) gnu_execute ENVIRONMENT=gnu
endif


.PHONY: win32
win32:
ifeq ($(ENVIRONMENT),)
	@$(MAKE) cross32_execute ENVIRONMENT=cross32
endif


.PHONY: win64
win64:
ifeq ($(ENVIRONMENT),)
	@$(MAKE) cross64_execute ENVIRONMENT=cross64
endif


.PHONY: help
help:
	@echo "Available targets:"
	@echo "    linux  -  build using \"gcc/g++\" (GNU C/C++ Compiler)" [default]
	@echo "    win32  -  build using \"i686-w64-mingw32-g++\" (32bit Windows target)"
	@echo "    win64  -  build using \"x86_64-w64-mingw32-g++\" (64bit Windows target)"
	@echo "    clean  -  remove compiled objects and main program"


.PHONY: gnu_execute
gnu_execute:  $(PLIBS)
	@echo Linking project...
	@$(GNUCPP) $(PLIBS) $(GNULINKLIBS) -o $(PNAME)
	@echo \"$(PNAME)\" produced succesfully!


.PHONY: cross32_execute
cross32_execute:  $(PLIBS)
	@echo Linking project...
	@$(CROSSCPP32) $(PLIBS) $(CROSSLINKLIBS) $(CROSSLINKFLAGS) -o $(PNAME).exe
	@echo \"$(PNAME).exe\" [win32] produced succesfully!


.PHONY: cross64_execute
cross64_execute:  $(PLIBS)
	@echo Linking project...
	@$(CROSSCPP64) $(PLIBS) $(CROSSLINKLIBS) $(CROSSLINKFLAGS) -o $(PNAME).exe
	@echo \"$(PNAME).exe\" [win64] produced succesfully!


.PHONY: clean
clean:
	@rm -v -f $(PNAME) $(PNAME).exe *.o core


%.o: %.cpp %.hpp
ifeq ($(ENVIRONMENT),gnu)
	@echo Compiling [g++/linux]: $<
	@$(GNUCPP) $(GNUCOMPILEFLAGS) -c $< -o $@
endif
ifeq ($(ENVIRONMENT),cross32)
	@echo Compiling [mingw32-g++/win32]: $<
	@$(CROSSCPP32) $(GNUCOMPILEFLAGS) -c $< -o $@
endif
ifeq ($(ENVIRONMENT),cross64)
	@echo Compiling [mingw32-g++/win64]: $<
	@$(CROSSCPP64) $(GNUCOMPILEFLAGS) -c $< -o $@
endif
