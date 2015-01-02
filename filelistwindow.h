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

#ifndef FILELIST_H
#define FILELIST_H

#include <QDialog>
#include <QTextStream>
#include "filelisttreemodel.h"
#include "mainwindow.h"
#include "closeconfirm.h"

namespace Ui {
    class FileListWindow;
}

class FileListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileListWindow(MainWindow *parent = 0);
    ~FileListWindow();

public slots:
    void addGroupSlot();
    void addDataFileSlot();
    void addBackgroundImageSlot();
    void removeSlot();
    void editSlot();
    void saveSlot();
    void saveSlot(const bool save);
    void closeSlot();
    void upSlot();
    void downSlot();

signals:
    void editFile(const QString & fileName);

private:
    void selectRow(const int row, const QModelIndex & parent = QModelIndex()) const;
    void selectRootNode(QModelIndex & selected) const;
    void writeItem(QTextStream & out, TreeItem * item, const QString & prepend) const;
    bool getOutFileName(QString & fileName, const QString & extension);
    template <class T> void writeLine(QTextStream & out, T * item, const QString & prepend) const;

    Ui::FileListWindow *ui;
    FileListTreeModel * fileListTreeModel_;
    bool modified_;
    CloseConfirm * closeConfirm_;
};

#endif // FILELIST_H
