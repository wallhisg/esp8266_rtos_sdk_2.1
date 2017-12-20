
LOCAL_PATH	:= src/wifi_tcp
LOCAL_MDLS	:= wifi_tcp

LOCAL_SRCS	:= $(wildcard $(LOCAL_PATH)/*.c)
LOCAL_INCS	:= $(wildcard ./include/$(LOCAL_MDLS)/*.h)
LOCAL_OBJS	:= $(patsubst %.c, %.o, $(LOCAL_SRCS))
LOCAL_DEPS	:= $(patsubst %.c, %.d, $(LOCAL_SRCS))

DEPS		+= $(sort $(LOCAL_DEPS))
OBJS		+= $(sort $(LOCAL_OBJS))

$(LOCAL_DEPS): $(LOCAL_SRCS) $(LOCAL_INCS)
	$(Q) $(CC) -MM $< > $@

