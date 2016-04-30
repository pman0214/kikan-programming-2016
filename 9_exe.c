#include <stdio.h>

static int print_matrix(const int *mat);

int main(void)
{
    int a[3][3];
    int b[3][3];
    int sum[3][3];              /* 和 */
    int sub[3][3];              /* 差 */
    int mul[3][3];              /* 積 */
    int i, j;

    /* 入力 */
    printf("行列A:\n");
    scanf("%d %d %d", &a[0][0], &a[0][1], &a[0][2]);
    scanf("%d %d %d", &a[1][0], &a[1][1], &a[1][2]);
    scanf("%d %d %d", &a[2][0], &a[2][1], &a[2][2]);
    printf("行列B:\n");
    scanf("%d %d %d", &b[0][0], &b[0][1], &b[0][2]);
    scanf("%d %d %d", &b[1][0], &b[1][1], &b[1][2]);
    scanf("%d %d %d", &b[2][0], &b[2][1], &b[2][2]);

    /* ループして計算 */
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            sum[i][j] = a[i][j] + b[i][j];
            sub[i][j] = a[i][j] - b[i][j];
            /* 積は少し注意が必要 */
            mul[i][j] =
                a[i][0] * b[0][j] +
                a[i][1] * b[1][j] +
                a[i][2] * b[2][j];
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

    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            printf(" %5d", *(mat+3*i+j));
        }
        printf("\n");
    }

    return 0;
}
