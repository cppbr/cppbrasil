/*
Copyright 2022 João Elson
jetmorju@gmail.com

Web-site: https://cppbrasil.com.br

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include "envevento.h"
#include "urlnfe.h"
#include <QSet>

EnvEvento::EnvEvento(ConfigNFe *confgNFe, NotaFiscal *notafiscal): evento(new Container<TEvento>),
   retorno(new RetEnvEvento()), config(confgNFe), notafiscal(notafiscal)
{
}

EnvEvento::~EnvEvento()
{
}

void EnvEvento::clear()
{
    this->evento->clear();
    this->retorno->clear();
    this->m_error.clear();
    this->m_xml.clear();
    this->m_versao.clear();
}

bool EnvEvento::enviarEvento(const int &idLote)
{
    retorno->clear();
    m_error.clear();
//salvarNotaVinculada serve para salvar notas onde a nota em questão foi emitida pelo próprio
//transmissor do evento.
    bool _ret = true;
    QString _servidor;
    if (!get_servidorEvento(_servidor))
    {
        emit errorOccurred(this->m_error);
        return false;
    }

    if (gerarXML(idLote))
    {
        //validar xml
        if (validarEvento())
        {
            //envia
            std::unique_ptr<WSEvento> _ws(new WSEvento(this->config,
                                                    this->config->certificado->get_cryptoLib(),
                                                    this->retorno.get()));

            connect(_ws.get(), &WSEvento::wsChange, this, &EnvEvento::wsChange);
            connect(_ws.get(), &WSEvento::errorOccurred, this, &EnvEvento::onError);


            _ret = _ws->send(this->m_xml.toUtf8(),
                                  ConvNF::versaoLayout(WebServicesNF::NFeRecepcaoEvento),
                                  _servidor);

            if (_ret)
            {
              tratarRetorno();
              if (!m_error.isEmpty())
              {
                  emit errorOccurred(m_error);
                  _ret = false;
              }
            }
        }
        else
        {
            emit errorOccurred(this->m_error);
            _ret = false;
        }
    }
    else
    {
        emit errorOccurred(this->m_error);
        _ret = false;
    }

    return _ret;
}

QString EnvEvento::get_versao() const
{
    return this->m_versao;
}

void EnvEvento::set_versao(const QString &versao)
{
    this->m_versao = versao;
}

QString EnvEvento::get_error() const
{
    return this->m_error;
}

void EnvEvento::onError(const QString &error)
{
    this->m_error = error;
    emit errorOccurred(error);
}

bool EnvEvento::assinarEvento(const QString &evento, const QString &uri, QString &output)
{
    HashType _sha1 = HashType::sha1;
    QByteArray _xmldoc, _schema, _nodedoc, _nodesign, _uri, _xml_assinado;
    XMLReturnType _returntype = XMLReturnType::XMLSign; //retornar xml assinado
    bool _ret = true;

    _xmldoc.append(evento.toUtf8());
    _schema.append(config->arquivos->get_caminhoSchema().toLocal8Bit());
    _nodedoc.append("infEvento");
    _nodesign.append("SignedInfo");
    _uri.append(uri.toLocal8Bit());

    std::unique_ptr<CppXML> _libxml(new CppLibXml2(config->certificado->get_cryptoLib(), _schema));

    //assina o xml
    if (_libxml->signXML(_xmldoc, _sha1, _nodedoc, _nodesign, _uri,
                         _returntype, _xml_assinado))
    {
        output.append(_xml_assinado);
    }
    else
    {
        _ret = false;
        //retorna o erro ao invés do xml assinado
        output = _libxml->get_error();
    }

    return _ret;
}

bool EnvEvento::gerarXML(const int &idLote)
{
    bool _ret = true;
    QString _evento;
    this->m_error.clear();
    this->m_xml.clear();
    bool prorrogacao = false;
    bool rtc = false;
    int epec = 0;
    int entregas = 0;
    int orgaoEntrega = 0;
    for (const TEvento *item : *evento)
    {
        const int tipo = static_cast<int>(item->infEvento->get_tpEvento());
        if (item->infEvento->get_tpEvento() == TpEvento::Cancelamento ||
            item->infEvento->get_tpEvento() == TpEvento::CancelamentoSubstituicao)
        {
            m_error = "Eventos 110111 e 110112 devem ser enviados por EventoNotaFiscal::cancNFe.";
            return false;
        }
        if (tipo >= 411500 && tipo <= 411503)
        {
            m_error = "Eventos 411500 a 411503 são exclusivos do Fisco e não podem ser enviados pelo contribuinte.";
            return false;
        }
        prorrogacao = prorrogacao || (tipo >= 111500 && tipo <= 111503);
        if (item->infEvento->get_tpEvento() == TpEvento::EPEC)
            ++epec;
        rtc = rtc || !ConvNF::descricaoEventoRTC(item->infEvento->get_tpEvento()).isEmpty();
        const int orgao = ConvNF::orgaoEventoEntrega(item->infEvento->get_tpEvento());
        if (orgao)
        {
            if (orgaoEntrega && orgaoEntrega != orgao)
            {
                m_error = "Comprovante e insucesso na entrega usam autorizadores distintos.";
                return false;
            }
            orgaoEntrega = orgao;
            ++entregas;
        }
    }
    if (entregas)
    {
        if (entregas != evento->count() || entregas > 20 || idLote < 0 ||
            config->get_ModeloDF() != ModeloDF::NFe || (!m_versao.isEmpty() && m_versao != "1.00"))
        {
            m_error = "Eventos de entrega exigem lote separado, modelo 55, versão 1.00, idLote não negativo e até 20 eventos.";
            return false;
        }
        for (const TEvento *item : *evento)
        {
            const auto ambiente = item->infEvento->get_tpAmb();
            if (ambiente != TpAmb::None && ambiente != config->webservices->get_tpAmb())
            {
                m_error = "O ambiente do evento de entrega deve ser igual ao ambiente configurado.";
                return false;
            }
        }
    }
    if (rtc)
    {
        if (evento->count() != 1 || idLote < 0 || (!m_versao.isEmpty() && m_versao != "1.00"))
        {
            m_error = "Eventos da RTC exigem envio individual, versão 1.00 e idLote não negativo.";
            return false;
        }
        const auto &inf = *evento->value(0)->infEvento;
        const bool autorizadorDaNota = ConvNF::usaAutorizadorDaNota(inf);
        const QString modelo = inf.get_chNFe().mid(20, 2);
        const auto ambiente = config->webservices->get_tpAmb();
        const int uf = ConvNF::ufToInt(config->webservices->get_uf());
        if (modelo != ConvNF::modeloDFToStr(config->get_ModeloDF()) ||
            (modelo != "55" && !(modelo == "65" && autorizadorDaNota)))
            m_error = "A chave e o modelo configurado devem corresponder ao modelo aceito pelo evento.";
        else if (inf.get_tpAmb() != TpAmb::None && inf.get_tpAmb() != ambiente)
            m_error = "O ambiente do evento deve ser igual ao ambiente configurado.";
        else if (autorizadorDaNota && inf.get_chNFe().left(2).toInt() != uf)
            m_error = "Configure a UF da nota para a vinculação de pagamento ou seu cancelamento.";
        if (!m_error.isEmpty())
            return false;
    }
    if (epec > 0)
    {
        if (epec != evento->count() || config->get_ModeloDF() != ModeloDF::NFe ||
            idLote < 0 || epec > 20 || (!m_versao.isEmpty() && m_versao != "1.00"))
        {
            m_error = "EPEC exige lote separado, modelo 55, versão 1.00, idLote não negativo e até 20 eventos.";
            return false;
        }
        QSet<QString> chaves;
        for (const TEvento *item : *evento)
        {
            const auto *inf = item->infEvento.get();
            if (inf->get_tpAmb() != TpAmb::None && inf->get_tpAmb() != config->webservices->get_tpAmb())
            {
                m_error = "O ambiente do EPEC deve ser igual ao ambiente configurado.";
                return false;
            }
            if (chaves.contains(inf->get_chNFe()))
            {
                m_error = "O lote aceita um EPEC por chave de acesso.";
                return false;
            }
            chaves.insert(inf->get_chNFe());
        }
    }
    if (prorrogacao)
    {
        if (config->get_ModeloDF() != ModeloDF::NFe || idLote < 0 || evento->count() > 20 ||
            (!m_versao.isEmpty() && m_versao != "1.00"))
        {
            m_error = "Prorrogação exige modelo 55, versão 1.00, idLote não negativo e até 20 eventos por lote.";
            return false;
        }
        for (const TEvento *item : *evento)
        {
            const int tipo = static_cast<int>(item->infEvento->get_tpEvento());
            if (tipo < 111500 || tipo > 111503 ||
                item->infEvento->get_chNFe().left(2).toInt() != ConvNF::ufToInt(config->webservices->get_uf()) ||
                (item->infEvento->get_tpAmb() != TpAmb::None &&
                 item->infEvento->get_tpAmb() != config->webservices->get_tpAmb()))
            {
                m_error = "Envie prorrogação em lote separado, com UF e ambiente iguais aos configurados.";
                return false;
            }
        }
    }
    if (!config->certificado->get_cryptoLib())
    {
        m_error = "Carregue o certificado digital antes de enviar eventos.";
        return false;
    }

    //versão - Caso não seja informado, será 1.00 que é a versão do momento.
    if (this->m_versao.isEmpty())
      this->m_versao = "1.00";

    for (int i = 0; i < evento->count(); ++i)
    {
        //Só pode gerar o xml depois de fazer a atualização dos eventos, devido a alguns
        //campos que necessitam de ajustes.
        updateEvento(i);
        evento->value(i)->gerarXML();
        if (evento->value(i)->get_error().isEmpty())
            evento->value(i)->assinarXML(this->config);
        //assina xml
        if (evento->value(i)->get_error().isEmpty())
        {
            _evento.append(CppUtility::extractStr(evento->value(i)->get_XMLAssinado(), "<evento", "/evento>"));
        }
        else
        {
            //o erro será retornado em output
            this->m_error += "Evento: " + QString::number(i) + " - " +
                             evento->value(i)->get_error();
            _ret = false;
        }
    }
    //só completa o xml caso não tenha ocorrido erro.
    if (_ret)
    {
        this->m_xml += "<envEvento xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\""+ this->m_versao +"\">";
        this->m_xml += "<idLote>" + CppUtil::insZeroLeft(QString::number(idLote),15) + "</idLote>";
        this->m_xml += _evento;
        this->m_xml += "</envEvento>" ;
    }

    return _ret;
}


void EnvEvento::updateEvento(int &index)
{
    const TpEvento tipo = evento->value(index)->infEvento->get_tpEvento();
    if (tipo == TpEvento::EPEC)
    {
        auto *inf = evento->value(index)->infEvento.get();
        inf->set_cOrgao(91);
        inf->set_verEvento("1.00");
        inf->detEvento->set_descEvento("EPEC");
        if (inf->get_nSeqEvento() == 0)
            inf->set_nSeqEvento(1);
    }
    const int orgaoEntrega = ConvNF::orgaoEventoEntrega(tipo);
    if (orgaoEntrega)
    {
        auto &inf = *evento->value(index)->infEvento;
        inf.set_cOrgao(orgaoEntrega);
        inf.set_verEvento("1.00");
        inf.detEvento->set_cOrgaoAutor(inf.get_chNFe().left(2).toInt());
        inf.detEvento->set_descEvento(ConvNF::descricaoEventoEntrega(tipo));
    }
    if (!ConvNF::descricaoManifestacao(tipo).isEmpty())
    {
        auto *inf = evento->value(index)->infEvento.get();
        inf->set_cOrgao(91);
        inf->set_verEvento("1.00");
    }
    if (tipo == TpEvento::PedidoProrrogacao1 || tipo == TpEvento::PedidoProrrogacao2 ||
        tipo == TpEvento::CancelPedidoProrrogacao1 || tipo == TpEvento::CancelPedidoProrrogacao2)
    {
        auto *inf = evento->value(index)->infEvento.get();
        inf->set_cOrgao(ConvNF::ufToInt(config->webservices->get_uf()));
        inf->set_verEvento("1.00");
        inf->detEvento->set_descEvento(
            tipo == TpEvento::PedidoProrrogacao1 || tipo == TpEvento::PedidoProrrogacao2
            ? "Pedido de Prorrogacao" : "Cancelamento de Pedido de Prorrogacao");
    }
    if (tipo == TpEvento::AtorInteressadoNFeTransp)
    {
        auto *inf = evento->value(index)->infEvento.get();
        inf->set_cOrgao(91);
        inf->set_verEvento("1.00");
        inf->detEvento->set_descEvento("Ator interessado na NF-e");
        if (inf->detEvento->get_tpAutorizacao() == TpAutorizacao::PermiteTranspAutTransp &&
            inf->detEvento->get_xCondUso().isEmpty())
            inf->detEvento->set_xCondUso("O emitente ou destinatário da NF-e, declara que permite o transportador declarado no campo CNPJ/CPF deste evento a autorizar os transportadores subcontratados ou redespachados a terem acesso ao download da NF-e");
    }
    if (tipo == TpEvento::ECONF || tipo == TpEvento::CancelECONF)
    {
        evento->value(index)->infEvento->set_verEvento("1.00");
        evento->value(index)->infEvento->detEvento->set_descEvento(
            tipo == TpEvento::ECONF ? "ECONF" : "Cancelamento Conciliação Financeira");
        if (config->get_ModeloDF() == ModeloDF::NFe)
            evento->value(index)->infEvento->set_cOrgao(92);
        else
        {
            const QString url = UrlNFe::get_urlServidor(config->webservices->get_uf(),
                config->webservices->get_tpAmb(), TpEmis::Normal, ModeloDF::NFCe,
                config->get_VersaoNF(), WebServicesNF::NFeRecepcaoEvento);
            const QString urlSVRS = UrlNFe::get_urlServidor(config->webservices->get_uf(),
                config->webservices->get_tpAmb(), TpEmis::Normal, ModeloDF::NFCe,
                config->get_VersaoNF(), WebServicesNF::NFeRecepcaoEvento, "SVRS");
            evento->value(index)->infEvento->set_cOrgao(!urlSVRS.isEmpty() && url == urlSVRS
                ? 92 : ConvNF::ufToInt(config->webservices->get_uf()));
        }
    }
    if (!ConvNF::descricaoEventoRTC(tipo).isEmpty())
    {
        auto &inf = *evento->value(index)->infEvento;
        auto &det = *inf.detEvento;
        const int uf = ConvNF::ufToInt(config->webservices->get_uf());
        if (inf.get_cOrgao() == 0)
        {
            // No cancelamento, a NT informa a UF no envelope, inclusive no envio à SVRS.
            const bool usarUF = ConvNF::usaAutorizadorDaNota(inf) || tipo == TpEvento::CancelamentoEvento;
            inf.set_cOrgao(usarUF ? uf : 92);
        }
        if (det.get_cOrgaoAutor() == 0)
            det.set_cOrgaoAutor(uf);
        inf.set_verEvento("1.00");
        det.set_descEvento(ConvNF::descricaoEventoRTC(tipo));
    }
    //versão
    evento->value(index)->set_versao(this->m_versao);
    //nSeqEvento deve ser tratado pelo software do usuário e não pela lib, portanto,
    //se não for informado ocorrerá erro na valização.
    //caso seja maior que 0 e menor que 10, será acrescido zero a esquerda.
    //nSeqEvento deve ter dois algarismos, para não ocorrer erro no ID.
    QString _strSeq;
    if ((evento->value(index)->infEvento->get_nSeqEvento() > 0) &&
        (evento->value(index)->infEvento->get_nSeqEvento() < 10))
    {
        _strSeq = CppUtil::insZeroLeft(QString::number(evento->value(index)->infEvento->get_nSeqEvento()), 2);
    }
    else
    {
        _strSeq = QString::number(evento->value(index)->infEvento->get_nSeqEvento());
    }
    QString _id = "ID" + ConvNF::tpEventoToStr(evento->value(index)->infEvento->get_tpEvento())
                 + evento->value(index)->infEvento->get_chNFe()
                 + _strSeq;

    evento->value(index)->infEvento->set_Id(_id);

    //se não informou o ambiente, será o padrão de configurações
    if (evento->value(index)->infEvento->get_tpAmb() == TpAmb::None)
        evento->value(index)->infEvento->set_tpAmb(config->webservices->get_tpAmb());
}


bool EnvEvento::get_servidorEvento(QString &servidor)
{
    servidor.clear();
    if (!evento->isEmpty())
    {
        const TpEvento primeiro = evento->value(0)->infEvento->get_tpEvento();
        const auto &inf = *evento->value(0)->infEvento;
        if (!ConvNF::descricaoEventoRTC(primeiro).isEmpty())
        {
            if (!ConvNF::usaAutorizadorDaNota(inf))
                servidor = "SVRS";
            return true;
        }
        if (const int orgao = ConvNF::orgaoEventoEntrega(primeiro))
        {
            servidor = orgao == 91 ? "AN" : "SVRS";
            return true;
        }
        if (primeiro == TpEvento::EPEC)
        {
            servidor = "AN";
            return true;
        }
        int manifestacoes = 0;
        for (const TEvento *item : *evento)
            if (!ConvNF::descricaoManifestacao(item->infEvento->get_tpEvento()).isEmpty())
                ++manifestacoes;
        if (manifestacoes > 0)
        {
            if (manifestacoes != evento->count())
            {
                m_error = "Envie manifestações do destinatário em lote separado dos demais eventos.";
                return false;
            }
            if (config->get_ModeloDF() != ModeloDF::NFe || manifestacoes > 20 ||
                (!m_versao.isEmpty() && m_versao != "1.00"))
            {
                m_error = "Manifestação do destinatário exige modelo 55, versão 1.00 e até 20 eventos por lote.";
                return false;
            }
            for (const TEvento *item : *evento)
            {
                const TpAmb ambiente = item->infEvento->get_tpAmb();
                if (ambiente != TpAmb::None && ambiente != config->webservices->get_tpAmb())
                {
                    m_error = "O ambiente da manifestação deve ser igual ao ambiente configurado.";
                    return false;
                }
            }
            servidor = "AN";
            return true;
        }
        bool temAtor = false;
        for (const TEvento *item : *evento)
            temAtor = temAtor || item->infEvento->get_tpEvento() == TpEvento::AtorInteressadoNFeTransp;
        if (temAtor)
        {
            if (config->get_ModeloDF() != ModeloDF::NFe)
            {
                m_error = "Ator interessado é um evento exclusivo da NF-e modelo 55.";
                return false;
            }
            for (const TEvento *item : *evento)
                if (item->infEvento->get_tpEvento() != TpEvento::AtorInteressadoNFeTransp)
                {
                    m_error = "Envie Ator interessado em lote separado dos demais tipos de evento.";
                    return false;
                }
            servidor = "AN";
            return true;
        }
        bool temEconf = false;
        for (const TEvento *item : *evento)
            temEconf = temEconf || item->infEvento->get_tpEvento() == TpEvento::ECONF ||
                       item->infEvento->get_tpEvento() == TpEvento::CancelECONF;
        if (temEconf)
        {
            for (const TEvento *item : *evento)
            {
                if (item->infEvento->get_tpEvento() != primeiro)
                {
                    m_error = "Envie ECONF e cancelamento de ECONF em lotes separados por tipo de evento.";
                    return false;
                }
            }
            if (config->get_ModeloDF() == ModeloDF::NFe)
                servidor = "SVRS";
            return true;
        }
    }
    return true;
}

bool EnvEvento::validarEvento()
{
    bool _ret = true;
    QByteArray _schemaPath(config->arquivos->get_caminhoSchema().toLocal8Bit());
    QByteArray _schemaName;
    if (!evento->isEmpty() && evento->value(0)->infEvento->get_tpEvento() == TpEvento::ECONF)
        _schemaName.append("envEventoEConf_v");
    else if (!evento->isEmpty() && evento->value(0)->infEvento->get_tpEvento() == TpEvento::CancelECONF)
        _schemaName.append("envEventoCancEConf_v");
    else if (!evento->isEmpty() && evento->value(0)->infEvento->get_tpEvento() == TpEvento::AtorInteressadoNFeTransp)
        _schemaName.append("envEventoAtorInteressado_v");
    else
        _schemaName.append("envEvento_v");
    _schemaName.append(this->m_versao.toLocal8Bit());
    _schemaName.append(".xsd");
    std::unique_ptr<CppXML> _libxml(new CppLibXml2(config->certificado->get_cryptoLib(), _schemaPath));

    this->m_error.clear(); //limpando erro
    if (!_libxml->parseXML(this->m_xml.toUtf8(), _schemaName))
    {
       this->m_error = _libxml->get_error();
        _ret = false;
    }

    if (_ret)
        for (TEvento *item : *evento)
        {
            const int tipo = static_cast<int>(item->infEvento->get_tpEvento());
            if ((!ConvNF::descricaoEventoRTC(item->infEvento->get_tpEvento()).isEmpty() && !item->validarDetalheRTC(config)) ||
                (tipo >= 111500 && tipo <= 111503 && !item->validarProrrogacao(config)))
            {
                m_error = item->get_error();
                return false;
            }
        }

    return _ret;
}

void EnvEvento::tratarRetorno()
{
    const bool loteEntrega = !evento->isEmpty() && ConvNF::orgaoEventoEntrega(evento->value(0)->infEvento->get_tpEvento());
    if (loteEntrega)
    {
        if (retorno->get_cStat() != 128)
            return;
        if (retorno->retEvento->count() != evento->count())
        {
            m_error = "A quantidade de retornos do lote de entrega diverge da quantidade enviada.";
            return;
        }
        QSet<QString> enviados;
        for (const auto *item : *evento)
            enviados.insert(item->infEvento->get_Id());
        for (const auto *item : *retorno->retEvento)
        {
            const auto &inf = *item->infEvento;
            if (inf.get_cStat() != 135 && inf.get_cStat() != 136)
                continue;
            const QString id = "ID" + ConvNF::tpEventoToStr(inf.get_tpEvento()) + inf.get_chNFe() +
                               QString::number(inf.get_nSeqEvento()).rightJustified(2, '0');
            if (!enviados.remove(id) || inf.get_tpAmb() != config->webservices->get_tpAmb() ||
                inf.get_nProt().isEmpty())
            {
                m_error = "A confirmação de entrega está duplicada ou diverge do evento enviado, ambiente ou protocolo.";
                return;
            }
        }
    }
    if (!evento->isEmpty())
    {
        const auto *inf = evento->value(0)->infEvento.get();
        const int tipo = static_cast<int>(inf->get_tpEvento());
        if (tipo >= 111500 && tipo <= 111503)
        {
            tratarRetornoProrrogacao();
            return;
        }
        if (!ConvNF::descricaoEventoRTC(inf->get_tpEvento()).isEmpty())
        {
            if (retorno->get_cStat() != 128)
                return;
            if (retorno->retEvento->count() != 1)
            {
                m_error = "O lote processado da RTC deve retornar exatamente um evento.";
                return;
            }
            const auto &ret = *retorno->retEvento->value(0)->infEvento;
            if (ret.get_cStat() != 135 && ret.get_cStat() != 136)
                return;
            if (ret.get_tpEvento() != inf->get_tpEvento() || ret.get_chNFe() != inf->get_chNFe() ||
                ret.get_nSeqEvento() != inf->get_nSeqEvento())
            {
                m_error = "O retorno da RTC não corresponde ao evento enviado.";
                return;
            }
        }
    }
    //atualiza e salva o xml do evento
    QByteArray _schema = this->config->arquivos->get_caminhoSchema().toLocal8Bit();
    std::unique_ptr<CppXML> _libxml(new CppLibXml2(this->config->certificado->get_cryptoLib(), _schema));

    //unindo xml assinado com o retorno do web services
    if (this->retorno->retEvento->count() > 0)
    {
        for (int i = 0; i < this->retorno->retEvento->count(); ++i)
        {
            int _cStat = this->retorno->retEvento->value(i)->infEvento->get_cStat();

            for (int j = 0; j < evento->count(); ++j)
            {
                //verifica se é o mesmo evento
                if ((this->retorno->retEvento->value(i)->infEvento->get_chNFe() ==
                     this->evento->value(j)->infEvento->get_chNFe()) &&
                    (this->retorno->retEvento->value(i)->infEvento->get_nSeqEvento() ==
                     this->evento->value(j)->infEvento->get_nSeqEvento()) &&
                    (this->retorno->retEvento->value(i)->infEvento->get_tpEvento() ==
                     this->evento->value(j)->infEvento->get_tpEvento()))
                {
                    const TpEvento tipo = evento->value(j)->infEvento->get_tpEvento();
                    const bool manifestacao = !ConvNF::descricaoManifestacao(tipo).isEmpty();
                    const bool rtc = !ConvNF::descricaoEventoRTC(tipo).isEmpty();
                    const bool entrega = ConvNF::orgaoEventoEntrega(tipo) != 0;
                    if ((manifestacao || rtc || entrega) && (_cStat == 135 || _cStat == 136))
                    {
                        auto *ret = retorno->retEvento->value(i);
                        const auto *enviado = evento->value(j);
                        if (rtc && (ret->infEvento->get_tpAmb() != enviado->infEvento->get_tpAmb() ||
                                    ret->infEvento->get_nProt().isEmpty()))
                        {
                            m_error = "Retorno de evento com ambiente divergente ou sem protocolo.";
                            return;
                        }
                        const QString processado =
                            "<procEventoNFe xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\"1.00\">" +
                            CppUtility::extractStr(enviado->get_XMLAssinado(), "<evento", "/evento>") +
                            ret->get_XML() + "</procEventoNFe>";
                        ret->set_XML(processado);
                        if (config->arquivos->get_salvar())
                            ret->salvarXML(config->arquivos->get_caminhoEvento(),
                                           enviado->infEvento->get_Id().mid(2) + "-procEventoNFe");
                        break;
                    }
                    if (_cStat == 135 || (_cStat == 136 &&
                        tipo == TpEvento::AtorInteressadoNFeTransp))
                    {
                        QByteArray _procEventoNFe, _xmlOriginal, _retEvento;
                        _procEventoNFe.append(QString("<procEventoNFe xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao =\"" + get_versao() + "\">").toLocal8Bit());
                        _procEventoNFe.append("</procEventoNFe>");

                        _xmlOriginal.append("<a>");
                        _xmlOriginal.append(CppUtility::extractStr(this->evento->value(j)->get_XMLAssinado(), "<evento", "/evento>").toUtf8());
                        _xmlOriginal.append("</a>");

                        //xml do evento é inserido ao retornar do webservices.
                        _retEvento.append("<a>");
                        _retEvento.append(CppUtility::extractStr(this->retorno->retEvento->value(i)->get_XML(), "<retEvento", "/retEvento>").toUtf8());
                        _retEvento.append("</a>");

                        //primeiro o xml original dentro de procEventoNFe
                        if (_libxml->addChildXML(_procEventoNFe, _xmlOriginal, QByteArray("procEventoNFe")))
                        {
                            //agora o retEvento abaixo do xml original
                            if (_libxml->addChildXML(_procEventoNFe, _retEvento, QByteArray("procEventoNFe")))
                            {
                                //atualiza o xml de retEvento da classe RetEnvEvento com o xml completo
                                this->retorno->retEvento->value(i)->set_XML(_procEventoNFe);
                                //caso esteja configurado para salvar
                                if (this->config->arquivos->get_salvar())
                                {
                                    //O nome do arquivo deve ser o Id enviado, já que o Id retornado é o mesmo protocolo.
                                    QString _nomeArqEvento =  evento->value(j)->infEvento->get_Id().mid(3, evento->value(j)->infEvento->get_Id().length() - 3) +
                                                             "-procEventoNFe";
                                    this->retorno->retEvento->value(i)->salvarXML(this->config->arquivos->get_caminhoEvento(), _nomeArqEvento);
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

bool EnvEvento::tratarRetornoProrrogacao()
{
    m_error.clear();
    // 135 registra a solicitação; o deferimento vem em outro evento, emitido pelo Fisco.
    if (retorno->get_cStat() != 128) return true;
    CppLibXml2 xml(config->certificado->get_cryptoLib(), config->arquivos->get_caminhoSchema().toUtf8());
    for (RetEvento *ret : *retorno->retEvento)
    {
        if (ret->infEvento->get_cStat() != 135) continue;
        TEvento *enviado = nullptr;
        for (TEvento *item : *evento)
        {
            if (ret->infEvento->get_chNFe() == item->infEvento->get_chNFe() &&
                ret->infEvento->get_tpEvento() == item->infEvento->get_tpEvento() &&
                ret->infEvento->get_nSeqEvento() == item->infEvento->get_nSeqEvento() &&
                ret->infEvento->get_tpAmb() == item->infEvento->get_tpAmb())
            {
                enviado = item;
                break;
            }
        }
        if (!enviado)
        {
            m_error = "O retorno de prorrogação não corresponde ao evento enviado.";
            return false;
        }
        QByteArray processado = "<procEventoNFe xmlns=\"http://www.portalfiscal.inf.br/nfe\" versao=\"1.00\"></procEventoNFe>";
        const QByteArray original = ("<a>" + CppUtility::extractStr(enviado->get_XMLAssinado(), "<evento", "/evento>") + "</a>").toUtf8();
        const QByteArray resposta = ("<a>" + CppUtility::extractStr(ret->get_XML(), "<retEvento", "/retEvento>") + "</a>").toUtf8();
        if (!enviado->validarProrrogacao(config))
        {
            m_error = enviado->get_error();
            return false;
        }
        if (!xml.addChildXML(processado, original, "procEventoNFe") ||
            !xml.addChildXML(processado, resposta, "procEventoNFe") ||
            !xml.parseXML(processado, "procEventoNFe_v1.00.xsd"))
        {
            m_error = xml.get_error();
            return false;
        }
        ret->set_XML(QString::fromUtf8(processado));
        if (config->arquivos->get_salvar())
            ret->salvarXML(config->arquivos->get_caminhoEvento(), enviado->infEvento->get_Id().mid(2) + "-procEventoNFe");
    }
    return true;
}

