#include <cs50.h>
#include <ctype.h> // isalpha & isspace
#include <math.h>  //round
#include <stdio.h>
#include <string.h> //strlen

// here no need for functions

int main(void)
{
    // ask for the text
    string text = get_string("Text: ");

    // determ the counters
    int letters = 0;
    // start for 1 word
    int words = 1;
    int sentences = 0;
    int n = strlen(text);

    // one loop to count chars & words
    for (int i = 0; i < n; i++)
    {
        // first:count chars
        if (isalpha(text[i]))
        {
            letters++;
        }

        // second:count words
        else if (isspace(text[i]))
        {
            words++;
        }

        // third:count the sentences (the end of it is one of these marks)
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    // 4. تطبيق المعادلة (Coleman-Liau Index)

    //(float)
    float L = ((float) letters / words) * 100;
    float S = ((float) sentences / words) * 100;

    float index = 0.0588 * L - 0.296 * S - 15.8;

    // نستخدم round لتقريب الرقم العشري إلى أقرب عدد صحيح
    int grade = round(index);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
