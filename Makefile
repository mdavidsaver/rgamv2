#Makefile at top of application tree
TOP = .

include $(TOP)/configure/CONFIG
DIRS := configure
DIRS := rgamv2App

# requires DLS python bits
#ifeq ($(wildcard etc),etc)
#	include $(TOP)/etc/makeIocs/Makefile.iocs
#	UNINSTALL_DIRS += documentation/doxygen $(IOC_DIRS)
#endif

# Comment out the following line to disable building of example iocs
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard iocs))
include $(TOP)/configure/RULES_TOP
