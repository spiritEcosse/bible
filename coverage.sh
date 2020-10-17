#!/bin/bash

mkdir -p build && \
cd build && \
cmake \
-DCMAKE_BUILD_TYPE:STRING=Debug \
-DCMAKE_CXX_COMPILER:STRING=/usr/bin/g++ \
-DCMAKE_C_COMPILER:STRING=/usr/bin/gcc \
-DCMAKE_PREFIX_PATH:STRING=/home/igor/Qt5.6.3/5.6.3/gcc_64 \
-DQT_QMAKE_EXECUTABLE:STRING=/home/igor/Qt5.6.3/5.6.3/gcc_64/bin/qmake \
-DCMAKE_BUILD_TYPE=Coverage \
-DBUILD_TESTING=false \
.. && \
make && \
make tests && \
gcovr -r ../src . --html-details --html -o index.html
xdg-open index.html
