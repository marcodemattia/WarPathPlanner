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

#ifndef SHAREDMODEL_H
#define SHAREDMODEL_H

#include <QAbstractTableModel>
#include <shareditem.h>
#include <associatedunitsmodel.h>



#include <QAbstractListModel>

class SharedModel;

class AssociatedUnitsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    AssociatedUnitsModel(SharedModel * model);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    const QList<QVariant> * fullData() const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    // bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setData(const QVariant &value);
    // Qt::ItemFlags flags(const QModelIndex &index) const { return Qt::ItemIsSelectable; }
    // bool insertRows(int row, int count, const QModelIndex &parent);
    void removeRows(int row, const QModelIndex &parent = QModelIndex());
    void setSharedUnitsRow(const int row);

signals:
    void setSharedUnitsRowSignal() const;

private:
    SharedModel * sharedModel_;
    int sharedUnitsRow_;
};






class SharedModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SharedModel(const QString & data, QObject *parent = 0);
    int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant data( const int row, const int column ) const;
    const QList<QVariant> * fullData( const int row ) const;
    const SharedItem * data( const int row ) const;
    bool setData(const QModelIndex & selected, const QVariant & value, int column);
    bool setAssociatedUnit(const int row, const QVariant & value);
    void removeAssociatedUnit(const int row, const int index);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void insertChild(const QList<QVariant> & columnData, const QModelIndex & selected, const bool = true);
    void removeChild(const QModelIndex & selected);
    bool swapChilds(const QModelIndex & selected, const bool up);

signals:

public slots:

private:
    QList<QVariant> header;
    QList<SharedItem> items;
    // friend class AssociatedUnitsModel;
};

#endif // SHAREDMODEL_H
