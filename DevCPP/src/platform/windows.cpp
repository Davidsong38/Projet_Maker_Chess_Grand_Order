//
// Created by sebas on 30/12/2024.
//

#include "platform.h"
#include "log.h"
#include <cstdio>

#include <windows.h>
#include "glcorearb.h"
#include "wglext.h"


extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; // NVIDIA
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;   // AMD
}

static HWND window;
static HDC dc;
static BOOL isMouseHidden = FALSE;
static BOOL isMouseLocked = FALSE;

static glm::ivec2 currentWindowSize;

LRESULT CALLBACK windows_window_callback(HWND curWindow, UINT msg, WPARAM wParam, LPARAM lParam){
    LRESULT result=0;
    switch (msg){
        case WM_CLOSE:{
            exit(EXIT_SUCCESS);
            break;
        }
        case WM_SIZE:{
            RECT rect={};
            GetClientRect(curWindow,&rect);
            currentWindowSize.x = rect.right - rect.left;
            currentWindowSize.y = rect.bottom - rect.top;
            break;
        }
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:{
            // ???
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:{
            // ??????
            break;
        }
        default:{
            result = DefWindowProcA(curWindow, msg, wParam, lParam);
        }
    }
    return result;
}

int platform_create_window(int width, int height, const char* title){
    HINSTANCE instance  = GetModuleHandleA(nullptr);
    WNDCLASSA wc        = {};
    wc.hInstance        = instance;
    wc.hIcon            = LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(nullptr,IDC_ARROW);
    wc.lpszClassName    = title;
    wc.lpfnWndProc      = windows_window_callback;
    if (!RegisterClassA(&wc)) return false;
    int dwStyle = WS_MAXIMIZE;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
    //Fake Window initialisation
    {
        window = CreateWindowExA(0,title,title,
            dwStyle,
            0,0,
            width,height,
            nullptr,
            nullptr,
            instance,
            nullptr
        );
        ASSERT(window != nullptr,"Failed to create Windows Window");
        HDC fakeDC=GetDC(window);
        ASSERT(fakeDC,"Failed to get HDC");
        PIXELFORMATDESCRIPTOR pfd = {0};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.cDepthBits = 24;
        int pixelFormat = ChoosePixelFormat(fakeDC, &pfd);
        ASSERT(pixelFormat,"Failed to choose pixel format");
        ASSERT(SetPixelFormat(fakeDC,pixelFormat,&pfd),"Failed to set pixel format");
        //Create fake rendering context
        HGLRC fakeRC = wglCreateContext(fakeDC);
        ASSERT(fakeDC,"Failed to create render context");
        ASSERT(wglMakeCurrent(fakeDC,fakeRC),"Failed to make current");
        wglChoosePixelFormatARB =
            (PFNWGLCHOOSEPIXELFORMATARBPROC) platform_load_gl_function((char*)"wglChoosePixelFormatARB");
        wglCreateContextAttribsARB =
            (PFNWGLCREATECONTEXTATTRIBSARBPROC) platform_load_gl_function((char*)"wglCreateContextAttribsARB");
        ASSERT(wglCreateContextAttribsARB && wglChoosePixelFormatARB,"Failed to load OpenGL functions");
        //Clean up
        wglMakeCurrent(fakeDC,nullptr);
        wglDeleteContext(fakeRC);
        ReleaseDC(window,fakeDC);
        DestroyWindow(window);
    }
    //Actual Window initialisation
    {
        //Border size
        {
            RECT borderRect = {};
            AdjustWindowRectEx(&borderRect,dwStyle,0,0);
            width += borderRect.right - borderRect.left;
            height += borderRect.bottom - borderRect.top;
        }
        window = CreateWindowExA(0,title,title,
                            dwStyle,
                            0,0,
                            width,height,
                            nullptr,
                            nullptr,
                            instance,
                            nullptr
        );
        ASSERT(window != nullptr,"Failed to create Windows Window");
        dc=GetDC(window);
        ASSERT(dc,"Failed to get HDC");
        const int pixelAttribs[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_SWAP_METHOD_ARB,    WGL_SWAP_COPY_ARB,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,     32,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     24,
            0   //Terminator
        };
        UINT numPixelFormat;
        int pixelFormat=0;
        ASSERT(wglChoosePixelFormatARB(
            dc,pixelAttribs,
            nullptr,    //Float list
            1,          //Max Formats
            &pixelFormat,
            &numPixelFormat
        ),"Failed to wglChoosePixelFormatARB");
        PIXELFORMATDESCRIPTOR pfd = {0};
        DescribePixelFormat(dc,pixelFormat,sizeof(PIXELFORMATDESCRIPTOR),&pfd);
        ASSERT(SetPixelFormat(dc,pixelFormat,&pfd),"Failed to SetPixelFormat");
        const int contextAttribs[]={
            WGL_CONTEXT_MAJOR_VERSION_ARB,  4,
            WGL_CONTEXT_MINOR_VERSION_ARB,  3,
            WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_FLAGS_ARB,          WGL_CONTEXT_DEBUG_BIT_ARB,
            0   //Terminator
        };
        HGLRC rc=wglCreateContextAttribsARB(dc,nullptr,contextAttribs);
        ASSERT(rc,"Failed to create render context for OpenGL");
        ASSERT(wglMakeCurrent(dc,rc),"Failed to wglMakeCurrent");
    }
    ShowWindow(window, SW_SHOW);
    return EXIT_SUCCESS;
}

glm::ivec2 platform_get_window_size() {
    return currentWindowSize;
}

void* platform_load_gl_function(char* funName){
    PROC proc = wglGetProcAddress(funName);
    if (!proc){
        static HMODULE openglDLL=LoadLibraryA((LPCSTR)"opengl32.dll");
        proc = GetProcAddress(openglDLL,funName);
        if (!proc){
            char msg[64]{0};
            sprintf(msg,"Failed to load: %s",funName);
            log(LOG_FATAL,msg);
            return nullptr;
        }
    }
    return (void*) proc;
}

void platform_update_window() {
    //Gather input
    MSG msg;
    while (PeekMessageA(&msg,window,0,0,PM_REMOVE)){
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    //Mouse Visibility
    POINT point = {};
    GetCursorPos(&point);
    ScreenToClient(window,&point);
    if (isMouseHidden) {
        while(ShowCursor(true) <= 0);
    }
}

void platform_swap_buffers() {
    SwapBuffers(dc);
}

void lockMouse() {
    isMouseLocked=TRUE;
}
void unlockMouse() {
    isMouseLocked=FALSE;
}
void hideMouse() {
    isMouseHidden=TRUE;
}
void showMouse() {
    isMouseHidden=FALSE;
}