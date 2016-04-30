#include <stdio.h>
#include <math.h>

int main(void)
{
    double a, b, theta;
    double x;

    /* a, b, thetaを入力させる */
    printf("a?: ");
    scanf("%lf", &a);
    printf("b?: ");
    scanf("%lf", &b);
    printf("theta?: ");
    scanf("%lf", &theta);

    /* 角度をラジアンに変換 */
    theta *= M_PI / 180;

    /* 余弦定理を使ってまずはx^2を算出 */
    x = pow(a, 2) + pow(b, 2) - 2*a*b*cos(theta);
    /* 平方根を求める */
    x = sqrt(x);

    printf("x = %lf\n", x);

    return 0;
}
