#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QList>
#include <QFileInfo>
#include "helpers.h"


struct File {

    QString name;
    QString path;

    static File fromUrl(const QUrl url) {
        File file { url.fileName(), url.toLocalFile()};
        return file;
    }

    static File fromFileInto(const QFileInfo & fileInfo )
    {
        File file { fileInfo.fileName(), fileInfo.absoluteFilePath() };
        return file;
    }

    QList<File> getChildrenAsFiles() {

        QList<File> children;
        QDir dir { path };

        if(dir.exists()) {
            for(const auto & fileInfo : dir.entryInfoList(QDir::AllEntries))
                children.append(File::fromFileInto(fileInfo));
        }

        return children;
    }

    bool is_directory() { return helpers::is_directory(path); }

    bool is_regular_file() { return helpers::is_regular_file(path); }

};

#endif // FILE_H
