#include "Paillier.h"
#include "main.h"

mpz_t KPub [3];

void InitKey(mpz_t LKey[6]);
void ClientKey(mpz_t KPri[3]);
void Client_Send_Encrypt(mpz_t tab_cli[], int col);
void Client_Receive_Decrypt(mpz_t valueReturn, mpz_t KPri[3]);
void Serveur(mpz_t valueReturn, mpz_t tab_serv[], mpz_t tab_cli[]);
