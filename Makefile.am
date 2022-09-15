# Makefile.am for i.MX GStreamer NN Inference demo plugin
#
# SPDX-License-Identifier: LGPL-2.0+
# Copyright 2021 NXP

# Tensorflow Lite
TFLITE_LIBS = -ltensorflow-lite

##############################################################################
# libposenetimx8.la
##############################################################################
plugin_LTLIBRARIES = libposenetimx8.la

#bin_PROGRAMS = gstnninferencedemo
HEADERS_TF = \
  src\general_inferencer.h \
  src\tflite_inference.h \
  include\tensorResultToPassOn.h \
  include\tensorResultToPassOnFloat.h \
  include\tensorResultToPassOnUint8.h \
  src\coralIcarusInference.h \
  include\coralIcarusInterface.h

SOURCES_TF = \
  src\general_inferencer.cpp \
  src\tflite_inference.cpp \
  src\tensorResultToPassOn.cpp \
  src\tensorResultToPassOnFloat.cpp \
  src\tensorResultToPassOnUint8.cpp \
  src\coralIcarusInference.cpp

noinst_HEADERS = $(HEADERS_TF)
libposenetimx8_la_SOURCES = $(SOURCES_TF)

if USE_ION
libposenetimx8_la_CFLAGS = -DUSE_ION
endif

libposenetimx8_la_CXXFLAGS = \
  $(libposenetimx8_la_CFLAGS)

libposenetimx8_la_LIBADD = \
  $(TFLITE_LIBS) \
  -lgooglecoraledgetpuposenet

# package name
PACKAGE_NAME=icarusTensorflowInferencer

# README
pkgdatadir = $(datadir)/$(PACKAGE_NAME)
dist_pkgdata_DATA = \
  $(top_srcdir)/README \
  $(top_srcdir)/COPYING-LGPL2.0 \
  $(top_srcdir)/COPYING-LGPL2.1

# don't install libposenetimx8.la
install-data-hook:
	@(cd $(DESTDIR)/$(plugindir) && $(RM) $(plugin_LTLIBRARIES))
