/**
 * @file user.h
 * @brief Header file defining user-related data structures and functions
 * @author ist1113656 (Taha Adar Ozsoy)
 */
#ifndef USER_H
#define USER_H

/**< Maximum buffer size for input processing */
#define BUFFER_SIZE 65535 

/**< Size of hash table for today's vaccinations */
#define HASH_SIZE 1009 

/**< Maximum length of the name of the vacine 
* that can be stored in the system as data */
#define MAX_NOME 51 

/**< Maximum length of the name of the vacine batch
*  that can be stored in the system as data*/
#define MAX_NOME_LOTE 21 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Represents a vaccination record for a user
 */
typedef struct User {
    char* nome;   /**< User's name (dynamically allocated) */
    char nome_vacina[MAX_NOME];  /**< Name of the vaccine administered */

    /**< Batch identifier used for vaccination */
    char lote_usado[MAX_NOME_LOTE];

    int dia_de_applicacao;                /**< Day of application */
    int mes_de_aplicacao;                 /**< Month of application */
    int ano_de_aplicacao;                 /**< Year of application */
    int data_aplicacao_int; /**< Integer representation of application date */
    struct User* next;  /**< Pointer to next user in linked list */
} User;

/**
 * @brief Creates a new user vaccination record
 * @param nome User's name
 * @param vacina Name of the vaccine
 * @param lote Batch identifier used
 * @param dia Day of application
 * @param mes Month of application
 * @param ano Year of application
 * @return Pointer to the newly created user record
 */
User* criar_user(char* nome, char* vacina, char* lote,
                 int dia, int mes, int ano);

/**
 * @brief Records a vaccine application for a user
 * @param head Pointer to the head pointer of the user list
 * @param nome User's name
 * @param vacina Name of the vaccine
 * @param lote Batch identifier used
 * @param dia Day of application
 * @param mes Month of application
 * @param ano Year of application
 */
void aplicar_vacina(User** head, char* nome, char* vacina,
                   char* lote, int dia, int mes, int ano);

/**
 * @brief Frees all memory allocated for user records
 * @param head Pointer to the head of the user list
 */
void free_users(User* head);

/**
 * @brief Represents a record of vaccination performed today
 */
typedef struct VaxHoje {
    char* nome_user;          /**< User's name (dynamically allocated) */

    /**< Name of the vaccine (dynamically allocated) */
    char* nome_vacina;

    struct VaxHoje* next;     /**< Pointer to next record in hash bucket */
} VaxHoje;

/**
 * @brief Hash table to track vaccinations performed on the current day
 */
typedef struct {
    VaxHoje* table[HASH_SIZE];
    int current_day;
    int current_month;
    int current_year;
} HojeVaxTable;

/**
 * @brief Creates a new hash table for tracking today's vaccinations
 * @param day Current day
 * @param month Current month
 * @param year Current year
 * @return Pointer to the newly created hash table
 */
HojeVaxTable* criar_vax_table_hoje(int day, int month, int year);

/**
 * @brief Frees all memory allocated for the today's vaccination hash table
 * @param table Pointer to the hash table
 */
void free_hoje_vax_table(HojeVaxTable* table);

/**
 * @brief Checks if a user has been vaccinated today with a specific vaccine
 * @param table Pointer to the hash table
 * @param nome_user User's name
 * @param nome_vacina Name of the vaccine
 * @return 1 if the user was vaccinated today
 * with the given vaccine, 0 otherwise
 */
int eh_vacinado_hoje(HojeVaxTable* table, char* nome_user, char* nome_vacina);

/**
 * @brief Records a vaccination performed today
 * @param table Pointer to the hash table
 * @param nome_user User's name
 * @param nome_vacina Name of the vaccine
 */
void recorde_vacinacao_hoje(HojeVaxTable* table, char* nome_user,
                           char* nome_vacina);

/**
 * @brief Resets the hash table if the current date has changed
 * @param table Pointer to the hash table
 * @param day New current day
 * @param month New current month
 * @param year New current year
 */
void reset_table_hoje(HojeVaxTable* table, int day, int month, int year);

#endif