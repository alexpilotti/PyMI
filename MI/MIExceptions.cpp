#include "stdafx.h"
#include "MIExceptions.h"
#include <locale>
#include <codecvt>
#include <cstdint>


using namespace MI;

const MI_Char* MI::MI_RESULT_STRINGS[] =
{
    "MI_RESULT_OK",
    "MI_RESULT_FAILED",
    "MI_RESULT_ACCESS_DENIED",
    "MI_RESULT_INVALID_NAMESPACE",
    "MI_RESULT_INVALID_PARAMETER",
    "MI_RESULT_INVALID_CLASS",
    "MI_RESULT_NOT_FOUND",
    "MI_RESULT_NOT_SUPPORTED",
    "MI_RESULT_CLASS_HAS_CHILDREN",
    "MI_RESULT_CLASS_HAS_INSTANCES",
    "MI_RESULT_INVALID_SUPERCLASS",
    "MI_RESULT_ALREADY_EXISTS",
    "MI_RESULT_NO_SUCH_PROPERTY",
    "MI_RESULT_TYPE_MISMATCH",
    "MI_RESULT_QUERY_LANGUAGE_NOT_SUPPORTED",
    "MI_RESULT_INVALID_QUERY",
    "MI_RESULT_METHOD_NOT_AVAILABLE",
    "MI_RESULT_METHOD_NOT_FOUND",
    "MI_RESULT_NAMESPACE_NOT_EMPTY",
    "MI_RESULT_INVALID_ENUMERATION_CONTEXT",
    "MI_RESULT_INVALID_OPERATION_TIMEOUT",
    "MI_RESULT_PULL_HAS_BEEN_ABANDONED",
    "MI_RESULT_PULL_CANNOT_BE_ABANDONED",
    "MI_RESULT_FILTERED_ENUMERATION_NOT_SUPPORTED",
    "MI_RESULT_CONTINUATION_ON_ERROR_NOT_SUPPORTED",
    "MI_RESULT_SERVER_LIMITS_EXCEEDED",
    "MI_RESULT_SERVER_IS_SHUTTING_DOWN"
};

const char* Exception::what() const noexcept
{
    return this->m_message.c_str();
}

void Exception::SetMessageFromWString(const std::string& message)
{
    // Convert to UTF8
    //std::string_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
   //this->m_message = cv.to_bytes(message).c_str();
    this->m_message = message;
}

std::string MIException::MIResultToWString(MI_Result miResult) const
{

    if (miResult < sizeof(MI_RESULT_STRINGS) / sizeof(MI_RESULT_STRINGS[0]))
    {
        return MI_RESULT_STRINGS[miResult];
    }
    else
    {
        return "Unknown MI_Result";
    }
}

MIException::MIException(MI_Result result, MI_Uint32 errorCode, const std::string& message) :
    m_result(result), m_errorCode(errorCode), Exception(message)
{
    if (!message.length())
    {
        this->SetMessageFromWString(this->MIResultToWString(this->m_result));
    }
}
