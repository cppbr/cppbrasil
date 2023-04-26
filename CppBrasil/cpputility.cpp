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


#include "cpputility.h"

CppUtility::CppUtility()
{
}

void CppUtility::clearDateTime(QDateTime &datetime)
{
    QDateTime _dtclear;
    datetime = _dtclear;
}

int CppUtility::dvModulo11(const QString &str)
{
    int _peso[] = {4,3,2,9,8,7,6,5,4,3,2,9,8,7,6,5,4,3,2,9,8,
                  7,6,5,4,3,2,9,8,7,6,5,4,3,2,9,8,7,6,5,4,3,2};

    int _soma = 0;
    int _n_peso = sizeof(_peso) / sizeof(int);
    for (int i = 0; i < _n_peso; ++i)
    {
        QString _schave = str.data()[i];
        int _ichave = _schave.toInt();
        _soma = _soma + (_ichave * _peso[i]);
    }
    int _resto = _soma % 11;
    int _dv = 11 - _resto;
    if (_dv >= 10)
      _dv = 0;
    return _dv;
}

QString CppUtility::insZeroLeft(const QString &value, const int &size)
{
//insere zero a esquerda, recebendo a quantidade de zeros em size e retornando string
    QString _r = value;
    if (value.length() < size)
      _r = _r.rightJustified(size, '0');

    return _r;
}

QString CppUtility::dateTimeToStr(const QDateTime &datetime, const DtH &dateReturn)
{
    switch (dateReturn) {
        case DtH::Date:
            return datetime.toString("yyyy-MM-dd");
            break;
        case DtH::DateTime:
            return datetime.toString("yyyy-MM-dd hh:mm:ss");
            break;
        case DtH::DateTimeUTC:
            return datetime.toOffsetFromUtc(datetime.offsetFromUtc()).toString(Qt::ISODate);
            break;
        case DtH::Time:
            return datetime.toString("hh:mm:ss");
            break;
        case DtH::DateTimeNumber:
            return datetime.toString("yyyyMMddhhmmss");
            break;
        case DtH::DateBr:
            return datetime.toString("dd/MM/yyyy");
            break;
        case DtH::DateTimeBr:
            return datetime.toString("dd/MM/yyyy hh:mm:ss");
            break;
        default: return datetime.toString("yyyy-MM-dd");
    }
}

QString CppUtility::doubleToStrDecimal(const double &value, const int &decimalSize)
{
    return QString::number(value, 'f', decimalSize);
}

QString CppUtility::extractStr(const QString &str, const QString &beginStr, const QString &endStr)
{
    int _ibegin;
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        _ibegin = str.indexOf(QRegExp(beginStr));
    #else
        _ibegin = str.indexOf(QRegularExpression(beginStr));
    #endif


    QString _result(str.mid(_ibegin, str.length() - _ibegin));

    int _iend;
    if (endStr.isEmpty())
        _iend = _result.length();
    else
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            _iend = _result.indexOf(QRegExp(endStr)) + endStr.length();
        #else
            _iend = _result.indexOf(QRegularExpression(endStr)) + endStr.length();
        #endif

    _result = _result.left(_iend);
    return _result;
}

QByteArray CppUtility::extractStr(const QByteArray &str, const QByteArray &beginStr, const QByteArray &endStr)
{
    QString _res = extractStr(QString::fromLocal8Bit(str, str.size()),
                              QString::fromLocal8Bit(beginStr, beginStr.size()),
                              QString::fromLocal8Bit(endStr, endStr.size()));
    return _res.toLocal8Bit();

}

int CppUtility::findString(const QString &str, const QString &strValue)
{
    int _pos;
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        _pos = str.indexOf(QRegExp(strValue));
    #else
        _pos = str.indexOf(QRegularExpression(strValue));
    #endif

    return _pos;
}

bool CppUtility::fileExists(const QString &caminho)
{
    //analisa se o arquivo existe
    const QFileInfo _file(caminho);
    if (!_file.exists())
        return false;

    return true;
}

bool CppUtility::parsePath(const QString &path, QString &output)
{
    //analisa o path e retorna na forma correta em output
    QString _res(path);
    const QChar _bsw(0x005c); //hex backslash windowsOS
    output.replace(QString(_bsw), QString("/"));

    if (_res.right(1) != "/")
      _res.append("/");

    const QFileInfo _outputDir(_res);
    if ((!_outputDir.exists()) || (!_outputDir.isDir()) || (!_outputDir.isWritable())) {
        //qWarning() << "o diretório de saída não existe, não é um diretório ou não é gravável"
        //           << outputDir.absoluteFilePath();
        return false;
    }

    output = _res;
    return true;
}

bool CppUtility::parsePath(const QString &path)
{
    //analisa o path e retorna ok se existe, se é um diretório ou se é gravável
    const QFileInfo _outputDir(path);
    if ((!_outputDir.exists()) || (!_outputDir.isDir()) || (!_outputDir.isWritable())) {
        //qWarning() << "o diretório de saída não existe, não é um diretório ou não é gravável"
        //           << outputDir.absoluteFilePath();
        return false;
    }
    return true;
}

bool CppUtility::saveFile(const QString &path, const QString &fileName,
                          const TipoArquivo &tArquivo, const QByteArray &dados)
{
    bool _res = true;
    QString _aPath, _file, _ext;

    switch (tArquivo) {
        case TipoArquivo::HTML :
            _ext = ".html";
            break;
        case TipoArquivo::TXT :
            _ext = ".txt";
            break;
        case TipoArquivo::XML :
            _ext = ".xml";
            break;
        default: _ext = ".txt";
    }

    if (!parsePath(path, _aPath))
       _res = false;

    //salva xml
    if (_res)
    {
        if (fileName.right(4) == _ext)
            _file = fileName;
        else
            _file = fileName + _ext;

        QFile outFile(_aPath + _file);
        if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
        {
            qWarning() << "Falha ao abrir o arquivo para gravação.";
            _res = false;
        }else
        {
            QTextStream stream( &outFile );
            stream << dados;
            outFile.close();
        }
    }
    return _res;
}

QString CppUtility::formatCPF(const QString &str)
{
    //123.456.789-01
    QString _res = str.mid(0, 3) + "." + str.mid(3, 3)
                   + "." + str.mid(6, 3)
                   + "-" + str.mid(9, 2);

    return _res;
}

QString CppUtility::formatCNPJ(const QString &str)
{
    //12.345.678/9012-34
    QString _res = str.mid(0, 2) + "." + str.mid(2, 3)
                   + "." + str.mid(5, 3)
                   + "/" + str.mid(8, 4)
                   + "-" + str.mid(12, 2);

    return _res;
}

QString CppUtility::formatMoney(const double &value, const int &decimalSize)
{
    //#.###,##
    QLocale _locale = QLocale(QLocale::German); //German usa a vírgula como separador decimal
    QString _val = _locale.toString(value, 'f', decimalSize);

    return _val;
}

int CppUtility::random(const int &digitos)
{
    QString _sDig = "999999999"; //maximo de digitos
    int _iDig = digitos;
    int _max;
    //se não informar nada, o max será até 999
    if (_iDig <= 0)
      _max = 999;
    else
        if (_iDig >= 10)
        {
            //digitos não pode ultrapassar o tamanho Int_max que é 2147483647 (10 digitos)
            _max = INT32_MAX;
            _iDig = 10;
        }
        else
        {
            #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                _max = _sDig.left(digitos).toInt();
            #else
                _max = QStringView{_sDig}.left(_iDig).toInt();
            #endif
        }

    int _ran, i = 0, _lenDig;
    //obtendo um número aleatório
    _ran = QRandomGenerator::global()->bounded(1, _max);

    //numeros proibidos
    QList<int> _list = {0, 11111111, 22222222, 33333333, 44444444, 55555555, 66666666,
                 77777777, 88888888, 99999999, 12345678, 23456789, 34567890, 45678901,
                 56789012, 67890123, 78901234, 89012345, 90123456, 01234567};

_dig:
    //enquanto o total de digitos de _ran for menor que o informado em digitos,
    //adiciona mais um digito aleatório de 0 a 9 à direita
    _lenDig = QString::number(_ran).length();
    while (_lenDig < _iDig)
    {
        QString _sRand = QString::number(_ran);
        int _iRand = QRandomGenerator::global()->bounded(0, 9);
        _sRand += QString::number(_iRand);

        //verifica se o numero obtido é maior que Int_max, caso seja tem que diminuir.
        unsigned int _uiV = _sRand.toUInt();
        if (_uiV > INT32_MAX) //se maior diminuirá de Int_max um valor aleatório entre 0 a 100000
            _ran = (INT32_MAX - QRandomGenerator::global()->bounded(0, 100000));
        else
            _ran = _sRand.toInt();

        _lenDig = QString::number(_ran).length();
    }

    //verifica novamente se o total de digitos de _ran é menor que o informado em _iDig
    if (QString::number(_ran).length() < _iDig)
      goto _dig;

    //verifica se o numero concebido é igual a algum número proibido da lista _list
    while (i < (_list.count() -1))
    {
        ++i;
        //se ran for igual a algum número do vetor, faz um novo ran e retorna ao _dig
        //até que gere um número que seja diferente ao vetor
        if (_list[i] == _ran)
        {
           _ran = QRandomGenerator::global()->bounded(1, _max);
           i = 0;
           goto _dig;
        }
    }

    return _ran;
}

