#####
# A CMake build file for the OpenContainingFolder service, a service that can
# walk directories and invoke JS callbacks for the contained items.
# 
# Copyright 2009, Yahoo!
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
# 
#  3. Neither the name of Yahoo! nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# 
# Original Author: Gordon Durand
#####

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../external/bp-service-framework/build_support/BuildConfigs.cmake)

SET(SERVICE_NAME OpenContainingFolder)
PROJECT(${SERVICE_NAME}Service)

INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}/../external/dist/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../external/bp-service-framework/include"
)
 
LINK_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}/../external/dist/lib"
  "${CMAKE_CURRENT_SOURCE_DIR}/../external/dist/lib/${CMAKE_BUILD_TYPE}"
)
 
SET(OS_SRCS)
IF (WIN32)
   SET(OS_SRCS service_Windows.cpp)
ELSEIF (APPLE)
   SET(OS_SRCS service_Darwin.mm)
ENDIF ()

# boost libs
#
IF (WIN32)
   # Visual Studio does some autolink magic with boost, no need
   # to specify library
ELSE ()
   SET(BOOST_LIBS "boost_filesystem" "boost_system")
ENDIF ()
SET(OS_LIBS)
IF (APPLE)
   # need cocao headers and library
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   MARK_AS_ADVANCED(COCOA_LIBRARY)
   SET(OS_LIBS ${COCOA_LIBRARY})
ENDIF() 
SET(SRCS service.cpp ${OS_SRCS})
SET(HDRS "")
SET(LIBS bpfile_s ${BOOST_LIBS} ${OS_LIBS})

BPAddCppService()

