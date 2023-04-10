/*
 *  Copyright (C) 2023 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "RendererStarfish.h"

#include "../RenderFactory.h"
#include "DVDCodecs/Video/DVDVideoCodecStarfish.h"
#include "ServiceBroker.h"
#include "rendering/gles/RenderSystemGLES.h"
#include "settings/MediaSettings.h"
#include "utils/log.h"
#include "windowing/wayland/WinSystemWaylandWebOS.h"

CRendererStarfish::CRendererStarfish()
{
  CLog::LogF(LOGINFO, "CRendererStarfish: Instanced");
}

CRendererStarfish::~CRendererStarfish()
{
  if (m_acbID) {
    long tid = 0;
    m_acbAPI.setState(m_acbID, APPSTATE_FOREGROUND, PLAYSTATE_UNLOADED, &tid);
    m_acbAPI.finalize(m_acbID);
    m_acbAPI.destroy(m_acbID);
  }
}

CBaseRenderer* CRendererStarfish::Create(CVideoBuffer* buffer)
{
  if (buffer && dynamic_cast<CStarfishVideoBuffer*>(buffer))
    return new CRendererStarfish();
  return nullptr;
}

DllAcbAPI CRendererStarfish::m_acbAPI;

bool CRendererStarfish::Configure(const VideoPicture& picture, float fps, unsigned int orientation)
{
  auto windowName = dynamic_cast<KODI::WINDOWING::WAYLAND::CWinSystemWaylandWebOS*>(CServiceBroker::GetWinSystem())
        ->GetExportedWindowName();

  if (!windowName.size()) {
    if (!m_acbAPI.IsLoaded()) {
      m_acbAPI.EnableDelayedUnload(false);
      if (!m_acbAPI.Load()) {
        CLog::LogF(LOGERROR, "ACB library load failed");
        return false;
      }
    }

    auto mediaID = dynamic_cast<CStarfishVideoBuffer*>(picture.videoBuffer)->mediaID;
    m_acbID = m_acbAPI.create();
    CLog::LogF(LOGDEBUG, "Initializing ACB, mediaID: {}, ACB ID: {}", mediaID, m_acbID);
    if (!m_acbAPI.initialize(m_acbID, PLAYER_TYPE_MSE, getenv("APPID"), &CRendererStarfish::AcbCallback)) {
      CLog::LogF(LOGERROR, "ACB initialization failed");
      m_acbID = 0;
      return false;
    } else {
      long tid = 0;
      // FIXME: change to SINK_TYPE_MAIN after webos-userland update
      m_acbAPI.setSinkType(m_acbID, SINK_TYPE_AUTO);
      m_acbAPI.setMediaId(m_acbID, mediaID.c_str());
      m_acbAPI.setState(m_acbID, APPSTATE_FOREGROUND, PLAYSTATE_LOADED, &tid);

      m_gotFirstFrame = false;
    }
  }

  m_iFlags = GetFlagsChromaPosition(picture.chroma_position) |
             GetFlagsColorMatrix(picture.color_space, picture.iWidth, picture.iHeight) |
             GetFlagsColorPrimaries(picture.color_primaries) |
             GetFlagsStereoMode(picture.stereoMode);

  return CLinuxRendererGLES::Configure(picture, fps, orientation);
}

bool CRendererStarfish::Register()
{
  VIDEOPLAYER::CRendererFactory::RegisterRenderer("starfish", CRendererStarfish::Create);
  return true;
}

void CRendererStarfish::ManageRenderArea()
{
  CBaseRenderer::ManageRenderArea();

  if ((m_exportedDestRect != m_destRect || m_exportedSourceRect != m_sourceRect) &&
      !m_sourceRect.IsEmpty() && !m_destRect.IsEmpty())
  {
    auto origRect =
        CRect{0, 0, static_cast<float>(m_sourceWidth), static_cast<float>(m_sourceHeight)};
    if (m_acbID != 0) {
      long tid;
      CLog::LogF(LOGDEBUG, "ACB display: {} {} {}x{} ; {} {} {}x{}", m_sourceRect.x1, m_sourceRect.y1, m_sourceRect.Width(), m_sourceRect.Height(), m_destRect.x1, m_destRect.y1, m_destRect.Width(), m_destRect.Height());
      m_acbAPI.setCustomDisplayWindow(m_acbID, m_sourceRect.x1, m_sourceRect.y1, m_sourceRect.Width(), m_sourceRect.Height(), m_destRect.x1, m_destRect.y1, m_destRect.Width(), m_destRect.Height(), false, &tid);
    } else {
      static_cast<KODI::WINDOWING::WAYLAND::CWinSystemWaylandWebOS*>(CServiceBroker::GetWinSystem())
          ->SetExportedWindow(origRect, m_sourceRect, m_destRect);
    }
    m_exportedSourceRect = m_sourceRect;
    m_exportedDestRect = m_destRect;
  }
}

bool CRendererStarfish::Supports(ERENDERFEATURE feature) const
{
  return (feature == RENDERFEATURE_ZOOM || feature == RENDERFEATURE_STRETCH ||
          feature == RENDERFEATURE_PIXEL_RATIO || feature == RENDERFEATURE_VERTICAL_SHIFT ||
          feature == RENDERFEATURE_ROTATION);
}

void CRendererStarfish::AddVideoPicture(const VideoPicture& picture, int index)
{
  if (m_acbID && !m_gotFirstFrame) {
    long tid;
    auto mediaVideoData = dynamic_cast<CStarfishVideoBuffer*>(picture.videoBuffer)->mediaVideoData;
    if (mediaVideoData.size()) {
      m_acbAPI.setMediaVideoData(m_acbID, mediaVideoData.c_str());
      // TODO: change to PLAYSTATE_PLAYING after webos-userland update
      m_acbAPI.setState(m_acbID, APPSTATE_FOREGROUND, PLAYSTATE_SEAMLESS_LOADED, &tid);
      CLog::LogF(LOGDEBUG, "ACB PLAYING request: {}", tid);
      m_gotFirstFrame = true;
    }
  }
}

void CRendererStarfish::ReleaseBuffer(int idx)
{
}

CRenderInfo CRendererStarfish::GetRenderInfo()
{
  CRenderInfo info;
  info.max_buffer_size = 4;
  return info;
}

bool CRendererStarfish::LoadShadersHook()
{
  return true;
}

bool CRendererStarfish::RenderHook(int index)
{
  return true;
}

bool CRendererStarfish::CreateTexture(int index)
{
  return true;
}

void CRendererStarfish::DeleteTexture(int index)
{
}

bool CRendererStarfish::UploadTexture(int index)
{
  return true;
}

bool CRendererStarfish::IsGuiLayer()
{
  return false;
}

void CRendererStarfish::RenderUpdateVideo(bool clear, unsigned int flags, unsigned int alpha)
{
}

void CRendererStarfish::AcbCallback(long acbId, long taskId, long eventType, long appState, long playState, const char *reply) {
  std::string logstr = reply ? reply : "";
  CLog::LogF(LOGDEBUG, "acbid: {}, taskid: {}, eventType: {}, appState: {}, playState: {}, reply: {}", acbId, taskId, eventType, appState, playState, logstr);
}
