#ifndef QFILE_H
#define QFILE_H

#include <QtCore/qfiledevice.h>
#include <QtCore/qstring.h>
#include <stdio.h>

#ifdef open
#error qfile.h must be included before any header file that defines open
#endif

QT_BEGIN_NAMESPACE

class QTemporaryFile;
class QFilePrivate;

class Q_CORE_EXPORT QFile : public QFileDevice
{
#ifndef QT_NO_QOBJECT
    Q_OBJECT
#endif
    Q_DECLARE_PRIVATE(QFile)

public:
    QFile();
    QFile(const QString &name);
#ifndef QT_NO_QOBJECT
    explicit QFile(QObject *parent);
    QFile(const QString &name, QObject *parent);
#endif
    virtual ~QFile();

    QString fileName() const override;
    virtual void setFileName(const QString &name);
    virtual inline qint64 write(const QByteArray &)
    {
        return 1;
    }
    virtual void close() override {}

#if defined(Q_OS_DARWIN)
    // Mac always expects filenames in UTF-8... and decomposed...
    static inline QByteArray encodeName(const QString &fileName)
    {
        return fileName.normalized(QString::NormalizationForm_D).toUtf8();
    }
    static QString decodeName(const QByteArray &localFileName)
    {
        // note: duplicated in qglobal.cpp (qEnvironmentVariable)
        return QString::fromUtf8(localFileName).normalized(QString::NormalizationForm_C);
    }
#else
    static inline QByteArray encodeName(const QString &fileName)
    {
        return fileName.toLocal8Bit();
    }
    static QString decodeName(const QByteArray &localFileName)
    {
        return QString::fromLocal8Bit(localFileName);
    }
#endif
    inline static QString decodeName(const char *localFileName)
        { return decodeName(QByteArray(localFileName)); }

#if QT_DEPRECATED_SINCE(5,0)
    typedef QByteArray (*EncoderFn)(const QString &fileName);
    typedef QString (*DecoderFn)(const QByteArray &localfileName);
    QT_DEPRECATED static void setEncodingFunction(EncoderFn) {}
    QT_DEPRECATED static void setDecodingFunction(DecoderFn) {}
#endif

    bool exists() const;
    static bool exists(const QString &fileName);

#if QT_DEPRECATED_SINCE(5, 13)
    QT_DEPRECATED_X("Use QFile::symLinkTarget() instead")
    QString readLink() const;
    QT_DEPRECATED_X("Use QFile::symLinkTarget(QString) instead")
    static QString readLink(const QString &fileName);
#endif
    QString symLinkTarget() const;
    static QString symLinkTarget(const QString &fileName);

    virtual bool remove();
    static bool remove(const QString &fileName);

    bool rename(const QString &newName);
    static bool rename(const QString &oldName, const QString &newName);

    bool link(const QString &newName);
    static bool link(const QString &oldname, const QString &newName);

    bool copy(const QString &newName);
    static bool copy(const QString &fileName, const QString &newName);

    virtual bool open(OpenMode flags) override;
    bool open(FILE *f, OpenMode ioFlags, FileHandleFlags handleFlags=DontCloseHandle);
    bool open(int fd, OpenMode ioFlags, FileHandleFlags handleFlags=DontCloseHandle);

    qint64 size() const override;

    bool resize(qint64 sz) override;
    static bool resize(const QString &filename, qint64 sz);

    Permissions permissions() const override;
    static Permissions permissions(const QString &filename);
    bool setPermissions(Permissions permissionSpec) override;
    static bool setPermissions(const QString &filename, Permissions permissionSpec);

protected:
#ifdef QT_NO_QOBJECT
    QFile(QFilePrivate &dd);
#else
    QFile(QFilePrivate &dd, QObject *parent = nullptr);
#endif

private:
    friend class QTemporaryFile;
    Q_DISABLE_COPY(QFile)
};

QT_END_NAMESPACE

#endif // QFILE_H
