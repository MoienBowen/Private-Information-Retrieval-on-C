#include "Paillier.h"
#include "PIR_Naive.h"
#include "PIR_2D.h"

void Client_Send_Encrypt_3D(mpz_t tab_cli_row[], mpz_t tab_cli_col[],
                            mpz_t tab_cli_layer[], int row, int col, int layer);
                            
void Client_Receive_Decrypt_3D(mpz_t uu, mpz_t uv, mpz_t vu, mpz_t vv,
                                mpz_t KPri[3]);

void Serveur_3D(mpz_t uu, mpz_t uv, mpz_t vu, mpz_t vv,
                mpz_t tab_serv[MAX_GRID_SIZE][MAX_GRID_SIZE][MAX_GRID_SIZE],
                mpz_t tab_cli_row[MAX_GRID_SIZE],
                mpz_t tab_cli_col[MAX_GRID_SIZE],
                mpz_t tab_cli_layer[MAX_GRID_SIZE]);
