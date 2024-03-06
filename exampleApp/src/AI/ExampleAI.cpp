/*
 * @Author HuangLi
 * @Email lihuang@ihep.ac.cn
 * @Date 3/5/24
 * @Version 1.0.0
 */

#include <stddef.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include "limits"

#include "recGbl.h"
#include "dbAccess.h"

#include "recSup.h"
#include "devSup.h"
#include "aiRecord.h"
#include "epicsExport.h"
#include <iostream>

using namespace std;

static long init(int *);

static long initRecord(aiRecord *pv);

static long readAI(aiRecord *pv);

struct
{
    long number;
    DEVSUPFUN report;
    long (*init)(int *);
    long (*initRecord)(aiRecord *);
    DEVSUPFUN getIOIntInfo;
    long (*readAI)(aiRecord *);
    DEVSUPFUN specialLinConv;
} EXAMPLE_AI = {
    6,
    NULL,
    init,
    initRecord,
    NULL,
    readAI,
    NULL};

epicsExportAddress(dset, EXAMPLE_AI);

static long init(int *after)
{
    std::cout << "init" << std::endl;
    return 0;
}

static long initRecord(aiRecord *pv)
{
    std::cout << "initRecord" << std::endl;
    pv->udf = FALSE;
    return 0;
}

static long readAI(aiRecord *pv)
{
    if (pv->pact)
    {
        epicsFloat64 val = 0;
        srand(time(nullptr));
        int min = 1;
        int max = 100;
        val = min + (max - min) * (rand() / (double)RAND_MAX);
        std::cout << "val: " << val << std::endl;
        pv->val = val;
        pv->udf = FALSE;
    }
    else
    {
        pv->pact = TRUE;
        CALLBACK *pCallback = (CALLBACK *)(calloc(1, sizeof(CALLBACK)));
        callbackRequestProcessCallbackDelayed(pCallback, priorityMedium, pv, 0.1);
    }
    return 2;
}