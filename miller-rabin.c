#include <stdio.h>
#include "gmp.h"
#include <stdbool.h>
#include <time.h>

#define ARR_SIZE 12
#define HEX 16
#define NUM_ROUNDS 15

bool millerRabinTest(mpz_t num, mpz_t d, mpz_t random)
{
  mpz_t x, num_minus_1;
  unsigned long int i, seed;

  bool ret_value = false;

  mpz_init(x);
  mpz_init(num_minus_1);

  mpz_sub_ui(num_minus_1, num, 1);

  mpz_powm(x, random, d, num);

  if (!mpz_cmp_ui(x, 1) || !mpz_cmp(x, num_minus_1))
  {
    ret_value = true;
    goto finally;
  }

  while (mpz_cmp(d, num_minus_1))
  {
    mpz_mul(x, x, x);
    mpz_mod(x, x, num);
    mpz_mul_ui(d, d, 2);

    if (!mpz_cmp_ui(x, 1))
    {
      ret_value = false;
      goto finally;
    }
    if (!mpz_cmp(x, num_minus_1))
    {
      ret_value = true;
      goto finally;
    }
  }
finally:
  mpz_clear(x);
  mpz_clear(num_minus_1);
  return ret_value;
}

bool isPrime(mpz_t num, int k)
{
  if (!mpz_cmp_ui(num, 2) || !_mpz_cmp_ui(num, 3))
    return true;
  if (!mpz_cmp_ui(num, 4))
    return false;

  mpz_t d, random, max;
  gmp_randstate_t state;
  bool ret_val = true;

  mpz_init(d);
  mpz_init(max);
  mpz_init(random);

  mpz_sub_ui(d, num, 1);
  mpz_sub_ui(max, num, 4);

  gmp_randinit_mt(state);
  gmp_randseed_ui(state, time(NULL));

  while (!mpz_fdiv_ui(d, 2))
    mpz_fdiv_q_ui(d, d, 2);

  for (int i = 0; i < k; i++)
  {
    mpz_urandomm(random, state, max);
    mpz_add_ui(random, random, 2);

    if (!millerRabinTest(num, d, random))
    {
      ret_val = false;
      goto finally;
    }
  }
finally:
  mpz_clear(d);
  mpz_clear(random);
  mpz_clear(max);
  gmp_randclear(state);
  return ret_val;
}

int main()
{
  // char intStr[1024];
  char intArr[ARR_SIZE][1024] = {
      "108C1",
      "6CE45",
      "17C79D",
      "1B206B",
      "A98AC6D",
      "7CAB1C23",
      "97340264E77E568659",
      "5AD789FB37AADE289F",
      "7D25FB921140550EBFB0D84B",
      "34BA3709DF728A732FDB1787",
      "62A6F3AFC5E548D4EC00DEB0873FDE7386C10994190130A7",
      "D37ADA29632ADDE177CE24E4671D5F59DEDB7E193EA8B117"};

  mpz_t num;
  mpz_init(num);

  // printf("Enter a number: ");
  // scanf("%1023s", intStr);

  for (int i = 0; i < ARR_SIZE; i++)
  {
    int flag = mpz_set_str(num, intArr[i], HEX);

    printf("Number: 0x");
    mpz_out_str(stdout, HEX, num);

    if (isPrime(num, NUM_ROUNDS))
      printf("\nResult: Prime");
    else
      printf("\nResult: Composite");
    printf("\n\n");
  }

  mpz_clear(num);
}

/*

Output:

Pritheesh's Air:Spring 2019 pritheesh$ gcc miller-rabin.c -lgmp
Pritheesh's Air:Spring 2019 pritheesh$ ./a.out 
Number: 0x108c1
Result: Prime

Number: 0x6ce45
Result: Composite

Number: 0x17c79d
Result: Composite

Number: 0x1b206b
Result: Prime

Number: 0xa98ac6d
Result: Prime

Number: 0x7cab1c23
Result: Composite

Number: 0x97340264e77e568659
Result: Prime

Number: 0x5ad789fb37aade289f
Result: Composite

Number: 0x7d25fb921140550ebfb0d84b
Result: Prime

Number: 0x34ba3709df728a732fdb1787
Result: Composite

Number: 0x62a6f3afc5e548d4ec00deb0873fde7386c10994190130a7
Result: Composite

Number: 0xd37ada29632adde177ce24e4671d5f59dedb7e193ea8b117
Result: Prime

Pritheesh's Air:Spring 2019 pritheesh$ 
*/