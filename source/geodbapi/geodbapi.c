/**
 * Copyright © 2024 MapAware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file geodbapi.c
 * @author 350137278@qq.com
 * @brief Geography Database API
 *
 * @version 1.0.2
 * @create     2013-04-24
 * @date 2024-11-02 02:09:40
 */
#include "geodbapi_i.h"


static int on_next_row(void* userarg, int column_size, char* column_value[], char* column_name[])
{
    int i;

    for (i = 0; i < column_size; i++) {
        printf("%s = %s\n", column_name[i], (char*)column_value[i]);
    }
    printf("----\n");
    return 0;
}


int geodb_context_open(const char* dbfile)
{
    int ret, flags;
    char * err;

    ret = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);

    ret = sqlite3_initialize();

    flags = SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_SHAREDCACHE;

    sqlite3 *geodb = 0;
    ret = sqlite3_open_v2(dbfile, &geodb, flags, 0);

    ret = sqlite3_key_v2(geodb, 0, "pass", 4);

    err = 0;
    ret = sqlite3_exec(geodb, "select * from abc", on_next_row, 0 /* userdata */, &err);
    if (err) {
        sqlite3_free(err);
    }


    ret = sqlite3_close_v2(geodb);
    return ret;
}