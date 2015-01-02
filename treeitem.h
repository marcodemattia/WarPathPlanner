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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TreeItem
{
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);
    void insertChild(TreeItem *child, const int position);
    void removeChild(const int position);
    void swapChilds(const int position1, const int position2);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    const QList<QVariant> * fullData() const;
    void setData(int column, const QVariant & value);
    int row() const;
    TreeItem *parent();
    void setParent(TreeItem * parent);

private:
    QList<TreeItem*> childItems;
    QList<QVariant> itemData;
    TreeItem *parentItem;
};

#endif // TREEITEM_H
