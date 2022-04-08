#pragma once

class ErrorHandler {
public:
  static ErrorHandler &Get() {
    static ErrorHandler s_Instance;
    return s_Instance;
  }

  static void Initialize();
  static void SetupCallbacks();

private:
  ErrorHandler() {}
  ErrorHandler(ErrorHandler const &);
  void operator=(ErrorHandler const &);

  void ErrorCallback(int error_code, const char *description);
};