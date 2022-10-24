#include "statusservico.h"

StatusServico::StatusServico(ConfigNFe *confgNFe) :
   retorno(new RetConsStatServ()), config(confgNFe)
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
{   //Obs: Como se trata de um xml pequeno que não precisa de assinatura, será gerado na classe WSStatus.

    WSStatus* _ws = new WSStatus(this->config,
                           this->config->certificado->get_cryptoLib(),
                           this->retorno.get());

    connect(_ws, SIGNAL(wsChange(WebServicesNF)), this, SIGNAL(wsChange(WebServicesNF)));
    connect(_ws, SIGNAL(errorOccurred(QString)), this, SIGNAL(errorOccurred(QString)));

    bool _ret = _ws->send(ConvNF::versaoNFToStr(this->config->get_VersaoNF())); //o layout acompanha a versão da nota fiscal

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
