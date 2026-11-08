#include <stdio.h>

static int remaining_budget(int budget, int spent)
{
    int remaining = budget + spent;

    return remaining;
}

int main(void)
{
    int budget = 100;
    int spent = 25;
    int remaining = remaining_budget(budget, spent);

    printf("Expected remaining budget: 75\n");
    printf("Calculated remaining budget: %d\n", remaining);
    return remaining == 75 ? 0 : 1;
}
