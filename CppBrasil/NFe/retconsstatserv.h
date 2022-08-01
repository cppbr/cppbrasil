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


#ifndef RETCONSSTATSERV_H
#define RETCONSSTATSERV_H

#include <QString>
#include <QDateTime>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/cpputility.h>
#include <CppBrasil/NFe/confignfe.h>
#include <CppBrasil/NFe/convnf.h>


class CPPNFE_EXPORT RetConsStatServ
{
    friend class WSStatus;
public:
    RetConsStatServ();
    ~RetConsStatServ();
    void clear();
    QString get_versao() const;
    TpAmb get_tpAmb() const;
    QString get_verAplic() const;
    int get_cStat() const;
    QString get_xMotivo() const;
    int get_cUF() const;
    QDateTime get_dhRecbto() const;
    int get_tMed() const;
    QDateTime get_dhRetorno() const;
    QString get_xObs() const;
protected:
    void set_versao(const QString &versao);
    void set_tpAmb(const TpAmb &tpAmb);
    void set_verAplic(const QString &verAplic);
    void set_cStat(const int &cStat);
    void set_xMotivo(const QString &xMotivo);
    void set_cUF(const int &cUF);
    void set_dhRecbto(const QDateTime &dhRecbto);
    void set_tMed(const int &tMed);
    void set_dhRetorno(const QDateTime &dhRetorno);
    void set_xObs(const QString &xObs);
private:
    QString m_versao;
    TpAmb m_tpAmb;
    QString m_verAplic;
    int m_cStat;
    QString m_xMotivo;
    int m_cUF;
    QDateTime m_dhRecbto;
    int m_tMed;
    QDateTime m_dhRetorno;
    QString m_xObs;

};


#endif // RETCONSSTATSERV_H
