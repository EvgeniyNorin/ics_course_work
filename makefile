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
# ---------------------------------------------------------------------------

all: pulse_width_modulation

clean:
	rm -f $(NAME).hex
	rm -f $(NAME).bin
	rm -f $(NAME).map
	rm $(NAME).mem
	rm $(NAME).lnk
	rm pm3p_*.txt
	rm com?_log.txt
	rm $(TARBALL).gz
	rm $(SRC_DIR)\*.asm
	rm $(SRC_DIR)\*.rel
	rm $(SRC_DIR)\*.rst
	rm $(SRC_DIR)\*.sym
	rm $(SRC_DIR)\*.lst 

load:
	$(M3P) lfile load.m3p


dist:
	tar -cvf $(TARBALL) --exclude=*.tar .
	gzip $(TARBALL)

term:
	$(M3P) echo $(COMLOG) $(BAUD)  openchannel $(COMPORT) +echo 6 term -echo bye



LIST_SRC = \
$(SRC_DIR)/main.c \
${SRC_DIR}/state.c


LIST_OBJ = $(LIST_SRC:.c=.rel)

pulse_width_modulation : $(LIST_OBJ) makefile
	$(CC) $(LIST_OBJ) -o pulse_width_modulation.hex $(LFLAGS)
	$(M3P) hb166 pulse_width_modulation.hex pulse_width_modulation.bin bye


$(LIST_OBJ) : %.rel : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@  

