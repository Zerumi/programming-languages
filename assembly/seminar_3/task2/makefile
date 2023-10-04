
ASM = nasm
ASMFLAGS = -f elf64
LD = ld

%.o: %.asm
	$(ASM) -o $@ $(ASMFLAGS) $(basename $@).asm
	
program: lib.o lib_test.o
	$(LD) -o program lib.o lib_test.o
	
.PHONY: clean
clean:
	rm *.o program


