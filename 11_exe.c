#include <stdio.h>

int main(void)
{
    long max_num;
    long f1 = 0;
    long f2 = 1;
    long tmp;

    /* 個数の入力 */
    printf("いくつ以下のフィボナッチ数？: ");
    scanf("%ld", &max_num);

    /* 特別な場合 */
    switch (max_num)
    {
    case 0:
        return 0;
    case 1:
        return 0;
    }

    /* その他の場合
     *   最初の1つはそのまま表示させる
     */
    printf("%ld", f1);
    while (f2 <= max_num)
    {
        printf(", %ld", f2);
        tmp = f1 + f2;
        f1 = f2;
        f2 = tmp;
    }
    /* 最後に改行を表示させる */
    printf("\n");

    return 0;
}
