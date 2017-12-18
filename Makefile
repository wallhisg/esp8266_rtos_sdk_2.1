SRCS		:=
OBJS		:=
DEPS		:=
TMPS		:=
LIBS		:=
PROGRAMS	:=
#======================================================================
# MODULES
MODULES		:= src/driver
# LIBS
MODULES		+= lib/
# APP
MODULES 	+= user/tcp_api

INCLUDE_DIRS	:= include
vpath %.h $(INCLUDE_DIRS)
#======================================================================
# select which tools to use as compiler, 
# librarian and linker
#======================================================================
CFLAGS	+= -std=c++11 -Wall -O2
CFLAGS	+= $(addprefix -I , $(INCLUDE_DIRS))
#======================================================================
# select which tools to use as compiler, librarian and linker
#======================================================================
AR = ar
ARFLAGS = rv
#======================================================================
# Enviroment
#======================================================================
MKDIR		:= mkdir -p
RM		:= rm -f
MV		:= mf -f
#======================================================================
# Common modules
include $(patsubst %, ./%/xmodule.mk, $(MODULES))
# Current program

#======================================================================
.PHONY: default build run all clean

default: all

all: $(OBJS) $(PROGRAMS)

run: $(PROGRAMS)
	@$<
clean:
	$(RM) $(OBJS) $(DEPS) $(PROGRAMS) $(TMPS)
	

