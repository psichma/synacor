# Makefile for SFML_Project


###############################################################################################
###############################################################################################
###############################################################################################

##########################################
# USED LIBS
##########################################
# SFML      --> https://www.sfml-dev.org/
#   -graphic library for drawing 2D stuff
#
# rapidxml  --> http://rapidxml.sourceforge.net/
#   - for reading xml files
#
# spdlog    --> https://github.com/gabime/spdlog
#   - for logging





###############################################################################################
###############################################################################################
###############################################################################################

##########################################
# CHANGE THESE ACCORDING TO YOUR SETUP   #
##########################################

# this project is setup in a way that all inludes are in one directory (softlinks)
#INC=/home/patrick/libs/all_includes
INC=/home/patrick/libs/all_includes

# the SFML library, best case scenario it is built by yourself
SFML_LIB=/home/patrick/libs/SFML-2.5.1/lib
SPDLOG_LIB=/home/patrick/libs/spdlog/build


###############################################################################################
###############################################################################################
###############################################################################################

##########################################
# !!! DO NOT CHANGE THESE !!!            #
##########################################

# final program name
PROGNAME=pseng_demo

# all neccesary paths
PROJECT_PATH=$(shell pwd)
TESTDATAPATH=$(PROJECT_PATH)/testData
BUILDPATH=$(PROJECT_PATH)/build
OBJECTPATH=$(PROJECT_PATH)/obj
STARTFILE=$(BUILDPATH)/run
PROGFILE=$(BUILDPATH)/$(PROGNAME)
GCC=g++-8

# all neccessary links
SFML_LINKS= -lsfml-graphics -lsfml-window -lsfml-system -lstdc++fs -lspdlog
PS2D_LIB_LINKS=-lmapsystem -lservicesystem
MISC_LINKS=-lpthread

# the PS2D libraries will be created in the build directory
PS2D_LIB=build

# general flags for every compile unit
FLAGS=-std=c++2a -Wall -Wextra

# add the OBJECTPATH prefix to all objects
OBJECTS=$(addprefix $(OBJECTPATH)/, GuiButton.o GuiConstants.o GuiElement.o GuiTextBox.o GuiWindow.o ResourceService.o ServiceManager.o main.o)

# make the variables available for child make units
export

comp: main.cpp utils.o
	$(GCC) $(FLAGS) main.cpp utils.o -o comp



utils.o: utils.cpp
	$(GCC) $(FLAGS) -c utils.cpp -o utils.o


###############################################################################################
###############################################################################################
###############################################################################################


##########################################
#  TARGETS                               #
##########################################

#all: CHECKDIRS $(BUILDPATH)/$(PROGFILE)

#$(BUILDPATH)/$(PROGFILE): src_dir
#	$(GCC) $(FLAGS) -I$(INC) -L$(SFML_LIB) -L$(SPDLOG_LIB) $(OBJECTS) -o $(PROGFILE) $(MISC_LINKS) $(SFML_LINKS)
#	$(shell echo '#/bin/bash\nEXEC_DIR=$$(command dirname -- "$${0}")\n' \
#	             'export LD_LIBRARY_PATH=$(SFML_LIB):.\n' \
#				 'cd $$EXEC_DIR\n' \
#				 './$(PROGNAME)' > $(STARTFILE))
#	$(shell chmod 777 $(STARTFILE))
#	cp ./pseng/resources/* $(BUILDPATH)/resources

#src_dir:
#	make -C pseng all

#CHECKDIRS:
#	if ! [ -d "build" ]; then mkdir build; fi
#	if ! [ -d "obj" ]; then mkdir obj; fi
#	if ! [ -d "build/resources" ]; then mkdir build/resources; fi

clean:
	rm -f comp
	rm -f *.o
