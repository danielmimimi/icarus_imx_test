#!/bin/bash
# ****************************************************************************
#  
#                      Simple ARM build Skript
# ----------------------------------------------------------------------------
#
#  @author            Daniel Klauser
#           
#  @copyright         CC Intelligent Sensors and Networks
#                     at Lucerne University of Applied Sciences 
#                     and Arts T&A, Switzerland. 
#
# ****************************************************************************
export TARGET_TYPE=arm
make clean
make all

