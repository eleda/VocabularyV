#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    //void on_pushButton_3_clicked();

    void on_pushButton_addModify_clicked();

    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_search_clicked();

    void on_listWidget_searchResults_currentRowChanged(int currentRow);

    void on_pushButton_delete_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_listWidget_searchResults_doubleClicked(const QModelIndex &index);

    void on_actionNew_triggered();

    void on_actionEdit_triggered();

    void on_actionGo_To_Words_triggered();

    void on_actionDelete_triggered();

    void on_lineEdit_meaning_returnPressed();

    void on_listWidget_activated(const QModelIndex &index);

    void on_actionSearch_For_triggered();

    void on_lineEdit_searchfor_returnPressed();

    void on_actionReload_triggered();

    void on_actionSave_triggered();

    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QStringList *entries;
    QStringList *searchResults;
    QList<int> *searchIndexes;
    bool modified;
    bool rewrite;
    void load();
    void save();
    void setUpdate(bool);
    void addModifyWord();
    void deleteWord();
    void searchNow();
    void updateAddModifyFields(int currentRow);
    void setModified(bool);
};

#endif // MAINWINDOW_H
