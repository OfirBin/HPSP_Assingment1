CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g -I. -Idata
SUPPORT = ../_support/mission2
TESTS = $(SUPPORT)/tests
LIBRARY = $(SUPPORT)/lib/libb7investigation.a
TERMINAL = ../_support/common/lib/libb7terminal.a
TERMINAL_WRAP = -Wl,--wrap=printf -Wl,--wrap=fprintf -Wl,--wrap=puts -Wl,--wrap=fputs
TARGET = mission2
TEST_BINS = mission2_test_a mission2_test_b mission2_test_c mission2_test_d

.PHONY: all test-a test-b test-c test-d run clean

all: $(TARGET)

$(TARGET): agents.c main.c agents.h investigation.h \
	data/mission2_data.c data/mission2_data.h $(LIBRARY) $(TERMINAL)
	$(CC) $(CFLAGS) agents.c main.c data/mission2_data.c $(LIBRARY) $(TERMINAL) $(TERMINAL_WRAP) -o $(TARGET)

mission2_test_%: agents.c $(TESTS)/test_stage_%.o $(TERMINAL)
	$(CC) $(CFLAGS) agents.c $(TESTS)/test_stage_$*.o $(TERMINAL) $(TERMINAL_WRAP) -o $@

mission2_stage_reveal: $(TESTS)/stage_reveal.o $(LIBRARY) $(TERMINAL)
	$(CC) $(CFLAGS) $(TESTS)/stage_reveal.o $(LIBRARY) $(TERMINAL) $(TERMINAL_WRAP) -o $@

mission2_main_capacity: main.c $(TESTS)/test_main_capacity.o $(TERMINAL)
	$(CC) $(CFLAGS) main.c $(TESTS)/test_main_capacity.o $(TERMINAL) $(TERMINAL_WRAP) -o $@

test-a: mission2_test_a mission2_stage_reveal
	@./mission2_test_a || { echo "FAIL: Stage A contract is not implemented correctly yet." >&2; exit 1; }
	@./mission2_stage_reveal A

test-b: mission2_test_b mission2_main_capacity mission2_stage_reveal
	@./mission2_test_b || { echo "FAIL: Stage B contract is not implemented correctly yet." >&2; exit 1; }
	@./mission2_main_capacity || { echo "FAIL: Stage B main must pass the full working-array capacity." >&2; exit 1; }
	@echo "PASS: Stage B main passes the correct working-array capacity."
	@./mission2_stage_reveal B

test-c: mission2_test_c mission2_stage_reveal
	@./mission2_test_c || { echo "FAIL: Stage C contract is not implemented correctly yet." >&2; exit 1; }
	@./mission2_stage_reveal C

test-d: mission2_test_d mission2_stage_reveal
	@./mission2_test_d || { echo "FAIL: Stage D contract is not implemented correctly yet." >&2; exit 1; }
	@./mission2_stage_reveal D

run: $(TARGET)
	@test -f data/mission2_data.c || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./$(TARGET)

clean:
	rm -f $(TARGET) $(TEST_BINS) mission2_stage_reveal mission2_main_capacity
