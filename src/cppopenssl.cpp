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


#include "cppopenssl.h"
CppOpenSSL::CppOpenSSL(const QByteArray &p12path, const QByteArray &pass): m_p12path(p12path), m_pass(pass)
{
}

CppOpenSSL::~CppOpenSSL()
{
    //const char *m_p12path - ponteiro de fora
    //const char *m_pass - ponteiro de fora
    clear();
    if (this->m_pkey != NULL)
        EVP_PKEY_free(this->m_pkey);
    if (m_cert != NULL)
        X509_free(m_cert);
    if (m_ca != NULL)
        sk_X509_pop_free(m_ca, X509_free);
    //sk_X509_free(_ca); //metodo antigo, não deve ser utilizado para limpar a estrutura x509 ca
}

bool CppOpenSSL::digest_s(const HashType &hashtype, const QByteArray &input,
                          const bool returnBase64, QByteArray &output)
{
    return (hash(hashtype, input, NULL , false, returnBase64, output));
}

//------------------------------------------------------------------------
// CppCrypto class
//------------------------------------------------------------------------

bool CppOpenSSL::insertCAcert(const QString &certPath)
{
    if (m_pkey == NULL)
        return false;

    return insertCA(certPath.toLocal8Bit());

}

bool CppOpenSSL::digest(const HashType &hashtype, const QByteArray &input,
                        const bool returnBase64, QByteArray &output)
{
    bool _ret = (hash(hashtype, input, &this->m_pkey, false, returnBase64, output));
    if (_ret == false)
      set_error(QStringLiteral("Erro ao assinar documento"));

    return _ret;
}

bool CppOpenSSL::signature(const HashType &hashtype, const QByteArray &input,
                           const bool returnBase64, QByteArray &output)
{
    bool _ret = (hash(hashtype, input, &this->m_pkey, true, returnBase64, output));
    if (_ret == false)
      set_error(QStringLiteral("Erro ao assinar documento"));

    return _ret;
}

bool CppOpenSSL::load()
{
    return (loadOpenssl());
}

bool CppOpenSSL::unload()
{
    return (unloadOpenssl());
}

//------------------------------------------------------------------------
// CppOpenSSL class
//------------------------------------------------------------------------

bool CppOpenSSL::insertCA(const char *certPath)
{
    BIO *bio = NULL;
    bio = BIO_new_file(certPath, "r");
    if (bio == NULL)
    {
        return false;
    }

    X509 *cert = NULL;
    cert = PEM_read_bio_X509(bio, NULL, 0, NULL);
    if (cert == NULL)
    {
        BIO_free(bio);
        return false;
    }

    if (m_ca == NULL)
      m_ca = sk_X509_new_null();

    sk_X509_push(m_ca, X509_dup(cert));
    X509_free(cert);
    BIO_free(bio);

    certToDER();

    return true;
}

QDateTime CppOpenSSL::asn1_to_QDateTime(const ASN1_TIME *aTime)
{
    ASN1_GENERALIZEDTIME *_gt = ASN1_TIME_to_generalizedtime(aTime, NULL);
    QString _t = QString::fromLatin1((char*)_gt->data, _gt->length);
    QDate _resDate(QDate::fromString(_t.left(8), "yyyyMMdd"));
    QString _sTime = QString("%1%2%3%4%5").arg(_t.mid(8, 2), ":", _t.mid(10, 2),
                                             ":", _t.mid(12, 2));
    QTime _resTime(QTime::fromString(_sTime,"hh:mm:ss"));
    QDateTime _result(_resDate, _resTime, Qt::UTC);
    ASN1_GENERALIZEDTIME_free(_gt);
    return _result;
}

bool CppOpenSSL::certToBase64(X509 *cert)
{
    BIO *_bio = NULL;
    char *_bio_out = NULL;
    QByteArray _b64;
    bool _ret = true;
    int _len = 0;
    try{
        _bio = BIO_new(BIO_s_mem());
        if (i2d_X509_bio(_bio, cert))
        {
            _len = BIO_pending(_bio);
            _bio_out = (char*) calloc(_len + 1, sizeof(char));
            if (BIO_read(_bio, _bio_out, _len))
            {
                for(int i = 0; i < _len; i++)
                {
                  _b64.append(_bio_out[i]);
                }
                set_certBase64(QString(_b64.toBase64()));
                _b64.clear();
            }else
            {
                _ret = false;
            }
        }else
          _ret = false;

        if (_bio != NULL)
          BIO_free(_bio);
        if (_bio_out != NULL)
          free(_bio_out);
        return _ret;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        if (_bio != NULL)
          BIO_free(_bio);
        if (_bio_out != NULL)
          free(_bio_out);
        return false;
    }
}

bool CppOpenSSL::issuerName(X509 *cert)
{
     X509_NAME *_issuer = NULL;
     BIO *_bio = NULL;
     bool _ret = true;
     int _len = 0;
     char *_str_issuer = NULL;
     QString _res;
    try{
        _issuer = X509_get_issuer_name(cert);
        _bio = BIO_new(BIO_s_mem());
        if (X509_NAME_print_ex(_bio, _issuer, 0, XN_FLAG_RFC2253))
        {
            _len = BIO_pending(_bio);
            _str_issuer = (char*) calloc(_len + 1, sizeof(char));
            if (BIO_read(_bio, _str_issuer, _len))
            {
                for(int i = 0; i < _len; i++)
                {
                    _res.append(_str_issuer[i]);
                }
                set_issuerName(_res);
                _res.clear();
            }else
                _ret = false;
        }else
            _ret = false;

        if (_bio != NULL)
          BIO_free(_bio);
        if (_str_issuer != NULL)
          free(_str_issuer);
       return _ret;

     } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        if (_bio != NULL)
          BIO_free(_bio);
        if (_str_issuer != NULL)
          free(_str_issuer);
        return false;
    }
}

bool CppOpenSSL::serialNumber(X509 *cert)
{
    BIO *_bio = NULL;
    ASN1_INTEGER *_serial = NULL;
    bool _ret = true;
    int _len = 0;
    char *_str_serial = NULL;
    QString _res;
    try{
        _bio = BIO_new(BIO_s_mem());
        _serial = X509_get_serialNumber(cert);
        for(int i=0; i < _serial->length; i++) {
             BIO_printf(_bio, "%02x", _serial->data[i] );
        }
        _len = BIO_pending(_bio);
        _str_serial = (char*) calloc(_len + 1, sizeof(char));
        if (BIO_read(_bio, _str_serial, _len))
        {
            for(int i = 0; i < _len; i++)
            {
                _res.append(_str_serial[i]);
            }
            set_numeroSerie(_res);
            _res.clear();
        } else
            _ret = false;

        if (_bio != NULL)
          BIO_free(_bio);
        if (_str_serial != NULL)
          free(_str_serial);
        return _ret;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        if (_bio != NULL)
          BIO_free(_bio);
        if (_str_serial != NULL)
          free(_str_serial);
        return false;
    }
}

bool CppOpenSSL::subjectName(X509 *cert)
{
    X509_NAME *_subject = NULL;
    BIO *_bio = NULL;
    bool _ret = true;
    int _len = 0;
    char *_str_subject = NULL;
    QString _res;
    try{
        _subject = X509_get_subject_name(cert);
        _bio = BIO_new(BIO_s_mem());
        if (X509_NAME_print_ex(_bio, _subject, 0, XN_FLAG_RFC2253))
        {
            _len = BIO_pending(_bio);
            _str_subject = (char*) calloc(_len + 1, sizeof(char));
            if (BIO_read(_bio, _str_subject, _len))
            {
                for(int i = 0; i < _len; i++)
                {
                    _res.append(_str_subject[i]);
                }
                set_subjectName(_res);
                _res.clear();
            }else
                _ret = false;
        }else
            _ret = false;

        if (_bio != NULL)
          BIO_free(_bio);
        if (_str_subject != NULL)
          free(_str_subject);
        return _ret;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        if (_bio != NULL)
          BIO_free(_bio);
        if (_str_subject != NULL)
          free(_str_subject);
        return false;
    }
}

bool CppOpenSSL::dataEmissao(X509 *cert)
{
    try{
        ASN1_TIME* _certTime = X509_get_notBefore(cert);
        QDateTime _dt = asn1_to_QDateTime(_certTime);
        if (!_dt.isValid())
        {
           return false;
        }
        set_dataEmissao(_dt);

        return true;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        return false;
    }

}

bool CppOpenSSL::dataVencimento(X509 *cert)
{
    try{
        ASN1_TIME* _certTime = X509_get_notAfter(cert);
        QDateTime _dt = asn1_to_QDateTime(_certTime);
        if (!_dt.isValid())
        {
           return false;
        }
        set_dataVencimento(_dt);

        return true;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        return false;
    }

}

bool CppOpenSSL::certificadora()
{
    try{
        QString _texto = CppUtility::extractStr(get_issuerName(),"CN=", ",");
        QString _certificadora(_texto.mid(3, _texto.length() - 4)); // - 4 = 3 para o início e 1 para a virgura ","
        set_certificadora(_certificadora);

        return true;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        return false;
    }
}

bool CppOpenSSL::razaoSocial()
{
    try{
        QString _text(CppUtility::extractStr(get_subjectName(),"CN=", ","));
        int _index;
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            _index = _text.indexOf(QRegExp(":"));
        #else
            _index = _text.indexOf(QRegularExpression(":"));
        #endif

        QString _razao(_text.mid(3, _index - 4)); // - 4 = 3 para o início e 1 para a virgura ","
        set_razaoSocial(_razao);
        return true;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        return false;
    }

}

bool CppOpenSSL::cnpj()
{
    try{
        QString _text(CppUtility::extractStr(get_subjectName(),"CN=", ","));
        int _index;
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            _index = _text.indexOf(QRegExp(":"));
        #else
            _index = _text.indexOf(QRegularExpression(":"));
        #endif

        QString _cnpj(_text.mid(_index + 1, _text.length() - (_index + 2))); // _index + 1 remove ":" e _index + 2 remove ","
        set_cnpj(_cnpj);
        return true;

    } catch (std::exception const& e) {
        qWarning() << "erro: " << e.what();
        return false;
    }
}

bool CppOpenSSL::hash(const HashType &hashtype, const QByteArray &input, EVP_PKEY **pKey,
                      const bool tosign, const bool base64, QByteArray &output)
{
    unsigned char *_hash = NULL;
    EVP_MD_CTX* _ctx = NULL;
    const EVP_MD *_md = NULL;
    int _len = 0;
    bool _ret = true;
    QByteArray _res;

    try{
        QByteArray _sha(hashTypeToStr(hashtype).toLocal8Bit());

        _ctx = EVP_MD_CTX_create();
        if (_ctx == NULL)
        {
           _ret = false;
        }
        if (_ret)
        {
           // inicialize md para sha1
            _md = EVP_get_digestbyname(_sha.data());
           if (!_md)
           {
               _ret = false;
           }
        }
        if (_ret)
        {
            if (EVP_DigestInit_ex(_ctx, _md, NULL) != 1)
            {
                _ret = false;
            }
        }

        if (_ret)
        {
            // inicialize digest
            // atualiza digest
            if (EVP_DigestUpdate(_ctx, input.data(), input.length()) != 1)
            {
                _ret = false;
            }
        }
        if (_ret)
        {
            if (tosign)
            { //signature
                _hash = (unsigned char *) OPENSSL_malloc(EVP_PKEY_size(*pKey));//malloc(EVP_PKEY_size(*pKey)) a função malloc com openssl ocorre erro no MSVC ao liberar memória  >:(
                if (!EVP_SignFinal(_ctx, _hash, (unsigned int *)&_len, *pKey))
                {
                    _ret = false;
                }
            }else
            { //digestvalue
                _hash = (unsigned char *) OPENSSL_malloc(EVP_MD_size(_md));//malloc(EVP_MD_size(_md));
                if (!EVP_DigestFinal_ex(_ctx, _hash, (unsigned int *)&_len))
                {
                    _ret = false;
                }
            }
        }
        if (_ret)
        {
            for (int i = 0; i < _len; i ++) {
               _res.append(_hash[i]);
            }
            output.clear();
            if (base64)
                output.append(_res.toBase64());
            else
                output.append(_res);

            _res.clear();
        }

        if (_ctx != NULL)
          EVP_MD_CTX_destroy(_ctx);
        if (_hash != NULL)
          OPENSSL_clear_free((unsigned char*)_hash, _len);

        return _ret;

    } catch (std::exception const& e) {
        qWarning() << QStringLiteral( "Erro ao assinar documento: ") << e.what();
        if (_ctx != NULL)
          EVP_MD_CTX_destroy(_ctx);
        if (_hash != NULL)
          OPENSSL_clear_free((unsigned char*)_hash, _len);

        return false;
    }
}

QByteArray CppOpenSSL::x509ToQByteArray(X509 *cert)
{
    QByteArray _der(i2d_X509(cert, NULL), 0);
    if(!_der.isEmpty() )
    {
        unsigned char *p = (unsigned char*)_der.data();
        i2d_X509(cert, &p );
    }
    return _der;

}

bool CppOpenSSL::certToDER()
{
    try {
        //pKey
        QByteArray _pKeyder(i2d_PrivateKey(m_pkey, NULL), 0);
        unsigned char *_ppke = (unsigned char*)_pKeyder.data();
        i2d_PrivateKey(m_pkey, &_ppke);
        set_pkeyDER(_pKeyder);
        //cert
        QByteArray _cert(x509ToQByteArray(m_cert));
        set_certDER(_cert);
        //ca
        QList<QByteArray> _ca;
        for(int i = 0; i < sk_X509_num(m_ca); ++i)
            _ca << x509ToQByteArray(sk_X509_value(m_ca, i));
        set_caDER(_ca);

        return true;

    } catch (std::exception const& e) {
        qWarning() << QStringLiteral( "Erro ao converter certificados em DER: ") << e.what();
        return false;
    }
}

bool CppOpenSSL::loadOpenssl()
{
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    OPENSSL_no_config();
    PKCS12 *_p12 = NULL;
    BIO *_bioP12 = NULL;


    try {
            if (!unloadOpenssl())//clear all
            {
                set_error(QStringLiteral("Erro ao limpar antes de carregar openssl."));
                return false;
            }

            if (!(_bioP12 = BIO_new_file(this->m_p12path, "rb")))
            {
                set_error(QStringLiteral("Erro ao abrir certificado."));
                return false;
            }

            if (!(_p12 = d2i_PKCS12_bio(_bioP12, NULL)))
            {
                set_error(QStringLiteral("Erro ao carregar arquivo p12."));
                BIO_free(_bioP12);
                return false;
            }
            BIO_free(_bioP12);

            if (PKCS12_parse(_p12, this->m_pass, &this->m_pkey, &m_cert, &m_ca) != 1)
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: parse."));
                PKCS12_free(_p12);
                return false;
            }
            PKCS12_free(_p12);

            if (!m_ca)
            {
                //ca não precisa retornar false, pode ser inserido a qualquer momento
                set_error(QStringLiteral("Erro ao carregar Certificado: ca."));
                //return false;
            }
            if (!certToBase64(m_cert))
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: b64."));
                return false;
            }

            if (!issuerName(m_cert))
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: issuer."));
                return false;
            }

            if (!serialNumber(m_cert))
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: serial."));
                return false;
            }

            if (!subjectName(m_cert))
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: subject."));
                return false;
            }

            if (!dataEmissao(m_cert))
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: data emissao."));
                return false;
            }
            if (!dataVencimento(m_cert))
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: data vencimento."));
                return false;
            }
            if (!certificadora())
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: dados certificadora."));
                return false;
            }
            if (!razaoSocial())
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: razao social."));
                return false;
            }
            if (!cnpj())
            {
                set_error(QStringLiteral("Erro ao carregar Certificado: cnpj."));
                return false;
            }

            if(!certToDER())
            {
                set_error(QStringLiteral("Erro ao converter certificados em DER."));
                return false;
            }
            return true;

    } catch (std::exception const& e) {
        set_error(QString("%1%2").arg(QStringLiteral("Erro ao carregar Certificado: "), e.what()));
        qWarning() << QStringLiteral("Erro ao carregar Certificado: ") << e.what();
        if (_bioP12 != NULL)
          BIO_free(_bioP12);
        if (_p12 != NULL)
          PKCS12_free(_p12);
        // if error clear all
        if (m_pkey != NULL)
            EVP_PKEY_free(m_pkey);
        if (m_cert != NULL)
            X509_free(m_cert);
        if (m_ca != NULL)
            sk_X509_pop_free(m_ca, X509_free);

        return false;
    }
}

bool CppOpenSSL::unloadOpenssl()
{
    try{
        clear();
        //const char *m_p12path - ponteiro de fora
        //const char *m_pass - ponteiro de fora
        if (m_pkey != NULL)
            EVP_PKEY_free(m_pkey);
        if (m_cert != NULL)
            X509_free(m_cert);
        if (m_ca != NULL)
            sk_X509_pop_free(m_ca, X509_free);

        return true;

    } catch (std::exception const& e) {
        qWarning() << QStringLiteral("Erro em unload: ") << e.what();
        return false;
    }
}

