#include "decoder.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    char line[256];
    int recovered = 0;
    int rejected = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        line [strcspn(line, "\r\n")] = 0; // remove \n and \r
        if (process_transmission(line)) {
            recovered++;
        } else {
            rejected++;
        }
    }

    printf("Recovered: %d | Rejected: %d\n", recovered, rejected);
    return 0;
    
    
    /*
     * TODO: read lines with fgets until EOF, remove \n and optional \r,
     * call process_transmission, count both outcomes, and print the summary.
     
    puts("Recovered: 0 | Rejected: 0");
    return 0;
    */
}

