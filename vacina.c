/**
 * @file vacina.c
 * @brief Implementation of vaccine-related data structures and functions
 * @author ist1113656 (Taha Adar Ozsoy)
 */
#include "vacina.h"

/**
 * @brief Converts date components to a single integer representation
 * 
 * @param dia Day component of the date
 * @param mes Month component of the date
 * @param ano Year component of the date
 * @return Integer representation of the date
 * in format YYYYMMDD for easy comparison
 */
int ddmmyy_int(int dia, int mes, int ano) {
    return ano * 10000 + mes * 100 + dia;
}

/**
 * @brief Creates a new vaccine batch with the provided information
 * 
 * @param nome Name of the vaccine
 * @param lote Batch identifier
 * @param dia Expiration day
 * @param mes Expiration month
 * @param ano Expiration year
 * @param dosas Number of available doses
 * @return Pointer to the newly created batch structure
 */
LoteVacina* criar_lote(char* nome, char* lote, int dia, int mes,
                      int ano, int dosas){
    LoteVacina* novo_lote = (LoteVacina*)malloc(sizeof(LoteVacina));

    if (!novo_lote) {
        printf("No memory\n");
        exit(1);
    }

    strcpy(novo_lote->nome, nome);
    strcpy(novo_lote->lote, lote);
    novo_lote->dia_de_expiracao = dia;
    novo_lote->mes_de_expiracao = mes;
    novo_lote->ano_de_expiracao = ano;
    novo_lote->dosas_disponiveis = dosas;
    novo_lote->total_aplicacoes = 0;
    novo_lote->data_expiracao_int = ddmmyy_int(dia, mes, ano);
    novo_lote->next = NULL;

    return novo_lote; 
}

/**
 * @brief Adds a new batch to the beginning of the linked list
 * 
 * @param head Pointer to the head pointer of the batch list
 * @param novo_lote Pointer to the new batch to be added
 */
void adicionar_lote(LoteVacina** head, LoteVacina* novo_lote) {
    novo_lote->next = *head; 
    *head = novo_lote;  
}

/**
 * @brief Frees all memory allocated for the batches in the linked list
 * @param head Pointer to the head of the batch list
 */
void free_lotes(LoteVacina* head) {
    LoteVacina* current = head;
    while (current != NULL) {
        LoteVacina* temp = current; 
        current = current->next;       
        free(temp);                     
    }
}