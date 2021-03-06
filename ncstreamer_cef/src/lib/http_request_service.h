/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


#ifndef NCSTREAMER_CEF_SRC_LIB_HTTP_REQUEST_SERVICE_H_
#define NCSTREAMER_CEF_SRC_LIB_HTTP_REQUEST_SERVICE_H_


#include <memory>
#include <string>
#include <thread>  // NOLINT

#include "boost/asio/io_service.hpp"
#include "boost/property_tree/ptree.hpp"

#include "ncstreamer_cef/src/lib/http_request.h"


namespace ncstreamer {
class HttpRequestService {
 public:
  HttpRequestService();
  virtual ~HttpRequestService();

  void Get(
      const std::string &uri,
      const HttpRequest::ErrorHandler &err_handler,
      const HttpRequest::OpenHandler &open_handler,
      const HttpRequest::ReadHandler &read_handler,
      const HttpRequest::ResponseCompleteHandler &complete_handler);

  void Get(
      const std::string &uri,
      const HttpRequest::ErrorHandler &err_handler,
      const HttpRequest::ResponseCompleteHandler &complete_handler);

  void Post(
      const std::string &uri,
      const boost::property_tree::ptree &post_content,
      const HttpRequest::ErrorHandler &err_handler,
      const HttpRequest::OpenHandler &open_handler,
      const HttpRequest::ReadHandler &read_handler,
      const HttpRequest::ResponseCompleteHandler &complete_handler);

  void Post(
      const std::string &uri,
      const boost::property_tree::ptree &post_content,
      const HttpRequest::ErrorHandler &err_handler,
      const HttpRequest::ResponseCompleteHandler &complete_handler);

 private:
  boost::asio::io_service io_service_;
  boost::asio::io_service::work io_service_work_;
  std::thread io_thread_;

  std::shared_ptr<HttpRequest> http_request_;
};
}  // namespace ncstreamer


#endif  // NCSTREAMER_CEF_SRC_LIB_HTTP_REQUEST_SERVICE_H_
