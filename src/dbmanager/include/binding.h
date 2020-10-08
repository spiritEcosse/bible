#ifndef BINDING_H
#define BINDING_H

#include "sqlite_orm.h"
#include <QByteArray>


namespace db {

    inline char* QByteArrayToChar(QByteArray byte) {
        return byte.data();
    }

    inline std::unique_ptr<QByteArray> QByteArrayFromChar(const char* c) {
        return std::make_unique<QByteArray>(c);
    }

}

namespace sqlite_orm {

    /**
     *  First of all is a type_printer template class.
     *  It is responsible for sqlite type string representation.
     *  We want Gender to be `TEXT` so let's just derive from
     *  text_printer. Also there are other printers: real_printer and
     *  integer_printer. We must use them if we want to map our type to `REAL` (double/float)
     *  or `INTEGER` (int/long/short etc) respectively.
     */
    template<>
    struct type_printer<QByteArray> : public text_printer {};

    /**
     *  This is a binder class. It is used to bind c++ values to sqlite queries.
     *  Here we have to create gender string representation and bind it as string.
     *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
     *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
     *  More here https://www.sqlite.org/c3ref/bind_blob.html
     */
    template<>
    struct statement_binder<QByteArray> {
        int bind(sqlite3_stmt *stmt, int index, const QByteArray &value) {
            return statement_binder<const char*>().bind(stmt, index, db::QByteArrayToChar(value));
//            return sqlite3_bind_text(stmt, index++, QByteArrayToChar(value).c_str(), -1, SQLITE_TRANSIENT);
        }
    };

    /**
     *  field_printer is used in `dump` and `where` functions. Here we have to create
     *  a string from mapped object.
     */
    template<>
    struct field_printer<QByteArray> {
        char* operator()(const QByteArray &t) const {
            return db::QByteArrayToChar(t);
        }
    };

    /**
     *  This is a reverse operation: here we have to specify a way to transform string received from
     *  database to our Gender object. Here we call `GenderFromString` and throw `std::runtime_error` if it returns
     *  nullptr. Every `row_extractor` specialization must have `extract(const char*)` and `extract(sqlite3_stmt *stmt,
     * int columnIndex)` functions which return a mapped type value.
     */
    template<>
    struct row_extractor<QByteArray> {
        QByteArray extract(const char *row_value) {
            if(auto byte = db::QByteArrayFromChar(row_value)) {
                return *byte;
            } else {
                throw std::runtime_error("incorrect QByteArray data (" + std::string(row_value) + ")");
            }
        }

        QByteArray extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char *)str);
        }
    };
}

#endif // BINDING_H
