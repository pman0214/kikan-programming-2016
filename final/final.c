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
    int height;
    int width;
    int depth;
} img_info_t;

/*======================================================================
 * global variables
 *======================================================================*/

/*------------------------------
 * private
 *------------------------------*/

/*======================================================================
 * prototype declarations for private functions
 *======================================================================*/
static int arg_handler(int argc, char *argv[], opr_t *opr);
static int global_init(opr_t *opr);
static void global_deinit(opr_t *opr);
static void usage(void);
static int load_img_info(FILE *imgfile, img_info_t *info);
static int split_words(char *line, char *words[], int words_len);

/*======================================================================
 * functions
 *======================================================================*/
int main(int argc, char *argv[])
{
    opr_t opr;                  /* operation parameters */
    int   ret;                  /* return value handler */
    img_info_t in_img;          /* input image info */

    /* set a default trace level to ERROR */
    T_init(T_E);

    /* initialize data */
    memset(&opr, 0, sizeof(opr));
    memset(&in_img, 0, sizeof(in_img));

    /* handling options, arguments */
    ret = arg_handler(argc, argv, &opr);
    if (ret < 0)
    {
        return 0x80010010;
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
        return -1;
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
    /* we now reach to a line describing the image size */
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
    info->height = (int)strtol(sizes[0], NULL, 10);
    info->width  = (int)strtol(sizes[1], NULL, 10);

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

    T_M(T_D1, 0x40050f00, "Image size=%dx%d, depth=%d\n",
        info->height, info->width, info->depth);

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

/* end of final.c */
