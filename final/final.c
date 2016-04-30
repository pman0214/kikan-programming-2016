/* 
 * Copyright (c) 2016, Shigemi Ishida, Kyushu University
 * All rights reserved.
 *
 * DO NOT REDISTRIBUTE THIS PROGRAM NOR A PART OF THIS PROGRAM.
 */

/**
 * @file
 *      Main module.
 * @author
 *      Shigemi Ishida <ishida+devel@f.ait.kyushu-u.ac.jp>
 *
 * Convert a PPM image to the gray-scaled one.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "tool.h"
#include "trace.h"

/*======================================================================
 * definitions, macros
 *======================================================================*/
#define MAX_WIDTH       1024
#define MAX_HEIGHT      768
#define MAX_DEPTH       255

/*======================================================================
 * typedefs, structures
 *======================================================================*/
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

/**
 * @struct
 *      operation parameters.
 */
typedef struct opr_strct
{
    unsigned int trace_level;   /**< tracer output level */
    char *in_filename;          /**< input filename */
    char *out_filename;         /**< output filename */

    FILE *infile;               /**< input file pointer */
    FILE *outfile;              /**< output file pointer */
} opr_t;

/**
 * @struct
 *      image info.
 */
typedef struct img_info_strct
{
    int width;
    int height;
    int depth;
} img_info_t;

/*======================================================================
 * prototype declarations for private functions
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

/*======================================================================
 * functions
 *======================================================================*/
int main(int argc, char *argv[])
{
    opr_t opr;                  /* operation parameters */
    int   ret;                  /* return value handler */
    img_info_t in_img;          /* input image info */
    unsigned char img_data[MAX_HEIGHT*MAX_WIDTH*3];

    /* set a default trace level to ERROR */
    T_init(T_E);

    /* initialize data */
    memset(&opr, 0, sizeof(opr));
    memset(&in_img, 0, sizeof(in_img));

    /* handling options, arguments */
    ret = arg_handler(argc, argv, &opr);
    if (ret < 0)
    {
        return ret;
    }

    ret = global_init(&opr);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }

    T_M(T_D2, 0x00010100, "finished initialization.\n");

    /*----------------------------------------------------------------------*/

    /* load image info */
    ret = load_img_info(opr.infile, &in_img);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }
    /* check image size and depth */
    if ( (in_img.width  > MAX_WIDTH) ||
         (in_img.height > MAX_HEIGHT) ||
         (in_img.depth  > MAX_DEPTH) )
    {
        T_M(T_E, 0x80010200, "Image size %dx%d, depth=%d is over range.\n",
            in_img.width, in_img.height, in_img.depth);
        global_deinit(&opr);
        return 0x80010200;
    }

    /* read image data */
    ret = load_img(img_data, &opr, &in_img);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }

    /* convert to gray scaled image */
    ret = gray_convert(img_data, &in_img);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }

    /* save to an output file */
    ret = write_img_flipped(img_data, &opr, &in_img);
    if (ret < 0)
    {
        global_deinit(&opr);
        return ret;
    }

    /*----------------------------------------------------------------------*/

    global_deinit(&opr);

    T_M(T_D2, 0x0001ffff, "program completed.\n");

    return 0;
}

/*======================================================================
 * private functions
 *======================================================================*/
static int arg_handler(int argc, char *argv[], opr_t *opr)
{
    int ret;

    /*------------------------------
     * handling options
     *------------------------------*/
    for (;;)
    {
        ret = getopt(argc, argv, "hd:");

        if (ret < 0)
        {
            break;
        }
        switch (ret)
        {
        case 'h':
            usage();
            T_M(T_D2, 0x400100ff, "exit with showing help.\n");
            exit(0);
        case 'd':
            if (!is_number(optarg))
            {
                T_M(T_E, 0xc0010100, "invalid debug level parameter: %s.\n", optarg);
                return 0xc0010100;
            }
            opr->trace_level = (unsigned int)strtol(optarg, NULL, 10);
            ret = T_init(opr->trace_level);
            if (ret < 0)
            {
                T_init(T_E);
            }
            break;
        case '?':               /* invalid option */
            T_M(T_E, 0xc00101ee, "invalid option.\n", optopt);
            usage();
            return 0xc00101ee;
            break;
        default:                /* no route to here */
            T_M(T_E, 0xc00101ff, "invalid option.\n", optopt);
            usage();
            return 0xc00101ff;
            break;
        }
    }

    /*------------------------------
     * handling arguments
     *------------------------------*/
    switch (argc - optind)
    {
    case 2:
        opr->in_filename  = argv[optind];
        opr->out_filename = argv[optind+1];
        break;
    default:
        T_M(T_E, 0xc0010500, "invalid arguments.\n");
        usage();
        return 0xc0010700;
        break;
    }

    return 0;
}

/*----------------------------------------------------------------------*/
static int global_init(opr_t *opr)
{
    /* open input file */
    opr->infile = fopen(opr->in_filename, "rb");
    if (!opr->infile) {
        T_M(T_E, 0xc0020100, "Cannot open input file: %s.\n", strerror(errno));
        return 0xc0020100;
    }

    /* open output file */
    opr->outfile = fopen(opr->out_filename, "wb");
    if (!opr->infile) {
        T_M(T_E, 0xc0020200, "Cannot open output file: %s.\n", strerror(errno));
        return 0xc0020200;
    }

    return 0;
}

/*----------------------------------------------------------------------*/
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
static void usage(void)
{
    puts("Usage: final [-h] [-d <debug_level>] <in_file> <out_file>");
    puts("Options:");
    puts("  -h show this help and exit");
    puts("  -d specify debug message level");
    printf("    %d\tERROR (default)\n", T_E);
    printf("    %d\tWARNING\n", T_W);
    printf("    %d\tINFO\n", T_I);
    printf("    %d\tDEBUG1\n", T_D1);
    printf("    %d\tDEBUG2\n", T_D2);

    return;
}

/*----------------------------------------------------------------------*/
static int load_img_info(FILE *imgfile, img_info_t *info)
{
    char buf[10];
    char *pcRet;
    char *sizes[2];
    int ret;

    /* first line is "P6" */
    pcRet = fgets(buf, sizeof(buf), imgfile);
    if (!pcRet)
    {
        T_M(T_E, 0xc00500f0, "Cannot read from input file: %s", strerror(errno));
        return 0xc00500f0;
    }
    T_M(T_D2, 0x40050100, "%s", buf);

    /* skip comments */
    pcRet = fgets(buf, sizeof(buf), imgfile);
    if (!pcRet)
    {
        T_M(T_E, 0xc00501f0, "Cannot read from input file");
        return 0xc00501f0;
    }

    while (buf[0] == '#')
    {
        T_M(T_D2, 0x40050101, "%s\n", buf);

        /* read until the end of the line */
        while (buf[strlen(buf)-1] != '\n')
        {
            pcRet = fgets(buf, sizeof(buf), imgfile);
            if (!pcRet)
            {
                T_M(T_E, 0xc00502f0, "Cannot read from input file: %s.\n", strerror(errno));
                return 0xc00502f0;
            }
            T_M(T_D2, 0x40050102, "%s\n", buf);
        }

        /* read next line */
        pcRet = fgets(buf, sizeof(buf), imgfile);
    }

    /*------------------------------*/
    /* we now reach at a line describing the image size */
    T_M(T_D2, 0x40050200, "%s\n", buf);
    /* remove new line code */
    buf[strlen(buf)-1] = '\0';
    /* split with space */
    ret = split_words(buf, sizes, 2);
    if (ret != 2)
    {
        T_M(T_E, 0xc0050280, "Invalid image size description.\n");
        return 0xc0050280;
    }
    /* retrieve image size */
    info->width  = (int)strtol(sizes[0], NULL, 10);
    info->height = (int)strtol(sizes[1], NULL, 10);

    /*------------------------------*/
    /* retrieve next line to derive depth */
    pcRet = fgets(buf, sizeof(buf), imgfile);
    if (!pcRet)
    {
        T_M(T_E, 0xc0050301, "Cannot read from input file");
        return 0xc0050301;
    }
    /* remove new line code */
    buf[strlen(buf)-1] = '\0';

    /* retrieve image depth */
    info->depth = (int)strtol(buf, NULL, 10);

    T_M(T_D2, 0x40050f00, "Image size=%dx%d, depth=%d\n",
        info->width, info->height, info->depth);

    return 0;
}

/*----------------------------------------------------------------------*/
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
static int load_img(unsigned char *buf, opr_t *opr, img_info_t *info)
{
    long cnt;
    long max;
    int ret;

    cnt = 0;
    max = info->height * info->width * 3;
    while (cnt < max)
    {
        /* read 3-byte (RGB-byte) blocks */
        ret = fread(buf+cnt, 1, info->width*3, opr->infile);
        if (ret <= 0)
        {
            T_M(T_E, 0xc0070100, "Invalid image file format.\n");
            return 0xc0070100;
        }
        T_D(T_D2, 0x40070200, buf+cnt, info->width*3);
        cnt += ret;
        T_M(T_D2, 0x40070210, "cnt=%d, buf+cnt=%p\n",
            cnt, buf+cnt);
    }
    T_D(T_D2, 0x40070300, buf, max);
    T_M(T_D1, 0x40070310, "read  cnt=%d, buf=%p\n",
        cnt, buf);

    return 0;
}

/*----------------------------------------------------------------------*/
static int gray_convert(unsigned char *buf, img_info_t *info)
{
    unsigned char pixel[3];     /* rgb data */
    unsigned char y;
    long cnt;
    long max;

    T_D(T_D2, 0x40080000, buf, info->height*info->width*3);

    max = info->height * info->width * 3;
    for (cnt=0; cnt < max; cnt+=3)
    {
        /* read r, g, b */
        memcpy(pixel, buf+cnt, 3);
        /* calculate Y = 0.299R + 0.587G + 0.114B  */
        y = 0.299 * pixel[0] +
            0.587 * pixel[1] +
            0.114 * pixel[2];
        T_M(T_D2, 0x40080100+cnt, "r,g,b=%02x,%02x,%02x, y=%02x\n",
            pixel[0], pixel[1], pixel[2], y);

        /* overwrite the original image with the gray scaled one */
        *(buf+cnt)   = y;
        *(buf+cnt+1) = y;
        *(buf+cnt+2) = y;
        T_D(T_D2, 0x40080200+cnt, buf, info->height*info->width*3);
    }
    T_D(T_D2, 0x4008f000, buf, info->height*info->width*3);

    return 0;
}

/*----------------------------------------------------------------------*/
static int write_img_flipped(unsigned char *buf, opr_t *opr, img_info_t *info)
{
    int row;
    int col;
    int ret;

    fputs("P6\n", opr->outfile);
    fprintf(opr->outfile, "%d %d\n%d\n", info->width, info->height, info->depth);

    int cnt = 0;
    for (row=0; row < info->height; row++)
    {
        /* gray scale data has same RGB value,
         * so we can reversely read each pixel */
        for (col=info->width; col > 0; )
        {
            col--;
            ret = fwrite(buf+row*info->width*3+col*3, 1, 3, opr->outfile);
            if (ret <= 0)
            {
                T_M(T_E, 0xc0090400, "Cannot write to an outputfile %s: %s.\n",
                    opr->out_filename, strerror(errno));
                return 0xc0090400;
            }
            cnt += ret;
        }
    }
    T_M(T_D1, 0x40090f00, "write cnt=%d\n", cnt);

    return 0;
}

/* end of final.c */
