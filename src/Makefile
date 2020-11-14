export MALLOC_MMAP_THRESHOLD_=1
export MALLOC_CHECK_=1
export MALLOC_PERTURB_=1

CC=gcc

# -mmitigate-rop is obsolete.
# FIXME: -ftrapv isn't 'hardening', per-se...
HARDENING=-fstack-protector-all -pie -fPIE -ftrapv

LDFLAGS=
LIBS=-levent_openssl -lssl -lcrypto -levent -lpcre 
INCLUDEDIRS=headers

CFLAGS=-W -Wall -O2 -Wno-unused-variable -Wno-unused-parameter $(HARDENING) -I$(INCLUDEDIRS)

ifeq ($(shell uname -s),Darwin)
	LDFLAGS=-Wl
else
	LDFLAGS=-Wl,-z,relro,-z,now 
endif

OBJECTS=blocklist.o \
file_ops.o \
match_algorithms.o \
matchlist.o \
mem_ops.o \
OctopusWAF.o \
opt_extract.o \
proxy.o \
server.o \
strsec.o \
utils.o \
validate.o

../bin/OctopusWAF: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo
	@echo " Execute \"bin/OctopusWAF\" to start..."
	@echo

# Dependencies to build object files.
include make.dep

.PHONY: clean distclean

clean:
	-rm $(OBJECTS)

distclean: clean
	rm ../bin/OctopusWAF
