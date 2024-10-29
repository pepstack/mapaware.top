#!/bin/bash

workspaceFolder=$(cd "$(dirname "$0")" && pwd)
echo "$workspaceFolder"

cd ${workspaceFolder} && make BUILD=DEBUG

cd ${workspaceFolder}
