/*
 * The Sleuth Kit
 *
 * Brian Carrier [carrier <at> sleuthkit [dot] org]
 * Copyright (c) 2003-2014 Brian Carrier.  All rights reserved
 */

/**
 * \file tsk_hashdb_i.h
 * Contains the internal library definitions for the hash database functions.  This should
 * be included by the code in the hash database library.
 */

#ifndef _TSK_HASHDB_I_H
#define _TSK_HASHDB_I_H

// Include the other internal TSK header files
#include "tsk/base/tsk_base_i.h"

// include the external header file
#include "tsk_hashdb.h"

#include <string.h>
#include <ctype.h>
#include <wchar.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <locale.h>

#ifdef TSK_WIN32
#include <io.h>
#include <fcntl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define TSK_HDB_MAXLEN	512     ///< Default buffer size used in many places

#define TSK_HDB_OFF_LEN 16      ///< Number of digits used in offset field in index

/**
 * Get the length of an index file line - 2 for comma and newline 
 */
#define TSK_HDB_IDX_LEN(x) \
    ( TSK_HDB_HTYPE_LEN(x) + TSK_HDB_OFF_LEN + 2)

/**
 * Strings used in index header.  It is one longer than a 
 * sha-1 hash - so that it always sorts to the top */
#define TSK_HDB_IDX_HEAD_TYPE_STR	"00000000000000000000000000000000000000000"
#define TSK_HDB_IDX_HEAD_NAME_STR	"00000000000000000000000000000000000000001"

    // "Base" hash database functions.
    extern uint8_t hdb_base_db_name_from_path(TSK_HDB_INFO *hdb_info);
    extern uint8_t hdb_info_base_open(TSK_HDB_INFO *, const TSK_TCHAR *);
    extern const TSK_TCHAR *hdb_base_get_db_path(TSK_HDB_INFO *);
    extern const char *hdb_base_get_db_name(TSK_HDB_INFO *);
    extern const TSK_TCHAR *hdb_base_get_index_path(TSK_HDB_INFO *);
    extern uint8_t hdb_base_has_index(TSK_HDB_INFO*, TSK_HDB_HTYPE_ENUM);
    extern uint8_t hdb_base_make_index(TSK_HDB_INFO *, TSK_TCHAR *);
    extern uint8_t hdb_base_open_index(TSK_HDB_INFO *, TSK_HDB_HTYPE_ENUM);
    extern int8_t hdb_base_lookup_str(TSK_HDB_INFO*, const char*, TSK_HDB_FLAG_ENUM, TSK_HDB_LOOKUP_FN, void*);
    extern int8_t hdb_base_lookup_bin(TSK_HDB_INFO*, uint8_t *, uint8_t, TSK_HDB_FLAG_ENUM, TSK_HDB_LOOKUP_FN, void*);
    extern uint8_t hdb_base_has_verbose_lookup(TSK_HDB_INFO*);
    extern int8_t hdb_base_lookup_verbose_str(TSK_HDB_INFO*, const char*, void**);
    extern uint8_t hdb_base_add_entry(TSK_HDB_INFO*, const char*, const char*, const char*, const char*, const char *);
    extern void hdb_info_base_close(TSK_HDB_INFO *);

    // Hash database functions common to all text format hash databases
    // (NSRL, md5sum, EnCase, HashKeeper, index only). These databases have
    // external indexes. 
    extern TSK_TEXT_HDB_INFO *text_hdb_open(FILE *hDb, const TSK_TCHAR *db_path);
    extern uint8_t text_hdb_open_idx(TSK_TEXT_HDB_INFO *hdb_info, TSK_HDB_HTYPE_ENUM htype);
    extern uint8_t text_hdb_idx_initialize(TSK_TEXT_HDB_INFO *, TSK_TCHAR *);
    extern uint8_t text_hdb_idx_add_entry_str(TSK_TEXT_HDB_INFO *, char *, TSK_OFF_T);
    extern uint8_t text_hdb_idx_add_entry_bin(TSK_TEXT_HDB_INFO *hdb_info, 
        unsigned char *hvalue, int hlen, TSK_OFF_T offset);
    extern uint8_t text_hdb_idx_finalize(TSK_TEXT_HDB_INFO *);
    extern int8_t text_hdb_lookup_str(TSK_HDB_INFO * hdb_info_base, 
        const char *hash, TSK_HDB_FLAG_ENUM flags, TSK_HDB_LOOKUP_FN action,
        void *ptr);
    extern int8_t text_hdb_lookup_bin(TSK_HDB_INFO * hdb_info, uint8_t * hash, 
        uint8_t len, TSK_HDB_FLAG_ENUM flags, 
        TSK_HDB_LOOKUP_FN action, void *ptr);
    extern void text_db_close(TSK_HDB_INFO *hdb_info) ;

    // Hash database functions for NSRL hash databases. 
    extern uint8_t nsrl_test(FILE *);
    extern TSK_HDB_INFO *nsrl_open(FILE *hDb, const TSK_TCHAR *db_path);
    extern uint8_t nsrl_makeindex(TSK_HDB_INFO *, TSK_TCHAR * htype);
    extern uint8_t nsrl_getentry(TSK_HDB_INFO *, const char *, TSK_OFF_T,
                                 TSK_HDB_FLAG_ENUM, TSK_HDB_LOOKUP_FN,
                                 void *);

    // Hash database functions for hash databases generated using md5Sum. 
    extern uint8_t md5sum_test(FILE *);
    extern TSK_HDB_INFO *md5sum_open(FILE *hDb, const TSK_TCHAR *db_path);
    extern uint8_t md5sum_makeindex(TSK_HDB_INFO *, TSK_TCHAR * htype);
    extern uint8_t md5sum_getentry(TSK_HDB_INFO *, const char *, TSK_OFF_T,
                                   TSK_HDB_FLAG_ENUM, TSK_HDB_LOOKUP_FN,
                                   void *);

    // Hash database functions for hash databases generated using EnCase. 
    extern uint8_t encase_test(FILE *);
    extern TSK_HDB_INFO *encase_open(FILE *hDb, const TSK_TCHAR *db_path);
    extern uint8_t encase_make_index(TSK_HDB_INFO *, TSK_TCHAR * htype);
    extern uint8_t encase_get_entry(TSK_HDB_INFO *, const char *, TSK_OFF_T,
                                   TSK_HDB_FLAG_ENUM, TSK_HDB_LOOKUP_FN,
                                   void *);

    // Hash database functions for hash databases generated using HashKeeper. 
    extern uint8_t hk_test(FILE *);
    extern TSK_HDB_INFO *hk_open(FILE *hDb, const TSK_TCHAR *db_path);
    extern uint8_t hk_makeindex(TSK_HDB_INFO *, TSK_TCHAR * htype);
    extern uint8_t hk_getentry(TSK_HDB_INFO *, const char *, TSK_OFF_T,
                               TSK_HDB_FLAG_ENUM, TSK_HDB_LOOKUP_FN,
                               void *);

    // Hash database functions for external index files standing in for the 
    // original hash databases. 
    extern TSK_HDB_INFO *idxonly_open(const TSK_TCHAR *db_path);
    extern uint8_t idxonly_makeindex(TSK_HDB_INFO *, TSK_TCHAR * htype);
    extern uint8_t idxonly_getentry(TSK_HDB_INFO *, const char *,
                                    TSK_OFF_T, TSK_HDB_FLAG_ENUM,
                                    TSK_HDB_LOOKUP_FN, void *);

    // Hash database functions for SQLite hash databases.
    extern uint8_t sqlite_hdb_is_sqlite_file(FILE * hFile);
	extern uint8_t sqlite_hdb_create_db(TSK_TCHAR*);
    extern TSK_HDB_INFO *sqlite_hdb_open(TSK_TCHAR *db_path);
    extern int8_t sqlite_hdb_lookup_str(TSK_HDB_INFO *, const char *, TSK_HDB_FLAG_ENUM, TSK_HDB_LOOKUP_FN, void *);
    extern int8_t sqlite_hdb_lookup_bin(TSK_HDB_INFO *, uint8_t *, uint8_t, TSK_HDB_FLAG_ENUM, TSK_HDB_LOOKUP_FN, void *);
    extern uint8_t sqlite_hdb_has_verbose_lookup(TSK_HDB_INFO *hdb_info);
    extern int8_t sqlite_hdb_lookup_verbose_str(TSK_HDB_INFO *, const char *, void **);
    extern uint8_t sqlite_hdb_add_entry(TSK_HDB_INFO *hdb_info_base, 
        const char *filename, const char *md5, const char *sha1, 
        const char *sha256, const char *comment);
    extern void sqlite_hdb_close(TSK_HDB_INFO *hdb_info_base);

#ifdef __cplusplus
}
#endif
#endif
