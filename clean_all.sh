#!/bin/bash
#
# @file clean_all.sh
#   remove all mediate files
#
# @author mapaware@hotmail.com
# @copyright © 2024-2030 mapaware.top All Rights Reserved.
# @version 1.2.17
#
# @since 2024-10-29 12:11:33
# @date 2024-10-29 12:11:33
#
#######################################################################
_name_=$(basename "$0")
_cdir_=$(cd "$(dirname "$0")" && pwd)
_file_=""${_cdir_}"/"${_name_}""

# Treat unset variables as an error
set -o nounset

# Treat any error as exit
set -o errexit

#######################################################################

echo "$_name_: remove all intermediate files and dirs in: ${_cdir_}"

cd ${_cdir_} && rm -f *.stackdump *.o
cd ${_cdir_} && /usr/bin/find ./source -type f -name *.o | xargs -I {} sh -c "rm -f {}"
cd ${_cdir_} && /usr/bin/find ./projects -type d -name .vs | xargs -I {} sh -c "rm -rf {}"
cd ${_cdir_} && /usr/bin/find ./projects -type d -name build | xargs -I {} sh -c "rm -rf {}"
cd ${_cdir_} && /usr/bin/find ./projects -type d -name output | xargs -I {} sh -c "rm -rf {}"

echo "$_name_: done."