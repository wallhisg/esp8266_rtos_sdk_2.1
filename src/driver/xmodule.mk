LOCAL_BASE	:= src
LOCAL_MDLS	:= driver/uart driver/spi driver/gpio
LOCAL_BLDS	:= $(addprefix $(LOCAL_BASE)/, $(LOCAL_MDLS))
LOCAL_INCS	:= $(wildcard $(INCLUDE_DIRS)/$(LOCAL_MDLS)/*.h)

LOCAL_SRCS	:= $(wildcard $(LOCAL_MDLS)/*.c)
LOCAL_SRCS	+= $(foreach sdir, $(LOCAL_BLDS), $(wildcard $(sdir)/*.c))

LOCAL_OBJS	:= $(patsubst %.c, %.o, $(LOCAL_SRCS))
LOCAL_DEPS	:= $(patsubst %.c, %.d, $(LOCAL_SRCS))

SRCS		+= $(LOCAL_SRCS)
OBJS		+= $(LOCAL_OBJS)
DEPS		+= $(LOCAL_DEPS)

$(LOCAL_DEPS): $(LOCAL_SRCS) $(LOCAL_INCS)
	@$(CC) -MM $< > $@
	@$(CC) $< -MM -MT $@ >> $@

$(LOCAL_OBJS): $(LOCAL_SRCS) $(LOCAL_DEPS)

