/**
 * @file user.c
 * @brief Implementation of user-related functions for vaccine management
 * @author ist1113656 (Taha Adar Ozsoy)
 */

#include "user.h"
#include "vacina.h"

#define BUFFER_SIZE 65535

#include <string.h>

/**
 * @brief Creates a new user vaccination record
 * 
 * Allocates memory for a user record and copies the provided information.
 * 
 * @param nome User's name
 * @param vacina Name of the vaccine
 * @param lote Batch identifier used
 * @param dia Day of application
 * @param mes Month of application
 * @param ano Year of application
 * @return Pointer to the newly created user record
 */
User* criar_user(char* nome, char* vacina, char* lote,
                int dia, int mes, int ano) {
    User* novo_user = (User*)malloc(sizeof(User));

    if (!novo_user) {
        printf("No memory\n");
        exit(1);
    }

    size_t nome_len = strlen(nome);
    novo_user->nome = (char*)malloc(strlen(nome) + 1);
    if (!novo_user->nome) {
        free(novo_user);
        printf("No memory\n");
        exit(1);
    }
    
    memcpy(novo_user->nome, nome, nome_len + 1);
    memcpy(novo_user->nome_vacina, vacina, strlen(vacina) + 1);
    memcpy(novo_user->lote_usado, lote, strlen(lote) + 1);

    novo_user->dia_de_applicacao = dia;
    novo_user->mes_de_aplicacao = mes;
    novo_user->ano_de_aplicacao = ano;
    novo_user->data_aplicacao_int = ddmmyy_int(dia, mes, ano);
    novo_user->next = NULL;

    return novo_user;
}

/**
 * @brief Records a vaccine application for a user
 * 
 * Creates a new user record and adds it to the beginning of the linked list.
 * 
 * @param head Pointer to the head pointer of the user list
 * @param nome User's name
 * @param vacina Name of the vaccine
 * @param lote Batch identifier used
 * @param dia Day of application
 * @param mes Month of application
 * @param ano Year of application
 */
void aplicar_vacina(User** head, char* nome, char* vacina,
                   char* lote, int dia, int mes, int ano) {
    User* novo_user = criar_user(nome, vacina, lote, dia, mes, ano);
    novo_user->next = *head;  
    *head = novo_user;
}

/**
 * @brief Frees all memory allocated for user records
 * 
 * Traverses the user linked list and releases memory for each node.
 * 
 * @param head Pointer to the head of the user list
 */
void free_users(User* head) {
    User* current = head;
    while (current) {
        User* temp = current;
        current = current->next;
        free(temp->nome);
        free(temp);
    }
}

/**
 * @brief Creates a new hash table for tracking today's vaccinations
 * 
 * Initializes the hash table with empty buckets and the current date.
 * 
 * @param day Current day
 * @param month Current month
 * @param year Current year
 * @return Pointer to the newly created hash table
 */
HojeVaxTable* criar_vax_table_hoje(int day, int month, int year) {
    int i;
    HojeVaxTable* table = (HojeVaxTable*)malloc(sizeof(HojeVaxTable));
    if (!table) {
        printf("No memory\n");
        exit(1);
    }
    
    for (i = 0; i < HASH_SIZE; i++) {
        table->table[i] = NULL;
    }
    
    table->current_day = day;
    table->current_month = month;
    table->current_year = year;
    
    return table;
}

/**
 * @brief Frees all memory allocated for the today's vaccination hash table
 * 
 * Releases memory for all records in each bucket and the table itself.
 * 
 * @param table Pointer to the hash table
 */
void free_hoje_vax_table(HojeVaxTable* table) {
    int i;
    if (!table) return;
    
    for (i = 0; i < HASH_SIZE; i++) {
        VaxHoje* current = table->table[i];
        while (current) {
            VaxHoje* temp = current;
            current = current->next;
            free(temp->nome_user);
            free(temp->nome_vacina);
            free(temp);
        }
    }
    
    free(table);
}

/**
 * @brief Computes a hash value for the user-vaccine pair
 * 
 * Generates a hash index in the range [0, HASH_SIZE-1] using
 * a simple multiplication-based hash function.
 * 
 * @param nome_user User's name
 * @param nome_vacina Name of the vaccine
 * @return Hash value
 */
static unsigned int hash_function(const char* nome_user,
                                 const char* nome_vacina) {
    unsigned int hash = 0;
    
    while (*nome_user) {
        hash = (hash * 31) + (*nome_user++);
    }
    
    while (*nome_vacina) {
        hash = (hash * 31) + (*nome_vacina++);
    }
    
    return hash % HASH_SIZE;
}

/**
 * @brief Checks if a user has been vaccinated today with a specific vaccine
 * 
 * Searches the hash table for a record matching the user-vaccine pair.
 * 
 * @param table Pointer to the hash table
 * @param nome_user User's name
 * @param nome_vacina Name of the vaccine
 * @return 1 if the user was vaccinated today 
 * with the given vaccine, 0 otherwise
 */
int eh_vacinado_hoje(HojeVaxTable* table, char* nome_user, char* nome_vacina) {
    unsigned int index = hash_function(nome_user, nome_vacina);
    
    VaxHoje* current = table->table[index];
    while (current) {
        if (strcmp(current->nome_user, nome_user) == 0 && 
            strcmp(current->nome_vacina, nome_vacina) == 0) {
            return 1;
        }
        current = current->next;
    }
    
    return 0; 
}

/**
 * @brief Records a vaccination performed today
 * 
 * Creates a new record in the hash table for the user-vaccine pair.
 * 
 * @param table Pointer to the hash table
 * @param nome_user User's name
 * @param nome_vacina Name of the vaccine
 */
void recorde_vacinacao_hoje(HojeVaxTable* table, char* nome_user,
                           char* nome_vacina) {
    unsigned int index = hash_function(nome_user, nome_vacina);
    char* name_copy;
    char* vaccine_copy;
    
    VaxHoje* new_record = (VaxHoje*)malloc(sizeof(VaxHoje));
    if (!new_record) {
        printf("No memory\n");
        exit(1);
    }
    
    name_copy = (char*)malloc(strlen(nome_user) + 1);
    vaccine_copy = (char*)malloc(strlen(nome_vacina) + 1);
    
    if (!name_copy || !vaccine_copy) {
        free(new_record);
        if (name_copy) free(name_copy);
        if (vaccine_copy) free(vaccine_copy);
        printf("No memory\n");
        exit(1);
    }
    
    strcpy(name_copy, nome_user);
    strcpy(vaccine_copy, nome_vacina);
    
    new_record->nome_user = name_copy;
    new_record->nome_vacina = vaccine_copy;
    
    new_record->next = table->table[index];
    table->table[index] = new_record;
}

/**
 * @brief Resets the hash table if the current date has changed
 * 
 * Clears all records in the hash table and updates the current date.
 * 
 * @param table Pointer to the hash table
 * @param day New current day
 * @param month New current month
 * @param year New current year
 */
void reset_table_hoje(HojeVaxTable* table, int day, int month, int year) {
    int i;
    if (table->current_day != day || 
        table->current_month != month || 
        table->current_year != year) {
        
        for (i = 0; i < HASH_SIZE; i++) {
            VaxHoje* current = table->table[i];
            while (current) {
                VaxHoje* temp = current;
                current = current->next;
                free(temp->nome_user);
                free(temp->nome_vacina);
                free(temp);
            }
            table->table[i] = NULL;
        }
        
        table->current_day = day;
        table->current_month = month;
        table->current_year = year;
    }
}