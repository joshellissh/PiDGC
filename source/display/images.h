#ifndef IMAGES_H
#define IMAGES_H

#include "dirent.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <map>
#include <QDirIterator>
#include <QFileInfo>

using namespace std;

typedef map<string, QImage> ImageMap;

inline ImageMap *loadImages(QString resourceDir) {
    ImageMap *imageMap = new ImageMap;

    QDirIterator it(resourceDir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo fileInfo(it.next());
        imageMap->insert(pair<string, QImage>(fileInfo.fileName().toStdString(), QImage(fileInfo.filePath())));
        qDebug() << "Loading image:" << fileInfo.filePath();
    }

    return imageMap;
}

#endif // IMAGES_H
