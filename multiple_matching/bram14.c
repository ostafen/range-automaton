#include "include/define.h"
#include "include/main.h"
#include <stdint.h>
#include <immintrin.h>

#define DSIGMA 65536
#define HS(x, i) (x[i] << 13) + (x[i + 1] << 12) + (x[i + 2] << 11) + (x[i + 3] << 10) + (x[i + 4] << 9) + (x[i + 5] << 8) + (x[i + 6] << 7) + (x[i + 7] << 6) + (x[i + 8] << 5) + (x[i + 9] << 4) + (x[i + 10] << 3) + (x[i + 11] << 2) + (x[i + 12] << 1) + x[i + 13]
#define Q 14

typedef struct NODE_T
{
    int patt;
    struct NODE_T *next;
} NODE;

typedef struct LIST_T
{
    NODE *T[DSIGMA];
} LIST;

int search(unsigned char *x, int m, unsigned char *y, int n, int r, double *time1, double *time2)
{
    NODE nodes[r];
    int next_node = 0;
    LIST ls;

    TIMER *_timer = (TIMER *)malloc(sizeof(TIMER));
    // **preprocessing**
    timer_start(_timer);
    uint16_t c;
    int F[DSIGMA], L[DSIGMA];
    for (int i = 0; i < DSIGMA; i++)
    {
        F[i] = m;
        L[i] = -1;
        ls.T[i] = NULL;
    }

    for (int j = 0; j < r; j++)
    {
        const char *_x = x + j * m;
        for (int i = 0; i < m - Q + 1; i++)
        {
            c = HS(_x, i);
            F[c] = MIN(F[c], i);
            L[c] = MAX(L[c], i);
        }
        NODE *nd = &nodes[next_node++];
        nd->patt = j;
        nd->next = ls.T[c];
        ls.T[c] = nd;
    }
    timer_stop(_timer);
    (*time1) = timer_elapsed(_timer) * 1000;
    // **end preprocessing*

    /* Searching ************************************/
    timer_start(_timer);
    int count = 0;
    int j = 0;
    while (j <= n - m + Q)
    {
        c = HS(y, j + m - Q);
        uint16_t hc = c;

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
                    for (NODE *nd = ls.T[hc]; nd != NULL; nd = nd->next)
                    {
                        if (!memcmp(y + j, x + nd->patt * m, m))
                            count++;
                    }
                }
            }

            if (i < 0)
                break;

            c = HS(y, j + i);
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