# base directory for the compiler

XTENSA_TOOLS_ROOT ?= /opt/Espressif/ESP8266_RTOS_SDK_2_1/crosstool-NG/builds/xtensa-lx106-elf/bin

# base directory of the ESP8266 SDK package, absolute
SDK_BASE	?= /opt/Espressif/ESP8266_RTOS_SDK_2_1

# esptool.py path and port
ESPTOOL		?= esptool.py
ESPPORT		?= /dev/ttyUSB0

# libraries used in this project, mainly provided by the SDK
LIBS		= 

# compiler flags using during compilation of source files
CFLAGS	= -g -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls \
	-mtext-section-literals -ffunction-sections -fdata-sections -fno-builtin-printf -fno-jump-tables

# linker flags used to generate the main object file
LDFLAGS	= -L$(SDK_BASE)/lib        \
	-Wl,--gc-sections   \
	-nostdlib	\
	-Wl,--no-check-sections	\
    -u call_user_start	\
	-Wl,-static						\
	-Wl,--start-group					\
	-lcirom \
	-lcrypto	\
	-lespconn	\
	-lespnow	\
	-lfreertos	\
	-lgcc					\
	-lhal					\
	-ljson	\
	-llwip	\
	-lmain	\
	-lmesh	\
	-lmirom	\
	-lnet80211	\
	-lnopoll	\
	-lphy	\
	-lpp	\
	-lpwm	\
	-lmbedtls               \
    -lopenssl               \
	-lsmartconfig	\
	-lspiffs	\
	-lwpa	\
	-lwps		\

# linker script used for the above linkier step
LD_SCRIPT	= eagle.app.v6.ld

# various paths from the SDK used in this project
SDK_LIBDIR	= lib
SDK_LDDIR	= ld

SDK_INCDIR	+= include 
SDK_INCDIR	+= extra_include
SDK_INCDIR	+= driver_lib/include
SDK_INCDIR	+= include/espressif
SDK_INCDIR	+= include/lwip
SDK_INCDIR	+= include/lwip/ipv4
SDK_INCDIR	+= include/lwip/ipv6
SDK_INCDIR	+= include/nopoll
SDK_INCDIR	+= include/spiffs
SDK_INCDIR	+= include/ssl
SDK_INCDIR	+= include/json
SDK_INCDIR	+= include/openssl
# we create two different files for uploading into the flash
# these are the names and options to generate them
FW_FILE_1_ADDR	:= 0x00000
FW_FILE_2_ADDR	:= 0x20000
FW_FILE_1	:= $(addprefix $(FW_BASE)/,$(FW_FILE_1_ADDR).bin)
FW_FILE_2	:= $(addprefix $(FW_BASE)/,$(FW_FILE_2_ADDR).bin)
# select which tools to use as compiler, librarian and linker
CC		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
AR		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-ar
LD		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc

#### no user configurable options below here
SDK_LIBDIR	:= $(addprefix $(SDK_BASE)/,$(SDK_LIBDIR))
SDK_INCDIR	:= $(addprefix -I$(SDK_BASE)/,$(SDK_INCDIR))
LIBS		:= $(addprefix -l,$(LIBS))
LD_SCRIPT	:= $(addprefix -T$(SDK_BASE)/$(SDK_LDDIR)/,$(LD_SCRIPT))

V ?= $(VERBOSE)
ifeq ("$(V)","1")
Q :=
vecho := @true
else
Q := @
vecho := @echo
endif
