#include "code128.h"
#include "code128_const.h"

Code128::Code128(const QString &toEncode)
{
    TYPES t = TYPES::DYNAMIC;
    Code128(toEncode, t);
}

Code128::Code128(const QString &toEncode, const TYPES &types): m_raw_data(toEncode)
{
    QString tempEnc = toEncode;
    TYPES tempTypes = types;
    init_Code128(tempEnc, tempTypes);
}

Code128::~Code128()
{
}

void Code128::init_Code128(QString &toEncode, TYPES &type)
{
    this->m_raw_data = toEncode;
    this->m_type = type;
    this->m_errorMsg = "";
    this->m_encoded_data = "";

    this->m_listC128.append(element_t("0", " ", " ", "00", "11011001100"));
    this->m_listC128.append(element_t("1", "!", "!", "01", "11001101100"));
    this->m_listC128.append(element_t("2", "\"", "\"", "02", "11001100110"));
    this->m_listC128.append(element_t("3", "#", "#", "03", "10010011000"));
    this->m_listC128.append(element_t("4", "$", "$", "04", "10010001100"));
    this->m_listC128.append(element_t("5", "%", "%", "05", "10001001100"));
    this->m_listC128.append(element_t("6", "&", "&", "06", "10011001000"));
    this->m_listC128.append(element_t("7", "'", "'", "07", "10011000100"));
    this->m_listC128.append(element_t("8", "(", "(", "08", "10001100100"));
    this->m_listC128.append(element_t("9", ")", ")", "09", "11001001000"));
    this->m_listC128.append(element_t("10", "*", "*", "10", "11001000100"));
    this->m_listC128.append(element_t("11", "+", "+", "11", "11000100100"));
    this->m_listC128.append(element_t("12", ",", ",", "12", "10110011100"));
    this->m_listC128.append(element_t("13", "-", "-", "13", "10011011100"));
    this->m_listC128.append(element_t("14", ".", ".", "14", "10011001110"));
    this->m_listC128.append(element_t("15", "/", "/", "15", "10111001100"));
    this->m_listC128.append(element_t("16", "0", "0", "16", "10011101100"));
    this->m_listC128.append(element_t("17", "1", "1", "17", "10011100110"));
    this->m_listC128.append(element_t("18", "2", "2", "18", "11001110010"));
    this->m_listC128.append(element_t("19", "3", "3", "19", "11001011100"));
    this->m_listC128.append(element_t("20", "4", "4", "20", "11001001110"));
    this->m_listC128.append(element_t("21", "5", "5", "21", "11011100100"));
    this->m_listC128.append(element_t("22", "6", "6", "22", "11001110100"));
    this->m_listC128.append(element_t("23", "7", "7", "23", "11101101110"));
    this->m_listC128.append(element_t("24", "8", "8", "24", "11101001100"));
    this->m_listC128.append(element_t("25", "9", "9", "25", "11100101100"));
    this->m_listC128.append(element_t("26", ":", ":", "26", "11100100110"));
    this->m_listC128.append(element_t("27", ";", ";", "27", "11101100100"));
    this->m_listC128.append(element_t("28", "<", "<", "28", "11100110100"));
    this->m_listC128.append(element_t("29", "=", "=", "29", "11100110010"));
    this->m_listC128.append(element_t("30", ">", ">", "30", "11011011000"));
    this->m_listC128.append(element_t("31", "?", "?", "31", "11011000110"));
    this->m_listC128.append(element_t("32", "@", "@", "32", "11000110110"));
    this->m_listC128.append(element_t("33", "A", "A", "33", "10100011000"));
    this->m_listC128.append(element_t("34", "B", "B", "34", "10001011000"));
    this->m_listC128.append(element_t("35", "C", "C", "35", "10001000110"));
    this->m_listC128.append(element_t("36", "D", "D", "36", "10110001000"));
    this->m_listC128.append(element_t("37", "E", "E", "37", "10001101000"));
    this->m_listC128.append(element_t("38", "F", "F", "38", "10001100010"));
    this->m_listC128.append(element_t("39", "G", "G", "39", "11010001000"));
    this->m_listC128.append(element_t("40", "H", "H", "40", "11000101000"));
    this->m_listC128.append(element_t("41", "I", "I", "41", "11000100010"));
    this->m_listC128.append(element_t("42", "J", "J", "42", "10110111000"));
    this->m_listC128.append(element_t("43", "K", "K", "43", "10110001110"));
    this->m_listC128.append(element_t("44", "L", "L", "44", "10001101110"));
    this->m_listC128.append(element_t("45", "M", "M", "45", "10111011000"));
    this->m_listC128.append(element_t("46", "N", "N", "46", "10111000110"));
    this->m_listC128.append(element_t("47", "O", "O", "47", "10001110110"));
    this->m_listC128.append(element_t("48", "P", "P", "48", "11101110110"));
    this->m_listC128.append(element_t("49", "Q", "Q", "49", "11010001110"));
    this->m_listC128.append(element_t("50", "R", "R", "50", "11000101110"));
    this->m_listC128.append(element_t("51", "S", "S", "51", "11011101000"));
    this->m_listC128.append(element_t("52", "T", "T", "52", "11011100010"));
    this->m_listC128.append(element_t("53", "U", "U", "53", "11011101110"));
    this->m_listC128.append(element_t("54", "V", "V", "54", "11101011000"));
    this->m_listC128.append(element_t("55", "W", "W", "55", "11101000110"));
    this->m_listC128.append(element_t("56", "X", "X", "56", "11100010110"));
    this->m_listC128.append(element_t("57", "Y", "Y", "57", "11101101000"));
    this->m_listC128.append(element_t("58", "Z", "Z", "58", "11101100010"));
    this->m_listC128.append(element_t("59", "[", "[", "59", "11100011010"));
    this->m_listC128.append(element_t("60", "\\", "\\", "60", "11101111010"));
    this->m_listC128.append(element_t("61", "]", "]", "61", "11001000010"));
    this->m_listC128.append(element_t("62", "^", "^", "62", "11110001010"));
    this->m_listC128.append(element_t("63", "_", "_", "63", "10100110000"));
    this->m_listC128.append(element_t("64", NUL, "`", "64", "10100001100"));
    this->m_listC128.append(element_t("65", SOH, "a", "65", "10010110000"));
    this->m_listC128.append(element_t("66", STX, "b", "66", "10010000110"));
    this->m_listC128.append(element_t("67", ETX, "c", "67", "10000101100"));
    this->m_listC128.append(element_t("68", EOT, "d", "68", "10000100110"));
    this->m_listC128.append(element_t("69", ENQ, "e", "69", "10110010000"));
    this->m_listC128.append(element_t("70", ACK, "f", "70", "10110000100"));
    this->m_listC128.append(element_t("71", BEL, "g", "71", "10011010000"));
    this->m_listC128.append(element_t("72", BS, "h", "72", "10011000010"));
    this->m_listC128.append(element_t("73", TAB, "i", "73", "10000110100"));
    this->m_listC128.append(element_t("74", LF, "j", "74", "10000110010"));
    this->m_listC128.append(element_t("75", VT, "k", "75", "11000010010"));
    this->m_listC128.append(element_t("76", FF, "l", "76", "11001010000"));
    this->m_listC128.append(element_t("77", CR, "m", "77", "11110111010"));
    this->m_listC128.append(element_t("78", SO, "n", "78", "11000010100"));
    this->m_listC128.append(element_t("79", SI, "o", "79", "10001111010"));
    this->m_listC128.append(element_t("80", DLE, "p", "80", "10100111100"));
    this->m_listC128.append(element_t("81", DC1, "q", "81", "10010111100"));
    this->m_listC128.append(element_t("82", DC2, "r", "82", "10010011110"));
    this->m_listC128.append(element_t("83", DC3, "s", "83", "10111100100"));
    this->m_listC128.append(element_t("84", DC4, "t", "84", "10011110100"));
    this->m_listC128.append(element_t("85", NAK, "u", "85", "10011110010"));
    this->m_listC128.append(element_t("86", SYN, "v", "86", "11110100100"));
    this->m_listC128.append(element_t("87", ETB, "w", "87", "11110010100"));
    this->m_listC128.append(element_t("88", CAN, "x", "88", "11110010010"));
    this->m_listC128.append(element_t("89", EM, "y", "89", "11011011110"));
    this->m_listC128.append(element_t("90", SUB, "z", "90", "11011110110"));
    this->m_listC128.append(element_t("91", ESC, "{", "91", "11110110110"));
    this->m_listC128.append(element_t("92", FS, "|", "92", "10101111000"));
    this->m_listC128.append(element_t("93", GS, "}", "93", "10100011110"));
    this->m_listC128.append(element_t("94", RS, "~", "94", "10001011110"));
    this->m_listC128.append(element_t("95", US, DEL, "95", "10111101000"));
    this->m_listC128.append(element_t("96", FNC3, FNC3, "96", "10111100010"));
    this->m_listC128.append(element_t("97", FNC2, FNC2, "97", "11110101000"));
    this->m_listC128.append(element_t("98", SHIFT, SHIFT, "98", "11110100010"));
    this->m_listC128.append(element_t("99", CODE_C, CODE_C, "99", "10111011110"));
    this->m_listC128.append(element_t("100", CODE_B, FNC4, CODE_B, "10111101110"));
    this->m_listC128.append(element_t("101", FNC4, CODE_A, CODE_A, "11101011110"));
    this->m_listC128.append(element_t("102", FNC1, FNC1, FNC1, "11110101110"));
    this->m_listC128.append(element_t("103", START_A, START_A, START_A, "11010000100"));
    this->m_listC128.append(element_t("104", START_B, START_B, START_B, "11010010000"));
    this->m_listC128.append(element_t("105", START_C, START_C, START_C, "11010011100"));
    this->m_listC128.append(element_t("", STOP, STOP, STOP, "11000111010"));

    this->m_startAElement = element_t("103", START_A, START_A, START_A, "11010000100");
    this->m_startBElement = element_t("104", START_B, START_B, START_B, "11010010000");
    this->m_startCElement = element_t("105", START_C, START_C, START_C, "11010011100");
    this->m_codeAElement  = element_t("101", FNC4, CODE_A, CODE_A, "11101011110");
    this->m_codeBElement  = element_t("100", CODE_B, FNC4, CODE_B, "10111101110");
    this->m_codeCElement  = element_t("99", CODE_C, CODE_C, "99", "10111011110");
    this->m_codeStop = element_t("", STOP, STOP, STOP, "11000111010");




}

QString Code128::calculateCheckDigit()
{
    unsigned int CheckSum = 0;
    QString s;
    element_t row;

    for (unsigned int i = 0; i < m_formattedData.size(); ++i)
    {
        //get char to find
        s = m_formattedData[i];

        //try to find value in the A column
        int index = findCharEncodingRow(COLUMN::COLUMN_A, s);
        if (index > -1)
        {
            //found in encoding type A
            row = m_listC128.at(index);
        }
        else
        {
            index = findCharEncodingRow(COLUMN::COLUMN_B, s);
            if (index > -1)
            {
                //found in encoding B
                row = m_listC128.at(index);
            }
            else
            {
                index = findCharEncodingRow(COLUMN::COLUMN_C, s);
                if (index > -1)
                {
                    //found in encoding C
                    row = m_listC128.at(index);
                }
                else
                {
                    //TODO: not found in A B or C so this is an invalid character, how should we handle that?
                }
            }
        }

        unsigned int value = row.Value.toInt();//atoi(row->Value.toLocal8Bit());
        unsigned int addition = value * ((i == 0) ? 1 : i);
        CheckSum +=  addition;
    }//for

    unsigned int Remainder = (CheckSum % 103);
    row = m_listC128.at(findCharEncodingRow(COLUMN::COLUMN_VALUE, QString::number(Remainder)));
    QString _res = row.Encoding;
    return _res;
}

void Code128::breakUpDataForEncoding()
{
    QString temp = "";
    QString tempRawData = m_raw_data;

    //breaking the raw data up for code A and code B will mess up the encoding
    if (m_type == TYPES::A || m_type == TYPES::B)
    {
        for (unsigned int i = 0; i < m_raw_data.size(); ++i)
        {

            m_formattedData.append(QStringLiteral("") + m_raw_data.at(i));
        }
        return;
    }//if
    else if (m_type == TYPES::C)
    {
        bool _isNumber;
        m_raw_data.toUInt(&_isNumber, 10);
        if (!_isNumber)
        {
            m_errorMsg = "EC128-6: Only numeric values can be encoded with C128-C.";
        }

        //CODE C: adds a 0 to the front of the Raw_Data if the length is not divisible by 2
        if (m_raw_data.size() % 2 > 0)
        {
            tempRawData = QStringLiteral("0") + m_raw_data;
        }
    }//if

    for (unsigned int i = 0; i < tempRawData.size(); ++i)
    {
        char c = tempRawData.at(i).toLatin1();
        if (isdigit(c))
        {
            if (temp == "")
            {
                temp += c;
            }//if
            else
            {
                m_formattedData.append(temp + c);
                temp = "";
            }//else
        }//if
        else
        {
            if (temp != "")
            {
                m_formattedData.append(temp);
                temp = "";
            }//if
            m_formattedData.append(QStringLiteral("") + c);
        }//else
    }//for

    //if something is still in temp go ahead and push it onto the queue
    if (temp != "")
    {
        m_formattedData.append(temp);
        temp = "";
    }//if
}

void Code128::insertStartandCodeCharacters()
{
    QString currentCodeString = "";
    QList<QString>::Iterator it;
    it = m_formattedData.begin();

    if (m_type != TYPES::DYNAMIC)
    {
        switch (m_type)
        {
        case TYPES::A: m_formattedData.insert(it, START_A);
            break;
        case TYPES::B: m_formattedData.insert(it, START_B);
            break;
        case TYPES::C: m_formattedData.insert(it, START_C);
            break;
        default: m_errorMsg = "EC128-4: Unknown start type in fixed type encoding.";
            break;
        }
    }//if
    else
    {
        try
        {
            for (unsigned int i = 0; i < (m_formattedData.size()); ++i)
            {
                int col = 0;
                QList<element_t> tempStartChars = findStartorCodeCharacter(m_formattedData.value(i), col);

                //check all the start characters and see if we need to stay with the same codeset or if a change of sets is required
                bool sameCodeSet = false;
                element_t row;
                for (unsigned int i = 0; i < tempStartChars.size(); ++i)
                {
                    row = tempStartChars.value(i);
                    if (row.A == currentCodeString || row.B == currentCodeString || row.C == currentCodeString)
                    {
                        sameCodeSet = true;
                        break;
                    }//if
                }//foreach

                //only insert a new code char if starting a new codeset
                //if (CurrentCodeString == "" || !tempStartChars[0][col].ToString().EndsWith(CurrentCodeString)) /* Removed because of bug */

                if (currentCodeString == "" || !sameCodeSet)
                {
                    element_t CurrentCodeSet = tempStartChars.value(0);

                    if (CurrentCodeSet.A == START_A || CurrentCodeSet.B == START_A || CurrentCodeSet.C == START_A)
                    {
                        currentCodeString = START_A;
                    }
                    else if (CurrentCodeSet.A == START_B || CurrentCodeSet.B == START_B || CurrentCodeSet.C == START_B)
                    {
                        currentCodeString = START_B;
                    }
                    else if (CurrentCodeSet.A == START_C || CurrentCodeSet.B == START_C || CurrentCodeSet.C == START_C)
                    {
                        currentCodeString = START_C;
                    }
                    else
                    {
                        m_errorMsg = "No start character found in CurrentCodeSet.";
                    }

                    m_formattedData.insert(it++, currentCodeString);
                }//if
            }//for
        }
        catch (const std::exception& e)
        {
            m_errorMsg = "EC128-3: Could not insert start and code characters.\n Message: " + QString(e.what());
        }//catch
    }//else
}

QString Code128::GetEncoding()
{
    m_formattedData.clear();

    //break up data for encoding
    breakUpDataForEncoding();
    //insert the start characters
    insertStartandCodeCharacters();

    QString CheckDigit = calculateCheckDigit();

    m_encoded_data = "";
    for (unsigned int i = 0; i < m_formattedData.size(); ++i)
    {
        QString s = m_formattedData.value(i);

        //handle exception with apostrophes in select statements
        element_t E_Row;

        //select encoding only for type selected
        switch (m_type)
        {
            int ind;
        case TYPES::A:
            ind = findCharEncodingRow(COLUMN::COLUMN_A, s);
            if (ind >= 0)
            {
                E_Row = m_listC128.at(ind);
            }
            break;
        case TYPES::B:
            ind = findCharEncodingRow(COLUMN::COLUMN_B, s);
            if (ind >= 0)
            {
                E_Row = m_listC128.at(ind);
            }
            break;
        case TYPES::C:
            ind = findCharEncodingRow(COLUMN::COLUMN_C, s);
            if (ind >= 0)
            {
                E_Row = m_listC128.at(ind);
            }
            break;
        case TYPES::DYNAMIC:
            ind = findCharEncodingRow(COLUMN::COLUMN_A, s);

            if (ind < 0)
            {
                ind = findCharEncodingRow(COLUMN::COLUMN_B, s);

                if (ind < 0)
                {
                    ind = findCharEncodingRow(COLUMN::COLUMN_C, s);
                }//if
            }//if

            if (ind >= 0)
            {
                E_Row = m_listC128.at(ind);
            }
            break;
        default:
            break;
        }//switch

        if (E_Row.Encoding.isEmpty())
        {
            m_errorMsg = "EC128-5: Could not find encoding of a value( " + s + " ) in C128";
            return QString();
        }

        m_encoded_data += E_Row.Encoding;
    }//foreach

    //add the check digit
    m_encoded_data += CheckDigit;

    //add the stop character
    m_encoded_data += m_codeStop.Encoding;

    //add the termination bars
    m_encoded_data += "11";

    return m_encoded_data;
}

QList<element_t> Code128::findStartorCodeCharacter(const QString &s, int &col)
{
    QList<element_t> rows;

    //if two chars are numbers (or FNC1) then START_C or CODE_C
    if (s.size() > 1 && (s.at(0).isDigit() || s.at(0) == FNC1) && (s.at(1).isDigit() || s.at(1) == FNC1))
    {
        if (!&m_startCharacter)
        {
            m_startCharacter = m_startCElement;
            rows.append(m_startCharacter);
        }//if
        else
            rows.push_back(m_startCElement);

        col = 1;
    }//if
    else
    {
        bool AFound = false;
        bool BFound = false;
        for (unsigned int i = 0; i < m_listC128.size(); ++i)
        {
            element_t row = m_listC128.at(i);
            try
            {
                if (!AFound && s == row.A)
                {
                    AFound = true;
                    col = 2;

                    if (!&m_startCharacter)
                    {
                        m_startCharacter = m_startAElement;
                        rows.insert(rows.begin(), m_startCharacter);
                    }//if
                    else
                    {
                        rows.insert(rows.begin(), m_codeAElement);
                    }//else
                }//if
                else if (!BFound && s == row.B)
                {
                    BFound = true;
                    col = 1;

                    if (!&m_startCharacter)
                    {
                        m_startCharacter = m_startBElement;
                        rows.insert(rows.begin(), m_startCharacter);
                    }//if
                    else
                    {
                        rows.insert(rows.begin(), m_codeBElement);
                    }
                }//else
                else if (AFound && BFound)
                {
                    break;
                }
            }//try
            catch (const std::exception& e)
            {
                m_errorMsg = "EC128-1: " + QString(e.what());
            }//catch
        }//foreach

        if (rows.size() <= 0)
        {
            m_errorMsg = "EC128-2: Could not determine start character.";
        }
    }//else

    return rows;
}

int Code128::findCharEncodingRow(COLUMN codeType, QString value)
{
    for (unsigned int i = 0; i < m_listC128.size() - 1; ++i)
    {
        switch(codeType)
        {
        case COLUMN::COLUMN_A:
            if (m_listC128.at(i).A == value)
            {
                return i;
            }
            break;
        case COLUMN::COLUMN_B:
            if (m_listC128.at(i).B == value)
            {
                return i;
            }
            break;
        case COLUMN::COLUMN_C:
            if (m_listC128.at(i).C == value)
            {
                return i;
            }
            break;
        case COLUMN::COLUMN_VALUE:
            if (m_listC128.at(i).Value == value)
            {
                return i;
            }
            break;
        default:
            break;
        };
    }

    return -1;
}

