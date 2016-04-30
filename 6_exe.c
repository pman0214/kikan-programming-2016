#include <stdio.h>
#include <math.h>

int main(void)
{
    float height;
    float weight;
    float roher;

    /* 入力 */
    printf("身長 (cm): ");
    scanf("%f", &height);
    printf("体重 (kg): ");
    scanf("%f", &weight);

    /* ローレル指数を計算して表示 */
    roher = weight / pow(height, 3) * 1e7;
    printf("ローレル指数 = %.1f\n", roher);

    /* 判定を表示 */
    printf("判定: ");
    if (roher < 100)
    {
        printf("やせすぎ\n"); 
    }
    else if (roher < 115)
    {
        printf("やせぎみ\n");
    }
    else if (roher < 145)
    {
        printf("標準\n");
    }
    else if (roher < 160)
    {
        printf("太りぎみ\n");
    }
    else
    {
        printf("太りすぎ\n");
    }

    return 0;
}
