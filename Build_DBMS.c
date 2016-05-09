#include "Build_DBMS.h"

void Build_DBMS(mpz_t tab_serv[MAX_GRID_SIZE])
{
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_init_set_ui(tab_serv[i], i);
    }
}

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

void Build_DBMS_3D(mpz_t tab_serv_3D[MAX_GRID_SIZE][MAX_GRID_SIZE][MAX_GRID_SIZE])
{
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        for(int j = 0; j < MAX_GRID_SIZE; j++)
        {
            for(int k = 0; k < MAX_GRID_SIZE; k++)
            {
                mpz_init_set_ui(tab_serv_3D[i][j][k], 1000000+i*10000+j*100+k);
            }
        }
    }
}

void Build_DBMS_xD(int lenth, mpz_t * tab_serv_xD)
{

    for(int i = 0; i < (lenth / MAX_GRID_SIZE); i++)
    {
        for(int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_init_set_ui(tab_serv_xD[i * MAX_GRID_SIZE + j], (1000000 + 1000 * i + j));
        }
    }
}
