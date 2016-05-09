#include "PIR_2D.h"

void Client_Send_Encrypt_2D(mpz_t tab_cli_row[], mpz_t tab_cli_col[],
                            int row, int col)
{
    Client_Send_Encrypt (tab_cli_row, row);
    Client_Send_Encrypt (tab_cli_col, col);
}

void Client_Receive_Decrypt_2D(mpz_t value_v, mpz_t value_u,mpz_t KPri[3])
{
    Client_Receive_Decrypt (value_u, KPri);
    Client_Receive_Decrypt (value_v, KPri);
    mpz_mul (value_u, KPub[0], value_u);
    mpz_add (value_u, value_v, value_u);
    Client_Receive_Decrypt (value_u, KPri);
}

void Serveur_2D(mpz_t u, mpz_t v, mpz_t tab_serv[MAX_GRID_SIZE][MAX_GRID_SIZE],
                mpz_t tab_cli_row[MAX_GRID_SIZE],
                mpz_t tab_cli_col[MAX_GRID_SIZE])
{
    mpz_t col_tmp[MAX_GRID_SIZE];
    mpz_t row_tmp[MAX_GRID_SIZE];
    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_init (col_tmp[i]);
        mpz_init (row_tmp[i]);
    }
    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        Serveur(row_tmp[i], tab_serv[i], tab_cli_col);
        mpz_fdiv_qr (col_tmp[i], row_tmp[i], row_tmp[i], KPub[0]);
    }
    Serveur(v, row_tmp, tab_cli_row);
    Serveur(u, col_tmp, tab_cli_row);
    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_clear (row_tmp[i]);
        mpz_clear (col_tmp[i]);
    }
}
