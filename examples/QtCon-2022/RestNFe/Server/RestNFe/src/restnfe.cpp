#include "restnfe.h"

#include "root.h"

using namespace Cutelyst;

RestNFe::RestNFe(QObject *parent) : Application(parent)
{
}

RestNFe::~RestNFe()
{
}

bool RestNFe::init()
{
    new Root(this);

    return true;
}

