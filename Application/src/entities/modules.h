#ifndef MODULES_H
#define MODULES_H

class QString;
class QJsonObject;

class Modules
{
public:
    Modules();
    Modules(QJsonObject qJsonModule);
    int size() const;
private:
    void sizeToInt(const QString& str);
    double m_size = 0; // WARNING: int or double
};

#endif // MODULES_H
