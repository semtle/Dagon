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

#ifndef DAGON_IMAGE_H_
#define DAGON_IMAGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "Geometry.h"
#include "DGObject.h"

////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////

class Config;
class DGTexture;

////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////

class DGImage : public DGObject {
 public:
  DGImage();
  DGImage(const std::string &fromFileName);
  ~DGImage() {};
  
  // Checks
  bool hasTexture();

  // Gets
  int* arrayOfCoordinates();
  Point position();
  Size size();
  DGTexture* texture();

  // Sets
  void setPosition(float x, float y);
  void setSize(float width, float height);
  void setTexture(const std::string &fromFileName);

  // State changes
  void move(float offsetX, float offsetY);
  void scale(float factor);
  
 private:
  Config& config;
  
  int _arrayOfCoordinates[8];
  DGTexture* _attachedTexture;
  bool _hasTexture = false;
  Rect _rect;
  
  void _calculateCoordinates();
};

#endif // DAGON_IMAGE_H_
