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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/assisi-dspic-fft.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/assisi-dspic-fft.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../actuators/peltier.c ../actuators/pwm.c ../casu/initizializeHardware.c ../casu/main.c ../casu/error.c ../fft/fft.c ../peripheral/gpio/digitalIO.c ../peripheral/i2c/i2c2.c ../peripheral/i2c/i2c1.c ../peripheral/spi/spi1.c ../peripheral/spi/spi2.c ../peripheral/timer/timer2.c ../peripheral/timer/timer45.c ../peripheral/timer/timer1.c ../peripheral/timer/timerFunctions.c ../pic-to-beaglebone/pic2beaglebone.c ../sensors/adt7320/adt7320.c ../sensors/adt7420/adt7420.c ../sensors/adxl345/adxl345.c ../sensors/vcnl4000/proxiVCNL4000.c ../sensors/proximity.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/470364171/peltier.o ${OBJECTDIR}/_ext/470364171/pwm.o ${OBJECTDIR}/_ext/761110993/initizializeHardware.o ${OBJECTDIR}/_ext/761110993/main.o ${OBJECTDIR}/_ext/761110993/error.o ${OBJECTDIR}/_ext/1360924389/fft.o ${OBJECTDIR}/_ext/290349517/digitalIO.o ${OBJECTDIR}/_ext/1099012534/i2c2.o ${OBJECTDIR}/_ext/1099012534/i2c1.o ${OBJECTDIR}/_ext/1099024072/spi1.o ${OBJECTDIR}/_ext/1099024072/spi2.o ${OBJECTDIR}/_ext/399099551/timer2.o ${OBJECTDIR}/_ext/399099551/timer45.o ${OBJECTDIR}/_ext/399099551/timer1.o ${OBJECTDIR}/_ext/399099551/timerFunctions.o ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o ${OBJECTDIR}/_ext/1728059366/adt7320.o ${OBJECTDIR}/_ext/1728060327/adt7420.o ${OBJECTDIR}/_ext/1733332440/adxl345.o ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o ${OBJECTDIR}/_ext/299869866/proximity.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/470364171/peltier.o.d ${OBJECTDIR}/_ext/470364171/pwm.o.d ${OBJECTDIR}/_ext/761110993/initizializeHardware.o.d ${OBJECTDIR}/_ext/761110993/main.o.d ${OBJECTDIR}/_ext/761110993/error.o.d ${OBJECTDIR}/_ext/1360924389/fft.o.d ${OBJECTDIR}/_ext/290349517/digitalIO.o.d ${OBJECTDIR}/_ext/1099012534/i2c2.o.d ${OBJECTDIR}/_ext/1099012534/i2c1.o.d ${OBJECTDIR}/_ext/1099024072/spi1.o.d ${OBJECTDIR}/_ext/1099024072/spi2.o.d ${OBJECTDIR}/_ext/399099551/timer2.o.d ${OBJECTDIR}/_ext/399099551/timer45.o.d ${OBJECTDIR}/_ext/399099551/timer1.o.d ${OBJECTDIR}/_ext/399099551/timerFunctions.o.d ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o.d ${OBJECTDIR}/_ext/1728059366/adt7320.o.d ${OBJECTDIR}/_ext/1728060327/adt7420.o.d ${OBJECTDIR}/_ext/1733332440/adxl345.o.d ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o.d ${OBJECTDIR}/_ext/299869866/proximity.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/470364171/peltier.o ${OBJECTDIR}/_ext/470364171/pwm.o ${OBJECTDIR}/_ext/761110993/initizializeHardware.o ${OBJECTDIR}/_ext/761110993/main.o ${OBJECTDIR}/_ext/761110993/error.o ${OBJECTDIR}/_ext/1360924389/fft.o ${OBJECTDIR}/_ext/290349517/digitalIO.o ${OBJECTDIR}/_ext/1099012534/i2c2.o ${OBJECTDIR}/_ext/1099012534/i2c1.o ${OBJECTDIR}/_ext/1099024072/spi1.o ${OBJECTDIR}/_ext/1099024072/spi2.o ${OBJECTDIR}/_ext/399099551/timer2.o ${OBJECTDIR}/_ext/399099551/timer45.o ${OBJECTDIR}/_ext/399099551/timer1.o ${OBJECTDIR}/_ext/399099551/timerFunctions.o ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o ${OBJECTDIR}/_ext/1728059366/adt7320.o ${OBJECTDIR}/_ext/1728060327/adt7420.o ${OBJECTDIR}/_ext/1733332440/adxl345.o ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o ${OBJECTDIR}/_ext/299869866/proximity.o

# Source Files
SOURCEFILES=../actuators/peltier.c ../actuators/pwm.c ../casu/initizializeHardware.c ../casu/main.c ../casu/error.c ../fft/fft.c ../peripheral/gpio/digitalIO.c ../peripheral/i2c/i2c2.c ../peripheral/i2c/i2c1.c ../peripheral/spi/spi1.c ../peripheral/spi/spi2.c ../peripheral/timer/timer2.c ../peripheral/timer/timer45.c ../peripheral/timer/timer1.c ../peripheral/timer/timerFunctions.c ../pic-to-beaglebone/pic2beaglebone.c ../sensors/adt7320/adt7320.c ../sensors/adt7420/adt7420.c ../sensors/adxl345/adxl345.c ../sensors/vcnl4000/proxiVCNL4000.c ../sensors/proximity.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/assisi-dspic-fft.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP256MU810
MP_LINKER_FILE_OPTION=,--script=p33EP256MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/470364171/peltier.o: ../actuators/peltier.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/470364171 
	@${RM} ${OBJECTDIR}/_ext/470364171/peltier.o.d 
	@${RM} ${OBJECTDIR}/_ext/470364171/peltier.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../actuators/peltier.c  -o ${OBJECTDIR}/_ext/470364171/peltier.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/470364171/peltier.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/470364171/peltier.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/470364171/pwm.o: ../actuators/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/470364171 
	@${RM} ${OBJECTDIR}/_ext/470364171/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/470364171/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../actuators/pwm.c  -o ${OBJECTDIR}/_ext/470364171/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/470364171/pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/470364171/pwm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/761110993/initizializeHardware.o: ../casu/initizializeHardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/761110993 
	@${RM} ${OBJECTDIR}/_ext/761110993/initizializeHardware.o.d 
	@${RM} ${OBJECTDIR}/_ext/761110993/initizializeHardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../casu/initizializeHardware.c  -o ${OBJECTDIR}/_ext/761110993/initizializeHardware.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/761110993/initizializeHardware.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/761110993/initizializeHardware.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/761110993/main.o: ../casu/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/761110993 
	@${RM} ${OBJECTDIR}/_ext/761110993/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/761110993/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../casu/main.c  -o ${OBJECTDIR}/_ext/761110993/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/761110993/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/761110993/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/761110993/error.o: ../casu/error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/761110993 
	@${RM} ${OBJECTDIR}/_ext/761110993/error.o.d 
	@${RM} ${OBJECTDIR}/_ext/761110993/error.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../casu/error.c  -o ${OBJECTDIR}/_ext/761110993/error.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/761110993/error.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/761110993/error.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360924389/fft.o: ../fft/fft.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360924389 
	@${RM} ${OBJECTDIR}/_ext/1360924389/fft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360924389/fft.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../fft/fft.c  -o ${OBJECTDIR}/_ext/1360924389/fft.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360924389/fft.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360924389/fft.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/290349517/digitalIO.o: ../peripheral/gpio/digitalIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/290349517 
	@${RM} ${OBJECTDIR}/_ext/290349517/digitalIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/290349517/digitalIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/gpio/digitalIO.c  -o ${OBJECTDIR}/_ext/290349517/digitalIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/290349517/digitalIO.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/290349517/digitalIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1099012534/i2c2.o: ../peripheral/i2c/i2c2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1099012534 
	@${RM} ${OBJECTDIR}/_ext/1099012534/i2c2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1099012534/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/i2c/i2c2.c  -o ${OBJECTDIR}/_ext/1099012534/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1099012534/i2c2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1099012534/i2c2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1099012534/i2c1.o: ../peripheral/i2c/i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1099012534 
	@${RM} ${OBJECTDIR}/_ext/1099012534/i2c1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1099012534/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/i2c/i2c1.c  -o ${OBJECTDIR}/_ext/1099012534/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1099012534/i2c1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1099012534/i2c1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1099024072/spi1.o: ../peripheral/spi/spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1099024072 
	@${RM} ${OBJECTDIR}/_ext/1099024072/spi1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1099024072/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/spi/spi1.c  -o ${OBJECTDIR}/_ext/1099024072/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1099024072/spi1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1099024072/spi1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1099024072/spi2.o: ../peripheral/spi/spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1099024072 
	@${RM} ${OBJECTDIR}/_ext/1099024072/spi2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1099024072/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/spi/spi2.c  -o ${OBJECTDIR}/_ext/1099024072/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1099024072/spi2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1099024072/spi2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/399099551/timer2.o: ../peripheral/timer/timer2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/399099551 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer2.o.d 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/timer/timer2.c  -o ${OBJECTDIR}/_ext/399099551/timer2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/399099551/timer2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/399099551/timer2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/399099551/timer45.o: ../peripheral/timer/timer45.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/399099551 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer45.o.d 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer45.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/timer/timer45.c  -o ${OBJECTDIR}/_ext/399099551/timer45.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/399099551/timer45.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/399099551/timer45.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/399099551/timer1.o: ../peripheral/timer/timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/399099551 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer1.o.d 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/timer/timer1.c  -o ${OBJECTDIR}/_ext/399099551/timer1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/399099551/timer1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/399099551/timer1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/399099551/timerFunctions.o: ../peripheral/timer/timerFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/399099551 
	@${RM} ${OBJECTDIR}/_ext/399099551/timerFunctions.o.d 
	@${RM} ${OBJECTDIR}/_ext/399099551/timerFunctions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/timer/timerFunctions.c  -o ${OBJECTDIR}/_ext/399099551/timerFunctions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/399099551/timerFunctions.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/399099551/timerFunctions.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/383178330/pic2beaglebone.o: ../pic-to-beaglebone/pic2beaglebone.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/383178330 
	@${RM} ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o.d 
	@${RM} ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic-to-beaglebone/pic2beaglebone.c  -o ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/383178330/pic2beaglebone.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/383178330/pic2beaglebone.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1728059366/adt7320.o: ../sensors/adt7320/adt7320.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1728059366 
	@${RM} ${OBJECTDIR}/_ext/1728059366/adt7320.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728059366/adt7320.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/adt7320/adt7320.c  -o ${OBJECTDIR}/_ext/1728059366/adt7320.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1728059366/adt7320.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1728059366/adt7320.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1728060327/adt7420.o: ../sensors/adt7420/adt7420.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1728060327 
	@${RM} ${OBJECTDIR}/_ext/1728060327/adt7420.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728060327/adt7420.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/adt7420/adt7420.c  -o ${OBJECTDIR}/_ext/1728060327/adt7420.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1728060327/adt7420.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1728060327/adt7420.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733332440/adxl345.o: ../sensors/adxl345/adxl345.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733332440 
	@${RM} ${OBJECTDIR}/_ext/1733332440/adxl345.o.d 
	@${RM} ${OBJECTDIR}/_ext/1733332440/adxl345.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/adxl345/adxl345.c  -o ${OBJECTDIR}/_ext/1733332440/adxl345.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733332440/adxl345.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733332440/adxl345.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o: ../sensors/vcnl4000/proxiVCNL4000.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1035759892 
	@${RM} ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o.d 
	@${RM} ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/vcnl4000/proxiVCNL4000.c  -o ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/299869866/proximity.o: ../sensors/proximity.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/299869866 
	@${RM} ${OBJECTDIR}/_ext/299869866/proximity.o.d 
	@${RM} ${OBJECTDIR}/_ext/299869866/proximity.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/proximity.c  -o ${OBJECTDIR}/_ext/299869866/proximity.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/299869866/proximity.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/299869866/proximity.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/470364171/peltier.o: ../actuators/peltier.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/470364171 
	@${RM} ${OBJECTDIR}/_ext/470364171/peltier.o.d 
	@${RM} ${OBJECTDIR}/_ext/470364171/peltier.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../actuators/peltier.c  -o ${OBJECTDIR}/_ext/470364171/peltier.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/470364171/peltier.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/470364171/peltier.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/470364171/pwm.o: ../actuators/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/470364171 
	@${RM} ${OBJECTDIR}/_ext/470364171/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/470364171/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../actuators/pwm.c  -o ${OBJECTDIR}/_ext/470364171/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/470364171/pwm.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/470364171/pwm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/761110993/initizializeHardware.o: ../casu/initizializeHardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/761110993 
	@${RM} ${OBJECTDIR}/_ext/761110993/initizializeHardware.o.d 
	@${RM} ${OBJECTDIR}/_ext/761110993/initizializeHardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../casu/initizializeHardware.c  -o ${OBJECTDIR}/_ext/761110993/initizializeHardware.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/761110993/initizializeHardware.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/761110993/initizializeHardware.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/761110993/main.o: ../casu/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/761110993 
	@${RM} ${OBJECTDIR}/_ext/761110993/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/761110993/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../casu/main.c  -o ${OBJECTDIR}/_ext/761110993/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/761110993/main.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/761110993/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/761110993/error.o: ../casu/error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/761110993 
	@${RM} ${OBJECTDIR}/_ext/761110993/error.o.d 
	@${RM} ${OBJECTDIR}/_ext/761110993/error.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../casu/error.c  -o ${OBJECTDIR}/_ext/761110993/error.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/761110993/error.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/761110993/error.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360924389/fft.o: ../fft/fft.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360924389 
	@${RM} ${OBJECTDIR}/_ext/1360924389/fft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360924389/fft.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../fft/fft.c  -o ${OBJECTDIR}/_ext/1360924389/fft.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360924389/fft.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360924389/fft.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/290349517/digitalIO.o: ../peripheral/gpio/digitalIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/290349517 
	@${RM} ${OBJECTDIR}/_ext/290349517/digitalIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/290349517/digitalIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/gpio/digitalIO.c  -o ${OBJECTDIR}/_ext/290349517/digitalIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/290349517/digitalIO.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/290349517/digitalIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1099012534/i2c2.o: ../peripheral/i2c/i2c2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1099012534 
	@${RM} ${OBJECTDIR}/_ext/1099012534/i2c2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1099012534/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/i2c/i2c2.c  -o ${OBJECTDIR}/_ext/1099012534/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1099012534/i2c2.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1099012534/i2c2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1099012534/i2c1.o: ../peripheral/i2c/i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1099012534 
	@${RM} ${OBJECTDIR}/_ext/1099012534/i2c1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1099012534/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/i2c/i2c1.c  -o ${OBJECTDIR}/_ext/1099012534/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1099012534/i2c1.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1099012534/i2c1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1099024072/spi1.o: ../peripheral/spi/spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1099024072 
	@${RM} ${OBJECTDIR}/_ext/1099024072/spi1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1099024072/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/spi/spi1.c  -o ${OBJECTDIR}/_ext/1099024072/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1099024072/spi1.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1099024072/spi1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1099024072/spi2.o: ../peripheral/spi/spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1099024072 
	@${RM} ${OBJECTDIR}/_ext/1099024072/spi2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1099024072/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/spi/spi2.c  -o ${OBJECTDIR}/_ext/1099024072/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1099024072/spi2.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1099024072/spi2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/399099551/timer2.o: ../peripheral/timer/timer2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/399099551 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer2.o.d 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/timer/timer2.c  -o ${OBJECTDIR}/_ext/399099551/timer2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/399099551/timer2.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/399099551/timer2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/399099551/timer45.o: ../peripheral/timer/timer45.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/399099551 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer45.o.d 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer45.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/timer/timer45.c  -o ${OBJECTDIR}/_ext/399099551/timer45.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/399099551/timer45.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/399099551/timer45.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/399099551/timer1.o: ../peripheral/timer/timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/399099551 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer1.o.d 
	@${RM} ${OBJECTDIR}/_ext/399099551/timer1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/timer/timer1.c  -o ${OBJECTDIR}/_ext/399099551/timer1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/399099551/timer1.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/399099551/timer1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/399099551/timerFunctions.o: ../peripheral/timer/timerFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/399099551 
	@${RM} ${OBJECTDIR}/_ext/399099551/timerFunctions.o.d 
	@${RM} ${OBJECTDIR}/_ext/399099551/timerFunctions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../peripheral/timer/timerFunctions.c  -o ${OBJECTDIR}/_ext/399099551/timerFunctions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/399099551/timerFunctions.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/399099551/timerFunctions.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/383178330/pic2beaglebone.o: ../pic-to-beaglebone/pic2beaglebone.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/383178330 
	@${RM} ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o.d 
	@${RM} ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic-to-beaglebone/pic2beaglebone.c  -o ${OBJECTDIR}/_ext/383178330/pic2beaglebone.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/383178330/pic2beaglebone.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/383178330/pic2beaglebone.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1728059366/adt7320.o: ../sensors/adt7320/adt7320.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1728059366 
	@${RM} ${OBJECTDIR}/_ext/1728059366/adt7320.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728059366/adt7320.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/adt7320/adt7320.c  -o ${OBJECTDIR}/_ext/1728059366/adt7320.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1728059366/adt7320.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1728059366/adt7320.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1728060327/adt7420.o: ../sensors/adt7420/adt7420.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1728060327 
	@${RM} ${OBJECTDIR}/_ext/1728060327/adt7420.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728060327/adt7420.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/adt7420/adt7420.c  -o ${OBJECTDIR}/_ext/1728060327/adt7420.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1728060327/adt7420.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1728060327/adt7420.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733332440/adxl345.o: ../sensors/adxl345/adxl345.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733332440 
	@${RM} ${OBJECTDIR}/_ext/1733332440/adxl345.o.d 
	@${RM} ${OBJECTDIR}/_ext/1733332440/adxl345.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/adxl345/adxl345.c  -o ${OBJECTDIR}/_ext/1733332440/adxl345.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733332440/adxl345.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733332440/adxl345.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o: ../sensors/vcnl4000/proxiVCNL4000.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1035759892 
	@${RM} ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o.d 
	@${RM} ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/vcnl4000/proxiVCNL4000.c  -o ${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1035759892/proxiVCNL4000.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/299869866/proximity.o: ../sensors/proximity.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/299869866 
	@${RM} ${OBJECTDIR}/_ext/299869866/proximity.o.d 
	@${RM} ${OBJECTDIR}/_ext/299869866/proximity.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../sensors/proximity.c  -o ${OBJECTDIR}/_ext/299869866/proximity.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/299869866/proximity.o.d"      -g -omf=elf -mlarge-data -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/299869866/proximity.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/assisi-dspic-fft.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  /opt/microchip/xc16/v1.21/lib/libdsp-elf.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/assisi-dspic-fft.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    /opt/microchip/xc16/v1.21/lib/libdsp-elf.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/assisi-dspic-fft.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  /opt/microchip/xc16/v1.21/lib/libdsp-elf.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/assisi-dspic-fft.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    /opt/microchip/xc16/v1.21/lib/libdsp-elf.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/assisi-dspic-fft.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
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
