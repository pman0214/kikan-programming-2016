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

/*
 * ■仕様
 * ・PPM画像ファイルを読み込み，グレースケール変換した上で左右を反転さ
 *   せてファイルに出力する。
 * ・指定可能な画像ファイルは最大1024x768ピクセルで、色数は256で固定と
 *   する。
 * ・入出力ファイル名はコマンドライン引数で指定する。
 *   - 指定がない場合にはエラーと使い方を表示して終了する。
 *   - 入出力ファイルが開けない場合はエラーを表示して終了する。
 * ・「-h」を付けて実行すると使い方を表示して終了する。
 * ・入力画像ファイルのフォーマットがおかしいときにはエラーを表示して
 *   終了する。
 * ・PPM形式の画像にはコメントが複数行あっても無くても良い。
 * ・エラーは標準エラー出力に表示する。
 */

/**
 * @file
 *      PPM形式画像をグレースケール化して左右反転
 * @author
 *      Shigemi Ishida <ishida+devel@f.ait.kyushu-u.ac.jp>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

/*======================================================================
 * definitions, macros
 *======================================================================*/
#define MAX_WIDTH       1024
#define MAX_HEIGHT      768
#define MAX_DEPTH       255

/*======================================================================
 * typedefs, structures
 *======================================================================*/
/**
 * @struct
 *      動作用パラメータ格納用
 */
typedef struct opr_strct
{
    char *in_filename;          /**< 入力ファイル名 */
    char *out_filename;         /**< 出力ファイル名 */

    FILE *infile;               /**< 入力ファイルポインタ */
    FILE *outfile;              /**< 出力ファイルポインタ */
} opr_t;

/**
 * @struct
 *      画像情報
 */
typedef struct img_info_strct
{
    int width;
    int height;
    int depth;
} img_info_t;

/*======================================================================
 * プロトタイプ宣言
 *======================================================================*/
static int arg_handler(int argc, char *argv[], opr_t *opr);
static int global_init(opr_t *opr);
static void global_deinit(opr_t *opr);
static void usage(void);
static int load_img_info(FILE *imgfile, img_info_t *info);
static int split_words(char *line, char *words[], int words_len);
static int load_img(unsigned char *buf, opr_t *opr, img_info_t *info);
static int gray_convert(unsigned char *buf, img_info_t *info);
static int write_img_flipped(unsigned char *buf, opr_t *opr, img_info_t *info);
static int is_number(char *num_str);

/*======================================================================
 * functions
 *======================================================================*/
int main(int argc, char *argv[])
{
    opr_t opr;                  /* 動作パラメータ */
    int   ret;                  /* return値格納用 */
    img_info_t in_img;          /* 入力画像情報 */
    unsigned char img_data[MAX_HEIGHT*MAX_WIDTH*3]; /* 画像データ */

    /* 初期化 */
    memset(&opr, 0, sizeof(opr));

    /* オプションや引数の処理 */
    ret = arg_handler(argc, argv, &opr);
    if (ret < 0)
    {
        return ret;
    }

    /* 初期化処理 */
    ret = global_init(&opr);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }

    /*----------------------------------------------------------------------*/

    /* 入力画像情報を読み込み */
    ret = load_img_info(opr.infile, &in_img);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }
    /* 画像サイズをチェック */
    if ( (in_img.width  > MAX_WIDTH) ||
         (in_img.height > MAX_HEIGHT) ||
         (in_img.depth  > MAX_DEPTH) )
    {
        fprintf(stderr, "Image size %dx%d, depth=%d is over range.\n",
            in_img.width, in_img.height, in_img.depth);
        global_deinit(&opr);
        return 0x80010200;
    }

    /* 画像データ部の読み込み */
    ret = load_img(img_data, &opr, &in_img);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }

    /* グレースケール化 */
    ret = gray_convert(img_data, &in_img);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }

    /* 左右反転してファイルに書き出し */
    ret = write_img_flipped(img_data, &opr, &in_img);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }

    /*----------------------------------------------------------------------*/

    global_deinit(&opr);

    return 0;
}

/*======================================================================
 * private functions
 *======================================================================*/
/**
 * @brief       Argument handler.
 * @param[in]   argc Number of arguments.
 * @param[in]   argv Arguments.
 * @param[out]  opr Operation parameters.
 * @return      Returns 0 on success.
 *              Returns minus value on any error.
 */
static int arg_handler(int argc, char *argv[], opr_t *opr)
{
    int ret;

    /*------------------------------
     * オプションの処理
     *------------------------------*/
    for (;;)
    {
        ret = getopt(argc, argv, "h");

        if (ret < 0)
        {
            break;
        }
        switch (ret)
        {
        case 'h':
            usage();
            exit(0);
        case '?':               /* invalid option */
            fprintf(stderr, "Invalid option %c.\n", optopt);
            usage();
            return 0xc00101ee;
            break;
        default:                /* no route to here */
            fprintf(stderr, "Invalid option %c.\n", optopt);
            usage();
            return 0xc00101ff;
            break;
        }
    }

    /*------------------------------
     * 引数の処理
     *------------------------------*/
    switch (argc - optind)
    {
    case 2:
        opr->in_filename  = argv[optind];
        opr->out_filename = argv[optind+1];
        break;
    default:
        fprintf(stderr, "Invalid arguments.\n");
        usage();
        return 0xc0010700;
        break;
    }

    return 0;
}

/*----------------------------------------------------------------------*/
/**
 * @brief       Initialization process.
 * @param[in]   opr Operation parameters.
 * @return      Returns 0 on success.
 *              Returns minus value on any error.
 */
static int global_init(opr_t *opr)
{
    /* 入力ファイルを開く */
    opr->infile = fopen(opr->in_filename, "rb");
    if (!opr->infile) {
        fprintf(stderr, "Cannot open input file: %s.\n", strerror(errno));
        return 0xc0020100;
    }

    /* 出力ファイルを開く */
    opr->outfile = fopen(opr->out_filename, "wb");
    if (!opr->infile) {
        fprintf(stderr, "Cannot open output file: %s.\n", strerror(errno));
        return 0xc0020200;
    }

    return 0;
}

/*----------------------------------------------------------------------*/
/**
 * @brief       De-initialization process.
 * @param[in]   opr Operation parameters.
 */
static void global_deinit(opr_t *opr)
{
    if (opr->infile)
    {
        fclose(opr->infile);
        opr->infile = NULL;
    }

    if (opr->outfile)
    {
        fclose(opr->outfile);
        opr->outfile = NULL;
    }

    return;
}

/*----------------------------------------------------------------------*/
/**
 * @brief       Print usage.
 */
static void usage(void)
{
    puts("Usage: final [-h] <in_file> <out_file>");
    puts("Options:");
    puts("  -h show this help and exit");

    return;
}

/*----------------------------------------------------------------------*/
/**
 * @brief       Read image information from image file.
 * @param[in]   imgfile File pointer to image file.
 * @param[out]  info Image information struct.
 * @return      Returns 0 on success.
 *              Returns minus value on any error.
 */
static int load_img_info(FILE *imgfile, img_info_t *info)
{
    char buf[10];
    char *pcRet;
    char *sizes[2];
    int ret;

    /* 最初の行は「P6」 */
    pcRet = fgets(buf, sizeof(buf), imgfile);
    if (!pcRet)
    {
        fprintf(stderr, "Cannot read from input file: %s.\n", strerror(errno));
        return 0xc00500f0;
    }

    /* コメントを読み飛ばす */
    pcRet = fgets(buf, sizeof(buf), imgfile);
    if (!pcRet)
    {
        fprintf(stderr, "Cannot read from input file: %s.\n", strerror(errno));
        return 0xc00501f0;
    }
    while (buf[0] == '#')
    {
        /* 行末まで読み飛ばす */
        while (buf[strlen(buf)-1] != '\n')
        {
            pcRet = fgets(buf, sizeof(buf), imgfile);
            if (!pcRet)
            {
                fprintf(stderr, "Cannot read from input file: %s.\n", strerror(errno));
                return 0xc00502f0;
            }
        }

        /* 次の行を読んでおく */
        pcRet = fgets(buf, sizeof(buf), imgfile);
        if (!pcRet)
        {
            fprintf(stderr, "Cannot read from input file: %s.\n", strerror(errno));
            return 0xc00502f1;
        }
    }

    /*------------------------------*/
    /* この時点で画像サイズを表す行に到達しているはず */
    /* 行末の改行コードを削除 */
    buf[strlen(buf)-1] = '\0';
    /* スペースで分離 */
    ret = split_words(buf, sizes, 2);
    /* 2個の値が書いてないときはフォーマットエラー */
    if (ret != 2)
    {
        fprintf(stderr, "Invalid image size description.\n");
        return 0xc0050280;
    }
    /* 数字が書いてないときはフォーマットエラー */
    if ( (!is_number(sizes[0])) || (!is_number(sizes[1])) )
    {
        fprintf(stderr, "Invalid image size description.\n");
        return 0xc0050290;
    }
    /* 画像の幅と高さを取得 */
    info->width  = (int)strtol(sizes[0], NULL, 10);
    info->height = (int)strtol(sizes[1], NULL, 10);

    /*------------------------------*/
    /* depthも取得 */
    pcRet = fgets(buf, sizeof(buf), imgfile);
    if (!pcRet)
    {
        fprintf(stderr, "Cannot read from input file: %s.\n", strerror(errno));
        return 0xc0050301;
    }
    /* 行末の改行コードを削除 */
    buf[strlen(buf)-1] = '\0';
    if (!is_number(buf))
    {
        fprintf(stderr, "Invalid image depth description.\n");
        return 0xc0050290;
    }
    /* depthを取得 */
    info->depth = (int)strtol(buf, NULL, 10);

    return 0;
}

/*----------------------------------------------------------------------*/
/**
 * @brief       Split words with blanks.
 * @param[in]   line String including a line to process.
 * @param[out]  words Pointer array to store word strings.
 * @param[in]   words_len Length of word string array.
 * @return      Returns number of words on success.
 *              Returns minus value on any error.
 */
static int split_words(char *line, char *words[], int words_len)
{
    int cnt;
    char *start;
    char *pos;

    for (cnt=0, pos=line, start=line; *pos != '\0'; pos++)
    {
        if (*pos == ' ')
        {
            /* split words */
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

/*----------------------------------------------------------------------*/
/**
 * @brief       Load image binary data.
 * @param[out]  buf Image data buffer.
 * @param[in]   opr Operation parameters.
 * @param[in]   info Image information struct.
 * @return      Returns 0 on success.
 *              Returns minus value on any error.
 */
static int load_img(unsigned char *buf, opr_t *opr, img_info_t *info)
{
    long cnt;
    long max;
    int ret;

    cnt = 0;
    max = info->height * info->width * 3;
    while (cnt < max)
    {
        /* 3バイト（RGBバイト）を読む */
        ret = fread(buf+cnt, 1, info->width*3, opr->infile);
        if (ret <= 0)
        {
            fprintf(stderr, "Invalid image file format.\n");
            return 0xc0070100;
        }
        cnt += ret;
    }

    return 0;
}

/*----------------------------------------------------------------------*/
/**
 * @brief       Convert image to the gray scaled image.
 * @param[in,out] buf Image data buffer.
 * @param[in]   info Image information struct.
 * @return      Returns 0 on success.
 *              Returns minus value on any error.
 */
static int gray_convert(unsigned char *buf, img_info_t *info)
{
    unsigned char pixel[3];     /* rgb data */
    unsigned char y;
    long cnt;
    long max;

    max = info->height * info->width * 3;
    for (cnt=0; cnt < max; cnt+=3)
    {
        /* r, g, bを読む */
        memcpy(pixel, buf+cnt, 3);
        /* 輝度を計算 Y = 0.299R + 0.587G + 0.114B  */
        y = 0.299 * pixel[0] +
            0.587 * pixel[1] +
            0.114 * pixel[2];

        /* 元のデータに上書きしてしまう */
        *(buf+cnt)   = y;
        *(buf+cnt+1) = y;
        *(buf+cnt+2) = y;
    }

    return 0;
}

/*----------------------------------------------------------------------*/
/**
 * @brief       Write image data to file with horizontal flipped operation.
 * @param[in]   buf Image data buffer.
 * @param[in]   opr Operation parameters.
 * @param[in]   info Image information struct.
 * @return      Returns 0 on success.
 *              Returns minus value on any error.
 */
static int write_img_flipped(unsigned char *buf, opr_t *opr, img_info_t *info)
{
    int row;
    int col;
    int ret;

    /* 画像情報を出力 */
    fputs("P6\n", opr->outfile);
    fprintf(opr->outfile, "%d %d\n%d\n", info->width, info->height, info->depth);

    int cnt = 0;
    for (row=0; row < info->height; row++)
    {
        /* 各行について右のピクセルからたどって出力していく */
        for (col=info->width; col > 0; )
        {
            col--;
            ret = fwrite(buf+row*info->width*3+col*3, 1, 3, opr->outfile);
            if (ret <= 0)
            {
                fprintf(stderr, "Cannot write to an outputfile %s: %s.\n",
                    opr->out_filename, strerror(errno));
                return 0xc0090400;
            }
            cnt += ret;
        }
    }

    return 0;
}

/*----------------------------------------------------------------------*/
/**
 * @brief       Check if string is numerical characters.
 * @param[in]   num_str String to be checked.
 * @return      Returns 1 if the string is numerical, otherwise returns 0.
 */
static int is_number(char *num_str)
{
    for (; *num_str != '\0'; num_str++)
    {
        if (!isdigit((unsigned char)*num_str))
        {
            return(0);
        }
    }
    return(1);
}

/* end of final.c */
