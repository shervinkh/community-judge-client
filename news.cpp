/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "news.h"
#include "mainwidget.h"
#include "connection.h"
#include "newsdialog.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

News::News(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    reqButton = new QPushButton("Re&quest");
    reqButton->setMinimumSize(reqButton->sizeHint() * 1.3);
    addButton = new QPushButton("&Add");
    addButton->setMinimumSize(addButton->sizeHint() * 1.3);
    editButton = new QPushButton("&Edit");
    editButton->setMinimumSize(editButton->sizeHint() * 1.3);
    removeButton = new QPushButton("&Remove");
    removeButton->setMinimumSize(removeButton->sizeHint() * 1.3);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(reqButton);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();

    lastOp = Operation::None;

    lw = new QListWidget;

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(buttonLayout);
    totalLayout->addWidget(lw);
    setLayout(totalLayout);

    connect(reqButton, SIGNAL(clicked()), this, SLOT(request()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(add()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(remove()));
}

void News::init()
{
    addButton->setVisible(mw->isAdmin());
    editButton->setVisible(mw->isAdmin());
    removeButton->setVisible(mw->isAdmin());

    data.clear();
    updateList();
}

void News::onSwitch()
{
    if (!mw->connection()->isBusy())
        request();
}

void News::updateList()
{
    QListWidgetItem *item;
    while (item = lw->takeItem(0))
        delete item;

    if (data.isEmpty())
    {
        QLabel *txt = new QLabel("<h1>No News</h1>");
        txt->setAlignment(Qt::AlignCenter);
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(txt->sizeHint());
        item->setFlags(Qt::ItemIsEnabled);
        lw->insertItem(0, item);
        lw->setItemWidget(item, txt);
    }
    else
    {
        for (int i = 0; i < data.size(); i++)
        {
            QString str = QString("<h2>%1</h2>%2<br/><br/>").arg(data[i]["title"].toString())
                                                            .arg(data[i]["content"].toString());
            QLabel *txt = new QLabel(str);
            txt->setOpenExternalLinks(true);
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(txt->sizeHint());
            lw->insertItem(i, item);
            lw->setItemWidget(item, txt);
        }
    }
}

void News::request()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::GetNews);
        lastOp = Operation::Request;
        mw->connection()->send();
    }
}

void News::operationDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        if (lastOp == Operation::Request)
            DS >> data;
        else if (lastOp == Operation::Add)
        {
            qint64 tmp;
            DS >> tmp;
            tmpChange["id"] = tmp;
            data.append(tmpChange);
        }
        else if (lastOp == Operation::Edit)
            data[lastRow] = tmpChange;
        else if (lastOp == Operation::Remove)
            data.removeAt(lastRow);

        lastOp = Operation::None;
        tmpChange = DataTable();
        lastRow = -1;

        updateList();
    }
}

void News::add()
{
    NewsDialog ND(DataTable(), this);
    if (ND.exec())
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
        {
            (*DS) << static_cast<int>(StatCode::EditNews);
            tmpChange = ND.giveData();
            tmpChange["id"] = -1;
            (*DS) << tmpChange;
            lastOp = Operation::Add;
            mw->connection()->send();
        }
    }
}

void News::edit()
{
    int curid = lw->currentRow();
    if (!data.isEmpty() && curid != -1)
    {
        NewsDialog ND(data[curid], this);
        if (ND.exec())
        {
            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
            {
                (*DS) << static_cast<int>(StatCode::EditNews);
                tmpChange = ND.giveData();
                (*DS) << tmpChange;
                lastOp = Operation::Edit;
                lastRow = curid;
                mw->connection()->send();
            }
        }
    }
}

void News::remove()
{
    int curid = lw->currentRow();
    if (!data.isEmpty() && curid != -1)
    {
        QString str = QString("Are you sure you want to remove %1?").arg(data[curid]["title"].toString());
        if (QMessageBox::warning(this, "Confirm", str, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
            {
                (*DS) << static_cast<int>(StatCode::RemoveNews);
                (*DS) << data[curid]["id"].toLongLong();
                lastOp = Operation::Remove;
                lastRow = curid;
                mw->connection()->send();
            }
        }
    }
}
