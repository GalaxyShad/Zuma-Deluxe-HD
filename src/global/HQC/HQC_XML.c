#include "../HQC.h"

#include <expat.h>

HQC_XmlParser HQC_XmlParser_Create(const char* encoding) {
    return XML_ParserCreate(encoding);
}


void HQC_XmlParser_SetHandlers(
    HQC_XmlParser parser, HQC_XmlParser_StartHandler startFunction, HQC_XmlParser_EndHandler endFunction
) {
    XML_SetElementHandler(parser, startFunction, endFunction);
}


void HQC_XmlParser_Parse(HQC_XmlParser parser, const char* buffer, size_t count, bool isDone) {
    return (XML_Parse(parser, buffer, count, isDone) == XML_STATUS_OK) ?
        NULL :
        HQC_RaiseErrorHeaderFormat(
            "Expat fail",
            "%s",
            XML_ErrorString(XML_GetErrorCode(parser))
        );
}


void HQC_XmlParser_Free(HQC_XmlParser parser) {
    XML_ParserFree(parser);
}





