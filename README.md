# Qt Frameless window on windows OS

## About  

This is a template projects that let you know how to make frameless window via Qt on windows OS.  
There are two ways to make frameless windows, one uses QtWidget, the other uses QtQuick.  
&nbsp;  

## Showcase of FrameLessQtWidget  
![FrameLessQtWidgetShowcase](/resources/FrameLessQtWidgetShowcase.gif)
&nbsp;  

## Showcase of FrameLessQtQuick  
![FrameLessQtWidgetShowcase](/resources/FrameLessQtQuickShowcase.gif)
&nbsp;  

## How to build  

Environment that you may need to install to build this project are listed below.  
&nbsp;  

### CMake  

This is **necessary** to build this project.  
I have tested this template project with CMake 3.23.  
I also used **CMakePresets.json** file to make this project, but this is not essential.
&nbsp;  

### Qt  

This project **requires** Qt5 to run.  
I have tested this template project with Qt 5.15.10.  
I haven't tested it on Qt 6, but it should work.  
&nbsp;  

### Visual Studio 2022  

I made this project using MSVC 2022 compiler but other version of MSVC compiler should work too.  
&nbsp;  

### VS Code  

I made this project using VS Code, but this is not **necessary**.
In case you are wondering which VS Code extension I used in this project, I have listed the extension list below.

* [C/C++ by Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake by twxs](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
* [CMake Tools by Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

&nbsp;  
### Other Instructions  

You have to modify **CMakePresets.json** file.  
```json
{
    "description": "Windows builds only",
    "name": "windows-base",
    "hidden": true,
    "binaryDir": "${sourceDir}/Build/${presetName}",
    "installDir": "${sourceDir}/Installed/${presetName}",
    "toolchainFile": "D:/ProgramFiles/vcpkg/scripts/buildsystems/vcpkg.cmake",
    "condition": {
        "type": "equals",
        "lhs": "${hostSystemName}",
        "rhs": "Windows"
    }
}
```
If you don't use vcpkg, remove the **toolchainFile** entry.  
but if you do, then change this path to your own vcpkg path.  
&nbsp;  

You have to change **generator** item too in **CMakePresets.json** file.  
```json
{
    "description": "MSVC",
    "name": "msvc",
    "hidden": true,
    "generator": "Visual Studio 17 2022",
    "inherits": "windows-base"
}
```
If you are already using VS 2022, you are good to go.  
But if you're not, change the **generator** you're using.  
For example, VS 2019 is **Visual Studio 16 2019** and VS 2017 is **Visual Studio 15 2017**.  
You should only use the Visual Studio generator!  
&nbsp;  

And you should also change **CMakeLists.txt** file.  
```cmake
# This path should be set to your directory path where Qt5Config.cmake (or Qt6Config.cmake) is located. 
set(QT_DIR "D:\\ProgramFiles\\Qt\\5.15.10\\msvc2022_64\\lib\\cmake\\Qt5")
```
As the comment above, write your own directory path where Qt5Config.cmake is located.  
&nbsp;  

If you have followed the process above, you are ready to build this template project.  
Once you have built the project, you might find the **.exe** file in your own ```Build\msvc-x64\Release(or Debug)``` directory.  
&nbsp;  

## Key point of making frameless window  

As you can see from the project code, **WM_NCCALCSIZE**, **WM_NCHITTEST** is the key message you should focus on.  
&nbsp;  

Modifying the WM_NCCALCSIZE message process helps you to remove window borders.  
```c++
case WM_NCCALCSIZE: {
    if (msg->lParam)
    {
        WINDOWPLACEMENT wp;
        GetWindowPlacement(m_hwnd, &wp);

        if (wp.showCmd == SW_MAXIMIZE)
        {
            NCCALCSIZE_PARAMS *sz = (NCCALCSIZE_PARAMS *)msg->lParam;
            sz->rgrc[0].left += 8;
            sz->rgrc[0].top += 8;
            sz->rgrc[0].right -= 8;
            sz->rgrc[0].bottom -= 8;

            // Save window mode state.
            m_quick_window->findChild<QObject *>("maximumButton")->setProperty("checked", true);
        }
        else if (wp.showCmd == SW_NORMAL)
            // Save window mode state.
            m_quick_window->findChild<QObject *>("maximumButton")->setProperty("checked", false);
    }
    return true;
}
```
&nbsp;  

Modifying the WM_NCHITTEST message process helps you change and move frameless window.  
```c++
case WM_NCHITTEST: {
    RECT winrect;
    GetWindowRect(msg->hwnd, &winrect);
    long x = GET_X_LPARAM(msg->lParam);
    long y = GET_Y_LPARAM(msg->lParam);

    if (x >= winrect.left && x < winrect.left + m_resize_border_width &&
        y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
    {
        *result = HTBOTTOMLEFT;
        return true;
    }

    if (x < winrect.right && x >= winrect.right - m_resize_border_width &&
        y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
    {
        *result = HTBOTTOMRIGHT;
        return true;
    }

    if (x >= winrect.left && x < winrect.left + m_resize_border_width &&
        y >= winrect.top && y < winrect.top + m_resize_border_width)
    {
        *result = HTTOPLEFT;
        return true;
    }

    if (x < winrect.right && x >= winrect.right - m_resize_border_width &&
        y >= winrect.top && y < winrect.top + m_resize_border_width)
    {
        *result = HTTOPRIGHT;
        return true;
    }

    if (x >= winrect.left && x < winrect.left + m_resize_border_width)
    {
        *result = HTLEFT;
        return true;
    }

    if (x < winrect.right && x >= winrect.right - m_resize_border_width)
    {
        *result = HTRIGHT;
        return true;
    }

    if (y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
    {
        *result = HTBOTTOM;
        return true;
    }

    if (y >= winrect.top && y < winrect.top + m_resize_border_width)
    {
        *result = HTTOP;
        return true;
    }

    *result = HTTRANSPARENT;
    break;
}
```
&nbsp;  

## Licence  

This project template is licensed under the [MIT License](https://github.com/tongmon/qt-frameless-windows/blob/main/LICENSE).
