#include "Paillier.h"
#include "PIR_Naive.h"

void Client_Send_Encrypt_2D(mpz_t tab_cli_row[], mpz_t tab_cli_col[],
                            int row, int col);

void Client_Receive_Decrypt_2D(mpz_t value_v, mpz_t value_u,mpz_t KPri[3]);

void Serveur_2D(mpz_t u, mpz_t v, mpz_t tab_serv[MAX_GRID_SIZE][MAX_GRID_SIZE],
                mpz_t tab_cli_row[MAX_GRID_SIZE], mpz_t tab_cli_col[]);
