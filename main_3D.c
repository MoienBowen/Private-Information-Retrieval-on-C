#include "Paillier.h"
#include "PIR_Naive.h"
#include "PIR_2D.h"
#include "PIR_3D.h"


mpz_t tab_serv_3D [MAX_GRID_SIZE][MAX_GRID_SIZE][MAX_GRID_SIZE];
mpz_t tab_cli_row [MAX_GRID_SIZE];
mpz_t tab_cli_col [MAX_GRID_SIZE];
mpz_t tab_cli_layer [MAX_GRID_SIZE];
int row, col, layer;


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


int main()
{

    Build_DBMS_3D(tab_serv_3D);
    do {
        if (row > 4 || col > 4 || layer > 4)
        {
            printf("\nValue of row or col too big, please retry with another.\n");
        }
        printf("\nPlease input the position you want to read (Request max 4):\n"
                ">>> x (abscissa) : ");
        scanf("%d", &row);
        printf(">>> y (ordinate) : ");
        scanf("%d", &col);
        printf(">>> z (applicate) : ");
        scanf("%d", &layer);
    } while(row > 4 || col > 4 || layer > 4);


    printf("\nComing soon...\n\n");

    mpz_t KPri[3], uu, uv, vu, vv;

    ClientKey(KPri);
    Client_Send_Encrypt_3D(tab_cli_row, tab_cli_col, tab_cli_layer,
                            row, col, layer);
    mpz_init(uu);
    mpz_init(uv);
    mpz_init(vu);
    mpz_init(vv);

    Serveur_3D(uu, uv, vu, vv, tab_serv_3D,
                tab_cli_row, tab_cli_col, tab_cli_layer);

    Client_Receive_Decrypt_3D(uu, uv, vu, vv, KPri);

    gmp_printf ("\nvalueReturn : %Zd\n", uu);

    for(int i = 0; i < 3; i++)
    {
        mpz_clear(KPri[i]);
        mpz_clear(KPub[i]);
    }
    for(int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_clear(tab_cli_col[i]);
        mpz_clear(tab_cli_row[i]);
        mpz_clear(tab_cli_layer[i]);
        for(int j = 0; j < MAX_GRID_SIZE; j++)
        {
            for(int k = 0; k < MAX_GRID_SIZE; k++)
            {
                mpz_clear(tab_serv_3D[i][j][k]);
            }
        }
    }
    mpz_clear(uu);
    mpz_clear(uv);
    mpz_clear(vu);
    mpz_clear(vv);
}
