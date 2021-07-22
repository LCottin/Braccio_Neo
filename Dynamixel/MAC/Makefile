# *** ENTER THE TARGET NAME HERE ***
TARGET      = Braccio

# important directories used by assorted rules and other variables
DIR_DXL    = ..
DIR_OBJS   = ../objects

# compiler options
CX          = g++
CXFLAGS     = -O2 -Wall $(INCLUDES) -std=c++14
LNKCC       = $(CX)
LNKFLAGS    = $(CXFLAGS) #-Wl,-rpath,$(DIR_THOR)/lib

#---------------------------------------------------------------------
# Core components (all of these are likely going to be needed)
#---------------------------------------------------------------------
INCLUDES   += -I$(DIR_DXL)/include/dynamixel_sdk
LIBRARIES  += -ldxl_mac_cpp -lm 

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
		  src/MX106T.cpp \
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
