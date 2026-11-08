CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g -I.
SANITIZERS = -fsanitize=address,undefined -fno-omit-frame-pointer
TARGET = mission6_bonus
DEBUG_TARGET = mission6_bonus_debug
SUPPORT = ../_support/mission6/lib/libb7mission6.a
TERMINAL = ../_support/common/lib/libb7terminal.a
TERMINAL_WRAP = -Wl,--wrap=printf -Wl,--wrap=fprintf -Wl,--wrap=puts -Wl,--wrap=fputs
TESTS = ../_support/mission6/tests
PACKETS = data/mission6_packets.txt
MEETINGS = data/mission6_meetings.bin
TEST_BINS = mission6_test_a mission6_test_b mission6_test_c mission6_test_d mission6_test_e mission6_test_f

.PHONY: all test-a test-b test-c test-d test-e test-f run debug-run clean

all: $(TARGET)

$(TARGET): bitwise.c main.c bitwise.h investigation.h $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) bitwise.c main.c $(SUPPORT) $(TERMINAL) $(TERMINAL_WRAP) -o $(TARGET)

$(DEBUG_TARGET): bitwise.c main.c bitwise.h investigation.h $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) $(SANITIZERS) bitwise.c main.c $(SUPPORT) $(TERMINAL) $(TERMINAL_WRAP) -o $(DEBUG_TARGET)

mission6_test_%: bitwise.c $(TESTS)/test_stage_%.o $(TERMINAL)
	$(CC) $(CFLAGS) bitwise.c $(TESTS)/test_stage_$*.o $(TERMINAL) $(TERMINAL_WRAP) -o $@

mission6_stage_f_bitwise.o: bitwise.c bitwise.h
	$(CC) $(CFLAGS) -c bitwise.c -o $@
	objcopy --weaken-symbol=decode_channel $@

mission6_test_f: mission6_stage_f_bitwise.o $(TESTS)/test_stage_f.o $(TERMINAL)
	$(CC) $(CFLAGS) mission6_stage_f_bitwise.o $(TESTS)/test_stage_f.o $(TERMINAL) $(TERMINAL_WRAP) -o $@

test-a: mission6_test_a
	@./mission6_test_a

test-b: mission6_test_b
	@./mission6_test_b

test-c: mission6_test_c
	@./mission6_test_c

test-d: mission6_test_d
	@./mission6_test_d

test-e: mission6_test_e
	@./mission6_test_e

test-f: mission6_test_f
	@./mission6_test_f

run: $(TARGET)
	@test -f $(PACKETS) -a -f $(MEETINGS) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./$(TARGET)

debug-run: $(DEBUG_TARGET)
	@test -f $(PACKETS) -a -f $(MEETINGS) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./$(DEBUG_TARGET)

clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(TEST_BINS) mission6_stage_f_bitwise.o
