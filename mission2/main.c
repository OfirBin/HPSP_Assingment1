#include "investigation.h"

int main(void)
{
    Agent working[16];
    size_t capacity = sizeof(working) / sizeof(working[0]); /* TODO Stage B: use sizeof on working. */

    return run_investigation(working, capacity);
}
