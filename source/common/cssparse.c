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
 * @file cssparse.c
 * @brief
 *
 * @author mapaware@hotmail.com
 * @copyright © 2024-2030 mapaware.top All Rights Reserved.
 * @version 0.0.1
 *
 * @since 2024-10-08 23:51:27
 * @date 2024-10-16 22:09:02
 *
 * @note
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "cssparse.h"
#include "smallregex.h"


#ifdef _DEBUG
#   define DEBUG_ASSERT(cond)  assert((cond));
#else
#   define DEBUG_ASSERT(cond)  ;
#endif


#define CssCheckNumKeys(keys)  do { \
        if (keys >= CSS_KEYINDEX_INVALID_4096) { \
            printf("Error: Keys is too many (%d).\n", keys); \
            abort(); \
        } \
    } while(0)


static const char* css_bitflag_array[] = {
    "readonly",
    "hidden",
    "hilight",
    "pickup",
    "dragging",
    "deleting",
    "fault",
    "flash",
    "zoomin",
    "zoomout",
    "panning",
    0
};


struct CssKeyField {
    unsigned int flags : 16;  // 16 个标记位
    unsigned int type : 8;    // 类型
    unsigned int length : 8;  // 名称长度
    unsigned int offset : 20;  // <= 0xFFFFF
    unsigned int keyidx : 12;  // 如果当前是 class, 这个值为定义 class 的 {key:value ...} 的索引位置
};


typedef struct CssKeyArrayData {
    struct CssStringBuffer *cssString;

    union {
        struct CssStringBuffer *__align_dummy;

        struct {
            int32_t  UsedKeys;
            int32_t  SizeKeys;
        };
    };

    struct CssKeyField keysArray[0];
} CssKeyArrayHead;


static CssKeyArrayHead * CssKeyArrayHeadData(CssKeyArray cssKeys)
{
    return (CssKeyArrayHead *) ((char*)cssKeys - sizeof(struct CssKeyArrayData));
}


static int CssStyleClassSplit(const char *str, int len, const char *outstrs[], int outstrslen[], int maxoutnum)
{
    int num;
    const char *start, *end;

    num = 0;
    start = str;
    end = start;
    while (num < maxoutnum && end - str < len) {
        if (*end == 32) {
            outstrs[num] = start;
            outstrslen[num] = (int)(end++ - start);
            start = end;
            num++;
        }
        else {
            end++;
        }
    }
    if (*start) {
        outstrs[num] = start;
        outstrslen[num] = (int)(end - start);
        num++;
    }
    return num;
}


static int cssKeyTypeIsClass(char keytype)
{
    return (keytype == css_type_class || keytype == css_type_id || keytype == css_type_asterisk) ? 1 : 0;
}

static int cssGetKeyBitFlag(const char* classflag, int flaglen)
{
    if (flaglen < 4 || flaglen > 10) {
        return css_bitflag_none;
    }

    for (int i = 0; css_bitflag_array[i] != 0; i++) {
        if (!strncmp(css_bitflag_array[i], classflag, flaglen)) {
            return (CssBitFlag)(1 << i);
        }
    }

    return css_bitflag_none;
}


static int cssParseClassFlags(const char* start, int len, int offsets[], int lengths[], int classflags[], int nsize)
{
    const char* p = start;
    const char* end = start + len;
    const char* str = 0;
    int n = 0;

    while (len-- > 0 && n < nsize) {
        if (*p == ' ' || *p == ',' || *p == '|' || *p == '\0') {
            if (str) {
                offsets[n] = (int)(str - start);
                lengths[n] = (int)(p - str);
                n++;
                str = 0;
            }
        }
        else if (!str) {
            str = p;
        }

        p++;
    }

    if (str) {
        offsets[n] = (int)(str - start);
        lengths[n] = (int)(p - str);
        n++;
    }

    /**
    * .a .b C D E  {...} -- both class a and b have all props C D E
    * .a, .b c d e {...} -- only class b has props C D E
    * .a c .b d e  {...} -- class a has all props, class b has D E
    * .a c, .b d e {...} -- class a has C, class b has D E
    */

    for (int i = 0; i < n; i++) {
        // 从开始取类名称
        int off0 = offsets[i];
        int len0 = lengths[i];

        // -1 表示 key 不是 class
        int classflag = -1;

        str = start + off0;
        CssKeyType keytype = str[0];

        if (cssKeyTypeIsClass(keytype)) {
            // 0 表示 key 是 class
            classflag = 0;

            // 如果是类, 查找后面逗号第一次出现的位置 stop
            const char* stop = str + len0;
            while (stop != end && *stop != ',') {
                stop++;
            }

            // 查找后面的 key
            for (int j = i + 1; j < n; j++) {
                int koff = offsets[j];
                int klen = lengths[j];

                if (koff < stop - start) {
                    // 逗号前面所有的 key
                    p = start + koff;

                    if (!cssKeyTypeIsClass(*p)) {
                        classflag |= cssGetKeyBitFlag(p, klen);
                    }
                }
            }
        }

        // 只需要取出 classflags[i] >= 0 的就是 class
        // >= 0: is bitflag of class
        //  < 0: is class its self
        classflags[i] = classflag;
    }

    return n;
}


static int setCssKeyField(const char* cssString, struct CssKeyField* keyField, CssKeyType keytype, char* begin, int length)
{
    int outkeys = 0;

    char* end = begin + length - 1;

    // 剔除头部无效字符
    while (length > 0) {
        if (*begin == ':' || *begin == ';' || *begin == ' ') {
            begin++;
            length--;
            continue;
        }
        break;
    }

    // 剔除尾部无效字符
    while (length > 0) {
        if (*end == ';' || *end == '}' || *end == ' ') {
            end--;
            length--;
            continue;
        }
        break;
    }

    if (cssKeyTypeIsClass(keytype)) {
        int offsets[CSS_VALUELEN_INVALID_256];
        int lengths[CSS_VALUELEN_INVALID_256];
        int keyflags[CSS_VALUELEN_INVALID_256];

        int numFlags = cssParseClassFlags(begin, length, offsets, lengths, keyflags, CSS_VALUELEN_INVALID_256);

        for (int k = 0; k < numFlags; k++) {
            const char* classkey = begin + offsets[k];
            int keyflag = keyflags[k];
            if (keyflag >= 0) {
                if (lengths[k] < CSS_VALUELEN_INVALID_256) {
                    if (keyField) {
                        keyField[outkeys].type = keytype;
                        keyField[outkeys].flags = keyflag;
                        keyField[outkeys].offset = (int)(classkey - cssString);
                        keyField[outkeys].length = lengths[k];
                    }
                    outkeys++;
                }
                else {
                    printf("Error: css key has too many chars: %.*s\n", lengths[k], classkey);
                    abort();
                }
            }
        }
    }
    else {
        if (length < CSS_VALUELEN_INVALID_256) {
            if (keyField) {
                keyField->offset = (unsigned int)(begin - cssString);
                keyField->length = (unsigned int)length;
                keyField->type = (unsigned int)keytype;
                keyField->flags = css_bitflag_none;
            }

            outkeys = 1;
        }
        else {
            printf("Error: css key has too many chars: %s\n", begin);
            abort();
        }
    }

    return outkeys;
}


// 检查并设置索引
// 成功返回 UsedKeys, 失败返回 0
static int CssKeyArrayBuild(const char* cssString, CssKeyArray cssKeys, int numKeys)
{
    struct CssKeyField* NotKey = cssKeys + numKeys;
    struct CssKeyField* start = cssKeys;
    struct CssKeyField* offkey = start;

    if (numKeys < 2 || !cssKeyTypeIsClass(offkey->type)) {
        return 0;
    }

    while (++offkey != NotKey) {
        if (!cssKeyTypeIsClass(offkey->type)) { // offkey meets {k:v}
            // {} key 的索引必须为 0
            offkey->keyidx = 0;

            if (cssKeyTypeIsClass(start->type)) {
                while (start != offkey) {
                    start++->keyidx = (unsigned int)(offkey - cssKeys);
                }
                DEBUG_ASSERT(start == offkey)
            }
        }
        else { // offkey meets class
            if (!cssKeyTypeIsClass(start->type)) {
                start = offkey;
            }
        }
    }

    CssKeyArrayHeadData(cssKeys)->UsedKeys = numKeys;

    return numKeys;
}


static CssKeyArray CssCreateKeysArray(int num, CssString cssString)
{
    if (num >= CSS_KEYINDEX_INVALID_4096) {
        printf("Error: too many keys(=%d)\n", num);
        return 0;
    }

    size_t bsize = sizeof(CssKeyArrayHead) + num * sizeof(struct CssKeyField);
    CssKeyArrayHead * data = (CssKeyArrayHead *) malloc(bsize);
    if (! data) {
        printf("Error: Out of memory\n");
        abort();
    }
    memset(data, 0, bsize);

    data->cssString = cssString;
    data->SizeKeys = (int32_t)num;
    data->UsedKeys = 0;

    return data->keysArray;
}


static int cssParseKeys(CssString cssString, CssKeyArray outKeys)
{
    int p, q, len;
    char tmpChar, * markStr;

    char* css, * begin, * end, * start, * next;
    int keys = 0;

    const int SizeKeys = CssKeyArrayGetSize(outKeys);

    int cssLen = cssString->sblen;

    css = cssString->sbbuf;
    while (*css) {
        tmpChar = *css;
        if (tmpChar == 9 || tmpChar == 13 || tmpChar == 34 || tmpChar == 39) {
            // 用空格替代字符: [\t \r " ']
            *css = 32;  // space
        }
        else if (tmpChar == 10) {
            // 用 ; 替代换行符: [\n]
            *css = 59;
        }
        css++;
    }

    // 用空格替换注释: "/* ... */"
    struct small_regex* recomment = regex_compile("/\\*.*?\\*/");
    css = cssString->sbbuf;
    while (*css) {
        p = regex_matchp(recomment, css);
        if (p < 0) {
            break;
        }
        start = css + p;
        next = strstr(start, "*/") + 2;
        len = (unsigned int)(next - start);

        while (len-- > 0) {
            *start++ = 32;
        }
        css = next;
    }
    regex_free(recomment);

    // 查找每个属性集: "{ key: value; ... }"
    struct small_regex* reclass = regex_compile("{.*?}");
    struct small_regex* rekey = regex_compile(":.*?;");

    css = cssString->sbbuf;
    while (*css) {
        p = regex_matchp(reclass, css);
        if (p < 0) {
            break;
        }

        start = css + p;
        DEBUG_ASSERT(*start == '{')

            next = strchr(start, '}') + 1;
        len = (unsigned int)(next - start);

        // 获取选择器名
        CssKeyType keytype = css_type_none;
        begin = css;
        while (begin < start) {
            // 选择器名只处理 3 种情况:
            if (cssKeyTypeIsClass(*begin)) {
                keytype = (CssKeyType)*begin;
                break;
            }
            begin++;
            p--;
        }

        if (p > 0) {
            // 如果发现选择器
            keys += setCssKeyField(cssString->sbbuf, ((outKeys && keys < SizeKeys) ? &outKeys[keys] : 0), keytype, begin, p);
            CssCheckNumKeys(keys);

            markStr = start + len - 1;
            DEBUG_ASSERT(*markStr == '}');
            *markStr = ';';
            tmpChar = *next; *next = '\0';

            start++;
            while (start < next) {
                // 查找属性: key : value ;
                q = regex_matchp(rekey, start);
                if (q < 0) {
                    break;
                }

                begin = start + q;
                end = strchr(begin, ';') + 1;
                len = (unsigned int)(end - begin);

                DEBUG_ASSERT(*begin == ':');
                DEBUG_ASSERT(begin[len - 1] == ';');

                // set key
                keys += setCssKeyField(cssString->sbbuf, ((outKeys && keys < SizeKeys) ? &outKeys[keys] : 0), css_type_key, start, q);
                CssCheckNumKeys(keys);

                // set value
                keys += setCssKeyField(cssString->sbbuf, ((outKeys && keys < SizeKeys) ? &outKeys[keys] : 0), css_type_value, begin, len);
                CssCheckNumKeys(keys);

                start = end;
            }

            DEBUG_ASSERT(*markStr == ';')
                * markStr = '}';

            DEBUG_ASSERT(*next == '\0')
                * next = tmpChar;
        }

        // go to next braces: {}
        css = next;
    }
    regex_free(rekey);
    regex_free(reclass);

    // 用户必须判断返回的 keys > 0
    if (keys > SizeKeys) {
        // 负值表示输入的空间不够, 其绝对值为需要的空间大小
        return -keys;
    }

    if (!CssKeyArrayBuild(cssString->sbbuf, outKeys, keys)) {
        // 失败返回 0
        printf("Error: CssKeyArrayBuild() failed.\n");
        return 0;
    }

    // 成功返回保存 Keys 数目
    return CssKeyArrayGetUsed(outKeys);
}


CssString CssStringNew(const char* cssStr, size_t cssStrLen)
{
    size_t cbSize = (cssStrLen + 16) / 16 * 16;
    if (cbSize > CSS_STRING_BSIZE_MAX_1048576) {
        printf("Error: size is too long\n");
        return 0;
    }

    struct CssStringBuffer *sbuf = (struct CssStringBuffer *) malloc(sizeof(struct CssStringBuffer) + sizeof(char) * cbSize);
    if (!sbuf) {
        printf("Error: Out of memory.\n");
        abort();
    }

    if (cssStr) {
        memcpy(sbuf->sbbuf, cssStr, cssStrLen);
    }
    else {
        memset(sbuf->sbbuf, 0, cssStrLen);
    }

    sbuf->sbbuf[cssStrLen] = '\0';
    sbuf->sbsize = (uint32_t) cbSize;
    sbuf->sblen = (uint32_t) cssStrLen;

    return sbuf;
}


CssString CssStringNewFromFile(FILE* cssfile)
{
    // 必须一次读全部输入文件内容
    rewind(cssfile);
    if (fseek(cssfile, 0, SEEK_END) == 0) {
        int bsize = (int)ftell(cssfile);
        if (bsize >= CSS_STRING_BSIZE_MAX_1048576) {
            printf("Error: css file is too big.\n");
            return 0;
        }

        rewind(cssfile);
        CssString css = CssStringNew(0, bsize);
        if (fread(css->sbbuf, sizeof(char), bsize, cssfile) == bsize) {
            css->sbbuf[bsize] = 0;
            css->sblen = bsize;
            return css;
        }
        CssStringFree(css);
    }
    return 0;
}


void CssStringFree(CssString cssString)
{
    if (cssString) {
        free(cssString);
    }
}


void CssKeyArrayFree(CssKeyArray cssKeys)
{
    if (cssKeys) {
        CssKeyArrayHead *data = CssKeyArrayHeadData(cssKeys);
        CssStringFree(data->cssString);
        free(data);
    }
}


const char* CssKeyArrayGetString(const CssKeyArray cssKeys, unsigned int offset)
{
    CssKeyArrayHead* data = CssKeyArrayHeadData(cssKeys);
    return (offset < data->cssString->sblen ? &data->cssString->sbbuf[offset] : 0);
}


int CssKeyArrayGetSize(const CssKeyArray cssKeys)
{
    if (cssKeys) {
        CssKeyArrayHead* data = CssKeyArrayHeadData(cssKeys);
        return (int) data->SizeKeys;
    }
    return 0;
}


int CssKeyArrayGetUsed(const CssKeyArray cssKeys)
{
    if (cssKeys) {
        CssKeyArrayHead* data = CssKeyArrayHeadData(cssKeys);
        return (int)data->UsedKeys;
    }
    return 0;
}


CssKeyArray CssStringParse(CssString cssString)
{
    int numKeys = cssParseKeys(cssString, 0);
    if (numKeys < 0) {
        numKeys *= -1;
        CssKeyArray keysArray = CssCreateKeysArray(numKeys, cssString);
        if (keysArray) {
            if (cssParseKeys(cssString, keysArray) == numKeys) {
                return keysArray;
            }
            CssKeyArrayHead* data = CssKeyArrayHeadData(keysArray);
            data->cssString = 0;
            CssKeyArrayFree(keysArray);
        }
    }
    return 0;
}


const CssKeyArrayNode CssKeyArrayGetNode(const CssKeyArray cssKeys, int index)
{
    int numKeys = CssKeyArrayGetUsed(cssKeys);
    if (index < numKeys) {
        return (CssKeyArrayNode)(cssKeys + index);
    }
    return 0;
}


CssKeyType CssKeyGetType(const CssKeyArrayNode cssKey)
{
    return cssKey->type;
}


int CssKeyGetFlag(const CssKeyArrayNode cssKey)
{
    return (int)(cssKey->flags);
}


int CssKeyOffsetLength(const CssKeyArrayNode cssKeyNode, int* bOffset)
{
    *bOffset = (int)cssKeyNode->offset;
    return (int)cssKeyNode->length;
}


int CssKeyTypeIsClass(const CssKeyArrayNode cssKeyNode)
{
    return cssKeyTypeIsClass(cssKeyNode->type);
}


int CssClassGetKeyIndex(const CssKeyArrayNode cssClassKey)
{
    if (!cssClassKey->keyidx) {
        return -1;
    }
    else {
        return cssClassKey->keyidx;
    }
}


int CssKeyFlagToString(int keyflag, char* outbuf, size_t bufsize)
{
    if (keyflag > 0) {
        if (!outbuf) {
            int outlen = 0;
            for (int i = 0; css_bitflag_array[i] != 0; i++) {
                int bitmask = (1 << i);
                if (keyflag & bitmask) {
                    outlen += (int)strlen(css_bitflag_array[i]) + 1;
                }
            }
            return outlen + 1;
        }
        else {
            char* bbuf = outbuf;
            for (int i = 0; css_bitflag_array[i] != 0; i++) {
                int bitmask = (1 << i);
                if (keyflag & bitmask) {
                    size_t bbufsz = bufsize + outbuf - bbuf;
                    int len = snprintf(bbuf, bbufsz, "%s ", css_bitflag_array[i]);
                    if (len <= 0) {
                        printf("Error: out of memory.\n");
                        abort();
                    }
                    bbuf += len;
                }
            }
            *bbuf = 0;
            return (int)(bbuf - outbuf);
        }
    }

    *outbuf = 0;
    return 0;
}


void CssKeyArrayPrint(const CssKeyArray cssKeys, FILE* outfd)
{
    // 每个 class 最多有 16 个状态, 每个状态名称10 个字符长度, 因此 CSS_KEYINDEX_INVALID_4096 足够
    char classKeyFlags[CSS_KEYINDEX_INVALID_4096];

    const int numKeys = CssKeyArrayGetUsed(cssKeys);

    int nk = 0;

    while (nk < numKeys - 1) {
        CssKeyArrayNode classKeyNode = CssKeyArrayGetNode(cssKeys, nk++);

        if (CssKeyTypeIsClass(classKeyNode)) {
            int bflagsLen = CssKeyFlagToString(CssKeyGetFlag(classKeyNode), classKeyFlags, sizeof(classKeyFlags) / sizeof(classKeyFlags[0]));

            int keyIndex = CssClassGetKeyIndex(classKeyNode);

            int offset = 0;
            int length = CssKeyOffsetLength(classKeyNode, &offset);

            fprintf(outfd, "%.*s %.*s{\n", length, CssKeyArrayGetString(cssKeys, offset), bflagsLen, classKeyFlags);

            while (keyIndex < numKeys) {
                CssKeyArrayNode keyNode = CssKeyArrayGetNode(cssKeys, keyIndex++);
                if (CssKeyTypeIsClass(keyNode)) {
                    // 遇到 class 就转向下一个 class
                    break;
                }

                // 遇到 key, 同时取后面的 value
                CssKeyArrayNode valNode = CssKeyArrayGetNode(cssKeys, keyIndex++);

                DEBUG_ASSERT(CssKeyGetType(keyNode) == css_type_key)
                DEBUG_ASSERT(CssKeyGetType(valNode) == css_type_value)

                int keyoffs, valoffs;
                int keylen = CssKeyOffsetLength(keyNode, &keyoffs);
                int vallen = CssKeyOffsetLength(valNode, &valoffs);

                fprintf(outfd, "  %.*s: %.*s;\n", keylen, CssKeyArrayGetString(cssKeys, keyoffs), vallen, CssKeyArrayGetString(cssKeys, valoffs));
            }

            fprintf(outfd, "}\n");
        }
    }
}


int CssKeyArrayQueryClass(const CssKeyArray cssKeys, CssKeyType classType, const char *className, int classNameLen, CssKeyArrayNode classNodes[32])
{
    const char *styleNames[32];
    int styleLens[32];
    int numstyles = CssStyleClassSplit(className, classNameLen, styleNames, styleLens, sizeof(styleLens) / sizeof(styleLens[0]));

    int retNodes = 0;
    const int NumKeys = CssKeyArrayGetUsed(cssKeys);

    for (int n = 0; n < numstyles; n++) {
        // 遍历每个 class
        for (int i = 0; i < NumKeys; i++) {
            const CssKeyArrayNode keyNode = CssKeyArrayGetNode(cssKeys, i);
            if (CssKeyGetType(keyNode) == classType) {
                int boffset = 0;
                if (CssKeyOffsetLength(keyNode, &boffset) == styleLens[n]) {
                    const char* keyName = CssKeyArrayGetString(cssKeys, boffset);
                    if (!strncmp(keyName, styleNames[n], styleLens[n])) {
                        if (retNodes < 32) {
                            classNodes[retNodes++] = keyNode;
                        }
                    }
                }
            }
        }
    }
    return retNodes;
}