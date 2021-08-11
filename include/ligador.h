//Tabela de Símbolos
typedef struct SymTable {
    char symbol[50];
    int mem_addr;
    struct SymTable *next;
}SymTable;

int get_address(char *symbol, SymTable *head);

int has_symbol(char symbol[], SymTable *head);

SymTable * add_address(char symbol[], int addr, SymTable *head);

void print_table(SymTable *head);

//Passos do ligador
SymTable * link_tables(int agrc, char *files[], SymTable *head);

void link_prog(int argc, char *argv[], SymTable *head);
