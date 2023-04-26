#ifndef ROOT_H
#define ROOT_H

#include <Cutelyst/Controller>

using namespace Cutelyst;

class Root : public Controller
{
    Q_OBJECT
    C_NAMESPACE("")
public:
    explicit Root(QObject *parent = nullptr);
    ~Root();

    C_ATTR(nota, :Local :AutoArgs :ActionClass(REST))
    void nota(Context *c) {};

    C_ATTR(nota_POST, :Private)
    void nota_POST(Context *c);

    C_ATTR(index, :Path :AutoArgs)
    void index(Context *c);

    C_ATTR(defaultPage, :Path)
    void defaultPage(Context *c);



private:
    C_ATTR(End, :ActionClass("RenderView"))
    void End(Context *c) { Q_UNUSED(c); }
};

#endif //ROOT_H

