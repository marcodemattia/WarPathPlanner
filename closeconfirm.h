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

#ifndef CLOSECONFIRM_H
#define CLOSECONFIRM_H

#include <QDialog>

namespace Ui {
    class CloseConfirm;
}

class CloseConfirm : public QDialog
{
    Q_OBJECT

public:
    explicit CloseConfirm(QWidget *parent = 0);
    ~CloseConfirm();

public slots:
    void yesSlot();
    void noSlot();

signals:
    void saveChanges(const bool save);

private:
    Ui::CloseConfirm *ui;
};

#endif // CLOSECONFIRM_H
