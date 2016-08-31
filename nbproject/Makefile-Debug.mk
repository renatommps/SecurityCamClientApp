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
CND_PLATFORM=GNU-Linux
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
	${OBJECTDIR}/bufferManager.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/messageDealer.o \
	${OBJECTDIR}/processingTask.o \
	${OBJECTDIR}/storageTask.o \
	${OBJECTDIR}/synchronizationAndStatusDealer.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pthread -std=c++11
CXXFLAGS=-pthread -std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib -Wl,-rpath,/usr/local/include/opencv2 -Wl,-rpath,/usr/local/lib `pkg-config --libs pthread-stubs` -lpthread  -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab -lzmq -lzmq -lzmqpp -lzmqpp -lpthread   

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/zqmclient

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/zqmclient: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/zqmclient ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/bufferManager.o: bufferManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/opencv2 -I/usr/local/include/opencv2/opencv.hpp `pkg-config --cflags pthread-stubs`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bufferManager.o bufferManager.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/opencv2 -I/usr/local/include/opencv2/opencv.hpp `pkg-config --cflags pthread-stubs`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/messageDealer.o: messageDealer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/opencv2 -I/usr/local/include/opencv2/opencv.hpp `pkg-config --cflags pthread-stubs`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/messageDealer.o messageDealer.cpp

${OBJECTDIR}/processingTask.o: processingTask.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/opencv2 -I/usr/local/include/opencv2/opencv.hpp `pkg-config --cflags pthread-stubs`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/processingTask.o processingTask.cpp

${OBJECTDIR}/storageTask.o: storageTask.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/opencv2 -I/usr/local/include/opencv2/opencv.hpp `pkg-config --cflags pthread-stubs`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/storageTask.o storageTask.cpp

${OBJECTDIR}/synchronizationAndStatusDealer.o: synchronizationAndStatusDealer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/opencv2 -I/usr/local/include/opencv2/opencv.hpp `pkg-config --cflags pthread-stubs`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/synchronizationAndStatusDealer.o synchronizationAndStatusDealer.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/zqmclient

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
