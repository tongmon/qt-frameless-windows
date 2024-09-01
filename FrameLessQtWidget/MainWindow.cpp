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
    : QMainWindow(parent), m_minimize_btn{nullptr}, m_maximize_btn{nullptr}, m_close_btn{nullptr}, m_resize_border_width{6}
{
    m_hwnd = reinterpret_cast<HWND>(winId());

    // Set window shadows.
    const MARGINS aero_shadow_on = {1, 1, 1, 1};
    ::DwmExtendFrameIntoClientArea(m_hwnd, &aero_shadow_on);

    QObject::connect(windowHandle(), &QWindow::screenChanged, this, &MainWindow::onScreenChanged);

    // Add widget. (Initialize central widget)
    QWidget *entire_widget = new QWidget(this);
    entire_widget->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(entire_widget);

    // Layout for entire widgets.
    QVBoxLayout *entire_layout = new QVBoxLayout(this);
    entire_widget->setLayout(entire_layout);
    entire_layout->setContentsMargins(0, 0, 0, 0);
    entire_layout->setSpacing(0);

    // Initialize title bar widget
    m_titlebar_widget = new QWidget(this);
    entire_layout->addWidget(m_titlebar_widget);
    m_titlebar_widget->setFixedHeight(35); // Default title bar height is 35
    m_titlebar_widget->setContentsMargins(0, 0, 0, 0);
    m_titlebar_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Layout for title bar
    QHBoxLayout *titlebar_layout = new QHBoxLayout(this);
    m_titlebar_widget->setLayout(titlebar_layout);
    titlebar_layout->setContentsMargins(0, 0, 0, 0);
    titlebar_layout->setSpacing(0);

    QWidget *custom_titlebar_widget = new QWidget(this);
    titlebar_layout->addWidget(custom_titlebar_widget);
    custom_titlebar_widget->setContentsMargins(0, 0, 0, 0);
    custom_titlebar_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Minimize button setup.
    m_minimize_btn = new QPushButton(this);
    titlebar_layout->addWidget(m_minimize_btn);
    m_minimize_btn->setFixedWidth(46);
    m_minimize_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
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
    m_maximize_btn->setFixedWidth(46);
    m_maximize_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
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
    m_close_btn->setFixedWidth(46);
    m_close_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
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

    // Layout for title bar customization.
    m_custom_titlebar_layout = new QHBoxLayout(custom_titlebar_widget);
    custom_titlebar_widget->setLayout(m_custom_titlebar_layout);
    m_custom_titlebar_layout->setContentsMargins(0, 0, 0, 0);
    m_custom_titlebar_layout->setSpacing(0);
    m_custom_titlebar_layout->setAlignment(Qt::AlignLeft);

    QObject::connect(m_minimize_btn, &QPushButton::clicked, this, &MainWindow::onMinimizeButtonClicked);
    QObject::connect(m_maximize_btn, &QPushButton::clicked, this, &MainWindow::onMaximizeButtonClicked);
    QObject::connect(m_close_btn, &QPushButton::clicked, this, &MainWindow::onCloseButtonClicked);

    entire_layout->setAlignment(titlebar_layout, Qt::AlignTop);

    m_content_widget = new QWidget(this);
    entire_layout->addWidget(m_content_widget);
    m_content_widget->setContentsMargins(0, 0, 0, 0);
    m_content_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Set default title bar palette.
    auto pal = m_titlebar_widget->palette();
    pal.setColor(QPalette::Background, QColor(30, 34, 39));
    m_titlebar_widget->setAutoFillBackground(true);
    m_titlebar_widget->setPalette(pal);

    // Set default content widget palette.
    pal = m_content_widget->palette();
    pal.setColor(QPalette::Background, QColor(35, 39, 46));
    m_content_widget->setAutoFillBackground(true);
    m_content_widget->setPalette(pal);
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
        if (determineNonClickableWidgetUnderMouse(m_custom_titlebar_layout, local_x, local_y))
        {
            *result = HTCAPTION;
            return true;
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

// This is used to change the `active` state of widgets in custom title bar.
bool MainWindow::event(QEvent *evt)
{
    switch (evt->type())
    {
    case QEvent::WindowActivate: {
        setWidgetsActiveStateInCustomTitlebar(m_custom_titlebar_layout, true);
        break;
    }

    case QEvent::WindowDeactivate: {
        setWidgetsActiveStateInCustomTitlebar(m_custom_titlebar_layout, false);
        break;
    }

    default:
        break;
    }

    return QMainWindow::event(evt);
}

// Determine whether the current mouse coordinate is on the non-clickable widget or not using a recursive method.
bool MainWindow::determineNonClickableWidgetUnderMouse(QLayout *layout, int x, int y)
{
    if (!layout->count() && layout->geometry().contains(x, y))
        return true;

    for (size_t i = 0; i < layout->count(); i++)
    {
        auto item = layout->itemAt(i)->widget();
        if (item)
        {
            if (item->geometry().contains(x, y))
                return !item->property("clickable widget").toBool();
        }
        else
        {
            auto child_layout = layout->itemAt(i)->layout();
            if (child_layout && child_layout->geometry().contains(x, y))
                return determineNonClickableWidgetUnderMouse(child_layout, x, y);
        }
    }
    return false;
}

// Set `active' state using recursive method.
void MainWindow::setWidgetsActiveStateInCustomTitlebar(QLayout *layout, bool active_state)
{
    for (size_t i = 0; i < layout->count(); i++)
    {
        auto item = layout->itemAt(i)->widget();
        if (item)
        {
            item->setProperty("active", active_state);
            item->setStyleSheet(item->styleSheet());
        }
        else
        {
            auto child_layout = layout->itemAt(i)->layout();
            if (child_layout)
                setWidgetsActiveStateInCustomTitlebar(child_layout, active_state);
        }
    }
}

QWidget &MainWindow::getContentWidget()
{
    return *m_content_widget;
}

QWidget &MainWindow::getTitlebarWidget()
{
    return *m_titlebar_widget;
}

QHBoxLayout &MainWindow::getCustomTitlebarLayout()
{
    return *m_custom_titlebar_layout;
}

void MainWindow::setResizeBorderWidth(const int &resize_border_width)
{
    m_resize_border_width = resize_border_width;
}

void MainWindow::setTitlebarHeight(const int &titlebar_height)
{
    m_titlebar_widget->setFixedHeight(titlebar_height);
}

// Render again when frame is moved to another monitor.
void MainWindow::onScreenChanged(QScreen *screen)
{
    SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

void MainWindow::onMinimizeButtonClicked()
{
    SendMessage(m_hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void MainWindow::onMaximizeButtonClicked()
{
    SendMessage(m_hwnd, WM_SYSCOMMAND, m_maximize_btn->isChecked() ? SC_MAXIMIZE : SC_RESTORE, 0);

    // Remove the hover state from the maximize button.
    m_maximize_btn->setAttribute(Qt::WA_UnderMouse, false);
}

void MainWindow::onCloseButtonClicked()
{
    SendMessage(m_hwnd, WM_CLOSE, 0, 0);
}
