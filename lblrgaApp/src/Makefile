TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = lblrga
# lblrga.dbd will be created and installed
DBD += lblrga.dbd

# lblrga.dbd will be made up from these files:
lblrga_DBD += base.dbd
lblrga_DBD += asyn.dbd
lblrga_DBD += drvAsynIPPort.dbd
lblrga_DBD += rgamv2Include.dbd

# Add all the support libraries needed by this IOC
lblrga_LIBS += rgamv2
lblrga_LIBS += asyn

# lblrga_registerRecordDeviceDriver.cpp derives from lblrga.dbd
lblrga_SRCS += lblrga_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
lblrga_SRCS_DEFAULT += lblrgaMain.cpp
lblrga_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#lblrga_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
lblrga_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

