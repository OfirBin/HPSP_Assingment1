CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g -I.
LDFLAGS = -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=free
SANITIZERS = -fsanitize=address,undefined -fno-omit-frame-pointer
TARGET = mission5
DEBUG_TARGET = mission5_debug
SUPPORT = ../_support/mission5/lib/libb7mission5.a
TERMINAL = ../_support/common/lib/libb7terminal.a
TERMINAL_WRAP = -Wl,--wrap=printf -Wl,--wrap=fprintf -Wl,--wrap=puts -Wl,--wrap=fputs
TESTS = ../_support/mission5/tests
EVIDENCE = data/mission5_emergency.b7
TEST_BINS = mission5_test_a mission5_test_b mission5_test_c mission5_test_d

.PHONY: all test-a test-b test-c test-d run debug-run clean

all: $(TARGET)

$(TARGET): emergency.c main.c emergency.h investigation.h $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) emergency.c main.c $(SUPPORT) $(TERMINAL) $(LDFLAGS) $(TERMINAL_WRAP) -o $(TARGET)

$(DEBUG_TARGET): emergency.c main.c emergency.h investigation.h $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) $(SANITIZERS) emergency.c main.c $(SUPPORT) $(TERMINAL) $(LDFLAGS) $(TERMINAL_WRAP) -o $(DEBUG_TARGET)

mission5_test_%: emergency.c $(TESTS)/test_stage_%.o $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) emergency.c $(TESTS)/test_stage_$*.o $(SUPPORT) $(TERMINAL) $(LDFLAGS) $(TERMINAL_WRAP) -o $@

test-a: mission5_test_a
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./mission5_test_a

test-b: mission5_test_b
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./mission5_test_b

test-c: mission5_test_c
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./mission5_test_c

test-d: mission5_test_d
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./mission5_test_d

run: $(TARGET)
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./$(TARGET) --device BEERSHEVANATOR --mode emergency --action self-destruct

debug-run: $(DEBUG_TARGET)
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./$(DEBUG_TARGET) --device BEERSHEVANATOR --mode emergency --action self-destruct

clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(TEST_BINS)
