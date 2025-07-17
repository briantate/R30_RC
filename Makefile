export

# Define what we are building
TARGET = tests

# Build input directories 
FIRMWARE_DIR := src
TEST_DIR := tests
COVERAGE_DIR := $(TEST_DIR)/coverage
LIB_DIR := $(FIRMWARE_DIR)/lib

# List of the test headers and sources. The TEST code should be
# hardware independent and should be able to be used in the test harness
TEST_HEADERS := $(wildcard $(TEST_DIR)/*.h*)
TEST_HEADERS += $(wildcard $(TEST_DIR)/*.hpp*)
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.c)
TEST_SOURCES += $(wildcard $(TEST_DIR)/*.cpp)
TEST_INCLUDES := $(patsubst %,-I%,$(sort $(dir $(TEST_HEADERS))))

# # Gcov and Gcovr flags
# GCOVR_EXCLUDE_DIR = '($(TEST_DIR)|$(LIB_DIR))'
# GCOVR_EXCLUDE_FLAG := --exclude $(GCOVR_EXCLUDE_DIR)
# GCOVR_FLAGS := $(GCOVR_EXCLUDE_FLAG) --txt --html-details --html=$(COVERAGE_DIR)/coverage.html

# Gcov and Gcovr flags
GCOVR_EXCLUDE_DIR = '($(TEST_DIR)|$(LIB_DIR))'
GCOVR_EXCLUDE_FLAG := --exclude $(GCOVR_EXCLUDE_DIR)

GCOVR_FLAGS := $(GCOVR_EXCLUDE_FLAG) \
			   --object-directory tests/obj \
               --root . \
               --txt \
               --html-details \
               --html=$(COVERAGE_DIR)/coverage.html \
# 			   --filter src/


#######################################
# build targets
#######################################
clean_unit_tests:
	-rm -fR $(COVERAGE_DIR)
	-rm -fR $(TEST_DIR)/junit
	-rm -fR $(TEST_DIR)/lib
	-rm -fR $(TEST_DIR)/obj

.PHONY: unit_tests
unit_tests: 
	$(MAKE) -j CC=gcc -f $(TEST_DIR)/cpputest.mk
	mkdir -p $(COVERAGE_DIR)
	@gcovr $(GCOVR_FLAGS)

mccabe_analysis:
	@pmccabe -vt $(APP_SOURCES) $(APP_C_SOURCES)

.PHONY: docker_image
docker_image:
	docker build -t dev .

.PHONY: docker_run
docker_run:
	docker run --rm -it --privileged \
	-v "$$(pwd):/home/app" \
	-v "/dev/bus/usb:/dev/bus/usb" \
	dev:latest bash

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***