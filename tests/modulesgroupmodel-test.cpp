#include "gtest_global.h"

#include "mock_modulesgroupmodel.h"
#include "mock_qsqldatabase.h"
#include "mock_qsqlquery.h"
#include "mock_qsqlerror.h"
#include "mock_qstringlist.h"


// The fixture for testing class ModulesGroupModelTest.
class ModulesGroupModelTest : public TestWithParam<const char*> {
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
  MockQSqlDatabase mockQSqlDatabase;
  MockQStringList mockQStringList;

  MockModulesGroupModel mockModulesGroupModel;
  ModulesGroupModel* modulesGroupModel;

  const QString tableName = "modules_group";
  const QString query = BuiltInDefaultValue<const QString>::Get();

  // Objects declared here can be used by all tests in the test case for Foo.
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

TEST_P(ModulesGroupModelTest, correctSize) {
    EXPECT_EQ(mockModulesGroupModel.correctSize(GetParam()), sizes.value(GetParam()));
}

INSTANTIATE_TEST_CASE_P(PossibleIncomingSizes, ModulesGroupModelTest, ValuesIn(sizes.keys()));

TEST_F(ModulesGroupModelTest, init)
{
    MockModulesGroupModel mockModulesGroupModel;
    ON_CALL(mockModulesGroupModel, init())
            .WillByDefault(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentInit));
    {
        InSequence s;
        EXPECT_CALL(mockModulesGroupModel, setTable(tableName));
        EXPECT_CALL(mockModulesGroupModel, createTable(tableName));
        EXPECT_CALL(mockModulesGroupModel, select());
    }

    mockModulesGroupModel.init();
}

TEST_F(ModulesGroupModelTest, createTable)
{
    QString sql = QString(
                "CREATE TABLE IF NOT EXISTS '%1' ("
                "   'id'        INTEGER PRIMARY KEY AUTOINCREMENT, "
                "   'language'  CHAR(50), "
                "   'type'      CHAR(50), "
                "   'region'    CHAR(50) "
                ).arg(tableName);

    ON_CALL(mockModulesGroupModel, createTable(tableName))
            .WillByDefault(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::ParentCreateTable));

    {
        InSequence s;
        EXPECT_CALL(mockModulesGroupModel, database())
                .WillOnce(ReturnPointee(&mockQSqlDatabase));
        EXPECT_CALL(mockQSqlDatabase, tables())
                .WillOnce(ReturnPointee(&mockQStringList));
        EXPECT_CALL(mockQStringList, contains(tableName, Qt::CaseSensitive))
                .WillOnce(Return(false));
        EXPECT_CALL(mockModulesGroupModel, execLastError(sql))
                .WillOnce(Return(true));
    }

    EXPECT_TRUE(mockModulesGroupModel.createTable(tableName));

//    {
//        InSequence s;
//        EXPECT_CALL(mockModulesGroupModel, database())
//                .WillOnce(ReturnPointee(&mockQSqlDatabase));
//        EXPECT_CALL(mockQSqlDatabase, tables())
//                .WillRepeatedly(Return(QStringList{tableName}));
//    }
//    EXPECT_FALSE(mockModulesGroupModel.createTable(tableName));
}

TEST_F(ModulesGroupModelTest, execLastError)
{
    mockModulesGroupModel.query_ = &mockQSqlQuery;

    ON_CALL(mockModulesGroupModel, execLastError(query))
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
