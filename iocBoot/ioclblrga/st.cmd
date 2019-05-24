#!../../bin/linux-x86_64/lblrga

#- You may have to change test to something else
#- everywhere it appears in this file

#< envPaths

## Register all support components
dbLoadDatabase("../../dbd/lblrga.dbd",0,0)
lblrga_registerRecordDeviceDriver(pdbbase) 

drvAsynIPPortConfigure("IP", "192.168.3.10:10014", 0, 0, 0)

mv2init("RGA", "IP")

#asynSetTraceMask("RGA",0,0x3f)
#asynSetTraceIOMask("RGA",0,2)

#asynSetTraceMask("IP",0,0xf)
#asynSetTraceIOMask("IP",0,2)

## Load record instances
dbLoadRecords("../../db/rgamv2.template","device=RGA1,PORT=RGA")

iocInit()
