#include "Paillier.h"
#include "PIR_Naive.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void Client_Send_Encrypt_xD(int dim, mpz_t ** tab_cli,
                            int positions[dim]);

void Serveur_xD(int dim, mpz_t ** tab_cli,
                mpz_t Answer[(int)pow(2, (dim - 1))], mpz_t * tab_serv);

void Client_Receive_Decrypt_xD(int dim, mpz_t Answer[(int)pow(2, (dim - 1))],
                    mpz_t KPri[3]);
