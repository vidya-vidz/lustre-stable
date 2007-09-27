/* -*- mode: c; c-basic-offset: 8; indent-tabs-mode: nil; -*-
 * vim:expandtab:shiftwidth=8:tabstop=8:
 *
 *  Copyright (C) 2001, 2002 Cluster File Systems, Inc.
 *
 *   This file is part of Lustre, http://www.lustre.org.
 *
 *   Lustre is free software; you can redistribute it and/or
 *   modify it under the terms of version 2 of the GNU General Public
 *   License as published by the Free Software Foundation.
 *
 *   Lustre is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Lustre; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _OBD_SUPPORT
#define _OBD_SUPPORT

#include <libcfs/kp30.h>
#include <lvfs.h>

/* global variables */
extern atomic_t obd_memory;
extern int obd_memmax;
extern unsigned int obd_fail_loc;
extern unsigned int obd_fail_val;
extern unsigned int obd_debug_peer_on_timeout;
extern unsigned int obd_dump_on_timeout;
extern unsigned int obd_dump_on_eviction;
/* obd_timeout should only be used for recovery, not for 
   networking / disk / timings affected by load (use Adaptive Timeouts) */
extern unsigned int obd_timeout;          /* seconds */
extern unsigned int ldlm_timeout;         /* seconds */
extern unsigned int adaptive_timeout_max; /* seconds */
extern unsigned int adaptive_timeout_history; /* seconds */
extern unsigned int obd_sync_filter;
extern unsigned int obd_max_dirty_pages;
extern atomic_t obd_dirty_pages;
extern cfs_waitq_t obd_race_waitq;
extern int obd_race_state;
extern unsigned int obd_alloc_fail_rate;

/* Timeout definitions */
#define OBD_TIMEOUT_DEFAULT 100
#define LDLM_TIMEOUT_DEFAULT 20
/* Time to wait for all clients to reconnect during recovery */
/* Should be very conservative; must catch the first reconnect after reboot */
#define OBD_RECOVERY_TIMEOUT (obd_timeout * 5 / 2)
/* Change recovery-small 26b time if you change this */
#define PING_INTERVAL max(obd_timeout / 4, 1U)
/* Client may skip 1 ping; wait for 2.5 */
#define PING_EVICT_TIMEOUT (PING_INTERVAL * 5 / 2)
#define DISK_TIMEOUT 50          /* Beyond this we warn about disk speed */
#define CONNECTION_SWITCH_MIN 5U /* Connection switching rate limiter */
 /* Max connect interval for nonresponsive servers; ~50s to avoid building up
    connect requests in the LND queues, but within obd_timeout so we don't
    miss the recovery window */
#define CONNECTION_SWITCH_MAX min(50U, max(CONNECTION_SWITCH_MIN,obd_timeout))
#define CONNECTION_SWITCH_INC 5  /* Connection timeout backoff */
#ifndef CRAY_XT3
/* In general this should be low to have quick detection of a system 
   running on a backup server. (If it's too low, import_select_connection
   will increase the timeout anyhow.)  */
#define INITIAL_CONNECT_TIMEOUT max(CONNECTION_SWITCH_MIN,obd_timeout/20)
#else
/* ...but for very large systems (e.g. CRAY) we need to keep the initial 
   connect t.o. high (bz 10803), because they will nearly ALWAYS be doing the
   connects for the first time (clients "reboot" after every process, so no
   chance to generate adaptive timeout data. */
#define INITIAL_CONNECT_TIMEOUT max(CONNECTION_SWITCH_MIN,obd_timeout/2)
#endif
#define LONG_UNLINK 300          /* Unlink should happen before now */


#define OBD_FAIL_MDS                     0x100
#define OBD_FAIL_MDS_HANDLE_UNPACK       0x101
#define OBD_FAIL_MDS_GETATTR_NET         0x102
#define OBD_FAIL_MDS_GETATTR_PACK        0x103
#define OBD_FAIL_MDS_READPAGE_NET        0x104
#define OBD_FAIL_MDS_READPAGE_PACK       0x105
#define OBD_FAIL_MDS_SENDPAGE            0x106
#define OBD_FAIL_MDS_REINT_NET           0x107
#define OBD_FAIL_MDS_REINT_UNPACK        0x108
#define OBD_FAIL_MDS_REINT_SETATTR       0x109
#define OBD_FAIL_MDS_REINT_SETATTR_WRITE 0x10a
#define OBD_FAIL_MDS_REINT_CREATE        0x10b
#define OBD_FAIL_MDS_REINT_CREATE_WRITE  0x10c
#define OBD_FAIL_MDS_REINT_UNLINK        0x10d
#define OBD_FAIL_MDS_REINT_UNLINK_WRITE  0x10e
#define OBD_FAIL_MDS_REINT_LINK          0x10f
#define OBD_FAIL_MDS_REINT_LINK_WRITE    0x110
#define OBD_FAIL_MDS_REINT_RENAME        0x111
#define OBD_FAIL_MDS_REINT_RENAME_WRITE  0x112
#define OBD_FAIL_MDS_OPEN_NET            0x113
#define OBD_FAIL_MDS_OPEN_PACK           0x114
#define OBD_FAIL_MDS_CLOSE_NET           0x115
#define OBD_FAIL_MDS_CLOSE_PACK          0x116
#define OBD_FAIL_MDS_CONNECT_NET         0x117
#define OBD_FAIL_MDS_CONNECT_PACK        0x118
#define OBD_FAIL_MDS_REINT_NET_REP       0x119
#define OBD_FAIL_MDS_DISCONNECT_NET      0x11a
#define OBD_FAIL_MDS_GETSTATUS_NET       0x11b
#define OBD_FAIL_MDS_GETSTATUS_PACK      0x11c
#define OBD_FAIL_MDS_STATFS_PACK         0x11d
#define OBD_FAIL_MDS_STATFS_NET          0x11e
#define OBD_FAIL_MDS_GETATTR_NAME_NET    0x11f
#define OBD_FAIL_MDS_PIN_NET             0x120
#define OBD_FAIL_MDS_UNPIN_NET           0x121
#define OBD_FAIL_MDS_ALL_REPLY_NET       0x122
#define OBD_FAIL_MDS_ALL_REQUEST_NET     0x123
#define OBD_FAIL_MDS_SYNC_NET            0x124
#define OBD_FAIL_MDS_SYNC_PACK           0x125
#define OBD_FAIL_MDS_DONE_WRITING_NET    0x126
#define OBD_FAIL_MDS_DONE_WRITING_PACK   0x127
#define OBD_FAIL_MDS_ALLOC_OBDO          0x128
#define OBD_FAIL_MDS_PAUSE_OPEN          0x129
#define OBD_FAIL_MDS_STATFS_LCW_SLEEP    0x12a
#define OBD_FAIL_MDS_OPEN_CREATE         0x12b
#define OBD_FAIL_MDS_OST_SETATTR         0x12c
#define OBD_FAIL_MDS_QUOTACHECK_NET      0x12d
#define OBD_FAIL_MDS_QUOTACTL_NET        0x12e
#define OBD_FAIL_MDS_CLIENT_ADD          0x12f
#define OBD_FAIL_MDS_GETXATTR_NET        0x130
#define OBD_FAIL_MDS_GETXATTR_PACK       0x131
#define OBD_FAIL_MDS_SETXATTR_NET        0x132
#define OBD_FAIL_MDS_SETXATTR            0x133
#define OBD_FAIL_MDS_SETXATTR_WRITE      0x134
#define OBD_FAIL_MDS_FS_SETUP            0x135
#define OBD_FAIL_MDS_RESEND              0x136
#define OBD_FAIL_MDS_LLOG_CREATE_FAILED  0x137
#define OBD_FAIL_MDS_LOV_SYNC_RACE       0x138
#define OBD_FAIL_MDS_OSC_PRECREATE       0x139
#define OBD_FAIL_MDS_LLOG_SYNC_TIMEOUT   0x13a

#define OBD_FAIL_OST                     0x200
#define OBD_FAIL_OST_CONNECT_NET         0x201
#define OBD_FAIL_OST_DISCONNECT_NET      0x202
#define OBD_FAIL_OST_GET_INFO_NET        0x203
#define OBD_FAIL_OST_CREATE_NET          0x204
#define OBD_FAIL_OST_DESTROY_NET         0x205
#define OBD_FAIL_OST_GETATTR_NET         0x206
#define OBD_FAIL_OST_SETATTR_NET         0x207
#define OBD_FAIL_OST_OPEN_NET            0x208
#define OBD_FAIL_OST_CLOSE_NET           0x209
#define OBD_FAIL_OST_BRW_NET             0x20a
#define OBD_FAIL_OST_PUNCH_NET           0x20b
#define OBD_FAIL_OST_STATFS_NET          0x20c
#define OBD_FAIL_OST_HANDLE_UNPACK       0x20d
#define OBD_FAIL_OST_BRW_WRITE_BULK      0x20e
#define OBD_FAIL_OST_BRW_READ_BULK       0x20f
#define OBD_FAIL_OST_SYNC_NET            0x210
#define OBD_FAIL_OST_ALL_REPLY_NET       0x211
#define OBD_FAIL_OST_ALL_REQUEST_NET     0x212
#define OBD_FAIL_OST_LDLM_REPLY_NET      0x213
#define OBD_FAIL_OST_BRW_PAUSE_BULK      0x214
#define OBD_FAIL_OST_ENOSPC              0x215
#define OBD_FAIL_OST_EROFS               0x216
#define OBD_FAIL_OST_ENOENT              0x217
#define OBD_FAIL_OST_QUOTACHECK_NET      0x218
#define OBD_FAIL_OST_QUOTACTL_NET        0x219
#define OBD_FAIL_OST_CHECKSUM_RECEIVE    0x21a
#define OBD_FAIL_OST_CHECKSUM_SEND       0x21b
#define OBD_FAIL_OST_BRW_SIZE            0x21c
#define OBD_FAIL_OST_DROP_REQ            0x21d
#define OBD_FAIL_OST_SETATTR_CREDITS     0x21e
#define OBD_FAIL_OST_HOLD_WRITE_RPC      0x21f
#define OBD_FAIL_OST_LLOG_RECOVERY_TIMEOUT 0x221
#define OBD_FAIL_OST_CANCEL_COOKIE_TIMEOUT 0x222
#define OBD_FAIL_OST_PAUSE_CREATE        0x223
#define OBD_FAIL_OST_BRW_PAUSE_PACK      0x224

#define OBD_FAIL_LDLM                    0x300
#define OBD_FAIL_LDLM_NAMESPACE_NEW      0x301
#define OBD_FAIL_LDLM_ENQUEUE            0x302
#define OBD_FAIL_LDLM_CONVERT            0x303
#define OBD_FAIL_LDLM_CANCEL             0x304
#define OBD_FAIL_LDLM_BL_CALLBACK        0x305
#define OBD_FAIL_LDLM_CP_CALLBACK        0x306
#define OBD_FAIL_LDLM_GL_CALLBACK        0x307
#define OBD_FAIL_LDLM_ENQUEUE_EXTENT_ERR 0x308
#define OBD_FAIL_LDLM_ENQUEUE_INTENT_ERR 0x309
#define OBD_FAIL_LDLM_CREATE_RESOURCE    0x30a
#define OBD_FAIL_LDLM_ENQUEUE_BLOCKED    0x30b
#define OBD_FAIL_LDLM_REPLY              0x30c
#define OBD_FAIL_LDLM_RECOV_CLIENTS      0x30d
#define OBD_FAIL_LDLM_ENQUEUE_OLD_EXPORT 0x30e
#define OBD_FAIL_LDLM_GLIMPSE            0x30f
#define OBD_FAIL_LDLM_CANCEL_RACE        0x310
#define OBD_FAIL_LDLM_CANCEL_EVICT_RACE  0x311

#define OBD_FAIL_OSC                     0x400
#define OBD_FAIL_OSC_BRW_READ_BULK       0x401
#define OBD_FAIL_OSC_BRW_WRITE_BULK      0x402
#define OBD_FAIL_OSC_LOCK_BL_AST         0x403
#define OBD_FAIL_OSC_LOCK_CP_AST         0x404
#define OBD_FAIL_OSC_MATCH               0x405
#define OBD_FAIL_OSC_BRW_PREP_REQ        0x406
#define OBD_FAIL_OSC_SHUTDOWN            0x407
#define OBD_FAIL_OSC_CHECKSUM_RECEIVE    0x408
#define OBD_FAIL_OSC_CHECKSUM_SEND       0x409

#define OBD_FAIL_PTLRPC                  0x500
#define OBD_FAIL_PTLRPC_ACK              0x501
#define OBD_FAIL_PTLRPC_RQBD             0x502
#define OBD_FAIL_PTLRPC_BULK_GET_NET     0x503
#define OBD_FAIL_PTLRPC_BULK_PUT_NET     0x504
#define OBD_FAIL_PTLRPC_DROP_RPC         0x505
#define OBD_FAIL_PTLRPC_DELAY_SEND       0x506
#define OBD_FAIL_PTLRPC_DELAY_RECOV      0x507
#define OBD_FAIL_PTLRPC_CLIENT_BULK_CB   0x508
#define OBD_FAIL_PTLRPC_PAUSE_REQ        0x50a
#define OBD_FAIL_PTLRPC_PAUSE_REP        0x50c

#define OBD_FAIL_OBD_PING_NET            0x600
#define OBD_FAIL_OBD_LOG_CANCEL_NET      0x601
#define OBD_FAIL_OBD_LOGD_NET            0x602
#define OBD_FAIL_OBD_QC_CALLBACK_NET     0x603
#define OBD_FAIL_OBD_DQACQ               0x604

#define OBD_FAIL_TGT_REPLY_NET           0x700
#define OBD_FAIL_TGT_CONN_RACE           0x701
#define OBD_FAIL_TGT_FORCE_RECONNECT     0x702
#define OBD_FAIL_TGT_DELAY_CONNECT       0x703
#define OBD_FAIL_TGT_DELAY_RECONNECT     0x704
#define OBD_FAIL_TGT_DELAY_PRECREATE     0x705

#define OBD_FAIL_MDC_REVALIDATE_PAUSE    0x800
#define OBD_FAIL_MDC_ENQUEUE_PAUSE       0x801
#define OBD_FAIL_MDC_OLD_EXT_FLAGS       0x802

#define OBD_FAIL_MGS                     0x900
#define OBD_FAIL_MGS_ALL_REQUEST_NET     0x901
#define OBD_FAIL_MGS_ALL_REPLY_NET       0x902
#define OBD_FAIL_MGC_PAUSE_PROCESS_LOG   0x903
#define OBD_FAIL_MGS_PAUSE_REQ           0x904
#define OBD_FAIL_MGS_PAUSE_TARGET_REG    0x905

#define OBD_FAIL_QUOTA_QD_COUNT_32BIT    0xA00

#define OBD_FAIL_LPROC_REMOVE            0xB00

#define OBD_FAIL_GENERAL_ALLOC           0xC00

/* Failure injection control */
#define OBD_FAIL_MASK_SYS    0x0000FF00
#define OBD_FAIL_MASK_LOC   (0x000000FF | OBD_FAIL_MASK_SYS)
#define OBD_FAIL_ONCE        0x80000000
#define OBD_FAILED           0x40000000
/* The following flags aren't made to be combined */
#define OBD_FAIL_SKIP        0x20000000 /* skip N then fail */
#define OBD_FAIL_SOME        0x10000000 /* fail N times */
#define OBD_FAIL_RAND        0x08000000 /* fail 1/N of the time */
#define OBD_FAIL_USR1        0x04000000 /* user flag */

static inline int obd_fail_check(__u32 id)
{
        static int count = 0;
        if (likely((obd_fail_loc & OBD_FAIL_MASK_LOC) != 
                   (id & OBD_FAIL_MASK_LOC)))
                return 0;
        
        if ((obd_fail_loc & (OBD_FAILED | OBD_FAIL_ONCE)) ==
            (OBD_FAILED | OBD_FAIL_ONCE)) {
                count = 0; /* paranoia */
                return 0;
        }

        if (obd_fail_loc & OBD_FAIL_RAND) {
                unsigned int ll_rand(void);
                if (obd_fail_val < 2)
                        return 0;
                if (ll_rand() % obd_fail_val > 0)
                        return 0;
        }

        if (obd_fail_loc & OBD_FAIL_SKIP) {
                count++;
                if (count < obd_fail_val) 
                        return 0;
                count = 0;
        }

        /* Overridden by FAIL_ONCE */
        if (obd_fail_loc & OBD_FAIL_SOME) {
                count++;
                if (count >= obd_fail_val) {
                        count = 0;
                        /* Don't fail anymore */
                        obd_fail_loc |= OBD_FAIL_ONCE;
                }
        }

        obd_fail_loc |= OBD_FAILED;
        /* Handle old checks that OR in this */
        if (id & OBD_FAIL_ONCE)
                obd_fail_loc |= OBD_FAIL_ONCE;

        return 1;
}

#define OBD_FAIL_CHECK(id)                                                   \
({                                                                           \
        int _ret_ = 0;                                                       \
        if (unlikely(obd_fail_loc && (_ret_ = obd_fail_check(id)))) {        \
                CERROR("*** obd_fail_loc=%x ***\n", id);                     \
        }                                                                    \
        _ret_;                                                               \
})

#define OBD_FAIL_CHECK_QUIET(id)                                             \
        (unlikely(obd_fail_loc) ? obd_fail_check(id) : 0)

/* deprecated - just use OBD_FAIL_CHECK */
#define OBD_FAIL_CHECK_ONCE OBD_FAIL_CHECK

#define OBD_FAIL_RETURN(id, ret)                                             \
do {                                                                         \
        if (unlikely(obd_fail_loc && obd_fail_check(id))) {                  \
                CERROR("*** obd_fail_return=%x rc=%d ***\n", id, ret);       \
                RETURN(ret);                                                 \
        }                                                                    \
} while(0)

#define OBD_FAIL_TIMEOUT(id, secs)                                           \
({      int _ret_ = 0;                                                       \
        if (unlikely(obd_fail_loc && (_ret_ = obd_fail_check(id)))) {        \
                CERROR("obd_fail_timeout id %x sleeping for %d secs\n",      \
                       (id), (secs));                                        \
                set_current_state(TASK_UNINTERRUPTIBLE);                     \
                cfs_schedule_timeout(CFS_TASK_UNINT,                         \
                                    cfs_time_seconds(secs));                 \
                set_current_state(TASK_RUNNING);                             \
                CERROR("obd_fail_timeout id %x awake\n", (id));              \
        }                                                                    \
        _ret_;                                                               \
})

#define OBD_FAIL_TIMEOUT_MS(id, ms)                                          \
({      int _ret_ = 0;                                                       \
        if (unlikely(obd_fail_loc && (_ret_ = obd_fail_check(id)))) {        \
                CERROR("obd_fail_timeout id %x sleeping for %d ms\n",        \
                       (id), (ms));                                          \
                set_current_state(TASK_UNINTERRUPTIBLE);                     \
                cfs_schedule_timeout(CFS_TASK_UNINT,                         \
                                     cfs_time_seconds(ms)/1000);             \
                set_current_state(TASK_RUNNING);                             \
                CERROR("obd_fail_timeout id %x awake\n", (id));              \
        }                                                                    \
        _ret_;                                                               \
})

#ifdef __KERNEL__
/* The idea here is to synchronise two threads to force a race. The
 * first thread that calls this with a matching fail_loc is put to
 * sleep. The next thread that calls with the same fail_loc wakes up
 * the first and continues. */
#define OBD_RACE(id)                                                         \
do {                                                                         \
        if (unlikely(obd_fail_loc && obd_fail_check(id))) {                  \
                obd_race_state = 0;                                          \
                CERROR("obd_race id %x sleeping\n", (id));                   \
                OBD_SLEEP_ON(obd_race_waitq, obd_race_state != 0);           \
                CERROR("obd_fail_race id %x awake\n", (id));                 \
        } else if ((obd_fail_loc & OBD_FAIL_MASK_LOC) ==                     \
                    ((id) & OBD_FAIL_MASK_LOC)) {                            \
                CERROR("obd_fail_race id %x waking\n", (id));                \
                obd_race_state = 1;                                          \
                wake_up(&obd_race_waitq);                                    \
        }                                                                    \
} while(0)
#else
/* sigh.  an expedient fix until OBD_RACE is fixed up */
#define OBD_RACE(foo) do {} while(0)
#endif

#define fixme() CDEBUG(D_OTHER, "FIXME\n");

extern atomic_t libcfs_kmemory;

#ifdef RANDOM_FAIL_ALLOC
#define HAS_FAIL_ALLOC_FLAG OBD_FAIL_CHECK_QUIET(OBD_FAIL_GENERAL_ALLOC)
#else
#define HAS_FAIL_ALLOC_FLAG 0
#endif

#define OBD_ALLOC_FAIL_BITS 24
#define OBD_ALLOC_FAIL_MASK ((1 << OBD_ALLOC_FAIL_BITS) - 1)
#define OBD_ALLOC_FAIL_MULT (OBD_ALLOC_FAIL_MASK / 100)

#if defined(LUSTRE_UTILS) /* this version is for utils only */
#define OBD_ALLOC_GFP(ptr, size, gfp_mask)                                    \
do {                                                                          \
        (ptr) = cfs_alloc(size, (gfp_mask));                                  \
        if ((ptr) == NULL) {                                                  \
                CERROR("kmalloc of '" #ptr "' (%d bytes) failed at %s:%d\n",  \
                       (int)(size), __FILE__, __LINE__);                      \
        } else {                                                              \
                memset(ptr, 0, size);                                         \
                CDEBUG(D_MALLOC, "kmalloced '" #ptr "': %d at %p\n",          \
                       (int)(size), ptr);                                     \
        }                                                                     \
} while (0)
#else /* this version is for the kernel and liblustre */
#define OBD_FREE_RTN0(ptr)                                                    \
({                                                                            \
        cfs_free(ptr);                                                        \
        (ptr) = NULL;                                                         \
        0;                                                                    \
})
#define OBD_ALLOC_GFP(ptr, size, gfp_mask)                                    \
do {                                                                          \
        (ptr) = cfs_alloc(size, (gfp_mask));                                  \
        if (likely((ptr) != NULL &&                                           \
                   (!HAS_FAIL_ALLOC_FLAG || obd_alloc_fail_rate == 0 ||       \
                    !obd_alloc_fail(ptr, #ptr, "km", size,                    \
                                    __FILE__, __LINE__) ||                    \
                    OBD_FREE_RTN0(ptr)))){                                    \
                memset(ptr, 0, size);                                         \
                atomic_add(size, &obd_memory);                                \
                if (atomic_read(&obd_memory) > obd_memmax)                    \
                        obd_memmax = atomic_read(&obd_memory);                \
                CDEBUG(D_MALLOC, "kmalloced '" #ptr "': %d at %p (tot %d)\n", \
                       (int)(size), ptr, atomic_read(&obd_memory));           \
        }                                                                     \
} while (0)
#endif

#ifndef OBD_ALLOC_MASK
# define OBD_ALLOC_MASK CFS_ALLOC_IO
#endif

#define OBD_ALLOC(ptr, size) OBD_ALLOC_GFP(ptr, size, OBD_ALLOC_MASK)
#define OBD_ALLOC_WAIT(ptr, size) OBD_ALLOC_GFP(ptr, size, CFS_ALLOC_STD)
#define OBD_ALLOC_PTR(ptr) OBD_ALLOC(ptr, sizeof *(ptr))
#define OBD_ALLOC_PTR_WAIT(ptr) OBD_ALLOC_WAIT(ptr, sizeof *(ptr))

#ifdef __arch_um__
# define OBD_VMALLOC(ptr, size) OBD_ALLOC(ptr, size)
#else
# define OBD_VMALLOC(ptr, size)                                               \
do {                                                                          \
        (ptr) = cfs_alloc_large(size);                                        \
        if ((ptr) == NULL) {                                                  \
                CERROR("vmalloc of '" #ptr "' (%d bytes) failed at %s:%d\n",  \
                       (int)(size), __FILE__, __LINE__);                      \
                CERROR("%d total bytes allocated by Lustre, %d by Portals\n", \
                       atomic_read(&obd_memory), atomic_read(&libcfs_kmemory));\
        } else {                                                              \
                memset(ptr, 0, size);                                         \
                atomic_add(size, &obd_memory);                                \
                if (atomic_read(&obd_memory) > obd_memmax)                    \
                        obd_memmax = atomic_read(&obd_memory);                \
                CDEBUG(D_MALLOC, "vmalloced '" #ptr "': %d at %p (tot %d)\n", \
                       (int)(size), ptr, atomic_read(&obd_memory));           \
        }                                                                     \
} while (0)
#endif

#ifdef CONFIG_DEBUG_SLAB
#define POISON(ptr, c, s) do {} while (0)
#else
#define POISON(ptr, c, s) memset(ptr, c, s)
#endif

#ifdef POISON_BULK
#define POISON_PAGE(page, val) do { memset(kmap(page), val, CFS_PAGE_SIZE);   \
                                    kunmap(page); } while (0)
#else
#define POISON_PAGE(page, val) do { } while (0)
#endif

#ifdef __KERNEL__
#define OBD_FREE(ptr, size)                                                   \
do {                                                                          \
        LASSERT(ptr);                                                         \
        atomic_sub(size, &obd_memory);                                        \
        CDEBUG(D_MALLOC, "kfreed '" #ptr "': %d at %p (tot %d).\n",           \
               (int)(size), ptr, atomic_read(&obd_memory));                   \
        POISON(ptr, 0x5a, size);                                              \
        cfs_free(ptr);                                                        \
        (ptr) = (void *)0xdeadbeef;                                           \
} while (0)

#ifdef HAVE_RCU
# ifdef HAVE_CALL_RCU_PARAM
#  define my_call_rcu(rcu, cb)            call_rcu(rcu, cb, rcu)
# else
#  define my_call_rcu(rcu, cb)            call_rcu(rcu, cb)
# endif
#else
# define my_call_rcu(rcu, cb)             (cb)(rcu)
#endif

#define OBD_FREE_RCU_CB(ptr, size, handle, free_cb)                           \
do {                                                                          \
        struct portals_handle *__h = (handle);                                \
        LASSERT(handle);                                                      \
        __h->h_ptr = (ptr);                                                   \
        __h->h_size = (size);                                                 \
        __h->h_free_cb = (void (*)(void *, size_t))(free_cb);                 \
        my_call_rcu(&__h->h_rcu, class_handle_free_cb);                       \
        (ptr) = (void *)0xdeadbeef;                                           \
} while(0)
#define OBD_FREE_RCU(ptr, size, handle) OBD_FREE_RCU_CB(ptr, size, handle, NULL)
#else
#define OBD_FREE(ptr, size) ((void)(size), free((ptr)))
#define OBD_FREE_RCU(ptr, size, handle) (OBD_FREE(ptr, size))
#define OBD_FREE_RCU_CB(ptr, size, handle, cb)     ((*(cb))(ptr, size))
#endif

#ifdef __arch_um__
# define OBD_VFREE(ptr, size) OBD_FREE(ptr, size)
#else
# define OBD_VFREE(ptr, size)                                                 \
do {                                                                          \
        LASSERT(ptr);                                                         \
        atomic_sub(size, &obd_memory);                                        \
        CDEBUG(D_MALLOC, "vfreed '" #ptr "': %d at %p (tot %d).\n",           \
               (int)(size), ptr, atomic_read(&obd_memory));                   \
        POISON(ptr, 0x5a, size);                                              \
        cfs_free_large(ptr);                                                  \
        (ptr) = (void *)0xdeadbeef;                                           \
} while (0)
#endif

/* we memset() the slab object to 0 when allocation succeeds, so DO NOT
 * HAVE A CTOR THAT DOES ANYTHING.  its work will be cleared here.  we'd
 * love to assert on that, but slab.c keeps kmem_cache_s all to itself. */
#define OBD_SLAB_FREE_RTN0(ptr, slab)                                         \
({                                                                            \
        cfs_mem_cache_free((slab), (ptr));                                    \
        (ptr) = NULL;                                                         \
        0;                                                                    \
}) 
#define OBD_SLAB_ALLOC(ptr, slab, type, size)                                 \
do {                                                                          \
        LASSERT(!in_interrupt());                                             \
        (ptr) = cfs_mem_cache_alloc(slab, (type));                            \
        if (likely((ptr) != NULL &&                                           \
                   (!HAS_FAIL_ALLOC_FLAG || obd_alloc_fail_rate == 0 ||       \
                    !obd_alloc_fail(ptr, #ptr, "slab-", size,                 \
                                    __FILE__, __LINE__) ||                    \
                    OBD_SLAB_FREE_RTN0(ptr, slab)))) {                        \
                memset(ptr, 0, size);                                         \
                atomic_add(size, &obd_memory);                                \
                if (atomic_read(&obd_memory) > obd_memmax)                    \
                        obd_memmax = atomic_read(&obd_memory);                \
                CDEBUG(D_MALLOC, "slab-alloced '"#ptr"': %d at %p (tot %d)\n",\
                       (int)(size), ptr, atomic_read(&obd_memory));           \
        }                                                                     \
} while (0)

#define OBD_FREE_PTR(ptr) OBD_FREE(ptr, sizeof *(ptr))

#define OBD_SLAB_FREE(ptr, slab, size)                                        \
do {                                                                          \
        LASSERT(ptr);                                                         \
        CDEBUG(D_MALLOC, "slab-freed '" #ptr "': %d at %p (tot %d).\n",       \
               (int)(size), ptr, atomic_read(&obd_memory));                   \
        atomic_sub(size, &obd_memory);                                        \
        POISON(ptr, 0x5a, size);                                              \
        cfs_mem_cache_free(slab, ptr);                                        \
        (ptr) = (void *)0xdeadbeef;                                           \
} while (0)

#define OBD_SLAB_ALLOC_PTR(ptr, slab)                                         \
        OBD_SLAB_ALLOC((ptr), (slab), CFS_ALLOC_STD, sizeof *(ptr))
#define OBD_SLAB_FREE_PTR(ptr, slab)                                          \
        OBD_SLAB_FREE((ptr), (slab), sizeof *(ptr))

#define KEY_IS(str) (keylen >= strlen(key) && strcmp(key, str) == 0)

#if defined(__linux__)
#include <linux/obd_support.h>
#elif defined(__APPLE__)
#include <darwin/obd_support.h>
#elif defined(__WINNT__)
#include <winnt/obd_support.h>
#else
#error Unsupported operating system.
#endif

#endif
