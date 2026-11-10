#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1024

typedef struct _StackElement* Position;
typedef struct _StackElement {
    double number;
    Position next;
} StackElement;

Position createElement(double number);
int push(Position head, double number);
int pop(Position head, double* number);
int deleteAll(Position head);
int readNumber(char* buffer, int* i, double* number);
int calculate(Position head, char operation);
int calculatePostfix(char* buffer, double* result);
int readFile(char* imeDatoteke, char* buffer);

int main(void)
{
    char imeDatoteke[MAX_LINE] = { 0 };
    char buffer[MAX_LINE] = { 0 };
    double result = 0;

    printf("Ime datoteke s postfiks izrazom: ");
    if (scanf(" %1023s", imeDatoteke) != 1)
        return -1;

    if (readFile(imeDatoteke, buffer) != 0)
        return -1;

    printf("Izraz: %s\n", buffer);

    if (calculatePostfix(buffer, &result) != 0)
        return -1;

    printf("Rezultat: %g\n", result);
    return 0;
}

Position createElement(double number)
{
    Position newElement = (Position)malloc(sizeof(StackElement));
    if (newElement == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    newElement->number = number;
    newElement->next = NULL;
    return newElement;
}

int push(Position head, double number)
{
    Position newElement = createElement(number);
    if (newElement == NULL)
        return -1;

    newElement->next = head->next;
    head->next = newElement;
    return 0;
}

int pop(Position head, double* number)
{
    Position first = head->next;

    if (first == NULL) {
        printf("Greska: stog je prazan, izraz nije ispravan!\n");
        return -1;
    }

    *number = first->number;
    head->next = first->next;
    free(first);
    return 0;
}

int deleteAll(Position head)
{
    double temp = 0;
    while (head->next != NULL)
        pop(head, &temp);
    return 0;
}

int readNumber(char* buffer, int* i, double* number)
{
    double rez = 0;
    double dec = 0.1;
    int predznak = 1;

    if (buffer[*i] == '-') {
        predznak = -1;
        (*i)++;
    }

    while (buffer[*i] >= '0' && buffer[*i] <= '9') {
        rez = rez * 10 + (buffer[*i] - '0');
        (*i)++;
    }

    if (buffer[*i] == '.') {
        (*i)++;
        while (buffer[*i] >= '0' && buffer[*i] <= '9') {
            rez = rez + (buffer[*i] - '0') * dec;
            dec = dec / 10;
            (*i)++;
        }
    }

    *number = predznak * rez;
    return 0;
}

int calculate(Position head, char operation)
{
    double first = 0, second = 0, result = 0;

    if (pop(head, &second) != 0)
        return -1;
    if (pop(head, &first) != 0)
        return -1;

    switch (operation) {
    case '+':
        result = first + second;
        break;
    case '-':
        result = first - second;
        break;
    case '*':
        result = first * second;
        break;
    case '/':
        if (second == 0) {
            printf("Greska: dijeljenje s nulom!\n");
            return -1;
        }
        result = first / second;
        break;
    default:
        return -1;
    }

    return push(head, result);
}

int calculatePostfix(char* buffer, double* result)
{
    StackElement head = { .number = 0, .next = NULL };
    int i = 0;
    char c = 0;
    double number = 0;

    while (buffer[i] != '\0') {
        c = buffer[i];

        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            i++;
        }
        else if ((c >= '0' && c <= '9') ||
                 (c == '-' && buffer[i + 1] >= '0' && buffer[i + 1] <= '9')) {
            readNumber(buffer, &i, &number);
            if (push(&head, number) != 0) {
                deleteAll(&head);
                return -1;
            }
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (calculate(&head, c) != 0) {
                deleteAll(&head);
                return -1;
            }
            i++;
        }
        else {
            printf("Greska: neispravan znak '%c' u izrazu!\n", c);
            deleteAll(&head);
            return -1;
        }
    }

    if (pop(&head, result) != 0)
        return -1;

    if (head.next != NULL) {
        printf("Greska: na stogu je ostalo vise brojeva, izraz nije ispravan!\n");
        deleteAll(&head);
        return -1;
    }

    return 0;
}

int readFile(char* imeDatoteke, char* buffer)
{
    int i = 0;
    FILE* fp = fopen(imeDatoteke, "r");

    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke %s!\n", imeDatoteke);
        return -1;
    }

    if (fgets(buffer, MAX_LINE, fp) == NULL) {
        printf("Datoteka %s je prazna!\n", imeDatoteke);
        fclose(fp);
        return -1;
    }

    fclose(fp);

    while (buffer[i] != '\0' && buffer[i] != '\n' && buffer[i] != '\r')
        i++;
    buffer[i] = '\0';

    return 0;
}
