#pragma once

#include <string>
#include <regex>
#include <sstream>
#include <mutex>

std::mutex error_message_mutex;

std::string apply_indent(std::string in) {
  return std::regex_replace(
    in,
    std::regex{"(^|\n)"},
    std::string{"$1"} + std::string(3, ' ')
  );
}

template<typename T>
std::string format_member(
  const std::string & name,
  const T& member
) {
  std::stringstream ss;
  ss << name << ":" << std::endl;
  ss << apply_indent(member.ToString());
  return ss.str();
}

template<>
std::string format_member<char>(
  const std::string & name,
  const char& member
) {
  std::stringstream ss;
  ss << name << ":" << std::endl;
  ss << apply_indent(
    [member](){ std::stringstream ss; ss << member; return ss.str(); }()
  );
  return ss.str();
}

template<>
std::string format_member<size_t>(
  const std::string & name,
  const size_t& member
) {
  std::stringstream ss;
  ss << name << ":" << std::endl;
  ss << apply_indent(
    std::to_string(member)
  );
  return ss.str();
}

template<>
std::string format_member<std::atomic<size_t>>(
  const std::string & name,
  const std::atomic<size_t>& member
) {
  std::stringstream ss;
  ss << name << ":" << std::endl;
  ss << apply_indent(
    std::to_string(member)
  );
  return ss.str();
}

template<>
std::string format_member<const void *>(
  const std::string & name,
  const void* const& member
) {
  std::stringstream ss;
  ss << name << ":" << std::endl;
  ss << apply_indent(
    [member](){ std::stringstream ss; ss << member; return ss.str(); }()
  );
  return ss.str();
}
