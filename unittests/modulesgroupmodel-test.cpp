#include "gtest_global.h"

#include "mock_modulesgroupmodel.h"
#include "mock_downloadmanager.h"
#include "mock_qsqldatabase.h"
#include "mock_qsqlquery.h"
#include "mock_qsqlerror.h"
#include "mock_qstringlist.h"
#include "mock_qjsonarray.h"
#include "mock_qfile.h"
#include "mock_qstring.h"
#include "mock_qjsondocument.h"
#include "mock_qjsonobject.h"
#include "mock_qjsonvalue.h"
#include "mock_qsettings.h"
#include "mock_qvariant.h"
#include "mock_qurl.h"
#include "mock_qbytearray.h"

#define REGISTRY "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw"
#define REGISTRY_INFO ""
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
      mockModulesGroupModel.registryVersion = &mockRegistryVersion;
      mockModulesGroupModel.qStringSql = &qStringSql;
      mockModulesGroupModel.qStringSelectSql = &qStringSelectSql;
      mockModulesGroupModel.qJsonParserError = &mockQJsonParseError;
      mockModulesGroupModel.qJsonDocument = &mockQJsonDocument;
      mockModulesGroupModel.manager = &mockManager;
      mockModulesGroupModel.qSettings = &mockQSettings;
      mockModulesGroupModel.query_ = &mockQSqlQuery;
      mockModulesGroupModel.urlRegistry = &mockQUrlRegistry;
      mockModulesGroupModel.urlRegistryInfo = &mockQUrlRegistryInfo;
      mockModulesGroupModel.qQByteArray = &mockQByteArray;
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
  StrictMock<MockQByteArray> mockQByteArray;
  StrictMock<MockQString> qStringSelectSql;
  StrictMock<MockQJsonParseError> mockQJsonParseError;
  StrictMock<MockQJsonDocument> mockQJsonDocument;
  QByteArray qByteArray = "data";
  DownloadManager* downloadManager;

  StrictMock<MockModulesGroupModel> mockModulesGroupModel;
  ModulesGroupModel* modulesGroupModel;
  MockDownloadManager mockManager;
  StrictMock<MockQSettings> mockQSettings;
  StrictMock<MockQUrl> mockQUrlRegistry;
  StrictMock<MockQUrl> mockQUrlRegistryInfo;

  MockQFile mockQFile;
  StrictMock<MockQFile> mockRegistryVersion;
  const QString tableName = "modules_group";
  const QUrl url = BuiltInDefaultValue<const QUrl>::Get();
  const QString registryFileName = "registry.json";
  const QString query = BuiltInDefaultValue<const QString>::Get();
  QFile::OpenMode qFileReadMode = QIODevice::ReadOnly | QIODevice::Text;
  QUrl::ParsingMode parsingMode = QUrl::ParsingMode::TolerantMode;

  // Objects declared here can be used by all tests in the test case for Foo.
};

MATCHER_P(HasSameParam, value, "") {
    if ( arg == value )
        return true;
    throw std::invalid_argument( "incorrect value" ); // WARNING: replace on custom Matcher
//    class DivisibleBy7Matcher : public MatcherInterface<int> {
//     public:
//      bool MatchAndExplain(int n,
//                           MatchResultListener* /* listener */) const override {
//        return (n % 7) == 0;
//      }

//      void DescribeTo(std::ostream* os) const override {
//        *os << "is divisible by 7";
//      }

//      void DescribeNegationTo(std::ostream* os) const override {
//        *os << "is not divisible by 7";
//      }
//    };

//    Matcher<int> DivisibleBy7() {
//      return MakeMatcher(new DivisibleBy7Matcher);
//    }
}

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

TEST_F(ModulesGroupModelTest, newRows)
{
//    ON_CALL(mockModulesGroupModel, newRows())
//            .WillByDefault(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentNewRows));

}

TEST_F(ModulesGroupModelTest, updateTable)
{
    EXPECT_CALL(mockModulesGroupModel, updateTable())
            .Times(3)
            .WillRepeatedly(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentUpdateTable));

    MockQJsonDocument mockQJsonDocumentData;
    MockQJsonObject mockQJsonObject;
    MockQJsonValue mockQJsonValue;
    MockQJsonArray mockQJsonArray;

    {
        InSequence s;
        EXPECT_CALL(mockQFile, open(qFileReadMode))
                .WillOnce(Return(false));
    }

    mockModulesGroupModel.updateTable();

    mockModulesGroupModel.qJsonParserError->error = QJsonParseError::NoError;

    {
        InSequence s;
        EXPECT_CALL(mockQFile, open(qFileReadMode))
                .WillOnce(Return(true));
        EXPECT_CALL(mockQFile, readAll())
                .WillOnce(Return(qByteArray));
        EXPECT_CALL(mockQJsonDocument, fromJson(qByteArray, mockModulesGroupModel.qJsonParserError))
                .WillOnce(ReturnPointee(&mockQJsonDocumentData));
        EXPECT_CALL(mockQFile, close());
        EXPECT_CALL(mockQJsonDocumentData, object())
                .WillOnce(ReturnPointee(&mockQJsonObject));
        EXPECT_CALL(mockQJsonObject, value(QString("downloads")))
                .WillOnce(ReturnPointee(&mockQJsonValue));
        EXPECT_CALL(mockQJsonValue, toArray())
                .WillOnce(ReturnPointee(&mockQJsonArray));
        EXPECT_CALL(mockModulesGroupModel, newRows(mockQJsonArray));
    }

    mockModulesGroupModel.updateTable();

    mockModulesGroupModel.qJsonParserError->error = QJsonParseError::UnterminatedObject;

    {
        InSequence s;
        EXPECT_CALL(mockQFile, open(qFileReadMode))
                .WillOnce(Return(true));
        EXPECT_CALL(mockQFile, readAll())
                .WillOnce(Return(qByteArray));
        EXPECT_CALL(mockQJsonDocument, fromJson(qByteArray, mockModulesGroupModel.qJsonParserError))
                .WillOnce(ReturnPointee(&mockQJsonDocumentData));
        EXPECT_CALL(mockQFile, close());
        EXPECT_CALL(mockQJsonDocumentData, object())
                .Times(0);
        EXPECT_CALL(mockQJsonObject, value(QString("downloads")))
                .Times(0);
        EXPECT_CALL(mockQJsonValue, toArray())
                .Times(0);
        EXPECT_CALL(mockModulesGroupModel, newRows(_))
                .Times(0);
    }

    mockModulesGroupModel.updateTable();
}

TEST_F(ModulesGroupModelTest, compareVersions)
{
    EXPECT_CALL(mockModulesGroupModel, compareVersions())
            .Times(3)
            .WillRepeatedly(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentCompareVerions));

    mockManager.fileNames = &mockQStringList;
    QString registryVersion("registry_version.json");
    MockQJsonDocument mockQJsonDocumentData;
    MockQJsonObject mockQJsonObject;
    MockQJsonValue mockQJsonValue;
    MockQJsonArray mockQJsonArray;


    {
        InSequence s;
        EXPECT_CALL(mockQStringList, last())
                .WillOnce(ReturnPointee(&registryVersion));
        EXPECT_CALL(mockRegistryVersion, setFileName(registryVersion));
        EXPECT_CALL(mockRegistryVersion, open(qFileReadMode))
                .WillOnce(Return(false));
    }

    mockModulesGroupModel.compareVersions();

    mockModulesGroupModel.qJsonParserError->error = QJsonParseError::UnterminatedObject;

    {
        InSequence s;
        EXPECT_CALL(mockQStringList, last())
                .WillOnce(ReturnPointee(&registryVersion));
        EXPECT_CALL(mockRegistryVersion, setFileName(registryVersion));
        EXPECT_CALL(mockRegistryVersion, open(qFileReadMode))
                .WillOnce(Return(true));
        EXPECT_CALL(mockRegistryVersion, readAll())
                .WillOnce(Return(qByteArray));
        EXPECT_CALL(mockQJsonDocument, fromJson(qByteArray, &mockQJsonParseError)) // WARNING: not right compare
                .WillOnce(ReturnPointee(&mockQJsonDocumentData));
        EXPECT_CALL(mockRegistryVersion, close());
    }

    mockModulesGroupModel.compareVersions();

    mockModulesGroupModel.qJsonParserError->error = QJsonParseError::NoError;


    {
        InSequence s;
        EXPECT_CALL(mockQStringList, last())
                .WillOnce(ReturnPointee(&registryVersion));
        EXPECT_CALL(mockRegistryVersion, setFileName(registryVersion));
        EXPECT_CALL(mockRegistryVersion, open(qFileReadMode))
                .WillOnce(Return(true));
        EXPECT_CALL(mockRegistryVersion, readAll())
                .WillOnce(Return(qByteArray));
        EXPECT_CALL(mockQJsonDocument, fromJson(qByteArray, &mockQJsonParseError)) // WARNING: not right compare
                .WillOnce(ReturnPointee(&mockQJsonDocumentData));
        EXPECT_CALL(mockRegistryVersion, close());
        EXPECT_CALL(mockQJsonDocumentData, object())
                .WillOnce(ReturnPointee(&mockQJsonObject));
        EXPECT_CALL(mockQJsonObject, value(QString("version")))
                .WillOnce(ReturnPointee(&mockQJsonValue));
        EXPECT_CALL(mockQJsonValue, toInt(0));
    }

    mockModulesGroupModel.compareVersions();
}

TEST_F(ModulesGroupModelTest, updateModules)
{
    EXPECT_CALL(mockModulesGroupModel, updateModules())
            .WillRepeatedly(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentUpdateModules));


    {
        InSequence s;
        EXPECT_CALL(mockQByteArray, fromBase64(HasSameParam(REGISTRY)))
                .WillOnce(Return(qByteArray));
        EXPECT_CALL(mockQUrlRegistry, fromEncoded(qByteArray, parsingMode)) // WARNING: not right compare
                .WillOnce(ReturnPointee(&url));
        EXPECT_CALL(mockManager, append(_));  // WARNING: pass Qurl object
        EXPECT_CALL(mockModulesGroupModel, setCountOldRows());
    }

    mockModulesGroupModel.updateModules();
}

TEST_F(ModulesGroupModelTest, setCountOldRows)
{
    EXPECT_CALL(mockModulesGroupModel, setCountOldRows())
            .WillRepeatedly(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentSetCountOldRows));

    QString sql("SELECT COUNT(*) as count FROM %1");
    MockQVariant var;
    int countOldRows = 1000;

    {
        InSequence s;
        EXPECT_CALL(qStringSelectSql, arg(tableName, 0, QChar(' ')))
                .WillOnce(Return(sql));
        EXPECT_CALL(mockModulesGroupModel, execLastError(sql));
        EXPECT_CALL(mockQSqlQuery, first());
        EXPECT_CALL(mockQSqlQuery, value(QString("count")))
                .WillOnce(ReturnPointee(&var));
        EXPECT_CALL(var, toInt(nullptr))
                .WillOnce(Return(countOldRows));
    }

    mockModulesGroupModel.setCountOldRows();
    EXPECT_THAT(mockModulesGroupModel.countOldRows, countOldRows);
    EXPECT_THAT(*ModulesGroupModel().qStringSelectSql, sql);
}

TEST_F(ModulesGroupModelTest, checkAvailabilityNewModules)
{
    EXPECT_CALL(mockModulesGroupModel, checkAvailabilityNewModules())
            .WillRepeatedly(Invoke(&mockModulesGroupModel, &MockModulesGroupModel::parentCheckAvailabilityNewModules));

    {
        InSequence s;
        EXPECT_CALL(mockQByteArray, fromBase64(HasSameParam(REGISTRY_INFO)))
                .WillOnce(Return(qByteArray));
        EXPECT_CALL(mockQUrlRegistryInfo, fromEncoded(qByteArray, parsingMode)) // WARNING: not right compare
                .WillOnce(ReturnPointee(&url));
        EXPECT_CALL(mockManager, append(_));  // WARNING: pass Qurl object
    }

    mockModulesGroupModel.checkAvailabilityNewModules();
}
