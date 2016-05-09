#include "PIR_Naive.h"

/* CLIENT */

void InitKey(mpz_t LKey[6])
{
    for(int i = 0; i < 6; i++)
    {
        mpz_init(LKey[i]);
    }
    for(int i = 0; i < 3; i++)
    {
        mpz_init(KPub[i]);
    }

    getKeyList(LKey);
    getPubKey(KPub, LKey);
}

void ClientKey(mpz_t KPri[3])
{
    mpz_t LKey[6];
    InitKey(LKey);

    for(int i = 0; i < 3; i++)
    {
        mpz_init(KPri[i]);
    }

    getPriKey(KPri, LKey);

    for(int i = 0; i < 6; i++)
    {
        mpz_clear(LKey[i]);
    }
}

void Client_Send_Encrypt(mpz_t tab_cli[MAX_GRID_SIZE], int col)
{
    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_init_set_ui(tab_cli[i], 0);
        encryption(tab_cli[i], tab_cli[i], KPub);
    }

    mpz_set_ui(tab_cli[col], 1);
    encryption(tab_cli[col], tab_cli[col], KPub);
}

void Client_Receive_Decrypt(mpz_t valueReturn, mpz_t KPri[3])
{
    decryption(valueReturn, valueReturn, KPub, KPri);
}

/* SERVEUR */

void Serveur(mpz_t valueReturn, mpz_t tab_serv[], mpz_t tab_cli[])
{
    mpz_t tmp;
    mpz_init_set_ui(tmp, 0);
    mpz_set_ui(valueReturn, 1);
    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_powm(tmp, tab_cli[i], tab_serv[i], KPub[2]);
        mpz_mul(valueReturn, tmp, valueReturn);

    }
    mpz_clear(tmp);
}
