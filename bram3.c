#include "include/define.h"
#include "include/main.h"
#include <stdint.h>

#define DSIGMA 65536
#define HS(x, i) (x[i] << 4) + (x[i + 1] << 2) + x[i + 2]

#define HS_UPDATE(h, x, i) (h >> 2) - (x[i + 3] >> 2) + (x[i] << 4)
#define Q 3

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
            F[c] = i;

        L[c] = i;
    }
    END_PREPROCESSING

    BEGIN_SEARCHING
    int count = 0;
    int j = 0;
    while (j <= n - m + Q)
    {
        c = HS(y, j + m - Q);
        int f = F[c];
        int l = L[c];

        int i = m - Q;
        int s = m - Q + 1;
        while (f <= l)
        {
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
                    if (!memcmp(y + j, x, m))
                        count++;
                }
            }

            if (i < 0)
                break;

            c = HS_UPDATE(c, y, j + i);

            f--;
            l--;
        }
        j += s;
    }
    END_SEARCHING
    return count;
}