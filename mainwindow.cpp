#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    entries = new QStringList();
    searchResults = new QStringList();
    searchIndexes = new QList<int>();
    load();
    setUpdate(false);
}

MainWindow::~MainWindow()
{
    delete entries;
    delete searchResults;
    delete searchIndexes;
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    if (modified)
    {
        int answ = QMessageBox::question(this, NULL, "Kívánsz menteni egyet?");
        if (answ == QMessageBox::Yes) {
            save();
        }
    }

    QApplication::exit(0);
}

void MainWindow::on_pushButton_addModify_clicked()
{
    addModifyWord();
}

void MainWindow::on_listWidget_itemChanged(QListWidgetItem *item)
{

}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if (currentRow > -1 ) {
    updateAddModifyFields(currentRow);
    }
}

void MainWindow::updateAddModifyFields(int currentRow)
{
    // valtaskor
    if (currentRow> -1) {
    int idx = ui->listWidget->currentIndex().row();
    QString currentEntry = entries->at(idx);

    ui->statusBar->showMessage(QString::number(idx) + '/' + QString::number(entries->count()));

    if (currentEntry.contains("="))
    {
        QStringList parts = currentEntry.split("=");
        ui->lineEdit_word->setText(parts.at(0));
        ui->lineEdit_meaning->setText(parts.at(1));
    }
    else
    {
        QStringList parts = currentEntry.split("=");
        ui->lineEdit_word->setText(currentEntry);
        ui->lineEdit_meaning->clear();
    }

    setUpdate(true);
    }
}


void MainWindow::on_pushButton_search_clicked()
{
  searchNow();
}

void MainWindow::searchNow()
{
    // keresés
    QString sfor = ui->lineEdit_searchfor->text();
    searchResults->clear();
    searchIndexes->clear();
    ui->listWidget_searchResults->clear();

    //qDebug() << searchIndexes->length();

    for (int i = 0; i < entries->length(); i++)
    {
        if(entries->at(i).contains(sfor, Qt::CaseInsensitive)) {
            searchResults->append(entries->at(i));
            searchIndexes->append(i);
        }
    }

    for (int i = 0; i < searchResults->length(); i++)
    {
        ui->listWidget_searchResults->addItem(searchResults->at(i));
    }
    ui->listWidget_searchResults->setEnabled(true);
    ui->listWidget_searchResults->setFocus();
}

void MainWindow::on_listWidget_searchResults_currentRowChanged(int currentRow)
{
    // ráugrás a keresendő elemre
    if (currentRow > -1) {
    ui->listWidget->setCurrentRow(searchIndexes->at(currentRow));
   }
}

void MainWindow::on_pushButton_delete_clicked()
{
    deleteWord();
}


void MainWindow::deleteWord()
{
    int idx = ui->listWidget->row(ui->listWidget->currentItem());

    if (idx == -1) {
     QMessageBox::information(this,NULL,"Nem jelölt ki elemet, vagy nincsenek elemek!");
    }
    else
        {

    int choice = QMessageBox::question(this, "Törlés", "Biztosan törli ezt? \n \n" + ui->listWidget->currentItem()->text());
    if ( choice == QMessageBox::Yes )
    {


    qDebug() << "ezt törölném: " << idx;
    qDebug() << entries->length();
    qDebug() << ui->listWidget->count();
    qDebug() << ui->listWidget->currentRow();
    delete ui->listWidget->takeItem(idx);
    entries->takeAt(idx);
    setUpdate(false);

    // kereséseknél is törölni.

    for (int i = 0; i < searchIndexes->length(); i++)
    {
        qDebug() << "itt " << searchIndexes->at(i);
        int ati = searchIndexes->at(i);
        if (searchIndexes->at(i) == idx)
        {
            searchIndexes->takeAt(i);
            searchResults->takeAt(i);
            delete ui->listWidget_searchResults->takeItem(i);
        }
    }

      setModified(true);
    }


    }
}

void MainWindow::addModifyWord(){

    // uj bejegyzes - modositas
        QString newEntry = ui->lineEdit_word->text() + "=" + ui->lineEdit_meaning->text();
    if (rewrite) {
        entries->replace(ui->listWidget->currentIndex().row(), newEntry);
        ui->listWidget->item(ui->listWidget->currentIndex().row())->setText(newEntry);
    } else {
        entries->append(newEntry);
        ui->listWidget->addItem(newEntry);
    }
    ui->lineEdit_word->clear();
    ui->lineEdit_meaning->clear();

    setModified(true);
    setUpdate(false);

}

void MainWindow::setUpdate(bool upd)
{
    if (upd)
    {
        ui->pushButton_addModify->setText("Módosít");
    }
    else
    {
        ui->lineEdit_word->clear();
        ui->lineEdit_meaning->clear();
        ui->pushButton_addModify->setText("Új");
    }
    rewrite = upd;
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    ui->lineEdit_word->setFocus();
}

void MainWindow::on_listWidget_searchResults_doubleClicked(const QModelIndex &index)
{
    updateAddModifyFields(ui->listWidget->currentRow());
    ui->lineEdit_word->setFocus();
}

void MainWindow::on_actionNew_triggered()
{
  setUpdate(false);
  ui->lineEdit_word->setFocus();
}

void MainWindow::on_actionEdit_triggered()
{
    if (ui->listWidget->currentRow() == -1)
    {
      QMessageBox::information(this,NULL,"Nem jelölt ki elemet.");
    }
    else
    {
       updateAddModifyFields(ui->listWidget->currentRow());
       ui->lineEdit_word->setFocus();
    }
}

void MainWindow::on_actionGo_To_Words_triggered()
{

    ui->listWidget->setFocus();
}

void MainWindow::on_actionDelete_triggered()
{
    deleteWord();
}

void MainWindow::on_lineEdit_meaning_returnPressed()
{
    addModifyWord();
    ui->lineEdit_word->setFocus();
}

void MainWindow::on_listWidget_activated(const QModelIndex &index)
{
    if (ui->listWidget->currentRow() > -1)
    {
         updateAddModifyFields(ui->listWidget->currentRow());
    }
}

void MainWindow::on_actionSearch_For_triggered()
{
    ui->lineEdit_searchfor->setFocus();
}

void MainWindow::on_lineEdit_searchfor_returnPressed()
{
    searchNow();
}

void MainWindow::on_actionReload_triggered()
{
    load();
}

void MainWindow::load()
{
    // betoltes
    QFile tfile(QDir::home().absoluteFilePath("words.txt"));
    if (!tfile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(NULL, NULL, "Nem lehetett betölteni a words.txt fájlt a HOME könyvtárból.");
    }

    QTextStream in(&tfile);

    entries->clear();
    ui->listWidget->clear();

    while (!in.atEnd()) {
        QString line = in.readLine();
        entries->append(line);
        ui->listWidget->addItem(line);
    }

    tfile.close();

    setModified(false);

    ui->listWidget->setFocus();
}

void MainWindow::save()
{
    QFile tfile(QDir::home().absoluteFilePath("words.txt"));
    if (!tfile.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, NULL, "Nem lehetett elmenteni a words.txt fájlt a HOME könyvtárból.");
    }

    QTextStream out(&tfile);

    for (int i = 0; i < entries->length(); i++)
    {
        out << entries->at(i) << '\n';
    }
    tfile.close();

    setModified(false);
}

void MainWindow::on_actionSave_triggered()
{
    save();
}

void MainWindow::setModified(bool modd) {
    modified = modd;
    if (modified) {
       setWindowTitle("Vocabulary*");
    } else {
       setWindowTitle("Vocabulary");
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, NULL, "Vocabulary version V \n Verzió 5.0.0 BETA \n Elekes Dávid 2017.");
}
