#include "entitybase.h"
#include "host.h"
#include "invaliddata.h"
#include "module.h"
#include "registry.h"

Q_DECLARE_METATYPE(modules::Registry)
Q_DECLARE_METATYPE(modules::Host)
Q_DECLARE_METATYPE(modules::Module)

namespace tests {

    template<class T>
    EntityBase<T>::EntityBase() {}

    template<class T>
    EntityBase<T>::~EntityBase() {}

    template<class T>
    void EntityBase<T>::constructor() {
        QFETCH(QJsonObject, json_object);
        QFETCH(T, object);
        QFETCH(bool, except);

        if(except) {
#if QT_VERSION > QT_VERSION_CHECK(6, 3, 0)
            QVERIFY_THROWS_EXCEPTION(T{json_object}, core::InvalidData);
#else
            QVERIFY_EXCEPTION_THROWN(T{json_object}, core::InvalidData);
#endif
        } else {
            QCOMPARE(T{json_object}, object);
        }
    }

    template class EntityBase<modules::Module>;
    template class EntityBase<modules::Host>;
    template class EntityBase<modules::Registry>;

}  // namespace tests
