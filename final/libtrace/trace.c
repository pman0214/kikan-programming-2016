/* 
 * Copyright (c) 2014 Fukuda Laboratory and Shigemi ISHIDA, Kyushu University
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

/**
 * @file
 *      Tracer.
 * @author
 *      Shigemi Ishida <ishida+devel@f.ait.kyushu-u.ac.jp>
 *
 * Trace functions for debugging.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  /* #ifdef HAVE_CONFIG_H */

#include <stdio.h>
#include <stdarg.h>
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#else
#warning "pthread not used for tracer."
#endif  /* #ifdef HAVE_PTHREAD_H */

#include "trace.h"

/*======================================================================
 * global variables
 *======================================================================*/

/*------------------------------
 * private
 *------------------------------*/
static int trace_level;
static const char *msg_head[] = 
{
    "Error",
    "Warning",
    "Info",
    "Debug1",
    "Debug2"
};
#ifdef HAVE_PTHREAD_H
static pthread_mutex_t msg_lock;
#endif  /* #ifdef HAVE_PTHREAD_H */

/*======================================================================
 * prototype declarations for private functions
 *======================================================================*/

/*======================================================================
 * functions
 *======================================================================*/
void T_M(const int trace_lvl, const unsigned int seq, const char *format, ...)
{
    va_list ap;

    /* do nothing if trace_lvl is greater than output trace level */
    if (trace_lvl > trace_level)
    {
        return;
    }

#ifdef HAVE_PTHREAD_H
    pthread_mutex_lock(&msg_lock);
#endif  /* #ifdef HAVE_PTHREAD_H */
    fprintf(stderr, "%s:%08x ", msg_head[trace_lvl], (unsigned int)seq);
#ifdef HAVE_PTHREAD_H
    fprintf(stderr, "%p ", (void *)pthread_self());
#endif  /* #ifdef HAVE_PTHREAD_H */
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
#ifdef HAVE_PTHREAD_H
    pthread_mutex_unlock(&msg_lock);
#endif  /* #ifdef HAVE_PTHREAD_H */

    return;
}

/*----------------------------------------------------------------------*/
void T_D(const int trace_lvl, const unsigned int seq, const void *target, const int length)
{
    unsigned char *print = (unsigned char*)target;
    int   cnt = 0;
    int   prt_cnt;
    int   len = length;

    /* do nothing if trace_lvl is greater than output trace level */
    if (trace_lvl > trace_level)
    {
        return;
    }

#ifdef HAVE_PTHREAD_H
    pthread_mutex_lock(&msg_lock);
#endif  /* #ifdef HAVE_PTHREAD_H */
    fprintf(stderr, "%s:%08x\n", msg_head[trace_lvl], (unsigned int)seq);
#ifdef HAVE_PTHREAD_H
    fprintf(stderr, "%p ", (void *)pthread_self());
#endif  /* #ifdef HAVE_PTHREAD_H */

    while (len > 0)
    {
        /* print address */
        fprintf(stderr, "%08X", cnt);
        /* print binary */
        for (prt_cnt = 0; prt_cnt < 16; prt_cnt++)
        {
            if (prt_cnt % 8 == 0)
            {
                fprintf(stderr, " ");
            }
            if (len-prt_cnt <= 0)
            {
                fprintf(stderr, "   ");
            } else {
                fprintf(stderr, " %02X", *(print+prt_cnt));
            }
        }
        /* print ascii */
        fprintf(stderr, " |");
        for (prt_cnt = 0; prt_cnt < 16; prt_cnt++)
        {
            if ( (*print >= 0x20) && (*print <= 0x7E) )
            {
                fprintf(stderr, "%c", *print);
            } else
            {
                fprintf(stderr, ".");
            }
            print++;
            len--;
            if (len <= 0)
            {
                break;
            }
        }
        fprintf(stderr, "|\n");
        cnt += 16;
    }
#ifdef HAVE_PTHREAD_H
    pthread_mutex_unlock(&msg_lock);
#endif  /* #ifdef HAVE_PTHREAD_H */

    return;
}

/*----------------------------------------------------------------------*/
int T_init(int trace_lvl)
{
    if ( (trace_lvl < T_E) ||
         (trace_lvl > T_D2) )
    {
        T_M(T_E, (unsigned int)-1, "debug level over ranged.\n");
        return(-1);
    }

    trace_level = trace_lvl;

#ifdef HAVE_PTHREAD_H
    pthread_mutex_init(&msg_lock, NULL);
#endif  /* #ifdef HAVE_PTHREAD_H */

    return(0);
}

/* end of trace.c */
