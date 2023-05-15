#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SLOWA 100
#define MAX_DLUGOSC_SLOWA 50

int main() {
    char words[][10] = {
        "paula",
        "domek",
        "bajda",
        "myszka",
        "kuba"
    };

    int i;
    int length = sizeof(words) / sizeof(words[0]);

    for (i = 0; i < length; i++) {
        printf("%s\n", words[i]);
    }

    srand(time(NULL));
    int indeks = rand() % length;

    int dlugosc = strlen(words[indeks]);

    char ack[dlugosc-1];
    char ack1[dlugosc-1];

    strcpy(ack, words[indeks]);
    printf("slowo: %s\n", ack);
    strcpy(ack1, words[indeks]);
   printf("slowo1: %c\n", ack1[dlugosc]);
    return 0;
}
