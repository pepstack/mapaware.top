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
 * @file maplayers.c
 * @brief
 *
 * @author mapaware@hotmail.com
 * @copyright © 2024-2030 mapaware.top All Rights Reserved.
 * @version 0.0.6
 *
 * @since 2024-10-16 21:58:17
 * @date 2024-11-01 01:24:43
 *
 * @note
 */
#include "maplayers.h"


void MapLayersCfgInit(struct MapLayersCfg* layersCfg)
{
    utarray_new(layersCfg->layers_array, &maplayerdata_icd);
}

void MapLayersCfgUninit(struct MapLayersCfg* layersCfg)
{
    cstrbufFree(&layersCfg->mapid);
    cstrbufFree(&layersCfg->description);
    cstrbufFree(&layersCfg->proj4def);

    utarray_free(layersCfg->layers_array);
}

void MapLayersCfgAdd(struct MapLayersCfg* layersCfg, const struct MapLayerData* layer)
{
    utarray_push_back(layersCfg->layers_array, layer);
}

void MapLayersCfgPrint(const struct MapLayersCfg* layersCfg)
{
    struct MapLayerData* layer;

    printf("[map:%.*s]\n", CBSTRLEN(layersCfg->mapid), CBSTR(layersCfg->mapid));

    if (layersCfg->description) {
        printf("description = %.*s\n", CBSTRLEN(layersCfg->description), CBSTR(layersCfg->description));
    }
    if (layersCfg->proj4def) {
        printf("proj4def = %.*s\n", CBSTRLEN(layersCfg->proj4def), CBSTR(layersCfg->proj4def));
    }

    printf("layers = ");
    layer = NULL;
    while ((layer = (struct MapLayerData *) utarray_next(layersCfg->layers_array, layer))) {
        printf("\\\n");
        printf("  + %.*s ", CBSTRLEN(layer->layerid), CBSTR(layer->layerid));
    }
    printf("\n");

    layer = NULL;
    while ((layer = (struct MapLayerData*)utarray_next(layersCfg->layers_array, layer))) {
        printf("\n[layer:%.*s]\n", CBSTRLEN(layer->layerid), CBSTR(layer->layerid));
        if (layer->shpfile) {
            printf("shpfile = %.*s\n", CBSTRLEN(layer->shpfile), CBSTR(layer->shpfile));
        }
        if (layer->stylefile) {
            printf("stylefile = %.*s\n", CBSTRLEN(layer->stylefile), CBSTR(layer->stylefile));
        }
        if (layer->styleclass) {
            printf("styleclass = %.*s\n", CBSTRLEN(layer->styleclass), CBSTR(layer->styleclass));
        }

        /* TODO:
        if (layer->groups) {
            printf("groups = %.*s\n", CBSTRLEN(layer->groups), CBSTR(layer->groups));
        }
        if (layer->states) {
            printf("states = %.*s\n", CBSTRLEN(layer->states), CBSTR(layer->states));
        }
        */
    }
}