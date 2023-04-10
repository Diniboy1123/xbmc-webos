/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "DynamicDll.h"
#include <appswitching-control-block/AcbAPI.h>

class DllAcbAPIInterface
{
public:
  virtual ~DllAcbAPIInterface() = default;

  virtual long create(void) = 0;
  virtual bool initialize(long acbId, AcbAPI_PlayerType playerType, const char *appId, AcbAPI_Callback callback) = 0;
  virtual int setMediaVideoData(long acbId, const char *payload) = 0;
  virtual int setState(long acbId, AcbAPI_AppState appState, AcbAPI_PlayState playState, long *taskId) = 0;
  virtual bool setMediaId(long acbId, const char *connId) = 0;
  virtual bool setSinkType(long acbId, AcbAPI_SinkType sinkType) = 0;
  virtual int setDisplayWindow(long acbId, long x, long y, long w, long h, bool fullScreen, long *taskId) = 0;
  virtual int setCustomDisplayWindow(long acbId, long in_x, long in_y, long in_w, long in_h, long out_x, long out_y, long out_w, long out_h, bool fullScreen, long *taskId) = 0;
  virtual int finalize(long acbId) = 0;
  virtual void destroy(long acbId) = 0;
};

class DllAcbAPI : public DllDynamic, DllAcbAPIInterface
{
  DECLARE_DLL_WRAPPER(DllAcbAPI, "libAcbAPI.so.1")

  DEFINE_METHOD0(long, create)
  DEFINE_METHOD4(bool, initialize, (long p1, AcbAPI_PlayerType p2, const char *p3, AcbAPI_Callback p4));
  DEFINE_METHOD2(int, setMediaVideoData, (long p1, const char *p2));
  DEFINE_METHOD4(int, setState, (long p1, AcbAPI_AppState p2, AcbAPI_PlayState p3, long *p4));
  DEFINE_METHOD2(bool, setMediaId, (long p1, const char *p2));
  DEFINE_METHOD2(bool, setSinkType, (long p1, AcbAPI_SinkType p2));
  DEFINE_METHOD7(int, setDisplayWindow, (long p1, long p2, long p3, long p4, long p5, bool p6, long *p7));
  DEFINE_METHOD11(int, setCustomDisplayWindow, (long p1, long p2, long p3, long p4, long p5, long p6, long p7, long p8, long p9, bool p10, long* p11));
  DEFINE_METHOD1(int, finalize, (long p1));
  DEFINE_METHOD1(void, destroy, (long p1));

  BEGIN_METHOD_RESOLVE()
    RESOLVE_METHOD_RENAME(AcbAPI_create, create)
    RESOLVE_METHOD_RENAME(AcbAPI_initialize, initialize)
    RESOLVE_METHOD_RENAME(AcbAPI_setMediaVideoData, setMediaVideoData)
    RESOLVE_METHOD_RENAME(AcbAPI_setState, setState)
    RESOLVE_METHOD_RENAME(AcbAPI_setMediaId, setMediaId)
    RESOLVE_METHOD_RENAME(AcbAPI_setSinkType, setSinkType)
    RESOLVE_METHOD_RENAME(AcbAPI_setDisplayWindow, setDisplayWindow)
    RESOLVE_METHOD_RENAME(AcbAPI_setCustomDisplayWindow, setCustomDisplayWindow)
    RESOLVE_METHOD_RENAME(AcbAPI_finalize, finalize)
    RESOLVE_METHOD_RENAME(AcbAPI_destroy, destroy)
  END_METHOD_RESOLVE()

};
