#pragma once

class ErrorHandler
{
public:
    ErrorHandler(const ErrorHandler &) = delete;
    static void Create()
    {
        static ErrorHandler s_Instance;
    }
    static ErrorHandler &Get()
    {
        static ErrorHandler s_Instance;
        return s_Instance;
    }

    static void SetupCallbacks();

private:
    ErrorHandler();
    ~ErrorHandler();

    void ErrorCallback(int error_code, const char *description);
};