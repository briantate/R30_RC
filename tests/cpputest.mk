#Set this to @ to keep the makefile quiet
# SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = tests/main

#--- Inputs ----#

CPPUTEST_HOME := /home/cpputest
PROJECT_HOME_DIR = /home/app

# --- SRC_FILES and SRC_DIRS ---
# Production code files are compiled and put into
# a library to link with the test runner.
#
# Test code of the same name overrides
# production code at link time.
#
# SRC_FILES specifies individual production
# code files.
#
# SRC_DIRS specifies directories containing
# production code C and CPP files.
#
SRC_FILES := $(shell find tests -type f -name '*.c')
SRC_FILES += src/state_machine/state_machine.c
# SRC_DIRS += 

# --- TEST_SRC_FILES and TEST_SRC_DIRS ---
# Test files are always included in the build.
# Production code is pulled into the build unless
# it is overriden by code of the same name in the
# test code.
#
# TEST_SRC_FILES specifies individual test files to build.
# TEST_SRC_DIRS, builds everything in the directory

# TEST_SRC_FILES +=
TEST_SRC_DIRS += tests
TEST_SRC_DIRS += tests/StateMachine
TEST_SRC_DIRS += tests/network

# --- MOCKS_SRC_DIRS ---
# MOCKS_SRC_DIRS specifies a directories where you can put your
# mocks, stubs and fakes.  You can also just put them
# in TEST_SRC_DIRS
MOCKS_SRC_DIRS += tests/mocks

# Turn on CppUMock, gcov
CPPUTEST_USE_EXTENSIONS = Y
CPPUTEST_USE_GCOV = Y

# INCLUDE_DIRS are searched in order after the included file's
# containing directory

INCLUDE_DIRS += src
INCLUDE_DIRS += src/config
INCLUDE_DIRS += src/network
INCLUDE_DIRS += src/state_machine
# INCLUDE_DIRS += src/ASF/common/boards
# INCLUDE_DIRS += src/ASF/common/services/serial
# INCLUDE_DIRS += src/ASF/common/services/serial/sam0_usart
# INCLUDE_DIRS += src/ASF/common/utils
# INCLUDE_DIRS += src/ASF/common/utils/interrupt
# INCLUDE_DIRS += src/ASF/common2/boards/user_board
# INCLUDE_DIRS += src/ASF/common2/services/delay
# INCLUDE_DIRS += src/ASF/common2/services/delay/sam0
# INCLUDE_DIRS += src/ASF/sam0/drivers/adc
# INCLUDE_DIRS += src/ASF/sam0/drivers/adc/adc_sam_l_c
# INCLUDE_DIRS += src/ASF/sam0/drivers/extint
# INCLUDE_DIRS += src/ASF/sam0/drivers/extint/extint_sam_l_c
# INCLUDE_DIRS += src/ASF/sam0/drivers/nvm
# INCLUDE_DIRS += src/ASF/sam0/drivers/port
# INCLUDE_DIRS += src/ASF/sam0/drivers/rtc
# INCLUDE_DIRS += src/ASF/sam0/drivers/rtc/rtc_sam_l_c
# INCLUDE_DIRS += src/ASF/sam0/drivers/sercom
# INCLUDE_DIRS += src/ASF/sam0/drivers/sercom/spi
# INCLUDE_DIRS += src/ASF/sam0/drivers/sercom/usart
# INCLUDE_DIRS += src/ASF/sam0/drivers/system
# INCLUDE_DIRS += src/ASF/sam0/drivers/system/clock
# INCLUDE_DIRS += src/ASF/sam0/drivers/system/clock/clock_samr30
# INCLUDE_DIRS += src/ASF/sam0/drivers/system/interrupt
# INCLUDE_DIRS += src/ASF/sam0/drivers/system/interrupt/system_interrupt_samr30
# INCLUDE_DIRS += src/ASF/sam0/drivers/system/pinmux
# INCLUDE_DIRS += src/ASF/sam0/drivers/system/power/power_sam_l
# INCLUDE_DIRS += src/ASF/sam0/drivers/system/reset/reset_sam_l
# INCLUDE_DIRS += src/ASF/sam0/drivers/tc
# INCLUDE_DIRS += src/ASF/sam0/drivers/tc/tc_sam_l_c
# INCLUDE_DIRS += src/ASF/sam0/utils
# INCLUDE_DIRS += src/ASF/sam0/utils/cmsis/samr30/include
# INCLUDE_DIRS += src/ASF/sam0/utils/cmsis/samr30/include/component
# INCLUDE_DIRS += src/ASF/sam0/utils/cmsis/samr30/include/instance
# INCLUDE_DIRS += src/ASF/sam0/utils/cmsis/samr30/include/pio
# INCLUDE_DIRS += src/ASF/sam0/utils/cmsis/samr30/source
# INCLUDE_DIRS += src/ASF/sam0/utils/header_files
# INCLUDE_DIRS += src/ASF/sam0/utils/preprocessor
# INCLUDE_DIRS += src/ASF/sam0/utils/stdio/stdio_serial
# INCLUDE_DIRS += src/ASF/thirdparty/CMSIS/include
# INCLUDE_DIRS += src/ASF/thirdparty/CMSIS/include
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/addons/sio2host/uart
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/include
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/services/pds/inc
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/services/pds/inc/nv
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/services/pds/inc/nv/External
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/services/pds/inc/wl
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/services/sleep_mgr
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/source/mimac
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/source/mimac/phy
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/source/mimac/phy/at86rf212b
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/source/miwi_p2p_star
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/miwi/source/sys
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/services/common_hw_timer
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/services/common_hw_timer/sam0
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/services/nvm
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/services/nvm/sam0
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/services/sal/inc
# INCLUDE_DIRS += src/ASF/thirdparty/wireless/services/trx_access
# INCLUDE_DIRS += src/config

INCLUDE_DIRS += $(CPPUTEST_HOME)/include
INCLUDE_DIRS += $(CPPUTEST_HOME)/include/Platforms/Gcc


# --- CPPUTEST_OBJS_DIR ---
# CPPUTEST_OBJS_DIR lets you control where the
# build artifact (.o and .d) files are stored.
#
# If you have to use "../" to get to your source path
# the makefile will put the .o and .d files in surprising
# places.
#
# To make up for each level of "../"in the source path,
# add place holder subdirectories to CPPUTEST_OBJS_DIR
# each.
# e.g. if you have "../../src", set to "test-objs/1/2"
#
# This is kind of a kludge, but it causes the
# .o and .d files to be put under objs.
CPPUTEST_OBJS_DIR = tests/obj

CPPUTEST_LIB_DIR = tests/lib

# You may have to tweak these compiler flags
#    CPPUTEST_WARNINGFLAGS - apply to C and C++
#    CPPUTEST_CFLAGS - apply to C files only
#    CPPUTEST_CXXFLAGS - apply to C++ files only
#    CPPUTEST_CPPFLAGS - apply to C and C++ Pre-Processor
#
# If you get an error like this
#     TestPlugin.h:93:59: error: 'override' keyword is incompatible
#        with C++98 [-Werror,-Wc++98-compat] ...
# The compiler is basically telling you how to fix the
# build problem.  You would add this flag setting
#     CPPUTEST_CXXFLAGS += -Wno-c++14-compat




# Some flags to quiet clang
ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang"), 1)
CPPUTEST_WARNINGFLAGS += -Wno-unknown-warning-option
CPPUTEST_WARNINGFLAGS += -Wno-covered-switch-default
CPPUTEST_WARNINGFLAGS += -Wno-reserved-id-macro
CPPUTEST_WARNINGFLAGS += -Wno-keyword-macro
CPPUTEST_WARNINGFLAGS += -Wno-documentation
CPPUTEST_WARNINGFLAGS += -Wno-missing-noreturn
endif

CPPUTEST_WARNINGFLAGS += -Wall
# CPPUTEST_WARNINGFLAGS += -Werror
CPPUTEST_WARNINGFLAGS += -Wfatal-errors
CPPUTEST_WARNINGFLAGS += -Wswitch-default
CPPUTEST_WARNINGFLAGS += -Wno-format-nonliteral
CPPUTEST_WARNINGFLAGS += -Wno-sign-conversion
CPPUTEST_WARNINGFLAGS += -Wno-pedantic
CPPUTEST_WARNINGFLAGS += -Wno-shadow
CPPUTEST_WARNINGFLAGS += -Wno-missing-field-initializers
CPPUTEST_WARNINGFLAGS += -Wno-unused-parameter
CPPUTEST_CFLAGS += -pedantic
CPPUTEST_CFLAGS += -Wno-missing-prototypes
CPPUTEST_CFLAGS += -Wno-strict-prototypes
CPPUTEST_CXXFLAGS += -Wno-c++14-compat
CPPUTEST_CXXFLAGS += --std=c++11
CPPUTEST_CXXFLAGS += -Wno-c++98-compat-pedantic
CPPUTEST_CXXFLAGS += -Wno-c++98-compat

# --- LD_LIBRARIES -- Additional needed libraries can be added here.
# commented out example specifies math library
LD_LIBRARIES = -lCppUTest -lCppUTestExt

# Look at $(CPPUTEST_HOME)/build/MakefileWorker.mk for more controls

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
