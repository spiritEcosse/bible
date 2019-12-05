#!/bin/bash

qmake ../tests.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug LIBS+=-lquazip5 && \
make qmake_all && \
make -j8 && \
./tests && \
lcov -c -d . -o coverage.info && \
lcov -r coverage.info "*Qt*.framework*" "*.moc" "*moc_*.cpp" "*/test/*" '/usr/include/*' '/usr/local/*' '**/tests*' -o coverage-filtered.info && \
genhtml coverage-filtered.info -o ../../${HTML} && \
lcov -d . -z
