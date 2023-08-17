#include "basetest.h"
#include "dereferenceiterator.h"
#include "modelbook.h"
#include "modelcomment.h"
#include "modelgroupmodules.h"
#include "modelhost.h"
#include "modelmodule.h"
#include "modelrecord.h"
#include "modelregistry.h"
#include "modelverse.h"
#include <JlCompress.h>

namespace tests {

    template<class T, class O, class S>
    BaseTest<T, O, S>::BaseTest() {}

    template<class T, class O, class S>
    BaseTest<T, O, S>::~BaseTest() {}

    template<class T, class O, class S>
    void BaseTest<T, O, S>::initTestCase() {
        // will be called before the first test function is executed.
        dir.mkdir(pathFiles);
        dir.setCurrent(pathFiles);
        dir.mkdir(dirDownload);
        dir.mkdir("modules");
        initDb();
    }

    template<class T, class O, class S>
    void BaseTest<T, O, S>::init() {
        // will be called before each test function is executed.
        dir.mkdir(pathFiles);
        dir.setCurrent(pathFiles);
        dir.mkdir(dirDownload);
        dir.mkdir("modules");
        initDb();
    }

    template<class T, class O, class S>
    void BaseTest<T, O, S>::cleanup() {
        dir.rmdir(dirDownload);
        cleanTable();
    }

    template<class T, class O, class S>
    void BaseTest<T, O, S>::cleanupTestCase() {
        dir.rmdir(dirDownload);
    }

    template<class T, class O, class S>
    std::vector<std::shared_ptr<T>> BaseTest<T, O, S>::helperSave(std::vector<std::shared_ptr<T>> &&entries) {
        const auto &objects = entries.size() == 0 ? helperGetObjects() : std::move(entries);
        m_db->save(objects.begin(), objects.end());
        return std::move(objects);
    }

    template<class T, class O, class S>
    std::vector<std::unique_ptr<T>> BaseTest<T, O, S>::helperSaveUnique(std::vector<std::unique_ptr<T>> &&entries) {
        auto &&objects = entries.size() == 0 ? helperGetObjectsUnique() : std::move(entries);
        m_db->save(objects.begin(), objects.end());
        return std::move(objects);
    }

    template<class T, class O, class S>
    void BaseTest<T, O, S>::cleanTable() {
        m_db->removeAll();
    }

    template<class T, class O, class S>
    void BaseTest<T, O, S>::initDb() {
        m_db = std::make_unique<db::Db<T, S>>(":memory:");
        cleanTable();
    }

    template<class T, class O, class S>
    std::vector<std::shared_ptr<T>> BaseTest<T, O, S>::helperGetObjects() const {
        return std::vector<std::shared_ptr<T>>{};
    }

    template<class T, class O, class S>
    std::vector<std::unique_ptr<T>> BaseTest<T, O, S>::helperGetObjectsUnique() const {
        return std::vector<std::unique_ptr<T>>{};
    }

    template<class T, class O, class S>
    QString BaseTest<T, O, S>::getModulePath() const {
        return QFileInfo(fileModuleArchive).absolutePath();
    }

    template<class T, class O, class S>
    QString BaseTest<T, O, S>::getModuleFolderPath() const {
        return folderModuleInModules.absolutePath();
    }

    template<class T, class O, class S>
    QString BaseTest<T, O, S>::getModuleFilePath(const QFile &file) const {
        return QFileInfo(file.fileName() != "" ? file : fileModuleArchive).absoluteFilePath();
    }

    template<class T, class O, class S>
    QString BaseTest<T, O, S>::makeUrlModuleHost() const {
        return QString(strUrl + getModulePath() + urlMask);
    }

    template<class T, class O, class S>
    QByteArray BaseTest<T, O, S>::urlModuleToBase64() const {
        return makeUrlModuleHost().toUtf8().toBase64();
    }

    template<class T, class O, class S>
    void BaseTest<T, O, S>::createFileModule() {
        QFile fileModule(QString("%1.sqlite").arg(moduleName));
        fileModule.open(QFile::WriteOnly);
        fileModule.close();
        QVERIFY(JlCompress::compressFile(fileModuleArchive.fileName(), fileModule.fileName()));
    }

    template<class T, class O, class S>
    void BaseTest<T, O, S>::createFileModuleInModules() {
        fileModuleArchiveInModules.open(QFile::WriteOnly);
        fileModuleArchiveInModules.close();
    }

    template<class T, class O, class S>
    void BaseTest<T, O, S>::createFolderModuleInModules() {
        folderModuleInModules.mkdir(".");
    }

    template class BaseTest<modules::Module, modules::ModelModule>;
    template class BaseTest<modules::GroupModules, modules::ModelGroupModules>;
    template class BaseTest<modules::Registry, modules::ModelRegistry>;
    template class BaseTest<modules::Host, modules::ModelHost>;
    template class BaseTest<modules::Book, modules::ModelBook, db::TranslationStorage>;
    template class BaseTest<modules::Verse, modules::ModelVerse, db::TranslationStorage>;
    template class BaseTest<modules::Verse, modules::ModelChapter, db::TranslationStorage>;
    template class BaseTest<modules::Comment, modules::ModelComment, db::TranslationCommentStorage>;
    template class BaseTest<modules::Record, modules::ModelRecord>;
}  // namespace tests
