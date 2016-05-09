#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

void getRandomPrime(mpz_t numrandom); /* 2^511 and 2^512 - 1 */

void getRandom(mpz_t numrandom); /* 2^511 and 2^512 - 1 */

void getKeyList(mpz_t LKey[5]);

void getPriKey (mpz_t KPri[3], mpz_t* LKey);
/* Private Keys: p, q and λ(n) */

void getPubKey (mpz_t KPub[3], mpz_t* LKey);
/* Public Keys: n, g and n^2 */

void encryption(mpz_t msg_secu, mpz_t m, mpz_t* KPub);
/* msg_secu = g^m * r^n (mod n) */

void lOfDecrypt(mpz_t resultL, mpz_t u, mpz_t* KPub);
/* L(u) = (u - 1) / n, ∀u ∈ Sn = {u | 0 < u < n^2 and u ≡ 1 (mod n)}  */

void decryption(mpz_t msg_clear, mpz_t msg_secu, mpz_t* KPub, mpz_t* KPri);
/* msg_clear = L(c^λ(n) (mod n^2)) / L(g^λ(n) (mod n^2)) (mod n) */

/* void InitKey(mpz_t LKey[5], mpz_t KPub[2], mpz_t KPri[3]); */
