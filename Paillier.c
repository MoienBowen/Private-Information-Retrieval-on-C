/**
 * Paviller CryptoSystem on C with gmp.h
 * Feb. 11, 2016
 **/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

void getRandom(mpz_t numrandom)
{
    long seed;
    gmp_randstate_t staterandom;
    gmp_randinit(staterandom, 0, 128);

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
}

void getKeyList(mpz_t LKey[5])
{
    mpz_t p, q, lambdan, n, g, psubone, qsubone, test;
    mpz_init(p);
    mpz_init(q);
    mpz_init(lambdan);
    mpz_init(n);
    mpz_init(g);
    mpz_init(test);
    mpz_init(psubone);
    mpz_init(qsubone);

    getRandom(p);
    getRandom(q);
    mpz_sub_ui(psubone, p, 1);
    mpz_sub_ui(qsubone, q, 1);
    while (mpz_cmp(p, q) == 0)
    {
        getRandom(p);
    }

    mpz_lcm(lambdan, psubone, qsubone);
    mpz_mul(n, p, q);

    mpz_gcd (test, n, lambdan);

    if(mpz_cmp_ui(test, 1) == 0)
    {
        mpz_add_ui(g, n, 1);
        mpz_set(LKey[0], p);
        mpz_set(LKey[1], q);
        mpz_set(LKey[2], lambdan);
        mpz_set(LKey[3], n);
        mpz_set(LKey[4], g);
    }

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(lambdan);
    mpz_clear(n);
    mpz_clear(g);
    mpz_clear(test);
    mpz_clear(psubone);
    mpz_clear(qsubone);
}

void getPubKey (mpz_t KPub[2], mpz_t* LKey)
{
    /* Public Keys: n and g */
    mpz_set(KPub[0], LKey[3]);
    mpz_set(KPub[1], LKey[4]);
}

void getPriKey (mpz_t KPri[3], mpz_t* LKey)
{
    /* Private Keys: p, q and lamdba(n) */
    mpz_set(KPri[0], LKey[0]);
    mpz_set(KPri[1], LKey[1]);
    mpz_set(KPri[2], LKey[2]);
}

void encryption(mpz_t msg_secu, mpz_t m, mpz_t* KPub)
{
    mpz_t gpowm, rtmp, rpown, nsquare, tmp;
    mpz_init(gpowm);
    mpz_init(rtmp);
    mpz_init(rpown);
    mpz_init(tmp);
    mpz_init(nsquare);

    getRandom(rtmp);
    mpz_mul(nsquare, KPub[0], KPub[0]);

    mpz_powm(gpowm, KPub[1], m, nsquare);
    mpz_powm(rpown, rtmp, KPub[0], nsquare);

    mpz_mul(tmp, gpowm, rpown);
    mpz_mod(msg_secu, tmp, nsquare);

    mpz_clear(gpowm);
    mpz_clear(rtmp);
    mpz_clear(rpown);
    mpz_clear(nsquare);
    mpz_clear(tmp);
}

void lOfDecoding(mpz_t resultL, mpz_t u, mpz_t* KPub)
{
    mpz_t resultLtmp;
    mpz_init(resultLtmp);

    mpz_sub_ui(resultLtmp, u, 1);
    mpz_div(resultL, resultLtmp, KPub[0]);

    mpz_clear(resultLtmp);
}

void decryption(mpz_t msg_clear, mpz_t msg_secu, mpz_t* KPub, mpz_t* KPri)
{
    mpz_t nsquare, abovetmp, above, belowtmp, below, resultdiv, tmp;

    mpz_init(nsquare);
    mpz_init(abovetmp);
    mpz_init(above);
    mpz_init(belowtmp);
    mpz_init(below);
    mpz_init(resultdiv);
    mpz_init(tmp);

    mpz_mul(nsquare, KPub[0], KPub[0]);
    mpz_powm(abovetmp, msg_secu, KPri[2], nsquare);
    mpz_powm(belowtmp, msg_secu, KPub[1], nsquare);
    lOfDecoding(above, abovetmp, KPub);
    lOfDecoding(below, belowtmp, KPub);
    mpz_mod(above, above, KPub[0]);
    mpz_powm_ui(below, below, (-1), KPub[0]);
    mpz_mul(tmp, above, below);
    mpz_mod(msg_clear, tmp, KPub[0]);

    mpz_clear(nsquare);
    mpz_clear(abovetmp);
    mpz_clear(above);
    mpz_clear(belowtmp);
    mpz_clear(below);
    mpz_clear(resultdiv);
    mpz_clear(tmp);
}

int main(int argc, char** argv)
{
    mpz_t mym, LKey[5], KPub[2], KPri[3], msg_secu, msg_clear;
    mpz_init(mym);
    mpz_init(msg_secu);
    mpz_init(msg_clear);

    /* Manual input m test */
    // gmp_printf("\nPlease Enter your massage in number:\n");
    // gmp_scanf("%Zd", mym);

    /* Random m input */
    getRandom(mym);

    /* Show random m */
    gmp_printf ("\n%s\n%Zd\n", "The random number m is:", mym);

    for(int i = 0; i < 5; i++)
    {
        mpz_init(LKey[i]);
    }
    for(int i = 0; i < 2; i++)
    {
        mpz_init(KPub[i]);
    }
    for(int i = 0; i < 3; i++)
    {
        mpz_init(KPri[i]);
    }
    // mpz_array_init(LKey[0], 5, 1024); // if initialize array like this ?
    // mpz_array_init(KPub[0], 2, 1024);
    // mpz_array_init(KPri[0], 3, 1024);

    getKeyList(LKey);
    getPubKey(KPub, LKey);
    getPriKey(KPri, LKey);

    /* Random test */
    // mpz_t test;
    // mpz_init(test);
    // getRandom(test);
    // gmp_printf ("\n%s is: \n%Zd\n", "The random number (mpz_t)", test);
    // mpz_clear(test);

    /* Show test for Key List*/
    // for(int i = 0; i < 5; i++)
    // {
    //     gmp_printf ("\nKey[ %d ] (mpz_t) is: \n%Zd\n", i, LKey[i]);
    // }

    encryption(msg_secu, mym, KPub);
    decryption(msg_clear, msg_secu, KPub, KPri);

    // gmp_printf ("\nThe message encryption: \n%Zd", msg_secu, ".\n");
    gmp_printf ("\nThe message decryption: \n%Zd", msg_clear, ".\n");

    for(int i = 0; i < 5; i++)
    {
        mpz_clear(LKey[i]);
    }
    for(int i = 0; i < 2; i++)
    {
        mpz_clear(KPub[i]);
    }
    for(int i = 0; i < 3; i++)
    {
        mpz_clear(KPri[i]);
    }

    mpz_clear(mym);
    mpz_clear(msg_secu);
    mpz_clear(msg_clear);

    return 0;
}
