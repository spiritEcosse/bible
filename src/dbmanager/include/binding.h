#ifndef BINDING_H
#define BINDING_H

#include "sqlite_orm.h"
#include <QByteArray>
#include <QDate>


namespace db {

    // QByteArray to char*

    inline char* QByteArrayToChar(QByteArray byte) {
        return byte.data();
    }

    inline std::unique_ptr<QByteArray> QByteArrayFromChar(const char* c) {
        return std::make_unique<QByteArray>(c);
    }

    // QDate to std::string

    inline std::string QDateToString(const QDate& date) {
        return date.toString().toStdString();
    }

    inline std::unique_ptr<QDate> QDateFromString(const std::string& str) {
        return std::make_unique<QDate>(QDate::fromString(QString::fromStdString(str)));
    }

}

namespace sqlite_orm {

    // QByteArray
    template<>
    struct type_printer<QByteArray> : public text_printer {};

    template<>
    struct statement_binder<QByteArray> {
        int bind(sqlite3_stmt *stmt, int index, const QByteArray &value) {
            return statement_binder<const char*>().bind(stmt, index, db::QByteArrayToChar(value));
//            return sqlite3_bind_text(stmt, index++, QByteArrayToChar(value).c_str(), -1, SQLITE_TRANSIENT);
        }
    };

    template<>
    struct field_printer<QByteArray> {
        char* operator()(const QByteArray &t) const {
            return db::QByteArrayToChar(t);
        }
    };

    template<>
    struct row_extractor<QByteArray> {
        QByteArray extract(const char *row_value) {
            if(auto byte = db::QByteArrayFromChar(row_value)) {
                return std::move(*byte);
            } else {
                throw std::runtime_error("incorrect QByteArray data (" + std::string(row_value) + ")");
            }
        }

        QByteArray extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char *)str);
        }
    };

    // QDate
    template<>
    struct type_printer<QDate> : public text_printer {};

    template<>
    struct statement_binder<QDate> {
        int bind(sqlite3_stmt *stmt, int index, const QDate& value) {
            return statement_binder<std::string>().bind(stmt, index, db::QDateToString(value));
        }
    };

    template<>
    struct field_printer<QDate> {
        std::string operator()(const QDate& date) const {
            return db::QDateToString(date);
        }
    };

    template<>
    struct row_extractor<QDate> {
        QDate extract(const char *row_value) {
            if(auto date = db::QDateFromString(row_value)) {
                return std::move(*date);
            } else {
                throw std::runtime_error("incorrect QDate data (" + std::string(row_value) + ")");
            }
        }

        QDate extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char *)str);
        }
    };

}

#endif // BINDING_H
