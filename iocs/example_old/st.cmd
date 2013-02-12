###########################################################
# Pete Owens 24/6/04 vxWorks/EPICS startup file, 
# Example to test rga stream device application
# This IOC is configured for :
#       Hytec IP Carrier 8002 card in slot 7 - 
#       Hytec 8515 Octal serial module in slot B
#	RGA on ports 0 & 1
#

rgaTop = "/home/pho76/diamond/rga"
#rgaTop     = "/home/diamond/R3.13.9/work/support/rga/Rx-y"
diamondTop = "/home/diamond/R3.13.9/prod/support/superTop/Rx-y"

###########################################################

cd diamondTop
cd "bin/ppc604" 
ld < iocCore
ld < baseLib   
ld < streamLib
ld < streamTty.o
ld < ipacLib
ld < drvHy8515.o

###########################################################

cd diamondTop
cd "dbd" 
dbLoadDatabase "baseApp.dbd"
dbLoadDatabase "drvIpac.dbd" 
dbLoadDatabase "stream.dbd" 

###########################################################
# Configure a Hytec 8002 carrier VME slot 7
#
#                        vmeslotnum, IPintlevel, HSintnum
IPAC7 = ipacEXTAddCarrier (&EXTHy8002, "7 2 11")

###########################################################
# Hytec 8515 IPOctal serial module in slot B on the IP carrier card. 
#
# Configure module on carrier 7, slot 1
# Params are : 
#	cardnum, 
#	vmeslotnum, 
#	ipslotnum, 
#	vectornum, 
#	intdelay (-ve => FIFO interrupt), 
#	halfduplexmode, 
#	delay845
#
MOD71 = Hy8515Configure (71, IPAC7, 1, 5, -32, 0, 0)

# Create devices
# Params are :
#	name
#	card number
#	port number
#	read buffer size
#	write buffer size
#
PORT710 = tyHYOctalDevCreate("/ty/71/0", MOD71, 0, 2500, 250)
PORT711 = tyHYOctalDevCreate("/ty/71/1", MOD71, 1, 2500, 250)
PORT712 = tyHYOctalDevCreate("/ty/71/2", MOD71, 2, 2500, 250)
PORT713 = tyHYOctalDevCreate("/ty/71/3", MOD71, 3, 2500, 250)
PORT714 = tyHYOctalDevCreate("/ty/71/4", MOD71, 4, 2500, 250)
PORT715 = tyHYOctalDevCreate("/ty/71/5", MOD71, 5, 2500, 250)
PORT716 = tyHYOctalDevCreate("/ty/71/6", MOD71, 6, 2500, 250)
PORT717 = tyHYOctalDevCreate("/ty/71/7", MOD71, 7, 2500, 250)

tyHYOctalConfig (PORT716, 115200, 'N', 1, 8, 'N')
#tyHYOctalConfig (PORT716, 9600, 'N', 1, 8, 'N')
tyHYOctalConfig (PORT717, 9600, 'E', 1, 8, 'N')

###########################################################
# Configure stream device
#
STREAM_PROTOCOL_DIR = malloc (100)
strcpy (STREAM_PROTOCOL_DIR, rgaTop)
strcat (STREAM_PROTOCOL_DIR, "/rgaApp/protocol")

ty_71_6_streamBus = "Tty"
ty_71_7_streamBus = "Tty"

###########################################################
# Load the databses & start the IOC
#
cd rgaTop
cd "example"

dbLoadTemplate "rga.sub"

streamTtyDebug=0

iocInit

###########################################################
