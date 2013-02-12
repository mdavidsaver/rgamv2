###########################################################
# Pete Owens 24/6/04 vxWorks/EPICS startup file, 
# Example to test simulated RGA stream device application
#

rgaTop = "/home/pho76/diamond/rga"
#rgaTop     = "/home/diamond/R3.13.9/work/support/rga/Rx-y"
diamondTop = "/home/diamond/R3.13.9/prod/support/superTop/Rx-y"

###########################################################

cd diamondTop
cd "bin/ppc604" 
ld < iocCore
ld < baseLib   
ld < pressArrLib

###########################################################

cd diamondTop
cd "dbd" 
dbLoadDatabase "baseApp.dbd"

###########################################################
# Load the databses & start the IOC
#
cd rgaTop
cd "example"

dbLoadTemplate "simulation_rga.substitutions"

iocInit

###########################################################
