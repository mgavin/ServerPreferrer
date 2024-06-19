/*
 * reference:
 * https://github.com/Martinii89/OrganizeMyGarageOpenSource/blob/master/OrganizeMyGarageV2/logging.h
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <format>
#include <source_location>
#include <string_view>

#include "bakkesmod/wrappers/cvarmanagerwrapper.h"

// TODO: create a class out of this someday
// TODO: take from martini's logger (garage organizer thingy)

namespace LOGGER {
namespace {
        struct FormatString {
                std::string_view     str;
                std::source_location loc {};

                FormatString(
                        const char *                 str,
                        const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}
                FormatString(
                        const std::string &&         str,
                        const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}

                [[nodiscard]] std::string GetLocation() const {
                        return std::format(
                                "[{} ({}:{})]",
                                loc.function_name(),
                                loc.file_name(),
                                loc.line());
                }
        };

        struct FormatWString {
                std::wstring_view    str;
                std::source_location loc {};

                FormatWString(
                        const wchar_t *              str,
                        const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}
                FormatWString(
                        const std::wstring &&        str,
                        const std::source_location & loc = std::source_location::current()) :
                        str(str), loc(loc) {}

                [[nodiscard]] std::wstring GetLocation() const {
                        auto basic_string = std::format(
                                "[{} ({}:{})]",
                                loc.function_name(),
                                loc.file_name(),
                                loc.line());
                        return std::wstring(basic_string.begin(), basic_string.end());
                }
        };
}  // namespace

enum class LOGLEVEL {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
};
enum class LOGOPTIONS {
        PERSIST = 1 << 0,  // write out to a file
        CONSOLE = 1 << 1,  // write out to the bakkesmod console
};

LOGLEVEL level = LOGLEVEL::INFO;  // default level

extern std::shared_ptr<CVarManagerWrapper> _globalCVarManager;
template<typename... Args>
inline void LOG(const FormatString & format_str, Args &&... args) {
        auto str = std::format(
                "{} {}",
                std::vformat(
                        format_str.str,
                        std::make_format_args(std::forward<Args>(args)...)),
                format_str.GetLocation());
        _globalCVarManager->log(std::vformat(format_str, std::make_format_args(args...)));
}

template<typename... Args>
inline void LOG(const FormatWString & wformat_str, Args &&... args) {
        auto str = std::format(
                L"{} {}",
                std::vformat(
                        wformat_str.str,
                        std::make_wformat_args(std::forward<Args>(args)...)),
                wformat_str.GetLocation());
        _globalCVarManager->log(std::vformat(wformat_str, std::make_wformat_args(args...)));
}

};  // namespace LOGGER

#endif
