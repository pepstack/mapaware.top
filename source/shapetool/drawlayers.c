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
 * @file drawlayers.c
 * @brief draw map layers by config file.
 *
 * @author mapaware@hotmail.com
 * @copyright © 2024-2030 mapaware.top All Rights Reserved.
 * @version 0.0.4
 *
 * @since 2024-10-16 22:31:46
 * @date 2024-10-27 05:13:44
 *
 * @note
 *   https://github.com/pepstack/shapefile
 *
 */
#include "drawlayers.h"



int maplayers2png(shapetool_flags *flags, shapetool_options *options)
{
    const char* CfgFile = CSTR_FILE_URI_PATH(options->layerscfg);

    // 读环境变量
    ConfVariables vars = { 0 };
    int numvars = ConfReadSectionVariables(CfgFile, "environments", &vars);
    if (numvars > 0) {
        for (int i = 0; i < numvars; i++) {
            printf("<%.*s> : {%.*s}\n", vars.keylens[i], vars.keys[i], vars.valuelens[i], vars.values[i]);
        }
    }

    // 读 [map:MAPID]
    void* sections = 0;
    int secs = ConfGetSectionList(CfgFile, &sections);
    if (secs > 0) {
        char buffer[READCONF_MAX_LINESIZE];

        for (int i = 0; i < secs; ++i) {
            char* sec, * family, * qualifier;

            sec = ConfSectionListGetAt(sections, i);
            if (ConfSectionParse(sec, &family, &qualifier) == 2) {
                if (!cstr_compare_len(family, -1, "map", 3, 0) && !cstr_compare_len(qualifier, -1, options->mapid->str, options->mapid->len, 0)) {
                    printf("[%s:%s]\n", family, qualifier);

                    int buflen = ConfReadValueParsed(CfgFile, "map", options->mapid->str, "layers", buffer, sizeof(buffer));

                    printf("layers={%.*s}\n", buflen, buffer);

                    char *layerid[SHAPETOOL_LAYERS_MAX];
                    int idlens[SHAPETOOL_LAYERS_MAX];
                    int layers = cstr_slpit_chr(buffer, buflen, 32, layerid, idlens, sizeof(idlens)/sizeof(idlens[0]));

                    for (int j = 0; j < layers; j++) {
                        printf("[layer:%.*s]\n", idlens[j], layerid[j]);

                        int valuelen = ConfReadValueParsed2(CfgFile, "layer", layerid[j], idlens[j], "shpfile", buffer, sizeof(buffer));
                        if (valuelen > 0) {
                            char* valuebuf = 0;
                            valuelen = ConfVariablesReplace(buffer, valuelen, &vars, &valuebuf);

                            printf("shpfile=%.*s\n", valuelen, valuebuf);
                            ConfMemFree(valuebuf);
                        }

                        mem_free(layerid[j]);
                    }
                }
            }
        }
    }

    ConfSectionListFree(sections);
    ConfVariablesClear(&vars);
    return SHAPETOOL_RES_ERR;
}