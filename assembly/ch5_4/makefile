RESULT_NAME = program

SRC_DIR = ./

ASM = nasm
ASMFLAGS = -f elf64
LD = ld

INC_FILES = $(wildcard $(SRC_DIR)*.inc)
ASM_FILES = $(wildcard $(SRC_DIR)*.asm)
O_FILES = $(patsubst %.asm,%.o,$(ASM_FILES))

%.o: %.asm
	$(ASM) -o $@ $(ASMFLAGS) $<
	
all:
	make --no-print-directory build
	make --no-print-directory test

test:
	echo "test =)"

build: $(O_FILES) $(INC_FILES)
	$(LD) -o $(RESULT_NAME) $(O_FILES)
	
clean:
	rm -f *.o $(RESULT_NAME)

rebuild:
	make --no-print-directory clean
	make --no-print-directory build

.PHONY: all test build clean rebuild
