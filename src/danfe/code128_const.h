#ifndef CODE128_CONST_H
#define CODE128_CONST_H
#include <QString>

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
#endif // CODE128_CONST_H
