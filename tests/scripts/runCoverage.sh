#!/bin/sh
 
############################################################################################################
# You should edit the following 3 paths when necessary
############################################################################################################

LCOV=lcov
GENHTML=genhtml
#BROWSER="/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"

SRC_DIR=`pwd`
HTML_RESULTS="html"

mkdir -p ${HTML_RESULTS}
 
# generate our initial info
"${LCOV}" -c -d "${SRC_DIR}" -c -o "${SRC_DIR}/coverage.info"

# remove some paths
"${LCOV}" -r "${SRC_DIR}/coverage.info" "*Qt*.framework*" "*Xcode.app*" "*.moc" "*moc_*.cpp" "*/test/*" -o "${SRC_DIR}/coverage-filtered.info"
 
# generate our HTML
"${GENHTML}" -o "${HTML_RESULTS}" "${SRC_DIR}/coverage-filtered.info"
 
# reset our counts
"${LCOV}" -d "${SRC_DIR}" -z
 
# open in browser and bring to front
xdg-open "${HTML_RESULTS}/index.html"
