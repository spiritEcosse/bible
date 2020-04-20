#/bin/bash

sed -i '164s!QVariant !QVariant\& !' ../src/DownloadManager.cpp
sed -i '159s!QUrl !QUrl\& !' ../src/DownloadManager.cpp
sed -i '167s!QUrl !QUrl\& !' ../src/DownloadManager.cpp
sed -i '41s!QString !QString\& !' ../src/DownloadManager.cpp
sed -i '40s!QJsonDocument !QJsonDocument\& !' ../src/ModulesGroupModel.cpp
sed -i '57s!QJsonDocument !QJsonDocument\& !' ../src/ModulesGroupModel.cpp
