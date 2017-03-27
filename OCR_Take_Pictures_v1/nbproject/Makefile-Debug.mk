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
CND_PLATFORM=MinGW-Windows
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
	${OBJECTDIR}/_ext/716087799/Serial.o \
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
LDLIBSOPTIONS=-L/C/OpenCV/opencv/release/lib -lopencv_calib3d249.dll -lopencv_calib3d_pch_dephelp -lopencv_contrib249.dll -lopencv_contrib_pch_dephelp -lopencv_core249.dll -lopencv_core_pch_dephelp -lopencv_features2d249.dll -lopencv_features2d_pch_dephelp -lopencv_flann249.dll -lopencv_flann_pch_dephelp -lopencv_gpu249.dll -lopencv_gpu_pch_dephelp -lopencv_haartraining_engine -lopencv_highgui249.dll -lopencv_highgui_pch_dephelp -lopencv_imgproc249.dll -lopencv_imgproc_pch_dephelp -lopencv_legacy249.dll -lopencv_legacy_pch_dephelp -lopencv_ml249.dll -lopencv_ml_pch_dephelp -lopencv_nonfree249.dll -lopencv_nonfree_pch_dephelp -lopencv_objdetect249.dll -lopencv_objdetect_pch_dephelp -lopencv_ocl249.dll -lopencv_ocl_pch_dephelp -lopencv_perf_calib3d_pch_dephelp -lopencv_perf_core_pch_dephelp -lopencv_perf_features2d_pch_dephelp -lopencv_perf_gpu_pch_dephelp -lopencv_perf_highgui_pch_dephelp -lopencv_perf_imgproc_pch_dephelp -lopencv_perf_nonfree_pch_dephelp -lopencv_perf_objdetect_pch_dephelp -lopencv_perf_ocl_pch_dephelp -lopencv_perf_photo_pch_dephelp -lopencv_perf_stitching_pch_dephelp -lopencv_perf_superres_pch_dephelp -lopencv_perf_video_pch_dephelp -lopencv_photo249.dll -lopencv_photo_pch_dephelp -lopencv_stitching249.dll -lopencv_stitching_pch_dephelp -lopencv_superres249.dll -lopencv_superres_pch_dephelp -lopencv_test_calib3d_pch_dephelp -lopencv_test_contrib_pch_dephelp -lopencv_test_core_pch_dephelp -lopencv_test_features2d_pch_dephelp -lopencv_test_flann_pch_dephelp -lopencv_test_gpu_pch_dephelp -lopencv_test_highgui_pch_dephelp -lopencv_test_imgproc_pch_dephelp -lopencv_test_legacy_pch_dephelp -lopencv_test_ml_pch_dephelp -lopencv_test_nonfree_pch_dephelp -lopencv_test_objdetect_pch_dephelp -lopencv_test_ocl_pch_dephelp -lopencv_test_photo_pch_dephelp -lopencv_test_stitching_pch_dephelp -lopencv_test_superres_pch_dephelp -lopencv_test_video_pch_dephelp -lopencv_ts249 -lopencv_ts_pch_dephelp -lopencv_video249.dll -lopencv_video_pch_dephelp -lopencv_videostab249.dll -lopencv_videostab_pch_dephelp

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ocr_take_pictures_v1.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ocr_take_pictures_v1.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ocr_take_pictures_v1 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/716087799/Serial.o: /C/Users/elliot/Documents/GitHub/ReceivingLine/OCR_Take_Pictures_v1/Serial.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/716087799
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/OpenCV/opencv/build/include/opencv -I/C/OpenCV/opencv/build/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/716087799/Serial.o /C/Users/elliot/Documents/GitHub/ReceivingLine/OCR_Take_Pictures_v1/Serial.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/OpenCV/opencv/build/include/opencv -I/C/OpenCV/opencv/build/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ocr_take_pictures_v1.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
