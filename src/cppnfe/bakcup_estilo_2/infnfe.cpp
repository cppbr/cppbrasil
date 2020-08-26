#include "infnfe.h"

InfNFe::InfNFe(QObject *parent) : QObject(parent)
{

}

QString InfNFe::get_versao() const
{
  return this->m_versao;
}

void InfNFe::set_versao(const QString &versao)
{
  this->m_versao = versao;
}

QString InfNFe::get_Id() const
{
  return this->m_Id;
}

void InfNFe::set_Id(const QString &id)
{
  this->m_Id = id;
}
