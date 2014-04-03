#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1410871718/Module_monitor.o \
	${OBJECTDIR}/Cfg_item.o \
	${OBJECTDIR}/Configuration.o \
	${OBJECTDIR}/Module.o \
	${OBJECTDIR}/Module_manager.o \
	${OBJECTDIR}/Module_usb.o \
	${OBJECTDIR}/Term_evaluation.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs libglog` `pkg-config --libs libusb-1.0` `pkg-config --libs xrandr` `pkg-config --libs gtk+-3.0` `pkg-config --libs x11`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk profile_manager

profile_manager: ${OBJECTFILES}
	${LINK.cc} -o profile_manager ${OBJECTFILES} ${LDLIBSOPTIONS} -lboost_program_options -lboost_system -lboost_filesystem -linotifytools

${OBJECTDIR}/_ext/1410871718/Module_monitor.o: /home/lefty/NetBeansProjects/profile_manager/Module_monitor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1410871718
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include `pkg-config --cflags libglog` `pkg-config --cflags libusb-1.0` `pkg-config --cflags xrandr` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags x11` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1410871718/Module_monitor.o /home/lefty/NetBeansProjects/profile_manager/Module_monitor.cpp

${OBJECTDIR}/Cfg_item.o: Cfg_item.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include `pkg-config --cflags libglog` `pkg-config --cflags libusb-1.0` `pkg-config --cflags xrandr` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags x11` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Cfg_item.o Cfg_item.cpp

${OBJECTDIR}/Configuration.o: Configuration.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include `pkg-config --cflags libglog` `pkg-config --cflags libusb-1.0` `pkg-config --cflags xrandr` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags x11` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Configuration.o Configuration.cpp

${OBJECTDIR}/Module.o: Module.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include `pkg-config --cflags libglog` `pkg-config --cflags libusb-1.0` `pkg-config --cflags xrandr` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags x11` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Module.o Module.cpp

${OBJECTDIR}/Module_manager.o: Module_manager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include `pkg-config --cflags libglog` `pkg-config --cflags libusb-1.0` `pkg-config --cflags xrandr` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags x11` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Module_manager.o Module_manager.cpp

${OBJECTDIR}/Module_usb.o: Module_usb.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include `pkg-config --cflags libglog` `pkg-config --cflags libusb-1.0` `pkg-config --cflags xrandr` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags x11` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Module_usb.o Module_usb.cpp

${OBJECTDIR}/Term_evaluation.o: Term_evaluation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include `pkg-config --cflags libglog` `pkg-config --cflags libusb-1.0` `pkg-config --cflags xrandr` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags x11` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Term_evaluation.o Term_evaluation.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include `pkg-config --cflags libglog` `pkg-config --cflags libusb-1.0` `pkg-config --cflags xrandr` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags x11` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} profile_manager

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
