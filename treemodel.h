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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include "treeitem.h"

//class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel(QObject *parent = 0) : QAbstractItemModel(parent) {}
    TreeModel(const QString &data, QObject *parent = 0);
    virtual ~TreeModel();

    virtual QVariant data(const QModelIndex &index, int role) const;
    const QList<QVariant> * fullData(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & selected, const QVariant & value, int column);
    TreeItem * pointer(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void insertChild(const QList<QVariant> & columnData, const QModelIndex & selected, const bool = true);
    void removeChild(const QModelIndex & selected);
    bool swapChilds(const QModelIndex & selected, const bool up);

    // Made public so that it can be accessed and additional elements can be inserted
    TreeItem *rootItem;

protected:
    void setupModelData(const QStringList &lines, TreeItem *parent);
    virtual void insertModelData(const QList<QVariant> &columnData, QList<TreeItem*> * parents);
};

#endif // TREEMODEL_H
