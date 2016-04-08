#include "Paillier.h"
#include "PIR_Naive.h"
#include <stdlib.h>
#include <stdio.h>

void Build_DBMS(mpz_t tab_serv[MAX_GRID_SIZE][MAX_GRID_SIZE])
{
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        for(int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_init_set_ui(tab_serv[i][j], i * 1000 + j);
        }
    }
}

int main(int argc, char** argv)
{
    Build_DBMS(tab_serv);

    int List_request[10][2];
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 2; j++)
        {
        List_request[i][j] =  -1;
        }
    }

    int row, col, position_request = 0;
    do
    {
        printf("\nPlease input the position you want to read:\n"
                "  >(Request max 10)\n"
                "  >(To Stop typing, input -1 for x or y)\n"
                "x (abscissa) :\n");
        scanf("%d", &row);
        printf("y (ordinate) :\n");
        scanf("%d", &col);
        if(row == -1 || col == -1)
        {
            break;
        }
        else if(col >= MAX_GRID_SIZE || row >= MAX_GRID_SIZE || row < -1 || col < -1)
        {
            printf("Value exceed the limit, try again.\n\n");
        }
        else
        {
            List_request[position_request][0] = row;
            List_request[position_request][1] = col;
            position_request++;
        }
    }while(row >= MAX_GRID_SIZE || col >= MAX_GRID_SIZE || position_request < 10);

    mpz_t KPri[3];
    position_request = 0;
    do
    {
        ClientKey(KPri);
        Client_Send_Encrypt(List_request[position_request][0], List_request[position_request][1]);
        mpz_init(valueReturn);
        Serveur(valueReturn);
        Client_Receive_Decrypt(valueReturn, KPri);
        gmp_printf ("\nvalueReturn : %Zd\n", valueReturn);
        position_request++;
    } while(List_request[position_request][0] != -1);


    for(int i = 0; i < 3; i++)
    {
        mpz_clear(KPri[i]);
        mpz_clear(KPub[i]);
    }
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        for(int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_clear(tab_serv[i][j]);
            mpz_clear(tab_cli[i][j]);
        }
    }
    mpz_clear(valueReturn);
}
