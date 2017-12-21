# Makefile for ESP8266 projects

# Output directors to store intermediate compiled files
# relative to the project directory
BUILD_BASE	= build
FW_BASE		= firmware

#======================================================================
# name for the target project
TARGET		= 
SRCS		:= 
OBJS		:= 
DEPS		:=
#======================================================================
# which modules (subdirectories) of the project to include in compiling
# Add new here
MODULES		= src/driver src/event src/wifi_tcp
EXTRA_INCDIR    = include

#======================================================================
#### no user configurable options below here
SRC_DIR		:= $(MODULES)
BUILD_DIR	:= $(BUILD_BASE)
INCLUDE_DIRS	:= include
EXTRA_INCDIR	:= $(addprefix -I,$(EXTRA_INCDIR))
vpath %.c $(SRC_DIR)
vpath %.h $(INCLUDE_DIRS)
vpath %.h $(EXTRA_INCDIR)
#======================================================================
# Common modules
include $(patsubst %, ./%/xmodule.mk, $(MODULES))
include ./Enviroment.mk

# *********************************************************************
# Current Project
#include ./user/tcp_api/xmodule.mk
#include ./user/smart_config/xmodule.mk
#include ./user/wifi_tcp/xmodule.mk
include ./user/blinky/xmodule.mk

#include ./user/GPIO/xmodule.mk
# *********************************************************************

#======================================================================
# Output files
APP_AR		:= $(addprefix $(BUILD_BASE)/,$(TARGET).a)
TARGET_OUT	:= $(addprefix $(BUILD_BASE)/,$(TARGET).out)


%.o: %.c %.d
	$(CC) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS) -c $< -o $@

#======================================================================
.PHONY: all checkdirs flash clean

all: checkdirs $(TARGET_OUT) $(FW_FILE_1) $(FW_FILE_2)

$(FW_BASE)/%.bin: $(TARGET_OUT) | $(FW_BASE)
	$(vecho) "FW $(FW_BASE)/"
	$(Q) $(ESPTOOL) elf2image -o $(FW_BASE)/ $(TARGET_OUT)

$(TARGET_OUT): $(APP_AR)
	$(vecho) "LD $@"
	$(LD) -L$(SDK_LIBDIR) $(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $(LIBS) $(APP_AR) -Wl,--end-group -o $@

$(APP_AR): $(OBJS)
	$(vecho) "AR $@"
	$(AR) cru $@ $^

flash: $(FW_FILE_1) $(FW_FILE_2)
	$(ESPTOOL) --port $(ESPPORT) write_flash $(FW_FILE_1_ADDR) $(FW_FILE_1) $(FW_FILE_2_ADDR) $(FW_FILE_2)

clean:
	$(Q) rm -rf $(FW_BASE) $(OBJS) $(DEPS) $(APP_AR) $(TARGET_OUT)
#======================================================================
checkdirs: $(BUILD_DIR) $(FW_BASE)

$(BUILD_DIR):
	$(Q) mkdir -p $@

$(FW_BASE):
	$(Q) mkdir -p $@
	
test:
	@echo	$(OBJS)

