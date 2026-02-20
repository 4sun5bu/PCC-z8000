# Makefile -- build and run PCC Z8000 end-to-end tests on the Z8002 emulator
#
# Usage: make          -- build everything and run all tests
#        make run_emu  -- build just the test driver
#        make clean    -- remove build artifacts

TOOLDIR   = ..
CZ8       = $(TOOLDIR)/cz8/cz8
AZ8       = $(TOOLDIR)/az8/az8
LDZ8      = $(TOOLDIR)/ldz8

EMU_DIR   = ../../z8000_emu
EMU_INC   = $(EMU_DIR)/include
EMU_LIB   = $(EMU_DIR)/build/libz8000.a

CXX       = c++
CXXFLAGS  = -std=c++17 -Wall -Wextra -I$(EMU_INC) -O2

# Test programs and their expected R0 values
TESTS     = hello arith control switch bitfield shift larith \
            pcc_math pcc_cmp pcc_struct pcc_structret pcc_union pcc_ptr pcc_scope pcc_optim
hello_EXPECT      = 42
arith_EXPECT      = 120
control_EXPECT    = 0
switch_EXPECT     = 0
bitfield_EXPECT   = 0
shift_EXPECT      = 0
larith_EXPECT     = 0
pcc_math_EXPECT   = 0
pcc_cmp_EXPECT    = 0
pcc_struct_EXPECT = 0
pcc_structret_EXPECT = 0
pcc_union_EXPECT  = 0
pcc_ptr_EXPECT    = 0
pcc_scope_EXPECT  = 0
pcc_optim_EXPECT  = 0

# Build artifacts
BOUTS     = $(TESTS:%=%.bout)

.PHONY: all clean test

all: test

# Build the emulator library if needed
$(EMU_LIB):
	$(MAKE) -C $(EMU_DIR) libz8000

# Build the test driver
run_emu: run_emu.cpp $(EMU_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $< $(EMU_LIB)

# Assemble crt0
crt0.b: $(TOOLDIR)/crt0.az8
	$(AZ8) -o $@ $<

# Assemble exit stub
exit.b: $(TOOLDIR)/lib/exit.az8
	$(AZ8) -o $@ $<

# Assemble arithmetic library (named liblong.b to avoid clash with arith.c test)
liblong.b: $(TOOLDIR)/lib/arith.az8
	$(AZ8) -o $@ $<

# Pattern rules for compiling and assembling test .c files
%.az8: %.c
	$(CZ8) < $< > $@

%.b: %.az8
	$(AZ8) -o $@ $<

# Link: crt0 first (so _start is at origin), then test, then exit
# -R 8 = text origin at 0x0008 (after 6-byte PSAP reset vector area)
# -x = discard local symbols
%.bout: crt0.b %.b liblong.b exit.b
	$(LDZ8) -x crt0.b -R 8 $*.b liblong.b exit.b -o $@

# Run all tests
test: run_emu $(BOUTS)
	@pass=0; fail=0; \
	for pair in hello:$(hello_EXPECT) arith:$(arith_EXPECT) control:$(control_EXPECT) \
	            switch:$(switch_EXPECT) bitfield:$(bitfield_EXPECT) shift:$(shift_EXPECT) \
	            larith:$(larith_EXPECT) pcc_math:$(pcc_math_EXPECT) pcc_cmp:$(pcc_cmp_EXPECT) \
	            pcc_struct:$(pcc_struct_EXPECT) pcc_structret:$(pcc_structret_EXPECT) \
	            pcc_union:$(pcc_union_EXPECT) pcc_ptr:$(pcc_ptr_EXPECT) \
	            pcc_scope:$(pcc_scope_EXPECT) pcc_optim:$(pcc_optim_EXPECT); do \
		t=$${pair%%:*}; exp=$${pair##*:}; \
		if ./run_emu $$t.bout -e $$exp; then \
			pass=$$((pass + 1)); \
		else \
			fail=$$((fail + 1)); \
		fi; \
	done; \
	echo ""; \
	echo "Results: $$pass passed, $$fail failed out of $(words $(TESTS)) tests"; \
	[ $$fail -eq 0 ]

clean:
	rm -f run_emu *.az8 *.b *.bout
