#/bin/bash

sed -i 's!QVariant target!QVariant\& target!g' ../src/DownloadManager.cpp
sed -i 's!QUrl redirectUrl!QUrl\& redirectUrl!g' ../src/DownloadManager.cpp
