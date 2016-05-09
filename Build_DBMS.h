#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "main.h"

void Build_DBMS(mpz_t tab_serv[MAX_GRID_SIZE]);
void Build_DBMS_2D(mpz_t tab_serv_2D[MAX_GRID_SIZE][MAX_GRID_SIZE]);
void Build_DBMS_3D(mpz_t tab_serv_3D[MAX_GRID_SIZE][MAX_GRID_SIZE][MAX_GRID_SIZE]);
void Build_DBMS_xD(int lenth, mpz_t * tab_serv_xD);
