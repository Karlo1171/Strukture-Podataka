#define _CRT_SECURE_NO_WARNINGS
#define MAX_BODOVA 100
#define DULJINA 50

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct {
    char ime[DULJINA];
    char prezime[DULJINA];
    int bodovi;
} Student;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    int n = 0;
    char redak[100];

    FILE* fp = fopen("studenti.txt", "r");

    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    // Brojimo koliko studenata ima u datoteci
    while (fgets(redak, sizeof(redak), fp) != NULL) {
        n++;
    }

    fclose(fp);

    if (n == 0) {
        printf("Datoteka je prazna.\n");
        return 0;
    }

    // Dinamicka alokacija prostora za n studenata
    Student* studenti = malloc(n * sizeof(Student));

    if (studenti == NULL) {
        printf("Greska pri alociranju memorije.\n");
        return 1;
    }

    // Ponovno otvaramo datoteku
    fp = fopen("studenti.txt", "r");

    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        free(studenti);
        return 1;
    }

    // Ucitavamo studente
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s %s %d",
            studenti[i].ime,
            studenti[i].prezime,
            &studenti[i].bodovi);
    }

    // Ispis
    for (int i = 0; i < n; i++) {

        double relativni_bodovi =
            (double)studenti[i].bodovi / MAX_BODOVA * 100;

        printf("%s %s %d %.2f%%\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            relativni_bodovi);
    }

    free(studenti);
    fclose(fp);

    return 0;
}