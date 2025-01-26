#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<std::string> getDeskNames();
    std::vector<std::tuple<std::string, std::string, int>> getLinks();
private slots:
    void on_buttonCreateDesk_clicked();

    void on_buttonCreateLink_clicked();

    void on_buttonFindPath_clicked();

    void on_ResetButton_clicked();

private:
    Ui::MainWindow *ui;
    int dijkstra(const std::string& start, const std::string& end, std::vector<std::string>& path);
};
#endif // MAINWINDOW_H
