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
CND_PLATFORM=MinGW_1-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lamp.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/newmain.o


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
LDLIBSOPTIONS=-L/C/Program\ Files\ \(x86\)/mosquitto -L/C/Program\ Files\ \(x86\)/mosquitto/devel /C/Code/mingw/lib/libopencv_calib3d300.dll.a /C/Code/mingw/lib/libopencv_core300.dll.a /C/Code/mingw/lib/libopencv_features2d300.dll.a /C/Code/mingw/lib/libopencv_flann300.dll.a /C/Code/mingw/lib/libopencv_hal300.a /C/Code/mingw/lib/libopencv_highgui300.dll.a /C/Code/mingw/lib/libopencv_imgcodecs300.dll.a /C/Code/mingw/lib/libopencv_imgproc300.dll.a /C/Code/mingw/lib/libopencv_ml300.dll.a /C/Code/mingw/lib/libopencv_objdetect300.dll.a /C/Code/mingw/lib/libopencv_photo300.dll.a /C/Code/mingw/lib/libopencv_shape300.dll.a /C/Code/mingw/lib/libopencv_stitching300.dll.a /C/Code/mingw/lib/libopencv_superres300.dll.a /C/Code/mingw/lib/libopencv_ts300.a /C/Code/mingw/lib/libopencv_video300.dll.a /C/Code/mingw/lib/libopencv_videoio300.dll.a /C/Code/mingw/lib/libopencv_videostab300.dll.a -lmosquitto -lmosquittopp -leay32 -lmosquitto -lmosquittopp -lpthreadVC2 -lssleay32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_calib3d300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_core300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_features2d300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_flann300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_hal300.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_highgui300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_imgcodecs300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_imgproc300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_ml300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_objdetect300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_photo300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_shape300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_stitching300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_superres300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_ts300.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_video300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_videoio300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: /C/Code/mingw/lib/libopencv_videostab300.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/lamp.o: lamp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/Program\ Files\ \(x86\)/mosquitto/devel -I/C/Program\ Files\ \(x86\)/mosquitto -include /C/Program\ Files\ \(x86\)/mosquitto/devel -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lamp.o lamp.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/Program\ Files\ \(x86\)/mosquitto/devel -I/C/Program\ Files\ \(x86\)/mosquitto -include /C/Program\ Files\ \(x86\)/mosquitto/devel -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/newmain.o: newmain.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/Program\ Files\ \(x86\)/mosquitto/devel -I/C/Program\ Files\ \(x86\)/mosquitto -include /C/Program\ Files\ \(x86\)/mosquitto/devel -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/newmain.o newmain.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opencvtesting.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
