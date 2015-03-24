#ifndef TAGLIB_LOCALE_HELPER_H
#define TAGLIB_LOCALE_HELPER_H

#include "tbytevector.h"
#include "taglib_export.h"

#ifndef DO_NOT_DOCUMENT
namespace LocaleHelper {

TAGLIB_EXPORT bool GBK2UTF8(const TagLib::ByteVector &bv, TagLib::ByteVector *out);

}
#endif // DO_NOT_DOCUMENT
#endif // TAGLIB_LOCALE_HELPER_H
