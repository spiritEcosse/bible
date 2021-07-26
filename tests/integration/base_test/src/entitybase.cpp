#include "entitybase.h"
#include "invaliddata.h"
#include "registry.h"
#include "host.h"
#include "module.h"

Q_DECLARE_METATYPE(modules::Registry)
Q_DECLARE_METATYPE(modules::Host)
Q_DECLARE_METATYPE(modules::Module)

namespace tests {

    template <class T>
    EntityBase<T>::EntityBase() {}

    template <class T>
    EntityBase<T>::~EntityBase() {}

    template <class T>
    void EntityBase<T>::constructor()
    {
        QFETCH(QJsonObject, json_object);
        QFETCH(T, object);
        QFETCH(bool, except);

        if (except) {
            QVERIFY_EXCEPTION_THROWN(T {json_object}, core::InvalidData);
        } else {
            QCOMPARE(T {json_object}, object);
        }
    }

    template class EntityBase<modules::Module>;
    template class EntityBase<modules::Host>;
    template class EntityBase<modules::Registry>;

}
