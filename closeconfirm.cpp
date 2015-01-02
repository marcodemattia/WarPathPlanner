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

#include "closeconfirm.h"
#include "ui_closeconfirm.h"

CloseConfirm::CloseConfirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloseConfirm)
{
    ui->setupUi(this);

    connect(ui->pushButton_Yes, SIGNAL(released()), this, SLOT(yesSlot()));
    connect(ui->pushButton_No, SIGNAL(released()), this, SLOT(noSlot()));
}

CloseConfirm::~CloseConfirm()
{
    delete ui;
}

void CloseConfirm::yesSlot()
{
    emit saveChanges(true);
    accept();
}

void CloseConfirm::noSlot()
{
    emit saveChanges(false);
    reject();
}
