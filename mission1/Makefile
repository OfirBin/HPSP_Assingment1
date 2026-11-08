CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g
SUPPORT = ../_support/mission1
TERMINAL = ../_support/common/lib/libb7terminal.a
TERMINAL_WRAP = -Wl,--wrap=printf -Wl,--wrap=fprintf -Wl,--wrap=puts -Wl,--wrap=fputs
TESTS = $(SUPPORT)/tests
FIXTURES = $(SUPPORT)/fixtures
TARGET = mission1
TEST_A = mission1_test_a
TEST_B = mission1_test_b

.PHONY: all test-a test-b test-c run clean

all: $(TARGET)

$(TARGET): decoder.c main.c decoder.h $(TERMINAL)
	$(CC) $(CFLAGS) decoder.c main.c $(TERMINAL) $(TERMINAL_WRAP) -o $(TARGET)

$(TEST_A): decoder.c $(TESTS)/test_decoder.o $(TERMINAL)
	$(CC) $(CFLAGS) decoder.c $(TESTS)/test_decoder.o $(TERMINAL) $(TERMINAL_WRAP) -o $(TEST_A)

$(TEST_B): decoder.c $(TESTS)/test_parser.o $(TERMINAL)
	$(CC) $(CFLAGS) decoder.c $(TESTS)/test_parser.o $(TERMINAL) $(TERMINAL_WRAP) -o $(TEST_B)

test-a: $(TEST_A)
	@./$(TEST_A) || { echo "FAIL: Part A contract is not implemented yet." >&2; exit 1; }

test-b: $(TEST_B)
	@./$(TEST_B) > .mission1_part_b.out || { echo "FAIL: Part B checksum or parser assertions failed." >&2; exit 1; }
	@diff -u $(FIXTURES)/parser_expected.txt .mission1_part_b.out || { echo "FAIL: Part B return values or stdout differ from the contract." >&2; exit 1; }
	@rm -f .mission1_part_b.out
	@echo "PASS: Part B exact output matches the contract."

test-c: $(TARGET)
	@./$(TARGET) < data/sample_input.txt > .mission1_part_c.out
	@diff -u $(FIXTURES)/sample_expected.txt .mission1_part_c.out || { echo "FAIL: Part C sample output or summary differs from the contract." >&2; exit 1; }
	@rm -f .mission1_part_c.out
	@echo "PASS: Part C end-to-end sample output matches exactly."

run: $(TARGET)
	@test -f data/transmissions.txt || { echo "Run ../operation_b7 from the package root first." >&2; exit 1; }
	@./$(TARGET) < data/transmissions.txt

clean:
	rm -f $(TARGET) $(TEST_A) $(TEST_B) .mission1_part_b.out .mission1_part_c.out
