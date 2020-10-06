#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QUrl>
#include <QJsonValue>

class QJsonObject;

class Registry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray url READ url WRITE setUrl)
    Q_PROPERTY(short priority READ priority WRITE setPriority)
    Q_PROPERTY(QByteArray infoUrl READ infoUrl WRITE setInfoUrl)
public:
    Registry();
    Registry(const Registry& other);
    Registry &operator=(const Registry &other);
    Registry(const QJsonObject& registryJson);
    Registry(const char* url, short && priority, const char* infoUrl);

    static const std::string tableName();
    static const std::vector<const char*> getColumns(const Registry& registry = Registry());
    static const std::string columnsJoinToString(const Registry& registry = Registry());

    Q_INVOKABLE QByteArray url() const;
    QUrl urlToQUrl() const;
    void setUrl(QByteArray& url);
    Q_INVOKABLE short priority() const;
    void setPriority(short &priority);
    Q_INVOKABLE QByteArray infoUrl() const;
    QUrl infoUrlToQUrl() const;
    void setInfoUrl(QByteArray& url);
    bool operator==(const Registry& other) const;
private:
    QByteArray m_url;
    short int m_priority;
    QByteArray m_infoUrl;

#ifndef QT_NO_DEBUG_STREAM
    friend QDebug operator<<(QDebug debug, const Registry& registry);
#endif
};

#endif // REGISTRY_H
