


ifndef PLATFORM
PLATFORM = niboburger-m16-15
endif

LIBDIR = ../../lib/$(PLATFORM)

include ../../platforms/$(PLATFORM).mk
include ../version.mk

LIBNAME = niboburger

TARGETS = \
        $(LIBDIR)/lib$(LIBNAME)_class.a \
        $(LIBDIR)/lib$(LIBNAME)_usart.a \
        $(LIBDIR)/lib$(LIBNAME)_base.a \
        $(LIBDIR)/lib$(LIBNAME)_pid.a \
        $(LIBDIR)/lib$(LIBNAME)_pid2.a \
        $(LIBDIR)/lib$(LIBNAME)_surface.a \
        $(LIBDIR)/lib$(LIBNAME)_utils.a \
        $(LIBDIR)/lib$(LIBNAME)_rc5.a \
        $(LIBDIR)/lib$(LIBNAME)_bgx1.a

        
#        $(LIBDIR)/lib$(LIBNAME)_pid2.a \
        

OBJ_DIR=obj-$(PLATFORM)

SRC = \
  base.c \
  delay.c \
  usart.c \
  motpwm.c \
  motpid.c \
  odometry.c \
  key.c \
  led.c \
  analog.c \
  surface.c \
  storage.c \
  clock.c \
  i2cmaster.c \
  bgx1.c \
  rc5.c \
  color.c \
  motpid2.c

CPP_SRC = \
  NIBOburger.cpp
  
NO_SRC = 
  


NO_OBJS =

ARDUINO_UTILITY_C = \
  base.c \
  delay.c \
  motpwm.c \
  motpid.c \
  odometry.c \
  key.c \
  led.c \
  analog.c \
  surface.c \
  storage.c \
  clock.c \
  rc5.c \
  color.c

ARDUINO_UTILITY_H = $(ARDUINO_UTILITY_C:%.c=%.h) iodefs.h iodefs_niboburger.h storage_nrl.h utils.h

ARDUINO_SRC = NIBOburger.h NIBOburger.cpp

ARDUINO_LIB_DIR = ../../arduino16/libraries

SVN_REVISION := $(shell svnversion -n)

C_OBJS = $(SRC:%.c=$(OBJ_DIR)/%.o)
C_DEPS = $(SRC:%.c=$(OBJ_DIR)/%.d)

CPP_OBJS = $(CPP_SRC:%.cpp=$(OBJ_DIR)/%.o)
CPP_DEPS = $(CPP_SRC:%.cpp=$(OBJ_DIR)/%.d)

OBJS = $(C_OBJS) $(CPP_OBJS)
DEPS = $(C_DEPS) $(CPP_DEPS)

# AVR compiler setup
PREFIX =
CC = $(PREFIX)avr-gcc
AR = $(PREFIX)avr-ar

# AVR compiler flags
CFLAGS = -Os -ffunction-sections -I..  -Wall
CFLAGS += -std=c99
CFLAGS += -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -D_NIBOBURGER_ -DVERSION="\"$(VERSION)\"" -DSVN_REVISION="\"$(SVN_REVISION)\""

CPPFLAGS = -Os -ffunction-sections -I..  -Wall
CPPFLAGS += -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -D_NIBOBURGER_ -DVERSION="\"$(VERSION)\"" -DSVN_REVISION="\"$(SVN_REVISION)\""

# build lib
all: $(TARGETS)

$(LIBDIR)/lib$(LIBNAME)_class.a: $(OBJ_DIR)/NIBOburger.o

$(LIBDIR)/lib$(LIBNAME)_usart.a: $(OBJ_DIR)/usart.o

$(LIBDIR)/lib$(LIBNAME)_base.a: $(OBJ_DIR)/key.o $(OBJ_DIR)/motpwm.o $(OBJ_DIR)/odometry.o $(OBJ_DIR)/led.o $(OBJ_DIR)/analog.o $(OBJ_DIR)/base.o $(OBJ_DIR)/clock.o $(OBJ_DIR)/storage.o $(OBJ_DIR)/color.o $(OBJ_DIR)/delay.o

$(LIBDIR)/lib$(LIBNAME)_pid.a: $(OBJ_DIR)/motpid.o

$(LIBDIR)/lib$(LIBNAME)_pid2.a: $(OBJ_DIR)/motpid2.o

$(LIBDIR)/lib$(LIBNAME)_surface.a: $(OBJ_DIR)/surface.o

$(LIBDIR)/lib$(LIBNAME)_utils.a: $(OBJ_DIR)/delay.o

$(LIBDIR)/lib$(LIBNAME)_rc5.a: $(OBJ_DIR)/rc5.o

$(LIBDIR)/lib$(LIBNAME)_bgx1.a: $(OBJ_DIR)/bgx1.o $(OBJ_DIR)/i2cmaster.o


$(LIBDIR)/%.a:
	@mkdir -p $(LIBDIR)
	rm -f $@
	$(AR) -q $@ $^


$(OBJ_DIR)/%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.d: %.c
	@mkdir -p $(OBJ_DIR)
	@echo "calculating dependencies for $<"
	@set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's#\($*\)\.o[ :]*#$(OBJ_DIR)/\1.o $@ : #g' > $@ ; \
	[ -s $@ ] || rm -f $@

$(OBJ_DIR)/%.o: %.cpp Makefile
	$(CC) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.d: %.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "calculating dependencies for $<"
	@set -e; $(CC) -MM $(CPPFLAGS) $< \
	| sed 's#\($*\)\.o[ :]*#$(OBJ_DIR)/\1.o $@ : #g' > $@ ; \
	[ -s $@ ] || rm -f $@


clean:
	@rm -rf obj obj-* *~ $(TARGETS)

tarball: all

copy_arduino:
	mkdir -p $(ARDUINO_LIB_DIR)/NIBOburger
	mkdir -p $(ARDUINO_LIB_DIR)/NIBOburger/src
	mkdir -p $(ARDUINO_LIB_DIR)/NIBOburger/src/niboburger
	cp $(ARDUINO_UTILITY_C) $(ARDUINO_LIB_DIR)/NIBOburger/src/niboburger/
	cp $(ARDUINO_UTILITY_H) $(ARDUINO_LIB_DIR)/NIBOburger/src/niboburger/
	cp $(ARDUINO_SRC) $(ARDUINO_LIB_DIR)/NIBOburger/src/

doc:
	rm -rf ../../doc/niboburger
	doxygen Doxyfile
	
ifneq ($(MAKECMDGOALS),clean) 
-include $(DEPS)
endif


.PHONY: clean all
.SECONDARY: $(OBJS) $(DEPS)

