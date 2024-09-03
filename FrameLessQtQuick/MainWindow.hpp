#ifndef HEADER__FILE__MAINWINDOW
#define HEADER__FILE__MAINWINDOW

#include <QAbstractNativeEventFilter>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QScreen>

class MainWindow : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

    QQuickWindow *m_quick_window;
    HWND m_hwnd;
    int m_resize_border_width;

  public:
    MainWindow(QQmlApplicationEngine *engine = nullptr);
    ~MainWindow();

    HWND getHandle();
    bool initWindow(QQmlApplicationEngine &engine);
    void onScreenChanged(QScreen *screen);

    bool eventFilter(QObject *obj, QEvent *evt);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool nativeEventFilter(const QByteArray &event_type, void *message, long *result);
#else
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result);
#endif

    // Functions used in main.qml files.
    Q_INVOKABLE void onMinimizeButtonClicked();
    Q_INVOKABLE void onMaximizeButtonClicked();
    Q_INVOKABLE void onCloseButtonClicked();
};

#endif // HEADER__FILE__MAINWINDOW
