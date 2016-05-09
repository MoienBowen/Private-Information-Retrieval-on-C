#include "PIR_3D.h"

void Client_Send_Encrypt_3D(mpz_t tab_cli_row[], mpz_t tab_cli_col[],
                            mpz_t tab_cli_layer[], int row, int col, int layer)
{
    Client_Send_Encrypt (tab_cli_row, row);
    Client_Send_Encrypt (tab_cli_col, col);
    Client_Send_Encrypt (tab_cli_layer, layer);
}

void Client_Receive_Decrypt_3D(mpz_t uu, mpz_t uv, mpz_t vu, mpz_t vv,
                                mpz_t KPri[3])
{
    Client_Receive_Decrypt (uu, KPri);
    mpz_mul (uu, KPub[0], uu);
    Client_Receive_Decrypt (uv, KPri);
    mpz_add (uu, uu, uv);
    Client_Receive_Decrypt (uu, KPri);

    Client_Receive_Decrypt (vu, KPri);
    mpz_mul (vu, KPub[0], vu);
    Client_Receive_Decrypt (vv, KPri);
    mpz_add (vu, vu, vv);
    Client_Receive_Decrypt (vu, KPri);

    mpz_mul (uu, KPub[0], uu);
    mpz_add (uu, uu, vu);
    Client_Receive_Decrypt (uu, KPri);
}

void Serveur_3D(mpz_t uu, mpz_t uv, mpz_t vu, mpz_t vv,
                mpz_t tab_serv[MAX_GRID_SIZE][MAX_GRID_SIZE][MAX_GRID_SIZE],
                mpz_t tab_cli_row[MAX_GRID_SIZE],
                mpz_t tab_cli_col[MAX_GRID_SIZE],
                mpz_t tab_cli_layer[MAX_GRID_SIZE])
{
    mpz_t u_u_tmp[MAX_GRID_SIZE];
    mpz_t u_v_tmp[MAX_GRID_SIZE];
    mpz_t v_u_tmp[MAX_GRID_SIZE];
    mpz_t v_v_tmp[MAX_GRID_SIZE];

    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_init (u_u_tmp[i]);
        mpz_init (u_v_tmp[i]);
        mpz_init (v_u_tmp[i]);
        mpz_init (v_v_tmp[i]);
    }

    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        Serveur_2D(u_u_tmp[i], v_u_tmp[i], tab_serv[i],
                    tab_cli_col, tab_cli_layer);
    }


    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_fdiv_qr (u_u_tmp[i], u_v_tmp[i], u_u_tmp[i], KPub[0]);
        mpz_fdiv_qr (v_u_tmp[i], v_v_tmp[i], v_u_tmp[i], KPub[0]);
    }

    Serveur(uu, u_u_tmp, tab_cli_row);
    Serveur(uv, u_v_tmp, tab_cli_row);
    Serveur(vu, v_u_tmp, tab_cli_row);
    Serveur(vv, v_v_tmp, tab_cli_row);

    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_clear (u_u_tmp[i]);
        mpz_clear (u_v_tmp[i]);
        mpz_clear (v_u_tmp[i]);
        mpz_clear (v_v_tmp[i]);
    }
}
