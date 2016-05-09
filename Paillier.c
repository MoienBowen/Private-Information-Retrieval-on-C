/**
 * Paviller CryptoSystem on C with gmp.h
 * Feb. 19, 2016
 **/
#include "Paillier.h"

void getRandomPrime(mpz_t numrandom)
{
    long seed;
    gmp_randstate_t staterandom;
    gmp_randinit_default(staterandom);
    mpz_t max, min, maxtmp;

    mpz_init(max);
    mpz_ui_pow_ui (max, 2, 512);
    mpz_sub_ui(max, max, 1);
    mpz_init(min);
    mpz_ui_pow_ui (min, 2, 511);
    mpz_init(maxtmp);
    mpz_sub(maxtmp, max, min); /* difference between 2^511 and 2^512 - 1 */

    time(&seed); /* use time (in seconds) to set the value of seed */
    gmp_randseed_ui(staterandom, seed);

    do
    {
        mpz_urandomm(numrandom, staterandom, maxtmp);
        mpz_add(numrandom, numrandom, min);
        mpz_nextprime(numrandom, numrandom);
    }
    while(mpz_cmp(numrandom, max) >= 0);

    mpz_clear(max);
    mpz_clear(min);
    mpz_clear(maxtmp);
    gmp_randclear(staterandom);
}

/* For random r, non prime accepted */
void getRandom(mpz_t numrandom)
{
    long seed;
    gmp_randstate_t staterandom;
    gmp_randinit_default(staterandom);

    mpz_t max, min, maxtmp;

    mpz_init(max);
    mpz_ui_pow_ui (max, 2, 512);
    mpz_sub_ui(max, max, 1);
    mpz_init(min);
    mpz_ui_pow_ui (min, 2, 511);
    mpz_init(maxtmp);
    mpz_sub(maxtmp, max, min); /* difference between 2^511 and 2^512 - 1 */

    time(&seed); /* use time (in seconds) to set the value of seed */
    gmp_randseed_ui(staterandom, seed);

    do
    {
        mpz_urandomm(numrandom, staterandom, maxtmp);
        mpz_add(numrandom, numrandom, min);
    }
    while(mpz_cmp(numrandom, max) >= 0);

    mpz_clear(max);
    mpz_clear(min);
    mpz_clear(maxtmp);
    gmp_randclear(staterandom);
}

void getKeyList(mpz_t LKey[6])
{
    mpz_t p, q, lambdan, n, nsquare, g, psubone, qsubone, test;
    mpz_init(p);
    mpz_init(q);
    mpz_init(lambdan);
    mpz_init(n);
    mpz_init(nsquare);
    mpz_init(g);
    mpz_init(test);
    mpz_init(psubone);
    mpz_init(qsubone);

    /* p and q prime */
    getRandomPrime(p);
    getRandomPrime(q);
    while (mpz_cmp(p, q) == 0)
    {
        getRandomPrime(p);
    }
    /* n = p * q */
    mpz_mul(n, p, q);
    mpz_mul(nsquare, n, n);
    /* λ(n) = lcm(p - 1, q - 1) */
    mpz_sub_ui(psubone, p, 1);
    mpz_sub_ui(qsubone, q, 1);
    mpz_lcm(lambdan, qsubone, psubone);
    mpz_gcd(test, n, lambdan);

    if(mpz_cmp_ui(test, 1) == 0)
    {
        /* g, ord n^2 (g) = n * α, α ∈ Zλ(n) and α|λ(n) */
        /* for time saving, g = 1 + n */
        mpz_add_ui(g, n, 1);
        mpz_set(LKey[0], p);
        mpz_set(LKey[1], q);
        mpz_set(LKey[2], lambdan);
        mpz_set(LKey[3], n);
        mpz_set(LKey[4], g);
        mpz_set(LKey[5], nsquare);
    }

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(lambdan);
    mpz_clear(n);
    mpz_clear(nsquare);
    mpz_clear(g);
    mpz_clear(test);
    mpz_clear(psubone);
    mpz_clear(qsubone);
}

void getPubKey (mpz_t KPub[3], mpz_t* LKey)
{
    /* Public Keys: n, g and n^2 */
    mpz_set(KPub[0], LKey[3]);
    mpz_set(KPub[1], LKey[4]);
    mpz_set(KPub[2], LKey[5]);

}

void getPriKey (mpz_t KPri[3], mpz_t* LKey)
{
    /* Private Keys: p, q and λ(n) */
    mpz_set(KPri[0], LKey[0]);
    mpz_set(KPri[1], LKey[1]);
    mpz_set(KPri[2], LKey[2]);
}

/* msg_secu = g^m * r^n (mod n) */
void encryption(mpz_t msg_secu, mpz_t m, mpz_t* KPub)
{
    mpz_t gpowm, r, rpown, tmp;
    mpz_init(gpowm);
    mpz_init(r);
    mpz_init(rpown);
    mpz_init(tmp);

    getRandom(r);
    mpz_powm(gpowm, KPub[1], m, KPub[2]);
    mpz_powm(rpown, r, KPub[0], KPub[2]);
    mpz_mul(tmp, gpowm, rpown);
    mpz_mod(msg_secu, tmp, KPub[2]);

    mpz_clear(gpowm);
    mpz_clear(r);
    mpz_clear(rpown);
    mpz_clear(tmp);
}

/* L(u) = (u - 1) / n, ∀u ∈ Sn = {u | 0 < u < n^2 and u ≡ 1 (mod n)}  */
void lOfDecrypt(mpz_t resultL, mpz_t u, mpz_t* KPub)
{
    mpz_t resultLtmp;
    mpz_init(resultLtmp);

    mpz_sub_ui(resultLtmp, u, 1);
    mpz_divexact(resultL, resultLtmp, KPub[0]);

    mpz_clear(resultLtmp);
}

/* msg_clear = L(c^λ(n) (mod n^2)) / L(g^λ(n) (mod n^2)) (mod n) */
void decryption(mpz_t msg_clear, mpz_t msg_secu, mpz_t* KPub, mpz_t* KPri)
{
    mpz_t abovetmp, above, belowtmp, below, tmp, left, right;

    mpz_init(abovetmp);
    mpz_init(above);
    mpz_init(belowtmp);
    mpz_init(below);
    mpz_init(left);
    mpz_init(right);
    mpz_init(tmp);

    mpz_powm(above, msg_secu, KPri[2], KPub[2]);
    mpz_powm(below, KPub[1], KPri[2], KPub[2]);
    lOfDecrypt(abovetmp, above, KPub);
    //lOfDecrypt(belowtmp, below, KPub); // result equal to λ(n)

    // a / b mod n = ((a mod n) * (b^(-1) mod n)) mod n
    mpz_mod(above, abovetmp, KPub[0]);
    // mpz_invert(below, belowtmp, KPub[0]); simple with line below
    mpz_invert(below, KPri[2], KPub[0]); // inverse of λ(n) mod n
    mpz_mul(tmp, above, below);
    mpz_mod(msg_clear, tmp, KPub[0]);

    mpz_clear(abovetmp);
    mpz_clear(above);
    mpz_clear(belowtmp);
    mpz_clear(below);
    mpz_clear(tmp);
    mpz_clear(left);
    mpz_clear(right);
}

// int main(int argc, char** argv)
// {
//     mpz_t mym, LKey[6], KPub[3], KPri[3], msg_secu, msg_clear;
//     mpz_init(mym);
//     mpz_init(msg_secu);
//     mpz_init(msg_clear);
//
//     /* Manual input m test */
//     // gmp_printf("\nPlease Enter your massage in number:\n");
//     // gmp_scanf("%Zd", mym);
//
//     /* Random m input */
//     getRandom(mym);
//
//     /* Show random m */
//     gmp_printf ("\n%s\n%Zd\n", "The random number m (orginal message) is:", mym);
//
//     for(int i = 0; i < 6; i++)
//     {
//         mpz_init(LKey[i]);
//     }
//     for(int i = 0; i < 3; i++)
//     {
//         mpz_init(KPub[i]);
//         mpz_init(KPri[i]);
//     }
//
//     /* How to initialize array like this ? */
//     // mpz_array_init(LKey[0], 5, 1024);
//     // mpz_array_init(KPub[0], 2, 1024);
//     // mpz_array_init(KPri[0], 3, 1024);
//
//     getKeyList(LKey);
//     getPubKey(KPub, LKey);
//     getPriKey(KPri, LKey);
//
//     /* Show the Key List*/
//     // for(int i = 0; i < 6; i++)
//     // {
//     //     gmp_printf ("\nKey [ %d ] (p, q, λ(n), n g) is: \n%Zd\n", i, LKey[i]);
//     // }
//
//     encryption(msg_secu, mym, KPub);
//     decryption(msg_clear, msg_secu, KPub, KPri);
//
//     //gmp_printf ("\n\nThe encryption message: \n%Zd\n", msg_secu);
//     gmp_printf ("\n\nThe decryption message: \n%Zd\n", msg_clear);
//
//     for(int i = 0; i < 6; i++)
//     {
//         mpz_clear(LKey[i]);
//     }
//     for(int i = 0; i < 3; i++)
//     {
//         mpz_clear(KPub[i]);
//         mpz_clear(KPri[i]);
//     }
//
//     mpz_clear(mym);
//     mpz_clear(msg_secu);
//     mpz_clear(msg_clear);
//
//     return 0;
// }

/**
 * test with:
 * gcc Paillier.c -lgmp && ./a.out
 **/

/* time in 0.06 s and 1.05 s */
