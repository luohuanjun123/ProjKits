/*
 *  Copyright 2015 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_BASE_FILE_ROTATING_LOG_SINK_H_
#define WEBRTC_BASE_FILE_ROTATING_LOG_SINK_H_

#include <string>

#include "base/constructormagic.h"
#include "base/filerotatingstream.h"
#include "base/logging.h"
#include "base/scoped_ptr.h"

namespace rtc {

// Log sink that uses a FileRotatingStream to write to disk.
// Init() must be called before adding this sink.
class FileRotatingLogSink : public LogSink {
 public:
  // |num_log_files| must be greater than 1 and |max_log_size| must be greater
  // than 0.
  FileRotatingLogSink(const std::string& log_dir_path,
                      const std::string& log_prefix,
                      size_t max_log_size,
                      size_t num_log_files);
  ~FileRotatingLogSink() override;

  // Writes the message to the current file. It will spill over to the next
  // file if needed.
  void OnLogMessage(const std::string& message) override;

  // Deletes any existing files in the directory and creates a new log file.
  virtual bool Init();

  // Disables buffering on the underlying stream.
  bool DisableBuffering();

 protected:
  explicit FileRotatingLogSink(FileRotatingStream* stream);

 private:
  scoped_ptr<FileRotatingStream> stream_;

  RTC_DISALLOW_COPY_AND_ASSIGN(FileRotatingLogSink);
};

// Log sink that uses a CallSessionFileRotatingStream to write to disk.
// Init() must be called before adding this sink.
class CallSessionFileRotatingLogSink : public FileRotatingLogSink {
 public:
  CallSessionFileRotatingLogSink(const std::string& log_dir_path,
                                 size_t max_total_log_size);
  ~CallSessionFileRotatingLogSink() override;

 private:
  RTC_DISALLOW_COPY_AND_ASSIGN(CallSessionFileRotatingLogSink);
};

}  // namespace rtc

#endif  // WEBRTC_BASE_FILE_ROTATING_LOG_SINK_H_
