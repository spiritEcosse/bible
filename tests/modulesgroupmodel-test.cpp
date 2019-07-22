#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QDebug>
#include <iostream>

#include "mock_modulesgroupmodel.h"
#include "mock_iqsqldatabase.h"
#include "mock_qsqlquery.h"
#include "mock_qsqlerror.h"

using ::testing::_;
using ::testing::TestWithParam;
using ::testing::Test;
using ::testing::ValuesIn;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Mock;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::Invoke;
using ::testing::DefaultValue;
using ::testing::ReturnPointee;
using ::testing::ByRef;
using ::testing::InvokeWithoutArgs;
using testing::internal::BuiltInDefaultValue;

// The fixture for testing class ModulesGroupModelTest.
class ModulesGroupModelTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  ModulesGroupModelTest()
      : modulesGroupModel(&mockModulesGroupModel)
  {
     // You can do set-up work for each test here.
  }

  ~ModulesGroupModelTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
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

  MockQSqlQuery mockQSqlQuery;
  MockQSqlError mockQSqlError;

  MockModulesGroupModel mockModulesGroupModel;
  ModulesGroupModel* modulesGroupModel;

  const QString tableName = "modules_group";
  const QString query = BuiltInDefaultValue<const QString>::Get();

  // Objects declared here can be used by all tests in the test case for Foo.
};

//TEST_F(ModulesGroupModelTest, init)
//{
//    MockModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery> mockModulesGroupModel;
//    ON_CALL(mockModulesGroupModel, init())
//            .WillByDefault(Invoke(&mockModulesGroupModel, &MockModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery>::parentInit));
//    {
//        InSequence s;
//        EXPECT_CALL(mockModulesGroupModel, setTable(tableName));
//        EXPECT_CALL(mockModulesGroupModel, query())
//                .WillOnce(ReturnRef(mockIQSqlQuery));
//        EXPECT_CALL(mockModulesGroupModel, createTable(tableName));
//        EXPECT_CALL(mockModulesGroupModel, select());
//    }

//    mockModulesGroupModel.init();
//}

//TEST_F(ModulesGroupModelTest, createTable)
//{
//    QString sql = QString(
//                "CREATE TABLE IF NOT EXISTS '%1' ("
//                "   'id'        INTEGER PRIMARY KEY AUTOINCREMENT, "
//                "   'language'  CHAR(50), "
//                "   'type'      CHAR(50), "
//                "   'region'    CHAR(50) "
//                ).arg(tableName);
//    NiceMock<MockModulesGroupModel mockModulesGroupModel(mockIQSqlDatabase);

//    ON_CALL(mockModulesGroupModel, createTable(_))
//            .WillByDefault(Invoke(&mockModulesGroupModel, &MockModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery>::ParentCreateTable));
//    {
//        InSequence s;
//        EXPECT_CALL(mockIQSqlDatabase, tables())
//                .WillOnce(Return(QStringList{}));
//        EXPECT_CALL(mockModulesGroupModel, execLastError(sql))
//                .WillRepeatedly(Return(true));
//    }

//    EXPECT_TRUE(mockModulesGroupModel.createTable(tableName));
//    EXPECT_CALL(mockIQSqlDatabase, tables())
//            .WillRepeatedly(Return(QStringList{tableName}));
//    EXPECT_FALSE(mockModulesGroupModel.createTable(tableName));
//}


TEST_F(ModulesGroupModelTest, execLastError)
{
    mockModulesGroupModel.query_ = &mockQSqlQuery;

    ON_CALL(mockModulesGroupModel, execLastError(_))
            .WillByDefault(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentExecLastError));

    EXPECT_CALL(mockQSqlQuery, exec(query))
            .WillOnce(Return(true));

    EXPECT_TRUE(mockModulesGroupModel.execLastError(query));

    {
        InSequence s;
        EXPECT_CALL(mockQSqlQuery, exec(query))
                .WillOnce(Return(false));
        EXPECT_CALL(mockQSqlQuery, lastError())
                .WillOnce(ReturnPointee(&mockQSqlError));
        EXPECT_CALL(mockQSqlError, text());
    }
    EXPECT_FALSE(mockModulesGroupModel.execLastError(query));
}
