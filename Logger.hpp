/*
 * reference:
 * https://github.com/Martinii89/OrganizeMyGarageOpenSource/blob/master/OrganizeMyGarageV2/logging.h
 *
 * TODO:
 *   - get a handle on enums with states / options
 *   - clean up the namespace? make it more intuitive to use?
 */

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <format>
#include <source_location>
#include <string_view>
// #include "flagpp/flags.hpp"

#include "bakkesmod/wrappers/cvarmanagerwrapper.h"

// import scoped_enum_bitmask;

#ifdef _WIN32
// ERROR macro is defined in Windows header
// To avoid conflict between these macro and declaration of ERROR / DEBUG in SEVERITY enum
// We save macro and undef it
#pragma push_macro("ERROR")
#pragma push_macro("DEBUG")
#undef ERROR
#undef DEBUG
#endif

namespace details {
struct FormatString {
      std::string          str;
      std::source_location loc {};

      FormatString(const char * str, const std::source_location & loc = std::source_location::current()) :
            str(str), loc(loc) {}
      FormatString(const std::string & str, const std::source_location & loc = std::source_location::current()) :
            str(str), loc(loc) {}
      FormatString(const std::string && str, const std::source_location & loc = std::source_location::current()) :
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
      FormatWString(const std::wstring & str, const std::source_location & loc = std::source_location::current()) :
            str(str), loc(loc) {}
      FormatWString(const std::wstring && str, const std::source_location & loc = std::source_location::current()) :
            str(str), loc(loc) {}

      [[nodiscard]] std::wstring GetLocation() const {
            auto basic_string = std::format("[{} ({}:{})]", loc.function_name(), loc.file_name(), loc.line());
            return std::wstring(basic_string.begin(), basic_string.end());
      }
};
}  // namespace details

enum class LOGLEVEL { INFO, DEBUG, WARNING, ERROR, OFF };

enum class LOGOPTIONS {
      NONE      = 0,
      PERSIST   = 1 << 0,  // write out to a file
      CONSOLE   = 1 << 1,  // write out to the bakkesmod console
      SOURCELOC = 1 << 2,  // include the source location
};

constexpr inline bool operator&(const LOGOPTIONS & lhs, const LOGOPTIONS & rhs) {
      return false;
}

class LOGGER {
private:
      // NOLINTBEGIN
      static inline LOGOPTIONS g_options  = LOGOPTIONS::NONE;  // default level of options...
      static inline LOGLEVEL   g_loglevel = LOGLEVEL::ERROR;   // default error level

      // NOLINTEND

      static inline std::shared_ptr<CVarManagerWrapper> g_cvarmanager;

      using FormatString  = details::FormatString;
      using FormatWString = details::FormatWString;

      template <typename... Args> static inline void LOG(const FormatString & format_str, Args &&... args) {
            g_cvarmanager->log(std::format("format_Str.str: {} ; # of argS: {}", format_str.str, sizeof...(args)));
            auto str = std::format(
                  "{}{}{}",
                  (g_options & LOGOPTIONS::SOURCELOC) ? format_str.GetLocation() : "",
                  (g_options & LOGOPTIONS::SOURCELOC) ? " " : "",
                  std::vformat(format_str.str, std::make_format_args(args...)));
            g_cvarmanager->log(std::move(str));
      }

      template <typename... Args> static inline void LOG(const FormatWString & wformat_str, Args &&... args) {
            g_cvarmanager->log(std::format("# of argS: {}", sizeof...(args)));
            auto str = std::format(
                  L"{}{}{}",
                  (g_options & LOGOPTIONS::SOURCELOC) ? wformat_str.GetLocation() : L"",
                  (g_options & LOGOPTIONS::SOURCELOC) ? L" " : L"",
                  std::vformat(wformat_str.str, std::make_wformat_args(args...)));
            g_cvarmanager->log(std::move(str));
      }

      // USING LOGLEVEL
      template <typename... Args>
      static inline void LOG(const LOGLEVEL & log_level, const FormatString & format_str, Args &&... args) {
            LOG(format_str, args...);
      }

      template <typename... Args>
      static inline void LOG(const LOGLEVEL & log_level, const FormatWString & wformat_str, Args &&... args) {
            if (log_level < g_loglevel) {
                  return;
            }
            LOG(wformat_str, args...);
      }

public:
      static inline void set_cvarmanager(std::shared_ptr<CVarManagerWrapper> cmw) { LOGGER::g_cvarmanager = cmw; }
      static inline void set_loglevel(const LOGLEVEL & nl) { g_loglevel = nl; }

      template <typename... Args> static inline void log_info(const FormatString & format_str, Args &&... args) {
            LOG(LOGLEVEL::INFO, format_str, args...);
      }

      template <typename... Args> static inline void log_info(const FormatWString & wformat_str, Args &&... args) {
            LOG(LOGLEVEL::INFO, wformat_str, args...);
      }

      template <typename... Args> static inline void log_debug(const FormatString & format_str, Args &&... args) {
            LOG(LOGLEVEL::DEBUG, format_str, args...);
      }

      template <typename... Args> static inline void log_debug(const FormatWString & wformat_str, Args &&... args) {
            LOG(LOGLEVEL::DEBUG, wformat_str, args...);
      }

      template <typename... Args> static inline void log_warning(const FormatString & format_str, Args &&... args) {
            LOG(LOGLEVEL::WARNING, format_str, args...);
      }

      template <typename... Args> static inline void log_warning(const FormatWString & wformat_str, Args &&... args) {
            LOG(LOGLEVEL::WARNING, wformat_str, args...);
      }

      template <typename... Args> static inline void log_error(const FormatString & format_str, Args &&... args) {
            LOG(LOGLEVEL::ERROR, format_str, args...);
      }

      template <typename... Args> static inline void log_error(const FormatWString & wformat_str, Args &&... args) {
            LOG(LOGLEVEL::ERROR, wformat_str, args...);
      }
};

#ifdef _WIN32
// We restore the ERROR Windows macro
#pragma pop_macro("ERROR")
#pragma pop_macro("DEBUG")
#endif

#endif
