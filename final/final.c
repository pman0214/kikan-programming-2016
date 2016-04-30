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

/*======================================================================
 * functions
 *======================================================================*/
int main(int argc, char *argv[])
{
    opr_t opr;                  /* operation parameters */
    int   ret;                  /* return value handler */

    /* set a default trace level to ERROR */
    T_init(T_E);

    /* initialize data */
    memset(&opr, 0, sizeof(opr));

    /* handling options, arguments */
    ret = arg_handler(argc, argv, &opr);
    if (ret < 0)
    {
        return(0x80010010);
    }

    ret = global_init(&opr);
    if (ret < 0)
    {
        global_deinit(&opr);
        return(ret);
    }

    T_M(T_D2, 0x00010100, "finished initialization.\n");

    /*----------------------------------------------------------------------*/

    global_deinit(&opr);

    T_M(T_D2, 0x0001ffff, "program completed.\n");

    return(0);
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
                return(0xc0010100);
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
            return(0xc00101ee);
            break;
        default:                /* no route to here */
            T_M(T_E, 0xc00101ff, "invalid option.\n", optopt);
            usage();
            return(0xc00101ff);
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
        return(0xc0010700);
        break;
    }

    return(0);
}

/*----------------------------------------------------------------------*/
static int global_init(opr_t *opr)
{
    /* open input file */
    opr->infile = fopen(opr->in_filename, "rb");
    if (!opr->infile) {
        perror("Cannot open input file");
        return -1;
    }

    /* open output file */
    opr->outfile = fopen(opr->out_filename, "wb");
    if (!opr->infile) {
        perror("Cannot open output file");
        return -2;
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

/* end of main.c */
