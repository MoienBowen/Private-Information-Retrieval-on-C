#include "PIR_XD.h"
#include "PIR_3D.h"
#include "Build_DBMS.h"

int main()
{
    mpz_t KPri[3];
    ClientKey(KPri);

    int option = 0;
    Choice:
    do
    {
        printf("\n---------------------------------\n"
                "Enter the dimension of your DBMS:\n"
                "[ 1] for one dimension\n"
                "[ 2] for two dimensions\n"
                "[ 3] for three dimensions\n"
                "[ 4] for x dimensions (x >= 3)\n"
                "[-1] for exit (you could input -1 for exit at anyplace)\n"
                "\nYour option: ");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
            {
                mpz_t tab_serv [MAX_GRID_SIZE];
                mpz_t tab_cli [MAX_GRID_SIZE];
                mpz_t valueReturn;
                int col = 0;

                do {
                    if (col > (MAX_GRID_SIZE - 1))
                    {
                        printf("\nValue of row or col too big, "
                                "please retry with another.\n");
                    }
                    printf("\nPlease input the position you want to read "
                            "(Request max %d):\n"
                            ">>> x (abscissa) : ", MAX_GRID_SIZE - 1);
                    scanf("%d", &col);
                } while(col > (MAX_GRID_SIZE - 1));

                Build_DBMS(tab_serv);

                Client_Send_Encrypt(tab_cli, col);
                mpz_init(valueReturn);
                Serveur(valueReturn, tab_serv, tab_cli);
                Client_Receive_Decrypt(valueReturn, KPri);
                gmp_printf ("\nvalueReturn : %Zd\n", valueReturn);

                for(int i = 0; i < MAX_GRID_SIZE; i++)
                {
                    mpz_clear(tab_cli[i]);
                    mpz_clear(tab_serv[i]);
                }
                mpz_clear(valueReturn);
                break;
            }

            case 2:
            {
                mpz_t tab_serv_2D [MAX_GRID_SIZE][MAX_GRID_SIZE];
                mpz_t tab_cli_row [MAX_GRID_SIZE];
                mpz_t tab_cli_col [MAX_GRID_SIZE];
                int row = 0, col = 0;

                do {
                    if (row > (MAX_GRID_SIZE - 1) || col > (MAX_GRID_SIZE - 1))
                    {
                        printf("\nValue of row or col too big, "
                                "please retry with another.\n");
                    }
                    printf("\nPlease input the position you want to read "
                            "(Request max %d):\n"
                            ">>> x (abscissa) : ", MAX_GRID_SIZE - 1);
                    scanf("%d", &row);
                    printf(">>> y (ordinate) : ");
                    scanf("%d", &col);
                } while(row > (MAX_GRID_SIZE - 1) || col > (MAX_GRID_SIZE - 1));

                Build_DBMS_2D(tab_serv_2D);

                mpz_t u, v;

                Client_Send_Encrypt_2D(tab_cli_row, tab_cli_col, row, col);
                mpz_init(u);
                mpz_init(v);
                Serveur_2D(u, v, tab_serv_2D, tab_cli_row, tab_cli_col);
                Client_Receive_Decrypt_2D(v, u, KPri);
                gmp_printf ("\nvalueReturn : %Zd\n", u);

                for(int i = 0; i < MAX_GRID_SIZE; i++)
                {
                    mpz_clear(tab_cli_col[i]);
                    mpz_clear(tab_cli_row[i]);
                    for(int j = 0; j < MAX_GRID_SIZE; j++)
                    {
                        mpz_clear(tab_serv_2D[i][j]);
                    }
                }
                break;
            }
            case 3:
            {
                mpz_t tab_serv_3D [MAX_GRID_SIZE][MAX_GRID_SIZE][MAX_GRID_SIZE];
                mpz_t tab_cli_row [MAX_GRID_SIZE];
                mpz_t tab_cli_col [MAX_GRID_SIZE];
                mpz_t tab_cli_layer [MAX_GRID_SIZE];
                int row = 0, col = 0, layer = 0;

                do {
                    if (row > (MAX_GRID_SIZE - 1) || col > (MAX_GRID_SIZE - 1)
                        || layer > (MAX_GRID_SIZE - 1))
                    {
                        printf("\nValue of row or col too big, "
                                "please retry with another.\n");
                    }
                    printf("\nPlease input the position you want to read "
                            "(Request max %d):\n"
                            ">>> x (abscissa) : ", MAX_GRID_SIZE - 1);
                    scanf("%d", &row);
                    printf(">>> y (ordinate) : ");
                    scanf("%d", &col);
                    printf(">>> z (applicate) : ");
                    scanf("%d", &layer);
                } while(row > (MAX_GRID_SIZE - 1) || col > (MAX_GRID_SIZE - 1)
                        || layer > (MAX_GRID_SIZE - 1));

                Build_DBMS_3D(tab_serv_3D);

                mpz_t uu, uv, vu, vv;

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
                break;
            }

            case 4:
            {
                int dim = 0;
                printf("\nPlease input the dimension of your data in server: \n");
                scanf("%d", &dim);
                if(dim == -1)
                {
                    goto bye;
                }
                if(dim < 3)
                {
                    goto Choice;
                }

                int lenth = pow(MAX_GRID_SIZE, dim);
                mpz_t * tab_serv = malloc(sizeof(mpz_t) * lenth);
                Build_DBMS_xD(lenth, tab_serv);

                mpz_t ** tab_cli = malloc(sizeof(mpz_t *) * dim);
                int positions[dim];


                printf("\nPlease input the position you want to read "
                        "(Request max %d):\n", MAX_GRID_SIZE - 1);
                for(int i = 0; i < dim; i++)
                {
                    do {
                        printf("The %d element of your coordinates: ", i + 1);
                        scanf("%d", &positions[i]);
                    }while(positions[i] > (MAX_GRID_SIZE - 1) );
                    if(positions[i] == -1)
                    {
                        goto bye;
                    }
                }

                Client_Send_Encrypt_xD(dim, tab_cli, positions);

                mpz_t Answer[(int)pow(2, (dim - 1))];
                for (int j = 0; j < (int)pow (2, (dim - 1)); j++)
                {
                    mpz_init (Answer[j]);
                }
                Serveur_xD(dim, tab_cli, Answer, tab_serv);

                Client_Receive_Decrypt_xD(dim, Answer, KPri);

                gmp_printf ("\nvalueReturn : %Zd\n", Answer[0]);

                /* Free part */
                for(int i = 0; i < (int)lenth; i++)
                {
                    mpz_clear(tab_serv[i]);
                }
                free(tab_serv);

                for(int i = 0; i < dim; i++)
                {
                    free(tab_cli[i]);
                }
                free(tab_cli);

                for (int j = 0; j < (int)pow (2, (dim - 1)); j++)
                {
                    mpz_clear(Answer[j]);
                }

                break;
            }

            case -1:
            {
                bye:
                {
                    printf("\nBye ! :)\n\n");
                    exit(EXIT_SUCCESS);
                }
            }

            default:
            {
                printf("\nError: Wrong option, retry with a valid option !\n");
                break;
            }
        }
    }while(option != -1);

    for(int i = 0; i < 3; i++)
    {
        mpz_clear(KPri[i]);
        mpz_clear(KPub[i]);
    }
}
