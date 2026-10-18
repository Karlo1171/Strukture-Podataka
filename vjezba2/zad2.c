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

int main(void)
{
    /* Head ne sadrzi podatke, samo pokazuje na prvi pravi element */
    Person Head = { .name = {0}, .lastName = {0}, .year = 0, .next = NULL };

    int izbor = -1;
    char ime[MAX_LEN], prezime[MAX_LEN];
    int god = 0;
    Position pronaden = NULL;

    while (izbor != 0) {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - dodaj na pocetak\n");
        printf("2 - ispisi listu\n");
        printf("3 - dodaj na kraj\n");
        printf("4 - pronadi po prezimenu\n");
        printf("5 - izbrisi po prezimenu\n");
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
            printList(Head.next);   /* preskacemo head, ispisujemo od prvog pravog elementa */
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

/* ispisuje listu - prima prvi pravi element (head->next) */
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

/*  dodaje novi element na kraj liste */
int insertEnd(Position head, char* ime, char* prezime, int god)
{
    Position newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL)
        return -1;

    while (head->next != NULL)     /* dolazimo do zadnjeg elementa */
        head = head->next;

    insertAfter(head, newPerson);
    return 0;
}

/*  pronalazi element po prezimenu; vraca NULL ako ga nema */
Position findLastName(Position head, char* prezime)
{
    Position p = NULL;

    for (p = head->next; p != NULL; p = p->next) {
        if (strcmp(p->lastName, prezime) == 0)
            return p;
    }
    return NULL;
}

/* brise element s danim prezimenom */
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