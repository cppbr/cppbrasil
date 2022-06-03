#ifndef CODE128_H
#define CODE128_H

#include <QString>
#include <QList>

enum class TYPES { DYNAMIC, A, B, C };
enum class COLUMN { COLUMN_VALUE, COLUMN_A, COLUMN_B, COLUMN_C, COLUMN_ENCODING };
struct element_t
{
    QString Value;
    QString A;
    QString B;
    QString C;
    QString Encoding;
    element_t(){};

    element_t(QString value, QString a, QString b, QString c, QString encoding) : Value(value), A(a), B(b), C(c), Encoding(encoding) { }
};

class Code128
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
};

#endif // CODE128_H
