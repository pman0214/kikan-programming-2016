#include <stdio.h>

int main(void)
{
    int num;
    int cnt;

    long f1 = 0;
    long f2 = 1;
    long tmp;

    /* 個数の入力 */
    printf("何個のフィボナッチ数？: ");
    scanf("%d", &num);

    /* 特別な場合 */
    if (num <= 0)
    {
        return 0;
    }
    if (num == 1)
    {
        printf("%ld\n", f1);
        return 0;
    }

    /* その他の場合
     *   最初の2つはそのまま表示させる
     */
    printf("%ld, %ld", f1, f2);
    for (cnt=2; cnt < num; cnt++)
    {
        tmp = f1 + f2;
        f1 = f2;
        f2 = tmp;
        printf(", %ld", tmp);
    }
    /* 最後に改行を表示させておく */
    printf("\n");

    return 0;
}
