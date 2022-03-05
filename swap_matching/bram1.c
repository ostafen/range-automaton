#include "include/define.h"
#include "include/main.h"
#include <stdint.h>

#define DSIGMA 256
#define HS(x, i) (x[i])
#define Q 1

int naive_check(unsigned char *P, int m, unsigned char *T, int s)
{
    int j = 0;
    while (j < m)
    {
        if (P[j] == T[s + j])
            j++;
        else
        {
            if (j + 1 < m && P[j] == T[s + j + 1] && P[j + 1] == T[s + j])
                j += 2;
            else
                return 0;
        }
    }
    return 1;
}

int search(unsigned char *x, int m, unsigned char *y, int n)
{
    uint16_t c;

    BEGIN_PREPROCESSING
    int F[DSIGMA], L[DSIGMA];
    for (int i = 0; i < DSIGMA; i++)
    {
        F[i] = m;
        L[i] = -1;
    }

    for (int i = 0; i < m - Q + 1; i++)
    {
        c = HS(x, i);

        if (F[c] == m)
            F[c] = i - (i != 0);

        L[c] = i + (i != m - Q);
    }
    END_PREPROCESSING

    //BEGIN_SEARCHING
    int naiveCount = 0;
    int count = 0;
    int j = 0;
    while (j <= n - m + Q)
    {
        c = HS(y, j + m - Q);
        int f = F[c];
        int l = L[c];

        int i = m - Q;
        int s = m - Q + 1;
        while (i >= 0 && f <= l)
        {
            c = HS(y, j + i);
            f = MAX(f, F[c]);
            l = MIN(l, L[c]);

            i--;

            if (f == 0)
            {
                if (i >= 0)
                {
                    s = i + 1;
                }
                else
                {
                    if (naive_check(x, m, y, j))
                        count++;

                    naiveCount++;
                }
            }
            f--;
            l--;
        }
        j += s;
    }
    //END_SEARCHING
    *run_time = (float)(naiveCount) / count;
    return count;
}