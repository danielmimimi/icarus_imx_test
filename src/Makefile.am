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
  general_inferencer.h \
  tflite_inference.h \
  tensorResultToPassOn.h \
  tensorResultToPassOnFloat.h \
  tensorResultToPassOnUint8.h \
  coralIcarusInference.h \
  coralIcarusInterface.h

SOURCES_TF = \
  general_inferencer.cpp \
  tflite_inference.cpp \
  tensorResultToPassOn.cpp \
  tensorResultToPassOnFloat.cpp \
  tensorResultToPassOnUint8.cpp \
  coralIcarusInference.cpp

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
