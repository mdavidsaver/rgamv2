#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <devSup.h>
#include <recGbl.h>
#include <epicsExport.h>
#include <registryFunction.h>
#include <dbAccess.h>
#include <cantProceed.h>
#include <epicsPrint.h>
#include <epicsTypes.h>
#include <epicsTime.h>

#include <waveformRecord.h>
#include <epicsStdio.h>
#include <dbAccess.h>
#include <dbFldTypes.h>

/*
	Waveform record device support.
	
	Grab data from ai records
	
	Assume ai record name pattern is "PH-TEST-%02d"
*/

#define N	10

struct dbAddr Addr[N];

static long rgaInit(waveformRecord *pwf)
{
	int i;

/* we only accept DOUBLE */

	if (pwf->ftvl != DBR_DOUBLE)
	{
		recGblRecordError(S_db_badDbrtype, (void *) pwf, "devWaveform (init_record) Illegal data type. Must be double.");
		
		return (S_db_badDbrtype);
	}
	
/* check input link */

	switch (pwf->inp.type)
	{
		case (INST_IO):
		{
			break;
		}
		
		default:
		{
			recGblRecordError(S_db_badField, (void *) pwf, "devWaveform (init_record) Illegal INP field");

			return (S_db_badField);
		}
	}

/* locate record and save it's address */
	
	for (i = 0; i < N; i++)
	{
		char name[40];
		
		epicsSnprintf(name, sizeof(name), "PH-TEST-%02d.VAL", i + 1);
	
		if (dbNameToAddr(name, &Addr[i]) != 0)
		{
			printf("rgaInit: Cannot find record %s\n", name);
			
			return (-1);
		}
		else
		{
			printf("%d %p\n", i, (void *) &Addr[i]);
		}
	}
	
	return (0);
}

static long rgaProc(waveformRecord *pwf)
{
	int i;
	epicsFloat64 *ptr = (epicsFloat64 *) pwf->bptr;
	long options = 0, nRequest = 1;
	
	for (i = 0; i < N; i++)
	{
		epicsFloat64 val;
		
	/* see page 219 of 3.14.11 App Dev Guide */

		if (dbGetField(&Addr[i], DBR_DOUBLE, &val, &options, &nRequest, NULL) == 0)
		{
			printf("%d %f\n", i, val); 
			
			ptr[i] = val; 
		}
		
		pwf->nord = 10;
	}
	
	return (0);
}

/*******************************************************************************/

struct DEVWFTS
{
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_wf;
};

struct DEVWFTS devWF = { 5, NULL, NULL, rgaInit, NULL, rgaProc };

epicsExportAddress(dset, devWF);
