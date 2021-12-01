#ifndef TST_MODULE_H
#define TST_MODULE_H

#include <QtTest>
#include "module.h"
#include "entitybase.h"

namespace modules {

    namespace tests {

        class tst_Module : public ::tests::EntityBase<Module> {
            Q_OBJECT
        public:
            tst_Module();
            ~tst_Module();

        private:
            Module helperGetModule();

        private slots:
            void constructor_data();
            void constructor() override;
            void convertSize_data();
            void convertSize();
            void m_languageShow_data();
            void m_languageShow();
        };
    }
}


#endif // TST_MODULE_H
