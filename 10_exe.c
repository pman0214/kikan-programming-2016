#include <stdio.h>
#include <string.h>

#define MAX_PEOPLE      30
#define SUBS            4

int main(void)
{
    int scores[MAX_PEOPLE][SUBS+2]; /* 成績格納用、+2は個人番号と合計用 */
    int swap[SUBS+2];               /* ソート時の入れ替え用 */
    int num_people;
    int cnt;
    int sort_max;
    int ret;

    /* 初期化 */
    memset(scores, 0, sizeof(scores));

    /*======================================================================
     * 成績入力
     */
    printf("番号順に成績を入力して下さい（「,」区切り、「.Enter」で終了）:\n");
    for (num_people=0; num_people < MAX_PEOPLE; num_people++)
    {
        scores[num_people][0] = num_people+1;
        ret = scanf("%d", &scores[num_people][2]);
        for (cnt=1; cnt < SUBS; cnt++)
        {
            ret += scanf(",%d", &scores[num_people][cnt+2]);
        }
        /* 規定数だけ入力がなかったら終了（.以外でも終了しちゃうけどOKとする） */
        if (ret < SUBS)
        {
            break;
        }
        /* 合計を計算 */
        scores[num_people][1] = 0;
        for (cnt=2; cnt < SUBS+2; cnt++)
        {
            scores[num_people][1] += scores[num_people][cnt];
        }
    }

    /*======================================================================
     * ソート
     */
    /* 最大値を減らしながら入れ替え操作をするのがバブルソート */
    for (sort_max = num_people; sort_max > 0; sort_max--)
    {
        /* sort_maxまでの範囲で入れ替えを行っていく */
        for (cnt=0; cnt < sort_max-1; cnt++)
        {
            /* 後のやつの方が合計が大きい？ */
            if (scores[cnt][1] < scores[cnt+1][1])
            {
                /* 入れ替え */
                memcpy(swap,              &scores[cnt][0],   sizeof(swap));
                memcpy(&scores[cnt][0],   &scores[cnt+1][0], sizeof(scores[cnt]));
                memcpy(&scores[cnt+1][0], swap,              sizeof(scores[cnt+1]));
            }
        }
    }

    /*======================================================================
     * 結果を出力
     */
    puts("");
    puts(" No. | Sum | Japanese | English | Math | Chemistry");
    puts("-----+-----+----------+---------+------+----------");
    for (cnt=0; cnt < num_people; cnt++)
    {
        /*     " No. | Sum | Japanese | English | Math | Chemistry"); */
        printf(" %3d | %3d |      %3d |     %3d |  %3d |       %3d\n",
               scores[cnt][0],
               scores[cnt][1],
               scores[cnt][2],
               scores[cnt][3],
               scores[cnt][4],
               scores[cnt][5]
            );
    }

    return 0;
}
