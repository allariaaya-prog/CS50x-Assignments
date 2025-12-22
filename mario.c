#include <cs50.h>
#include <stdio.h>

void draw(int n);

int main(void)
{
    int height = get_int("hieght:");
    draw(height);
}

void draw(int n)
{
    if (n <= 0)
    {
        return;
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i - 1 + 1; j++)

        {
            printf("#");
        }
        printf("#\n");
    }
}
