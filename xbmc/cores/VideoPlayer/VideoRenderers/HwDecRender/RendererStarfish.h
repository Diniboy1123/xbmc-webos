/*
 *  Copyright (C) 2023 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "cores/VideoPlayer/VideoRenderers/LinuxRendererGLES.h"
#include "DllAcbAPI.h"

class CRendererStarfish : public CLinuxRendererGLES
{
public:
  CRendererStarfish();
  ~CRendererStarfish() override;

  // Registration
  static CBaseRenderer* Create(CVideoBuffer* buffer);
  static bool Register();

  // Player functions
  void AddVideoPicture(const VideoPicture& picture, int index) override;
  void ReleaseBuffer(int idx) override;

  // Feature support
  CRenderInfo GetRenderInfo() override;

  bool IsGuiLayer() override;
  bool Configure(const VideoPicture& picture, float fps, unsigned int orientation) override;
  bool Supports(ERENDERFEATURE feature) const override;

protected:
  // textures
  bool UploadTexture(int index) override;
  void RenderUpdateVideo(bool clear, unsigned int flags, unsigned int alpha) override;
  void DeleteTexture(int index) override;
  bool CreateTexture(int index) override;

  // hooks for hw dec renderer
  bool LoadShadersHook() override;
  bool RenderHook(int index) override;
  void ManageRenderArea() override;

private:
  static DllAcbAPI m_acbAPI;

  CRect m_exportedSourceRect;
  CRect m_exportedDestRect;

  bool m_gotFirstFrame{false};
  long m_acbID{0};
  static void AcbCallback(long acbId, long taskId, long eventType, long appState, long playState, const char *reply);
};
