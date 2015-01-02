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

/**
 * Provides a simple tree model to show how to create and use hierarchical
 * models.
 */

#include <QtGui>

#include "treeitem.h"
#include "treemodel.h"

TreeModel::TreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Unit" << "Size" << "Cost" << "Type" << "Special";
    rootItem = new TreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
//    if (!index.isValid())
//        return QVariant();

//    if (role != Qt::DisplayRole)
//        return QVariant();

//    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    TreeItem * item = pointer(index, role);
    if( item == 0 ) return QVariant();

    return item->data(index.column());
}

const QList<QVariant> * TreeModel::fullData(const QModelIndex &index, int role) const
{
    TreeItem * item = pointer(index, role);
    if( item == 0 ) return 0;

    return item->fullData();
}

TreeItem * TreeModel::pointer(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return 0;

    if (role != Qt::DisplayRole)
        return 0;

    return static_cast<TreeItem*>(index.internalPointer());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;
    while( number < lines.count() ) {
        if( !lines[number].startsWith("#") && !lines[number].startsWith("@") ) {
            int position = 0;
            if( lines[number].startsWith("--") ) {
                position = 2;
            }
            else if( lines[number].startsWith("-") ) {
                position = 1;
            }

            QString lineData = lines[number].mid(position).trimmed();

            if (!lineData.isEmpty()) {
                // Read the column data from the rest of the line.
                QStringList columnStrings = lineData.split(";", QString::SkipEmptyParts);
                QList<QVariant> columnData;
                for (int column = 0; column < columnStrings.count(); ++column)
                    columnData << columnStrings[column].trimmed();

                if( columnData.size() < 5 ) {
                    for( int i=columnData.size(); i<5; ++i ) {
                        columnData << "";
                    }
                }
                if (position > indentations.last()) {
                    // The last child of the current parent is now the new parent
                    // unless the current parent has no children.

                    if (parents.last()->childCount() > 0) {
                        parents << parents.last()->child(parents.last()->childCount()-1);
                        indentations << position;
                    }
                } else {
                    while (position < indentations.last() && parents.count() > 0) {
                        parents.pop_back();
                        indentations.pop_back();
                    }
                }

                // Append a new item to the current parent's list of children.
                // parents.last()->appendChild(new TreeItem(columnData, parents.last()));
                insertModelData(columnData, &parents);
            }
        }
        number++;
    }
}

void TreeModel::insertModelData(const QList<QVariant> &columnData, QList<TreeItem*> * parents)
{
    parents->last()->appendChild(new TreeItem(columnData, parents->last()));
}

void TreeModel::insertChild(const QList<QVariant> & columnData, const QModelIndex & selected, const bool unit)
{
    // Without the signals for the layout change the treeView won't update correctly.
    emit layoutAboutToBeChanged();
    // Always append after the selected row
    int row = selected.row()+1;
    beginInsertRows(selected, row, row);
    TreeItem * parent = 0;

    // More lines, but at least it is not checking again for parent == 0 when unit is false
    if( unit ) {
        if( pointer(selected) == 0 ) parent = rootItem;
        else {
            parent = pointer(selected)->parent();
            if( parent == 0 ) {
                parent = pointer(selected);
            }
        }
    }
    else {
        parent = pointer(selected);
        row = parent->childCount();
    }

    // This should happen only when appending without any selection
    parent->insertChild(new TreeItem(columnData, parent), row);
    endInsertRows();
    emit layoutChanged();
}

void TreeModel::removeChild(const QModelIndex & selected)
{
    // In this case it works also without the layoutChange signals...
    // emit layoutAboutToBeChanged();
    beginRemoveRows(selected, selected.row(), selected.row());

    TreeItem * parent = pointer(selected)->parent();
    parent->removeChild(selected.row());

    endRemoveRows();
    // emit layoutChanged();
}

bool TreeModel::swapChilds(const QModelIndex & selected, const bool up)
{
    bool swapped = false;
    emit layoutAboutToBeChanged();
    if( up ) {
        if( selected.row() != 0 ) {
            pointer(selected)->parent()->swapChilds(selected.row(), selected.row()-1);
            swapped = true;
        }
    }
    else {
        int totalRows = 0;
        if( !selected.parent().isValid() ) {
            totalRows = rootItem->childCount();
        }
        else {
            totalRows = pointer(selected.parent())->childCount();
        }
        if( selected.row() != (totalRows-1) ) {
            pointer(selected)->parent()->swapChilds(selected.row(), selected.row()+1);
            swapped = true;
        }
    }
    emit layoutChanged();
    return swapped;
}

bool TreeModel::setData(const QModelIndex & selected, const QVariant & value, int column)
{
    emit layoutAboutToBeChanged();
    pointer(selected)->setData(column, value);
    emit layoutChanged();
    return true;
}
