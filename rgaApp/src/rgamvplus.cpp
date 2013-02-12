#include <string.h>
#include <asynPortDriver.h>
#include "asynOctetSyncIO.h"
#include <iocsh.h>
#include <epicsExport.h>
#include <registryFunction.h>
#include <drvAsynIPPort.h>
#include <epicsThread.h>
#include <epicsTimer.h>
#include <epicsMath.h>
#include "shareLib.h"
#include "epicsAlgorithm.h"

// STL
#include <algorithm>
#include <string>
#include <vector>

//#include "mvpluselllist.cpp"

// using namespace std;

#define NUM_PARAMS 272
#define NUM_MASSES 200
#define NUM_PJ 12
#define NUM_ANA_CHANNELS 6400
#define MAX_RGA_CHANS 8

double PaToMbar(double p)
{
    return 0.01*p;
}

class mvplus : public asynPortDriver
	{
    int P_BARM[NUM_MASSES];
    int P_PJMASS[NUM_PJ];
    int P_PJLABEL[NUM_PJ];
    int P_PJBIN[NUM_PJ];
	int P_ANA_ASK;
	int P_ANA;
	int P_SCANPERIOD;
	int P_PJSCANPERIOD;
	int P_BAR_ASK;
	int P_BAR;
	int P_SUMP_ASK;
	int P_SUMP;
	int P_TOTP_ASK;
	int P_TOTP;
	int P_DATA_ASK;
	int P_DATA;
	int P_CON;
	int P_HEAD;
	int P_HEADSTA_ASK;
	int P_HEADSTA;
	int P_SETSCAN;
	int P_SCAN_ASK;
	int P_SCAN;
	int P_FIL_ASK;
	int P_FIL;
	int P_SETFIL;
	int P_FILCON;
	int P_FILSTA_ASK;
	int P_FILSTA;
	int P_MLTCON;
	int P_MLTSTA_ASK;
	int P_MLTSTA;
	int P_DET_ASK;
	int P_DET;
	int P_ILK_ASK;
	int P_ILK;
	int P_MODE_ASK;
	int P_MODE;
	int P_SETMODE;
	int P_MASSMAP;

	public:
		mvplus(char * name, char * port);
		~mvplus();

		virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
		virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);

		virtual asynStatus 	readOctet (asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
		virtual asynStatus 	writeOctet (asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);

	    asynStatus connect();
	    void mainRun();
	    void HandleTimer();

	private:
	    asynStatus sendCommand(std::string data, double timeout);
		void processReceived();
        void RequestBarchartData(unsigned int);
        void RequestPeakjumpData(unsigned int);

		enum HeadState
		{
			STOP,
			LOCAL_CONTROL,
			DEGAS_FILAMENT,
			CAL_FARADAY,
			CAL_MULTIPLIER,
			BARCHART_50,
			BARCHART_100,
			BARCHART_200,
			ANALOG_200,
			NO_RGA,
			PEAK_JUMP,
		};

		enum ScanMode
		{
			SINGLE,
			CONTINUOUS
		};

		enum FilamentControl
		{
			OFF,
			ON
		};

		enum FilamentState
		{
			FS_OFF,
			FS_ON,
			FS_WARM_UP,
			FS_COOL_DOWN
		};

		enum CommandState
			{
			CMD_IDLE,
			CMD_BUSY,
			//CMD_COMPLETE
			};


		void HandleMassReading(unsigned mass, double reading);

		std::string makeAddBarChartCommand();
		bool isBarchart(HeadState sta);
		std::string getParameterName(int reason);


		const static int numFilaments_ = 2;
		const static int numDetectorIndexes_ = 4;
		std::string port_;
		asynUser* serialPortUser;

		HeadState headSta_[MAX_RGA_CHANS];

		// Bar chart and peak jump data request
		// timer counters.
	    unsigned scheduleGetInfoCounter_[MAX_RGA_CHANS];
	    unsigned scheduleGetInfoCounterPJ_[MAX_RGA_CHANS];

		// Simple analogues for old StreamDevice compatibility
		// vi prefix indicates virtual interface, as these member variables
		// will be updated asynchronously via the processReceived() thread.
		std::string viHead_[MAX_RGA_CHANS];
		unsigned	scan_period_[MAX_RGA_CHANS];
		unsigned	pjscan_period_[MAX_RGA_CHANS];
		epicsFloat64 ana_waveform_[MAX_RGA_CHANS][NUM_ANA_CHANNELS];

		class MainThread: public epicsThreadRunable
			{
			private:
				mvplus* owner;
				epicsThread thread;

			public:
				MainThread(mvplus* owner);
				virtual ~MainThread() {}
				virtual void run();
			};

		MainThread * mainThread_;


	    class EventTimer : public epicsTimerNotify
			{
			public:
				EventTimer(mvplus * owner, epicsTimerQueueActive &queue)
				: owner_(owner), timer_(queue.createTimer()), delay_(1.0) {}
				virtual expireStatus expire(const epicsTime & currentTime)
					{
					owner_->HandleTimer();
					return expireStatus(restart, delay_);
					}
				void start(double delay)
					{
					delay_ = delay;
					timer_.start(*this, delay);
					}
				~EventTimer() { timer_.destroy(); }

			private:
				mvplus * owner_;
				epicsTimer &timer_;
				double delay_;
			};

	    epicsTimerQueueActive * timerQueue_;
	    EventTimer * eventTimer_;
	};


//ASYN_CANBLOCK,  /* blocking   */
mvplus::mvplus(char * name, char *ser_port)
 :  asynPortDriver(name,
        9,            /* maxAddr -> 8 RGA channels per controller*/
        NUM_PARAMS,     /* max parameters */
        asynDrvUserMask |
        asynFloat64Mask | 
        asynFloat64ArrayMask |
        asynInt32Mask   |
        asynOctetMask,  /* interface mask*/
        asynFloat64Mask |
        asynFloat64ArrayMask |
        asynInt32Mask   |
        asynOctetMask,              /* interrupt mask */
        ASYN_CANBLOCK|ASYN_MULTIDEVICE,  /* blocking and multiple RGA heads per RGA PC (via ProcessEye) */
        1,              /* autoconnect */
        0,              /* default priority */
        0               /* default stack size */
    ),
    port_(ser_port),
    serialPortUser(NULL)
	{
	for (int rgaChan = 0; rgaChan < MAX_RGA_CHANS; rgaChan++)
		{
		scheduleGetInfoCounter_[rgaChan] = 0;
		scheduleGetInfoCounterPJ_[rgaChan] = 0;
		headSta_[rgaChan] = STOP;
	    viHead_[rgaChan] ="";
		scan_period_[rgaChan] = 10;
		pjscan_period_[rgaChan] = 5;

		// Initialise the analogue waveform
	    for (int i = 0; i < NUM_ANA_CHANNELS; i++)
			{
	    	ana_waveform_[rgaChan][i] = 0.0;
			}

		}
    // Uncomment this line to enable asyn trace flow and error
    //pasynTrace->setTraceMask(pasynUserSelf, 0xFF);

    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "Constructor\n");

	for (int i = 0; i < NUM_MASSES; ++i)
    	{
        char name[40];
        sprintf(name, "BARM%d", i+1);
        createParam(name, asynParamFloat64, &P_BARM[i]);
    	}

    for (int i = 0; i < NUM_PJ; ++i)
    	{
        char name[40];
        sprintf(name, "PJBIN%d", i+1);
        createParam(name, asynParamFloat64, &P_PJBIN[i]);
    	}
    for (int i = 0; i < NUM_PJ; ++i)
    	{
        char name[40];
        sprintf(name, "PJMASS%d", i+1);
        createParam(name, asynParamInt32, &P_PJMASS[i]);
    	}

    for (int i = 0; i < NUM_PJ; ++i)
    	{
        char name[40];
        sprintf(name, "PJLABEL%d", i+1);
        createParam(name, asynParamInt32, &P_PJLABEL[i]);
    	}

    createParam("ANA_ASK"     , asynParamInt32,        &P_ANA_ASK);
    createParam("ANA"         , asynParamFloat64Array, &P_ANA);
    createParam("SCANPERIOD"  , asynParamInt32,        &P_SCANPERIOD);
    createParam("PJSCANPERIOD", asynParamInt32,        &P_PJSCANPERIOD);
    createParam("BAR_ASK"     , asynParamInt32,        &P_BAR_ASK);
    createParam("BAR"         , asynParamFloat64,      &P_BAR);
    createParam("SUMP_ASK"    , asynParamInt32,        &P_SUMP_ASK);
    createParam("SUMP"        , asynParamFloat64,      &P_SUMP);
    createParam("TOTP_ASK"    , asynParamInt32,        &P_TOTP_ASK);
    createParam("TOTP"        , asynParamFloat64,      &P_TOTP);
    createParam("DATA_ASK"    , asynParamInt32,        &P_DATA_ASK);
    createParam("DATA"        , asynParamInt32,        &P_DATA);
    createParam("CON"         , asynParamInt32,        &P_CON);
    createParam("HEAD"        , asynParamOctet,        &P_HEAD);
    createParam("HEADSTA_ASK" , asynParamInt32,        &P_HEADSTA_ASK);
    createParam("HEADSTA"     , asynParamInt32,        &P_HEADSTA);
    createParam("SETSCAN"     , asynParamInt32,        &P_SETSCAN);
    createParam("SCAN_ASK"    , asynParamInt32,        &P_SCAN_ASK);
    createParam("SCAN"        , asynParamInt32,        &P_SCAN);
    createParam("FIL_ASK"     , asynParamInt32,        &P_FIL_ASK);
    createParam("FIL"         , asynParamInt32,        &P_FIL);
    createParam("SETFIL"      , asynParamInt32,        &P_SETFIL);
    createParam("FILCON"      , asynParamInt32,        &P_FILCON);
    createParam("FILSTA_ASK"  , asynParamInt32,        &P_FILSTA_ASK);
    createParam("FILSTA"      , asynParamInt32,        &P_FILSTA);
    createParam("MLTCON"      , asynParamInt32,        &P_MLTCON);
    createParam("MLTSTA_ASK"  , asynParamInt32,        &P_MLTSTA_ASK);
    createParam("MLTSTA"      , asynParamInt32,        &P_MLTSTA);
    createParam("DET_ASK"     , asynParamInt32,        &P_DET_ASK);
    createParam("DET"         , asynParamInt32,        &P_DET);
    createParam("ILK_ASK"     , asynParamInt32,        &P_ILK_ASK);
    createParam("ILK"         , asynParamInt32,        &P_ILK);
    createParam("MODE_ASK"    , asynParamInt32,        &P_MODE_ASK);
    createParam("MODE"        , asynParamInt32,        &P_MODE);
    createParam("SETMODE"     , asynParamInt32,        &P_SETMODE);
    createParam("MASSMAP"     , asynParamOctet,        &P_MASSMAP);

    // Number of parameters = 200 + 12+ 12 + 12 + 36 = 272

    connect();

    /* Start the thread */
    mainThread_ = new MainThread(this);
    timerQueue_ = &epicsTimerQueueActive::allocate(true);
    eventTimer_ = new EventTimer(this, *timerQueue_);
    eventTimer_->start(1.0);
	}

mvplus::~mvplus()
	{
    delete eventTimer_;
    delete mainThread_;
	}

mvplus::MainThread::MainThread(mvplus* owner)
    : owner(owner),
      thread(*this, "stateMachine", epicsThreadGetStackSize(epicsThreadStackMedium))
	{
    thread.start();
	}

void mvplus::MainThread::run()
	{
    owner->mainRun();
	}


void mvplus::mainRun()
	{
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "mvplus::mainRun() Starting processReceived thread\n");
    while (true)
    	{
        processReceived();
        //ProcessTask();
        //stateMachine();
    	}
	}


asynStatus mvplus::connect()
	{
    int serialPortAddress = 0;
    asynStatus status = pasynOctetSyncIO->connect(port_.c_str(), serialPortAddress, &serialPortUser, NULL);

    if(status == asynSuccess)
    	{
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,
            "Connected to serial port=%s (%d) %p\n",
                port_.c_str(), status, serialPortUser);
        pasynOctetSyncIO->setInputEos(serialPortUser, "\r", 1);
        pasynOctetSyncIO->setOutputEos(serialPortUser, "\r", 1);
    	}
    else
    	{
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
            "Failed to connect to serial port=%s error=%d\n", port_.c_str(), status);
    	}

    return status;
	}


bool mvplus::isBarchart(HeadState sta)
{
    bool isBar = false;
    switch (sta)
    {
    case BARCHART_50:
    case BARCHART_100:
    case BARCHART_200:
        isBar = true;
        break;

    default:
        break;
    }
    return isBar;
}

void mvplus::HandleTimer()
	{
    asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "Timer handler\n");

    for (int iChan = 0; iChan < MAX_RGA_CHANS; iChan++)
    	{
        ++scheduleGetInfoCounter_[iChan];
        ++scheduleGetInfoCounterPJ_[iChan];

		// Has time elapsed for next barchart data request?
		if (++scheduleGetInfoCounter_[iChan] > scan_period_[iChan])
			{
			++scheduleGetInfoCounter_[iChan] = 0;
			RequestBarchartData(iChan);
			}

		// Has time elapsed for next peak jump data request?
		if (++scheduleGetInfoCounterPJ_[iChan] > pjscan_period_[iChan])
			{
			++scheduleGetInfoCounterPJ_[iChan] = 0;
			RequestPeakjumpData(iChan);
			}
    	}
	}

void mvplus::RequestBarchartData(unsigned int rga_channel)
	{
	int statusData = 0;
	getIntegerParam(rga_channel, P_HEADSTA, &statusData);
	int i;
	char buffer[16];
	// statusData 5 => 1 - 50, 6 => 1 - 100, 7 => 1 - 200
	switch (statusData)
		{
		case 5: // 1 - 50
			for (i = 1; i < 51; i++)
				{
				sprintf(buffer, "%u rb %u", rga_channel, i);
				sendCommand(std::string(buffer), 1.0);
				}
			break;
		case 6: // 1 - 100
			for (i = 1; i < 101; i++)
				{
				sprintf(buffer, "%u rb %u", rga_channel, i);
				sendCommand(std::string(buffer), 1.0);
				}
			break;
		case 7: // 1 - 200
			for (i = 1; i < 201; i++)
				{
				sprintf(buffer, "%u rb %u", rga_channel, i);
				sendCommand(std::string(buffer), 1.0);
				}
			break;
		default:
			break;
		}
	}

void mvplus::RequestPeakjumpData(unsigned int rga_channel)
	{
	int statusData = 0;
	getIntegerParam(rga_channel, P_DATA, &statusData);
	int i;
	int iMass = 0;
	char buffer[16];
	// statusData 5 => Peak Jump
	// Wait until the RGA has Peak Jump data.
	if (statusData == 5)
		{
		// Get the parameter values defining the mass for each of the bins
		// then send a command to request the pressure at that mass.
		for (i = 0; i < NUM_PJ; i++)
			{
	        getIntegerParam(rga_channel, P_PJMASS[i], &iMass);
	        if ((iMass > 0) && (iMass < NUM_MASSES))
				{
				sprintf(buffer, "%u rp %u",rga_channel, iMass);
				sendCommand(std::string(buffer), 1.0);
				}
			}
		}
	}

void mvplus::processReceived()
	{
    	{
    	if (serialPortUser != 0)
        	{
            pasynOctetSyncIO->setInputEos(serialPortUser, "\r", 1);

            //asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "mvplus::processReceived() - called\n");

            char 		buffer[1600];
            buffer[sizeof(buffer)-1] = 0;
            unsigned 	int channel = 0;
            int 		eomReason;
            size_t 		nRead;
            asynStatus status = pasynOctetSyncIO->read(serialPortUser, buffer, sizeof(buffer)-1, 5.0, &nRead, &eomReason);
            if(status == asynSuccess)
            	{
                //asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "mvplus::processReceived(): Read ok, %d bytes\n", nRead);
                buffer[nRead] = 0;
                asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW,buffer);
                char headerBuffer[100];
                int npars = sscanf(buffer, "%u %s", &channel, headerBuffer);
                if ((npars > 1) && (channel < MAX_RGA_CHANS))
					{
					std::string event(headerBuffer);
					asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "mvplus::processReceived(): ASYN_EVENT: %s\n", event.c_str());
					if (event.find("hc") != std::string::npos)
						{
						char statusBuffer[100];
						npars = sscanf(buffer, "%*s hc %s", statusBuffer);
						if (npars > 0)
							{
							viHead_[channel] = statusBuffer;
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("hs") != std::string::npos)
						{
						int viSta = 0;
						npars = sscanf(buffer, "%*s %*s %d", &viSta);
						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_HEADSTA, viSta);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("rb") != std::string::npos)
						{
						double fPressure = 0.0;
						int iMass = 0;
						npars = sscanf(buffer, "%*s %*s %d %lf", &iMass, &fPressure);
						if ((npars > 1) && (iMass < NUM_MASSES) && (iMass > 0))
							{
							lock();
							setDoubleParam(channel, P_BARM[iMass], fPressure);
							unlock();
							if (fPressure > 0.01)
								{
								asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "mvplus::processReceived(): rb handler: buffer: %s  rga chan: %d Mass = %d   Pressure = %1.2g\n", buffer, channel, iMass, fPressure);
								}
							asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "mvplus::processReceived(): rb handler: buffer: %s  rga chan: %d Mass = %d   Pressure = %1.2g\n", buffer, channel, iMass, fPressure);
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("rp") != std::string::npos)
						{
						// Peak Jump data:
						//
						double fPressure = 0.0;
						int iReportedMass = 0;
						int iMass = 0;
						npars = sscanf(buffer, "%*s %*s %d %lf", &iReportedMass, &fPressure);
						asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "mvplus::processReceived(): rp handler: buffer: %s   Mass = %d   Pressure = %1.2g\n", buffer, iReportedMass, fPressure);
						// Check that the reported mass is in the nominal range
						if ((npars > 1) && (iReportedMass < NUM_MASSES) && (iReportedMass > 0))
							{
							// Find the appropriate PJBIN for this mass
							for (int i = 0; i < NUM_PJ; i++)
								{
						        getIntegerParam(channel, P_PJMASS[i], &iMass);
						        if (iMass == iReportedMass)
									{
						        	lock();
									setDoubleParam(channel, P_PJBIN[i], fPressure);
									unlock();
									lock();
									status = (asynStatus) callParamCallbacks();
									unlock();
									break; // got it
									}
								}
							}
						}
					else if (event.find("ra") != std::string::npos)
						{
						double fPressure = 0.0;
						int iChannel = 0;
						// Check the return format, maybe list of pressures delimited by ';'
						npars = sscanf(buffer, "%*s %*s %d %lf", &iChannel, &fPressure);
						asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "*!*!*!* mvplus::processReceived(): ra handler: buffer: %s   Channel = %d   Pressure = %1.2g\n", buffer, iChannel, fPressure);
						if ((npars > 1) && (iChannel < NUM_ANA_CHANNELS) && (iChannel >= 0))
							{
							ana_waveform_[channel][iChannel] = fPressure;

							// Only print diagnostic for every hundredth channel
							double hundreds = (iChannel / 100.0);
							if ((hundreds - int(hundreds)) < 0.001)
								{
								asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "mvplus::processReceived(): ra handler: buffer: %s   Channel = %d   Pressure = %1.2g\n", buffer, iChannel, fPressure);
								}
							lock();
							doCallbacksFloat64Array (ana_waveform_[channel], NUM_ANA_CHANNELS, P_ANA, channel);
							unlock();
							status = (asynStatus) callParamCallbacks();
							}

						// Now clear the request scan flag
						lock();
						setIntegerParam(channel, P_ANA_ASK, 0);
						status = (asynStatus) callParamCallbacks();
						unlock();
						}
					else if (event.find("sf") != std::string::npos)
						{
						int iVal = 0;
						npars = sscanf(buffer, "%*s %*s %d", &iVal);
						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_FIL, iVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("fs") != std::string::npos)
						{
						int iVal = 0;
						npars = sscanf(buffer, "%*s %*s %d", &iVal);
						//asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "mvplus::processReceived(): fs handler: buffer: %s  npars = %d  iVal = %d\n", buffer, npars, iVal);

						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_FILSTA, iVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							status = (asynStatus) callParamCallbacks();
							}
						}
					else if (event.find("is") != std::string::npos)
						{
						int iVal = 0;
						npars = sscanf(buffer, "%*s %*s %d", &iVal);
						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_ILK, iVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("ms") != std::string::npos)
						{
						int iVal = 0;
						npars = sscanf(buffer, "%*s %*s %d", &iVal);
						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_MLTSTA, iVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("ss") != std::string::npos)
						{
						int iVal = 0;
						npars = sscanf(buffer, "%*s %*s %d", &iVal);
						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_SCAN, iVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("cm") != std::string::npos)
						{
						int iVal = 0;
						npars = sscanf(buffer, "%*s %*s %d", &iVal);
						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_MODE, iVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("cd") != std::string::npos)
						{
						int iVal = 0;
						npars = sscanf(buffer, "%*s %*s %d", &iVal);
						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_DET, iVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("sd") != std::string::npos)
						{
						int iVal = 0;
						npars = sscanf(buffer, "%*s %*s %d", &iVal);
						if (npars > 0)
							{
							lock();
							setIntegerParam(channel, P_DATA, iVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("tp") != std::string::npos)
						{
						double dVal = 0.0;
						npars = sscanf(buffer, "%*s %*s %lf", &dVal);
						if (npars > 0)
							{
							lock();
							setDoubleParam(channel, P_TOTP, dVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					else if (event.find("sp") != std::string::npos)
						{
						double dVal = 0.0;
						npars = sscanf(buffer, "%*s %*s %lf", &dVal);
						if (npars > 0)
							{
							lock();
							setDoubleParam(channel, P_SUMP, dVal);
							unlock();
							lock();
							status = (asynStatus) callParamCallbacks();
							unlock();
							}
						}
					}
            	}
        	}
    	}
	}


asynStatus mvplus::sendCommand(const std::string data, double timeout)
	{
    size_t  nWrite = 0;
	std::string cmd;
    cmd = data;
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "Send cmd: %s\n", cmd.c_str());

    asynStatus status = pasynOctetSyncIO->write(serialPortUser, cmd.c_str(), cmd.length(), timeout, &nWrite);
    if (status == asynSuccess)
    	{
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "Write ok, %d bytes\n", nWrite);
    	}
    else
    	{
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,"Failed to write error=%d\n", status);
        epicsThreadSleep(0.5);
    	}

    return status;
	}


asynStatus mvplus::writeInt32(asynUser *pasynUser, epicsInt32 value)
	{
		asynStatus status = asynSuccess;
		int reason = pasynUser->reason;
		//asynPrint(pasynUser, ASYN_TRACE_FLOW, "mvplus::writeInt32 (%d) (%d)\n", reason, value);
		int channel = 0;
		this->getAddress(pasynUser, &channel); // Get the channel number
		asynPortDriver::writeInt32(pasynUser, value);

		if (reason == P_CON)
			{
			//asynPrint(pasynUser, ASYN_TRACE_FLOW, "CON\n");
			switch (value)
				{
				case STOP:
				case LOCAL_CONTROL:
				case BARCHART_50:
				case BARCHART_100:
				case BARCHART_200:
				case DEGAS_FILAMENT:
				case CAL_FARADAY:
				case CAL_MULTIPLIER:
				case ANALOG_200:
				case PEAK_JUMP:
					char buffer[50];
					sprintf(buffer, "%u hs %u", channel, value);
					sendCommand(std::string(buffer), 1.0);
					break;

				case NO_RGA:
					break;

				default:
					break;
				}
			lock();
			setIntegerParam(channel, P_CON, value);
			unlock();
			}
		else if (reason == P_SCANPERIOD)
			{
			lock();
			setIntegerParam(channel, P_SCANPERIOD, value);
			unlock();
			scan_period_[channel] = value;
			}
		else if (reason == P_PJSCANPERIOD)
			{
			lock();
			setIntegerParam(channel, P_PJSCANPERIOD, value);
			unlock();
			pjscan_period_[channel] = value;
			}
		else if (reason == P_SETSCAN)
			{
			//asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "SETSCAN\n");
			char buffer[50];
			sprintf(buffer, "%u ss %u", channel, value);
			sendCommand(std::string(buffer), 1.0);
			lock();
			setIntegerParam(channel, P_SETSCAN, value);
			unlock();
			}
		else if (reason == P_FILCON)
			{
			//asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "FILCON");
			// quick hack to force filament control change
			char buffer[50];
			sprintf(buffer, "%u fs %u", channel, value);
			sendCommand(std::string(buffer), 1.0);
			lock();
			setIntegerParam(channel, P_FILCON, value);
			unlock();
			}
		else if (reason == P_MLTCON)
			{
			//asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "MLTCON");
			if ((value >= 0) && (value < numDetectorIndexes_))
				{
				char buffer[50];
				sprintf(buffer, "%u ms %u", channel, value);
				sendCommand(std::string(buffer), 1.0);
				lock();
				setIntegerParam(channel, P_MLTCON, value);
				unlock();
				}
			}
		else if (reason == P_SETFIL)
			{
			//asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "SETFIL");
			if ((value >= 0) && (value < numFilaments_))
				{
				char buffer[50];
				sprintf(buffer, "%u sf %u", channel, value);
				sendCommand(std::string(buffer), 1.0);
				lock();
				setIntegerParam(channel, P_SETFIL, value);
				unlock();
				}
			}
		else if (reason == P_SETMODE)
			{
			//asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "SETMODE - TODO");
			// TODO - Find out what needs to go here
			// as far as can tell this doesn't do anything in MVPlus.
			char buffer[50];
			sprintf(buffer, "%u cm %u", channel, value);
			sendCommand(std::string(buffer), 1.0);
			lock();
			setIntegerParam(channel, P_SETMODE, value);
			unlock();
			}
		else if (reason == P_SUMP_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u sp ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_HEADSTA_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u hs ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_FIL_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u sf ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_FILSTA_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u fs ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_SCAN_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u ss ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_ANA_ASK)
			{
	        //sendCommand(std::string("1 ra 0 6400"), 1.0);
			if (value > 0)
				{
				int statusData = 0;
				getIntegerParam(channel, P_HEADSTA, &statusData);
				if (statusData == 8)
					{
					lock();
					setIntegerParam(channel, P_ANA_ASK, value);
					unlock();
					asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "mvplus::writeInt32(): P_ANA_ASK: value = %d\n", value);
					// For now ask for 32 bins around mass 2
					//sendCommand(std::string("1 ra 2 0"), 1.0);
					char buffer[50];
					sprintf(buffer, "%u ra 2", channel);
					sendCommand(std::string(buffer), 1.0);
					}
				}
			else
				{
				asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "mvplus::writeInt32(): P_ANA_ASK: value = %d\n", value);
				lock();
				setIntegerParam(channel, P_ANA_ASK, value);
				unlock();
				}
			}
		else if (reason == P_DATA_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u sd ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_TOTP_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u tp ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_ILK_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u is ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_MLTSTA_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u ms ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_MODE_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u cm ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
		else if (reason == P_DET_ASK)
			{
			char buffer[50];
			sprintf(buffer, "%u cd ?", channel);
			sendCommand(std::string(buffer), 1.0);
			}
//		else if (reason == P_BAR_ASK)
//			{
//	        sendCommand(std::string("1 rb ?"), 1.0);
//			}
		else if ((reason >= P_PJMASS[0]) && (reason <= P_PJMASS[NUM_PJ-1]))
			{
			int index = reason - P_PJMASS[0];
			if ((value > 0) && (value <= 200) && (index < NUM_PJ))
				{
				lock();
				setIntegerParam(channel, P_PJMASS[index], value);
				unlock();
		        asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "P_PJMASS[%d]=%d mass = %d\n", index, P_PJMASS[index], value);
				}
			}
		else
			{
	    	std::string par = getParameterName(reason);
			asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "UNKNOWN(%d) -> %s\n", reason, par.c_str());
			}

		/* Do callbacks so higher layers see any changes */
		lock();
		status = (asynStatus) callParamCallbacks();
		unlock();

		return asynSuccess;
	}



asynStatus mvplus::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
	{
	asynStatus status = asynSuccess;
    int reason = pasynUser->reason;
    int channel = 0;
	this->getAddress(pasynUser, &channel); // Get the channel number
    //asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "mvplus::readFloat64 (%d)\n", reason);
    epicsFloat64 rvalue   = 0.0;

    // try and match to a BAR mass reading first. Assume BARM parameters contiguous.
    int barIndex = (reason - P_BARM[0]);
    bool isBar = ((barIndex >= 0 && barIndex < NUM_MASSES) && (reason == P_BARM[barIndex]));

    if (isBar)
    	{
        getDoubleParam(channel, P_BARM[barIndex], &rvalue);
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "P_BARM[%d]=%d value = %1.3g\n", barIndex, P_BARM[barIndex], rvalue);
    	}
    else if (reason == P_SUMP)
    	{
        getDoubleParam(channel, P_SUMP, &rvalue);
    	}
    else if (reason == P_TOTP)
    	{
        getDoubleParam(channel, P_TOTP, &rvalue);
    	}
    else
    	{
    	std::string par = getParameterName(reason);
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "UNKNOWN %s\n", par.c_str());
    	}
    
    *value = rvalue;

    return asynSuccess;
	}


asynStatus 	mvplus::readOctet (asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
	{
	asynStatus status = asynSuccess;
	int channel = 0;
	this->getAddress(pasynUser, &channel); // Get the channel number

	int reason = pasynUser->reason;
    //asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "mvplus::readOctet (%d)\n", reason);
    value = "";
    *nActual = 0;
    if (reason == P_HEAD)
    	{
        //asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "P_HEAD");
        strcpy(value, viHead_[channel].c_str());
        *nActual = strlen(value);
    	}
    else
    	{
        asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "UNKOWN\n");
    	}

    return status;
	}

asynStatus 	mvplus::writeOctet (asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual)
	{
	asynStatus status = asynSuccess;
	int channel = 0;
	this->getAddress(pasynUser, &channel); // Get the channel number
    int reason = pasynUser->reason;
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "mvplus::writeOctet (%d) %s\n", reason, value);
    //asynPortDriver::writeOctet(pasynUser, value, maxChars, nActual);


	if ((reason >= P_PJLABEL[0]) && (reason <= P_PJLABEL[NUM_PJ-1]))
		{
		int index = reason - P_PJLABEL[0];
        asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "(1)P_PJLABEL[%d]=%d label = %s\n", index, P_PJLABEL[index], value);
		if (index < NUM_PJ)
			{
			lock();
			setStringParam(channel, P_PJLABEL[index], value);
			unlock();
	        asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "(2)P_PJLABEL[%d]=%d label = %s\n", index, P_PJLABEL[index], value);
			std::string label(value);
			*nActual = label.length();
			}
		}
    else if (reason == P_HEAD)
    	{
		//asynPrint(pasynUser, ASYN_TRACE_FLOW, "HEAD\n");
		std::string cmd(value);

		status = sendCommand(cmd, 1.0);

		if (status == asynSuccess)
			{
			*nActual = cmd.length();
			}
    	}
    else
		{
		}
	lock();
    status = (asynStatus) callParamCallbacks();
    unlock();

    return asynSuccess;
	}

std::string mvplus::getParameterName(int reason)
	{
    const char *paramName;
    if (getParamName(reason, &paramName) == asynSuccess)
    	{
        return paramName;
    	}
    return "Unkown";
	}


extern "C" void MVPlusInit(char *pszDevice, char *pszPort)
	{
	printf("MVPlusInit() %s %s\n", pszDevice, pszPort);
	new mvplus(pszDevice, pszPort);
	}
extern "C" void MVPlusConfig(char *pszDevice, char *pszPort)
	{
	printf("MVPlusConfig() %s %s\n", pszDevice, pszPort);
	new mvplus(pszDevice, pszPort);
	}



extern "C"
{
static const iocshArg InitArg[] = { { "name", iocshArgString } , { "port", iocshArgString }};
static const iocshArg * const InitArgs[] = { &InitArg[0], &InitArg[1] };
static const iocshFuncDef MVPlusInitFuncDef = {"MVPlusInit", 2, InitArgs};
}

static void InitCallFunc(const iocshArgBuf * args)
{
	printf("InitCallFunc %s %s\n", args[0].sval, args[1].sval);
	new mvplus(args[0].sval, args[1].sval);
}

extern "C" void mvplusAsynRegister(void)
    {
        iocshRegister(&MVPlusInitFuncDef,InitCallFunc);
    }

extern "C"
{
    epicsExportRegistrar(mvplusAsynRegister);

}

class ConfigRegister
	{
	public:
		ConfigRegister() { iocshRegister(&MVPlusInitFuncDef,InitCallFunc); }
	};
static ConfigRegister configRegisterObj;





//=========================
/* EPICS iocsh shell commands */
static const iocshArg Arg0 = { "RGA Head name",iocshArgString};
static const iocshArg Arg1 = { "Asyn Port",iocshArgString};
static const iocshArg * const MVPlusConfigArgs[] = {&Arg0, &Arg1};

static const iocshFuncDef MVPlusConfigFuncDef = {"MVPlusConfig",2,MVPlusConfigArgs};

static void MVPlusConfigCallFunc(const iocshArgBuf *args)
{
	printf("InitCallFunc %s %s\n", args[0].sval, args[1].sval);
	new mvplus(args[0].sval, args[1].sval);
}

extern "C" void MVPlusRegister(void)
{
iocshRegister(&MVPlusConfigFuncDef,MVPlusConfigCallFunc);
}

extern "C"
{
epicsExportRegistrar(MVPlusRegister);
}


