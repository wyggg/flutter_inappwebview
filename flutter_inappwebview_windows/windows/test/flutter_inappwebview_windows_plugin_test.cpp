#include <flutter/method_call.h>
#include <flutter/method_result_functions.h>
#include <flutter/standard_method_codec.h>
#include <gtest/gtest.h>
#include <windows.h>

#include <memory>
#include <string>
#include <variant>

#include "flutter_inappwebview_windows_plugin.h"
#include "in_app_webview/webview_channel_delegate.h"

namespace flutter_inappwebview_windows {
namespace test {

namespace {

using flutter::EncodableMap;
using flutter::EncodableValue;
using flutter::MethodCall;
using flutter::MethodResultFunctions;

}  // namespace

TEST(FlutterInappwebviewWindowsPlugin, GetPlatformVersion) {
  FlutterInappwebviewWindowsPlugin plugin;
  // Save the reply value from the success callback.
  std::string result_string;
  plugin.HandleMethodCall(
      MethodCall("getPlatformVersion", std::make_unique<EncodableValue>()),
      std::make_unique<MethodResultFunctions<>>(
          [&result_string](const EncodableValue* result) {
            result_string = std::get<std::string>(*result);
          },
          nullptr, nullptr));

  // Since the exact string varies by host, just ensure that it's a string
  // with the expected format.
  EXPECT_TRUE(result_string.rfind("Windows ", 0) == 0);
}

TEST(WebViewChannelDelegatePermissionRequestCallback, NullResultUsesDefaultBehavior) {
  flutter_inappwebview_plugin::WebViewChannelDelegate::PermissionRequestCallback callback;
  bool default_behavior_called = false;
  callback.defaultBehaviour = [&default_behavior_called](const auto result) {
    EXPECT_FALSE(result.has_value());
    default_behavior_called = true;
  };

  callback.Success();

  EXPECT_TRUE(default_behavior_called);
}

TEST(WebViewChannelDelegatePermissionRequestCallback, NonMapResultUsesDefaultBehavior) {
  flutter_inappwebview_plugin::WebViewChannelDelegate::PermissionRequestCallback callback;
  bool default_behavior_called = false;
  callback.defaultBehaviour = [&default_behavior_called](const auto result) {
    EXPECT_FALSE(result.has_value());
    default_behavior_called = true;
  };
  EncodableValue result(true);

  callback.Success(result);

  EXPECT_TRUE(default_behavior_called);
}

}  // namespace test
}  // namespace flutter_inappwebview_windows
