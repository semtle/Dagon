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

// TODO: This is like... the saddest state manager ever. Must
// migrate more functionality here.

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "DGState.h"

////////////////////////////////////////////////////////////
// Implementation - Constructor
////////////////////////////////////////////////////////////

DGState::DGState() {
    _current = DGStateNode;
    _previous = _current;
}

////////////////////////////////////////////////////////////
// Implementation - Destructor
////////////////////////////////////////////////////////////

DGState::~DGState() {
    // Nothing to do here
}

////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////

int DGState::current() {
    return _current;
}

int DGState::previous() {
    return _previous;
}

void DGState::set(int theState) {
    _previous = _current;
    _current = theState;
}

void DGState::setPrevious() {
    int auxState = _current;
    
    _current = _previous;
    _previous = auxState;
}
