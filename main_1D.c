#include "Paillier.h"
#include "PIR_Naive.h"
#include <stdlib.h>
#include <stdio.h>

mpz_t tab_serv [MAX_GRID_SIZE];
mpz_t tab_cli [MAX_GRID_SIZE];
mpz_t valueReturn;

void Build_DBMS(mpz_t tab_serv[MAX_GRID_SIZE])
{
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_init_set_ui(tab_serv[i], i);
    }
}

int main(int argc, char** argv)
{
    Build_DBMS(tab_serv);

    int List_request[10];
    for(int i = 0; i < 10; i++)
    {
        List_request[i] =  -1;
    }

    int col, position_request = 0;
    do
    {
        printf("\nPlease input the position you want to read:\n"
                "  > (Request max 10)\n"
                "  > (To Stop typing, input -1 for y)\n"
                ">>> y (ordinate) :\n");
        scanf("%d", &col);
        if(col == -1)
        {
            break;
        }
        else if(col >= MAX_GRID_SIZE || col < -1)
        {
            printf("Value exceed the limit, try again.\n\n");
        }
        else
        {
            List_request[position_request] = col;
            position_request++;
        }
    }while(position_request < 10 || col >= MAX_GRID_SIZE);

    printf("\nComing soon...\n\n");

    mpz_t KPri[3];
    position_request = 0;
    do
    {
        ClientKey(KPri);
        Client_Send_Encrypt(tab_cli, List_request[position_request]);
        mpz_init(valueReturn);
        Serveur(valueReturn, tab_serv, tab_cli);
        Client_Receive_Decrypt(valueReturn, KPri);
        gmp_printf ("\nvalueReturn : %Zd\n", valueReturn);
        position_request++;
    } while(position_request < 10 && List_request[position_request] != -1);


    for(int i = 0; i < 3; i++)
    {
        mpz_clear(KPri[i]);
        mpz_clear(KPub[i]);
    }
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_clear(tab_cli[i]);
        mpz_clear(tab_serv[i]);
    }
    mpz_clear(valueReturn);
}
