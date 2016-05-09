#include "PIR_XD.h"

void Client_Send_Encrypt_xD(int dim, mpz_t ** tab_cli,
                            int positions[dim])
{
    for(int i = 0; i < dim; i++)
    {
        tab_cli[i] = malloc(sizeof(mpz_t) * MAX_GRID_SIZE);
        Client_Send_Encrypt (tab_cli[i], positions[i]);
    }

}

void Serveur_xD(int dim, mpz_t ** tab_cli,
                mpz_t Answer[(int)pow(2, (dim - 1))],
                mpz_t tab_serv[MAX_GRID_SIZE][MAX_GRID_SIZE][MAX_GRID_SIZE])
{
    int n_tmp = (int)pow (2, (dim - 1)); // = 4 pour dim = 3

    mpz_t ** tab_u_v = malloc(sizeof(mpz_t *) * n_tmp);

    for(int i = 0; i < n_tmp; i++)
    {
        tab_u_v[i] = malloc(sizeof(mpz_t) * MAX_GRID_SIZE);
        for (int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_init (tab_u_v[i][j]);
        }
    }

    // It needs (dim - 1) for

    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {

        for (int j = 0; j < MAX_GRID_SIZE; j++)
        {
            Serveur(tab_u_v[0][j], tab_serv[i][j], tab_cli[2]);
            mpz_fdiv_qr (tab_u_v[0][j], tab_u_v[2][j], tab_u_v[0][j], KPub[0]);
        }

        Serveur(tab_u_v[1][i], tab_u_v[0], tab_cli[1]);
        Serveur(tab_u_v[3][i], tab_u_v[2], tab_cli[1]);
    }

    for(int j = 1; j < n_tmp; j+=2)
    {
        for (int i = 0; i < MAX_GRID_SIZE; i++)
        {
            mpz_fdiv_qr (tab_u_v[j - 1][i], tab_u_v[j][i], tab_u_v[j][i], KPub[0]);
        }
    }

    for(int i = 0; i < n_tmp; i++)
    {
        Serveur(Answer[i], tab_u_v[i], tab_cli[0]);
    }

    // end of for

    for(int i = 0; i < n_tmp; i++)
    {
        for (int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_clear(tab_u_v[i][j]);
        }
        free(tab_u_v[i]);
    }
}

void Client_Receive_Decrypt_xD(int dim, mpz_t Answer[(int)pow(2, (dim - 1))],
                                mpz_t KPri[3])
{
    int n_tmp = (int)pow(2, (dim - 1));
    int add_i = 1;

    while(n_tmp > 1)
    {
        for(int i = 0; i < n_tmp; i+=add_i*2)
        {
            Client_Receive_Decrypt (Answer[i], KPri);
            mpz_mul (Answer[i], KPub[0], Answer[i]);
            Client_Receive_Decrypt (Answer[i + add_i], KPri);
            mpz_add (Answer[i], Answer[i], Answer[i + add_i]);
        }
        n_tmp = n_tmp - add_i;
        add_i *= 2;
    }
    Client_Receive_Decrypt (Answer[0], KPri);
}
