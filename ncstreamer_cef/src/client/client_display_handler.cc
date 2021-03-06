/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


#include "ncstreamer_cef/src/client/client_display_handler.h"

#include <string>

#include "include/wrapper/cef_helpers.h"
#include "windows.h"  // NOLINT


namespace ncstreamer {
ClientDisplayHandler::ClientDisplayHandler() {
}


ClientDisplayHandler::~ClientDisplayHandler() {
}


void ClientDisplayHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                         const CefString &title) {
  CEF_REQUIRE_UI_THREAD();

  HWND hwnd = browser->GetHost()->GetWindowHandle();
  ::SetWindowTextW(hwnd, title.c_str());
}
}  // namespace ncstreamer
