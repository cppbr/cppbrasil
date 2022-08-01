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


#ifndef CODE128_H
#define CODE128_H

#include <QString>
#include <QList>
#include <CppBrasil/cppbrasil_global.h>


enum class TYPES { DYNAMIC, A, B, C };
enum class COLUMN { COLUMN_VALUE, COLUMN_A, COLUMN_B, COLUMN_C, COLUMN_ENCODING };
struct element_t
{
    element_t(){};
    element_t(QString value, QString a, QString b, QString c, QString encoding) : Value(value), A(a), B(b), C(c), Encoding(encoding) { }
    QString Value;
    QString A;
    QString B;
    QString C;
    QString Encoding;

};

class CPPDANFE_EXPORT Code128
{
public:
    Code128(const QString &toEncode);
    Code128(const QString &toEncode, const TYPES &types);
    ~Code128();
    QString GetEncoding();
private:
    QString m_raw_data;
    QString m_encoded_data;
    TYPES m_type;
    element_t m_startCharacter;
    QList<element_t> m_listC128;
    QList<QString> m_formattedData;
    QString m_errorMsg;

    element_t m_startAElement;
    element_t m_startBElement;
    element_t m_startCElement;
    element_t m_codeAElement;
    element_t m_codeBElement;
    element_t m_codeCElement;
    element_t m_codeStop;

    void init_Code128(QString &toEncode, TYPES &type);
    QString calculateCheckDigit();
    void breakUpDataForEncoding();
    void insertStartandCodeCharacters();
    //QString GetEncoding();
    QList<element_t> findStartorCodeCharacter(const QString &s, int &col);
    int findCharEncodingRow(COLUMN codeType, QString value);

    const QString NUL = "\0";
    const QString SOH = "\x01";
    const QString STX = "\x02";
    const QString ETX = "\x03";
    const QString EOT = "\x04";
    const QString ENQ = "\x05";
    const QString ACK = "\x06";
    const QString BEL = "\x07";
    const QString BS  = "\x08";
    const QString TAB = "\x09";
    const QString LF  = "\x0A";
    const QString VT  = "\x0B";
    const QString FF  = "\x0C";
    const QString CR  = "\x0D";
    const QString SO  = "\x0E";
    const QString SI  = "\x0F";
    const QString DLE = "\x10";
    const QString DC1 = "\x11";
    const QString DC2 = "\x12";
    const QString DC3 = "\x13";
    const QString DC4 = "\x14";
    const QString NAK = "\x15";
    const QString SYN = "\x16";
    const QString ETB = "\x17";
    const QString CAN = "\x18";
    const QString EM  = "\x19";
    const QString SUB = "\x1A";
    const QString ESC = "\x1B";
    const QString FS  = "\x1C";
    const QString GS  = "\x1D";
    const QString RS  = "\x1E";
    const QString US  = "\x1F";
    const QString DEL = "\x7F";
    const QString FNC1 = "È";
    const QString FNC2 = "É";
    const QString FNC3 = "Ë";
    const QString FNC4 = "Ê";
    const QString SHIFT = "ô";
    const QString START_A = "õ";
    const QString START_B = "ö";
    const QString START_C = "ø";
    const QString CODE_A = "ù";
    const QString CODE_B = "ú";
    const QString CODE_C = "û";
    const QString STOP = "ü";
    const QString ON = "\xFF";
    const QString OFF = "\x00";

};

#endif // CODE128_H
