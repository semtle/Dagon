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

#ifndef DG_CURSOR_H
#define DG_CURSOR_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "Action.h"
#include "Platform.h"

////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////

#define kDefCursorSize 128
#define kMaxCursorSize 128
#define DGMinCursorSize 32

class Config;
class Texture;

typedef struct {
    int type;
    Texture* image;
    Point origin;
} DGCursorData;

////////////////////////////////////////////////////////////
// Interface - Singleton class
////////////////////////////////////////////////////////////

// TODO: Define origin for cursors (for better placement)
class DGCursorManager : public Object {
    Config& config;
    
    std::vector<DGCursorData> _arrayOfCursors;
    std::vector<DGCursorData>::iterator _current;
    int _arrayOfCoords[8];
    int _half;
    bool _hasAction;
    bool _hasImage;
    bool _isDragging;
    bool _onButton; // Decide in Control?
    Action* _pointerToAction;    
    int _size;
    int _x;
    int _y;
    
    void _set(int typeOfCursor);
    
    DGCursorManager();
    DGCursorManager(DGCursorManager const&);
    DGCursorManager& operator=(DGCursorManager const&);
    ~DGCursorManager();
    
public:
    static DGCursorManager& instance() {
        static DGCursorManager cursorManager;
        return cursorManager;
    }

    Action* action();
    void bindImage();
    int* arrayOfCoords();
    bool hasAction();
    bool hasImage();
    bool isDragging();
    void load(int typeOfCursor, const char* imageFromFile, int offsetX = 0, int offsetY = 0);
    bool onButton();
    Point position();
    void removeAction();
    void setDragging(bool flag);
    void setOnButton(bool flag);    
    void setAction(Action* theAction);
    void setCursor(int typeOfCursor);
    void setSize(int size);
    void updateCoords(int x, int y);
};

#endif // DG_CURSOR_H
