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

#include "filelisttreemodel.h"

FileListTreeModel::FileListTreeModel(const QString & data)
{
    QList<QVariant> rootData;
    rootData << "DataFile" << "Background Image" << "Transparency [0-255]";
    rootItem = new TreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

void FileListTreeModel::insertModelData(const QList<QVariant> &columnData, QList<TreeItem*> *parents)
{
    if(columnData.at(0) == "DefaultBackgroundImage: images/WarPathDefaultBackground.jpg") return;
    QList<QVariant> rearrangedData;
    rearrangedData << columnData.at(0) << "" << columnData.at(2) << columnData.at(1) << "";
    parents->last()->appendChild(new TreeItem(rearrangedData, parents->last()));
}

QVariant FileListTreeModel::data(const QModelIndex &index, int role) const
{
    TreeItem * item = pointer(index, role);
    if( item == 0 ) return QVariant();
    if( index.column() == 1 ) return item->data(3);
    return item->data(index.column());
}

Qt::ItemFlags FileListTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if( index.column() == 2 ) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
