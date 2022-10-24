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


#include "httpopenssl.h"

HttpOpenSSL::HttpOpenSSL(const CppCrypto* crypto) : HttpClient(crypto)
{
    //pkey
    QByteArray _pkeyDER = crypto->get_pkeyDER();
    BIO *_bpkey = BIO_new_mem_buf(_pkeyDER.data(), _pkeyDER.length());
    if (!(m_pkey =  d2i_PrivateKey_bio(_bpkey, NULL)))
    {
        emit errorOccurred("Erro ao obter chave privada de streaming.");
    }
    BIO_free(_bpkey);

    //cert
    QByteArray _certDER = crypto->get_certDER();
    BIO *_bcert = BIO_new_mem_buf(_certDER.data(), _certDER.length());
    if (!(m_cert =  d2i_X509_bio(_bcert, NULL)))
    {
        emit errorOccurred("Erro ao obter chave publica de streaming.");
    }
    BIO_free(_bcert);

    //CA certs
    m_ca = sk_X509_new_null();
    foreach (const QByteArray &item, this->m_crypto->get_caDER())
    {
        BIO *_biocert = BIO_new_mem_buf(item.data(), item.length());
        X509 *_cert = NULL;
        if (!(_cert = d2i_X509_bio(_biocert, NULL)))
        {
            emit errorOccurred("Erro ao obter certificado AC de streaming.");
            break;
        }
        sk_X509_push(m_ca, X509_dup(_cert));
        BIO_free(_biocert);
        X509_free(_cert);
    }

}

HttpOpenSSL::~HttpOpenSSL()
{
    if (this->m_pkey != NULL)
        EVP_PKEY_free(this->m_pkey);
    if (m_cert != NULL)
        X509_free(m_cert);
    if (m_ca != NULL)
        sk_X509_pop_free(m_ca, X509_free);
}

QByteArray HttpOpenSSL::send(const QByteArray &host, const QByteArray &data,
                             const int &lenReturn)
{
    struct hostent *_hp;
    struct sockaddr_in _addr;
    int _sock;
    #ifdef _WIN32
       WORD wVersionRequested = MAKEWORD(2,2);
       WSADATA wsaData;
       WSAStartup(wVersionRequested, &wsaData);
    #endif

    //se o gethostbyname não funcionar, deve retornar daqui.
    QByteArray _h = getHostName(host, THost::host);
    if(!(_hp=gethostbyname(_h.data())))
    {
        emit errorOccurred("Erro ao identificar host por nome.");
        return "";
    }

    memset(&_addr,0, sizeof(_addr));
    _addr.sin_addr=*(struct in_addr*)_hp->h_addr_list[0];
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(443);

    if((_sock=socket(AF_INET,SOCK_STREAM, IPPROTO_TCP))<0)
    {
        emit errorOccurred("Erro ao criar socket.");
        return "";
    }

    if(::connect(_sock,(struct sockaddr *)&_addr, sizeof(_addr))<0)
    {
        emit errorOccurred("Erro ao conectar ao servidor");
        return "";
    }

    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *_meth = TLS_client_method();
    SSL_CTX *_ctx = SSL_CTX_new (_meth);
    SSL_CTX_use_cert_and_key(_ctx, m_cert, m_pkey, m_ca, 0);

    SSL *_ssl = SSL_new(_ctx);
    if (!_ssl)
    {
        emit errorOccurred("Erro ao criar SSL.\n");
        return "";
    }

    BIO *_sbio = BIO_new_socket(_sock, BIO_NOCLOSE);
    SSL_set_bio(_ssl, _sbio, _sbio);
    SSL_set_connect_state(_ssl);
    SSL_accept(_ssl);

    int _err = SSL_connect(_ssl);
    if (_err <= 0)
    {
        emit errorOccurred("Erro ao criar conexão SSL :" + QString::number(_err));
        return "";
    }

    QByteArray _pack = getCommand(host, data);
    sendPacket(_ssl, _pack.data());
    receivePacket(_ssl, lenReturn);
#ifdef __linux__
    close(_sock);
#elif _WIN32
    closesocket(_sock);
#endif

    SSL_free(_ssl);
    SSL_CTX_free(_ctx);

    return this->m_data;
}

int HttpOpenSSL::sendPacket(SSL *ssl, const char *buf)
{
    int _len = SSL_write(ssl, buf, strlen(buf));
    if (_len < 0)
    {
        int _err = SSL_get_error(ssl, _len);
        emit errorOccurred("Erro ao enviar dados :" + QString::number(_err));
        switch (_err)
        {
            case SSL_ERROR_WANT_WRITE:
                return 0;
            case SSL_ERROR_WANT_READ:
                return 0;
            case SSL_ERROR_ZERO_RETURN:
            case SSL_ERROR_SYSCALL:
            case SSL_ERROR_SSL:
            default:
                return -1;
        }
    }
    return _len;
}

int HttpOpenSSL::receivePacket(SSL *ssl, int len)
{
    int _head = 650; //post command + soap envelop + namespace soap
    int _sizeBf = _head + len;
    //char _buffer[_sizeBf]; só funciona no g++. Variable-length arrays estilo C99.
    QByteArray _buffer;
    _buffer.resize(_sizeBf);

    int _lenBuffe = SSL_read(ssl,_buffer.data() , _buffer.size());
    if (_lenBuffe < 0)
    {
        int _err = SSL_get_error(ssl, len);
        emit errorOccurred("Erro ao ler dados retornados :" + QString::number(_err));
        if (_err == SSL_ERROR_WANT_READ)
            return 0;
        if (_err == SSL_ERROR_WANT_WRITE)
            return 0;
        if (_err == SSL_ERROR_ZERO_RETURN || _err == SSL_ERROR_SYSCALL || _err == SSL_ERROR_SSL)
            return -1;
    } else
    {
        QString _data = QString::fromLocal8Bit(_buffer, _lenBuffe);
        QString _res = CppUtility::extractStr(_data, "\r\n\r\n", "").trimmed();
        this->m_data.clear();
        this->m_data.append(_res.toLocal8Bit());
    }
    return _lenBuffe;
}

QByteArray HttpOpenSSL::getCommand(const QByteArray &host, const QByteArray &data)
{
    QString _http, _host, _server;

    _host = getHostName(host, THost::host);
    _server = getHostName(host, THost::server);

    _http.append("POST "+ _server +" HTTP/1.1 \r\n");
    _http.append("Host: "+ _host +" \r\n");
    _http.append("Content-Type: text/xml; charset=utf-8\r\n");
    _http.append("Content-Length:" + QString::number(data.length()) +"\r\n\r\n");
    _http.append(data);

    return _http.toLocal8Bit();
}

QByteArray HttpOpenSSL::getHostName(const QByteArray &host, const THost &tipo)
{
    QString _host;

    if (tipo == THost::host)
    {
        _host = CppUtility::extractStr(QString(host), "//", ".br");
        return (_host.mid(2, _host.length() - 2).toLocal8Bit());
    } else
    {
        _host = CppUtility::extractStr(QString(host), ".br", "");
        return (_host.mid(3, _host.length() - 3).toLocal8Bit());
    }

}

