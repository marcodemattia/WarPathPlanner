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

#include "sharedmodel.h"
#include <QStringList>



AssociatedUnitsModel::AssociatedUnitsModel(SharedModel * model)
{
    sharedModel_ = model;
}

int AssociatedUnitsModel::rowCount(const QModelIndex &parent) const
{
    emit setSharedUnitsRowSignal();
    int a = sharedUnitsRow_;
    if( sharedUnitsRow_ != -1 ) {
        int size = sharedModel_->data(sharedUnitsRow_)->getAssociatedUnitsSize();
        return sharedModel_->data(sharedUnitsRow_)->getAssociatedUnitsSize();
    }
    return 0;
}

QVariant AssociatedUnitsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    emit setSharedUnitsRowSignal();
    if( sharedUnitsRow_ != -1 ) {
        QString item = sharedModel_->data(sharedUnitsRow_)->associatedUnit(index.row());
        return sharedModel_->data(sharedUnitsRow_)->associatedUnit(index.row());
    }
    return QVariant();
}

QVariant AssociatedUnitsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return "Associated Units";
}

bool AssociatedUnitsModel::setData(const QVariant &value)
{
    // emit layoutAboutToBeChanged();
    emit setSharedUnitsRowSignal();
    if( sharedUnitsRow_ != -1 ) {
        return sharedModel_->setAssociatedUnit(sharedUnitsRow_, value);
    }
    return false;
    // emit layoutChanged();
}

void AssociatedUnitsModel::removeRows(int row, const QModelIndex &parent)
{
    if( sharedUnitsRow_ != -1 ) {
        beginRemoveRows(parent, row, row);
        emit setSharedUnitsRowSignal();
        sharedModel_->removeAssociatedUnit(sharedUnitsRow_, row);
        endRemoveRows();
    }
}

void AssociatedUnitsModel::setSharedUnitsRow( const int row )
{
    // emit layoutAboutToBeChanged();
    sharedUnitsRow_ = row;
    // emit layoutChanged();
}







SharedModel::SharedModel(const QString & data, QObject *parent) :
    QAbstractTableModel(parent)
{
    header << "Option" << "Size" << "Cost" << "Type" << "Special";

    QStringList lines(data.split(QString("\n")));
    int number = 0;
    while( number < lines.count() ) {
        if( lines[number].startsWith("@") ) {
            QList<QVariant> sharedData;
            QStringList tokens(lines[number].split(":"));

            QString associatedUnits(tokens[0]);
            QStringList associatedUnitsList(associatedUnits.split("@"));
            associatedUnitsList.removeAt(0);
            for( int i=0; i<associatedUnitsList.size(); ++i ) {
                associatedUnitsList[i] = associatedUnitsList.at(i).trimmed();
            }

            QStringList option(tokens[1].split(";"));
            for( int i=0; i<option.size(); ++i ) {
                sharedData << option[i].trimmed();
            }
            if( sharedData.size() < 5 ) {
                for( int i=sharedData.size(); i<5; ++i ) {
                    sharedData << "";
                }
            }
            items.push_back(SharedItem(sharedData, associatedUnitsList));
        }
        ++number;
    }

}

int SharedModel::rowCount(const QModelIndex &parent) const
{
    return items.size();
}

int SharedModel::columnCount(const QModelIndex &parent) const
{
    if( items.isEmpty() ) return 0;
    // return items.at(0).getSize();
    return header.size();
}

QVariant SharedModel::data( const QModelIndex & index, int role ) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    return items.at(index.row()).data(index.column());
}

QVariant SharedModel::data( const int row, const int column ) const
{
    if( row >= rowCount() || column >= columnCount() ) return 0;
    return items.at(row).data(column);
}

const QList<QVariant> * SharedModel::fullData( const int row ) const
{
    if( row >= rowCount() ) return 0;
    return items.at(row).fullData();
}

const SharedItem * SharedModel::data( const int row ) const
{
    if( row >= rowCount() ) return 0;
    return &(items.at(row));
}

bool SharedModel::setData(const QModelIndex & selected, const QVariant & value, int column)
{
    emit layoutAboutToBeChanged();
    items[selected.row()].setData(column, value);
    emit layoutChanged();
    return true;
}

bool SharedModel::setAssociatedUnit(const int row, const QVariant & value)
{
    return items[row].setAssociatedUnit(value);
}

void SharedModel::removeAssociatedUnit(const int row, const int index)
{
    items[row].removeAssociatedUnit(index);
}

QVariant SharedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        // return items.at(0).data(section);
        return header.at(section);

    return QVariant();
}

void SharedModel::insertChild(const QList<QVariant> & columnData, const QModelIndex & selected, const bool)
{
    // Without the signals for the layout change the treeView won't update correctly.
    emit layoutAboutToBeChanged();
    // Always append after the selected row
    int row = selected.row()+1;
    beginInsertRows(selected, row, row);

    items.insert(items.begin()+row, SharedItem(columnData));

    endInsertRows();
    emit layoutChanged();
}

void SharedModel::removeChild(const QModelIndex & selected)
{
    emit layoutAboutToBeChanged();
    beginRemoveRows(selected, selected.row(), selected.row());

    items.removeAt(selected.row());

    endRemoveRows();
    emit layoutChanged();
}

bool SharedModel::swapChilds(const QModelIndex & selected, const bool up)
{
    bool swapped = false;
    emit layoutAboutToBeChanged();
    if( up ) {
        if( selected.row() != 0 ) {
            items.swap(selected.row(), selected.row()-1);
            swapped = true;
        }
    }
    else {
        if( selected.row() != (rowCount()-1) ) {
            items.swap(selected.row(), selected.row()+1);
            swapped = true;
        }
    }
    emit layoutChanged();
    return swapped;
}
