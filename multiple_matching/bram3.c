#include "include/define.h"
#include "include/main.h"
#include <stdint.h>
#include <immintrin.h>

#define DSIGMA 65536
#define HS(x, i) (x[i] << 4) + (x[i + 1] << 2) + x[i + 2]
#define HS_UPDATE(h, x, i) (h >> 2) - (x[i + 3] >> 2) + (x[i] << 4)
#define Q 3

int search(unsigned char *x, int m, unsigned char *y, int n, int r, double *time1, double *time2)
{
    TIMER *_timer = (TIMER *)malloc(sizeof(TIMER));

    // **preprocessing**
    timer_start(_timer);

    uint16_t c;

    int F[DSIGMA], L[DSIGMA];
    for (int i = 0; i < DSIGMA; i++)
    {
        F[i] = m;
        L[i] = -1;
    }

    for (int j = 0; j < r; j++)
    {
        for (int i = 0; i < m - Q + 1; i++)
        {
            const char *_x = x + j * m;
            c = HS(_x, i);
            F[c] = MIN(F[c], i);
            L[c] = MAX(L[c], i);
        }
    }
    timer_stop(_timer);
    (*time1) = timer_elapsed(_timer) * 1000;
    // **end preprocessing*+

    /* Searching ************************************/
    timer_start(_timer);
    int count = 0;
    int j = 0;
    while (j <= n - m + Q)
    {
        c = HS(y, j + m - Q);
        int f = F[c];
        int l = L[c];

        int i = m - Q;
        int s = m - Q + 1;
        //int f = 0; int l = m - Q + 1;
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
                    for (int patt = 0; patt < r; patt++)
                    {
                        if (!memcmp(y + j, x + patt * m, m))
                            count++;
                    }
                    break;
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
    timer_stop(_timer);
    (*time2) = timer_elapsed(_timer) * 1000;
    free(_timer);
    return count;
}