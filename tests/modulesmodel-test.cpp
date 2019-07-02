#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QDebug>
#include <iostream>

#include "mock_modulesmodel.h"
#include "mock_iqsqldatabase.h"

#include "../src/ModulesModel.h"


using ::testing::_;
using ::testing::TestWithParam;
using ::testing::Test;
using ::testing::ValuesIn;
using ::testing::NiceMock;
using ::testing::TypedEq;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Mock;
using ::testing::Invoke;

// The fixture for testing class ModulesModel.
class ModulesModelTest : public TestWithParam<const char*> {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  ModulesModelTest() {
     // You can do set-up work for each test here.
  }

  ~ModulesModelTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
//      delete mockModulesModel;
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for ModulesModel.
  MockModulesModel<MockIQSqlDatabase>* mockModulesModel;
};


static QHash<const char *, int> sizes = {
    { "100.0K", 102400 },
    { "1K", 1024 },
    { "12.32M", 12918456 },
    { "1m", 1048576 },
    { "0.05G", 53687091 },
    { "1g", 1073741824 },
    { "65700", 65700 },
};

// Tests that ModulesModel does correctSize.
TEST_P(ModulesModelTest, correctSize) {
    EXPECT_EQ(mockModulesModel->correctSize(GetParam()), sizes.value(GetParam()));
}

INSTANTIATE_TEST_CASE_P(PossibleIncomingSizes, ModulesModelTest, ValuesIn(sizes.keys()));

TEST_F(ModulesModelTest, init)
{
    const QString tableName = "modules";
    const QString tableNameRelated = "modules_group";

    mockModulesModel = new MockModulesModel<MockIQSqlDatabase>;
    {
        InSequence s;
        EXPECT_CALL(*mockModulesModel, createTable(tableName, tableNameRelated))
                .WillOnce(Return(true));
        EXPECT_CALL(*mockModulesModel, setTable(tableName));
        EXPECT_CALL(*mockModulesModel, select());
    }
    mockModulesModel->init();
    EXPECT_CALL(*mockModulesModel, createTable(tableName, tableNameRelated))
            .WillRepeatedly(Return(false));
    mockModulesModel->createTable(tableName, tableNameRelated);
}

TEST_F(ModulesModelTest, createTable)
{
    const QString tableName = "modules";
    const QString tableNameRelated = "modules_group";
    QString sql = QString(
                "CREATE TABLE IF NOT EXISTS '%1' ("
                "   'id'                INTEGER PRIMARY KEY AUTOINCREMENT, "
                "   'name'              CHAR(200) NOT NULL, "
                "   'description'       TEXT, "
                "   'abbreviation'      CHAR(50), "
                "   'information'       TEXT, "
                "   'language'          CHAR(50), "
                "   'language_show'     CHAR(50), "
                "   'update'            TEXT, "
                "   'urls'              TEXT, "
                "   'comment'           TEXT, "
                "   'size'              NUMERIC NOT NULL, "
                "   'region'            TEXT, "
                "   'default_download'  NUMERIC DEFAULT 0, "
                "   'hidden'            NUMERIC DEFAULT 0, "
                "   'copyright'         TEXT, "
                "   '%2_id'             NUMERIC NOT NULL, "
                "FOREIGN KEY ('%2_id')  REFERENCES %2(id)"
            ")"
                ).arg(tableName, tableNameRelated);
    MockIQSqlDatabase mockIQSqlDatabase;

    {
        InSequence s;
        EXPECT_CALL(mockIQSqlDatabase, tables());
        EXPECT_CALL(mockIQSqlDatabase, exec(sql));
    }

    ModulesModel<MockIQSqlDatabase> modulesModel(mockIQSqlDatabase, nullptr);
    modulesModel.createTable(tableName, tableNameRelated);
}

