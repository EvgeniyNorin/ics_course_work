# ---------------------------------------------------------------------------
# Project name

NAME	= pulse_width_modulation

# Settings for compilator and linker

CC      = sdcc
CFLAGS  = -I./include -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 

# Settings for system of autoincrement of build version

PROJECT  = $(shell type PROJECT)
VERSION  = $(shell type VERSION)
BUILD    = $(shell type BUILD)
TYPE     = $(shell type TYPE)

PROJNAME = ${PROJECT}-${VERSION}-${BUILD}-${TYPE}
TARBALL  = ${PROJNAME}.tar

# Settings for M3P

M3P		 = m3p
COMPORT	 = //./com6
COMLOG	 = $(COMPORT)_log.txt
BAUD	 = 9600	

# Directories with source

SRC_DIR = src
INC_DIR = include
# ---------------------------------------------------------------------------


SOURCES := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(INC_DIR)/*.h)
CXX_TEST=g++
CC_TEST=gcc
TESTS_DIR = tests
GTEST_DIR = thirdparty/googletest/googletest
BLD_DIR = build


test: $(BLD_DIR)/__tests
	$(BLD_DIR)/__tests

$(BLD_DIR)/__tests: $(TESTS_DIR)/tests.cc $(BLD_DIR)/libgtest.a $(TEST_OBJECTS)
	$(CXX_TEST) -std=c++11 -isystem ${GTEST_DIR}/include -pthread $(TESTS_DIR)/tests.cc \
		$(BLD_DIR)/libgtest.a $(TEST_OBJECTS) -o $(BLD_DIR)/__tests

$(BLD_DIR)/libgtest.a: | $(BLD_DIR)
	$(CXX_TEST) -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} \
		-pthread -c ${GTEST_DIR}/src/gtest-all.cc -o $(BLD_DIR)/gtest-all.o
	ar -rv $(BLD_DIR)/libgtest.a $(BLD_DIR)/gtest-all.o

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BLD_DIR)
	$(CC_TEST) -Wno-int-conversion -c $< -o $@

all: pulse_width_modulation

clean:
	rm -f $(NAME).hex
	rm -f $(NAME).bin
	rm -f $(NAME).map
	rm -f $(NAME).lk
	rm -f $(SRC_DIR)/*.asm
	rm -f $(SRC_DIR)/*.rel
	rm -f $(SRC_DIR)/*.rst
	rm -f $(SRC_DIR)/*.sym
	rm -f $(SRC_DIR)/*.lst 
	rm -f $(NAME).mem
	rm -f $(NAME).lnk
	rm -f pm3p_*.txt

load:
	$(M3P) lfile load.m3p


dist:
	tar -cvf $(TARBALL) --exclude=*.tar .
	gzip $(TARBALL)

term:
	$(M3P) echo $(COMLOG) $(BAUD)  openchannel $(COMPORT) +echo 6 term -echo bye

LIST_SRC = \
${SRC_DIR}/isr.c \
${SRC_DIR}/kb_controller.c \
${SRC_DIR}/kb_driver.c \
${SRC_DIR}/keyboard.c \
${SRC_DIR}/lcd.c \
${SRC_DIR}/led.c \
${SRC_DIR}/loop.c \
${SRC_DIR}/max.c \
${SRC_DIR}/sound.c \
${SRC_DIR}/state.c \
${SRC_DIR}/time.c \
${SRC_DIR}/timer_driver.c 

LIST_OBJ = $(LIST_SRC:.c=.rel)

pulse_width_modulation : $(LIST_OBJ) makefile
	$(CC) $(LIST_OBJ) -o pulse_width_modulation.hex $(LFLAGS)
	$(M3P) hb166 pulse_width_modulation.hex pulse_width_modulation.bin bye


$(LIST_OBJ) : %.rel : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@  

