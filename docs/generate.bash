#!/bin/bash
set -ex

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$DIR"

doxygen doxygen.cfg

source=$DIR/xml
dest=$DIR/astromd

pushd ~/Dev/doxygentomd
poetry run python -m doxygentomd $source $dest --style astro --baseurl=/game-host-adapter/
