/**
 * Copyright (C) 2017 NCSOFT Corporation
 */


#include "src/obs.h"

#include "obs-studio/libobs/obs.h"


namespace ncstreamer {
void Obs::SetUp() {
  obs_startup("en-US", nullptr, nullptr);
}


void Obs::ShutDown() {
  obs_shutdown();
}
}  // namespace ncstreamer
