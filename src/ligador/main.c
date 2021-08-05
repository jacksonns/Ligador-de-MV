#include <stdio.h>
#include <stdlib.h>

#include "ligador.h"

int main(int argc, char *argv[]) {
    FILE *file;
    if(argc == 1){
        printf("Forneça no mínimo um arquivo!\n");
        return 1;
    } else{
        for (int i = 1; i < argc; i++){
            file = fopen(argv[i], "rt");
            if (file == NULL) {
                printf("Forneça arquivo(s) válido(s)!\n");
                return 1;
            }
            fclose(file);
        }
    }

    SymTable *head = NULL;
    head = link_tables(argc, argv, head);
    link_prog(argc, argv, head);

    return 0;
}
