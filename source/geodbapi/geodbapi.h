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
 * @file geodbapi.h
 * @author 350137278@qq.com
 * @brief Geography Database API
 *
 * @version 1.0.1
 * @create     2013-04-24
 * @date 2024-10-28 21:32:55
 */
#ifndef GEODBAPI_H__
#define GEODBAPI_H__

#ifdef    __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if defined(GEODBAPI_DLL)
  /* win32 dynamic dll */
  # ifdef GEODBAPI_EXPORTS
  #   define GEODBAPI __declspec(dllexport)
  # else
  #   define GEODBAPI __declspec(dllimport)
  # endif
#else
  /* static lib or linux so */
  # define GEODBAPI  extern
#endif


#define GEODB_NAMELEN_MAX    30


typedef struct geodb_attr_t     * geodb_attr;
typedef struct geodb_conn_t     * geodb_conn;
typedef struct geodb_layer_t    * geodb_layer;
typedef struct geodb_shape_t    * geodb_shape;
typedef struct geodb_coldef_t   * geodb_coldef;
typedef struct geodb_stmt_t     * geodb_stmt;
typedef struct shape_filter_t   * shape_filter;


typedef enum {
    conn_attr = 0
} geodb_attr_name;


// geodb_attr API
GEODBAPI int geodb_attr_new(geodb_attr *attr);
GEODBAPI void geodb_attr_free(geodb_attr attr);

GEODBAPI int geodb_attr_set_integer(geodb_attr attr, geodb_attr_name name, int64_t value);
GEODBAPI int geodb_attr_get_integer(geodb_attr attr, geodb_attr_name name, int64_t *value);

GEODBAPI int geodb_attr_set_real(geodb_attr attr, geodb_attr_name name, double value);
GEODBAPI int geodb_attr_get_real(geodb_attr attr, geodb_attr_name name, double *value);

GEODBAPI int geodb_attr_set_bytes(geodb_attr attr, geodb_attr_name name, const char *inbuf, int bufsize);
GEODBAPI int geodb_attr_get_bytes(geodb_attr attr, geodb_attr_name name, char *outbuf, int bufsize);


// geodb_layer API
GEODBAPI int geodb_layer_new(geodb_attr attr, geodb_layer *layer);
GEODBAPI void geodb_layer_free(geodb_layer layer);


// geodb_coldef API
GEODBAPI int geodb_coldef_new(geodb_attr attr, geodb_coldef *coldef);
GEODBAPI void geodb_coldef_free(geodb_coldef coldef);


// geodb_stmt API
GEODBAPI int geodb_stmt_new(geodb_attr attr, geodb_stmt *stmt);
GEODBAPI int geodb_stmt_free(geodb_stmt stmt);


// geodb_conn API
GEODBAPI int geodb_conn_new(geodb_attr attr, geodb_conn *dbconn);
GEODBAPI void geodb_conn_free(geodb_conn dbconn);

GEODBAPI int geodb_conn_open(geodb_conn dbconn, const char *main_dbfile, int openflags);
GEODBAPI int geodb_conn_close(geodb_conn dbconn);

GEODBAPI int geodb_attach_db(geodb_conn dbconn, const char *dbfile, const char *dbname);
GEODBAPI int geodb_detach_db(geodb_conn dbconn, const char *dbname);

GEODBAPI int geodb_execute_stmt(geodb_conn dbconn, geodb_stmt stmt);
GEODBAPI int geodb_execute_sql(geodb_conn dbconn, const char *sql, int sqllen);

GEODBAPI int geodb_query_layers(geodb_conn dbconn, const char *dbname, const char *whereSQL, int *layerid);


// geodb_shape API
GEODBAPI int geodb_shape_new(geodb_conn dbconn, geodb_shape *shape);
GEODBAPI void geodb_shape_free(geodb_shape shape);


// shape_filter API
GEODBAPI int geodb_query_shapes(geodb_conn dbconn, geodb_layer layer, shape_filter filter);


#ifdef    __cplusplus
}
#endif
#endif /* GEODBAPI_H__ */