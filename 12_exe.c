#include <stdio.h>
#include <string.h>

#define MAX_SENTENCE    512     /* 英文の最大長 */

static int read_line(char *line, int max_len);
static int split_words(char *line, char *words[], int words_len);

/*----------------------------------------------------------------------*/
int main(void)
{
    int ret;
    int cnt;
    int word_num;                  /* 単語数 */
    char sentence[MAX_SENTENCE+2]; /* 改行コードと終端文字分だけ多めに確保 */
    char *words[MAX_SENTENCE];     /* 単語の先頭を格納するポインタの配列 */

    for (;;)
    {
        /* 1行読み込み */
        ret = read_line(sentence, MAX_SENTENCE+2);
        if (ret != 0)
        {
            break;
        }

        /* 単語に分割 */
        word_num = split_words(sentence, words, MAX_SENTENCE);

#ifdef DEBUG
        while (word_num > 0)
        {
            word_num--;
            fprintf(stderr, "words[%d]=%s\n", word_num, words[word_num]);
        }
#endif

        /* 単語毎に置換が必要なら置換して表示 */
        for (cnt=0; cnt < word_num; cnt++)
        {
            /* 文頭以外はスペースを入れる */
            if (cnt != 0)
            {
                printf(" ");
            }

            /* weまたはWeと一致 */
            if ( (strncmp(words[cnt], "we", 2) == 0) ||
                 (strncmp(words[cnt], "We", 2) == 0) )
            {
                /* 次の文字が'\0', ''', ',', '.', '!', '?'の場合は単語終了と判定 */
                switch (words[cnt][2])
                {
                case '\0':
                case '\'':
                case ',':
                case '.':
                case '!':
                case '?':
                    if (words[cnt][0] == 'w')
                    {
                        printf("you");
                    }
                    else
                    {
                        printf("You");
                    }
                    printf("%s", &words[cnt][2]);
                    break;
                default:
                    printf("%s", words[cnt]);
                    break;
                }
            }
            else
            {
                printf("%s", words[cnt]);
            }
        }
        puts("");
    }

    puts("Bye!");
    return 0;
}

/*----------------------------------------------------------------------
 * @brief   stdinから1行読み込む
 * @param[out]  line        読み込んだ行の格納先
 * @param[in]   max_len     最大格納長
 * @return  0   成功
 *          1   終了条件（改行のみ）
 *          -1  読み込みエラー
 *          -2  英文が長すぎる
 *----------------------------------------------------------------------*/
static int read_line(char *line, int max_len)
{
    char *read_status;          /* 1行読み込み状況 */
    char *pos_nl;               /* 改行コードの位置 */

    read_status = fgets(line, max_len, stdin);
    if (!read_status)
    {
        fprintf(stderr, "読み込みエラー\n");
        return -1;
    }

    /* 改行を探す */
    pos_nl = strchr(line, '\n');

    /* 改行がない場合は長すぎる場合 */
    if (!pos_nl)
    {
        fprintf(stderr, "英文が長すぎます\n");

        /* バッファに残っている分を読み捨てる */
        fgets(line, max_len, stdin);

        return -2;
    }

    /* 改行のみだったら終了 */
    if ((pos_nl == line) && (*(line+1) == '\0'))
    {
        return 1;
    }

    /* 改行は削除する */
    *pos_nl = '\0';

    return 0;
}

/*----------------------------------------------------------------------
 * @brief   単語に分割する
 * @param[in]   line        1行のデータ
 * @param[out]  words       単語の先頭を示すポイントの配列へのポインタ
 * @param[in]   words_len   単語の先頭を示すポイントの配列の長さ
 * @return  -1  単語の配列がオーバーフロー
 *          単語数（>= 0） 分割した単語数
 *----------------------------------------------------------------------*/
static int split_words(char *line, char *words[], int words_len)
{
    int cnt;
    char *start;
    char *pos;

    for (cnt=0, pos=line, start=line; *pos != '\0'; pos++)
    {
        if (*pos == ' ')
        {
            /* 単語毎に文字列に分割 */
            *pos = '\0';
            words[cnt++] = start;
            start = pos + 1;
        }

        if (cnt > words_len)
        {
            return -1;
        }
    }
    words[cnt++] = start;

    return cnt;
}
