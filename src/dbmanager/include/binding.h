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

    // LocalLanguage to char*

    inline char* LocalLanguageToChar(const core::LocalLanguage& language) {
        return language.code().toLocal8Bit().data();
    }

    inline std::unique_ptr<core::LocalLanguage> LocalLanguageFromChar(const char* c) {
        return std::make_unique<core::LocalLanguage>(c);
    }

    // QString to char*

    inline char* QStringToChar(const QString& value) {
        return value.toLocal8Bit().data();
    }

    inline std::unique_ptr<QString> QStringFromChar(const char* c) {
        return std::make_unique<QString>(c);
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
            return statement_binder<std::string>().bind(stmt, index, db::LocalLanguageToChar(value));
        }
    };

    template<>
    struct field_printer<core::LocalLanguage> {
        std::string operator()(const core::LocalLanguage& value) const {
            return db::LocalLanguageToChar(value);
        }
    };

    template<>
    struct row_extractor<core::LocalLanguage> {
        core::LocalLanguage extract(const char* row_value) {
            if(auto value = db::LocalLanguageFromChar(row_value)) {
                return std::move(*value);
            } else {
                throw std::runtime_error("incorrect LocalLanguage value (" + std::string(row_value) + ")");
            }
        }

        core::LocalLanguage extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char*)str);
        }
    };

    // QString
    template<>
    struct type_printer<QString> : public text_printer {};

    template<>
    struct statement_binder<QString> {
        int bind(sqlite3_stmt *stmt, int index, const QString& value) {
            return statement_binder<std::string>().bind(stmt, index, db::QStringToChar(value));
        }
    };

    template<>
    struct field_printer<QString> {
        std::string operator()(const QString& value) const {
            return db::QStringToChar(value);
        }
    };

    template<>
    struct row_extractor<QString> {
        QString extract(const char* row_value) {
            if(auto value = db::QStringFromChar(row_value)) {
                return std::move(*value);
            } else {
                throw std::runtime_error("incorrect QString value (" + std::string(row_value) + ")");
            }
        }

        QString extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char*)str);
        }
    };

}

#endif // BINDING_H
