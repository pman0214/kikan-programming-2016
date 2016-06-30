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

int print_matrix(const int *mat)
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
