from pkg_resources import require
require('dls_serial_sim')
from dls_serial_sim import serial_device

# Representation of a single RGA head.
class RgaHead(object):

    def __init__(self, name):
        self.name = name
        self.filament = 0
        self.imgInterlockStatus = 0
        self.multiplierStatus = 0
        self.controlModeStatus = 0
        self.scanModeStatus = 0
        self.headStatus = 0
        self.filamentStatus = 0

# A simulation of an RGA PC.  The simulation currently only responds
# correctly to the protocol, it does not provide any sensible data.
class RgaPc(serial_device):

    def __init__(self, name='none', numHeads=1, tcpPort=9100, ui=None):
        print "Create RGA controller %s" % name
        serial_device.__init__(self, ui=ui)
        self.name = name
        self.heads = {}
        for i in range(numHeads):
            self.heads[i+1] = RgaHead("%s:%s" % (name, i+1))
        serial_device.Terminator = "\r"
        self.start_ip(tcpPort)

    def createUi(self):
        '''Override to create the user interface for the simulation.'''
        return TerminalWindow()
    
    def rga(self, number):
        result = None
        if number in self.heads:
            result = self.heads[number]
        return result

    def reply(self, command):
        result = None
        printMessage = True
        parts = command.strip().split()
        if len(parts) == 3:
            printMessage = False
            chan = int(parts[0])
            cmd = parts[1]
            param = parts[2]
            if cmd == 'sf' and chan in self.heads:
                if param != '?':
                    self.heads[chan].filament = int(param)
                    printMessage = True
                result = '%s sf %s' % (chan, self.heads[chan].filament)
            elif cmd == 'is' and param == '?' and chan in self.heads: 
                result = '%s is %s' % (chan, self.heads[chan].imgInterlockStatus)
            elif cmd == 'sp' and param == '?' and chan in self.heads:
                # This one is not in the manual
                result = '%s sp 0' % chan
            elif cmd == 'ms' and chan in self.heads: 
                if param != '?':
                    self.heads[chan].multiplierStatus = int(param)
                    printMessage = True
                result = '%s ms %s' % (chan, self.heads[chan].multiplierStatus)
            elif cmd == 'cm' and param == '?' and chan in self.heads: 
                result = '%s cm %s' % (chan, self.heads[chan].controlModeStatus)
            elif cmd == 'ss' and chan in self.heads: 
                if param != '?':
                    self.heads[chan].scanModeStatus = int(param)
                    printMessage = True
                result = '%s ss %s' % (chan, self.heads[chan].scanModeStatus)
            elif cmd == 'tp' and param == '?' and chan in self.heads:
                # This one is not in the manual
                result = '%s tp 0' % chan
            elif cmd == 'hs' and chan in self.heads: 
                if param != '?':
                    self.heads[chan].headStatus = int(param)
                    printMessage = True
                result = '%s hs %s' % (chan, self.heads[chan].headStatus)
            elif cmd == 'fs' and chan in self.heads: 
                if param != '?':
                    self.heads[chan].filamentStatus = int(param)
                    printMessage = True
                result = '%s fs %s' % (chan, self.heads[chan].filamentStatus)
            elif cmd == 'hc' and param == '?': 
                if chan in self.heads:
                    result = '%s hc head%s' % (chan, self.heads[chan].name)
                else:
                    result = '%s hc 0' % chan
            elif cmd == 'sd' and param == '?' and chan in self.heads: 
                result = '%s sd 0' % chan
            elif cmd == 'cd' and param == '?' and chan in self.heads: 
                result = '%s cd 0' % chan
            elif cmd == 'ct' and param == '?' and chan in self.heads: 
                result = '%s ct 0' % chan
            else:
                printMessage = True
        if printMessage:
            text = "%s==>%s" % (repr(command), repr(result))
            self.diagnostic(text, 1)
        return result
