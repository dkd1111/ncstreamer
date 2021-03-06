/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


#include "ncstreamer_cef/src/command_line.h"

#include <codecvt>
#include <locale>
#include <memory>

#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ptree.hpp"


namespace ncstreamer {
CommandLine::CommandLine(const std::wstring &cmd_line)
    : is_renderer_{false},
      hides_settings_{false},
      video_quality_{},
      shows_sources_all_{false},
      sources_{},
      locale_{},
      ui_uri_{},
      remote_port_{0},
      in_memory_local_storage_{false},
      designated_user_{} {
  CefRefPtr<CefCommandLine> cef_cmd_line =
      CefCommandLine::CreateCommandLine();
  cef_cmd_line->InitFromString(cmd_line);

  const std::wstring &process_type =
      cef_cmd_line->GetSwitchValue(L"type");
  is_renderer_ = (process_type == L"renderer");

  hides_settings_ = ReadBool(cef_cmd_line, L"hides-settings", false);

  const std::wstring &video_quality =
      cef_cmd_line->GetSwitchValue(L"video-quality");
  video_quality_ = video_quality.empty() ? L"medium" : video_quality;

  shows_sources_all_ = ReadBool(cef_cmd_line, L"shows-sources-all", true);

  const std::wstring &sources_arg =
      cef_cmd_line->GetSwitchValue(L"sources");
  if (sources_arg.empty() == false) {
    sources_ = ParseSourcesArgument(sources_arg);
  }

  const std::wstring &locale =
      cef_cmd_line->GetSwitchValue(L"locale");
  locale_ = locale.empty() ? L"en-US" : locale;

  ui_uri_ = cef_cmd_line->GetSwitchValue(L"ui-uri");

  const std::wstring &remote_port =
      cef_cmd_line->GetSwitchValue(L"remote-port");
  try {
    remote_port_ = static_cast<uint16_t>(std::stoi(remote_port));
  } catch (...) {
    remote_port_ = 9002;
  }

  in_memory_local_storage_ =
      ReadBool(cef_cmd_line, L"in-memory-local-storage", false);

  designated_user_ = cef_cmd_line->GetSwitchValue(L"designated-user");
}


CommandLine::~CommandLine() {
}


bool CommandLine::ReadBool(
    const CefRefPtr<CefCommandLine> &cmd_line,
    const std::wstring &key,
    const bool &default_value) {
  if (cmd_line->HasSwitch(key) == false) {
    return default_value;
  }

  const std::wstring &value = cmd_line->GetSwitchValue(key);
  if (value == L"" ||
      value == L"true") {
    return true;
  }

  if (value == L"false") {
    return false;
  }

  return default_value;
}


std::vector<std::string>
    CommandLine::ParseSourcesArgument(const std::wstring &arg) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::string utf8 = converter.to_bytes(arg);

  std::vector<std::string> sources;
  boost::property_tree::ptree root;
  std::stringstream root_ss{utf8};
  try {
    boost::property_tree::read_json(root_ss, root);
    const auto &arr = root.get_child("sources", {});
    for (const auto &elem : arr) {
      const boost::property_tree::ptree &obj = elem.second;
      const auto &source = obj.get<std::string>("title");
      sources.emplace_back(source);
    }
  } catch (const std::exception &/*e*/) {
    sources.clear();
  }
  return sources;
}
}  // namespace ncstreamer
