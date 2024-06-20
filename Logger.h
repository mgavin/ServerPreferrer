/*
 * reference:
 * https://github.com/Martinii89/OrganizeMyGarageOpenSource/blob/master/OrganizeMyGarageV2/logging.h
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <bitset>
#include <format>
#include <source_location>
#include <string_view>

#include "flagpp/flags.hpp"

#include "bakkesmod/wrappers/cvarmanagerwrapper.h"

// TODO: create a class out of this someday
// TODO: take from martini's logger (garage organizer thingy)

extern std::shared_ptr<CVarManagerWrapper> _globalCVarManager;

namespace LOGGER {
namespace {
        struct FormatString {
                std::string_view     str;
                std::source_location loc {};

                FormatString(const char * str, const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}
                FormatString(
                        const std::string &          str,
                        const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}
                FormatString(
                        const std::string &&         str,
                        const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}

                [[nodiscard]] std::string GetLocation() const {
                        return std::format("[{} ({}:{})]", loc.function_name(), loc.file_name(), loc.line());
                }
        };

        struct FormatWString {
                std::wstring_view    str;
                std::source_location loc {};

                FormatWString(const wchar_t * str, const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}
                FormatWString(
                        const std::wstring &         str,
                        const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}
                FormatWString(
                        const std::wstring &&        str,
                        const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}

                [[nodiscard]] std::wstring GetLocation() const {
                        auto basic_string =
                                std::format("[{} ({}:{})]", loc.function_name(), loc.file_name(), loc.line());
                        return std::wstring(basic_string.begin(), basic_string.end());
                }
        };
}  // namespace

enum class LOGLEVEL {
        INFO,
        DEBUG,
        WARNING,
#ifdef ERROR
#undef ERROR
        ERROR,
#endif
};

enum class LOGOPTIONS {
        NONE      = 0,
        PERSIST   = 1 << 0,  // write out to a file
        CONSOLE   = 1 << 1,  // write out to the bakkesmod console
        SOURCELOC = 1 << 2,  // include the source location
};
}  // namespace LOGGER

template<>
constexpr bool flagpp::enabled<LOGGER::LOGOPTIONS> = true;

namespace LOGGER {

LOGOPTIONS options = LOGOPTIONS::NONE;  // default level of options...

template<typename... Args>
inline void LOG(const FormatString & format_str, Args &&... args) {
        auto str = std::format(
                "{}{}{}",
                options & LOGOPTIONS::SOURCELOC ? format_str.GetLocation() : "",
                options & LOGOPTIONS::SOURCELOC ? " " : "",
                std::vformat(format_str.str, std::make_format_args(std::forward<Args>(args)...)));
        _globalCVarManager->log(std::move(str));
}

template<typename... Args>
inline void LOG(const FormatWString & wformat_str, Args &&... args) {
        auto str = std::format(
                L"{}{}{}",
                options & LOGOPTIONS::SOURCELOC ? wformat_str.GetLocation() : "",
                options & LOGOPTIONS::SOURCELOC ? " " : "",
                std::vformat(wformat_str.str, std::make_wformat_args(std::forward<Args>(args)...)));
        _globalCVarManager->log(std::move(str));
}

};  // namespace LOGGER

#endif
