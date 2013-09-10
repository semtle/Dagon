////////////////////////////////////////////////////////////
//
// DAGON - An Adventure Game Engine
// Copyright (c) 2011-2013 Senscape s.r.l.
// All rights reserved.
//
// This Source Code Form is subject to the terms of the
// Mozilla Public License, v. 2.0. If a copy of the MPL was
// not distributed with this file, You can obtain one at
// http://mozilla.org/MPL/2.0/.
//
////////////////////////////////////////////////////////////

#ifndef DAGON_SPOT_H_
#define DAGON_SPOT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <vector>

#include "Action.h"
#include "Geometry.h"
#include "Colors.h"

////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////

enum SpotFlags {
  kSpotAuto = 0x1,
	kSpotClass = 0x2,
	kSpotLoop = 0x4,    
	kSpotSync = 0x8,    
	kSpotUser = 0x10
};

class Audio;
class Texture;
class DGVideo;

////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////

class Spot : public Object {
 public:
  Spot(std::vector<int> withArrayOfCoordinates,
         unsigned int onFace, int withFlags);
  ~Spot();
    
  // Checks
  bool hasAction();
  bool hasAudio();
  bool hasColor();
  bool hasFlag(int theFlag);
  bool hasTexture();
  bool hasVideo();
  bool isPlaying();
    
  // Gets
  Action* action();
  Audio* audio();
  int color();
  std::vector<int> arrayOfCoordinates();
  unsigned int face();
  Point origin();
  Texture* texture();
  int vertexCount();
  DGVideo* video();
  float volume();
    
  // Sets
  void setAction(Action* anAction);
  void setAudio(Audio* anAudio);
  void setColor(int aColor);
  void setOrigin(int x, int y);
  void setTexture(Texture* aTexture);
  void setVideo(DGVideo* aVideo);
  void setVolume(float theVolume);
    
  // State changes
  void play();
  void resize(int width, int height);
  void stop();
  
 private:
  Action* _actionData;
  Audio* _attachedAudio;
  Texture* _attachedTexture;
  DGVideo* _attachedVideo;

  std::vector<int> _arrayOfCoordinates;
  unsigned int _onFace;
  
  int _color = kColorBlack;
  int _flags;
  bool _hasAction = false;
  bool _hasAudio = false;
  bool _hasColor = false;
  bool _hasTexture = false;
  bool _hasVideo = false;
  bool _isPlaying = false;
  float _volume = 1.0;
  int _xOrigin = 0;
  int _yOrigin = 0;
  int _zOrder = 0; // For future use
  
  Spot(const Spot&);
  void operator=(const Spot&);
};

#endif // DAGON_SPOT_H_