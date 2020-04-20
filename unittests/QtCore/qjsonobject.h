#ifndef QJSONOBJECT_H
#define QJSONOBJECT_H

#include <QtCore/qjsonvalue.h>
#include <QtCore/qiterator.h>
#ifdef Q_COMPILER_INITIALIZER_LISTS
#include <QtCore/qpair.h>
#include <initializer_list>
#endif

QT_BEGIN_NAMESPACE

class QDebug;
template <class Key, class T> class QMap;
typedef QMap<QString, QVariant> QVariantMap;
template <class Key, class T> class QHash;
typedef QHash<QString, QVariant> QVariantHash;

class Q_CORE_EXPORT QJsonObject
{
public:
    QJsonObject();

#if defined(Q_COMPILER_INITIALIZER_LISTS) || defined(Q_QDOC)
    QJsonObject(std::initializer_list<QPair<QString, QJsonValue> > args)
    {
        initialize();
        for (std::initializer_list<QPair<QString, QJsonValue> >::const_iterator i = args.begin(); i != args.end(); ++i)
            insert(i->first, i->second);
    }
#endif

    virtual ~QJsonObject() {}

    QJsonObject(const QJsonObject &other);
    QJsonObject &operator =(const QJsonObject &other);

    QJsonObject(QJsonObject &&other) Q_DECL_NOTHROW
        : d(other.d), o(other.o)
    {
        other.d = nullptr;
        other.o = nullptr;
    }

    QJsonObject &operator =(QJsonObject &&other) Q_DECL_NOTHROW
    {
        swap(other);
        return *this;
    }

    void swap(QJsonObject &other) Q_DECL_NOTHROW
    {
        qSwap(d, other.d);
        qSwap(o, other.o);
    }

    static QJsonObject fromVariantMap(const QVariantMap &map);
    QVariantMap toVariantMap() const;
    static QJsonObject fromVariantHash(const QVariantHash &map);
    QVariantHash toVariantHash() const;

    QStringList keys() const;
    int size() const;
    inline int count() const { return size(); }
    inline int length() const { return size(); }
    bool isEmpty() const;

    virtual QJsonValue& value(const QString &key) const {}
    virtual QJsonValue& value(QLatin1String key) const {}
    QJsonValue operator[] (const QString &key) const;
    QJsonValue operator[] (QLatin1String key) const { return value(key); }
    QJsonValueRef operator[] (const QString &key);
    QJsonValueRef operator[] (QLatin1String key);

    void remove(const QString &key);
    QJsonValue take(const QString &key);
    bool contains(const QString &key) const;
    bool contains(QLatin1String key) const;

    bool operator==(const QJsonObject &other) const;
    bool operator!=(const QJsonObject &other) const;

    class const_iterator;

    class iterator
    {
        friend class const_iterator;
        friend class QJsonObject;
        QJsonObject *o;
        int i;

    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef int difference_type;
        typedef QJsonValue value_type;
        typedef QJsonValueRef reference;
        typedef QJsonValuePtr pointer;

        Q_DECL_CONSTEXPR inline iterator() : o(nullptr), i(0) {}
        Q_DECL_CONSTEXPR inline iterator(QJsonObject *obj, int index) : o(obj), i(index) {}

        inline QString key() const { return o->keyAt(i); }
        inline QJsonValueRef value() const { return QJsonValueRef(o, i); }
        inline QJsonValueRef operator*() const { return QJsonValueRef(o, i); }
#ifdef Q_QDOC
        inline QJsonValueRef* operator->() const;
#else
        inline QJsonValueRefPtr operator->() const { return QJsonValueRefPtr(o, i); }
#endif
        inline bool operator==(const iterator &other) const { return i == other.i; }
        inline bool operator!=(const iterator &other) const { return i != other.i; }

        inline iterator &operator++() { ++i; return *this; }
        inline iterator operator++(int) { iterator r = *this; ++i; return r; }
        inline iterator &operator--() { --i; return *this; }
        inline iterator operator--(int) { iterator r = *this; --i; return r; }
        inline iterator operator+(int j) const
        { iterator r = *this; r.i += j; return r; }
        inline iterator operator-(int j) const { return operator+(-j); }
        inline iterator &operator+=(int j) { i += j; return *this; }
        inline iterator &operator-=(int j) { i -= j; return *this; }

    public:
        inline bool operator==(const const_iterator &other) const { return i == other.i; }
        inline bool operator!=(const const_iterator &other) const { return i != other.i; }
    };
    friend class iterator;

    class const_iterator
    {
        friend class iterator;
        const QJsonObject *o;
        int i;

    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef int difference_type;
        typedef QJsonValue value_type;
        typedef QJsonValue reference;
        typedef QJsonValuePtr pointer;

        Q_DECL_CONSTEXPR inline const_iterator() : o(nullptr), i(0) {}
        Q_DECL_CONSTEXPR inline const_iterator(const QJsonObject *obj, int index)
            : o(obj), i(index) {}
        inline const_iterator(const iterator &other)
            : o(other.o), i(other.i) {}

        inline QString key() const { return o->keyAt(i); }
        inline QJsonValue value() const { return o->valueAt(i); }
        inline QJsonValue operator*() const { return o->valueAt(i); }
#ifdef Q_QDOC
        inline QJsonValue* operator->() const;
#else
        inline QJsonValuePtr operator->() const { return QJsonValuePtr(o->valueAt(i)); }
#endif
        inline bool operator==(const const_iterator &other) const { return i == other.i; }
        inline bool operator!=(const const_iterator &other) const { return i != other.i; }

        inline const_iterator &operator++() { ++i; return *this; }
        inline const_iterator operator++(int) { const_iterator r = *this; ++i; return r; }
        inline const_iterator &operator--() { --i; return *this; }
        inline const_iterator operator--(int) { const_iterator r = *this; --i; return r; }
        inline const_iterator operator+(int j) const
        { const_iterator r = *this; r.i += j; return r; }
        inline const_iterator operator-(int j) const { return operator+(-j); }
        inline const_iterator &operator+=(int j) { i += j; return *this; }
        inline const_iterator &operator-=(int j) { i -= j; return *this; }

        inline bool operator==(const iterator &other) const { return i == other.i; }
        inline bool operator!=(const iterator &other) const { return i != other.i; }
    };
    friend class const_iterator;

    // STL style
    inline iterator begin() { detach2(); return iterator(this, 0); }
    inline const_iterator begin() const { return const_iterator(this, 0); }
    inline const_iterator constBegin() const { return const_iterator(this, 0); }
    inline iterator end() { detach2(); return iterator(this, size()); }
    inline const_iterator end() const { return const_iterator(this, size()); }
    inline const_iterator constEnd() const { return const_iterator(this, size()); }
    iterator erase(iterator it);

    // more Qt
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    iterator find(const QString &key);
    iterator find(QLatin1String key);
    const_iterator find(const QString &key) const { return constFind(key); }
    const_iterator find(QLatin1String key) const { return constFind(key); }
    const_iterator constFind(const QString &key) const;
    const_iterator constFind(QLatin1String key) const;
    iterator insert(const QString &key, const QJsonValue &value);

    // STL compatibility
    typedef QJsonValue mapped_type;
    typedef QString key_type;
    typedef int size_type;

    inline bool empty() const { return isEmpty(); }

private:
    friend class QJsonPrivate::Data;
    friend class QJsonValue;
    friend class QJsonDocument;
    friend class QJsonValueRef;

    friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonObject &);

    QJsonObject(QJsonPrivate::Data *data, QJsonPrivate::Object *object);
    void initialize();
    // ### Qt 6: remove me and merge with detach2
    void detach(uint reserve = 0);
    bool detach2(uint reserve = 0);
    void compact();

    QString keyAt(int i) const;
    QJsonValue valueAt(int i) const;
    void setValueAt(int i, const QJsonValue &val);

    QJsonPrivate::Data *d;
    QJsonPrivate::Object *o;
};

Q_DECLARE_SHARED_NOT_MOVABLE_UNTIL_QT6(QJsonObject)

Q_CORE_EXPORT uint qHash(const QJsonObject &object, uint seed = 0);

#if !defined(QT_NO_DEBUG_STREAM) && !defined(QT_JSON_READONLY)
Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonObject &);
#endif

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QJsonObject &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QJsonObject &);
#endif

QT_END_NAMESPACE

#endif // QJSONOBJECT_H
