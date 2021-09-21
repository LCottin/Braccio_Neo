# *** ENTER THE TARGET NAME HERE ***
TARGET      = Braccio

# important directories used by assorted rules and other variables
DIR_OBJS   = ../objects

# compiler options
CX          = g++
CXFLAGS     = -O2 -Wall $(INCLUDES) -std=c++14
LNKCC       = $(CX)
LNKFLAGS    = $(CXFLAGS) #-Wl,-rpath,$(DIR_THOR)/lib

ARCH=armv6zk
ifeq "$(shell uname -m)" "armv7l"
ARCH=armv7-a
endif

#---------------------------------------------------------------------
# Core components (all of these are likely going to be needed)
#---------------------------------------------------------------------
INCLUDES   += -I../src/lib/SDK
LIBRARIES  += -ldxl_sbc_cpp -lrt -lm -lrf24-bcm -lrf24network -lraspicam -mfpu=vfp -mfloat-abi=hard -march=$(ARCH) -mtune=arm1176jzf-s

#---------------------------------------------------------------------
# Files
#---------------------------------------------------------------------
SOURCES = src/functions.cpp \
		  src/Motor.cpp \
		  src/XL320.cpp \
		  src/MX28AT.cpp \
		  src/MX64AT.cpp \
		  src/AX12A.cpp \
		  src/AX18A.cpp \
		  src/MX106AT.cpp \
		  src/MX12W.cpp \
		  src/BraccioNeo.cpp \
		  src/main.cpp \
    # *** OTHER SOURCES GO HERE ***

OBJECTS  = $(addsuffix .o,$(addprefix $(DIR_OBJS)/,$(basename $(notdir $(SOURCES)))))
#OBJETCS += *** ADDITIONAL STATIC LIBRARIES GO HERE ***


#---------------------------------------------------------------------
# Compiling Rules
#---------------------------------------------------------------------
$(TARGET): make_directory $(OBJECTS)
	$(LNKCC) $(LNKFLAGS) $(OBJECTS) -o $(TARGET) $(LIBRARIES)

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(DIR_OBJS) core *~ *.a *.so *.lo

make_directory:
	mkdir -p $(DIR_OBJS)/

$(DIR_OBJS)/%.o: ../src/%.cpp
	$(CX) $(CXFLAGS) -c $? -o $@

#---------------------------------------------------------------------
# End of Makefile
#---------------------------------------------------------------------
