/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


#include "ncstreamer_cef/src/render_process/render_load_handler.h"

#include "include/cef_v8.h"
#include "include/wrapper/cef_helpers.h"

#include "ncstreamer_cef/src/render_process_message_types.h"


namespace ncstreamer {
RenderLoadHandler::RenderLoadHandler() {
}


RenderLoadHandler::~RenderLoadHandler() {
}


void RenderLoadHandler::OnLoadEnd(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int httpStatusCode) {
  CEF_REQUIRE_RENDERER_THREAD();

  if (frame->IsValid() == false ||
      frame->IsMain() == false ||
      frame->IsFocused() == false) {
    return;
  }

  const Dimension<int> &gap = GetScrollGap(frame);
  if (gap.empty() == true) {
    return;
  }

  CefString msgType{RenderProcessMessage::kScrollGap};
  CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(msgType);
  CefRefPtr<CefListValue> args = msg->GetArgumentList();
  args->SetInt(0, gap.width());
  args->SetInt(1, gap.height());
  browser->SendProcessMessage(PID_BROWSER, msg);
}


Dimension<int>
    RenderLoadHandler::GetScrollGap(CefRefPtr<CefFrame> frame) const {
  CEF_REQUIRE_RENDERER_THREAD();

  static const CefString kJsCode =
      L"(function() {"
      L"  var e = document.documentElement;"
      L"  if (!e) {"
      L"    return null;"
      L"  }"
      L"  return {"
      L"    w: e.scrollWidth - e.clientWidth,"
      L"    h: e.scrollHeight - e.clientHeight"
      L"  };"
      L"})()";

  CefRefPtr<CefV8Context> context = frame->GetV8Context();
  CefRefPtr<CefV8Value> returnValue;
  CefRefPtr<CefV8Exception> exception;
  Dimension<int> gap{0, 0};

  context->Enter();
  bool success = context->Eval(kJsCode, returnValue, exception);
  if (success) {
    gap.set_width(returnValue->GetValue(L"w")->GetIntValue());
    gap.set_height(returnValue->GetValue(L"h")->GetIntValue());
  }
  context->Exit();

  return gap;
}
}  // namespace ncstreamer
