###########################################
# Simple Makefile for HIDAPI test program
#
# Alan Ott
# Signal 11 Software
# 2010-06-01
###########################################

all: main

CC       ?= gcc
CFLAGS   ?= -Wall -g -fpic

CXX      ?= g++
CXXFLAGS ?= -Wall -g -fpic

LDFLAGS  ?= -Wall -g


COBJS     = hid.o
CPPOBJS   = main.o window.o
OBJS      = $(COBJS) $(CPPOBJS)
LIBS_UDEV = `pkg-config libudev --libs` -lrt
LIBS      = $(LIBS_UDEV) `pkg-config gtkmm-3.0 libvlc --libs`
INCLUDES ?= -I./ `pkg-config libusb-1.0 gtkmm-3.0 libvlc --cflags`


# Console Test Program
main: $(COBJS) $(CPPOBJS)
	$(CXX) $(LDFLAGS) $^ $(LIBS_UDEV) -o $@

# Objects
$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

$(CPPOBJS): %.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) $< -o $@


clean:
	rm -f $(OBJS) hidtest-hidraw libhidapi-hidraw.so ../hidtest/hidtest.o

.PHONY: clean libs
