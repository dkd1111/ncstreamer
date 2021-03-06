/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


#ifndef NCSTREAMER_CEF_SRC_REMOTE_MESSAGE_TYPES_H_
#define NCSTREAMER_CEF_SRC_REMOTE_MESSAGE_TYPES_H_


namespace ncstreamer {
class RemoteMessage {
 public:
  enum class MessageType {
    kUndefined = 0,
    kStreamingStatusRequest,
    kStreamingStatusResponse,
    kStreamingStartRequest,
    kStreamingStartResponse,
    kStreamingStopRequest,
    kStreamingStopResponse,
    kSettingsQualityUpdateRequest,
    kSettingsQualityUpdateResponse,
    kNcStreamerExitRequest,
    kNcStreamerExitResponse,  // not used.
  };
};
}  // namespace ncstreamer


#endif  // NCSTREAMER_CEF_SRC_REMOTE_MESSAGE_TYPES_H_
