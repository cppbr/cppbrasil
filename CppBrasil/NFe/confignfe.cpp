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


#include "confignfe.h"

Certificado::Certificado()
{

}

Certificado::~Certificado()
{
}

//---------------------------------------------------------------

ConfigNFe::ConfigNFe(): certificado(new Certificado), arquivos(new ConfArquivos),
    webservices(new ConfWebServices)
{

}

ConfigNFe::~ConfigNFe()
{
}

ModeloDF ConfigNFe::get_ModeloDF() const
{
    return this->m_ModeloDF;
}

void ConfigNFe::set_ModeloDF(const ModeloDF &modeloDF)
{
    this->m_ModeloDF = modeloDF;
}

VersaoNF ConfigNFe::get_VersaoNF() const
{
    return this->m_VersaoNF;
}

void ConfigNFe::set_VersaoNF(const VersaoNF &VersaoNF)
{
    this->m_VersaoNF = VersaoNF;
}

QString ConfigNFe::get_IdCSC() const
{
    return this->m_IdCSC;
}

void ConfigNFe::set_IdCSC(const QString &IdCSC)
{
    this->m_IdCSC = IdCSC;
}

QString ConfigNFe::get_CSC() const
{
    return this->m_CSC;
}

void ConfigNFe::set_CSC(const QString &CSC)
{
    this->m_CSC = CSC;
}

VersaoQrCodeNF ConfigNFe::get_VersaoQrCodeNF() const
{
    return this->m_VersaoQRCodeNF;
}

void ConfigNFe::set_VersaoQrCodeNF(const VersaoQrCodeNF &versaoQRCodeNF)
{
    this->m_VersaoQRCodeNF = versaoQRCodeNF;

}

IndSinc ConfigNFe::get_indicadorSincrono() const
{
    return this->m_indSinc;
}

void ConfigNFe::set_indicadorSincrono(const IndSinc &indSinc)
{
    this->m_indSinc = indSinc;
}

TpEmis ConfigNFe::get_TipoEmissao() const
{
    return this->m_tipoEmissao;
}

void ConfigNFe::set_TipoEmissao(const TpEmis &tipoEmissao)
{
    this->m_tipoEmissao = tipoEmissao;
}

QString ConfigNFe::get_caminhoNF() const
{
    return get_caminho("nfe", arquivos->get_salvarPorCNPJ(), arquivos->get_salvarPorModelo(), arquivos->get_salvarPorAnoMesDia(), arquivos->get_salvarPorAnoMes(),
                       arquivos->get_salvarPorAno(), arquivos->get_salvarPorMes(), arquivos->get_salvarPorDia());
}

QString ConfigNFe::get_caminhoLogs() const
{
    return get_caminho("logs", false, false, false, false, false, false, false);
}

QString ConfigNFe::get_caminhoEvento() const
{
    //evento não deve salvar por documento, somente nfe/nfce
    return get_caminho("evento", arquivos->get_salvarPorCNPJ(), false, arquivos->get_salvarPorAnoMesDia(), arquivos->get_salvarPorAnoMes(),
                       arquivos->get_salvarPorAno(), arquivos->get_salvarPorMes(), arquivos->get_salvarPorDia());
}

QString ConfigNFe::get_caminho(const QString &pastaBase, const bool &porCNPJ, const bool &porModelo,
                               const bool &porAnoMesDia, const bool &porAnoMes, const bool &porAno,
                               const bool &porMes, const bool &porDia) const
{
    QString _path;
    QDir _dir;
    QDate _date = QDate::currentDate();

    _path = arquivos->get_caminhoSalvar();

    //verifica se tem barra no final
    if (_path.right(1) != "/")
      _path += "/";

    if (!CppUtility::parsePath(_path))
      return "";

    if (!pastaBase.isEmpty())
    {
        _path += (pastaBase + "/" );
        if (!CppUtility::parsePath(_path))
        {
            if (!_dir.mkdir(_path))
                return "";
        }
    }

    if (porCNPJ)
    {
        //unica forma de obter o cnpj sem ter que criar nenhum parametro nesta função é obter do
        //certificado digital.
        if (!certificado->get_cnpj().isEmpty())
        {
             _path += (certificado->get_cnpj() + "/");
             if (!CppUtility::parsePath(_path))
             {
                 if (!_dir.mkdir(_path))
                     return "";
             }
        }
    }

    if (porModelo)
    {
        _path += (ConvDF::modeloDFToStr(get_ModeloDF()) + "/");
        if (!CppUtility::parsePath(_path))
        {
            if (!_dir.mkdir(_path))
                return "";
        }
    }

    //quando escolher PorAnoMesDia, não pode existir outras datas separadas
    if (porAnoMesDia)
    {
        _path += (QString::number(_date.year()));
        _path += (QString("0" + QString::number(_date.month())).right(2));
        _path += (QString("0" + QString::number(_date.day())).right(2));
        _path += "/"; //fechamento
        if (!CppUtility::parsePath(_path))
        {
            if (!_dir.mkdir(_path))
                return "";
        }

    } else
    {
        //quando escolher PorAnoMes, somente o dia pode ser separado
        if (porAnoMes)
        {
            _path += (QString::number(_date.year()));
            _path += (QString("0" + QString::number(_date.month())).right(2));
            _path += "/"; //fechamento
            if (!CppUtility::parsePath(_path))
            {
                if (!_dir.mkdir(_path))
                    return "";
            }

        } else
        {
            //datas separadas.
            //ano separado
            if (porAno)
            {
              _path += (QString::number(_date.year()) + "/");
              if (!CppUtility::parsePath(_path))
              {
                  if (!_dir.mkdir(_path))
                      return "";
              }
            }
            //mes separado
            if (porMes)
            {
                //mes com 2 digitos
                _path += (QString("0" + QString::number(_date.month())).right(2) + "/");
                if (!CppUtility::parsePath(_path))
                {
                    if (!_dir.mkdir(_path))
                        return "";
                }
            }
        }
        // o dia pode ser separado quando escolher PorAnoMes, por isso, ficará fora da condição acima
        if (porDia)
        {
            //dia com 2 digitos
            _path += (QString("0" + QString::number(_date.day())).right(2) + "/");
            if (!CppUtility::parsePath(_path))
            {
                if (!_dir.mkdir(_path))
                    return "";
            }
        }
    }

    return _path;
}
