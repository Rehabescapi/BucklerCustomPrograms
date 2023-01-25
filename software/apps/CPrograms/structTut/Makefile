

# Testing multi-tasking in C
PROJECT_NAME = $(shell basename "$(realpath ./)")

# name:= main
# $(info $(name))
# Configurations
NRF_IC = nrf52832
SDK_VERSION = 15
SOFTDEVICE_MODEL = blank

# Source and header files
APP_HEADER_PATHS += .
APP_SOURCE_PATHS += .
APP_SOURCES = $(notdir $(wildcard ./*.c))

# Path to base of nRF52-base repo
NRF_BASE_DIR = ../../../nrf52x-base/

# Include board Makefile (if any)
include ../../../boards/buckler_revC/Board.mk

# Include main Makefile
include $(NRF_BASE_DIR)make/AppMakefile.mk


# CC=gcc


# execute:=$(name)
# CFLAGS=-Iincludes
# INCLUDE_DIR=include/
# INC_DIR =  -I ~/buckler_prv/software/nrf52x-base/sdk/nrf5_sdk_15.3.0/components/libraries/util
# SOURCE:=$(addsuffix .c, $(name))


# # run the following command whenever new.c changes

# $(execute): $(SOURCE)
# 	$(CC) $(SOURCE) -g -Wall $(INC_DIR) -o $(execute)	
# 	# @echo '$(PWD)'
# # code for cleaning the pre-built codes
# clean:
# 	echo "Removing the program executables."
# 	rm $(execute)
	
	
	


# INC_DIR =  -I ~/buckler_prv/software/nrf52x-base/sdk/nrf5_sdk_15.3.0/components/libraries/util
# # INC_DIR =  -I ~/buckler_prv/software/nrf52x-base/
# # INC_DIR += -I ~/buckler_prv/software/nrf52x-base/sdk/nrf5_sdk_15.3.0/components/libraries/util
# # INC_DIR = .
# # INC_DIR = $(wildcard $(HEAD_PATH)/*.h)

# $(PROJECT_NAME): main.c
# 	gcc main.c -g -Wall $(INC_DIR) -o main
