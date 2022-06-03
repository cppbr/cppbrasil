#include "statusservico.h"

StatusServico::StatusServico(ConfigNFe *confgNFe) :
   retorno(new RetConsStatServ()), m_config(confgNFe)
{

}

StatusServico::~StatusServico()
{

}

void StatusServico::clear()
{
    this->retorno->clear();
}

bool StatusServico::get_status()
{

    WSStatus* _ws = new WSStatus(this->m_config,
                           this->m_config->certificado->get_cryptoLib(),
                           this->retorno.get());

    connect(_ws, SIGNAL(wsChange(WebServicesNF)), this, SIGNAL(wsChange(WebServicesNF)));
    connect(_ws, SIGNAL(errorOccurred(QString)), this, SIGNAL(errorOccurred(QString)));

    bool _ret = _ws->send();

    if (!_ws->get_error().isEmpty())
    {
      this->m_error = _ws->get_error();
    }
    delete _ws;
    return _ret;
}


QString StatusServico::get_error() const
{
     return this->m_error;
}
