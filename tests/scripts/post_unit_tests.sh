#!/bin/bash

sed -i '164s!QVariant\&!QVariant!' ../../src/DownloadManager.cpp
sed -i '167s!QUrl\&!QUrl!' ../../src/DownloadManager.cpp
sed -i '40s!QString\&!QString!' ../../src/DownloadManager.cpp