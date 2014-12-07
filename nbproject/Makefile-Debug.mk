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
CND_PLATFORM=None-Windows
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
	${OBJECTDIR}/course_includes/lcddriver.o \
	${OBJECTDIR}/course_includes/termio.o \
	${OBJECTDIR}/dictionary/spi_flash.o \
	${OBJECTDIR}/display/vga2.o \
	${OBJECTDIR}/game_logic/boggle.o \
	${OBJECTDIR}/keyboard/keyboard.o \
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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/code.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/code.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/code ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/course_includes/lcddriver.o: course_includes/lcddriver.c 
	${MKDIR} -p ${OBJECTDIR}/course_includes
	${RM} "$@.d"
	$(COMPILE.c) -g -I/C/Program\ Files/Freescale/CWS12v5.1/lib/hc12c/include/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/course_includes/lcddriver.o course_includes/lcddriver.c

${OBJECTDIR}/course_includes/termio.o: course_includes/termio.c 
	${MKDIR} -p ${OBJECTDIR}/course_includes
	${RM} "$@.d"
	$(COMPILE.c) -g -I/C/Program\ Files/Freescale/CWS12v5.1/lib/hc12c/include/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/course_includes/termio.o course_includes/termio.c

${OBJECTDIR}/dictionary/spi_flash.o: dictionary/spi_flash.c 
	${MKDIR} -p ${OBJECTDIR}/dictionary
	${RM} "$@.d"
	$(COMPILE.c) -g -I/C/Program\ Files/Freescale/CWS12v5.1/lib/hc12c/include/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dictionary/spi_flash.o dictionary/spi_flash.c

${OBJECTDIR}/display/vga2.o: display/vga2.asm 
	${MKDIR} -p ${OBJECTDIR}/display
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/display/vga2.o display/vga2.asm

${OBJECTDIR}/game_logic/boggle.o: game_logic/boggle.c 
	${MKDIR} -p ${OBJECTDIR}/game_logic
	${RM} "$@.d"
	$(COMPILE.c) -g -I/C/Program\ Files/Freescale/CWS12v5.1/lib/hc12c/include/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_logic/boggle.o game_logic/boggle.c

${OBJECTDIR}/keyboard/keyboard.o: keyboard/keyboard.c 
	${MKDIR} -p ${OBJECTDIR}/keyboard
	${RM} "$@.d"
	$(COMPILE.c) -g -I/C/Program\ Files/Freescale/CWS12v5.1/lib/hc12c/include/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/keyboard/keyboard.o keyboard/keyboard.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I/C/Program\ Files/Freescale/CWS12v5.1/lib/hc12c/include/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/code.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
