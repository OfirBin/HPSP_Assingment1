CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g -I.
LDFLAGS = -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=free
SANITIZERS = -fsanitize=address,undefined -fno-omit-frame-pointer
TARGET = mission4
DEBUG_TARGET = mission4_debug
GDB_TARGET = mission4_gdb
GDB_DEMO = gdb_demo
SUPPORT = ../_support/mission4/lib/libb7mission4.a
TERMINAL = ../_support/common/lib/libb7terminal.a
TERMINAL_WRAP = -Wl,--wrap=printf -Wl,--wrap=fprintf -Wl,--wrap=puts -Wl,--wrap=fputs
GDB_DEMO_SOURCE = debug/gdb_demo.c
EVIDENCE = data/mission4_controller.b7

.PHONY: all test-a test-b test-c test-d debug-a debug-b debug-c debug-d \
	gdb-a gdb-b gdb-c gdb-d gdb-demo run clean

all: $(TARGET)

$(TARGET): controller.c controller.h $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) controller.c $(SUPPORT) $(TERMINAL) $(LDFLAGS) $(TERMINAL_WRAP) -o $(TARGET)

$(DEBUG_TARGET): controller.c controller.h $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) $(SANITIZERS) controller.c $(SUPPORT) $(TERMINAL) $(LDFLAGS) $(TERMINAL_WRAP) -o $(DEBUG_TARGET)

$(GDB_TARGET): controller.c controller.h $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) -O0 controller.c $(SUPPORT) $(TERMINAL) $(LDFLAGS) $(TERMINAL_WRAP) -o $(GDB_TARGET)

$(GDB_DEMO): $(GDB_DEMO_SOURCE)
	$(CC) -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 $(GDB_DEMO_SOURCE) -o $(GDB_DEMO)

test-a test-b test-c test-d run: $(TARGET)
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@mode=$$(printf '%s' '$@' | sed 's/test-//; s/run/RUN/' | tr 'a-z' 'A-Z'); B7_M4_MODE=$$mode ./$(TARGET)

debug-a debug-b debug-c debug-d: $(DEBUG_TARGET)
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@mode=$$(printf '%s' '$@' | sed 's/debug-//' | tr 'a-z' 'A-Z'); B7_M4_MODE=$$mode ./$(DEBUG_TARGET)

gdb-a gdb-b gdb-c gdb-d: $(GDB_TARGET)
	@test -f $(EVIDENCE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@mode=$$(printf '%s' '$@' | sed 's/gdb-//' | tr 'a-z' 'A-Z'); B7_M4_MODE=$$mode gdb ./$(GDB_TARGET)

gdb-demo: $(GDB_DEMO)
	@printf '%s\n' 'Start the tutorial with: gdb ./gdb_demo'

clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(GDB_TARGET) $(GDB_DEMO)
