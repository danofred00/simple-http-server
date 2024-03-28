#ifndef HELPERS_H
#define HELPERS_H

#include <filesystem>
#include <QString>
#include <QFile>

namespace fs = std::filesystem;
namespace helpers {

bool exists(const QString & path)
{
    return fs::exists(std::move(path.toStdString()));
}

bool is_directory(const QString & path)
{
    return fs::is_directory(std::move(path.toStdString()));
}

bool is_regular_file(const QString & path)
{
    return fs::is_regular_file(std::move(path.toStdString()));
}

bool is_symlink(const QString & path)
{
    return fs::is_symlink(std::move(path.toStdString()));
}

QString makePath(const QString base, const QString path)
{
    return QString("%1%2").arg(base,path);
}

QByteArray readFile(const QString & path)
{
    QFile file { path };

    if(file.exists()) {
        if(file.open(QFile::ReadOnly)) {
            auto content = file.readAll();
            file.close();
            return content;
        }
    }

    return QByteArray();
}

const QString absoluteToRelativePath(const QString & path, const QString & relativeTo)
{
    return QString::fromStdString(std::move(fs::relative(path.toStdString(), relativeTo.toStdString())));
}

bool replaceString(QByteArray & buffer, const QByteArrayView pattern, const QByteArrayView value)
{
    auto indexOfPattern = buffer.indexOf(pattern);

    if(indexOfPattern == -1)
        return false;

    auto sizeOfPattern = pattern.size();
    buffer.replace(indexOfPattern, sizeOfPattern, value);

    return true;
}

}

#endif // HELPERS_H
