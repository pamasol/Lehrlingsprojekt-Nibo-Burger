

ifndef PLATFORM
PLATFORM=atmega88a-8
endif

ifeq ($(PLATFORM),atmega88a-8)
TARGET=atmega88p
F_CPU=8000000
endif


EFUSE=0x01
HFUSE=0xdd
LFUSE=0xe2


ISP=stk500v2
PORT=/dev/ttyACM0

ifdef UCOM
AVRDUDE = -c ${ISP} -p ${TARGET} -P ${PORT}
endif

ifndef UCOM
# AVRDUDE = -c ${ISP} -p ${TARGET} -P ${PORT}
AVRDUDE = -V -c usbasp -p ${TARGET}
endif

PNAME = maroon_firmware

all: $(PNAME).hex

hex: $(PNAME).hex

help:
	@echo "Usage: make                same as make help"
	@echo "       make help           same as make"
	@echo "       make hex            create $(PNAME).hex"
	@echo "       make clean          remove redundant data"
	@echo "       make disasm         disasm $(PNAME)"
	@echo "       make flash          upload $(PNAME).hex into flash"
	@echo "       make fuses          program fuses"
	@echo "       make avrdude        test avrdude"
	@echo "       make chipinit       program fuses and upload $(PNAME).hex into flash"
	@echo "Current values:"
	@echo "       TARGET=${TARGET}"
	@echo "       LFUSE=${LFUSE}"
	@echo "       HFUSE=${HFUSE}"
	@echo "       CLOCK=$(F_CPU)"
	@echo "       ISP=${ISP}"
	@echo "       PORT=${PORT}"

#OPTIMIZE = -Os
OPTIMIZE = -Os -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions

COMPILE = avr-gcc -Wl,-u,vfprintf -lprintf_min -std=c99 -Wall $(OPTIMIZE) -Iusbdrv -I. -mmcu=$(TARGET) -DF_CPU=$(F_CPU)L # -DDEBUG_LEVEL=2

OBJECTS = \
          matrix.o \
          timing.o \
          shifter.o \
          pxbuf.o \
          comm.o \
          font.o \
          unicode.o \
          usart.o \
          process.o \
          textbuf.o \
          analog.o \
          main.o

          
main.o: main.c
	$(COMPILE) -c $< -o $@

%.o: %.c %.h
	$(COMPILE) -c $< -o $@
	
	
#-Wa,-ahlms=$<.lst

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

clean:
	rm -f $(PNAME).hex $(PNAME).lst $(PNAME).obj $(PNAME).cof $(PNAME).list $(PNAME).map $(PNAME).eep.hex $(PNAME).bin *.o $(PNAME).s

# file targets:
$(PNAME).bin:	$(OBJECTS)
	$(COMPILE) -o $(PNAME).bin $(OBJECTS) -Wl,-Map,$(PNAME).map

$(PNAME).hex:	$(PNAME).bin
	rm -f $(PNAME).hex $(PNAME).eep.hex
	avr-objcopy -j .text -j .data -O ihex $(PNAME).bin $(PNAME).hex
	avr-size -A $(PNAME).bin

#	./checksize $(PNAME).bin
# do the checksize script as our last action to allow successful compilation
# on Windows with WinAVR where the Unix commands will fail.

disasm:	$(PNAME).bin
	avr-objdump -d $(PNAME).bin

cpp:
	$(COMPILE) -E $(PNAME).c

flash: $(PNAME).hex
	avrdude ${AVRDUDE} -B20 -U flash:w:$(PNAME).hex

fuses:
	avrdude ${AVRDUDE} -B100 -u -U efuse:w:$(EFUSE):m -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m

avrdude:
	avrdude ${AVRDUDE} -B20 -v



chipinit: fuses flash
	@tput setaf 2; tput bold
	@echo "      ***************************"
	@echo "      * PROGRAMMING SUCCESSFULL *"
	@echo "      ***************************"
	@tput sgr0
	@echo

tarball: $(PNAME).hex
	cp $(PNAME).hex ../hex/programmer-$(PLATFORM).hex


