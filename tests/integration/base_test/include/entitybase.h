#ifndef ENTITYBASE_H
#define ENTITYBASE_H

#include <QtTest>

namespace tests {

    template <class T>
    class EntityBase : public QObject
    {
    public:
        EntityBase();
        ~EntityBase();

    protected:
        virtual void constructor();
    };

}

#endif // ENTITYBASE_H
