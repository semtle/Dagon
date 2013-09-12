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

#ifndef DG_CURSORLIB_H
#define DG_CURSORLIB_H

////////////////////////////////////////////////////////////
// NOTE: This header file should never be included directly.
// It's auto-included by DGProxy.h
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "DGCursorManager.h"

// FIXME: The cursor manager shouldn't inherit from Object...
// In fact, it shouldn't be a singleton.

////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////

static int DGCursorLibDisable(lua_State *L) {
    DGCursorManager::instance().Object::disable();
	
	return 0;
}

static int DGCursorLibEnable(lua_State *L) {
    DGCursorManager::instance().Object::enable();
	
	return 0;
}

static int DGCursorLibFadeIn(lua_State *L) {
    DGCursorManager::instance().Object::fadeIn();
	
	return 0;
}

static int DGCursorLibFadeOut(lua_State *L) {
    DGCursorManager::instance().Object::fadeOut();
	
	return 0;
}

static int DGCursorLibLoad(lua_State *L) {
    DGCursorManager::instance().load(static_cast<int>(luaL_checknumber(L, 1)),
                                     luaL_checkstring(L, 2),
                                     static_cast<int>(lua_tonumber(L, 3)),
                                     static_cast<int>(lua_tonumber(L, 4)));
    
    return 0;
}

////////////////////////////////////////////////////////////
// Static definitions
////////////////////////////////////////////////////////////

static const struct luaL_reg kCursorLib [] = {
	{"disable", DGCursorLibDisable},
	{"enable", DGCursorLibEnable},   
    {"fadeIn", DGCursorLibFadeIn}, 
    {"fadeOut", DGCursorLibFadeOut},
    {"load", DGCursorLibLoad},
	{NULL, NULL}
};

#endif // DG_CURSORLIB_H
