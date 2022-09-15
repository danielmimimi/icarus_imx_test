# ****************************************************************************
#  
#                      MAKEFILE
#
# ----------------------------------------------------------------------------
# 
#  @author            Daniel Klauser
#           
#  @copyright         CC Intelligent Sensors and Networks
#                     at Lucerne University of Applied Sciences 
#                     and Arts T&A, Switzerland.
#
# ****************************************************************************

#name of lib
LIBVCA=libposenetimx8.so.0.0.0

OBJDIR=$(TARGET_TYPE)-obj_rel

# includes
# ----------------------------------------------------------------------------
INCS +=-I .
INCS +=-I ./include
INCS +=-I ./src


# Source files
# ------------------------------------------------------------------------
SOURCES += src/coralIcarusInference.cpp
SOURCES += src/general_inferencer.cpp
SOURCES += src/tensorResultToPassOn.cpp
SOURCES += src/tensorResultToPassOnFloat.cpp
SOURCES += src/tensorResultToPassOnUint8.cpp
SOURCES += src/tflite_inference.cpp

MY_CFLAGS += -fPIC
MY_CFLAGS += -DWITHOUT_TENSORFLOW

# Command to make an object file:
COMPILE = $(CXX) $(PROFFLAG) $(INCS) $(CFLAGS) $(CFLAGS_GENERIC) $(MY_CFLAGS) -c

# ------------------------------------------------------------------------
# Object files
OBJS=$(foreach SRCFILE,$(SOURCES),$(OBJDIR)/$(SRCFILE:.cpp=.o))
# build rules
# ----------------------------------------------------------------------------

all: $(OBJDIR)/$(LIBVCA)

clean:
	rm -f $(OBJDIR)/*.o 
	rm -rf $(OBJDIR)/src
	rm -f $(OBJDIR)/$(LIBVCA)


$(OBJDIR)/$(LIBVCA): $(OBJS)
	echo target: $@
	$(CXX) -shared $^ -o $@

$(OBJDIR)/src/%.o: ./src/%.cpp
	mkdir -p $(OBJDIR)/src
	$(COMPILE) $(CFLAGS) $< -o $@
