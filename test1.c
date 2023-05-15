#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SLOWA 100
#define MAX_DLUGOSC_SLOWA 50

char slowa[MAX_SLOWA][MAX_DLUGOSC_SLOWA];

const char* losuj_i_zwroc_slowo_z_pliku(const char* nazwa_pliku)
{
    // Inicjalizacja generatora liczb pseudolosowych
    srand(time(NULL));

    // Otwieranie pliku
    FILE* plik = fopen(nazwa_pliku, "r");
    if (plik == NULL)
    {
        printf("Nie udalo sie otworzyc pliku.\n");
        return NULL;
    }

    // Wczytywanie slow z pliku do tablicy
    int ilosc_slow = 0;

    char slowo[MAX_DLUGOSC_SLOWA];
    while (ilosc_slow < MAX_SLOWA && fscanf(plik, "%s", slowo) == 1)
    {
        strcpy(slowa[ilosc_slow], slowo);
        ilosc_slow++;
    }

    fclose(plik);

    if (ilosc_slow == 0)
    {
        printf("Brak slow w pliku.\n");
        return NULL;
    }

    // Wylosowanie indeksu slowa
    int indeks = rand() % ilosc_slow;

    // Zwracanie wylosowanego slowa
    return slowa[indeks];
}

int main()
{
    const char* nazwa_pliku = "slowa.txt";
    const char* wylosowane_slowo = losuj_i_zwroc_slowo_z_pliku(nazwa_pliku);

        printf("Wylosowane slowo: %s\n", wylosowane_slowo);
        int dlugosc = strlen(wylosowane_slowo);
        printf("Dlugosc wylosowanego slowa: %d\n", dlugosc);

    char ack[dlugosc];
    strcpy(ack, wylosowane_slowo);

     printf("ack: %s\n", ack);

    return 0;
}
