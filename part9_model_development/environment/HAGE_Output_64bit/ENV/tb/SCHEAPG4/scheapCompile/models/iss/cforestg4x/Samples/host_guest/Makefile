

MACHINE=-mv850e3v5 -v -mextension -mfpsimd -mg4mh
#LNK_OPT=-Xlinker -Tsample.ld

CFLAGS = $(MACHINE) $(LNK_OPT) $(DEFS)

CC=$(V850GCC)
AS=v850-elf-as
LD=v850-elf-ld
NM=$(V850NM)
OBJCOPY = $(V850OBJCOPY)

TARGET_NAME = test
TARGET_ELF_NAME = $(TARGET_NAME).o
TARGET_MAP_NAME = $(TARGET_NAME).map
TARGET_HEX_NAME = $(TARGET_NAME).hex

SRCS = boot.s

ADD_FILE= sample.ld Makefile

all: runtest

build: $(SRCS) $(HEAD) $(ADD_FILE)
	$(AS) $(CFLAGS) $(SRCS) -o a.out
	$(LD) a.out -Tsample.ld -o $(TARGET_ELF_NAME)
	$(NM) -n $(TARGET_ELF_NAME) > $(TARGET_MAP_NAME)
	$(OBJCOPY) $(TARGET_ELF_NAME) $(TARGET_HEX_NAME) --srec-forceS3 -O srec

runtest: build
	@echo 'Running test'
	@./runcforest_py.sh
	@echo

clean:
	rm -f $(TARGET_ELF_NAME) $(TARGET_MAP_NAME) $(TARGET_HEX_NAME) cforest.log cforest_cs.log

clean_release:
	rm -f $(TARGET_ELF_NAME) $(TARGET_MAP_NAME) cforest.log cforest_cs.log
