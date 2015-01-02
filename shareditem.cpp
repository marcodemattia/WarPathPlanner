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

#include "shareditem.h"

SharedItem::SharedItem(const QList<QVariant> & data, const QStringList & associatedUnitsList)
{
    items = data;
    associatedUnits = associatedUnitsList;
}

int SharedItem::getSize() const
{
    return items.size();
}

int SharedItem::getAssociatedUnitsSize() const
{
    return associatedUnits.size();
}

QVariant SharedItem::data(const int column) const
{
    return items.at(column);
}

const QList<QVariant> * SharedItem::fullData() const
{
    return &items;
}

QString SharedItem::associatedUnit(const int index) const
{
    return associatedUnits.at(index);
}

void SharedItem::setData(const int column, const QVariant & value)
{
    items[column] = value;
}

bool SharedItem::setAssociatedUnit(const QVariant & value)
{
    if( !associatedUnits.contains(value.toString()) ) {
        associatedUnits.append(value.toString());
        return true;
    }
    return false;
}

void SharedItem::removeAssociatedUnit(const int row)
{
    associatedUnits.removeAt(row);
}
