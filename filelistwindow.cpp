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

#include "filelistwindow.h"
#include "ui_filelistwindow.h"
#include <QFile>
#include <QFileDialog>

FileListWindow::FileListWindow(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::FileListWindow),
    modified_(false)
{
    ui->setupUi(this);

    QFile file("FileList.txt");
    file.open(QIODevice::ReadOnly);
    fileListTreeModel_ = new FileListTreeModel(file.readAll());
    file.close();

    ui->treeView->setModel(fileListTreeModel_);

    ui->treeView->setColumnWidth(0, 200);
    ui->treeView->setColumnWidth(1, 150);
    ui->treeView->setColumnWidth(2, 50);

    connect(ui->pushButton_AddGroup, SIGNAL(released()), this, SLOT(addGroupSlot()));
    connect(ui->pushButton_AddDataFile, SIGNAL(released()), this, SLOT(addDataFileSlot()));
    connect(ui->pushButton_AddBackgroundImage, SIGNAL(released()), this, SLOT(addBackgroundImageSlot()));
    connect(ui->pushButton_Remove, SIGNAL(released()), this, SLOT(removeSlot()));
    connect(ui->pushButton_Edit, SIGNAL(released()), this, SLOT(editSlot()));
    connect(ui->pushButton_Save, SIGNAL(released()), this, SLOT(saveSlot()));
    connect(ui->pushButton_Close, SIGNAL(released()), this, SLOT(closeSlot()));
    connect(ui->upButton, SIGNAL(released()), this, SLOT(upSlot()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(downSlot()));
    connect(this, SIGNAL(editFile(QString)), parent, SLOT(loadFile(QString)));
}

FileListWindow::~FileListWindow()
{
    delete ui;
}

void FileListWindow::addGroupSlot()
{
    modified_ = true;
    QList<QVariant> columnData;
    columnData << "Group Name" << "" << "" << "" << "";
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        selectRootNode(selected);
        // insert after the current selection
        fileListTreeModel_->insertChild(columnData, selected);
        selectRow(selected.row()+1);
    }
    else {
        // No selection case
        int row = -1;
        // If the list is not empty set the selection to the last entry
        if( fileListTreeModel_->rootItem->childCount() != 0 ) {
            row = fileListTreeModel_->rootItem->childCount()-1;
        }
        fileListTreeModel_->insertChild(columnData, fileListTreeModel_->index(row, 0));
        selectRow(row+1);
    }
}

void FileListWindow::selectRow(const int row, const QModelIndex & parent) const
{
    QItemSelectionModel * selectionModel = ui->treeView->selectionModel();
    QModelIndex topLeft(fileListTreeModel_->index(row, 0, parent));
    QModelIndex bottomRight(fileListTreeModel_->index(row, 2, parent));
    QItemSelection selection(topLeft, bottomRight);
    // Using ClearAndSelect to impose single selection.
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
}

void FileListWindow::selectRootNode(QModelIndex & selected) const
{
    if( selected.parent().isValid() ) {
        selected = selected.parent().model()->index(selected.parent().row(), 0);
        selectRootNode(selected);
    }
}

void FileListWindow::addDataFileSlot()
{
    modified_ = true;
    QString fileName(QFileDialog::getOpenFileName(this));
    if( fileName == "" ) return;

    QList<QVariant> columnData;
    columnData << fileName << "" << "" << "" << "";
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        // Do nothing if the selected item is a suboption
        if( selected.parent().isValid() ) {
            fileListTreeModel_->insertChild(columnData, selected.parent(), false);
            return;
        }
        fileListTreeModel_->insertChild(columnData, selected, false);
        // Open the node
        ui->treeView->expand(selected);
        selectRow(fileListTreeModel_->pointer(selected)->childCount()-1, selected);
    }
}

void FileListWindow::addBackgroundImageSlot()
{
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        if( selected.parent().isValid() ) {
            QString fileName(QFileDialog::getOpenFileName(this));
            if( fileName == "" ) return;
            modified_ = true;
            fileListTreeModel_->setData(selected, fileName, 3);
        }
    }
}

void FileListWindow::removeSlot()
{
    modified_ = true;
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        fileListTreeModel_->removeChild(selected);
        int rows = fileListTreeModel_->rowCount(selected.parent());
        if( rows != 0 ) {
            int shift = 0;
            if( rows == selected.row() ) shift = 1;
            selectRow(selected.row()-shift, selected.parent());
        }
    }
}

void FileListWindow::editSlot()
{
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        emit editFile(fileListTreeModel_->fullData(selected)->at(0).toString());
        accept();
    }
}

void FileListWindow::saveSlot(const bool save)
{
    if( save ) saveSlot();
}

void FileListWindow::saveSlot()
{
    if( modified_ ) {
        // Open a QFileDialog to get the file name.
        QString fileName = "FileList.txt";
        // QString fileName;
        // if( getOutFileName(fileName, ".txt") ) {
        QFile data(fileName);
        if( data.open(QFile::WriteOnly | QFile::Truncate) ) {
            QTextStream out(&data);
            out << "DefaultBackgroundImage: images/WarPathDefaultBackground.jpg; 150" << endl << endl;
            // Loop recursively on the elements and write them
            TreeItem * item = fileListTreeModel_->rootItem;
            for( int i=0; i<item->childCount(); ++i ) {
                writeItem(out, item->child(i), "");
            }
            out << endl;
        }
    }
    modified_ = false;
    // }
    // accept();
}

bool FileListWindow::getOutFileName(QString & fileName, const QString & extension)
{
    // Open a QFileDialog to get the file name.
    fileName = QFileDialog::getSaveFileName(this);
    if( !fileName.isEmpty() ) {
        if( !fileName.contains(".") )fileName += extension;
        return true;
    }
    return false;
}

void FileListWindow::writeItem(QTextStream & out, TreeItem * item, const QString & prepend) const
{
    if( prepend == "" ) out << endl;
    // Write the item
    writeLine(out, item, prepend);
    // Write the children of the item
    for( int i=0; i<item->childCount(); ++i ) {
        writeItem(out, item->child(i), prepend.trimmed()+"- ");
    }
}

template <class T>
void FileListWindow::writeLine(QTextStream & out, T * item, const QString & prepend) const
{
    if( prepend == "" ) out << prepend << item->data(0).toString() << endl;
    else out << prepend << item->data(0).toString() << " ; " << item->data(3).toString() << " ; " << item->data(2).toString() << endl;
}

void FileListWindow::closeSlot()
{
    if( modified_ ) {
        closeConfirm_ = new CloseConfirm;
        connect(closeConfirm_, SIGNAL(saveChanges(bool)), this, SLOT(saveSlot(bool)));
        closeConfirm_->exec();
    }
    reject();
}

void FileListWindow::upSlot() {
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        if( fileListTreeModel_->swapChilds(selected, true) ) {
            selectRow(selected.row()-1, selected.parent());
        }
    }
}

void FileListWindow::downSlot() {
    if( ui->treeView->selectionModel() != 0 && !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ) {
        QModelIndex selected = ui->treeView->selectionModel()->selectedIndexes().at(0);
        if( fileListTreeModel_->swapChilds(selected, false) ) {
            selectRow(selected.row()+1, selected.parent());
        }
    }
}
