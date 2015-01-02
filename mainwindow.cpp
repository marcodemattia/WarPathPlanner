// Copyright 2009 Marco De Mattia, destroyar0@gmail.com
//
// This file is part of WarPath.
//
// WarPath is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// WarPath is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WarPath.  If not, see <http://www.gnu.org/licenses/>.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile.h>
#include <QFileDialog>
#include "filelistwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    activeView_(0)
{
    ui->setupUi(this);

    //    QFile file(":/default.txt");
    //    file.open(QIODevice::ReadOnly);
    //    model_ = new TreeModel(file.readAll());
    //    file.reset();
    //    sharedModel_ = new SharedModel(file.readAll());
    model_ = new TreeModel("");
    sharedModel_ = new SharedModel("");
    associatedUnitsModel_ = new AssociatedUnitsModel(sharedModel_);
    connect(associatedUnitsModel_, SIGNAL(setSharedUnitsRowSignal()), this, SLOT(setSelectedSharedOptionSlot()));
    //    file.close();
    setAllModels();

    ui->treeView->setColumnWidth(0, 200);
    ui->treeView->setColumnWidth(1, 50);
    ui->treeView->setColumnWidth(2, 50);
    ui->treeView->setColumnWidth(3, 100);
    ui->treeView->setColumnWidth(4, 200);
    ui->sharedTableView->setColumnWidth(0, 100);
    ui->sharedTableView->setColumnWidth(1, 40);
    ui->sharedTableView->setColumnWidth(2, 40);
    ui->sharedTableView->setColumnWidth(3, 65);
    ui->sharedTableView->setColumnWidth(4, 100);

    // Buttons
    connect(ui->addButton, SIGNAL(released()), this, SLOT(addButtonSlot()));
    connect(ui->addOptionButton, SIGNAL(released()), this, SLOT(addOptionButtonSlot()));
    connect(ui->removeButton, SIGNAL(released()), this, SLOT(removeButtonSlot()));
    connect(ui->upButton, SIGNAL(released()), this, SLOT(upButtonSlot()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(downButtonSlot()));

    // Shared options buttons
    connect(ui->addButton_shared, SIGNAL(released()), this, SLOT(addButtonSharedSlot()));
    connect(ui->removeButton_shared, SIGNAL(released()), this, SLOT(removeButtonSharedSlot()));
    connect(ui->upButton_shared, SIGNAL(released()), this, SLOT(upButtonSharedSlot()));
    connect(ui->downButton_shared, SIGNAL(released()), this, SLOT(downButtonSharedSlot()));

    // Editing
    connect(ui->nameEdit, SIGNAL(textEdited(QString)), this, SLOT(nameEditSlot(QString)));
    connect(this, SIGNAL(lineSelectedNameSignal(QString)), ui->nameEdit, SLOT(setText(QString)));
    connect(ui->typeEdit, SIGNAL(textEdited(QString)), this, SLOT(typeEditSlot(QString)));
    connect(this, SIGNAL(lineSelectedTypeSignal(QString)), ui->typeEdit, SLOT(setText(QString)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(treeViewClickedSlot(QModelIndex)));
    connect(ui->spinBox_min, SIGNAL(valueChanged(int)), this, SLOT(minValueChangedSlot(int)));
    // connect(ui->spinBox_max, SIGNAL(valueChanged(int)), this, SLOT(maxValueChangedSlot(int)));
    connect(ui->spinBox_max, SIGNAL(editingFinished()), this, SLOT(maxValueChangedSlot()));
    connect(ui->spinBox_cost, SIGNAL(valueChanged(int)), this, SLOT(costValueChangedSlot(int)));
    connect(this, SIGNAL(minChangedSignal(int)), ui->spinBox_min, SLOT(setValue(int)));
    connect(this, SIGNAL(maxChangedSignal(int)), ui->spinBox_max, SLOT(setValue(int)));
    connect(this, SIGNAL(costChangedSignal(int)), ui->spinBox_cost, SLOT(setValue(int)));
    connect(ui->specialEdit, SIGNAL(textEdited(QString)), this, SLOT(specialEditSlot(QString)));
    connect(this, SIGNAL(lineSelectedSpecialSignal(QString)), ui->specialEdit, SLOT(setText(QString)));

    // Shared options editing
    connect(ui->sharedTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(sharedTableClickedSlot(QModelIndex)));

    // Associated units
    connect(ui->pushButton_associate, SIGNAL(released()), this, SLOT(setAssociatedUnitSlot()));
    connect(ui->pushButton_removeAssociatedUnit, SIGNAL(released()), this, SLOT(removeAssociatedUnitSlot()));

    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(listViewClickedSlot(QModelIndex)));

    // Input/Output
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newDataFile()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(load()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionManage_DataFile_list, SIGNAL(triggered()), this, SLOT(fileListSlot()));

    // xModel
    connect(ui->checkBox_xModel, SIGNAL(toggled(bool)), this, SLOT(xModelCheckBox(bool)));
    connect(this, SIGNAL(xModelCheck(bool)), ui->checkBox_xModel, SLOT(setChecked(bool)));

    drawBackgroundImage("images/PlannerBackground.jpg");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model_;
}

/**
  * This is adding a new unit. There are three cases:
  * 1) no selection -> append at the end of the list
  * 2) another unit selected -> append after that unit
  * 3) an option is selected -> go back to find the parent unit (iteratively) and append after that
  */
void MainWindow::addButtonSlot()
{
    activeView_ = ui->treeView;
    QList<QVariant> columnData;
    columnData << "Name" << 1 << 1 << "Type" << "";

    // There is a selection
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        selectRootNode(selected);
        // insert after the current selection
        model_->insertChild(columnData, selected);
        selectRow(selected.row()+1);
    }
    else {
        // No selection case
        int row = -1;
        // If the list is not empty set the selection to the last entry
        if( model_->rootItem->childCount() != 0 ) {
            row = model_->rootItem->childCount()-1;
        }
        model_->insertChild(columnData, model_->index(row, 0));
        selectRow(row+1);
    }
    ui->nameEdit->setFocus();
    ui->nameEdit->selectAll();
}

/**
  * This is adding a new option. There are two cases:
  * 1) a unit or an option are selected -> append at the end of the respective list of options
  * 2) a suboption is selected -> do nothing.
  */
void MainWindow::addOptionButtonSlot()
{
    activeView_ = ui->treeView;
    QList<QVariant> columnData;
    columnData << "Name" << 1 << 1 << "Type" << "";
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        // Do nothing if the selected item is a suboption
        if( selected.parent().isValid() && selected.parent().parent().isValid() ) {
            return;
        }
        model_->insertChild(columnData, selected, false);
        // Open the node
        ui->treeView->expand(selected);
        selectRow(model_->pointer(selected)->childCount()-1, selected);
    }
    ui->nameEdit->setFocus();
    ui->nameEdit->selectAll();
}

void MainWindow::removeButtonSlot()
{
    activeView_ = ui->treeView;
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        model_->removeChild(selected);
        int rows = model_->rowCount(selected.parent());
        if( rows != 0 ) {
            int shift = 0;
            if( rows == selected.row() ) shift = 1;
            selectRow(selected.row()-shift, selected.parent());
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if( event->type() == QEvent::KeyPress ) {
        QKeyEvent * ke = static_cast<QKeyEvent *>(event);
        if( ke->key() == Qt::Key_Delete ) {
            removeButtonSlot();
        }
    }
}

void MainWindow::upButtonSlot() {
    activeView_ = ui->treeView;
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        if( model_->swapChilds(selected, true) ) {
            selectRow(selected.row()-1, selected.parent());
        }
    }
}

void MainWindow::downButtonSlot() {
    activeView_ = ui->treeView;
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        if( model_->swapChilds(selected, false) ) {
            selectRow(selected.row()+1, selected.parent());
        }
    }
}

void MainWindow::selectRootNode(QModelIndex & selected) const
{
    if( selected.parent().isValid() ) {
        selected = selected.parent().model()->index(selected.parent().row(), 0);
        selectRootNode(selected);
    }
}

//void MainWindow::selectTreeRow(const int row, const QModelIndex & parent) const
//{
//    QItemSelectionModel * selectionModel = activeView_->selectionModel();
//    selectRow(row, selectionModel, model_, parent);
//    treeViewClickedSlot(activeView_->selectionModel()->selectedIndexes().at(0));
//}

//void MainWindow::selectSharedRow(const int row, const QModelIndex & parent) const
//{
//    QItemSelectionModel * selectionModel = ui->sharedTableView->selectionModel();
//    selectRow(row, selectionModel, sharedModel_, parent);
//    sharedTableClickedSlot(ui->sharedTableView->selectionModel()->selectedIndexes().at(0));
//}

void MainWindow::selectRow(const int row, const QModelIndex & parent) const
{
    QItemSelectionModel * selectionModel = activeView_->selectionModel();
    QAbstractItemModel * model = activeView_->model();
    QModelIndex topLeft(model->index(row, 0, parent));
    QModelIndex bottomRight(model->index(row, 4, parent));
    QItemSelection selection(topLeft, bottomRight);
    // Using ClearAndSelect to impose single selection.
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
    if( activeView_ == ui->treeView ) treeViewClickedSlot(activeView_->selectionModel()->selectedIndexes().at(0));
    else sharedTableClickedSlot(activeView_->selectionModel()->selectedIndexes().at(0));
}

void MainWindow::nameEditSlot(const QString & name)
{
    setData(QVariant(name), 0);
}

void MainWindow::typeEditSlot(const QString & type)
{
    setData(QVariant(type), 3);
}

void MainWindow::specialEditSlot(const QString & special)
{
    setData(QVariant(special), 4);
}

void MainWindow::setData(const QVariant & value, const int column)
{
    if( activeView_ != 0 && activeView_->selectionModel() != 0 && !activeView_->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = activeView_->selectionModel()->selectedIndexes().at(0);
        // model_->pointer(selected)->setData(column, value);
        activeView_->model()->setData(selected, value, column);
    }
}

void MainWindow::treeViewClickedSlot(const QModelIndex & selected) const
{
    activeView_ = ui->treeView;
    const QList<QVariant> * data = model_->fullData(selected);
    emitClickedSlotSignals(data);
    if( model_->pointer(selected.parent()) == 0 ) ui->checkBox_xModel->setVisible(false);
    else ui->checkBox_xModel->setVisible(true);
}

void MainWindow::sharedTableClickedSlot(const QModelIndex & selected) const
{
    activeView_ = ui->sharedTableView;
    int row = selected.row();
    emitClickedSlotSignals(sharedModel_->fullData(row));
    associatedUnitsModel_->setSharedUnitsRow(row);
    ui->listView->setModel(0);
    ui->listView->setModel(associatedUnitsModel_);
    ui->checkBox_xModel->setVisible(true);
}

void MainWindow::emitClickedSlotSignals(const QList<QVariant> * data) const
{
    emit lineSelectedNameSignal(data->at(0).toString());
    emit lineSelectedTypeSignal(data->at(3).toString());
    QString special = data->at(4).toString();
    if( special.contains("xModel") ) {
        emit xModelCheck(true);
    }
    else {
        emit xModelCheck(false);
    }
    emit lineSelectedSpecialSignal(special);
    emitMinMax(data->at(1).toString());
    emit costChangedSignal(data->at(2).toInt());
}

void MainWindow::listViewClickedSlot(const QModelIndex & selected) const
{
    int a = selected.row();
    int b = 0;
}

void MainWindow::emitMinMax(const QString & numString) const
{
    int min = 0;
    int max = 0;
    if( numString.contains("-") ) {
        min = numString.split("-").at(0).toInt();
        max = numString.split("-").at(1).toInt();
    }
    else {
        min = numString.toInt();
        max = min;
    }
    emit minChangedSignal(min);
    emit maxChangedSignal(max);
}

void MainWindow::minValueChangedSlot(const int min)
{
    // QVariant num = model_->pointer(ui->treeView->selectionModel()->selectedIndexes().at(0))->data(1);
    int row = activeView_->selectionModel()->selectedIndexes().at(0).row();
    QModelIndex index = activeView_->model()->index(row, 1, activeView_->selectionModel()->selectedIndexes().at(0).parent());
    QVariant num = activeView_->model()->data(index);
    QString numString = num.toString();
    if( numString.contains("-") )
    {
        setMinMax(min, QVariant(numString.split("-").at(1)).toInt(), true);
    }
    else {
        setMinMax(min, numString.toInt(), true);
    }
}

// void MainWindow::maxValueChangedSlot(const int max)
void MainWindow::maxValueChangedSlot()
{
    // QVariant num = model_->pointer(ui->treeView->selectionModel()->selectedIndexes().at(0))->data(1);
    int max = ui->spinBox_max->value();
    int row = activeView_->selectionModel()->selectedIndexes().at(0).row();
    QModelIndex index = activeView_->model()->index(row, 1, activeView_->selectionModel()->selectedIndexes().at(0).parent());
    QVariant num = activeView_->model()->data(index);
    QString numString = num.toString();
    if( numString.contains("-") )
    {
        setMinMax(QVariant(numString.split("-").at(0)).toInt(), max, false);
    }
    else {
        setMinMax(numString.toInt(), max, false);
    }
}

void MainWindow::setMinMax(const int min, const int max, const bool setMin)
{
    if( min >= max ) {
        if( setMin ) {
            setData(QVariant(min), 1);
            emit maxChangedSignal(min);
        }
        else {
            setData(QVariant(max), 1);
            emit minChangedSignal(max);
        }
    }
    else {
        setData(QVariant(min).toString()+"-"+QVariant(max).toString(), 1);
    }
}

void MainWindow::costValueChangedSlot(const int value)
{
    setData(QVariant(value), 2);
}

void MainWindow::addButtonSharedSlot()
{
    activeView_ = ui->sharedTableView;
    QList<QVariant> columnData;
    columnData << "Name" << 1 << 1 << "Type" << "";

    // There is a selection
    if( ui->sharedTableView->selectionModel() != 0 && !ui->sharedTableView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->sharedTableView->selectionModel()->selectedIndexes().at(0);
        // insert after the current selection
        sharedModel_->insertChild(columnData, selected);
        selectRow(selected.row()+1);
    }
    else {
        // No selection case
        int row = -1;
        // If the list is not empty set the selection to the last entry
        if( sharedModel_->rowCount() != 0 ) {
            row = sharedModel_->rowCount()-1;
        }
        sharedModel_->insertChild(columnData, sharedModel_->index(row, 0));
        selectRow(row+1);
    }
    ui->nameEdit->setFocus();
    ui->nameEdit->selectAll();
}

void MainWindow::removeButtonSharedSlot()
{
    activeView_ = ui->sharedTableView;
    if( ui->sharedTableView->selectionModel() != 0 && !ui->sharedTableView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->sharedTableView->selectionModel()->selectedIndexes().at(0);
        sharedModel_->removeChild(selected);
        int rows = sharedModel_->rowCount(selected.parent());
        if( rows != 0 ) {
            int shift = 0;
            if( rows == selected.row() ) shift = 1;
            selectRow(selected.row()-shift, selected.parent());
        }
    }
}

void MainWindow::upButtonSharedSlot()
{
    activeView_ = ui->sharedTableView;
    if( ui->sharedTableView->selectionModel() != 0 && !ui->sharedTableView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->sharedTableView->selectionModel()->selectedIndexes().at(0);
        if( sharedModel_->swapChilds(selected, true) ) {
            selectRow(selected.row()-1, selected.parent());
        }
    }
}

void MainWindow::downButtonSharedSlot()
{
    activeView_ = ui->sharedTableView;
    if( ui->sharedTableView->selectionModel() != 0 && !ui->sharedTableView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->sharedTableView->selectionModel()->selectedIndexes().at(0);
        if( sharedModel_->swapChilds(selected, false) ) {
            selectRow(selected.row()+1, selected.parent());
        }
    }
}

void MainWindow::setSelectedSharedOptionSlot()
{
    if( ui->sharedTableView->selectionModel() != 0 && !ui->sharedTableView->selectionModel()->selectedIndexes().isEmpty() ) {
        associatedUnitsModel_->setSharedUnitsRow(ui->sharedTableView->selectionModel()->selectedIndexes().at(0).row());
    }
    else {
        associatedUnitsModel_->setSharedUnitsRow(-1);
    }
}

void MainWindow::setAssociatedUnitSlot()
{
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        associatedUnitsModel_->setData(model_->fullData(ui->treeView->selectionModel()->selectedIndexes().at(0))->at(0));
        ui->listView->setModel(0);
        ui->listView->setModel(associatedUnitsModel_);
    }
}

void MainWindow::removeAssociatedUnitSlot()
{
    if( ui->listView->selectionModel() != 0 && !ui->listView->selectionModel()->selectedIndexes().isEmpty() ) {
        associatedUnitsModel_->removeRows(ui->listView->selectionModel()->selectedIndexes().at(0).row());
    }
}

void MainWindow::xModelCheckBox(const bool checked)
{
    QString special(ui->specialEdit->text());
    if( checked ) {
        if( special.isEmpty() ) special = "xModel";
        else special = "xModel, "+special;
    }
    else {
        if(special.contains("xModel, ")) special = special.remove("xModel, ");
        else special = special.remove("xModel");
    }
    setData(special, 4);
    emit lineSelectedSpecialSignal(special);
}

void MainWindow::drawBackgroundImage(const QString & imageName)
{
    QString backgroundImage = QString::fromUtf8("QMainWindow {background-image: url(");
    if( !imageName.isEmpty() ) {
        backgroundImage += imageName;
    }
    backgroundImage += QString::fromUtf8("); background-clip: content;}");
    setStyleSheet(backgroundImage);


    // Set transparent background to all the other windows
    QString style("");
    style += QString::fromUtf8("QTableView, ");
    style += QString::fromUtf8("QListView, ");
    style += QString::fromUtf8("QTreeView, ");
    style += QString::fromUtf8("QFrame, ");
    style += QString::fromUtf8("QLabel ");
    style += QString::fromUtf8("{");
    style += QString::fromUtf8("background-color: rgba(255, 255, 255,");
    style += "220";
    style += QString::fromUtf8(");} ");

    ui->frame->setStyleSheet(style);

    style = "";
    style += QString::fromUtf8("QTableView, ");
    style += QString::fromUtf8("QListView, ");
    style += QString::fromUtf8("QTreeView, ");
    style += QString::fromUtf8("QFrame, ");
    style += QString::fromUtf8("QLabel ");
    style += QString::fromUtf8("{");
    style += QString::fromUtf8("background-color: rgba(255, 255, 255,");
    style += "200";
    style += QString::fromUtf8(");} ");

    ui->treeView->setStyleSheet(style);
    ui->sharedTableView->setStyleSheet(style);
    ui->listView->setStyleSheet(style);

}

void MainWindow::save()
{
    // Open a QFileDialog to get the file name.
    QString fileName;
    if( getOutFileName(fileName, ".txt") ) {
        QFile data(fileName);
        if( data.open(QFile::WriteOnly | QFile::Truncate) ) {
            QTextStream out(&data);
            out << "# Datafile" << endl;
            // Loop recursively on the elements and write them
            TreeItem * item = model_->rootItem;
            for( int i=0; i<item->childCount(); ++i ) {
                writeItem(out, item->child(i), "");
            }
            out << endl;
            writeSharedItems(out);
        }
    }
}

void MainWindow::writeItem(QTextStream & out, TreeItem * item, const QString & prepend) const
{
    if( prepend == "" ) out << endl;
    // Write the item
    writeLine(out, item, prepend);
    // Write the children of the item
    for( int i=0; i<item->childCount(); ++i ) {
        writeItem(out, item->child(i), prepend.trimmed()+"- ");
    }
}

void MainWindow::writeSharedItems(QTextStream & out) const
{
    for( int row=0; row<sharedModel_->rowCount(); ++row ) {
        const SharedItem * sharedItem = sharedModel_->data(row);
        for( int i=0; i<sharedItem->getAssociatedUnitsSize(); ++i ) {
            out << "@ " << sharedItem->associatedUnit(i) << " ";
        }
        writeLine(out, sharedItem, ": ");
    }
}

template <class T>
void MainWindow::writeLine(QTextStream & out, T * item, const QString & prepend) const
{
    for( int j=0; j<5; ++j ) {
        if( j == 0 ) out << prepend;
        out << item->data(j).toString();
        if( j < 4 ) out << " ; ";
        else out << endl;
    }
}

bool MainWindow::getOutFileName(QString & fileName, const QString & extension)
{
    // Open a QFileDialog to get the file name.
    fileName = QFileDialog::getSaveFileName(this);
    if( !fileName.isEmpty() ) {
        if( !fileName.contains(".") )fileName += extension;
        return true;
    }
    return false;
}

void MainWindow::load()
{
    QString fileName(QFileDialog::getOpenFileName(this));
    if( fileName == "" ) return;
    loadFile(fileName);
}

void MainWindow::loadFile(const QString & fileName)
{
    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);
    model_ = new TreeModel(inputFile.readAll());
    inputFile.reset();
    sharedModel_ = new SharedModel(inputFile.readAll());
    associatedUnitsModel_ = new AssociatedUnitsModel(sharedModel_);
    connect(associatedUnitsModel_, SIGNAL(setSharedUnitsRowSignal()), this, SLOT(setSelectedSharedOptionSlot()));
    inputFile.close();
    setAllModels();
}

void MainWindow::newDataFile()
{
    model_ = new TreeModel("");
    sharedModel_ = new SharedModel("");
    associatedUnitsModel_ = new AssociatedUnitsModel(sharedModel_);
    connect(associatedUnitsModel_, SIGNAL(setSharedUnitsRowSignal()), this, SLOT(setSelectedSharedOptionSlot()));
    setAllModels();
}

void MainWindow::setAllModels()
{
    ui->treeView->setModel(model_);
    ui->sharedTableView->setModel(sharedModel_);
    ui->listView->setModel(associatedUnitsModel_);
}

void MainWindow::fileListSlot()
{
    FileListWindow * fileListWindow = new FileListWindow(this);
    fileListWindow->exec();
}
