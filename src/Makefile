#
#  machina
#  Copyright (c) 2014, drmats
#  All rights reserved.
#  https://github.com/drmats/machina
#

PNAME               =  machina
PLIBS               =  m3d_message.o m3d.o primitives.o camera.o main_loop.o machina.o main.o
GNUCPP              =  g++
GNUCOMPILEFLAGS     =  -std=c++11 -Wall -Wpedantic -O2 -D_GNU_SOURCE=1
GNULINKLIBS         =  -lGL -lGLU -lm -lSDL2

.PHONY: default
linux:
	@$(MAKE) gnu_execute

.PHONY: gnu_execute
gnu_execute:  $(PLIBS)
	@echo Linking project...
	@$(GNUCPP) $(GNULINKLIBS) $(PLIBS) -o $(PNAME)
	@echo \"$(PNAME)\" produced succesfully!

.PHONY: clean
clean:
	@rm -v -f $(PNAME) *.o core

%.o: %.cpp %.h
	@echo Compiling: $<
	@$(GNUCPP) $(GNUCOMPILEFLAGS) -c $< -o $@
