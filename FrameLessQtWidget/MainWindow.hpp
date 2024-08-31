#ifndef HEADER__FILE__MAINWINDOW
#define HEADER__FILE__MAINWINDOW

#include <QApplication>
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

    std::vector<QLayoutItem *> m_movable_area;
    std::vector<QWidget *> m_activable_widgets;

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    bool nativeEvent(const QByteArray &event_type, void *message, long *result);
    bool event(QEvent *evt);
    void OnScreenChanged(QScreen *screen);
    void OnMinimizeButtonClicked();
    void OnMaximizeButtonClicked();
    void OnCloseButtonClicked();
};

#endif // HEADER__FILE__MAINWINDOW
