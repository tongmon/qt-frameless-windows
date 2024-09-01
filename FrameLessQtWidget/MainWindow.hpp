#ifndef HEADER__FILE__MAINWINDOW
#define HEADER__FILE__MAINWINDOW

#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QScreen>
#include <QSpacerItem>
#include <QWindow>
#include <Windows.h>
#include <Windowsx.h>
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    HWND m_hwnd;
    int m_resize_border_width;

    QPushButton *m_minimize_btn;
    QPushButton *m_maximize_btn;
    QPushButton *m_close_btn;

    QWidget *m_content_widget;
    QWidget *m_titlebar_widget;
    QHBoxLayout *m_custom_titlebar_layout;

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setResizeBorderWidth(const int &resize_border_width);
    void setTitlebarHeight(const int &titlebar_height);
    QWidget &getContentWidget();
    QWidget &getTitlebarWidget();
    QHBoxLayout &getCustomTitlebarLayout();

  private:
    bool nativeEvent(const QByteArray &event_type, void *message, long *result);
    bool event(QEvent *evt);
    bool determineNonClickableWidgetUnderMouse(QLayout *layout, int x, int y);
    void setWidgetsActiveStateInCustomTitlebar(QLayout *layout, bool active_state);
    void onScreenChanged(QScreen *screen);
    void onMinimizeButtonClicked();
    void onMaximizeButtonClicked();
    void onCloseButtonClicked();
};

#endif // HEADER__FILE__MAINWINDOW
