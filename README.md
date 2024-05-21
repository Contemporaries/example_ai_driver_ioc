# **IOC Dev Notes**

> IOC name: example

## **Create IOC by EPICS tool**

```shell
mkdir example_ioc
cd example_ioc
makeBaseApp.pl -t ioc example
makeBaseApp.pl -i -t ioc example
```

## **Create DB file**

> example.db

```shell
record(ai, "example:ai") {
    field(DESC, "this is a example of ai record")
    field(DTYP, "EXAMPLE_AI")
    field(SCAN, ".1 second")
}
```

### **Makefile**

```makefile
TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
​
#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += example.db
​
#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>
​
include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
​
​
```

## **Source code**

### **exampleMain.cpp**

```cpp
/*
 * @Author HuangLi
 * @Email lihuang@ihep.ac.cn
 * @Date 3/5/24
 * @Version 1.0.0
 */
#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
​
#include "epicsExit.h"
#include "epicsThread.h"
#include "iocsh.h"
​
int main(int argc,char *argv[])
{
    if(argc>=2) {
        iocsh(argv[1]);
        epicsThreadSleep(.2);
    }
    iocsh(NULL);
    epicsExit(0);
    return(0);
}
​
```

### **DBD**

> example.dbd define device type

```
# Devices
device(ai, INST_IO, EXAMPLE_AI, "EXAMPLE_AI")
```

### **ExampleAI.cpp**

```cpp
/*
 * @Author HuangLi
 * @Email lihuang@ihep.ac.cn
 * @Date 3/5/24
 * @Version 1.0.0
 */
​
#include <stddef.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include "limits"
​
#include "recGbl.h"
#include "dbAccess.h"
​
#include "recSup.h"
#include "devSup.h"
#include "aiRecord.h"
#include "epicsExport.h"
#include <iostream>
​
using namespace std;
​
static long init(int *);
​
static long initRecord(aiRecord *pv);
​
static long readAI(aiRecord *pv);
​
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
​
epicsExportAddress(dset, EXAMPLE_AI);
​
static long init(int *after)
{
    std::cout << "init" << std::endl;
    return 0;
}
​
static long initRecord(aiRecord *pv)
{
    std::cout << "initRecord" << std::endl;
    pv->udf = FALSE;
    return 0;
}
​
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
```

### **Makefile**

```makefile
TOP=../..
​
include $(TOP)/configure/CONFIG
USR_CXXFLAGS += -std=c++11
PROD_IOC = example
DBD += example.dbd
example_DBD += base.dbd ../DBD/example.dbd
SRC_DIRS += ../AI
​
example_SRCS += example_registerRecordDeviceDriver.cpp
example_SRCS += ExampleAI.cpp
example_SRCS_DEFAULT += exampleMain.cpp
example_SRCS_vxWorks += -nil-
example_LIBS += $(EPICS_BASE_IOC_LIBS)
​
​
include $(TOP)/configure/RULES
```

## **Run**

```shell
init
initRecord
init
iocRun: All initialization complete
val: 21.263
epics> val: 21.263
val: 21.263
val: 21.263
val: 21.263
```
