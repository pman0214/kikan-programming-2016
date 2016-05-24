#include <stdio.h>

int main(void)
{
    int x;                      /* 入力受け取り用 */
    int cnt;                    /* カウンタ */
    int check3;                 /* 3が入っているかチェックする用 */

    printf("整数を入力してください: ");
    scanf("%d",&x);             /* 入力 */

    for (cnt=0; cnt < x; cnt++)
    {
        check3 = cnt;
        while (check3 > 0)
        {
            if (check3 % 10 == 3)
            {
                printf("%d\n", cnt);
                break;
            }
            check3 /= 10;
        }
    }

    return 0;
}
