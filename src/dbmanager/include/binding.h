#ifndef BINDING_H
#define BINDING_H

#include "sqlite_orm.h"
#include <QByteArray>
#include <QDate>
#include "locallanguage.h"


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

    // LocalLanguage to std::string

    inline std::string LocalLanguageToString(const core::LocalLanguage& language) {
        return language.code().toStdString();
    }

    inline std::unique_ptr<core::LocalLanguage> LocalLanguageFromString(const std::string& str) {
        return std::make_unique<core::LocalLanguage>(QString::fromStdString(str));
    }

    // QString to std::string

    inline std::string QStringToString(const QString& value) {
        return value.toStdString();
    }

    inline std::unique_ptr<QString> QStringFromString(const std::string& str) {
        return std::make_unique<QString>(QString::fromStdString(str));
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
            if(auto value = db::QDateFromString(row_value)) {
                return std::move(*value);
            } else {
                throw std::runtime_error("incorrect QDate value (" + std::string(row_value) + ")");
            }
        }

        QDate extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char *)str);
        }
    };

    // LocalLanguage
    template<>
    struct type_printer<core::LocalLanguage> : public text_printer {};

    template<>
    struct statement_binder<core::LocalLanguage> {
        int bind(sqlite3_stmt *stmt, int index, const core::LocalLanguage& value) {
            return statement_binder<std::string>().bind(stmt, index, db::LocalLanguageToString(value));
        }
    };

    template<>
    struct field_printer<core::LocalLanguage> {
        std::string operator()(const core::LocalLanguage& value) const {
            return db::LocalLanguageToString(value);
        }
    };

    template<>
    struct row_extractor<core::LocalLanguage> {
        core::LocalLanguage extract(const std::string& row_value) {
            if(auto value = db::LocalLanguageFromString(row_value)) {
                return std::move(*value);
            } else {
                throw std::runtime_error("incorrect LocalLanguage value (" + std::string(row_value) + ")");
            }
        }

        core::LocalLanguage extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const std::string&)str);
        }
    };

    // QString
    template<>
    struct type_printer<QString> : public text_printer {};

    template<>
    struct statement_binder<QString> {
        int bind(sqlite3_stmt *stmt, int index, const QString& value) {
            return statement_binder<std::string>().bind(stmt, index, db::QStringToString(value));
        }
    };

    template<>
    struct field_printer<QString> {
        std::string operator()(const QString& value) const {
            return db::QStringToString(value);
        }
    };

    template<>
    struct row_extractor<QString> {
        QString extract(const std::string& row_value) {
            if(auto value = db::QStringFromString(row_value)) {
                return std::move(*value);
            } else {
                throw std::runtime_error("incorrect QString value (" + std::string(row_value) + ")");
            }
        }

        QString extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const std::string&)str);
        }
    };

}

#endif // BINDING_H
