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
#include <stdlib.h>

int main(void)
{
    int hour1;
    int min1;
    int hour2;
    int min2;
    int diff_hour;
    int diff_min;

    /* 時刻の入力と入力チェック */
    printf("時刻1を入力して下さい（H:M）: ");
    scanf("%d:%d", &hour1, &min1);
    if ( (hour1 < 0) || (hour1 >= 24) )
    {
        printf("不正な時刻です\n");
        return -1;
    }
    if ( (min1 < 0) || (min1 >= 60) )
    {
        printf("不正な時刻です\n");
        return -1;
    }
    
    printf("時刻2を入力して下さい（H:M）: ");
    scanf("%d:%d", &hour2, &min2);
    if ( (hour2 < 0) || (hour2 >= 24) )
    {
        printf("不正な時刻です\n");
        return -2;
    }
    if ( (min2 < 0) || (min2 >= 60) )
    {
        printf("不正な時刻です\n");
        return -2;
    }

    /* 時刻を分に変換 */
    min1 = hour1*60 + min1;
    min2 = hour2*60 + min2;

    /* 分の差を計算 */
    diff_min = abs(min1 - min2);

    /* 時:分に変換 */
    diff_hour = diff_min / 60;
    diff_min  = diff_min % 60;

    /* 結果出力 */
    printf("時間差 = %02d:%02d\n", diff_hour, diff_min);

    return 0;
}
