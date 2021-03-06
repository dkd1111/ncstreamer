/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


#ifndef NCSTREAMER_CEF_SRC_REMOTE_SERVER_H_
#define NCSTREAMER_CEF_SRC_REMOTE_SERVER_H_


#include <fstream>
#include <mutex>  // NOLINT
#include <string>
#include <thread>  // NOLINT
#include <vector>
#include <unordered_map>

#include "boost/asio/io_service.hpp"
#include "boost/property_tree/ptree.hpp"
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

#include "ncstreamer_cef/src/browser_app.h"


namespace ncstreamer {
class RemoteServer {
 public:
  static void SetUp(
      const BrowserApp *browser_app,
      uint16_t port);

  static void ShutDown();
  static RemoteServer *Get();

  void RespondStreamingStatus(
      int request_key,
      const std::string &status,
      const std::string &source_title,
      const std::string &user_name,
      const std::string &quality);

  void RespondStreamingStart(
      int request_key,
      const std::string &error);

  void RespondStreamingStop(
      int request_key,
      const std::string &error);

  void RespondSettingsQualityUpdate(
      int request_key,
      const std::string &error);

 private:
  class RequestCache {
   public:
    RequestCache();
    virtual ~RequestCache();

    int CheckIn(websocketpp::connection_hdl connection);
    websocketpp::connection_hdl CheckOut(int key);

   private:
    mutable std::mutex mutex_;
    std::unordered_map<int /*key*/, websocketpp::connection_hdl> cache_;
    int last_key_;
  };

  RemoteServer(
      const BrowserApp *browser_app,
      uint16_t port);

  virtual ~RemoteServer();

  void OnFail(websocketpp::connection_hdl connection);
  void OnOpen(websocketpp::connection_hdl connection);
  void OnClose(websocketpp::connection_hdl connection);
  void OnMessage(
      websocketpp::connection_hdl connection,
      websocketpp::connection<websocketpp::config::asio>::message_ptr msg);

  void OnStreamingStatusRequest(
      const websocketpp::connection_hdl &connection,
      const boost::property_tree::ptree &tree);

  void OnStreamingStartRequest(
      const websocketpp::connection_hdl &connection,
      const boost::property_tree::ptree &tree);

  void OnStreamingStopRequest(
      const websocketpp::connection_hdl &connection,
      const boost::property_tree::ptree &tree);

  void OnSettingsQualityUpdateRequest(
      const websocketpp::connection_hdl &connection,
      const boost::property_tree::ptree &tree);

  void OnNcStreamerExitRequest(
      const websocketpp::connection_hdl &connection,
      const boost::property_tree::ptree &tree);

  void LogError(const std::string &err_msg);
  void LogWarning(const std::string &warn_msg);
  void LogInfo(const std::string &info_msg);

  static RemoteServer *static_instance;

  const BrowserApp *const browser_app_;

  boost::asio::io_service io_service_;
  boost::asio::io_service::work io_service_work_;
  websocketpp::server<websocketpp::config::asio> server_;
  std::vector<std::thread> server_threads_;
  std::ofstream server_log_;

  RequestCache request_cache_;
};
}  // namespace ncstreamer


#endif  // NCSTREAMER_CEF_SRC_REMOTE_SERVER_H_
