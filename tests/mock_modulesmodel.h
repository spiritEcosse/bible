#ifndef MOCKMODULESMODEL_H
#define MOCKMODULESMODEL_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/ModulesModel.h"

class MockModulesModel : public ModulesModel
{
public:
    MOCK_METHOD2(createTable, bool(const QString &tableName, const QString &relatedTable));
    MOCK_METHOD1(setTable, void(const QString &tableName));
    MOCK_METHOD0(select, bool());
};

#endif // MOCKMODULESMODEL_H
