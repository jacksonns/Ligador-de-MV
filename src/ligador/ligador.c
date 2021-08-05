#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ligador.h"

// FUNÇÕES CONSTRUTORAS DA TABELA
// Retorna endereço inteiro de um dado símbolo
int get_address(char *symbol, SymTable *head){
    SymTable *aux = head;
    int addr;
    while(aux != NULL)
    {
        if(strcmp(symbol,aux->symbol) == 0)
        {
            addr = aux->mem_addr;
            break;
        }
        aux = aux->next;
    }
    return addr;
}

// Retorna 1 se tem o símbolo e 0 caso contrário
int has_symbol(char symbol[], SymTable *head){
    SymTable *aux = head;
    while (aux != NULL){
        if(strcmp(symbol,aux->symbol) == 0) return 1;
        aux = aux->next;
    }
    return 0;
}

// Adiciona endereço a um símbolo na tabela. Se não acha, cria novo símbolo
SymTable * add_address(char symbol[], int addr, SymTable *head){
    SymTable *aux = head;
    SymTable *new;
    if(has_symbol(symbol, head) == 1){
        while(aux != NULL){
            if(strcmp(symbol,aux->symbol) == 0){
                aux->mem_addr = addr;
            }
            aux = aux->next;
        }
    }
    else {
        new = (SymTable *) malloc(sizeof(SymTable));
        while (aux != NULL && aux->next != NULL) {
            aux = aux->next;
        }
        strcpy(new->symbol, symbol);
        new->mem_addr = addr;
        new->next= NULL;
        if (aux != NULL)
            aux->next = new;
        else
            head = new;
    }
    return head;
}

void print_table(SymTable *head){
    SymTable *aux = head;
    while (aux != NULL){
        printf("%s %d\n", aux->symbol, aux->mem_addr);
        aux = aux->next;
    }
}


/* Lê tabela de símbolos dos arquivos fornecidos e 
junta em uma só */
SymTable * link_tables(int argc, char *argv[], SymTable *head){
    FILE *file;
    char line[100];
    char *word, *symbol;
    int addr, prog_size = 0;
    // Itera sobre cada arquivo.
    for (int i = 1; i < argc; i++){
        file = fopen(argv[i], "rt");

        while(fgets(line, 100, file) != NULL){
            line[strcspn(line, "\r\n")] = 0;
            word = strtok(line, " ");
            if (word != NULL){
                if (strcmp(word, "size:") == 0){
                    word = strtok(NULL, " ");
                    int size = atoi(word);
                    prog_size += size;
                    break;
                } else if (strcmp(word, "prog:") == 0){
                    break;
                } else {
                    symbol = word;
                    word = strtok(NULL, " ");
                    addr = atoi(word);
                    if (!has_symbol(symbol, head)){
                        if (addr == -1) head = add_address(symbol, addr, head);
                        else head = add_address(symbol, addr + prog_size, head);
                    }
                    else {
                        if (get_address(symbol, head) == -1 && addr != -1) {
                            head = add_address(symbol, addr + prog_size, head);
                        }
                    }
                }
            }
        }

        fclose(file);
    }
    printf("TABELA: \n");
    print_table(head);
    return head;
}

void link_prog(int argc, char *argv[], SymTable *head){
    FILE *file;
    char line[100];
    char *word;
    int instr, mem_addr = 0;

    // Itera sobre cada arquivo.
    for (int i = 1; i < argc; i++){
        file = fopen(argv[i], "rt");
        // Ignora arquivo até achar label "prog"
        while(fgets(line, 100, file) != NULL){
            line[strcspn(line, "\r\n")] = 0;
            word = strtok(line, " ");
            if (word != NULL && strcmp(word, "prog:") == 0) break;
        }

        while(fscanf(file, "%s", word) != EOF){
            instr = atoi(word);
            printf("%d ", instr);
            mem_addr++;

            if ((instr >= 1) && (instr <= 15)) { //Instruções com pelo menos um registrador
                fscanf(file, "%s", word); //Próxima palavra
                printf("%s ", word);
                mem_addr ++;

                if ((instr == 5) || ((instr >= 8) && (instr <= 14))) { //Intruções com dois registradores
                    fscanf(file, "%s", word); //Próxima palavra
                    printf("%s ", word);
                    mem_addr++;
                } else if ((instr == 1) || (instr == 2)) { //Instruções com resgistrador e memória
                    fscanf(file, "%s", word); //Próxima palavra   
                    mem_addr++;   
                    printf("%d ", get_address(word, head) - mem_addr);
                }

            } else if ((instr >= 16) && (instr <= 19)) { //Instruções só com memória
                fscanf(file, "%s", word); //Próxima palavra
                mem_addr ++;
                printf("%d ", get_address(word, head) - mem_addr);
            }
        } 
        fclose(file);
    }
}