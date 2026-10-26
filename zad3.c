#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50

typedef struct _Person* Position;
typedef struct _Person {
    char name[MAX_LEN];
    char lastName[MAX_LEN];
    int year;
    Position next;
} Person;

Position createPerson(char* ime, char* prezime, int god);
int insertAfter(Position p, Position newPerson);
int insertBegin(Position head, char* ime, char* prezime, int god);
int insertEnd(Position head, char* ime, char* prezime, int god);
int printList(Position p);
Position findLastName(Position head, char* prezime);
int deletePerson(Position head, char* prezime);
int freeList(Position head);

/* 3. zadatak */
Position findPrevious(Position head, char* prezime);
int insertNewAfter(Position head, char* trazenoPrezime, char* ime, char* prezime, int god);
int insertBefore(Position head, char* trazenoPrezime, char* ime, char* prezime, int god);
int sortFun(Position head);
int writeToFile(Position head, char* imeDatoteke);
int readFromFile(Position head, char* imeDatoteke);

int main(void)
{
    Person Head = { .name = {0}, .lastName = {0}, .year = 0, .next = NULL };

    int izbor = -1;
    char ime[MAX_LEN], prezime[MAX_LEN];
    char trazeno[MAX_LEN];
    char imeDatoteke[MAX_LEN];
    int rez = 0;
    int god = 0;
    Position pronaden = NULL;

    while (izbor != 0) {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - dodaj na pocetak\n");
        printf("2 - ispisi listu\n");
        printf("3 - dodaj na kraj\n");
        printf("4 - pronadi po prezimenu\n");
        printf("5 - izbrisi po prezimenu\n");
        printf("6 - dodaj iza odredene osobe\n");
        printf("7 - dodaj ispred odredene osobe\n");
        printf("8 - sortiraj po prezimenu\n");
        printf("9 - upisi listu u datoteku\n");
        printf("10 - procitaj listu iz datoteke\n");
        printf("0 - izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos!\n");
            break;
        }

        switch (izbor) {
        case 1:
        case 3:
            printf("Unesi ime, prezime i godinu rodenja: ");
            if (scanf(" %49s %49s %d", ime, prezime, &god) != 3) {
                printf("Neispravan unos!\n");
                break;
            }
            if (izbor == 1)
                insertBegin(&Head, ime, prezime, god);
            else
                insertEnd(&Head, ime, prezime, god);
            break;

        case 2:
            printList(Head.next);   
            break;

        case 4:
            printf("Unesi prezime: ");
            scanf(" %49s", prezime);
            pronaden = findLastName(&Head, prezime);
            if (pronaden)
                printf("Pronaden: %s %s, %d\n", pronaden->name, pronaden->lastName, pronaden->year);
            else
                printf("Osoba s prezimenom %s ne postoji.\n", prezime);
            break;

        case 5:
            printf("Unesi prezime: ");
            scanf(" %49s", prezime);
            if (deletePerson(&Head, prezime) == 0)
                printf("Osoba izbrisana.\n");
            else
                printf("Osoba s prezimenom %s ne postoji.\n", prezime);
            break;

        case 6:
        case 7:
            printf(izbor == 6 ? "Iza koje osobe (prezime): " : "Ispred koje osobe (prezime): ");
            if (scanf(" %49s", trazeno) != 1)
                break;
            printf("Unesi ime, prezime i godinu rodenja nove osobe: ");
            if (scanf(" %49s %49s %d", ime, prezime, &god) != 3) {
                printf("Neispravan unos!\n");
                break;
            }
            if (izbor == 6)
                rez = insertNewAfter(&Head, trazeno, ime, prezime, god);
            else
                rez = insertBefore(&Head, trazeno, ime, prezime, god);

            if (rez == 0)
                printf("Osoba dodana.\n");
            else
                printf("Osoba s prezimenom %s ne postoji.\n", trazeno);
            break;

        case 8:
            sortFun(&Head);
            printf("Lista sortirana.\n");
            printList(Head.next);
            break;

        case 9:
            printf("Ime datoteke: ");
            scanf(" %49s", imeDatoteke);
            if (writeToFile(&Head, imeDatoteke) == 0)
                printf("Lista upisana u %s.\n", imeDatoteke);
            break;

        case 10:
            printf("Ime datoteke: ");
            scanf(" %49s", imeDatoteke);
            rez = readFromFile(&Head, imeDatoteke);
            if (rez >= 0)
                printf("Procitano osoba: %d\n", rez);
            break;

        case 0:
            break;

        default:
            printf("Nepoznat odabir.\n");
            break;
        }
    }

    freeList(&Head);
    return 0;
}

/* Alocira i popunjava novi element (zajednicko za insertBegin i insertEnd) */
Position createPerson(char* ime, char* prezime, int god)
{
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {               /* provjeri alokaciju! */
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    strcpy(newPerson->name, ime);
    strcpy(newPerson->lastName, prezime);
    newPerson->year = god;
    newPerson->next = NULL;

    return newPerson;
}

/* Umece newPerson odmah iza elementa p */
int insertAfter(Position p, Position newPerson)
{
    newPerson->next = p->next;
    p->next = newPerson;
    return 0;
}

/* dodaje novi element na pocetak liste (odmah iza head-a) */
int insertBegin(Position head, char* ime, char* prezime, int god)
{
    Position newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL)
        return -1;

    insertAfter(head, newPerson);
    return 0;
}

int printList(Position p)
{
    if (p == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    while (p != NULL) {
        printf("%s %s, %d\n", p->name, p->lastName, p->year);
        p = p->next;
    }
    return 0;
}

/* dodaje novi element na kraj liste */
int insertEnd(Position head, char* ime, char* prezime, int god)
{
    Position newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL)
        return -1;

    while (head->next != NULL)     
        head = head->next;

    insertAfter(head, newPerson);
    return 0;
}


Position findLastName(Position head, char* prezime)
{
    Position p = NULL;

    for (p = head->next; p != NULL; p = p->next) {
        if (strcmp(p->lastName, prezime) == 0)
            return p;
    }
    return NULL;
}

int deletePerson(Position head, char* prezime)
{
    Position prev = head;
    Position current = findLastName(head, prezime);

    if (current == NULL)
        return -1;

    while (prev->next != current)  /* trazimo element ispred onog kojeg brisemo */
        prev = prev->next;

    prev->next = current->next;    /* isto sto i prev->next->next */
    free(current);

    return 0;
}

/* Oslobada cijelu listu na kraju programa */
int freeList(Position head)
{
    Position temp = NULL;

    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    return 0;
}

/*  3. zad  */

/* Vraca element ISPRED onog s danim prezimenom (moze biti i head); NULL ako ga nema */
Position findPrevious(Position head, char* prezime)
{
    Position q = head;

    while (q->next != NULL) {
        if (strcmp(q->next->lastName, prezime) == 0)
            return q;
        q = q->next;
    }
    return NULL;
}

/*  dodaje novi element iza elementa s danim prezimenom */
int insertNewAfter(Position head, char* trazenoPrezime, char* ime, char* prezime, int god)
{
    Position newPerson = NULL;
    Position p = findLastName(head, trazenoPrezime);  

    if (p == NULL)
        return -1;

    newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL)
        return -2;

    insertAfter(p, newPerson);
    return 0;
}

/* dodaje novi element ispred elementa s danim prezimenom */
int insertBefore(Position head, char* trazenoPrezime, char* ime, char* prezime, int god)
{
    Position newPerson = NULL;
    Position prev = findPrevious(head, trazenoPrezime); /* element ispred trazenog */

    if (prev == NULL)
        return -1;

    newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL)
        return -2;

    insertAfter(prev, newPerson);   /* "iza prethodnog" = "ispred trazenog" */
    return 0;
}

/* sortira listu po prezimenima (zamjenom susjednih elemenata) */
int sortFun(Position head)
{
    Position q = head;          /* element ispred p */
    Position p = head->next;
    Position temp = NULL;

    if (p == NULL)              
        return 0;

    while (p->next != NULL) {
        if (strcmp(p->lastName, p->next->lastName) > 0) {
            /* zamijeni p i p->next:  q -> p -> temp  postaje  q -> temp -> p */
            temp = p->next;
            q->next = temp;
            p->next = temp->next;
            temp->next = p;

            q = head;           /* nakon zamjene krecemo ispocetka */
            p = q->next;
        }
        else {
            q = p;
            p = p->next;
        }
    }
    return 0;
}

/* upisuje listu u datoteku (jedna osoba po retku) */
int writeToFile(Position head, char* imeDatoteke)
{
    Position p = head->next;
    FILE* fp = fopen(imeDatoteke, "w");     

    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke %s!\n", imeDatoteke);
        return -1;
    }

    while (p != NULL) {
        fprintf(fp, "%s %s %d\n", p->name, p->lastName, p->year);
        p = p->next;
    }

    fclose(fp);                           
    return 0;
}

/* cita listu iz datoteke; osobe dodaje na kraj liste, vraca broj procitanih */
int readFromFile(Position head, char* imeDatoteke)
{
    char ime[MAX_LEN], prezime[MAX_LEN];
    int god = 0;
    int brojac = 0;
    FILE* fp = fopen(imeDatoteke, "r");     

    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke %s!\n", imeDatoteke);
        return -1;
    }

    while (fscanf(fp, " %49s %49s %d", ime, prezime, &god) == 3) {
        if (insertEnd(head, ime, prezime, god) != 0)
            break;
        brojac++;
    }

    fclose(fp);                            
    return brojac;
}
