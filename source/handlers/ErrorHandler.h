#pragma once

class ErrorHandler
{
public:
    static ErrorHandler &GetInstance()
    {
        static ErrorHandler s_Instance;
        return s_Instance;
    }

    static void Initialize() { GetInstance().InitializeFunc(); }
    static void SetupCallbacks();

private:
    ErrorHandler() {}
    ErrorHandler(ErrorHandler const &);
    void operator=(ErrorHandler const &);

    void InitializeFunc();
    void ErrorCallback(int error_code, const char *description);
};