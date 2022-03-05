#include "include/define.h"
#include "include/main.h"
#include <stdint.h>

#define DSIGMA 16380
#define HS(a, b, c, d) (a << 6) + (b << 4) + (c << 2) + d
#define Q 4
#define HASH uint16_t

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

void set(int *F, int *L, int a, int b, int c, int d, int i, int m)
{
    HASH h = HS(a, b, c, d);

    if (F[h] == m)
        F[h] = i; // - (i != 0);

    L[h] = i; // + (i != m - Q);
}

int search(unsigned char *x, int m, unsigned char *y, int n)
{
    HASH c;

    BEGIN_PREPROCESSING
    int F[DSIGMA], L[DSIGMA];
    for (int i = 0; i < DSIGMA; i++)
    {
        F[i] = m;
        L[i] = -1;
    }

    for (int i = 0; i < m - Q + 1; i++)
    {
        set(F, L, x[i], x[i + 1], x[i + 2], x[i + 3], i, m); // |abcd -> |abcd
        set(F, L, x[i + 1], x[i], x[i + 2], x[i + 3], i, m); // |abcd -> |bacd
        set(F, L, x[i + 1], x[i], x[i + 3], x[i + 2], i, m); // |abcd -> |badc
        set(F, L, x[i], x[i + 2], x[i + 1], x[i + 3], i, m); // |abcd -> |acbd
        set(F, L, x[i], x[i + 1], x[i + 3], x[i + 2], i, m); // |abcd -> |abdc
        if (i > 0)
        {
            set(F, L, x[i - 1], x[i + 1], x[i + 2], x[i + 3], i, m); // |abcd -> a|*bcd
            set(F, L, x[i - 1], x[i + 2], x[i + 1], x[i + 3], i, m); // |abcd -> a|*cbd
            set(F, L, x[i - 1], x[i + 1], x[i + 3], x[i + 2], i, m); // |abcd -> a|*bdc
            if (i < m - 4)
            {
                set(F, L, x[i - 1], x[i + 1], x[i + 2], x[i + 4], i, m); // |abcd -> a|*bc*d
                set(F, L, x[i - 1], x[i + 2], x[i + 1], x[i + 4], i, m); // |abcd -> a|*cb*d
            }
        }
        if (i < m - 4)
        {
            set(F, L, x[i], x[i + 1], x[i + 2], x[i + 4], i, m); // |abcd -> |abc*d
            set(F, L, x[i + 1], x[i], x[i + 2], x[i + 4], i, m); // |abcd -> |bac*d
            set(F, L, x[i], x[i + 2], x[i + 1], x[i + 4], i, m); // |abcd -> |acb*d
        }
    }
    END_PREPROCESSING

    BEGIN_SEARCHING
    int naiveCount = 0;
    int count = 0;
    int j = 0;
    while (j <= n - m + Q)
    {
        c = HS(y[j + m - 4], y[j + m - 3], y[j + m - 2], y[j + m - 1]);
        int f = F[c];
        int l = L[c];

        int i = m - Q;
        int s = m - Q + 1;
        while (i >= 0 && f <= l)
        {
            c = HS(y[j + i], y[j + i + 1], y[j + i + 2], y[j + i + 3]);
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