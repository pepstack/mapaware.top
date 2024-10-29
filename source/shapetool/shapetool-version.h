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
 * @file shapetool-version.h
 * @brief
 *
 * @author mapaware@hotmail.com
 * @copyright © 2024-2030 mapaware.top All Rights Reserved.
 * @version 0.0.8
 *
 * @since 2024-10-03 00:05:20
 * @date 2024-10-16 22:06:14
 *
 * @note
 */
#ifndef SHAPETOOL_VERSION_H__
#define SHAPETOOL_VERSION_H__

#ifndef SHAPETOOL_COMMON_H__
#  error shapetool-version.h should only be included by shapetool-common.h
#endif

#define SHAPETOOL_VERSION_MAJOR 0
#define SHAPETOOL_VERSION_MINOR 0
#define SHAPETOOL_VERSION_MICRO 1

#define SHAPETOOL_VERSION_STRING "0.0.1"

#define SHAPETOOL_VERSION_ENCODE(major, minor, micro) ( \
      ((major) * 10000)				\
    + ((minor) *   100)				\
    + ((micro) *     1))

#define SHAPETOOL_VERSION SHAPETOOL_VERSION_ENCODE( \
    SHAPETOOL_VERSION_MAJOR, \
    SHAPETOOL_VERSION_MINOR, \
    SHAPETOOL_VERSION_MICRO)

#endif /* SHAPETOOL_VERSION_H__ */