#!/bin/sh
 
############################################################################################################
# You should edit the following 3 paths when necessary
############################################################################################################

# SRC_DIR is the directory containing the .gcno files (%{buildDir} in Qt Creator)
SRC_DIR=`pwd`

# COV_DIR is the directory where the coverage results will be stored
COV_DIR="$SRC_DIR/coverage"

############################################################################################################

# Path where the HTML files should be saved
HTML_RESULTS="${COV_DIR}""/html"

# Create the html folder if it does not exists
mkdir -p ${HTML_RESULTS}

# Generate our initial info
lcov -d "${SRC_DIR}" -c -o "${COV_DIR}/coverage.info"
 
# Remove some paths/files which we don't want to calculate the code coverage (e.g. third party libraries) and generate a new coverage file filtered (feel free to edit it when necessary)
lcov -r "${COV_DIR}/coverage.info" "*Qt*.framework*" "*.h" "*/tests/*" "*.moc" "*moc_*.cpp" "*/test/*" "*/build*/*" -o "${COV_DIR}/coverage-filtered.info"
 
# Generate the HTML files
genhtml -o "${HTML_RESULTS}" "${COV_DIR}/coverage-filtered.info"
 
# Reset our counts
lcov -d "${COV_DIR}" -z
 
# Open the index.html
xdg-open "${HTML_RESULTS}/index.html"
