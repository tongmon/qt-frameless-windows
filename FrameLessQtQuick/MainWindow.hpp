#ifndef HEADER__FILE__MAINWINDOW
#define HEADER__FILE__MAINWINDOW

#include <QAbstractNativeEventFilter>
#include <QQuickWindow>
#include <QScreen>

class MainWindow : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

    QQuickWindow *m_quick_window;
    HWND m_hwnd;
    int m_resize_border_width;

  public:
    MainWindow(QQuickWindow *quick_window = nullptr);
    ~MainWindow();

    HWND GetHandle();
    bool SetQuickWindow(QQuickWindow *quick_window);
    void OnScreenChanged(QScreen *screen);

    bool eventFilter(QObject *obj, QEvent *evt);
    bool nativeEventFilter(const QByteArray &event_type, void *message, long *result);

    // Functions used in main.qml files.
    Q_INVOKABLE void onMinimizeButtonClicked();
    Q_INVOKABLE void onMaximizeButtonClicked();
    Q_INVOKABLE void onCloseButtonClicked();
};

#endif // HEADER__FILE__MAINWINDOW
