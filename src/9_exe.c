/* 
 * Copyright (c) 2016, Shigemi Ishida, Kyushu University
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#define MAT_SIZE        3

static int print_matrix(const int *mat);

int main(void)
{
    int a[MAT_SIZE][MAT_SIZE];
    int b[MAT_SIZE][MAT_SIZE];
    int sum[MAT_SIZE][MAT_SIZE]; /* 和 */
    int sub[MAT_SIZE][MAT_SIZE]; /* 差 */
    int mul[MAT_SIZE][MAT_SIZE]; /* 積 */
    int i, j, k;

    /* 入力 */
    printf("行列A:\n");
    for (i=0; i < MAT_SIZE; i++)
    {
        for (j=0; j < MAT_SIZE; j++)
        {
            scanf("%d", &a[i][j]);
        }
    }
    printf("行列B:\n");
    for (i=0; i < MAT_SIZE; i++)
    {
        for (j=0; j < MAT_SIZE; j++)
        {
            scanf("%d", &b[i][j]);
        }
    }

    /* ループして計算 */
    for (i=0; i < MAT_SIZE; i++)
    {
        for (j=0; j < MAT_SIZE; j++)
        {
            sum[i][j] = a[i][j] + b[i][j];
            sub[i][j] = a[i][j] - b[i][j];
            /* 積は少し注意が必要 */
            mul[i][j] = 0;
            for (k=0; k < MAT_SIZE; k++)
            {
                mul[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    printf("\n");

    /* 結果出力 */
    /* 和 */
    printf("A + B =\n");
    print_matrix(&sum[0][0]);
    /* 差 */
    printf("A - B =\n");
    print_matrix(&sub[0][0]);
    /* 積 */
    printf("AB =\n");
    print_matrix(&mul[0][0]);

    return 0;
}

static int print_matrix(const int *mat)
{
    int i;
    int j;

    for (i=0; i < MAT_SIZE; i++)
    {
        for (j=0; j < MAT_SIZE; j++)
        {
            printf(" %6d", *(mat+MAT_SIZE*i+j));
        }
        printf("\n");
    }

    return 0;
}
