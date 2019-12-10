#include "gtest_global.h"

#include "mock_modulesmodel.h"
#include "mock_qsqldatabase.h"
#include "mock_qsqlquery.h"
#include "mock_qsqlerror.h"
#include "mock_qstringlist.h"


// The fixture for testing class ModulesModel.
class ModulesModelTest : public TestWithParam<const char*> {
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    ModulesModelTest()
        : modulesModel(&mockModulesModel)
    {}

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
    MockQSqlQuery mockQSqlQuery;
    MockQSqlError mockQSqlError;
    MockQSqlDatabase mockQSqlDatabase;
    MockQStringList mockQStringList;

    MockModulesModel mockModulesModel;
    ModulesModel* modulesModel;

    const QString tableName = "modules";
    const QString relatedTable = "modules_group";
    const QString query = BuiltInDefaultValue<const QString>::Get();
};

TEST_F(ModulesModelTest, init)
{
    ON_CALL(mockModulesModel, init())
            .WillByDefault(Invoke(&mockModulesModel, &MockModulesModel::parentInit));

    {
        InSequence s;
        EXPECT_CALL(mockModulesModel, createTable(tableName, relatedTable));
        EXPECT_CALL(mockModulesModel, setTable(tableName));
        EXPECT_CALL(mockModulesModel, select());
    }

    mockModulesModel.init();
}

TEST_F(ModulesModelTest, createTable)
{
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
                ).arg(tableName, relatedTable);
    ON_CALL(mockModulesModel, createTable(tableName, relatedTable))
            .WillByDefault(Invoke(&mockModulesModel, &MockModulesModel::ParentCreateTable));
    {
        InSequence s;
        EXPECT_CALL(mockModulesModel, database())
                .WillOnce(ReturnPointee(&mockQSqlDatabase));
        EXPECT_CALL(mockQSqlDatabase, tables())
                .WillOnce(ReturnPointee(&mockQStringList));
        EXPECT_CALL(mockModulesModel, execLastError(sql))
                .WillRepeatedly(Return(true));
    }

    EXPECT_TRUE(mockModulesModel.createTable(tableName, relatedTable));
//    {
//        InSequence s;
//        EXPECT_CALL(mockModulesModel, database())
//                .WillOnce(ReturnPointee(&mockQSqlDatabase));
//        EXPECT_CALL(mockQSqlDatabase, tables())
//                .WillRepeatedly(Return(QStringList{tableName}));
//    }
//    EXPECT_FALSE(mockModulesModel.createTable(tableName, relatedTable));
}

TEST_F(ModulesModelTest, execLastError)
{
    mockModulesModel.query_ = &mockQSqlQuery;

    ON_CALL(mockModulesModel, execLastError(query))
            .WillByDefault(Invoke(&mockModulesModel, &MockModulesModel::parentExecLastError));

    EXPECT_CALL(mockQSqlQuery, exec(query))
            .WillOnce(Return(true));

    EXPECT_TRUE(mockModulesModel.execLastError(query));

    {
        InSequence s;
        EXPECT_CALL(mockQSqlQuery, exec(query))
                .WillOnce(Return(false));
        EXPECT_CALL(mockQSqlQuery, lastError())
                .WillOnce(ReturnPointee(&mockQSqlError));
        EXPECT_CALL(mockQSqlError, text());
    }
    EXPECT_FALSE(mockModulesModel.execLastError(query));
}
