#include "MainWindow.hpp"

#include <QMetaObject>
#include <QQmlContext>
#include <Windows.h>
#include <Windowsx.h>
#include <dwmapi.h>
#include <memory>
#include <stdexcept>

MainWindow::MainWindow(QQmlApplicationEngine *engine)
{
    if (engine)
        initWindow(*engine);
}

MainWindow::~MainWindow()
{
}

HWND MainWindow::getHandle()
{
    return m_hwnd;
}

bool MainWindow::initWindow(QQmlApplicationEngine &engine)
{
    m_quick_window = qobject_cast<QQuickWindow *>(engine.rootObjects().at(0));

    if (!m_quick_window)
        return false;

    m_hwnd = reinterpret_cast<HWND>(m_quick_window->winId());
    m_resize_border_width = m_quick_window->property("resizeBorderWidth").toInt() * m_quick_window->devicePixelRatio();

    QObject::connect(m_quick_window, &QQuickWindow::screenChanged, this, &MainWindow::onScreenChanged);

    // Set window shadows.
    const MARGINS aero_shadow_on = {1, 1, 1, 1};
    ::DwmExtendFrameIntoClientArea(m_hwnd, &aero_shadow_on);

    // Install event handler
    engine.installEventFilter(this);

    // Make cppConnector obj so that we can connect functions for Minimize, Maximize / Restore, Close feature.
    engine.rootContext()->setContextProperty("cppConnector", this);

    return true;
}

// Render again when frame is moved to another monitor.
void MainWindow::onScreenChanged(QScreen *screen)
{
    SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    switch (evt->type())
    {
    // case QEvent::Enter:
    //     break;
    // case QEvent::Leave:
    //     break;
    default:
        break;
    }

    return QObject::eventFilter(obj, evt);
}

bool MainWindow::nativeEventFilter(const QByteArray &event_type, void *message, long *result)
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

                // Save window mode state.
                m_quick_window->findChild<QObject *>("maximumButton")->setProperty("checked", true);
            }
            else if (wp.showCmd == SW_NORMAL)
                // Save window mode state.
                m_quick_window->findChild<QObject *>("maximumButton")->setProperty("checked", false);
        }
        return true;
    }

    // Process the mouse when it is on the window border.
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

    // Process window system menu
    case WM_SYSCOMMAND: {
        if (msg->wParam == VK_SPACE || (msg->wParam == SC_KEYMENU && msg->lParam == VK_SPACE))
        {
            HMENU menu = GetSystemMenu(m_hwnd, FALSE);
            if (menu)
            {
                MENUITEMINFO mii;
                mii.cbSize = sizeof(MENUITEMINFO);
                mii.fMask = MIIM_STATE;
                mii.fType = 0;

                mii.fState = MF_ENABLED;
                SetMenuItemInfo(menu, SC_RESTORE, FALSE, &mii);
                SetMenuItemInfo(menu, SC_SIZE, FALSE, &mii);
                SetMenuItemInfo(menu, SC_MOVE, FALSE, &mii);
                SetMenuItemInfo(menu, SC_MAXIMIZE, FALSE, &mii);
                SetMenuItemInfo(menu, SC_MINIMIZE, FALSE, &mii);

                mii.fState = MF_GRAYED;

                WINDOWPLACEMENT wp;
                GetWindowPlacement(m_hwnd, &wp);

                switch (wp.showCmd)
                {
                case SW_SHOWMAXIMIZED:
                    SetMenuItemInfo(menu, SC_SIZE, FALSE, &mii);
                    SetMenuItemInfo(menu, SC_MOVE, FALSE, &mii);
                    SetMenuItemInfo(menu, SC_MAXIMIZE, FALSE, &mii);
                    SetMenuDefaultItem(menu, SC_CLOSE, FALSE);
                    break;
                case SW_SHOWMINIMIZED:
                    SetMenuItemInfo(menu, SC_MINIMIZE, FALSE, &mii);
                    SetMenuDefaultItem(menu, SC_RESTORE, FALSE);
                    break;
                case SW_SHOWNORMAL:
                    SetMenuItemInfo(menu, SC_RESTORE, FALSE, &mii);
                    SetMenuDefaultItem(menu, SC_CLOSE, FALSE);
                    break;
                }

                RECT winrect;
                GetWindowRect(m_hwnd, &winrect);

                LPARAM cmd = TrackPopupMenu(menu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                                            // When the window is maximized, the pop-up menu activated by "Alt + Space" invades the other monitor.
                                            // To fix this, move the window a bit more to the left.
                                            winrect.left + (wp.showCmd == SW_SHOWMAXIMIZED ? 8 : 0),
                                            winrect.top, NULL, m_hwnd, nullptr);

                if (cmd)
                    PostMessage(m_hwnd, WM_SYSCOMMAND, cmd, 0);
            }
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

// Activated when the user clicks the Minimize button.
void MainWindow::onMinimizeButtonClicked()
{
    SendMessage(m_hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

// Activated when the user clicks the Maxmimize button.
void MainWindow::onMaximizeButtonClicked()
{
    bool checked = m_quick_window->findChild<QObject *>("maximumButton")->property("checked").toBool();
    SendMessage(m_hwnd, WM_SYSCOMMAND, checked ? SC_MAXIMIZE : SC_RESTORE, 0);
}

// Activated when the user clicks the Close button.
void MainWindow::onCloseButtonClicked()
{
    SendMessage(m_hwnd, WM_CLOSE, 0, 0);
}