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
 * @file geodb_layer.h
 * @author 350137278@qq.com
 * @brief geodb_layer
 *
 * @version 1.0.1
 * @create     2013-04-24
 * @date 2024-10-05 22:47:44
 */
#ifndef GEODB_LAYER_H__
#define GEODB_LAYER_H__

#ifdef    __cplusplus
extern "C" {
#endif

#include "geodbapi.h"


/*********************************************************************
sqlite>

-- 创建空间事件库 SQL: geodb_events
CREATE TABLE IF NOT EXISTS geodb_events(
    event_id        INTEGER PRIMARY KEY AUTOINCREMENT,
    event_name      VARCHAR(30) DEFAULT NULL,
    timestamp       REAL
);

-- 创建空间数据库 SQL: geodb_layers
CREATE TABLE IF NOT EXISTS geodb_layers(
    layer_id        INTEGER PRIMARY KEY AUTOINCREMENT,
    layer_name      VARCHAR(30) DEFAULT NULL,
    table_space     VARCHAR(30) DEFAULT NULL,
    table_owner     VARCHAR(30) DEFAULT NULL,
    user_table      VARCHAR(30) UNIQUE NOT NULL,
    col_userid      VARCHAR(30) NOT NULL,
    col_shapeid     VARCHAR(30) NOT NULL DEFAULT 'shapeid',
    col_updatetime  VARCHAR(30) NOT NULL DEFAULT 'updatetime',
    shape_table     VARCHAR(30) UNIQUE NOT NULL,
    index_table     VARCHAR(30) UNIQUE NOT NULL,
    event_table     VARCHAR(30) UNIQUE NOT NULL,
    shape_type      INT(2) NOT NULL,
    shape_encode    VARCHAR(9) NOT NULL DEFAULT 'WKB',
    next_shapeid    INTEGER,
    level_min       INTEGER,
    level_max       INTEGER,
    xmin            REAL NOT NULL DEFAULT 0,
    ymin            REAL NOT NULL DEFAULT 0,
    xmax            REAL NOT NULL DEFAULT 0,
    ymax            REAL NOT NULL DEFAULT 0,
    zmin            REAL NOT NULL DEFAULT 0,
    zmax            REAL NOT NULL DEFAULT 0,
    mmin            REAL NOT NULL DEFAULT 0,
    mmax            REAL NOT NULL DEFAULT 0,
    coordref        VARCHAR(30),
    description     VARCHAR(255),
    createtime      REAL,
    updatetime      REAL
);

-- 创建空间图层表 SQL: geodb_s_$(layer_id)
CREATE TABLE IF NOT EXISTS geodb_s_ffffffff(
    shapeid         INTEGER PRIMARY KEY AUTOINCREMENT,
    xmin            REAL,
    ymin            REAL,
    xmax            REAL,
    ymax            REAL,
    shape           BLOB,
    area            REAL DEFAULT 0,
    lens            REAL DEFAULT 0,
    flags           VARCHAR(10),
    timestamp       REAL
);

-- 创建图层空间表的索引表 SQL: geodb_i_$(layer_id)_$(level)
CREATE TABLE IF NOT EXISTS geodb_i_ffffffff_0(
    ix              INTEGER NOT NULL,
    iy              INTEGER NOT NULL,
    shapeid         INTEGER NOT NULL
);
CREATE INDEX geodb_i_ffffffff_0_uk ON geodb_i_ffffffff_0(ix, iy, shapeid);

-- 创建图层事件表 SQL: geodb_e_$(layer_id)
CREATE TABLE IF NOT EXISTS geodb_e_ffffffff(
    shapeid         INTEGER NOT NULL,
    event_id        INTEGER NOT NULL
);
CREATE INDEX geodb_e_ffffffff_uk ON geodb_e_ffffffff(shapeid, event_id);

*********************************************************************/
typedef struct geodb_layer_t
{
    int layer_id;
    char layer_name[GEODB_NAME_MAXLEN + 1];

    char table_space[GEODB_NAME_MAXLEN + 1];
    char table_owner[GEODB_NAME_MAXLEN + 1];

    char user_table[GEODB_NAME_MAXLEN + 1];
    char col_userid[GEODB_NAME_MAXLEN + 1];
    char col_shapeid[GEODB_NAME_MAXLEN + 1];
    char col_updatetime[GEODB_NAME_MAXLEN + 1];

    char shape_table[GEODB_NAME_MAXLEN + 1];
    char index_table[GEODB_NAME_MAXLEN + 1];
    char event_table[GEODB_NAME_MAXLEN + 1];

    char shape_type[GEODB_NAME_MAXLEN + 1];
    char shape_encode[10];

    int64_t next_shapeid;

    int level_min;
    int level_max;

    double xmin;
    double ymin;
    double xmax;
    double ymax;
    double zmin;
    double zmax;
    double mmin;
    double mmax;

    char coordref[GEODB_NAME_MAXLEN + 1];
    char description[256];

    double create_time;
    double update_time;
} * geodb_layer;






#ifdef    __cplusplus
}
#endif
#endif /* GEODB_LAYER_H__ */