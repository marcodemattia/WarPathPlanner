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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextStream>
#include <QItemSelectionModel>
#include <QAbstractItemView>
#include "treemodel.h"
#include "sharedmodel.h"
#include "associatedunitsmodel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void writeItem(QTextStream & out, TreeItem * item, const QString & prepend) const;
    void writeSharedItems(QTextStream & out) const;
    template<class T> void writeLine(QTextStream & out, T * item, const QString & prepend) const;
    bool getOutFileName(QString & fileName, const QString & extension);

public slots:
    void addButtonSlot();
    void addOptionButtonSlot();
    void removeButtonSlot();
    void upButtonSlot();
    void downButtonSlot();

    void addButtonSharedSlot();
    void removeButtonSharedSlot();
    void upButtonSharedSlot();
    void downButtonSharedSlot();

    void nameEditSlot(const QString & name);
    void typeEditSlot(const QString & type);
    void specialEditSlot(const QString & special);
    void treeViewClickedSlot(const QModelIndex & selected) const;
    void sharedTableClickedSlot(const QModelIndex & selected) const;

    void listViewClickedSlot(const QModelIndex & selected) const;

    void minValueChangedSlot(const int value);
    // void maxValueChangedSlot(const int value);
    void maxValueChangedSlot();
    void costValueChangedSlot(const int value);

    void setSelectedSharedOptionSlot();
    void setAssociatedUnitSlot();
    void removeAssociatedUnitSlot();

    void xModelCheckBox(const bool checked);

    void save();
    void load();
    void loadFile(const QString & fileName);
    void newDataFile();
    void fileListSlot();

signals:
    void lineSelectedNameSignal(const QString & name) const;
    void lineSelectedTypeSignal(const QString & type) const;
    void lineSelectedSpecialSignal(const QString & special) const;
    void minChangedSignal(const int min) const;
    void maxChangedSignal(const int max) const;
    void costChangedSignal(const int cost) const;
    void xModelCheck(const bool) const;

private:
    void selectRootNode(QModelIndex & selected) const;
//    void selectTreeRow(const int row, const QModelIndex & parent = QModelIndex()) const;
//    void selectSharedRow(const int row, const QModelIndex & parent = QModelIndex()) const;
    void selectRow(const int row, const QModelIndex & parent = QModelIndex()) const;
    void keyPressEvent(QKeyEvent * event);
    void setData(const QVariant & value, const int column);
    void setMinMax(const int min, const int max, const bool setMin);
    void emitMinMax(const QString & numString) const;
    void setAllModels();
    void emitClickedSlotSignals(const QList<QVariant> * data) const;
    void drawBackgroundImage(const QString & imageName);

    Ui::MainWindow *ui;
    TreeModel * model_;
    SharedModel * sharedModel_;
    mutable QAbstractItemView * activeView_;
    AssociatedUnitsModel * associatedUnitsModel_;
};

#endif // MAINWINDOW_H
