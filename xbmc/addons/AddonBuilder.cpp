/*
 *  Copyright (C) 2016-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "addons/AddonBuilder.h"

#include "ServiceBroker.h"
#include "addons/ContextMenuAddon.h"
#include "addons/FontResource.h"
#include "addons/GameResource.h"
#include "addons/ImageResource.h"
#include "addons/LanguageResource.h"
#include "addons/PluginSource.h"
#include "addons/Repository.h"
#include "addons/Scraper.h"
#include "addons/Service.h"
#include "addons/Skin.h"
#include "addons/UISoundsResource.h"
#include "addons/Webinterface.h"
#include "addons/addoninfo/AddonInfo.h"
#include "games/addons/GameClient.h"
#include "games/controllers/Controller.h"
#include "pvr/addons/PVRClient.h"
#include "utils/StringUtils.h"

using namespace KODI;

namespace ADDON
{

AddonPtr CAddonBuilder::Generate(const AddonInfoPtr& info, AddonType type)
{
  if (!info || info->ID().empty())
    return AddonPtr();

  if (type == AddonType::ADDON_UNKNOWN)
    type = info->MainType();
  if (type == AddonType::ADDON_UNKNOWN)
    return std::make_shared<CAddon>(info, AddonType::ADDON_UNKNOWN);

  // Handle screensaver special cases
  if (type == AddonType::ADDON_SCREENSAVER)
  {
    // built in screensaver or python screensaver
    if (StringUtils::StartsWithNoCase(info->ID(), "screensaver.xbmc.builtin.") ||
        URIUtils::HasExtension(info->LibName(), ".py"))
      return std::make_shared<CAddon>(info, type);
  }

  // Handle audio encoder special cases
  if (type == AddonType::ADDON_AUDIOENCODER)
  {
    // built in audio encoder
    if (StringUtils::StartsWithNoCase(info->ID(), "audioencoder.kodi.builtin."))
      return std::make_shared<CAddonDll>(info, type);
  }

  switch (type)
  {
    case AddonType::ADDON_AUDIODECODER:
    case AddonType::ADDON_AUDIOENCODER:
    case AddonType::ADDON_IMAGEDECODER:
    case AddonType::ADDON_INPUTSTREAM:
    case AddonType::ADDON_PERIPHERALDLL:
    case AddonType::ADDON_PVRDLL:
    case AddonType::ADDON_VFS:
    case AddonType::ADDON_VIZ:
    case AddonType::ADDON_SCREENSAVER:
      return std::make_shared<CAddonDll>(info, type);
    case AddonType::ADDON_GAMEDLL:
      return std::make_shared<GAME::CGameClient>(info);
    case AddonType::ADDON_PLUGIN:
    case AddonType::ADDON_SCRIPT:
      return std::make_shared<CPluginSource>(info, type);
    case AddonType::ADDON_SCRIPT_LIBRARY:
    case AddonType::ADDON_SCRIPT_LYRICS:
    case AddonType::ADDON_SCRIPT_MODULE:
    case AddonType::ADDON_SUBTITLE_MODULE:
    case AddonType::ADDON_SCRIPT_WEATHER:
      return std::make_shared<CAddon>(info, type);
    case AddonType::ADDON_WEB_INTERFACE:
      return std::make_shared<CWebinterface>(info);
    case AddonType::ADDON_SERVICE:
      return std::make_shared<CService>(info);
    case AddonType::ADDON_SCRAPER_ALBUMS:
    case AddonType::ADDON_SCRAPER_ARTISTS:
    case AddonType::ADDON_SCRAPER_MOVIES:
    case AddonType::ADDON_SCRAPER_MUSICVIDEOS:
    case AddonType::ADDON_SCRAPER_TVSHOWS:
    case AddonType::ADDON_SCRAPER_LIBRARY:
      return std::make_shared<CScraper>(info, type);
    case AddonType::ADDON_SKIN:
      return std::make_shared<CSkinInfo>(info);
    case AddonType::ADDON_RESOURCE_FONT:
      return std::make_shared<CFontResource>(info);
    case AddonType::ADDON_RESOURCE_IMAGES:
      return std::make_shared<CImageResource>(info);
    case AddonType::ADDON_RESOURCE_GAMES:
      return std::make_shared<CGameResource>(info);
    case AddonType::ADDON_RESOURCE_LANGUAGE:
      return std::make_shared<CLanguageResource>(info);
    case AddonType::ADDON_RESOURCE_UISOUNDS:
      return std::make_shared<CUISoundsResource>(info);
    case AddonType::ADDON_REPOSITORY:
      return std::make_shared<CRepository>(info);
    case AddonType::ADDON_CONTEXT_ITEM:
      return std::make_shared<CContextMenuAddon>(info);
    case AddonType::ADDON_GAME_CONTROLLER:
      return std::make_shared<GAME::CController>(info);
    default:
      break;
  }
  return AddonPtr();
}

}
