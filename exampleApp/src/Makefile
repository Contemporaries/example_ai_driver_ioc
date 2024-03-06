TOP=../..

include $(TOP)/configure/CONFIG
USR_CXXFLAGS += -std=c++11
PROD_IOC = example
DBD += example.dbd
example_DBD += base.dbd ../DBD/example.dbd
SRC_DIRS += ../AI

example_SRCS += example_registerRecordDeviceDriver.cpp
example_SRCS += ExampleAI.cpp
example_SRCS_DEFAULT += exampleMain.cpp
example_SRCS_vxWorks += -nil-
example_LIBS += $(EPICS_BASE_IOC_LIBS)


include $(TOP)/configure/RULES