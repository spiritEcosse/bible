#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QUrl>
#include <QJsonValue>

class QJsonObject;

namespace modules {

    class RegistryInvalidData: virtual public std::exception {

    protected:

        std::string error_message;      ///< Error message

    public:

        /** Constructor (C++ STL string, int, int).
         *  @param msg The error message
         *  @param err_num Error number
         *  @param err_off Error offset
         */
        explicit
        RegistryInvalidData(const std::string& msg)
            : error_message(msg)
            {}

        /** Destructor.
         *  Virtual to allow for subclassing.
         */
        virtual ~RegistryInvalidData() throw () {}

        /** Returns a pointer to the (constant) error description.
         *  @return A pointer to a const char*. The underlying memory
         *  is in possession of the Except object. Callers must
         *  not attempt to free the memory.
         */
        virtual const char* what() const throw () {
           return error_message.c_str();
        }

    };

    class Registry
    {
    public:
        Registry();
        Registry(const QJsonObject& registryJson);
        Registry(const QByteArray& url,
                 const QByteArray& infoUrl,
                 const short& priority = 0,
                 const bool& test = false);

        QUrl urlToQUrl() const;
        QUrl infoUrlToQUrl() const;
        bool operator==(const Registry& other) const;

        QByteArray m_url;
        QByteArray m_infoUrl;
        short m_priority;
        bool m_test;
        int m_id;

    #ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Registry& registry);
    #endif
    };

}

#endif // REGISTRY_H
