#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ASSISICasu_v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ASSISICasu_v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=/home/thaus/development/mplab/test/CASU/initizializeHardware.c /home/thaus/development/mplab/test/CASU/main.c /home/thaus/development/mplab/peripheral/gpio/digitalIO.c /home/thaus/development/mplab/peripheral/i2c/i2c2.c /home/thaus/development/mplab/pic-to-beaglebone/pic2beaglebone.c /home/thaus/development/mplab/peripheral/timer/timer1.c /home/thaus/development/mplab/peripheral/timer/timerFunctions.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1758399873/initizializeHardware.o ${OBJECTDIR}/_ext/1758399873/main.o ${OBJECTDIR}/_ext/1799869156/digitalIO.o ${OBJECTDIR}/_ext/773223707/i2c2.o ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o ${OBJECTDIR}/_ext/26830128/timer1.o ${OBJECTDIR}/_ext/26830128/timerFunctions.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1758399873/initizializeHardware.o.d ${OBJECTDIR}/_ext/1758399873/main.o.d ${OBJECTDIR}/_ext/1799869156/digitalIO.o.d ${OBJECTDIR}/_ext/773223707/i2c2.o.d ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o.d ${OBJECTDIR}/_ext/26830128/timer1.o.d ${OBJECTDIR}/_ext/26830128/timerFunctions.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1758399873/initizializeHardware.o ${OBJECTDIR}/_ext/1758399873/main.o ${OBJECTDIR}/_ext/1799869156/digitalIO.o ${OBJECTDIR}/_ext/773223707/i2c2.o ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o ${OBJECTDIR}/_ext/26830128/timer1.o ${OBJECTDIR}/_ext/26830128/timerFunctions.o

# Source Files
SOURCEFILES=/home/thaus/development/mplab/test/CASU/initizializeHardware.c /home/thaus/development/mplab/test/CASU/main.c /home/thaus/development/mplab/peripheral/gpio/digitalIO.c /home/thaus/development/mplab/peripheral/i2c/i2c2.c /home/thaus/development/mplab/pic-to-beaglebone/pic2beaglebone.c /home/thaus/development/mplab/peripheral/timer/timer1.c /home/thaus/development/mplab/peripheral/timer/timerFunctions.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/ASSISICasu_v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP256MU810
MP_LINKER_FILE_OPTION=,--script=p33EP256MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1758399873/initizializeHardware.o: /home/thaus/development/mplab/test/CASU/initizializeHardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758399873 
	@${RM} ${OBJECTDIR}/_ext/1758399873/initizializeHardware.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758399873/initizializeHardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/test/CASU/initizializeHardware.c  -o ${OBJECTDIR}/_ext/1758399873/initizializeHardware.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758399873/initizializeHardware.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758399873/initizializeHardware.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758399873/main.o: /home/thaus/development/mplab/test/CASU/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758399873 
	@${RM} ${OBJECTDIR}/_ext/1758399873/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758399873/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/test/CASU/main.c  -o ${OBJECTDIR}/_ext/1758399873/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758399873/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758399873/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1799869156/digitalIO.o: /home/thaus/development/mplab/peripheral/gpio/digitalIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1799869156 
	@${RM} ${OBJECTDIR}/_ext/1799869156/digitalIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/1799869156/digitalIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/peripheral/gpio/digitalIO.c  -o ${OBJECTDIR}/_ext/1799869156/digitalIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1799869156/digitalIO.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1799869156/digitalIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/773223707/i2c2.o: /home/thaus/development/mplab/peripheral/i2c/i2c2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/773223707 
	@${RM} ${OBJECTDIR}/_ext/773223707/i2c2.o.d 
	@${RM} ${OBJECTDIR}/_ext/773223707/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/peripheral/i2c/i2c2.c  -o ${OBJECTDIR}/_ext/773223707/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/773223707/i2c2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/773223707/i2c2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o: /home/thaus/development/mplab/pic-to-beaglebone/pic2beaglebone.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1727728105 
	@${RM} ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o.d 
	@${RM} ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/pic-to-beaglebone/pic2beaglebone.c  -o ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/26830128/timer1.o: /home/thaus/development/mplab/peripheral/timer/timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/26830128 
	@${RM} ${OBJECTDIR}/_ext/26830128/timer1.o.d 
	@${RM} ${OBJECTDIR}/_ext/26830128/timer1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/peripheral/timer/timer1.c  -o ${OBJECTDIR}/_ext/26830128/timer1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/26830128/timer1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/26830128/timer1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/26830128/timerFunctions.o: /home/thaus/development/mplab/peripheral/timer/timerFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/26830128 
	@${RM} ${OBJECTDIR}/_ext/26830128/timerFunctions.o.d 
	@${RM} ${OBJECTDIR}/_ext/26830128/timerFunctions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/peripheral/timer/timerFunctions.c  -o ${OBJECTDIR}/_ext/26830128/timerFunctions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/26830128/timerFunctions.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/26830128/timerFunctions.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1758399873/initizializeHardware.o: /home/thaus/development/mplab/test/CASU/initizializeHardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758399873 
	@${RM} ${OBJECTDIR}/_ext/1758399873/initizializeHardware.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758399873/initizializeHardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/test/CASU/initizializeHardware.c  -o ${OBJECTDIR}/_ext/1758399873/initizializeHardware.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758399873/initizializeHardware.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758399873/initizializeHardware.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758399873/main.o: /home/thaus/development/mplab/test/CASU/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758399873 
	@${RM} ${OBJECTDIR}/_ext/1758399873/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758399873/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/test/CASU/main.c  -o ${OBJECTDIR}/_ext/1758399873/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758399873/main.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758399873/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1799869156/digitalIO.o: /home/thaus/development/mplab/peripheral/gpio/digitalIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1799869156 
	@${RM} ${OBJECTDIR}/_ext/1799869156/digitalIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/1799869156/digitalIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/peripheral/gpio/digitalIO.c  -o ${OBJECTDIR}/_ext/1799869156/digitalIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1799869156/digitalIO.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1799869156/digitalIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/773223707/i2c2.o: /home/thaus/development/mplab/peripheral/i2c/i2c2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/773223707 
	@${RM} ${OBJECTDIR}/_ext/773223707/i2c2.o.d 
	@${RM} ${OBJECTDIR}/_ext/773223707/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/peripheral/i2c/i2c2.c  -o ${OBJECTDIR}/_ext/773223707/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/773223707/i2c2.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/773223707/i2c2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o: /home/thaus/development/mplab/pic-to-beaglebone/pic2beaglebone.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1727728105 
	@${RM} ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o.d 
	@${RM} ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/pic-to-beaglebone/pic2beaglebone.c  -o ${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1727728105/pic2beaglebone.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/26830128/timer1.o: /home/thaus/development/mplab/peripheral/timer/timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/26830128 
	@${RM} ${OBJECTDIR}/_ext/26830128/timer1.o.d 
	@${RM} ${OBJECTDIR}/_ext/26830128/timer1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/peripheral/timer/timer1.c  -o ${OBJECTDIR}/_ext/26830128/timer1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/26830128/timer1.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/26830128/timer1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/26830128/timerFunctions.o: /home/thaus/development/mplab/peripheral/timer/timerFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/26830128 
	@${RM} ${OBJECTDIR}/_ext/26830128/timerFunctions.o.d 
	@${RM} ${OBJECTDIR}/_ext/26830128/timerFunctions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  /home/thaus/development/mplab/peripheral/timer/timerFunctions.c  -o ${OBJECTDIR}/_ext/26830128/timerFunctions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/26830128/timerFunctions.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/26830128/timerFunctions.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/ASSISICasu_v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/ASSISICasu_v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/ASSISICasu_v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/ASSISICasu_v2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/ASSISICasu_v2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
