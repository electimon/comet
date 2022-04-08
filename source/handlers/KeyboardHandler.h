#pragma once

#include "MouseHandler.h"
#include "WindowHandler.h"

class KeyboardHandler {
public:
  static KeyboardHandler &Get() {
    static KeyboardHandler s_Instance;
    return s_Instance;
  }

  static void SetupCallbacks();

private:
  KeyboardHandler() {}
  KeyboardHandler(KeyboardHandler const &);
  void operator=(KeyboardHandler const &);

  void KeyCallback(int key, int scancode, int action, int mods);
};