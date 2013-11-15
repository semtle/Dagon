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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "CameraManager.h"
#include "Config.h"
#include "EffectsManager.h"
#include "Texture.h"
#include "TimerManager.h"

namespace dagon {

////////////////////////////////////////////////////////////
// Implementation - Constructor
////////////////////////////////////////////////////////////

EffectsManager::EffectsManager() :
cameraManager(CameraManager::instance()),
config(Config::instance()),
timerManager(TimerManager::instance())
{
  this->setValue("brightness", 100);
  this->setValue("contrast", 100);
  this->setValue("saturation", 100);
  this->setValue("dust", 0);
  this->setValue("dustColor", kColorWhite);
  this->setValue("dustSize", 2);
  this->setValue("dustSpeed", 1);
  this->setValue("dustSpread", 20);
  this->setValue("motionBlur", 4);
  this->setValue("noise", 350);
  this->setValue("sepia", 0);
  this->setValue("sharpen", 0);
  this->setValue("sharpenRatio", 25);
  this->setValue("throb", 0);
  this->setValue("throbStyle", 1);
  
  _isActive = false;
  _isInitialized = false;
}

////////////////////////////////////////////////////////////
// Implementation - Destructor
////////////////////////////////////////////////////////////

EffectsManager::~EffectsManager() {
  this->pause();
  
  if (_isInitialized) {
    glDetachShader(_program, _fragment);
    glDeleteShader(_fragment);
    glDeleteProgram(_program);
    
    delete _dustTexture;
    
    _isActive = false;
    _isInitialized = false;
  }
}

////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////

void EffectsManager::drawDust() {
  float internalSize = this->get("dustSize") / 2000.0f;
  if (this->get("dust") && config.effects) {
    // Temporary
    glPushMatrix();
    
    uint32_t aux = this->get("dustColor");
    
    uint8_t b = (aux & 0x000000ff);
    uint8_t g = (aux & 0x0000ff00) >> 8;
    uint8_t r = (aux & 0x00ff0000) >> 16;
    uint8_t a = (aux & 0xff000000) >> 24;
    
    glColor4f((float)(r / 255.0f), (float)(g / 255.0f), (float)(b / 255.0f), (float)(a / 255.f));
    
    _dustTexture->bind();
    for (int i = 0; i < (this->get("dust") * 100); i++) {
      _particles[i].x += _particles[i].xd / (100 - this->get("dustSpeed"));
      _particles[i].y += _particles[i].yd / (100 - this->get("dustSpeed"));
      _particles[i].z += _particles[i].zd / (100 - this->get("dustSpeed"));
      if (_particles[i].y <= -0.5f) {
        _buildParticle(i);
      }
      
      GLfloat texCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
      glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
      
      glRotatef(1.0f, 0.0f, 1.0f, 0.0f);
      
      GLfloat quadCoords[] = {
        _particles[i].x, _particles[i].y, _particles[i].z + internalSize,
        _particles[i].x, _particles[i].y + internalSize, _particles[i].z + internalSize,
        _particles[i].x + internalSize, _particles[i].y + internalSize, _particles[i].z + internalSize,
        _particles[i].x + internalSize, _particles[i].y, _particles[i].z + internalSize };
      
      glVertexPointer(3, GL_FLOAT, 0, quadCoords);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    glPopMatrix();
  }
}

void EffectsManager::init() {
  const char* pointerToData;
  
  if (kEffectsReadFromFile) {
    if (_textFileRead()) {
      pointerToData = _shaderData;
    }
    else return;
  }
  else pointerToData = kShaderData;
  
  _fragment = glCreateShader(GL_FRAGMENT_SHADER);
  
  glShaderSource(_fragment, 1, &pointerToData, NULL);
  glCompileShader(_fragment);
  
  _program = glCreateProgram();
  glAttachShader(_program, _fragment);
  
  glLinkProgram(_program);
  
  _isInitialized = true;
  
  // Initialize dust
  for (int i = 0; i < kEffectsMaxDust; ++i) {
    _buildParticle(i);
  }
  
  _dustTexture = new Texture;
  _dustTexture->loadFromMemory(kDustData, 3666);
}

void EffectsManager::pause() {
  if (_isActive) {
    glUseProgram(0);
    _isActive = false;
  }
}

void EffectsManager::play() {
  if (_isInitialized && !_isActive) {
    glUseProgram(_program);
    
    _isActive = true;
  }
}

void EffectsManager::set(const std::string& theName, int theValue) {
  if (_isInitialized) {
    GLint parameter;
    GLint intensity;
    
    this->play();
    
    // Temporary fix
    int key = this->index(theName);
                 
    switch (key) {
      case effects::kBrightness:
        intensity = glGetUniformLocation(_program, "AdjustBrightness");
        _theSettings[theName].value = theValue;
        theValue = theValue / 100.0f;
        break;
        
      case effects::kSaturation:
        intensity = glGetUniformLocation(_program, "AdjustSaturation");
        _theSettings[theName].value = theValue;
        theValue = theValue / 100.0f;
        break;
        
      case effects::kContrast:
        intensity = glGetUniformLocation(_program, "AdjustContrast");
        _theSettings[theName].value = theValue;
        theValue = theValue / 100.0f;
        break;
        
      case effects::kDustColor:
        _theSettings[theName].value = theValue;
        this->pause();
        return;
        
      case effects::kDust:
        if ((theValue * 100) < kEffectsMaxDust)
          _theSettings[theName].value = theValue;
        this->pause();
        return;
        
      case effects::kDustSize:
        _theSettings[theName].value = theValue;
        this->pause();
        return;
        
      case effects::kDustSpeed:
        _theSettings[theName].value = theValue;
        this->pause();
        return;
        
      case effects::kDustSpread:
        _theSettings[theName].value = theValue;
        this->pause();
        return;
        
      case effects::kMotionBlur:
        parameter = glGetUniformLocation(_program, "MotionBlurEnabled");
        intensity = glGetUniformLocation(_program, "MotionBlurIntensity");
        _theSettings[theName].value = theValue;
        if (theValue)
          glUniform1i(parameter, true);
        else
          glUniform1i(parameter, false);
        theValue = (10 - theValue) / 1.0f;
        break;
        
      case effects::kNoise:
        parameter = glGetUniformLocation(_program, "NoiseEnabled");
        intensity = glGetUniformLocation(_program, "NoiseIntensity");
        _theSettings[theName].value = theValue;
        if (theValue)
          glUniform1i(parameter, true);
        else
          glUniform1i(parameter, false);
        theValue = theValue / 100.0f;
        break;
        
      case effects::kSepia:
        parameter = glGetUniformLocation(_program, "SepiaEnabled");
        intensity = glGetUniformLocation(_program, "SepiaIntensity");
        _theSettings[theName].value = theValue;
        if (theValue)
          glUniform1i(parameter, true);
        else
          glUniform1i(parameter, false);
        theValue = theValue / 100.0f;
        break;
        
      case effects::kSharpenRatio:
        intensity = glGetUniformLocation(_program, "SharpenRatio");
        _theSettings[theName].value = theValue;
        theValue = theValue / 100.0f;
        break;
        
      case effects::kSharpen:
        parameter = glGetUniformLocation(_program, "SharpenEnabled");
        intensity = glGetUniformLocation(_program, "SharpenIntensity");
        _theSettings[theName].value = theValue;
        if (theValue)
          glUniform1i(parameter, true);
        else
          glUniform1i(parameter, false);
        theValue = theValue / 10.0f;
        break;
        
      case effects::kThrobStyle:
        _theSettings[theName].value = theValue;
        this->pause();
        return;
        
      case effects::kThrob:
        _theSettings[theName].value = theValue;
        if (!theValue) {
          // Special case, we reset the brightness and contrast
          intensity = glGetUniformLocation(_program, "AdjustBrightness");
          glUniform1f(intensity, this->get("brightness") / 100.0f);
          intensity = glGetUniformLocation(_program, "AdjustContrast");
          glUniform1f(intensity, this->get("contrast") / 100.0f);
        }
        this->pause();
        return;
        
      default:
        this->pause();
        return;
    }
    
    glUniform1f(intensity, theValue);
    
    // Special case to enable/disable adjustment if three values are normal
    if ((this->get("brightness") != 100) ||
        (this->get("contrast") != 100) ||
        (this->get("saturation") != 100)) {
      parameter = glGetUniformLocation(_program, "AdjustEnabled");
      glUniform1i(parameter, true);
    }
    else {
      parameter = glGetUniformLocation(_program, "AdjustEnabled");
      glUniform1i(parameter, false);
    }
    
    this->pause();
  }
}

void EffectsManager::update() {
  static float noise = 0.0f;
  
  if (_isActive) {
    GLint parameter;
    
    if (this->get("motionBlur")) {
      parameter = glGetUniformLocation(_program, "MotionBlurOffsetX");
      glUniform1f(parameter, cameraManager.motionHorizontal());
      
      parameter = glGetUniformLocation(_program, "MotionBlurOffsetY");
      glUniform1f(parameter, cameraManager.motionVertical());
    }
    
    if (this->get("noise")) {
      parameter = glGetUniformLocation(_program, "NoiseRand");
      glUniform1f(parameter, noise);
      
      if (noise < 1.0f)
        noise += 0.01f;
      else noise = 0.0f;
    }
    
    if (this->get("throb")) {
      static int handlerStyle1 = timerManager.createManual(0.1f, 100);
      static int handlerStyle2 = timerManager.createManual(2, 1000);
      static float aux;
      static float j = 1.0f;
      float internalIntensity = (100 - this->get("throb"));
      
      switch (this->get("throbStyle")) {
        case 1:
          if (timerManager.checkManual(handlerStyle1, 100)) {
            aux = (rand() % 10) - (rand() % 10);
            parameter = glGetUniformLocation(_program, "AdjustBrightness");
            glUniform1f(parameter, (this->get("brightness") / 100.0f) + (aux / internalIntensity)); // Suggested: 50
            
            aux = rand() % 10;
            parameter = glGetUniformLocation(_program, "AdjustContrast");
            glUniform1f(parameter, (this->get("contrast") / 100.0f) + (aux / internalIntensity));
          }
          break;
          
        case 2:
          parameter = glGetUniformLocation(_program, "AdjustBrightness");
          glUniform1f(parameter, (this->get("brightness") / 100.0f) + (aux * j));
          
          parameter = glGetUniformLocation(_program, "AdjustContrast");
          glUniform1f(parameter, (this->get("contrast") / 100.0f) + 0.15f);
          
          if (j > 0)
            j -= 0.1f;
          
          if (timerManager.checkManual(handlerStyle2, 1000)) {
            aux = (float)((rand() % 50) + 5) / internalIntensity; // Suggested: 10
            j = 1.0f;
          }
          break;
      }
    }
  }
}


////////////////////////////////////////////////////////////
// Implementation - Private methods
////////////////////////////////////////////////////////////

void EffectsManager::_buildParticle(int idx) {
  int s;
  
  s = rand() % (int)kEffectsDustFactor;
  _particles[idx].xd = -(s / kEffectsDustFactor - 0.5f) / (100 - this->get("dustSpread"));
  
  s = rand() % (int)kEffectsDustFactor;
  _particles[idx].zd = -(s / kEffectsDustFactor - 0.5f) / (100 - this->get("dustSpread"));
  
  s = rand() % (int)kEffectsDustFactor;
  _particles[idx].yd = -s / kEffectsDustFactor / (100 - this->get("dustSpread"));
  
  s = rand() % (int)kEffectsDustFactor;
  _particles[idx].x = s / kEffectsDustFactor - 0.5f;
  
  s = rand() % (int)kEffectsDustFactor;
  _particles[idx].y = s / kEffectsDustFactor;
  
  s = rand() % (int)kEffectsDustFactor;
  _particles[idx].z = s / kEffectsDustFactor - 0.5f;
}

// Modified example from Lighthouse 3D: http://www.lighthouse3d.com
bool EffectsManager::_textFileRead() {
  FILE* fh;
  long size = 0;
  
  fh = fopen(kEffectsFileName, "rt");
  if (fh != NULL) {
    
    fseek(fh, 0, SEEK_END);
    size = ftell(fh);
    rewind(fh);
    
    if (size > 0) {
      _shaderData = (char*)malloc(sizeof(char) * (size + 1));
      size = fread(_shaderData, sizeof(char), size, fh);
      _shaderData[size] = '\0';
    }
    
    fclose(fh);
    
    return true;
  }
  
  return false;
}
  
}
