#include "MainWindow.hpp"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMenu>
#include <QMenuBar>
#include <cstdlib>
#include <ctime>
#include <dwmapi.h>
#include <memory>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_minimize_btn{nullptr}, m_maximize_btn{nullptr}, m_close_btn{nullptr}
{
    m_hwnd = (HWND)winId();
    m_resize_border_width = 6;

    // Set window shadows.
    const MARGINS aero_shadow_on = {1, 1, 1, 1};
    ::DwmExtendFrameIntoClientArea(m_hwnd, &aero_shadow_on);

    // Set the Icon here.
    setWindowIcon(QIcon(":/icon/ApplicationIcon.png"));

    // Set window size.
    int window_width = 1024, window_height = 768;
    setGeometry(QApplication::desktop()->screen()->width() / 2 - window_width / 2,
                QApplication::desktop()->screen()->height() / 2 - window_height / 2,
                window_width, window_height);
    setMinimumSize(400, 300);

    QObject::connect(windowHandle(), &QWindow::screenChanged, this, &MainWindow::OnScreenChanged);

    // Add widget.
    QWidget *entire_widget = new QWidget(this);
    entire_widget->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(entire_widget);

    // Set palette.
    QPalette Pal(entire_widget->palette());
    Pal.setColor(QPalette::Background, QColor(30, 34, 39));
    entire_widget->setAutoFillBackground(true);
    entire_widget->setPalette(Pal);

    QVBoxLayout *entire_layout = new QVBoxLayout(this);
    entire_widget->setLayout(entire_layout);
    entire_layout->setContentsMargins(0, 0, 0, 0);
    entire_layout->setSpacing(0);

    QHBoxLayout *titlebar_layout = new QHBoxLayout(this);
    entire_layout->addLayout(titlebar_layout);
    titlebar_layout->setContentsMargins(0, 0, 0, 0);
    titlebar_layout->setSpacing(0);

    // Add a menu bar to the title bar.
    QMenuBar *menubar = new QMenuBar(this);
    titlebar_layout->addWidget(menubar);
    menubar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_activable_widgets.push_back(menubar);
    menubar->setStyleSheet(R"(
        QMenuBar {
            border-image: url(:/icon/Transparent.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QMenuBar::item {
            color: rgb(152, 160, 175);
        }
        QMenuBar::item[active="false"] {
            color: rgb(112, 120, 135);
        }
        QMenuBar::item:selected {
            spacing: 3px;           
            padding: 2px 10px;
            background-color: rgb(52, 56, 61);
            border-radius: 5px;
        }

        QMenu {
             color: rgb(152, 160, 175);
             background-color: rgb(30, 34, 39);
        }
        QMenu::item:selected {
            background-color: rgb(52, 56, 61);
        }
    )");

    QMenu *menu = menubar->addMenu("File");
    menu->addAction("New Text File");
    menu->addAction("New File");
    menu->addAction("New Window");
    menu->addAction("Open File");
    menu->addAction("Open Directory");

    menu = menubar->addMenu("Edit");
    menu->addAction("Undo");
    menu->addAction("Redo");
    menu->addAction("Cut");
    menu->addAction("Copy");
    menu->addAction("Paste");

    menu = menubar->addMenu("Selected Zone");
    menu->addAction("Select All");
    menu->addAction("Expand Select Zone");
    menu->addAction("Collapse Select Zone");
    menu->addAction("Add Cursor");
    menu->addAction("Select Line Mode");

    menu = menubar->addMenu("View");
    menu->addAction("Command Pallete");
    menu->addAction("Open View");
    menu->addAction("Shape");
    menu->addAction("Edit Layout");

    // Add an area element to the m_movable_area variable where the user can click to move the window.
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    titlebar_layout->addSpacerItem(spacer);
    m_movable_area.push_back(spacer);

    // Minimize button setup.
    m_minimize_btn = new QPushButton(this);
    titlebar_layout->addWidget(m_minimize_btn);
    m_minimize_btn->setFixedSize(46, 35);
    m_minimize_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_minimize_btn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Minimize.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }

        QPushButton:hover {
            background-color: rgba(255, 255, 255, 20%);
        }

        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 40%);
        }
      
        QPushButton:!active {
            border-image: url(:/icon/MinimizeDeactivated.png);
        }
    )");

    // Maximize button setup.
    m_maximize_btn = new QPushButton(this);
    titlebar_layout->addWidget(m_maximize_btn);
    m_maximize_btn->setFixedSize(46, 35);
    m_maximize_btn->setCheckable(true);
    m_maximize_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_maximize_btn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Maximize.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 20%);
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 40%);
        }
        QPushButton:checked {
            border-image: url(:/icon/Restore.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:checked:hover {
            background-color: rgba(255, 255, 255, 20%);
        }
        QPushButton:checked:pressed {
            background-color: rgba(255, 255, 255, 40%);
        }
        QPushButton:!active {
            border-image: url(:/icon/MaximizeDeactivated.png);
        }
        QPushButton:checked:!active {
            border-image: url(:/icon/RestoreDeactivated.png);
        }
    )");

    // Close button setup.
    m_close_btn = new QPushButton(this);
    titlebar_layout->addWidget(m_close_btn);
    m_close_btn->setFixedSize(46, 35);
    m_close_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_close_btn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Close.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }
        QPushButton:hover {
            border-image: url(:/icon/CloseHoverOrPressed.png);
            background-color: rgba(220, 41, 47, 90%);
        }
        QPushButton:pressed {
            border-image: url(:/icon/CloseHoverOrPressed.png);
            background-color: rgba(200, 41, 47, 60%);
        }
        QPushButton:!active {
            border-image: url(:/icon/CloseDeactivated.png);
        }
        QPushButton:hover:!active {
            border-image: url(:/icon/CloseHoverOrPressed.png);
        }
    )");

    QObject::connect(m_minimize_btn, &QPushButton::clicked, this, &MainWindow::OnMinimizeButtonClicked);
    QObject::connect(m_maximize_btn, &QPushButton::clicked, this, &MainWindow::OnMaximizeButtonClicked);
    QObject::connect(m_close_btn, &QPushButton::clicked, this, &MainWindow::OnCloseButtonClicked);

    entire_layout->setAlignment(titlebar_layout, Qt::AlignTop);

    QWidget *main_widget = new QWidget(this);
    entire_layout->addWidget(main_widget);
    main_widget->setContentsMargins(0, 0, 0, 0);
    main_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Set main widget palette.
    Pal = main_widget->palette();
    Pal.setColor(QPalette::Background, QColor(35, 39, 46));
    main_widget->setAutoFillBackground(true);
    main_widget->setPalette(Pal);

    QVBoxLayout *main_widget_layout = new QVBoxLayout(this);
    main_widget->setLayout(main_widget_layout);

    QPushButton *btn = new QPushButton("This is Button!", this);
    btn->setFixedSize(180, 35);
    main_widget_layout->addWidget(btn);
    main_widget_layout->setAlignment(btn, Qt::AlignHCenter);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::nativeEvent(const QByteArray &event_type, void *message, long *result)
{
    MSG *msg = (MSG *)message;

    switch (msg->message)
    {
    // Remove the default window frame by hooking the WM_NCCALCSIZE message.
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
            }
        }
        return true;
    }

    // Process the mouse when it is on the window border.
    case WM_NCHITTEST: {
        RECT winrect;
        GetWindowRect(msg->hwnd, &winrect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        long local_x = x - winrect.left;
        long local_y = y - winrect.top;

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

        // Check the area where the user can click to move the window.
        for (const auto &item : m_movable_area)
        {
            if (item->geometry().contains(local_x, local_y))
            {
                *result = HTCAPTION;
                return true;
            }
        }

        *result = HTTRANSPARENT;
        break;
    }
    case WM_SIZE: {
        if (m_maximize_btn)
        {
            WINDOWPLACEMENT wp;
            GetWindowPlacement(m_hwnd, &wp);
            m_maximize_btn->setChecked(wp.showCmd == SW_MAXIMIZE ? true : false);
        }
        break;
    }
    default:
        break;
    }

    return false;
}

// This is used to change the `active` state of the Maximize / Minimize / Close buttons.
bool MainWindow::event(QEvent *evt)
{
    switch (evt->type())
    {
    case QEvent::WindowActivate: {
        for (auto &item : m_activable_widgets)
        {
            item->setProperty("active", true);
            item->setStyleSheet(item->styleSheet());
        }
        break;
    }

    case QEvent::WindowDeactivate: {
        for (auto &item : m_activable_widgets)
        {
            item->setProperty("active", false);
            item->setStyleSheet(item->styleSheet());
        }
        break;
    }

    default:
        break;
    }

    return QMainWindow::event(evt);
}

// Render again when frame is moved to another monitor.
void MainWindow::OnScreenChanged(QScreen *screen)
{
    SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

void MainWindow::OnMinimizeButtonClicked()
{
    SendMessage(m_hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void MainWindow::OnMaximizeButtonClicked()
{
    SendMessage(m_hwnd, WM_SYSCOMMAND, m_maximize_btn->isChecked() ? SC_MAXIMIZE : SC_RESTORE, 0);

    // Remove the hover state from the maximize button.
    m_maximize_btn->setAttribute(Qt::WA_UnderMouse, false);
}

void MainWindow::OnCloseButtonClicked()
{
    SendMessage(m_hwnd, WM_CLOSE, 0, 0);
}
