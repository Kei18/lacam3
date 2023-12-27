#!/bin/sh
SCRIPT_DIR=$(cd $(dirname $0); pwd)

for f in $SCRIPT_DIR/scen/*.zip
do
    unzip $f -d $SCRIPT_DIR/scen
done
julia --project=$SCRIPT_DIR -e 'using Pkg; Pkg.instantiate()'
