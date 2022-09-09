#
# This file was derived from the Posenet-imx8 recipe in the
# Yocto Project Development Manual.
#

DESCRIPTION = "Shared Library for Posenet Inference with IMx8"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=d41d8cd98f00b204e9800998ecf8427e"


DEPENDS = "google-coral-posenet"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

SRCREV = "ef71901c48bed8f226c52b1e3f9ed7e880ef1ab4"
SRC_URI = "git://github.com/vamsimarri/posenet-imx8.git;branch=main"

S = "${WORKDIR}/git"

inherit autotools pkgconfig

COMPATIBLE_MACHINE = "(mx8mm|mx8mp)"
