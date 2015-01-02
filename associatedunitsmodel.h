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

//#ifndef ASSOCIATEDUNITSMODEL_H
//#define ASSOCIATEDUNITSMODEL_H

//#include "sharedmodel.h"
//#include <QAbstractListModel>

//class SharedModel;

//class AssociatedUnitsModel : public QAbstractListModel
//{
//    Q_OBJECT
//public:
//    AssociatedUnitsModel(SharedModel * model);
//    int rowCount(const QModelIndex &parent) const;
//    QVariant data(const QModelIndex &index, int role) const;
//    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
//    // bool setData(const QModelIndex &index, const QVariant &value, int role);
//    bool setData(const QVariant &value);
//    // Qt::ItemFlags flags(const QModelIndex &index) const { return Qt::ItemIsEditable; }
//    // bool insertRows(int row, int count, const QModelIndex &parent);
//    void removeRows(int row, const QModelIndex &parent = QModelIndex());
//    void setSharedUnitsRow(const int row );

//signals:
//    void setSharedUnitsRowSignal() const;

//private:
//    SharedModel * sharedModel_;
//    int sharedUnitsRow_;
//};

//#endif // ASSOCIATEDUNITSMODEL_H
