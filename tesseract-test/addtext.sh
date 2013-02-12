#!/usr/bin/env bash
set -x
org="$1"
file="${1%.*}.tiff"
convert "$org" org.tiff
tesseract org.tiff org.txt -l eng -psm $2 hocr
./cut.cpp "$org" org.txt.html 
