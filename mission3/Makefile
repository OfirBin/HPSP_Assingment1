CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g -I.
LDFLAGS = -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=free
TARGET = mission3
SUPPORT = ../_support/mission3/lib/libb7mission3.a
TERMINAL = ../_support/common/lib/libb7terminal.a
TERMINAL_WRAP = -Wl,--wrap=printf -Wl,--wrap=fprintf -Wl,--wrap=puts -Wl,--wrap=fputs
ARCHIVE = data/mission3_archive.b7

.PHONY: all test-a test-b test-c test-d run clean

all: $(TARGET)

$(TARGET): messages.c messages.h $(SUPPORT) $(TERMINAL)
	$(CC) $(CFLAGS) messages.c $(SUPPORT) $(TERMINAL) $(LDFLAGS) $(TERMINAL_WRAP) -o $(TARGET)

test-a: $(TARGET)
	@test -f $(ARCHIVE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@B7_M3_MODE=A ./$(TARGET)

test-b: $(TARGET)
	@test -f $(ARCHIVE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@B7_M3_MODE=B ./$(TARGET)

# Stage C: message_free and store_free
test-c: $(TARGET)
	@test -f $(ARCHIVE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@B7_M3_MODE=C ./$(TARGET)

# Stage D: store_clone
test-d: $(TARGET)
	@test -f $(ARCHIVE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@B7_M3_MODE=D ./$(TARGET)

run: $(TARGET)
	@test -f $(ARCHIVE) || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@B7_M3_MODE=RUN ./$(TARGET)

clean:
	rm -f $(TARGET)
