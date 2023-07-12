#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <cassert>
#include <limits>
#include <stdexcept>
#include <cctype>
#include <QString>

namespace Start {

::std::string base64_encode(const ::std::string &bindata);
::std::string base64_decode(const ::std::string &ascdata);
QString encodeString(QString string, QString key);
QString decodeString(QString encodedString, QString key);

}

#endif // BASE64_H
