////////////////////////////////////////////////////////////
//
// DAGON - An Adventure Game Engine
// Copyright (c) 2011-2014 Senscape s.r.l.
// All rights reserved.
//
// This Source Code Form is subject to the terms of the
// Mozilla Public License, v. 2.0. If a copy of the MPL was
// not distributed with this file, You can obtain one at
// http://mozilla.org/MPL/2.0/.
//
////////////////////////////////////////////////////////////

#ifndef DAGON_INTERFACE_H_
#define DAGON_INTERFACE_H_

namespace dagon {

////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////

class CameraManager;
class Config;
class CursorManager;
class Overlay;
class RenderManager;

////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////

class Interface {
  // References to singletons
  CameraManager& cameraManager;
  Config& config;
  CursorManager& cursorManager;
  RenderManager& renderManager;
  
  std::vector<Overlay*> _arrayOfOverlays;
  std::vector<Overlay*> _arrayOfActiveOverlays; // Visible overlays go here
  
public:
  Interface();
  ~Interface();
  
  void addOverlay(Overlay* overlay);
  void drawCursor();
  void drawHelpers();
  void drawOverlays();
  void fadeIn();
  void fadeOut();
  bool scanOverlays();
};
  
}

#endif // DAGON_INTERFACE_H_
