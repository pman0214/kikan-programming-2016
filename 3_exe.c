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

int main(void)
{
    int val;

    /* 整数を入力 */
    printf("正の整数を入力して下さい（255以下）: ");
    scanf("%d", &val);

    printf("16進数表記: 0x%X\n", val);
    printf(" 8進数表記: 0%o\n", val);

    /*------------------------------
     * 2進数を計算する
     *------------------------------*/
    /* 最大255なので8bitで表現できるから8個の変数を用意する */
    int bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0;
    
    /* bit0は2で割った余り */
    bit0 = val % 2;

    /* valを2で割れば余りは切り捨てられるからbit1が一番下のビットになる */
    val /= 2;
    /* 2で割った余りがbit1 */
    bit1 = val % 2;

    /* 以下同様にして全ビットを計算 */
    val /= 2;
    bit2 = val % 2;
    val /= 2;
    bit3 = val % 2;
    val /= 2;
    bit4 = val % 2;
    val /= 2;
    bit5 = val % 2;
    val /= 2;
    bit6 = val % 2;
    val /= 2;
    bit7 = val % 2;

    /* 全ビットを並べて表示 */
    printf(" 2進数表記: %d%d%d%d%d%d%d%d\n",
           bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0);

    return 0;
}
