#ifndef TST_HOST_H
#define TST_HOST_H

#include "entitybase.h"
#include "host.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_Host : public ::tests::EntityBase<Host> {
            Q_OBJECT

          public:
            tst_Host();
            ~tst_Host();

          private:
            Host helperGetObject();

          private slots:
            void constructor_data();
            void constructor() override;
            void pathToQUrl();
        };
    }
}  // namespace modules

#endif  // TST_HOST_H
