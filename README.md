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

## How to Use  

### FrameLessQtWidget  

You can use frameless window simply like this.  
```c++
QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

QApplication app(argc, argv);

MainWindow w;
w.show();

return app.exec();
```
It works but you would need more than this.
The MainWindow class offers several additional functions.  
&nbsp;  

First, you can initialize the resize border width and title bar height.  
```c++
MainWindow w;
w.setResizeBorderWidth(6); //! You can set the resize border width.
w.setTitlebarHeight(35);   //! You also can set the title bar height.
```
&nbsp;  

If you want to change the colors of the custom title bar, write the code like this.  
```c++
MainWindow w;

//! MainWindow class provides getTitlebarWidget() function.
//! It return title bar widget.
// Set titlebar widget palette.
auto pal = w.getTitlebarWidget().palette();
pal.setColor(QPalette::Background, QColor(30, 34, 39)); // Set the colors you want
w.getTitlebarWidget().setAutoFillBackground(true);
w.getTitlebarWidget().setPalette(pal);
```
&nbsp;  

If you want to change the colors of the content widget, write the code like this.  
```c++
MainWindow w;

//! MainWindow class provides getContentWidget() function.
//! It return main content widget.
// Set main content widget palette.
auto pal = w.getContentWidget().palette();
pal.setColor(QPalette::Background, QColor(35, 39, 46));
w.getContentWidget().setAutoFillBackground(true);
w.getContentWidget().setPalette(pal);
```
&nbsp;  

If you want to add some widget to the main widget, use ``getContentWidget()`` function like this.
```c++
MainWindow w;

// Add layout to main content widget.
QVBoxLayout *main_widget_layout = new QVBoxLayout(&w);
w.getContentWidget().setLayout(main_widget_layout);

// Add some button.
QPushButton *btn = new QPushButton("This is Button!", &w.getContentWidget());
btn->setFixedSize(180, 35);
main_widget_layout->addWidget(btn);
main_widget_layout->setAlignment(btn, Qt::AlignHCenter);
```
&nbsp;  

If you want to add menu bar on custom title bar, write the code like this.  
```c++
MainWindow w;

// Customize menu bar
QMenuBar *menubar = new QMenuBar(&w);
menubar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

// Initialize menu item.
QMenu *menu = menubar->addMenu("File");
menu->addAction("New File");
menu->addAction("Open File");
menu = menubar->addMenu("Edit");
menu->addAction("Undo");
menu->addAction("Redo");

// Add widget to title bar.
// "clickable widget" property should be true if you want this widget to work on the title bar.
menubar->setProperty("clickable widget", true);
w.getCustomTitlebarLayout().addWidget(menubar);
```
This ```menubar->setProperty("clickable widget", true);``` line determines whether the widget on the custom title bar is clickable or not.
If **clickable widget** property is true, the widget will work but you can't drag that area for moving frameless window.  
If **clickable widget** property is false, the widget will not react to your mouse but you can drag that area for moving frameless window.  
So if you add a widget to the custom title bar, don't forget to set the **clickable widget** property!  
&nbsp;  

### FrameLessQtQuick    

There are two things you need to follow in **main.cpp** file.   

First, right after declaring QGuiApplication, you need to initialize MainWindow and install native event filter.
```c++
QGuiApplication app(argc, argv);

//! Native event filter should be installed on MainWindow class at this time.
//! If you install NativeEventFilter later, it is really hard to make window to frameless window.
MainWindow win_quick_window;
app.installNativeEventFilter(&win_quick_window);
```
&nbsp;  

Second, use the ``initWindow()`` function in the linked lambda function like this.  
```c++
QQmlApplicationEngine engine;

const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

QObject::connect(
    &engine, &QQmlApplicationEngine::objectCreated,
    &app, [&](QObject *obj, const QUrl &obj_url) {
        // Initialize window here
        if ((!obj && url == obj_url) || !win_quick_window.initWindow(engine))
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
engine.load(url);
``` 
These two things are all you need to care about in the cpp part.   
&nbsp;  

In **main.qml**, ApplicationWindow flags should be **Qt.Window**.   
And there should be a resizeBorderWidth property like the one below.  
```qml
// Border width for window resizing
// This is used in the WinQuickWindow.cpp file.
property int resizeBorderWidth: 6
```
&nbsp;  

An example of the Minimize button is shown below.  
```qml
Button {
    id: minimizeButton
    background: Rectangle {
        color: parent.down ? Qt.rgba(1.0, 1.0, 1.0, 0.4) : (parent.hovered ? Qt.rgba(1.0, 1.0, 1.0, 0.2) :
                                                            Qt.rgba(1.0, 1.0, 1.0, 0.0))
    }
    Image {
        source: active ? "qrc:/icon/Minimize.png" : "qrc:/icon/MinimizeDeactivated.png"
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }

    // Trigger onMinimizeButtonClicked function implemented on C++ side.
    onClicked: {
        cppConnector.onMinimizeButtonClicked()
    }
}
```
```cppConnector.onMinimizeButtonClicked()``` this can trigger Minimize logic.  
&nbsp;  

An example of the Maximize button is shown below.  
Maximize button should have **objectName**, **checkable**, **checked** value like below.  
```qml
Button {
    id: maximizeButton
    objectName: "maximizeButton"
    checkable: true
    checked: false
    background: Rectangle {
        color: parent.down ? Qt.rgba(1.0, 1.0, 1.0, 0.4) : (parent.hovered ? Qt.rgba(1.0, 1.0, 1.0, 0.2) :
                                                                             Qt.rgba(1.0, 1.0, 1.0, 0.0))
    }
    Image {
        source: maximizeButton.checked ? (active ? "qrc:/icon/Restore.png" : "qrc:/icon/RestoreDeactivated.png") :
                                        (active ? "qrc:/icon/Maximize.png" : "qrc:/icon/MaximizeDeactivated.png")
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }

    // Trigger onMaximizeButtonClicked function implemented on C++ side.
    onClicked: {
        cppConnector.onMaximizeButtonClicked()
    }
}
```
```cppConnector.onMaximizeButtonClicked()``` this can trigger Maximize logic.  
&nbsp;  

An example of the Close button is shown below.  
```qml
Button {
    id: closeButton
    background: Rectangle {
        color: parent.down ? Qt.rgba(0.78, 0.16, 0.184, 0.6) : (parent.hovered ? Qt.rgba(0.86, 0.16, 0.184, 0.9) :
                                                                                 Qt.rgba(1.0, 1.0, 1.0, 0.0))
    }
    Image {
        source: closeButton.hovered ? "qrc:/icon/CloseHoverOrPressed.png" :
                                      (active ? "qrc:/icon/Close.png" : "qrc:/icon/CloseDeactivated.png")
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }

    // Trigger onMaximizeButtonClicked function implemented on C++ side.
    onClicked: {
        cppConnector.onCloseButtonClicked()
    }
}
```
```cppConnector.onCloseButtonClicked()``` this can trigger Close logic.  
&nbsp;  

You can make the area for dragging a frameless window like this.  
```qml
MouseArea {
    anchors.fill: parent

    // When you press this area, the window becomes movable.
    onPressed: {
        applicationWindow.startSystemMove()
    }

    // When you double-click this area, the window state changes as well.
    onDoubleClicked: {
        applicationWindow.visibility = maximizeButton.checked ? Window.Windowed : Window.Maximized
    }
}
```
&nbsp;  

The rest qml is up to you to fill in.  
Simply combine what you want!  
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
