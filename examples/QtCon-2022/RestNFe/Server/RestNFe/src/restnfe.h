#ifndef RESTNFE_H
#define RESTNFE_H

#include <Cutelyst/Application>

using namespace Cutelyst;

class RestNFe : public Application
{
    Q_OBJECT
    CUTELYST_APPLICATION(IID "RestNFe")
public:
    Q_INVOKABLE explicit RestNFe(QObject *parent = nullptr);
    ~RestNFe();

    bool init();
};

#endif //RESTNFE_H

