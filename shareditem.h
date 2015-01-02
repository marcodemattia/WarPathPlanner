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

#ifndef SHAREDITEM_H
#define SHAREDITEM_H

#include <QList>
#include <QStringList>
#include <QVariant>

class SharedItem
{
public:
    SharedItem(const QList<QVariant> & data, const QStringList & associatedUnitsList = QStringList());
    int getSize() const;
    int getAssociatedUnitsSize() const;
    QVariant data(const int column) const;
    const QList<QVariant> * fullData() const;
    QString associatedUnit(const int index) const;
    void setData(const int column, const QVariant & value);
    bool setAssociatedUnit(const QVariant & value);
    void removeAssociatedUnit(const int row);

private:
    QList<QVariant> items;
    QStringList associatedUnits;
};

#endif // SHAREDITEM_H
