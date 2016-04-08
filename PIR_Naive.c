#include "Paillier.h"
#include "PIR_Naive.h"
#include <stdlib.h>
#include <stdio.h>

/*
********************
     CLIENT
********************
*/

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

void Client_Send_Encrypt(int row, int col)
{
    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        for(int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_init_set_ui(tab_cli[i][j], 0);
            encryption(tab_cli[i][j], tab_cli[i][j], KPub);
        }
    }

    mpz_set_ui(tab_cli[row][col], 1); /* input by client */
    encryption(tab_cli[row][col], tab_cli[row][col], KPub);
}

void Client_Receive_Decrypt(mpz_t valueReturn, mpz_t KPri[3])
{
    decryption(valueReturn, valueReturn, KPub, KPri);
}

/*
********************
     SERVEUR
********************
*/

void Serveur(mpz_t valueReturn)
{
    mpz_t tmp;
    mpz_init_set_ui(tmp, 0);
    mpz_set_ui(valueReturn, 1);
    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        for (int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_powm(tmp, tab_cli[i][j], tab_serv[i][j], KPub[2]);
            mpz_mul(valueReturn, tmp, valueReturn);
        }
    }
    mpz_clear(tmp);
}


// int main(int argc, char** argv){
//
//     for(int i = 0; i < MAX_GRID_SIZE; i++)
//     {
//         mpz_init_set_ui(tab_serv[i], i);
//     }
//
//     int col;
//     do
//     {
//         printf("Please input the position you want to read:\n");
//         scanf("%d", &col);
//         if(col >= MAX_GRID_SIZE)
//         {
//             printf("Value exceed the limit, try again.\n\n");
//         }
//     }while(col >= MAX_GRID_SIZE);
//
//     mpz_t LKey[6], KPri[3];
//
//     ClientKey(KPri);
//     Client_Send_Encrypt(col);
//     mpz_init(valueReturn);
//     Serveur(valueReturn);
//     Client_Receive_Decrypt(valueReturn, KPri);
//     gmp_printf ("\nvalueReturn : %Zd\n", valueReturn);
//
//     for(int i = 0; i < 3; i++)
//     {
//         mpz_clear(KPri[i]);
//         mpz_clear(KPub[i]);
//     }
//     for(int i = 0; i < 10; i++)
//     {
//         mpz_clear(tab_serv[i]);
//         mpz_clear(tab_cli[i]);
//     }
//     mpz_clear(valueReturn);
// }
