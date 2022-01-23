#ifndef TST_REGISTRY_H
#define TST_REGISTRY_H

#include <QtTest>
#include "registry.h"
#include "entitybase.h"

namespace modules {

    namespace tests {

        class tst_Registry : public ::tests::EntityBase<Registry> {
            Q_OBJECT

        public:
            tst_Registry();
            ~tst_Registry();

        private:
            Registry helperGetObject();

        private slots:
            void constructor_data();
            void constructor() override;
            void urlToQUrl();
            void infoUrlToQUrl();
        };
    }
}


#endif // TST_REGISTRY_H
