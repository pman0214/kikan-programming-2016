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
