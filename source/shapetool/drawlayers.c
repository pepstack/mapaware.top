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
 * @version 0.0.5
 *
 * @since 2024-10-16 22:31:46
 * @date 2024-11-01 01:16:58
 *
 * @note
 *   https://github.com/pepstack/shapefile
 *
 */
#include "drawlayers.h"


int maplayers2png(shapetool_flags *flags, shapetool_options *options)
{
    struct MapLayersCfg maplayers = { 0 };

    MapLayersCfgInit(&maplayers);

    const char* CfgFile = CBSTR(options->layerscfg);

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
        int buflen;

        char layerbuf[SHAPETOOL_PATHLEN_INVALID];
        int layerbuflen;

        for (int i = 0; i < secs; ++i) {
            char* sec, * family, * qualifier;

            sec = ConfSectionListGetAt(sections, i);
            if (ConfSectionParse(sec, &family, &qualifier) == 2) {
                if (!cstr_compare_len(family, -1, "map", 3, 0) && !cstr_compare_len(qualifier, -1, options->mapid->str, options->mapid->len, 0)) {
                    printf("[%s:%s]\n", family, qualifier);

                    maplayers.mapid = cstrbufDup(maplayers.mapid, options->mapid->str, options->mapid->len);

                    buflen = ConfReadValueParsed(CfgFile, "map", options->mapid->str, "description", buffer, sizeof(buffer));
                    if (buflen) {
                        maplayers.description = cstrbufDup(maplayers.description, buffer, buflen);
                    }

                    buflen = ConfReadValueParsed(CfgFile, "map", options->mapid->str, "proj4def", buffer, sizeof(buffer));
                    if (buflen) {
                        maplayers.proj4def = cstrbufDup(maplayers.proj4def, buffer, buflen);
                    }

                    char* layerids[SHAPETOOL_LAYERS_MAX];
                    int idlens[SHAPETOOL_LAYERS_MAX];
                    int layers = 0;

                    buflen = ConfReadValueParsed(CfgFile, "map", options->mapid->str, "layers", buffer, sizeof(buffer));
                    if (buflen) {
                        layers = cstr_slpit_chr_nodup(buffer, buflen, 32, layerids, idlens, sizeof(idlens) / sizeof(idlens[0]));

                        for (int k = 0; k < layers; k++) {
                            char* output = 0;
                            int outlen;

                            struct MapLayerData layerdata;
                            MapLayerDataInit(&layerdata);

                            layerdata.layerid = cstrbufDup(0, layerids[k], idlens[k]);

                             // shpfile
                            layerbuflen = ConfReadValueParsed2(CfgFile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "shpfile", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    layerdata.shpfile = cstrbufDup(layerdata.shpfile, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            // stylefile
                            layerbuflen = ConfReadValueParsed2(CfgFile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "stylefile", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    layerdata.stylefile = cstrbufDup(layerdata.stylefile, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            // styleclass
                            layerbuflen = ConfReadValueParsed2(CfgFile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "styleclass", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    layerdata.styleclass = cstrbufDup(layerdata.styleclass, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            // groups
                            layerbuflen = ConfReadValueParsed2(CfgFile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "groups", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    // layerdata.groups = cstrbufDup(layerdata.groups, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            // states
                            layerbuflen = ConfReadValueParsed2(CfgFile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "states", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    // layerdata.states = cstrbufDup(layerdata.states, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            MapLayersCfgAdd(&maplayers, &layerdata);
                        }
                    }
                }
            }
        }
    }

    ConfSectionListFree(sections);
    ConfVariablesClear(&vars);

    MapLayersCfgPrint(&maplayers);

    MapLayersCfgUninit(&maplayers);

    return SHAPETOOL_RES_ERR;
}