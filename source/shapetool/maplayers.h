/******************************************************************************
* Copyright © 2024-2035 Light Zhang <mapaware@hotmail.com>, MapAware, Inc.
* ALL RIGHTS RESERVED.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/
/**
 * @file maplayers.h
 * @brief
 *
 * @author mapaware@hotmail.com
 * @copyright © 2024-2030 mapaware.top All Rights Reserved.
 * @version 0.0.6
 *
 * @since 2024-10-16 21:58:17
 * @date 2024-10-28 12:49:38
 *
 * @note
 */
#ifndef MAP_LAYERS_H__
#define MAP_LAYERS_H__

#ifdef    __cplusplus
extern "C" {
#endif

#include <common/cstrbuf.h>
#include <common/readconf.h>
#include <common/uthash/uthash.h>
#include <common/uthash/utarray.h>

struct MapLayerData {
    cstrbuf layerid;   /* Use this field as the key */

    cstrbuf shpfile;
    cstrbuf stylefile;
    cstrbuf styleclass;

    UT_hash_handle hh; /* makes this structure hashable */
};


struct MapLayersCfg {
    cstrbuf mapid;
    cstrbuf description;
    cstrbuf proj4def;

    UT_array * layers_array;
    struct MapLayerData * layers_hash;
};


void MapLayersCfgInit(struct MapLayersCfg *layersCfg);


#ifdef  __cplusplus
}
#endif
#endif /* MAP_LAYERS_H__ */