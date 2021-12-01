#ifndef TST_GROUPMODULES_H
#define TST_GROUPMODULES_H

#include <QtTest>

namespace modules {

    namespace tests {

        class tst_GroupModules : public QObject
        {
            Q_OBJECT

        public:
            tst_GroupModules();
            ~tst_GroupModules();

        private slots:
            void m_name();
            void m_name_data();
            void language();
            void language_data();
            void m_region();
            void m_region_data();
            void getLanguageName_data();
            void getLanguageName();
            void setLanguageName_data();
            void setLanguageName();
        };

    }

}


#endif // TST_GROUPMODULES_H
