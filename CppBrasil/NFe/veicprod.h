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


#ifndef VEICPROD_H
#define VEICPROD_H

#include <QString>
#include <CppBrasil/cppbrasil_global.h>
#include <CppBrasil/NFe/convnf.h>

class CPPNFE_EXPORT VeicProd
{
public:
    VeicProd();
    ~VeicProd();
    void clear();
    TpOp get_tpOp() const;
    void set_tpOp(const TpOp &tpOp);
    QString get_chassi() const;
    void set_chassi(const QString &chassi);
    QString get_cCor() const;
    void set_cCor(const QString &cCor);
    QString get_xCor() const;
    void set_xCor(const QString &xCor);
    QString get_pot() const;
    void set_pot(const QString &pot);
    QString get_cilin() const;
    void set_cilin(const QString &cilin);
    QString get_pesoL() const;
    void set_pesoL(const QString &pesoL);
    QString get_pesoB() const;
    void set_pesoB(const QString &pesoB);
    QString get_nSerie() const;
    void set_nSerie(const QString &nSerie);
    QString get_tpComb() const;
    void set_tpComb(const QString &tpComb);
    QString get_nMotor() const;
    void set_nMotor(const QString &nMotor);
    QString get_CMT() const;
    void set_CMT(const QString &CMT);
    QString get_dist() const;
    void set_dist(const QString &dist);
    int get_anoMod() const;
    void set_anoMod(const int &anoMod);
    int get_anoFab() const;
    void set_anoFab(const int &anoFab);
    QString get_tpPint() const;
    void set_tpPint(const QString &tpPint);
    QString get_tpVeic() const;
    void set_tpVeic(const QString &tpVeic);
    QString get_espVeic() const;
    void set_espVeic(const QString &espVeic);
    QString get_VIN() const;
    void set_VIN(const QString &VIN);
    CondVeic get_condVeic() const;
    void set_condVeic(const CondVeic &condVeic);
    QString get_cMod() const;
    void set_cMod(const QString &cMod);
    QString get_cCorDENATRAN() const;
    void set_cCorDENATRAN(const QString &cCorDENATRAN);
    int get_lota() const;
    void set_lota(const int &lota);
    TpRest get_tpRest() const;
    void set_tpRest(const TpRest &tpRest);

private:
    TpOp m_tpOp;
    QString m_chassi;
    QString m_cCor;
    QString m_xCor;
    QString m_pot;
    QString m_cilin;
    QString m_pesoL;
    QString m_pesoB;
    QString m_nSerie;
    QString m_tpComb;
    QString m_nMotor;
    QString m_CMT;
    QString m_dist;
    int m_anoMod;
    int m_anoFab;
    QString m_tpPint;
    QString m_tpVeic;
    QString m_espVeic;
    QString m_VIN;
    CondVeic m_condVeic;
    QString m_cMod;
    QString m_cCorDENATRAN;
    int m_lota;
    TpRest m_tpRest;
};

#endif // VEICPROD_H
