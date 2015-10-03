
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

#define INITIAL_SIZE 5
#define SCALING_FACTOR 2

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_symbol(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containing 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
    SymbolTable *table = (SymbolTable *) malloc(INITIAL_SIZE * sizeof(SymbolTable));

    if (table == NULL) {
      allocation_failed();
    }

    /** Allocate memory for the array containing symbols. **/
    table->tbl = (Symbol *) malloc(0);
    if (table->tbl == NULL) {
      allocation_failed();
    }

    table->len = 0;
    table->cap = INITIAL_SIZE;
    table->mode = mode;

    return table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    free(table->tbl);
    free(table);
}

/* A suggested helper function for copying the contents of a string. */
static char* create_copy_of_str(const char* str) {
    size_t len = strlen(str) + 1;
    char *buf = (char *) malloc(len);
    if (!buf) {
       allocation_failed();
    }
    strncpy(buf, str, len); 
    return buf;
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   ADDR is given as the byte offset from the first instruction. The SymbolTable
   must be able to resize itself as more elements are added. 

   Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
   return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists 
   in the table, you should call name_already_exists() and return -1. If memory
   allocation fails, you should call allocation_failed(). 

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
    if ((addr % 4) != 0) {
      addr_alignment_incorrect();
      return -1;
    }

    /** Creates a copy of NAME. **/
    char *copy_of_str = create_copy_of_str(name);

    /** Creates a copy of the symbols in TABLE. **/
    Symbol copy[table->len];
    int count;
    for(count = 0; count < table->len; count++) {
      Symbol current_symbol = table->tbl[count];

      /** If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists. **/
      int equal = strcmp(current_symbol.name, copy_of_str);
      if (equal == 0 && table->mode == SYMTBL_UNIQUE_NAME) {
        name_already_exists(copy_of_str);
        return -1;
      }

      copy[count] = current_symbol;
    }

    /** Increase the size of the symbols array by 1 and
        allocate more memeory to the array. **/
    table->len = table->len + 1;
    table->tbl = realloc(table->tbl, (table->len * sizeof(Symbol)));
    if (table->tbl == NULL) {
      allocation_failed();
    }

    /** Now that we have resized our symbols array, add all 
        of the symbols back into the resized array. **/
    int i = 0;
    while (i < table->len - 1) {
      table->tbl[i] = copy[i];
    }

    /** Add the new symbol to the end of the array. **/
    Symbol new_symbol = {copy_of_str, addr};
    table->tbl[table->len - 1] = new_symbol;

    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    int count;
    for (count = 0; count < table->len; count++) {
      Symbol current_symbol = table->tbl[count];
      if (strcmp(current_symbol.name, name) == 0) {
        return current_symbol.addr;
      }
    }
    return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    /* YOUR CODE HERE */
}
