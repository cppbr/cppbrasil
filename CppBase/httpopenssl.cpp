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

HttpOpenSSL::HttpOpenSSL(const CppCrypto* crypto, const int &timeout) : HttpClient(crypto)
{
    this->m_timeout = timeout;

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
            BIO_free(_biocert);
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

QByteArray HttpOpenSSL::send(const QByteArray &host, const QByteArray &data)
{
    struct hostent *_hp;
    struct sockaddr_in _addr;
    int _sock;
    #ifdef _WIN32
       WORD wVersionRequested = MAKEWORD(2,2);
       WSADATA wsaData;
       WSAStartup(wVersionRequested, &wsaData);
    #endif

    this->m_data.clear();

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
    //fecha o socket em qualquer retorno
    std::unique_ptr<int, void(*)(int*)> _sockGuard(&_sock, [](int *s) {
#ifdef _WIN32
        closesocket(*s);
#else
        close(*s);
#endif
    });

    //timeout de envio e leitura (mesmo padrão do HttpQt)
    int _timeout = (this->m_timeout > 0) ? this->m_timeout : 15000;
#ifdef _WIN32
    DWORD _tv = _timeout;
#else
    struct timeval _tv;
    _tv.tv_sec = _timeout / 1000;
    _tv.tv_usec = (_timeout % 1000) * 1000;
#endif
    setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&_tv, sizeof(_tv));
    setsockopt(_sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&_tv, sizeof(_tv));

    if(::connect(_sock,(struct sockaddr *)&_addr, sizeof(_addr))<0)
    {
        emit errorOccurred("Erro ao conectar ao servidor");
        return "";
    }

    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_CONFIG |
                     OPENSSL_INIT_LOAD_SSL_STRINGS |
                     OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)> _ctx(SSL_CTX_new(TLS_client_method()), SSL_CTX_free);
    if (!_ctx)
    {
        emit errorOccurred("Erro ao criar contexto SSL.");
        return "";
    }
#ifdef SSL_OP_IGNORE_UNEXPECTED_EOF
    //servidores que fecham a conexão sem close_notify
    SSL_CTX_set_options(_ctx.get(), SSL_OP_IGNORE_UNEXPECTED_EOF);
#endif
    if (SSL_CTX_use_cert_and_key(_ctx.get(), m_cert, m_pkey, m_ca, 0) != 1)
    {
        emit errorOccurred("Erro ao carregar certificado na conexão SSL.");
        return "";
    }

    std::unique_ptr<SSL, decltype(&SSL_free)> _ssl(SSL_new(_ctx.get()), SSL_free);
    if (!_ssl)
    {
        emit errorOccurred("Erro ao criar SSL.");
        return "";
    }

    SSL_set_fd(_ssl.get(), _sock);
    SSL_set_tlsext_host_name(_ssl.get(), _h.data()); //SNI

    int _ret = SSL_connect(_ssl.get());
    if (_ret <= 0)
    {
        int _err = SSL_get_error(_ssl.get(), _ret);
        emit errorOccurred("Erro ao criar conexão SSL :" + QString::number(_err) + " - " + getErrorName(_err));
        return "";
    }

    if (sendPacket(_ssl.get(), getCommand(host, data)))
        receivePacket(_ssl.get());

    SSL_shutdown(_ssl.get());

    return this->m_data;
}

bool HttpOpenSSL::sendPacket(SSL *ssl, const QByteArray &buf)
{
    int _len = SSL_write(ssl, buf.constData(), buf.size());
    if (_len <= 0)
    {
        int _err = SSL_get_error(ssl, _len);
        emit errorOccurred("Erro ao enviar dados :" + QString::number(_err) + " - " + getErrorName(_err));
        return false;
    }
    return true;
}

void HttpOpenSSL::receivePacket(SSL *ssl)
{
    int _len;
    int _err = SSL_ERROR_NONE;
    QByteArray _data;
    QByteArray _buf;
    _buf.resize(1024 * 8);
    //lê até o fim da resposta: Content-Length completo, último chunk ou conexão fechada
    while (!responseComplete(_data))
    {
        _len = SSL_read(ssl, _buf.data(), _buf.size());
        if (_len <= 0)
        {
            _err = SSL_get_error(ssl, _len);
            break;
        }
        _data.append(_buf.constData(), _len);
    }

    if ((_err == SSL_ERROR_WANT_READ) || (_err == SSL_ERROR_WANT_WRITE))
    {
        //socket bloqueante: só acontece quando o SO_RCVTIMEO expira
        emit errorOccurred("Erro ao ler dados retornados: tempo de resposta esgotado (timeout).");
        return;
    }
    if ((_err != SSL_ERROR_NONE) && (_err != SSL_ERROR_ZERO_RETURN))
    {
        emit errorOccurred("Erro ao ler dados retornados :" + QString::number(_err) + " - " + getErrorName(_err));
        return;
    }

    int _end = _data.indexOf("\r\n\r\n");
    if (_end < 0)
    {
        emit errorOccurred("Erro ao ler dados retornados: resposta HTTP incompleta.");
        return;
    }

    QByteArray _body = _data.mid(_end + 4);
    if (headerValue(_data.left(_end), "Transfer-Encoding").toLower().contains("chunked"))
        _body = decodeChunked(_body);

    this->m_data.append(_body.trimmed());
}

bool HttpOpenSSL::responseComplete(const QByteArray &data)
{
    int _end = data.indexOf("\r\n\r\n");
    if (_end < 0)
        return false;

    QByteArray _header = data.left(_end);
    int _sizeBody = data.size() - (_end + 4);

    if (headerValue(_header, "Transfer-Encoding").toLower().contains("chunked"))
        return data.endsWith("\r\n0\r\n\r\n") || (_sizeBody >= 5 && data.mid(_end + 4).startsWith("0\r\n\r\n"));

    bool _ok = false;
    int _length = headerValue(_header, "Content-Length").toInt(&_ok);
    if (_ok)
        return _sizeBody >= _length;

    return false; //sem tamanho: lê até o servidor fechar a conexão
}

QByteArray HttpOpenSSL::headerValue(const QByteArray &header, const QByteArray &name)
{
    const QList<QByteArray> _lines = header.split('\n');
    for (const QByteArray &_line : _lines)
    {
        int _i = _line.indexOf(':');
        if ((_i > 0) && (_line.left(_i).trimmed().toLower() == name.toLower()))
            return _line.mid(_i + 1).trimmed();
    }
    return QByteArray();
}

QByteArray HttpOpenSSL::decodeChunked(const QByteArray &body)
{
    QByteArray _res;
    int _pos = 0;
    while (_pos < body.size())
    {
        int _eol = body.indexOf("\r\n", _pos);
        if (_eol < 0)
            break;
        bool _ok = false;
        //tamanho em hexa, podendo ter extensões depois de ';'
        int _size = body.mid(_pos, _eol - _pos).split(';').first().trimmed().toInt(&_ok, 16);
        if (!_ok || _size <= 0)
            break;
        _res.append(body.mid(_eol + 2, _size));
        _pos = _eol + 2 + _size + 2;
    }
    return _res;
}

QByteArray HttpOpenSSL::getCommand(const QByteArray &host, const QByteArray &data)
{
    QByteArray _http;

    _http.append("POST " + getHostName(host, THost::server) + " HTTP/1.1\r\n");
    _http.append("Host: " + getHostName(host, THost::host) + "\r\n");
    _http.append("Content-Type: " + m_contentType + "\r\n");
    _http.append("Content-Length: " + QByteArray::number(data.size()) + "\r\n");
    _http.append("Connection: close\r\n\r\n");
    _http.append(data);

    return _http;
}

QByteArray HttpOpenSSL::getHostName(const QByteArray &host, const THost &tipo)
{
    QString _host;

    if (tipo == THost::host)
    {
        _host = CppUtility::extractStr(QString(host), "//", ".br");
        return (_host.mid(2, _host.length() - 2).toLocal8Bit());
    }
    else
    {
        _host = CppUtility::extractStr(QString(host), ".br", "");
        return (_host.mid(3, _host.length() - 3).toLocal8Bit());
    }

}

QString HttpOpenSSL::getErrorName(const int &err)
{
    QString _erroName;
    switch (err)
    {
        case SSL_ERROR_SSL:
            _erroName = "SSL_ERROR_SSL";
            break;
        case SSL_ERROR_WANT_READ:
            _erroName = "SSL_ERROR_WANT_READ";
            break;
        case SSL_ERROR_WANT_WRITE:
            _erroName = "SSL_ERROR_WANT_WRITE";
            break;
        case SSL_ERROR_WANT_X509_LOOKUP:
            _erroName = "SSL_ERROR_WANT_X509_LOOKUP";
            break;
        case SSL_ERROR_SYSCALL:
            _erroName = "SSL_ERROR_SYSCALL";
            break;
        case SSL_ERROR_ZERO_RETURN:
            _erroName = "SSL_ERROR_ZERO_RETURN";
            break;
        case SSL_ERROR_WANT_CONNECT:
            _erroName = "SSL_ERROR_WANT_CONNECT";
            break;
        case SSL_ERROR_WANT_ACCEPT:
            _erroName = "SSL_ERROR_WANT_ACCEPT";
            break;
        case SSL_ERROR_WANT_ASYNC:
            _erroName = "SSL_ERROR_WANT_ASYNC";
            break;
        case SSL_ERROR_WANT_ASYNC_JOB:
            _erroName = "SSL_ERROR_WANT_ASYNC_JOB";
            break;
        case SSL_ERROR_WANT_CLIENT_HELLO_CB:
            _erroName = "SSL_ERROR_WANT_CLIENT_HELLO_CB";
            break;
        default:
            _erroName = "SSL_ERROR_UNIDENTIFIED";
    }

    return _erroName;
}

