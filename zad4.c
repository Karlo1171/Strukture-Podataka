#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct _Pol* Position;
typedef struct _Pol {
    int coef;
    int pow;
    Position next;
} polinom;

Position createElement(int coef, int pow);
int insertBegin(Position q, Position newPol);
int deleteAfter(Position q);
int insertSorted(Position head, Position newPol);
int readNumber(char* buffer, int* i, int* broj);
int parseLine(Position head, char* buffer);
int readFile(char* imeDatoteke, Position headF, Position headS);
int printPoly(char* naziv, Position head);
int addPoly(Position headR, Position headF, Position headS);
int multiplyPoly(Position headR, Position headF, Position headS);
int freePoly(Position head);

int main(void)
{
    polinom headF = { .coef = 0, .pow = 0, .next = NULL };
    polinom headS = { .coef = 0, .pow = 0, .next = NULL };
    polinom headSum = { .coef = 0, .pow = 0, .next = NULL };
    polinom headMul = { .coef = 0, .pow = 0, .next = NULL };
    char imeDatoteke[MAX_LINE] = { 0 };

    printf("Ime datoteke s polinomima: ");
    if (scanf(" %1023s", imeDatoteke) != 1)
        return -1;

    if (readFile(imeDatoteke, &headF, &headS) != 0) {
        freePoly(&headF);
        freePoly(&headS);
        return -1;
    }

    printPoly("P1(x)      ", &headF);
    printPoly("P2(x)      ", &headS);

    if (addPoly(&headSum, &headF, &headS) == 0)
        printPoly("P1 + P2    ", &headSum);
    if (multiplyPoly(&headMul, &headF, &headS) == 0)
        printPoly("P1 * P2    ", &headMul);

    freePoly(&headF);
    freePoly(&headS);
    freePoly(&headSum);
    freePoly(&headMul);
    return 0;
}

Position createElement(int coef, int pow)
{
    Position newPol = (Position)malloc(sizeof(polinom));
    if (newPol == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    newPol->coef = coef;
    newPol->pow = pow;
    newPol->next = NULL;
    return newPol;
}

int insertBegin(Position q, Position newPol)
{
    newPol->next = q->next;
    q->next = newPol;
    return 0;
}

int deleteAfter(Position q)
{
    Position temp = q->next;
    if (temp == NULL)
        return -1;
    q->next = temp->next;
    free(temp);
    return 0;
}

int insertSorted(Position head, Position newPol)
{
    Position q = head;

    while (q->next != NULL && newPol->pow < q->next->pow)
        q = q->next;

    if (q->next != NULL && newPol->pow == q->next->pow) {
        q->next->coef += newPol->coef;
        free(newPol);
        if (q->next->coef == 0)
            deleteAfter(q);
    }
    else {
        insertBegin(q, newPol);
    }
    return 0;
}

int readNumber(char* buffer, int* i, int* broj)
{
    int predznak = 1;
    int rez = 0;

    while (buffer[*i] == ' ' || buffer[*i] == '\t')
        (*i)++;

    if (buffer[*i] == '\0' || buffer[*i] == '\n' || buffer[*i] == '\r')
        return 0;

    if (buffer[*i] == '-' || buffer[*i] == '+') {
        if (buffer[*i] == '-')
            predznak = -1;
        (*i)++;
    }

    if (buffer[*i] < '0' || buffer[*i] > '9')
        return -1;

    while (buffer[*i] >= '0' && buffer[*i] <= '9') {
        rez = rez * 10 + (buffer[*i] - '0');
        (*i)++;
    }

    *broj = predznak * rez;
    return 1;
}

int parseLine(Position head, char* buffer)
{
    int i = 0;
    int coef = 0, pow = 0, r = 0;
    Position newPol = NULL;

    while (1) {
        r = readNumber(buffer, &i, &coef);
        if (r == 0)
            break;
        if (r == -1) {
            printf("Neispravan znak u retku: %s", buffer);
            return -1;
        }

        r = readNumber(buffer, &i, &pow);
        if (r != 1) {
            printf("Neispravan format retka: %s", buffer);
            return -1;
        }

        if (coef == 0)
            continue;

        newPol = createElement(coef, pow);
        if (newPol == NULL)
            return -1;
        insertSorted(head, newPol);
    }
    return 0;
}

int readFile(char* imeDatoteke, Position headF, Position headS)
{
    char buffer[MAX_LINE] = { 0 };
    FILE* fp = fopen(imeDatoteke, "r");

    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke %s!\n", imeDatoteke);
        return -1;
    }

    if (fgets(buffer, MAX_LINE, fp) == NULL || parseLine(headF, buffer) != 0) {
        printf("Greska pri citanju prvog polinoma.\n");
        fclose(fp);
        return -1;
    }
    if (fgets(buffer, MAX_LINE, fp) == NULL || parseLine(headS, buffer) != 0) {
        printf("Greska pri citanju drugog polinoma.\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int printPoly(char* naziv, Position head)
{
    Position p = head->next;
    int prvi = 1;
    int c = 0;

    printf("%s= ", naziv);

    if (p == NULL) {
        printf("0\n");
        return 0;
    }

    while (p != NULL) {
        c = p->coef;

        if (prvi) {
            if (c < 0) printf("-");
        }
        else {
            printf(c < 0 ? " - " : " + ");
        }
        if (c < 0) c = -c;

        if (p->pow == 0)
            printf("%d", c);
        else {
            if (c != 1) printf("%d", c);
            if (p->pow == 1) printf("x");
            else printf("x^%d", p->pow);
        }

        prvi = 0;
        p = p->next;
    }
    printf("\n");
    return 0;
}

int addPoly(Position headR, Position headF, Position headS)
{
    Position p = NULL;
    Position newPol = NULL;

    for (p = headF->next; p != NULL; p = p->next) {
        newPol = createElement(p->coef, p->pow);
        if (newPol == NULL) return -1;
        insertSorted(headR, newPol);
    }
    for (p = headS->next; p != NULL; p = p->next) {
        newPol = createElement(p->coef, p->pow);
        if (newPol == NULL) return -1;
        insertSorted(headR, newPol);
    }
    return 0;
}

int multiplyPoly(Position headR, Position headF, Position headS)
{
    Position p = NULL, q = NULL;
    Position newPol = NULL;

    for (p = headF->next; p != NULL; p = p->next) {
        for (q = headS->next; q != NULL; q = q->next) {
            newPol = createElement(p->coef * q->coef, p->pow + q->pow);
            if (newPol == NULL) return -1;
            insertSorted(headR, newPol);
        }
    }
    return 0;
}

int freePoly(Position head)
{
    while (head->next != NULL)
        deleteAfter(head);
    return 0;
}
