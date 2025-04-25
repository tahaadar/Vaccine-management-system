/**
 * @file vacina.h
 * @brief Header file containing vaccine data structures and related functions
 * @author ist1113656 (Taha Adar Ozsoy)
 */

#ifndef VACINA_H
#define VACINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**< Maximum number of vaccine batches that can be stored in the system */
#define MAX_LOTES 1000

/**< Maximum length of the name of the vacine
 *  that can be stored in the system as data */
#define MAX_NOME 51 

/**< Maximum length of the name of the vacine batch
 *  that can be stored in the system as data*/
#define MAX_NOME_LOTE 21

/**
 * @brief Represents a vaccine batch
 */
typedef struct LoteVacina {
    char nome[MAX_NOME];            /**< Name of the vaccine */
    char lote[MAX_NOME_LOTE];       /**< Batch identifier in hexadecimal */
    int dia_de_expiracao;           /**< Expiration day */
    int mes_de_expiracao;           /**< Expiration month */
    int ano_de_expiracao;           /**< Expiration year */
    int dosas_disponiveis;          /**< Number of available doses */
    int total_aplicacoes;           /**< Number of applied doses */

    /**< Integer representation of expiration date */
    int data_expiracao_int;

    struct LoteVacina* next;        /**< Pointer to next batch in linked list */
} LoteVacina;

/**
 * @brief Converts date components to a single integer representation
 * @param dia Day component of the date
 * @param mes Month component of the date
 * @param ano Year component of the date
 * @return Integer representation of the date in format YYYYMMDD
 */
int ddmmyy_int(int dia, int mes, int ano);

/**
 * @brief Creates a new vaccine batch
 * @param nome Name of the vaccine
 * @param lote Batch identifier
 * @param dia Expiration day
 * @param mes Expiration month
 * @param ano Expiration year
 * @param dosas Number of available doses
 * @return Pointer to the newly created batch
 */
LoteVacina* criar_lote(char* nome, char* lote, int dia,
                      int mes, int ano, int dosas);

/**
 * @brief Adds a new batch to the beginning of the linked list
 * @param head Pointer to the head pointer of the batch list
 * @param novo_lote Pointer to the new batch to be added
 */
void adicionar_lote(LoteVacina** head, LoteVacina* novo_lote);

/**
 * @brief Frees all memory allocated for the batches in the linked list
 * @param head Pointer to the head of the batch list
 */
void free_lotes(LoteVacina* head);

#endif 