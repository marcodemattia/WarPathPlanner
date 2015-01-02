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

#include "associatedunitsmodel.h"

//AssociatedUnitsModel::AssociatedUnitsModel(SharedModel * model)
//{
//    sharedModel_ = model;
//}

//int AssociatedUnitsModel::rowCount(const QModelIndex &parent) const
//{
//    emit setSharedUnitsRowSignal();
//    return sharedModel_->data(sharedUnitsRow_)->getAssociatedUnitsSize();
//}

//QVariant AssociatedUnitsModel::data(const QModelIndex &index, int role) const
//{
//    emit setSharedUnitsRowSignal();
//    return sharedModel_->data(sharedUnitsRow_)->associatedUnit(index.row());
//}

//QVariant AssociatedUnitsModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    return "Associated Units";
//}

//bool AssociatedUnitsModel::setData(const QVariant &value)
//{
//    emit setSharedUnitsRowSignal();
//    return sharedModel_->setAssociatedUnit(sharedUnitsRow_, value);
//}

//void AssociatedUnitsModel::removeRows(int row, const QModelIndex &parent)
//{
//    beginRemoveRows(parent, row, row);
//    emit setSharedUnitsRowSignal();
//    sharedModel_->removeAssociatedUnit(sharedUnitsRow_, row);
//    endRemoveRows();
//}

//void AssociatedUnitsModel::setSharedUnitsRow( const int row )
//{
//    sharedUnitsRow_ = row;
//}
