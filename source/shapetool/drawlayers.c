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
 * @version 0.0.6
 *
 * @since 2024-10-16 22:31:46
 * @date 2024-11-02 01:19:37
 *
 * @note
 *   https://github.com/pepstack/shapefile
 *
 */
#include "drawlayers.h"

#include <geodbapi/geodbapi.h>


static int load_maplayers_cfg(const char * cfgfile, shapetool_options* options, struct MapLayersCfg * maplayers)
{
    // 读环境变量
    ConfVariables vars = { 0 };
    int numvars = ConfReadSectionVariables(cfgfile, "environments", &vars);
    if (numvars > 0) {
        for (int i = 0; i < numvars; i++) {
            printf("<%.*s> : {%.*s}\n", vars.keylens[i], vars.keys[i], vars.valuelens[i], vars.values[i]);
        }
    }

    // 读 [map:MAPID]
    void* sections = 0;
    int secs = ConfGetSectionList(cfgfile, &sections);
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

                    maplayers->mapid = cstrbufDup(maplayers->mapid, options->mapid->str, options->mapid->len);

                    buflen = ConfReadValueParsed(cfgfile, "map", options->mapid->str, "description", buffer, sizeof(buffer));
                    if (buflen) {
                        maplayers->description = cstrbufDup(maplayers->description, buffer, buflen);
                    }

                    buflen = ConfReadValueParsed(cfgfile, "map", options->mapid->str, "proj4def", buffer, sizeof(buffer));
                    if (buflen) {
                        maplayers->proj4def = cstrbufDup(maplayers->proj4def, buffer, buflen);
                    }

                    char* layerids[SHAPETOOL_LAYERS_MAX];
                    int idlens[SHAPETOOL_LAYERS_MAX];
                    int layers = 0;

                    buflen = ConfReadValueParsed(cfgfile, "map", options->mapid->str, "layers", buffer, sizeof(buffer));
                    if (buflen) {
                        layers = cstr_slpit_chr_nodup(buffer, buflen, 32, layerids, idlens, sizeof(idlens) / sizeof(idlens[0]));

                        for (int k = 0; k < layers; k++) {
                            char* output = 0;
                            int outlen;

                            struct MapLayerData layerdata;
                            MapLayerDataInit(&layerdata);

                            layerdata.layerid = cstrbufDup(0, layerids[k], idlens[k]);

                            // shpfile
                            layerbuflen = ConfReadValueParsed2(cfgfile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "shpfile", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    layerdata.shpfile = cstrbufDup(layerdata.shpfile, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            // stylefile
                            layerbuflen = ConfReadValueParsed2(cfgfile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "stylefile", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    layerdata.stylefile = cstrbufDup(layerdata.stylefile, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            // styleclass
                            layerbuflen = ConfReadValueParsed2(cfgfile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "styleclass", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    layerdata.styleclass = cstrbufDup(layerdata.styleclass, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            // groups
                            layerbuflen = ConfReadValueParsed2(cfgfile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "groups", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    // layerdata.groups = cstrbufDup(layerdata.groups, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            // states
                            layerbuflen = ConfReadValueParsed2(cfgfile, "layer", CBSTR(layerdata.layerid), CBSTRLEN(layerdata.layerid), "states", layerbuf, sizeof(layerbuf));
                            if (layerbuflen > 0) {
                                outlen = ConfVariablesReplace(layerbuf, layerbuflen, &vars, &output);
                                if (outlen) {
                                    // layerdata.states = cstrbufDup(layerdata.states, output, outlen);
                                    ConfMemFree(output);
                                }
                            }

                            MapLayersCfgAddLayer(maplayers, &layerdata);
                        }
                    }
                }
            }
        }
    }

    ConfSectionListFree(sections);
    ConfVariablesClear(&vars);

    return MapLayersCfgGetLayers(maplayers);
}


static int load_maplayers_json(const char * jsonfile, shapetool_options* options, struct MapLayersCfg* maplayers)
{
    filehandle_t fh = file_open_read(jsonfile);
    if (fh != filehandle_invalid) {
        sb8 cb = file_size(fh);
        if (cb > 0 && cb < 8192*1024) {
            file_seek(fh, 0, fseek_pos_set);
            cstrbuf jsonbuf = cstrbufNew((ub4)(cb + 1), 0, 0);
            jsonbuf->len = file_readbytes(fh, jsonbuf->str, jsonbuf->maxsz);
            if (jsonbuf->len < jsonbuf->maxsz) {
                cJSON * json = cJSON_ParseWithLength(jsonbuf->str, jsonbuf->len);

                if (!json) {
                    const char* errstr = cJSON_GetErrorPtr();
                    if (errstr) {
                        fprintf(stderr, "Error before: %s\n", errstr);
                    }
                    cstrbufFree(&jsonbuf);
                    file_close(&fh);
                    return 0;
                }

                char * jsonstring = cJSON_Print(json);
                printf("%s\n", jsonstring);
                cJSON_free(jsonstring);

                // parse json to maplayers
                cJSON * envs = cJSON_GetObjectItemCaseSensitive(json, "environments");
                if (envs) {
                    cJSON* var = 0;
                    cJSON_ArrayForEach(var, envs)
                    {
                        printf("%s: %s\n", var->string, cJSON_GetStringValue(var));
                    }
                }

                // map
                cJSON * maps = cJSON_GetObjectItemCaseSensitive(json, "map");
                if (maps) {
                    cJSON* mapItem = cJSON_GetObjectItemCaseSensitive(maps, CBSTR(options->mapid));
                    if (mapItem) {
                        cJSON *mDescription = cJSON_GetObjectItemCaseSensitive(mapItem, "description");
                        cJSON *mProj4def = cJSON_GetObjectItemCaseSensitive(mapItem, "proj4def");
                        cJSON *mLayers = cJSON_GetObjectItemCaseSensitive(mapItem, "layers");
                        if (mLayers) {
                            cJSON * layerid = 0;
                            cJSON_ArrayForEach(layerid, mLayers)
                            {
                                printf("%s\n", layerid->valuestring);
                                cJSON * layerRoot = cJSON_GetObjectItemCaseSensitive(json, "layer");
                                if (layerRoot) {
                                    cJSON* layerElt = cJSON_GetObjectItemCaseSensitive(layerRoot, cJSON_GetStringValue(layerid));
                                    if (layerElt) {
                                        cJSON* shpfile = cJSON_GetObjectItemCaseSensitive(layerElt, "shpfile");
                                        cJSON* groups = cJSON_GetObjectItemCaseSensitive(layerElt, "groups");
                                        cJSON* states = cJSON_GetObjectItemCaseSensitive(layerElt, "states");

                                        if (shpfile) {
                                            printf("%s: %s\n", shpfile->string, cJSON_GetStringValue(shpfile));
                                        }
                                        if (groups) {

                                        }
                                        if (states) {
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                cJSON_Delete(json);
            }
            cstrbufFree(&jsonbuf);
        }
        file_close(&fh);
    }

    return 0;
}


int maplayers2png(shapetool_flags *flags, shapetool_options *options)
{
    struct MapLayersCfg maplayers;
    MapLayersCfgInit(&maplayers);

    int layers = 0;

    if (cstr_endwith(CBSTR(options->maplayers), CBSTRLEN(options->maplayers), ".cfg", 4)) {
        layers = load_maplayers_cfg(CBSTR(options->maplayers), options, &maplayers);
    }
    else {
        layers = load_maplayers_json(CBSTR(options->maplayers), options, &maplayers);
    }

    if (layers > 0) {
        MapLayersCfgPrint(&maplayers);
    }

    MapLayersCfgUninit(&maplayers);

    ////////////////////////
    geodb_context_open("C:/TEMP/test.geodb");

    ///////////////////////
    return SHAPETOOL_RES_ERR;
}