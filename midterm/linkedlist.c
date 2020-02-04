#include <stdio.h> 
#include <stdlib.h> 

typedef struct listelement
{
    struct listelement *prev, *next;
    int number;
}listelement;

listelement * head = NULL;
void delete_list();
void new_element(int input);
void printlist();
void remove_element(int d);

int main(){
    new_element(5);
    new_element(6);
    //new_element(9);
    printlist();
    //remove_element(9);
    //printlist();
    //delete_list();
    //printlist();
}

void new_element(int input){
    listelement *element = (listelement*)malloc(sizeof(listelement));
    if(head == NULL){
        head = element;
        element->number = input;
        return;
    }
    element->next = head;
    head->prev = element;
    head = element;
    element->number = input;
    return;
}

void printlist(){
    if(head == NULL){
        printf("nothing in the list");
        return;
    }
    listelement *element = head;
    for(;element != 0; element = element->next ){
        printf("Data: %d\n", element->number);
        printf("Prev: %x\n", element->prev);
        printf("Next: %x\n", element->next);
    } 
}

void remove_element(int d){
    listelement *element = head;
    for(;element != 0; element = element->next ){
        if(element->number == d && element->next == 0 && element->prev == 0){
            free(element);
            head = 0;
            return;
        }
        if(element->number == d && element->next == 0){
            element->prev->next = 0;
            free(element);
            return;
        }
        else if(element->number == d && element->prev == 0){
            element->next->prev = 0;
            head = element->next;
            free(element);
            return;
        }
        else if(element->number == d && element->prev != 0 && element->next != 0){
            element->prev->next = element->next;
            element->next->prev = element->prev;
            free(element);
            return;
            
        }
    }
    printf("Not in list\n");
    return;
}

void delete_list(){
    listelement *element = head;
    while(element != 0){
        listelement *temp = element->next;
        free(element);
        element = temp;
    }
    head = NULL;
}