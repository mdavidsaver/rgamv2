import socket


# An MV2 RGA simulation
class RgaMv2(object):

    def __init__(self):
        '''Create an MV2 RGA simulator.'''
        self.name = ''
        self.InTerminator = "\r\n"
        self.OutTerminator = "\r\r"
        self.userApplication = ''
        self.userVersion = ''
        self.barchartName = ''
        self.barchartStartMass = 1
        self.barchartEndMass = 50
        self.barchartFilterMode = 'PeakCenter'
        self.barchartAccuracy = 5
        self.barchartEGainIndex = 0
        self.barchartSourceIndex = 0
        self.barchartDetectorIndex = 0
        self.scanMeasurement = ''

    def reply(self, command):
        '''Handle commands.  Lots still TODO'''
        result = ''
        tokens = command.split()
        if len(tokens) >= 1 and tokens[0] == "Release":
            result = 'Release OK\r\n'
        elif len(tokens) >= 1 and tokens[0] == "Sensors":
            result = 'Sensors OK\r\nState SerialNumber Name\r\nReady LM70-00197021 \"Chamber A\"\r\n'
        elif len(tokens) >= 2 and tokens[0] == "Select":
            result = 'Select OK\r\nSerialNumber LM70-00197021\r\nState Ready\r\n'
        elif len(tokens) >= 1 and tokens[0] == "FilamentInfo":
            result = 'FilamentInfo OK\r\n'\
                'SummaryState OFF\r\n'\
                'ActiveFilament 2\r\n'\
                'ExternalTripEnable No\r\n'\
                'ExternalTripMode Trip\r\n'\
                'EmissionTripEnable Yes\r\n'\
                'MaxOnTime 900\r\n'\
                'OnTimeRemaining 0\r\n'\
                'Trip None\r\n'\
                'Drive Off\r\n'\
                'EmissionTripState OK\r\n'\
                'ExternalTripState OK\r\n'\
                'RVCTripState OK\r\n'
        elif len(tokens) >= 1 and tokens[0] == "MultiplierInfo":
            result = 'MultiplierInfo OK\r\n'\
                'InhibitWhenFilamentOff Yes\r\n'\
                'InhibitWhenRVCHeaterOn Yes\r\n'\
                'MultiplierOn No\r\n'\
                'LockedByFilament Yes\r\n'\
                'LockedByRVC No\r\n'\
                'LockedBySoftware No\r\n'
        elif len(tokens) >= 3 and tokens[0] == "Control":
            self.userApplication = tokens[1]
            self.userVersion = tokens[2]
            result = 'Control OK\r\nSerialNumber LM70-00197021\r\n'
        elif len(tokens) >= 3 and tokens[0] == "AnalogInputInterval":
            result = 'AnalogInputInterval OK\r\n'
        elif len(tokens) >= 3 and tokens[0] == "AnalogInputEnable":
            result = 'AnalogInputEnable OK\r\n'
        elif len(tokens) >= 1 and tokens[0] == "MeasurementRemoveAll":
            result = 'MeasurementRemoveAll OK\r\n'
        elif len(tokens) >= 1 and tokens[0] == "SensorState":
            result = 'SensorState OK\r\n' \
                'State InUse\r\n'\
                'UserApplication %s\r\n'\
                'UserVersion %s\r\n'\
                'UserAddress 127.0.0.1\r\n' % (self.userApplication, self.userVersion)
        elif len(tokens) >= 1 and tokens[0] == "Info":
            result = 'Info OK\r\n' \
                'SerialNumber LM70-00197021\r\n' \
                'Name \"Chamber A\"\r\n' \
                'State Ready\r\n' \
                'UserApplication %s\r\n' \
                'UserVersion %s\r\n' \
                'UserAddress 127.0.0.1\r\n' \
                'ProductID 70 MicroVision+\r\n' \
                'RFConfiguration 0 \"Smart Head\"\r\n' \
                'DetectorType 0 Faraday\r\n' \
                'SEMSupply 3000 3.0kV\r\n' \
                'ExternalHardware 0 None\r\n' \
                'TotalPressureGauge 0 \"Not Fitted\"\r\n' \
                'FilamentType 0 Tungsten\r\n' \
                'ControlUnitUse 4 \"Standard RGA\"\r\n' \
                'SensorType 1 \"Standard Open Source\"\r\n' \
                'InletType 1 None\r\n' \
                'Version V3.70\r\n' \
                'NumEGains 3\r\n' \
                'NumDigitalPorts 2\r\n' \
                'NumAnalogInputs 4\r\n' \
                'NumAnalogOutputs 1\r\n' \
                'NumSourceSettings 6\r\n' \
                'NumInlets 1\r\n' \
                'MaxMass 200\r\n' \
                'ActiveFilament 1\r\n' \
                'FullScaleADCAmps 0.000002\r\n' \
                'FullScaleADCCount 8388608\r\n' \
                'PeakResolution 32\r\n' \
                'ConfigurableIonSource Yes\r\n' \
                'RolloverCompensation No\r\n' % (self.userApplication, self.userVersion)
        elif len(tokens) >= 1 and tokens[0] == 'ScanStop':
            result = 'ScanStop OK\r\n'
        elif len(tokens) >= 9 and tokens[0] == 'AddBarchart':
            self.barchartName = tokens[1]
            self.barchartStartMass = int(tokens[2])
            self.barchartEndMass = int(tokens[3])
            self.barchartFilterMode = tokens[4]
            self.barchartAccuracy = int(tokens[5])
            self.barchartEGainIndex = int(tokens[6])
            self.barchartSourceIndex = int(tokens[7])
            self.barchartDetectorIndex = int(tokens[8])
            result = 'AddBarchart OK\r\n' + \
                ('Name %s\r\n' % self.barchartName) + \
                ('StartMass %s\r\n' % self.barchartStartMass) + \
                ('EndMass %s\r\n' % self.barchartEndMass) + \
                ('FilterMode %s\r\n' % self.barchartFilterMode) + \
                ('Accuracy %s\r\n' % self.barchartAccuracy) + \
                ('EGainIndex %s\r\n' % self.barchartEGainIndex) + \
                ('SourceIndex %s\r\n' % self.barchartSourceIndex) + \
                ('DetectorIndex %s\r\n' % self.barchartDetectorIndex)
        elif len(tokens) >= 2 and tokens[0] == 'ScanAdd':
            self.scanMeasurement = tokens[1]
            result = 'ScanAdd OK\r\nMeasurement %s\r\n' % self.scanMeasurement
        elif len(tokens) >= 2 and tokens[0] == 'ScanStart':
            self.numberOfScans = int(tokens[1])
            result = 'ScanStart OK\r\n'
        elif len(tokens) >= 2 and tokens[0] == 'ScanResume':
            self.numberOfScans = int(tokens[1])
            result = 'ScanResume OK\r\n'
        else:
            print("Unknown command", tokens)
        return result+'\r\n\r\r'

    def process(self):
        '''Perform the background processing'''
        pass

    
def run():
    S = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    S.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    S.bind(('127.0.0.1', 10014))
    S.listen(1)

    sim = RgaMv2()

    while True:
        C, addr = S.accept()
        print("Accept from", addr)

        F = C.makefile('r+b', 0)

        for line in F:
            print(">>>>>>>>>>>>>>>")
            print(line)
            out = sim.reply(line)
            if out:
                print("<<<<<<<<<<<<<<<<<<<<<<")
                print(out)
                F.write(out)
                F.flush()
        print("Closed from", addr)

if __name__=='__main__':
    run()
