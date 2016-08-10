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
#include <string.h>

#define MAX_LEN     128

int main(void)
{
    int cnt;
    int len;
    /* 入力文字列: NULL文字と改行文字を考慮して最大文字数+2で確保する */
    char sentence[MAX_LEN+2];

    printf("英文を入力して下さい（最大%d文字）:\n", MAX_LEN);
    fgets(sentence, MAX_LEN+2, stdin);

    /* 文字列の長さを取得 */
    len = strlen(sentence);

    /* 最後の文字は改行なので消す */
    len--;
    sentence[len] = '\0';

    /* 文字列の配列を1つずつ調べて、スペースの直後を大文字に変換
     *   最後の文字はチェックする必要がないのでlen-1まで調べる
     */
    /* 1文字目だけは別途見ておく */
    if ( (sentence[0] >= 'a') && (sentence[0] <= 'z') )
    {
        /* 大文字に変換 */
        sentence[0] += 'A' - 'a';
    }
    for (cnt=0; cnt < len-1; cnt++)
    {
        /* スペース？ */
        if (sentence[cnt] == ' ')
        {
            /* 次の文字は小文字？ */
            if ( (sentence[cnt+1] >= 'a') && (sentence[cnt+1] <= 'z') )
            {
                /* 大文字に変換 */
                sentence[cnt+1] += 'A' - 'a';
            }
        }
    }

    /* 結果を表示 */
    printf("%s\n", sentence);

    return 0;
}
