#define MAX_GRID_SIZE 64

mpz_t tab_serv [MAX_GRID_SIZE][MAX_GRID_SIZE];
mpz_t tab_cli [MAX_GRID_SIZE][MAX_GRID_SIZE];
mpz_t KPub [3];
mpz_t valueReturn;

void InitKey(mpz_t LKey[6]);
void ClientKey(mpz_t KPri[3]);
void Client_Send_Encrypt(int row, int col);
void Client_Receive_Decrypt(mpz_t valueReturn, mpz_t KPri[3]);
void Serveur(mpz_t valueReturn);
