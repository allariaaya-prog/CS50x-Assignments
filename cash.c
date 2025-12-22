#include <cs50.h>
#include <math.h> // to get round function
  int main(void)
  {
    // first asking about the number in dollar and term it
   float dollars;
   int total_coins = 0;
   do // using do to ensure that the instructions inside do
    {
        dollars = get_float("Change owed: ");
    }
    // because money always positive
    while (dollars < 0);
    // turn the dollars into cents
    int cents = round(dollars * 100);
    int total_coins = 0;
    while (cents >= 25)
    {
        cents = cents - 25; //
        total_coins++;      // to add a coin by one
    }
    while (cents >= 10)
    {
        cents = cents - 10;
        total_coins++;
    }
    while (cents >= 5)
    {
        cents = cents - 5;
        total_coins++;
    }
    while (cents >= 1)
    {
        cents = cents - 1;
        total_coins++;
    }
    printf("%i\n", total_coins);
    return 0;
