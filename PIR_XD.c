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
                mpz_t Answer[(int)pow(2, (dim - 1))], mpz_t * tab_serv)
{
    int lenth_u_v = pow(MAX_GRID_SIZE, (dim - 1));
    mpz_t * tab_u_v_1 = malloc(sizeof(mpz_t) * lenth_u_v);
    mpz_t * tab_u_v_2 = malloc(sizeof(mpz_t) * lenth_u_v);
    for (int i = 0; i < lenth_u_v; i++)
    {
        mpz_init(tab_u_v_1[i]);
        mpz_init(tab_u_v_2[i]);
    }

    mpz_t tab_tmp_1[MAX_GRID_SIZE];
    mpz_t tab_tmp_2[MAX_GRID_SIZE];
    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_init(tab_tmp_1[i]);
        mpz_init(tab_tmp_2[i]);
    }

    int n_tab_cli = dim - 1;

    /* Part 1: First operation note to tab_u_v */
    for (int i = 0; i < lenth_u_v; i++)
    {

        for (int j = 0; j < MAX_GRID_SIZE; j++)
        {
            mpz_set(tab_tmp_1[j], tab_serv[i * MAX_GRID_SIZE + j]);

        } // one line copy done from server

        /* compute every table with tab_cli and change to u and v*/
        Serveur(tab_u_v_1[i], tab_tmp_1, tab_cli[n_tab_cli]);
        mpz_fdiv_qr (tab_u_v_1[i], tab_u_v_2[i], tab_u_v_1[i], KPub[0]);
    } /* Part 1 END */

    /* Part 2: separation and compute circularly */
    int count_position = 0;
    int count_uv = 0;
    for (int d = 1; d <= (dim - 2); d++)
    {
        n_tab_cli--;
        for (int i = 0; i < (lenth_u_v / (pow(2, (d - 1)))); i++)
        {
            if(count_position != (MAX_GRID_SIZE - 1))
            {
                mpz_set(tab_tmp_1[count_position], tab_u_v_1[i]);
                mpz_set(tab_tmp_2[count_position], tab_u_v_2[i]);
                count_position++;
            }
            else
            {
                mpz_set(tab_tmp_1[count_position], tab_u_v_1[i]);
                mpz_set(tab_tmp_2[count_position], tab_u_v_2[i]);
                Serveur(tab_u_v_1[count_uv], tab_tmp_1, tab_cli[n_tab_cli]);
                Serveur(tab_u_v_2[count_uv], tab_tmp_2, tab_cli[n_tab_cli]);
                count_uv++;
                count_position = 0;
            }
        } /* Compute done */

        for (int i = 0; i < (lenth_u_v / (d * MAX_GRID_SIZE)); i++) //
        {
            mpz_fdiv_qr (tab_u_v_1[i],
                         tab_u_v_1[i + (lenth_u_v / (d * MAX_GRID_SIZE))],
                         tab_u_v_1[i], KPub[0]);
            mpz_fdiv_qr (tab_u_v_2[i],
                         tab_u_v_2[i + (lenth_u_v / (d * MAX_GRID_SIZE))],
                         tab_u_v_2[i], KPub[0]);
        }
    } /* Part 2 END */

    /* Part 3: Compute the value return to client */
    count_position = 0;
    count_uv = 0;
    for (int i = 0; i < (MAX_GRID_SIZE * (pow(2, (dim - 2)))); i++)
    {
        if(count_position != (MAX_GRID_SIZE - 1))
        {
            mpz_set(tab_tmp_1[count_position], tab_u_v_1[i]);
            mpz_set(tab_tmp_2[count_position], tab_u_v_2[i]);
            count_position++;
        }
        else
        {
            mpz_set(tab_tmp_1[count_position], tab_u_v_1[i]);
            mpz_set(tab_tmp_2[count_position], tab_u_v_2[i]);
            count_position = 0;
            Serveur(Answer[count_uv], tab_tmp_1, tab_cli[0]);
            Serveur(Answer[count_uv + (int)pow(2, (dim - 2))],
                    tab_tmp_2, tab_cli[0]);

            if(count_uv < (pow(2, (dim - 2))) - 2)
            {
                count_uv+=2;
            }
            else
            {
                count_uv = 1;
            }
        }
    } /* Part 3 END */

    for (int i = 0; i < MAX_GRID_SIZE; i++)
    {
        mpz_clear(tab_tmp_1[i]);
        mpz_clear(tab_tmp_2[i]);
    }

    for (int i = 0; i < lenth_u_v; i++)
    {
        mpz_clear(tab_u_v_1[i]);
        mpz_clear(tab_u_v_2[i]);
    }
    free(tab_u_v_1);
    free(tab_u_v_2);

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
