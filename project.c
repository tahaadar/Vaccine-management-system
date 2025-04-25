/* iaed25 - ist1113656 - project */
/**
 * @file project.c
 * @brief Main implementation file for the vaccine management system
 * @author ist1113656 (Taha Adar Ozsoy)
 */

#include "vacina.h"
#include "user.h"

 /**< Max length for batch name string incl. null byte */
#define NOME_LOTE_MAX 21

/**< Max length for vaccine name string incl. null byte */
#define NOME_VACINA_MAX 51  

/**< Buffer size for reading input lines */
#define BUFFER_SIZE 65535

/**< System limit for number of vaccine batches */
#define MAX_LOTES 1000       

/**
 * @brief Checks if the vaccine batch list has reached maximum capacity
 * @param lista_vacina Pointer to the first vaccine batch
 * @return 1 if full, 0 otherwise
 */
int lista_vacina_eh_cheio(LoteVacina* lista_vacina);

/**
 * @brief Verifies if a batch with the given ID already exists
 * @param lote Batch ID to check
 * @param lista_vacina Pointer to the first vaccine batch
 * @return 1 if exists, 0 otherwise
 */
int lote_existe(char* lote, LoteVacina* lista_vacina);

/**
 * @brief Validates batch ID format (uppercase hex digits)
 * 
 * Checks that all characters are valid hex digits (0-9, A-F)
 * 
 * @param lote Batch ID to validate
 * @return 1 if valid, 0 otherwise
 */
int eh_lote_valido(char* lote);

/**
 * @brief Checks if vaccine name follows the required format
 * 
 * Validates that it contains no spaces unless quoted
 * 
 * @param nome Vaccine name
 * @return 1 if valid, 0 otherwise
 */
int eh_nome_valido(char* nome);

/**
 * @brief Validates a date against system date
 * 
 * Makes sure the date is valid and not in the past
 * 
 * @param dia Day to check
 * @param mes Month to check
 * @param ano Year to check
 * @param dia_sistema Current system day
 * @param mes_sistema Current system month
 * @param ano_sistema Current system year
 * @return 1 if valid, 0 otherwise
 */
int eh_data_valido(int* dia, int* mes, int* ano, int* dia_sistema,
                int* mes_sistema, int* ano_sistema);

/**
 * @brief Processes user input to create a new vaccine batch
 * 
 * Reads batch details, validates them, and adds to the system
 */
void obter_dados_lote(LoteVacina** lista_vacinas, char* lote, int* dia, 
            int* mes, int* ano, int dia_atual, int mes_atual, int ano_atual, 
            int* num_dosas, char* noma_vacina, int use_portugues);

/**
 * @brief Lists vaccine batches based on user input
 * 
 * Can list all batches or filter by vaccine name(s)
 */
void listar_vacinas(LoteVacina* lista_vacina, int use_portuguese);

/**
 * @brief Applies a vaccine dose to a user
 * 
 * Selects the oldest valid batch and records application
 */
void aplicar_dose_vacina(LoteVacina* lista_vacina, User** lista_user,
                        HojeVaxTable* vax_table_hoje, int dia_atual,
                        int mes_atual, int ano_atual, int use_portuguese);

/**
 * @brief Removes availability of a specified batch
 * 
 * Either deletes batch or marks it as fully used
 */
void retirar_disponibilidade(LoteVacina** lista_vacina, User* lista_user,
                            int use_portuguese);

/**
 * @brief Advances system time
 * 
 * Updates the current date and refreshes the daily vaccination table
 */
void avancar_tempo(int* dia_sistema, int* mes_sistema, int* ano_sistema,
                HojeVaxTable* vax_table_hoje, int use_portuguese);

/**
 * @brief Deletes vaccination records based on criteria
 * 
 * Can delete by user, date, and/or batch
 */
void apagar_aplicacoes(User** lista_user, LoteVacina* lista_vacina, 
                        int dia_atual, int mes_atual, int ano_atual,
                        int use_portuguese);

/**
 * @brief Lists vaccination applications
 * 
 * Shows all applications or just those for specific user
 */
void listar_aplicacoes(User* lista_user, int use_portuguese);

/**
 * @brief Program entry point
 * 
 * Initializes system and handles command processing
 */
int main(int argumento_num, char *argumento_val[]) {
    LoteVacina* lista_vacinas = NULL;
    User* lista_users = NULL;
    HojeVaxTable* vax_table_hoje = NULL;
    char comando;
    int dia, mes, ano, num_dosas;
    char lote[NOME_LOTE_MAX], nome_vacina[NOME_VACINA_MAX];
    int use_portugues = 0;

    /* Initial system date is 01-01-2025 */
    int dia_sistema = 1, mes_sistema = 1, ano_sistema = 2025;

    /* Check if Portuguese language flag is set */
    if (argumento_num > 1 && strcmp(argumento_val[1], "pt") == 0) {
        use_portugues = 1;
    }

    vax_table_hoje = criar_vax_table_hoje(dia_sistema, mes_sistema, 
                                        ano_sistema);
    
    char line_buffer[BUFFER_SIZE];
    while (1) {
        int ler_res = scanf(" %c", &comando);

        if (ler_res == EOF) {
            break;
        }
        
        switch (comando) {
            case 'q':
                free_lotes(lista_vacinas);
                free_users(lista_users);
                free_hoje_vax_table(vax_table_hoje);
                return 0;
            case 'c':
                obter_dados_lote(&lista_vacinas, lote, &dia, &mes, &ano, 
                                dia_sistema, mes_sistema, ano_sistema,
                                &num_dosas, nome_vacina, use_portugues);
                break;
            case 'l':
                listar_vacinas(lista_vacinas, use_portugues);
                break;
            case 'a':
                aplicar_dose_vacina(lista_vacinas, &lista_users, vax_table_hoje,
                                    dia_sistema, mes_sistema, ano_sistema,
                                    use_portugues);
                break;
            case 'r':
                retirar_disponibilidade(&lista_vacinas, lista_users,
                                       use_portugues);
                break;
            case 't':
                avancar_tempo(&dia_sistema, &mes_sistema, &ano_sistema,
                            vax_table_hoje, use_portugues);
                break;
            case 'd':
                apagar_aplicacoes(&lista_users, lista_vacinas, dia_sistema,
                                mes_sistema, ano_sistema, use_portugues);
                break;
            case 'u':
                listar_aplicacoes(lista_users, use_portugues);
                break;
            default:
                fgets(line_buffer, BUFFER_SIZE, stdin);
                break;
        }
    }

    /* Clean up before exiting */
    free_lotes(lista_vacinas);
    free_users(lista_users);
    free_hoje_vax_table(vax_table_hoje);
    return 0;
}

/**
 * @brief Processes batch creation data from user input
 * 
 * Reads and validates input parameters for creating a new batch.
 * Reports appropriate errors if validation fails.
 */
void obter_dados_lote(LoteVacina** lista_vacinas, char* lote, int* dia, 
            int* mes, int* ano, int dia_atual, int mes_atual, int ano_atual,
                int* num_dosas, char* nome_vacina, int use_portugues){
    char input_buffer[BUFFER_SIZE];

    if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL){
        printf(use_portugues ? "sem memória.\n" : "No memory.\n");
        return;
    }

    /* Pre-check batch name length to avoid buffer overflows */
    char primeiro_scan[BUFFER_SIZE];
    if (sscanf(input_buffer, " %s", primeiro_scan) == 1 && 
        strlen(primeiro_scan) >= NOME_LOTE_MAX) {
        printf(use_portugues ? "lote inválido\n" : "invalid batch\n");
        return;
    }

    int input = sscanf(input_buffer, " %20s %d-%d-%d %d %50s",
                    lote, dia, mes, ano, num_dosas, nome_vacina);

    if(input == 6){
        if (!eh_data_valido(dia, mes, ano, &dia_atual, &mes_atual,
                            &ano_atual)) {
            printf(use_portugues ? "data inválida\n" : "invalid date\n");
            return;
        }else if (!eh_lote_valido(lote)) {
            printf(use_portugues ? "lote inválido\n" : "invalid batch\n");
            return;
        }else if (!eh_nome_valido(nome_vacina)) {
            printf(use_portugues ? "nome inválido\n" : "invalid name\n");
        } else if (lista_vacina_eh_cheio(*lista_vacinas)) {
            printf(use_portugues ? "demasiadas vacinas\n" : 
                "too many vaccines\n");
        } else if (lote_existe(lote, *lista_vacinas)) {
            printf(use_portugues ? "número de lote duplicado\n" : 
                "duplicate batch number\n");
        } else if (*num_dosas <= 0) {
            printf(use_portugues ? "quantidade inválida\n" : 
                "invalid quantity\n");
        } else {
            adicionar_lote(lista_vacinas, 
                        criar_lote(nome_vacina, lote, *dia, *mes,
                                    *ano, *num_dosas));
            printf("%s\n", lote);
        }
    }
}

/**
 * @brief Counts total number of batches in list
 * 
 * Used to check against MAX_LOTES limit
 * 
 * @param lista_vacina Head of batch list
 * @return Number of batches
 */
int conta_lote(LoteVacina* lista_vacina) {
    int conta = 0;
    LoteVacina* current = lista_vacina; 

    while (current != NULL) {
        conta++; 
        current = current->next; 
    }

    return conta;
}

/**
 * @brief Checks if batch limit is reached
 * 
 * @param lista_vacina Head of batch list
 * @return 1 if full, 0 otherwise
 */
int lista_vacina_eh_cheio(LoteVacina* lista_vacina) {
    return conta_lote(lista_vacina) >= MAX_LOTES;
}

/**
 * @brief Verifies if batch ID already exists in system
 * 
 * @param lote Batch ID to check
 * @param lista_vacina Head of batch list
 * @return 1 if exists, 0 otherwise
 */
int lote_existe(char* lote, LoteVacina* lista_vacina) {
    LoteVacina* current = lista_vacina;
    while (current != NULL) {
        if (strcmp(current->lote, lote) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

/**
 * @brief Validates batch ID format
 * 
 * Checks that all characters are valid hex digits (0-9, A-F)
 * 
 * @param lote Batch ID to validate
 * @return 1 if valid, 0 otherwise
 */
int eh_lote_valido(char* lote) {
    int len = strlen(lote);

    for (int i = 0; i < len; i++) {
        if (!((lote[i] >= '0' && lote[i] <= '9') ||
                (lote[i] >= 'A' && lote[i] <= 'F'))) {
            return 0;  
        }
    }
    return 1;
}

/**
 * @brief Validates vaccine name format
 * 
 * Names can't contain whitespace unless quoted
 * 
 * @param nome Name to validate
 * @return 1 if valid, 0 otherwise
 */
int eh_nome_valido(char* nome) {
    int len = strlen(nome);
    if (len >= NOME_VACINA_MAX-1) return 0;
    
    int eh_citado = (nome[0] == '"' && nome[len - 1] == '"');
    
    if (eh_citado) {
        for (int i = 1; i < len - 1; i++) {
            if (nome[i] == '"') {
                return 0;
            }
        }
    } else {
        for (int i = 0; i < len; i++) {
            if (nome[i] == ' ' || nome[i] == '\t') {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * @brief Validates date and checks it's not in the past
 * 
 * Makes sure date is valid (checking month, days, years and leap years)
 * and after system date
 * @return 1 if valid, 0 otherwise
 */
int eh_data_valido(int* dia, int* mes, int* ano, int* dia_sistema,
                 int* mes_sistema, int* ano_sistema) {
    if (*mes < 1 || *mes > 12) return 0;
    if (*dia < 1 || *dia > 31) return 0;

    /* Check days in month */
    if ((*mes == 4 || *mes == 6 || *mes == 9 || *mes == 11) && 
        *dia > 30) return 0;
    if (*mes == 2) {
        int leap = (*ano % 4 == 0 && (*ano % 100 != 0 || *ano % 400 == 0));
        if (*dia > (leap ? 29 : 28)) return 0;
    }

    /* Make sure date isn't in the past */
    int data_input = ddmmyy_int(*dia, *mes, *ano);
    int data_sistema = ddmmyy_int(*dia_sistema, *mes_sistema, *ano_sistema);
    if (data_input < data_sistema) return 0;

    return 1;
}

/**
 * @brief Compares two batches for sorting
 * 
 * First by expiration date, then alphabetically by batch ID
 * 
 * @param lote_a First batch
 * @param lote_b Second batch
 * @return Negative if a<b, positive if a>b, 0 if equal
 */
int comparar_lotes(LoteVacina* lote_a, LoteVacina* lote_b) {
    if (lote_a->data_expiracao_int != lote_b->data_expiracao_int) {
        return lote_a->data_expiracao_int - lote_b->data_expiracao_int;
    }
    
    return strcmp(lote_a->lote, lote_b->lote);
}

/**
 * @brief Merge step of mergesort for batch sorting
 * 
 * Helper function for the mergeSort algorithm
 */
void mergelotes(LoteVacina** array_lotes, int e, int m, int d){
    int i, j, k;
    int n1 = m - e + 1;     
    int n2 = d - m;

    LoteVacina** Esquerda = (LoteVacina**)malloc(n1 * sizeof(LoteVacina*));;
    LoteVacina** Direito = (LoteVacina**)malloc(n2 * sizeof(LoteVacina*));

    for(i = 0; i < n1; i++){
        Esquerda[i] = array_lotes[e + i];
    }
    for(j = 0; j < n2; j++){
        Direito[j] = array_lotes[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = e;
    while(i < n1 && j < n2){
        if (comparar_lotes(Esquerda[i], Direito[j]) <= 0){
            array_lotes[k] = Esquerda[i];
            i++;
        } else{
            array_lotes[k] = Direito[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array_lotes[k] = Esquerda[i];
        i++;
        k++;
    }
    while (j < n2) {
        array_lotes[k] = Direito[j];
        j++;
        k++;
    }

    free(Esquerda);
    free(Direito);
}

/**
 * @brief Sorts batches using merge sort algorithm
 * 
 * Recursive implementation for performance
 */
void mergeSort(LoteVacina** array_lote, int e, int d) {
    if (e < d) {
        int m = e + (d - e) / 2;
        
        mergeSort(array_lote, e, m);
        mergeSort(array_lote, m + 1, d);
        
        mergelotes(array_lote, e, m, d);
    }
}

/**
 * @brief Lists vaccine batches according to user criteria
 * 
 * Displays batches sorted by expiration date and batch ID.
 * Can filter by vaccine name(s) if specified.
 */
void listar_vacinas(LoteVacina* lista_vacina, int use_portuguese) {
    char input_buffer[BUFFER_SIZE];
    char nome_vacinas[10][NOME_VACINA_MAX]; 
    int num_nomes = 0;
    int num_lotes = conta_lote(lista_vacina);
    
    /* Nothing to list */
    if (num_lotes == 0) {
        return; 
    }
    
    /* Create array for sorting */
    LoteVacina** array_lotes = (LoteVacina**)
        malloc(num_lotes * sizeof(LoteVacina*));
    if (!array_lotes) {
        printf(use_portuguese ? "sem memória\n" : "No memory\n");
        return;
    }
    
    /* Copy list to array for easier sorting */
    LoteVacina* current = lista_vacina;
    int i = 0;
    while (current != NULL) {
        array_lotes[i++] = current;
        current = current->next;
    }
    
    /* Sort batches by expiration date and batch ID */
    mergeSort(array_lotes, 0, num_lotes-1);
    
    /* Get filter parameters, if any */
    if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {
        free(array_lotes);
        return;
    }
    
    int len = strlen(input_buffer);
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
        len--;
    }
    
    /* Trim leading whitespace */
    char* trimmed_input = input_buffer;
    while (*trimmed_input &&
            isspace((unsigned char)*trimmed_input)) trimmed_input++;
    
    /* Parse vaccine names from input if provided */
    if (trimmed_input[0] != '\0') {
        char* nome = strtok(trimmed_input, " ");
        while (nome != NULL && num_nomes < 10) {
            strcpy(nome_vacinas[num_nomes], nome);
            num_nomes++;
            nome = strtok(NULL, " ");
        }
    }
    
    /* Show all batches if no filters were provided */
    if (num_nomes == 0) {
        for (i = 0; i < num_lotes; i++) {
            printf("%s %s %02d-%02d-%d %d %d\n", 
                array_lotes[i]->nome, 
                array_lotes[i]->lote,
                array_lotes[i]->dia_de_expiracao, 
                array_lotes[i]->mes_de_expiracao, 
                array_lotes[i]->ano_de_expiracao,
                array_lotes[i]->dosas_disponiveis,
                array_lotes[i]->total_aplicacoes);
        }
    } else {
        /* Show only batches matching filter names */
        for (int j = 0; j < num_nomes; j++) {
            int found = 0;
            for (i = 0; i < num_lotes; i++) {
                if (strcmp(array_lotes[i]->nome, nome_vacinas[j]) == 0) {
                    printf("%s %s %02d-%02d-%d %d %d\n", 
                           array_lotes[i]->nome, 
                           array_lotes[i]->lote,
                           array_lotes[i]->dia_de_expiracao, 
                           array_lotes[i]->mes_de_expiracao, 
                           array_lotes[i]->ano_de_expiracao,
                           array_lotes[i]->dosas_disponiveis,
                           array_lotes[i]->total_aplicacoes);
                    found = 1;
                }
            }
            if (!found) {
                printf(use_portuguese ? "%s: vacina inexistente\n" : 
                      "%s: no such vaccine\n", nome_vacinas[j]);
            }
        }
    }
    
    free(array_lotes);
}

/**
 * @brief Parses user input for vaccine application
 * 
 * Handles quoted and unquoted user names.
 * 
 * @param nome_usuario Output buffer for user name
 * @param nome_vacina Output buffer for vaccine name
 * @return 1 on success, 0 on error
 */
int obter_dados_aplicacao(char* nome_usuario, char* nome_vacina) {
    char input_buffer[BUFFER_SIZE];
    
    nome_usuario[0] = '\0';
    nome_vacina[0] = '\0';
    
    if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {
        return 0;
    }
    
    int len = strlen(input_buffer);
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
        len--;
    }
    
    /* Skip initial whitespace */
    char* p = input_buffer;
    while (*p && isspace((unsigned char)*p)) p++;
    
    /* Handle quoted user names */
    if (*p == '"') {
        p++; 
        char* end_quote = strchr(p, '"');
        
        if (end_quote == NULL) {
            return 0;
        }
        
        int name_len = end_quote - p;
        if (name_len >= BUFFER_SIZE) {
            name_len = BUFFER_SIZE - 1; 
        }
        
        memcpy(nome_usuario, p, name_len);
        nome_usuario[name_len] = '\0';
        
        p = end_quote + 1;
    } else {
        /* Handle unquoted user names */
        char* space = strchr(p, ' ');
        if (space == NULL) {
            memcpy(nome_usuario, p, BUFFER_SIZE - 1);
            nome_usuario[BUFFER_SIZE - 1] = '\0';
            return 1;
        }
        
        int name_len = space - p;
        if (name_len >= BUFFER_SIZE) {
            name_len = BUFFER_SIZE - 1; 
        }
        memcpy(nome_usuario, p, name_len);
        nome_usuario[name_len] = '\0';
        
        p = space;
    }
    
    /* Skip whitespace after user name */
    while (*p && isspace((unsigned char)*p)) p++;
    
    if (*p == '\0') {
        return 1;
    }
    
    /* Get vaccine name - limited to 50 chars + null terminator */
    memcpy(nome_vacina, p, 50);
    nome_vacina[50] = '\0';
    
    return 1;
}

/**
 * @brief Finds oldest valid batch of a specific vaccine
 * 
 * Returns batch that expires soonest but is still valid.
 * Only considers batches with available doses.
 * 
 * @param lista_vacina Head of batch list
 * @param nome_vacina Name of vaccine to find
 * @param dia_atual Current system day
 * @param mes_atual Current system month
 * @param ano_atual Current system year
 * @return Pointer to found batch or NULL if none available
 */
LoteVacina* encontrar_lote_mais_antigo(LoteVacina* lista_vacina,
                                     char* nome_vacina, int dia_atual,
                                     int mes_atual, int ano_atual) {
    LoteVacina* current = lista_vacina;
    LoteVacina* lote_mais_antigo = NULL;
    int data_atual_int = ddmmyy_int(dia_atual, mes_atual, ano_atual);

    while (current != NULL) {
        if (strcmp(current->nome, nome_vacina) == 0) {
            /* Check if batch is valid and has doses */
            if (current->dosas_disponiveis > 0 &&
               data_atual_int <= current->data_expiracao_int) {             
                /* Track oldest valid batch */
                if (lote_mais_antigo == NULL || 
                    current->data_expiracao_int < 
                    lote_mais_antigo->data_expiracao_int) {
                    lote_mais_antigo = current;
                }
            }
        }
        current = current->next;
    }
    
    return lote_mais_antigo;
}
/**
 * @brief Applies a vaccine dose to a user
 * 
 * Finds oldest valid batch and records application.
 * Handles validation and error cases.
 */
void aplicar_dose_vacina(LoteVacina* lista_vacina, User** lista_user,
                         HojeVaxTable* vax_table_hoje, int dia_atual,
                         int mes_atual, int ano_atual, int use_portuguese) {
    char nome_usuario[BUFFER_SIZE];
    char nome_vacina[51];
    
    if (!obter_dados_aplicacao(nome_usuario, nome_vacina)) {
        return;
    }
    
    /* Make sure vaccination table is current */
    reset_table_hoje(vax_table_hoje, dia_atual, mes_atual, ano_atual);
    
    /* Check if user already got this vaccine today */
    if (eh_vacinado_hoje(vax_table_hoje, nome_usuario, nome_vacina)) {
        printf(use_portuguese ? "já vacinado\n" : "already vaccinated\n");
        return;
    }
    
    /* Find oldest valid batch with doses */
    LoteVacina* lote = encontrar_lote_mais_antigo(lista_vacina, nome_vacina,
                                dia_atual, mes_atual, ano_atual);
    
    if (lote == NULL) {
        printf(use_portuguese ? "esgotado\n" : "no stock\n");
        return;
    }
    
    /* Update batch info */
    lote->dosas_disponiveis--;
    lote->total_aplicacoes++;
    
    /* Record vaccination */
    aplicar_vacina(lista_user, nome_usuario, nome_vacina, lote->lote,
                  dia_atual, mes_atual, ano_atual);
    
    /* Track today's vaccination to prevent duplicates */
    recorde_vacinacao_hoje(vax_table_hoje, nome_usuario, nome_vacina);
    
    printf("%s\n", lote->lote);
}

/**
 * @brief Gets batch ID from user input for removal
 * 
 * @param lote Output buffer for batch ID
 * @return 1 if successful, 0 on error
 */
int obter_dados_retirada(char* lote) {
    char input_buffer[BUFFER_SIZE];

    if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {
        printf("buffer size reached\n");
        return 0;
    }

    int input_c = sscanf(input_buffer, " %20s", lote);
    return (input_c == 1);
}

/**
 * @brief Counts applications from a specific batch
 * 
 * @param lista_user Head of user list
 * @param lote Batch ID to count
 * @param lista_vacina Batch list to find vaccine name
 * @return Number of applications from this batch
 */
int contar_aplicacoes_lote(User* lista_user, char* lote,
                          LoteVacina* lista_vacina) {
    LoteVacina* current_lote = lista_vacina;
    char nome_vacina_lote[51];
    int encontrado = 0;
    
    /* Find vaccine name for this batch */
    while (current_lote != NULL) {
        if (strcmp(current_lote->lote, lote) == 0) {
            strcpy(nome_vacina_lote, current_lote->nome);
            encontrado = 1;
            break;
        }
        current_lote = current_lote->next;
    }
    
    if (!encontrado) {
        return 0;
    }
    
    /* Count applications */
    int contador = 0;
    User* current = lista_user;
    
    while (current != NULL) {
        if (strcmp(current->nome_vacina, nome_vacina_lote) == 0) {
            contador++;
        }
        current = current->next;
    }
    
    return contador;
}

/**
 * @brief Removes a batch from the list
 * 
 * Handles both head node and interior node cases.
 * 
 * @param lista_vacina Pointer to head pointer of batch list
 * @param lote Batch ID to remove
 */
void remover_lote(LoteVacina** lista_vacina, char* lote) {
    LoteVacina* current = *lista_vacina;
    LoteVacina* previous = NULL;
    
    while (current != NULL) {
        if (strcmp(current->lote, lote) == 0) {
            if (previous == NULL) {
                *lista_vacina = current->next;
            } else {
                previous->next = current->next;
            }
            
            free(current);
            return;
        }
        
        previous = current;
        current = current->next;
    }
}

/**
 * @brief Removes a batch from availability
 * 
 * Either completely removes batch or marks it as having
 * no more available doses depending on usage.
 */
void retirar_disponibilidade(LoteVacina** lista_vacina, User* lista_user,
                             int use_portuguese) {
    char lote[21];
    
    if (!obter_dados_retirada(lote)) {
        return;
    }
    
    /* Locate batch in system */
    LoteVacina* current = *lista_vacina;
    LoteVacina* batch_to_update = NULL;
    
    while (current != NULL) {
        if (strcmp(current->lote, lote) == 0) {
            batch_to_update = current;
            break;
        }
        current = current->next;
    }
    
    if (batch_to_update == NULL) {
        printf(use_portuguese ? "%s: lote inexistente\n" : 
              "%s: no such batch\n", lote);
        return;
    }
    
    /* Count applications from this batch */
    int aplicacoes = contar_aplicacoes_lote(lista_user, lote, *lista_vacina);
    
    if (aplicacoes == 0) {
        /* Remove batch entirely if never used */
        remover_lote(lista_vacina, lote);
    } else {
        /* Mark batch as having no more available doses */
        batch_to_update->dosas_disponiveis = 0;
    }
    
    printf("%d\n", aplicacoes);
}

/**
 * @brief Parses time advancement input from user
 * 
 * @param dia Output for day
 * @param mes Output for month
 * @param ano Output for year
 * @return 1 if successful, 0 on error
 */
int obter_dados_avanco_tempo(int* dia, int* mes, int* ano) {
    char input_buffer[BUFFER_SIZE];

    if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {
        return 0;
    }
    
    /* Handle empty input (just show current date) */
    char* trimmed = input_buffer;
    while (isspace((unsigned char)*trimmed)) trimmed++;
    
    if (*trimmed == '\0' || *trimmed == '\n') {
        *dia = 0;
        *mes = 0;
        *ano = 0;
        return 1;
    }
    
    /* Parse date components */
    int input_c = sscanf(trimmed, "%d-%d-%d", dia, mes, ano);
    return (input_c == 3);
}
/**
 * @brief Validates date for time advancement
 * 
 * Makes sure date is valid and not before current system date
 * 
 * @param dia Day to check
 * @param mes Month to check
 * @param ano Year to check
 * @param dia_atual Current system day
 * @param mes_atual Current system month
 * @param ano_atual Current system year
 * @return 1 if valid, 0 otherwise
 */
int eh_data_avanco_valido(int dia, int mes, int ano, int dia_atual,
                         int mes_atual, int ano_atual) {
    /* Compare with current date */
    int data_atual = ddmmyy_int(dia_atual, mes_atual, ano_atual);
    int data_avanco = ddmmyy_int(dia, mes, ano);

    if (data_avanco < data_atual) return 0;
    
    /* Basic validity checks */
    if (mes < 1 || mes > 12) return 0;
    
    /* Check days in month including leap years */
    int dias_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (mes == 2 && (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0))) {
        dias_mes[2] = 29;
    }
    
    if (dia < 1 || dia > dias_mes[mes]) return 0;
    
    return 1;
}

/**
 * @brief Advances or displays system time
 * 
 * Can show current date or set a new future date
 */
void avancar_tempo(int* dia_sistema, int* mes_sistema, int* ano_sistema,
                  HojeVaxTable* vax_table_hoje, int use_portuguese) {
    int dia, mes, ano;
    
    if (!obter_dados_avanco_tempo(&dia, &mes, &ano)) {
        return;
    }
    
    /* Just display current date when no arguments */
    if (dia == 0 && mes == 0 && ano == 0) {
        printf("%02d-%02d-%d\n", *dia_sistema, *mes_sistema, *ano_sistema);
        return;
    }
    
    /* Validate new date */
    if (!eh_data_avanco_valido(dia, mes, ano, *dia_sistema, *mes_sistema,
                              *ano_sistema)) {
        printf(use_portuguese ? "data inválida\n" : "invalid date\n");
        return;
    }
    
    /* Update system date */
    *dia_sistema = dia;
    *mes_sistema = mes;
    *ano_sistema = ano;
    
    /* Clear today's vaccination records */
    reset_table_hoje(vax_table_hoje, dia, mes, ano);
    
    printf("%02d-%02d-%d\n", *dia_sistema, *mes_sistema, *ano_sistema);
}

/**
 * @brief Parses input for deleting vaccination records
 * 
 * Handles three formats: just username, username+date, or username+date+batch
 * Deals with both quoted and unquoted usernames
 * 
 * @param nome_usuario Output buffer for username
 * @param dia Output for day component (0 if not provided)
 * @param mes Output for month component (0 if not provided)
 * @param ano Output for year component (0 if not provided)
 * @param lote Output buffer for batch ID (empty if not provided)
 * @return 1 if parsing succeeded, 0 on failure
 */
int obter_dados_apagar(char* nome_usuario, int* dia, int* mes, int* ano,
                      char* lote) {
    char input_buffer[BUFFER_SIZE];
    *dia = 0;
    *mes = 0;
    *ano = 0;
    lote[0] = '\0';  

    if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {
        return 0;
    }
    
    int len = strlen(input_buffer);
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
        len--;
    }
    
    /* Skip leading whitespace */
    char* ptr = input_buffer;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    /* Handle quoted username */
    if (*ptr == '"') {
        ptr++; 
        char* end_quote = strchr(ptr, '"');
        
        if (end_quote == NULL) {
            return 0;
        }
        
        int name_len = end_quote - ptr;
        if (name_len >= BUFFER_SIZE) name_len = BUFFER_SIZE-1;
        
        memcpy(nome_usuario, ptr, name_len);
        nome_usuario[name_len] = '\0';
        
        ptr = end_quote + 1;
    } else {
        /* Handle unquoted username */
        char* space = strchr(ptr, ' ');
        if (space == NULL) {
            strcpy(nome_usuario, ptr);
            return 1;
        }
        
        int name_len = space - ptr;
        if (name_len >= BUFFER_SIZE) name_len = BUFFER_SIZE-1; 
        
        memcpy(nome_usuario, ptr, name_len);
        nome_usuario[name_len] = '\0';
        
        ptr = space + 1;
    }
    
    /* Skip whitespace after username */
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    if (*ptr == '\0') {
        return 1; /* Only username provided */
    }
    
    /* Try to parse date */
    char* date_start = ptr;
    int date_parse = sscanf(date_start, "%d-%d-%d", dia, mes, ano);
    
    if (date_parse != 3) {
        return 1; /* Invalid date format, just return username */
    }
    
    /* Look for batch ID after date */
    ptr = strchr(date_start, ' ');
    if (ptr == NULL) {
        return 1; /* No batch ID provided */
    }
    
    ptr++;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    
    if (*ptr != '\0') {
        strcpy(lote, ptr);
    }
    
    return 1;
}
/**
 * @brief Validates date for deletion operation
 * 
 * Checks that date is valid and not in the future
 * 
 * @param dia Day to check
 * @param mes Month to check
 * @param ano Year to check
 * @param dia_atual Current system day
 * @param mes_atual Current system month
 * @param ano_atual Current system year
 * @return 1 if valid, 0 otherwise
 */
int eh_data_delecao_valida(int dia, int mes, int ano, int dia_atual,
                          int mes_atual, int ano_atual) {
    /* Special case - no date provided */
    if (dia == 0 && mes == 0 && ano == 0) {
        return 1;
    }
    
    /* Date must not be in the future */
    if (ano > ano_atual) return 0;
    if (ano == ano_atual && mes > mes_atual) return 0;
    if (ano == ano_atual && mes == mes_atual && dia > dia_atual) return 0;
    
    /* Basic validity checks */
    if (mes < 1 || mes > 12) return 0;
    
    /* Days in month validity */
    int dias_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (mes == 2 && (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0))) {
        dias_mes[2] = 29;
    }
    
    if (dia < 1 || dia > dias_mes[mes]) return 0;
    
    return 1;
}

/**
 * @brief Deletes vaccination records based on criteria
 * 
 * Helper function that does the actual deletion work
 * 
 * @param lista_user Pointer to head pointer of user list
 * @param nome_usuario Username to filter by
 * @param dia Day to filter by (0 for any day)
 * @param mes Month to filter by (0 for any month)
 * @param ano Year to filter by (0 for any year)
 * @param lote Batch ID to filter by (empty for any batch)
 * @param lista_vacina Batch list (needed to lookup vaccine names)
 * @return Number of records deleted
 */
int apagar_registros(User** lista_user, char* nome_usuario, int dia, int mes,
                     int ano, char* lote, LoteVacina* lista_vacina) {
    User* current = *lista_user;
    User* prev = NULL;
    int contador = 0;
    
    /* Check if batch exists when provided */
    if (lote[0] != '\0') {
        int batch_existe = 0;
        LoteVacina* batch_check = lista_vacina;
        
        while (batch_check != NULL) {
            if (strcmp(batch_check->lote, lote) == 0) {
                batch_existe = 1;
                break;
            }
            batch_check = batch_check->next;
        }
        
        if (!batch_existe) {
            printf("%s: no such batch\n", lote);
            return 0;
        }
    }
    
    /* Validate that user exists in records */
    int user_existe = 0;
    User* user_check = *lista_user;
    
    while (user_check != NULL) {
        if (strcmp(user_check->nome, nome_usuario) == 0) {
            user_existe = 1;
            break;
        }
        user_check = user_check->next;
    }
    
    if (!user_existe) {
        printf("%s: no such user\n", nome_usuario);
        return 0;
    }
    
    /* Traverse list and delete matching records */
    while (current != NULL) {
        int should_delete = 0;
        
        if (strcmp(current->nome, nome_usuario) == 0) {
            /* Case 1: Delete all records for this user */
            if (dia == 0 && mes == 0 && ano == 0 && lote[0] == '\0') {
                should_delete = 1;
            }
            /* Case 2: Delete records for this user on specific date */
            else if (dia != 0 && lote[0] == '\0') {
                if (current->dia_de_applicacao == dia && 
                    current->mes_de_aplicacao == mes && 
                    current->ano_de_aplicacao == ano) {
                    should_delete = 1;
                }
            }
            /* Case 3: Delete records for this user on specific date with
            specific batch */
            else if (dia != 0 && lote[0] != '\0') {
                if (current->dia_de_applicacao == dia && 
                    current->mes_de_aplicacao == mes && 
                    current->ano_de_aplicacao == ano) {
                    
                    /* Need to find vaccine name for this batch */
                    LoteVacina* temp = lista_vacina;
                    char vacina_lote[51] = "";
                    
                    while (temp != NULL) {
                        if (strcmp(temp->lote, lote) == 0) {
                            strcpy(vacina_lote, temp->nome);
                            break;
                        }
                        temp = temp->next;
                    }
                    
                    if (strcmp(current->nome_vacina, vacina_lote) == 0) {
                        should_delete = 1;
                    }
                }
            }
        }
        
        if (should_delete) {
            User* to_delete = current;
          
            /* Update list pointers */
            if (prev == NULL) {
                *lista_user = current->next;
                current = *lista_user;
            } else {
                prev->next = current->next;
                current = current->next;
            }
            
            /* Free memory and count deletion */
            free(to_delete);
            contador++;
        } else {
            prev = current;
            current = current->next;
        }
    }
    
    return contador;
}

/**
 * @brief Handles deletion of vaccination records
 * 
 * Main function that processes input and calls helper functions
 */
void apagar_aplicacoes(User** lista_user, LoteVacina* lista_vacina,
                      int dia_atual, int mes_atual, int ano_atual,
                      int use_portuguese) {
    char nome_usuario[BUFFER_SIZE];
    int dia, mes, ano;
    char lote[21];
    
    if (!obter_dados_apagar(nome_usuario, &dia, &mes, &ano, lote)) {
        return;
    }
    
    /* Validate date if provided */
    if (dia != 0 && !eh_data_delecao_valida(dia, mes, ano, dia_atual,
                                            mes_atual, ano_atual)) {
        printf(use_portuguese ? "data inválida\n" : "invalid date\n");
        return;
    }
    
    /* Check if user exists */
    int user_exists = 0;
    User* user_check = *lista_user;
    
    while (user_check != NULL) {
        if (strcmp(user_check->nome, nome_usuario) == 0) {
            user_exists = 1;
            break;
        }
        user_check = user_check->next;
    }
    
    if (!user_exists) {
        printf(use_portuguese ? "%s: utente inexistente\n" :
              "%s: no such user\n", nome_usuario);
        return;
    }
    
    /* Check if batch exists */
    if (lote[0] != '\0') {
        int batch_exists = 0;
        LoteVacina* batch_check = lista_vacina;
        
        while (batch_check != NULL) {
            if (strcmp(batch_check->lote, lote) == 0) {
                batch_exists = 1;
                break;
            }
            batch_check = batch_check->next;
        }
        
        if (!batch_exists) {
            printf(use_portuguese ? "%s: lote inexistente\n" : 
                  "%s: no such batch\n", lote);
            return;
        }
    }
    
    /* Perform deletions */
    int deleted = apagar_registros(lista_user, nome_usuario, dia, mes, ano,
                                  lote, lista_vacina);
    
    printf("%d\n", deleted);
}

/**
 * @brief Parses input for listing applications
 * 
 * Handles both empty input and username specification
 * Supports quoted and unquoted usernames
 * 
 * @param nome_usuario Output buffer for username (empty if not provided)
 * @return 1 if successful, 0 on error
 */
int obter_dados_listar_usuarios(char* nome_usuario) {
    char input_buffer[BUFFER_SIZE];
    nome_usuario[0] = '\0';  

    if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {
        return 0;
    }
    
    int len = strlen(input_buffer);
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
        len--;
    }
    
    /* Skip whitespace */
    char* trimmed = input_buffer;
    while (*trimmed && isspace((unsigned char)*trimmed)) trimmed++;
    
    if (*trimmed == '\0') {
        return 1; /* No username specified */
    }
    
    /* Handle quoted username */
    if (*trimmed == '"') {
        trimmed++; 
        char* end_quote = strchr(trimmed, '"');
        
        if (end_quote == NULL) {
            return 0;
        }
        
        int name_len = end_quote - trimmed;
        if (name_len >= BUFFER_SIZE) name_len = BUFFER_SIZE-1; 
        memcpy(nome_usuario, trimmed, name_len);
        nome_usuario[name_len] = '\0';
    } else {
        /* Handle unquoted username */
        strcpy(nome_usuario, trimmed);
    }
    
    return 1;
}

/**
 * @brief Comparison function for sorting applications by date
 * 
 * Used for sorting applications chronologically
 * 
 * @param a Pointer to first User pointer
 * @param b Pointer to second User pointer
 * @return Negative if a<b, positive if a>b, 0 if equal
 */
int comparar_aplicacoes_por_data(const void* a, const void* b) {
    User* user_a = *((User**)a);
    User* user_b = *((User**)b);
    
    /* Sort by year first */
    if (user_a->ano_de_aplicacao != user_b->ano_de_aplicacao) {
        return user_a->ano_de_aplicacao - user_b->ano_de_aplicacao;
    }
    
    /* Then by month */
    if (user_a->mes_de_aplicacao != user_b->mes_de_aplicacao) {
        return user_a->mes_de_aplicacao - user_b->mes_de_aplicacao;
    }
    
    /* Then by day */
    if (user_a->dia_de_applicacao != user_b->dia_de_applicacao) {
        return user_a->dia_de_applicacao - user_b->dia_de_applicacao;
    }

    return 0;
}

/**
 * @brief Sorts applications by date using insertion sort
 * 
 * Insertion sort is efficient for small datasets
 * and partially sorted data
 * 
 * @param array_users Array of User pointers to sort
 * @param num_users Number of users in array
 */
void ordenar_aplicacoes(User** array_users, int num_users){
    /* Reverse initial array - not sure why this is done, 
       might be to make later sorting easier if data is
       likely to be in reverse order already */
    for (int i = 0; i < num_users / 2; i++) {
        User* temp = array_users[i];
        array_users[i] = array_users[num_users - 1 - i];
        array_users[num_users - 1 - i] = temp;
    }
    
    /* Standard insertion sort algorithm */
    int i, j;
    User* chave;
    
    for (i = 1; i < num_users; i++) {
        chave = array_users[i];
        j = i - 1;
        
        while (j >= 0 && comparar_aplicacoes_por_data(&array_users[j],
                                                     &chave) > 0) {
            array_users[j + 1] = array_users[j];
            j = j - 1;
        }
        array_users[j + 1] = chave;
    }
}

/**
 * @brief Finds batch ID for a vaccine name
 * 
 * Helper function to get batch details
 * 
 * @param lista_vacina Head of vaccine batch list
 * @param nome_vacina Name of vaccine to find
 * @return Batch ID or NULL if not found
 */
char* encontrar_lote_por_vacina(LoteVacina* lista_vacina, char* nome_vacina) {
    LoteVacina* current = lista_vacina;
    
    while (current != NULL) {
        if (strcmp(current->nome, nome_vacina) == 0) {
            return current->lote;
        }
        current = current->next;
    }
    
    return NULL;
}

/**
 * @brief Lists vaccination applications
 * 
 * Lists all applications or only those for a specific user
 * Sorts by date of application
 */
void listar_aplicacoes(User* lista_user, int use_portuguese){
    char nome_usuario[BUFFER_SIZE];
    
    if (!obter_dados_listar_usuarios(nome_usuario)) {
        return;
    }
    
    /* If username specified, check that it exists */
    if (nome_usuario[0] != '\0') {
        int user_found = 0;
        User* check = lista_user;
        
        while (check != NULL) {
            if (strcmp(check->nome, nome_usuario) == 0) {
                user_found = 1;
                break;
            }
            check = check->next;
        }
        
        if (!user_found) {
            printf(use_portuguese ? "%s: utente inexistente\n" :
                 "%s: no such user\n", nome_usuario);
            return;
        }
    }

    /* Count total records for array allocation */
    int num_total = 0;
    User* current = lista_user;
    
    while (current != NULL) {
        num_total++;
        current = current->next;
    }
    
    if (num_total == 0 && nome_usuario[0] != '\0') {
        printf(use_portuguese ? "%s: utente inexistente\n" :
              "%s: no such user\n", nome_usuario);
        return;
    }

    if (num_total == 0) {
        return; /* Nothing to list */
    }
    
    /* Allocate array for sorting */
    User** array_users = (User**)malloc(num_total * sizeof(User*));
    if (!array_users) {
        printf(use_portuguese ? "sem memória\n" : "No memory\n");
        return;
    }
    
    /* Fill array with matching records */
    current = lista_user;
    int index = 0;
    
    if (nome_usuario[0] == '\0') {
        /* Add all records if no username filter */
        while (current != NULL) {
            array_users[index++] = current;
            current = current->next;
        }
    } else {
        int user_found = 0;
        
        /* Add only records for specified user */
        while (current != NULL) {
            if (strcmp(current->nome, nome_usuario) == 0) {
                array_users[index++] = current;
                user_found = 1;
            }
            current = current->next;
        }
        
        if (!user_found) {
            printf(use_portuguese ? "%s: utente inexistente\n" :
                  "%s: no such user\n", nome_usuario);
            free(array_users);
            return;
        }
    }
    
    /* Sort records by date */
    ordenar_aplicacoes(array_users, index);
    
    /* Print sorted records */
    for (int i = 0; i < index; i++) {
        printf("%s %s %02d-%02d-%d\n",
               array_users[i]->nome,
               array_users[i]->lote_usado,  
               array_users[i]->dia_de_applicacao,
               array_users[i]->mes_de_aplicacao,
               array_users[i]->ano_de_aplicacao);
    }
    
    free(array_users);
}