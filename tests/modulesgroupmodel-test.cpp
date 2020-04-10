#include "gtest_global.h"

#include "mock_modulesgroupmodel.h"
#include "mock_qsqldatabase.h"
#include "mock_qsqlquery.h"
#include "mock_qsqlerror.h"
#include "mock_qstringlist.h"
#include "mock_qjsonarray.h"
#include "mock_qfile.h"
#include "mock_qstring.h"
#include "mock_qjsondocument.h"
#include "mock_qjsonobject.h"

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
      mockModulesGroupModel.registry = &mockQFile;
      mockModulesGroupModel.qStringSql = &qStringSql;
      mockModulesGroupModel.qJsonParserError = &mockQJsonParseError;
      mockModulesGroupModel.qJsonDocument = &mockQJsonDocument;
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  MockQSqlQuery mockQSqlQuery;
  MockQSqlError mockQSqlError;
  MockQSqlDatabase mockQSqlDatabase;
  MockQStringList mockQStringList;
  StrictMock<MockQString> qStringSql;
  StrictMock<MockQJsonParseError> mockQJsonParseError;
  StrictMock<MockQJsonDocument> mockQJsonDocument;
  QByteArray data;

  StrictMock<MockModulesGroupModel> mockModulesGroupModel;
  ModulesGroupModel* modulesGroupModel;

  MockQFile mockQFile;
  const QString tableName = "modules_group";
  const QString registryFileName = "registry.json";
  const QString query = BuiltInDefaultValue<const QString>::Get();
  QFile::OpenMode qFileReadMode = QIODevice::ReadOnly | QIODevice::Text;

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(ModulesGroupModelTest, init)
{
    EXPECT_CALL(mockModulesGroupModel, init())
            .WillOnce(
               Invoke(
                    &mockModulesGroupModel, &MockModulesGroupModel::parentInit
                    )
                );

    {
        InSequence s;
        EXPECT_CALL(mockModulesGroupModel, createTable());
        EXPECT_CALL(mockModulesGroupModel, setTable(tableName));
        EXPECT_CALL(mockModulesGroupModel, select());
        EXPECT_CALL(mockModulesGroupModel, setEditStrategy(QSqlTableModel::OnManualSubmit));
        EXPECT_CALL(mockQFile, setFileName(registryFileName));
    }

    mockModulesGroupModel.init();
}

TEST_F(ModulesGroupModelTest, createTable)
{
    QString sql("CREATE TABLE IF NOT EXISTS '%1' ("
                "   'id'        INTEGER PRIMARY KEY AUTOINCREMENT, "
                "   'language'  CHAR(50), "
                "   'type'      CHAR(50), "
                "   'region'    CHAR(50) "
                ")"
                );
    EXPECT_CALL(mockModulesGroupModel, createTable())
            .Times(2)
            .WillRepeatedly(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::ParentCreateTable));

    {
        InSequence s;
        EXPECT_CALL(mockModulesGroupModel, database())
                .WillOnce(ReturnPointee(&mockQSqlDatabase));
        EXPECT_CALL(mockQSqlDatabase, tables())
                .WillOnce(ReturnPointee(&mockQStringList));
        EXPECT_CALL(mockQStringList, contains(tableName, Qt::CaseSensitive))
                .WillOnce(Return(false));
        EXPECT_CALL(qStringSql, arg(tableName, 0, QChar(' ')))
                .WillOnce(Return(sql));
        EXPECT_CALL(mockModulesGroupModel, execLastError(sql))
                .WillOnce(Return(true));
    }

    EXPECT_TRUE(mockModulesGroupModel.createTable());

    {
        InSequence s;
        EXPECT_CALL(mockModulesGroupModel, database())
                .WillOnce(ReturnPointee(&mockQSqlDatabase));
        EXPECT_CALL(mockQSqlDatabase, tables())
                .WillRepeatedly(ReturnPointee(&mockQStringList));
        EXPECT_CALL(mockQStringList, contains(tableName, Qt::CaseSensitive))
                .WillOnce(Return(true));
    }

    EXPECT_FALSE(mockModulesGroupModel.createTable());
    EXPECT_THAT(*ModulesGroupModel().qStringSql, sql);
}

TEST_F(ModulesGroupModelTest, execLastError)
{
    mockModulesGroupModel.query_ = &mockQSqlQuery;

    EXPECT_CALL(mockModulesGroupModel, execLastError(query))
            .Times(2)
            .WillRepeatedly(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentExecLastError));

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

TEST_F(ModulesGroupModelTest, newRows) {
//    ON_CALL(mockModulesGroupModel, newRows())
//            .WillByDefault(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentNewRows));

}

TEST_F(ModulesGroupModelTest, updateTable) {
    EXPECT_CALL(mockModulesGroupModel, updateTable())
            .Times(2)
            .WillRepeatedly(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentUpdateTable));

    {
        InSequence s;
        EXPECT_CALL(mockQFile, open(qFileReadMode))
                .WillOnce(Return(false));
        EXPECT_CALL(mockQFile, readAll())
                .Times(0)
                .WillOnce(Return(data));
        EXPECT_CALL(mockQJsonDocument, fromJson(data, mockModulesGroupModel.qJsonParserError))
                .Times(0);
        EXPECT_CALL(mockQFile, close())
                .Times(0);
        EXPECT_CALL(mockModulesGroupModel, newRows(_))
                .Times(0);
    }

    mockModulesGroupModel.updateTable();

    mockModulesGroupModel.qJsonParserError->error = QJsonParseError::UnterminatedObject;

    {
        InSequence s;
        EXPECT_CALL(mockQFile, open(qFileReadMode))
                .WillOnce(Return(true));
        EXPECT_CALL(mockQFile, readAll())
                .WillOnce(Return(data));
        EXPECT_CALL(mockQJsonDocument, fromJson(data, mockModulesGroupModel.qJsonParserError));
        EXPECT_CALL(mockQFile, close());
        EXPECT_CALL(mockModulesGroupModel, newRows(_))
                .Times(0);
    }

    mockModulesGroupModel.updateTable();

    mockModulesGroupModel.qJsonParserError->error = QJsonParseError::NoError;
    MockQJsonDocument mockQJsonDocumentData;
    MockQJsonObject mockQJsonObject;

//    {
//        InSequence s;
//        EXPECT_CALL(mockQFile, open(qFileReadMode))
//                .WillOnce(Return(true));
//        EXPECT_CALL(mockQFile, readAll())
//                .WillOnce(Return(data));
//        EXPECT_CALL(mockQJsonDocument, fromJson(data, mockModulesGroupModel.qJsonParserError));
//        EXPECT_CALL(mockQFile, close());
////        EXPECT_CALL(mockQJsonDocumentData, object())
////                .WillOnce(ReturnPointee(&mockQJsonObject));
//        EXPECT_CALL(mockModulesGroupModel, newRows(_));
//    }

//    mockModulesGroupModel.updateTable();
}
