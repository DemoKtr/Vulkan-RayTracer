#pragma once
 

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <iostream>

namespace scripts {

    class Script {

        std::string libraryPath;
        void* handle;

        void* loadLibrary();
        void closeLibrary();
        void* getFunction(std::string functionName);
    public:

        Script(std::string libraryPath);
        Script();
        ~Script();
        


        void execute_onStart_function();
        void execute_onUpdate_function(float deltaTime);
        void execute_onEnd_function();

        

       



    };

}