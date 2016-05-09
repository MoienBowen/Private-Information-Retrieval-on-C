#include "Paillier.h"
#include "PIR_Naive.h"
#include "PIR_2D.h"
#include <stdlib.h>
#include <stdio.h>

mpz_t tab_serv_2D [MAX_GRID_SIZE][MAX_GRID_SIZE];
mpz_t tab_cli_row [MAX_GRID_SIZE];
mpz_t tab_cli_col [MAX_GRID_SIZE];
int row, col;


void Build_DBMS_2D(mpz_t tab_serv_2D[MAX_GRID_SIZE][MAX_GRID_SIZE])
{
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        for(int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_init_set_ui(tab_serv_2D[i][j], 10000+i*100+j);
        }
    }
}

int main()
{

    Build_DBMS_2D(tab_serv_2D);
    do {
        if (row > 4 || col > 4)
        {
            printf("\nValue of row or col too big, please retry with another.\n");
        }
        printf("\nPlease input the position you want to read (Request max 4):\n"
                ">>> x (abscissa) : ");
        scanf("%d", &row);
        printf(">>> y (ordinate) : ");
        scanf("%d", &col);
    } while(row > 4 || col > 4);


    printf("\nComing soon...\n\n");

    mpz_t KPri[3], u, v;

    ClientKey(KPri);
    Client_Send_Encrypt_2D(tab_cli_row, tab_cli_col, row, col);
    mpz_init(u);
    mpz_init(v);
    Serveur_2D(u, v, tab_serv_2D, tab_cli_row, tab_cli_col);
    Client_Receive_Decrypt_2D(v, u, KPri);
    gmp_printf ("\nvalueReturn : %Zd\n", u);

    for(int i = 0; i < 3; i++)
    {
        mpz_clear(KPri[i]);
        mpz_clear(KPub[i]);
    }
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_clear(tab_cli_col[i]);
        mpz_clear(tab_cli_row[i]);
        for(int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_clear(tab_serv_2D[i][j]);
        }
    }
}
