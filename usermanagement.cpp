/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "usermanagement.h"
#include "mainwidget.h"
#include "connection.h"
#include "userrecorddialog.h"
#include <limits>

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

UserManagement::UserManagement(MainWidget *MW, QWidget *parent) :
   QWidget(parent), mw(MW)
{
    unameLabel = new QLabel("Username Filter: ");
    unameLE = new QLineEdit;
    unameLabel->setBuddy(unameLE);

    countLabel = new QLabel("Count: ");
    countLE = new QLineEdit;
    countLE->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    countLE->setText("10");
    countLabel->setBuddy(countLE);

    filterLabel = new QLabel("Filters: ");
    enabledCheckBox = new QCheckBox("Enabled");
    enabledCheckBox->setTristate(true);
    enabledCheckBox->setCheckState(Qt::PartiallyChecked);
    filterLabel->setBuddy(enabledCheckBox);

    sortLabel = new QLabel("Sort By: ");
    sortCombo = new QComboBox;
    sortCombo->addItem("ID");
    sortCombo->addItem("Name");
    sortCombo->addItem("Score");
    sortCombo->addItem("#Corrects");
    sortCombo->addItem("#Submits");
    sortLabel->setBuddy(sortCombo);

    sortTypeCombo = new QComboBox;
    sortTypeCombo->addItem("Ascending");
    sortTypeCombo->addItem("Descending");

    connect(sortCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSortType()));

    requestButton = new QPushButton("Re&quest");
    requestButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(requestButton, SIGNAL(clicked()), this, SLOT(request()));

    tw = new QTableWidget;
    tw->setColumnCount(6);
    tw->setHorizontalHeaderItem(0, new QTableWidgetItem("Username"));
    tw->setHorizontalHeaderItem(1, new QTableWidgetItem("Description"));
    tw->setHorizontalHeaderItem(2, new QTableWidgetItem("Registered On"));
    tw->setHorizontalHeaderItem(3, new QTableWidgetItem("#Submits"));
    tw->setHorizontalHeaderItem(4, new QTableWidgetItem("#Corrects"));
    tw->setHorizontalHeaderItem(5, new QTableWidgetItem("Score"));
    tw->resizeColumnsToContents();

    tw->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tw, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(handleMenu(QPoint)));
    connect(tw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(handleDblClick(int,int)));

    contextMenu = new QMenu(this);
    resetPasswdAction = contextMenu->addAction("Reset Pass&word");
    changeAdminAction = contextMenu->addAction("");
    deleteAction = contextMenu->addAction("&Delete");
    changeActivationAction = contextMenu->addAction("");
    editDescAction = contextMenu->addAction("&Edit Description");
    renameAction = contextMenu->addAction("&Rename User");
    recordAction = contextMenu->addAction("User Rec&ord");

    layT1 = new QHBoxLayout;
    layT1->addWidget(unameLabel);
    layT1->addWidget(unameLE);
    layT1->addStretch();
    layT1->addWidget(countLabel);
    layT1->addWidget(countLE);

    layT2 = new QHBoxLayout;
    layT2->addWidget(filterLabel);
    layT2->addWidget(enabledCheckBox);
    layT2->addStretch();
    layT2->addWidget(sortLabel);
    layT2->addWidget(sortCombo);
    layT2->addWidget(sortTypeCombo);

    layT12 = new QVBoxLayout;
    layT12->addLayout(layT1);
    layT12->addLayout(layT2);

    layUp = new QHBoxLayout;
    layUp->addStretch();
    layUp->addLayout(layT12);
    layUp->addWidget(requestButton);
    layUp->addStretch();

    layout = new QVBoxLayout;
    layout->addLayout(layUp);
    layout->addWidget(tw);

    setLayout(layout);
    handleSortType();
}

void UserManagement::init()
{
    unameLE->setText("");
    countLE->setText("10");
    enabledCheckBox->setCheckState(Qt::PartiallyChecked);
    sortCombo->setCurrentIndex(0);
    data.clear();
    updateData();
}

void UserManagement::onSwitch()
{

}

void UserManagement::updateData()
{
    tw->setRowCount(0);
    tw->setRowCount(data.size());

    for (int i = 0; i < data.size(); i++)
    {
        QTableWidgetItem *col[tw->columnCount()];
        for (int j = 0; j < tw->columnCount(); j++)
        {
            col[j] = new QTableWidgetItem;
            col[j]->setFlags(Qt::ItemIsEnabled);
        }

        QColor backgroundColor = Qt::white;

        QString name = data[i].name();
        if (data[i].isAdmin())
        {
            name += "(A)";
            backgroundColor = Qt::blue;
        }
        if (!data[i].enabled())
        {
            name += "(D)";
            backgroundColor = Qt::gray;
        }
        col[0]->setText(name);

        QString desc = data[i].description();
        if (desc.length() > 20)
            desc = desc.mid(0, 20) + "...";
        col[1]->setText(desc);
        col[2]->setText(data[i].joinDate().toString(DATE_TIME_FORMAT));
        col[3]->setText(QString::number(data[i].numSubmits()));
        col[4]->setText(QString::number(data[i].numCorrects()));
        col[5]->setText(QString::number(data[i].score(), 'f', 3));

        for (int j = 0; j < tw->columnCount(); j++)
        {
            col[j]->setBackgroundColor(backgroundColor);
            tw->setItem(i, j, col[j]);
        }
    }
    tw->resizeColumnsToContents();
}

void UserManagement::handleSortType()
{
    if (sortCombo->currentText() == "Name")
        sortTypeCombo->setCurrentIndex(0);
    else
        sortTypeCombo->setCurrentIndex(1);
}

void UserManagement::request()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(requestDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::UserQuery);
        DataTable DT;

        if (!unameLE->text().isEmpty())
            DT["username"] = unameLE->text();

        if (!countLE->text().isEmpty())
            DT["count"] = countLE->text();

        if (enabledCheckBox->checkState() != Qt::PartiallyChecked)
            DT["enabled"] = enabledCheckBox->checkState() == Qt::Checked;

        DT["order"] = sortCombo->currentText().toLower();
        DT["order_type"] = sortTypeCombo->currentIndex() ? "desc" : "asc";

        (*DS) << DT;
        mw->connection()->send();
    }
}

void UserManagement::requestDone(StatCode stat, const QByteArray &BA)
{
    QDataStream DS(BA);
    DS.setVersion(QDataStream::Qt_4_8);

    if (!mw->handleGeneralStatCodes(stat))
    {
        data.clear();
        DS >> data;
        updateData();
    }
}

void UserManagement::operationDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::NotTheOnlyAdmin)
            QMessageBox::warning(this, "Error", "Cannot remove the only admin.");
        else if (stat == StatCode::AlreadyExists)
            QMessageBox::warning(this, "Error", "Another user with the same name exists.");
        else if (stat == StatCode::OperationSuccessful)
        {
            QMessageBox::information(this, "Done", "Operation was successful.");

            if (data[lastRow].name() == mw->currentUsername())
            {
                if (newAuth.contains("user"))
                    mw->setAuth(newAuth["user"].toString(), mw->currentPassword());
                else if (newAuth.contains("pass"))
                    mw->setAuth(mw->currentUsername(), newAuth["pass"].toByteArray());
            }

            if (newUsr == Q_NULLPTR)
                data.removeAt(lastRow);
            else
                data[lastRow] = *newUsr;

            updateData();
        }
    }

    newAuth.clear();
    delete newUsr;
    newUsr = Q_NULLPTR;
}

void UserManagement::handleMenu(const QPoint &pos)
{
    QPoint globalPos = tw->mapToGlobal(pos);
    QTableWidgetItem *curItem = tw->itemAt(pos);

    if (curItem)
    {
        int row = tw->row(curItem);

        changeAdminAction->setText(data[row].isAdmin() ? "&Unadmin" : "&Make Admin");
        changeActivationAction->setText(data[row].enabled() ? "Di&sable" : "&Enable");

        QAction *action = contextMenu->exec(globalPos);
        lastRow = row;

        if (action == resetPasswdAction)
            resetPassword(row);
        else if (action == changeAdminAction)
            changeAdmin(row);
        else if (action == deleteAction)
            deleteUser(row);
        else if (action == changeActivationAction)
            changeActivation(row);
        else if (action == editDescAction)
            editDescription(row);
        else if (action == renameAction)
            rename(row);
        else if (action == recordAction)
            userRecord(row);
    }
}

void UserManagement::handleDblClick(int i, int j)
{
    if (j == 1)
        QMessageBox::information(this, "Description", data[i].description());
}

void UserManagement::resetPassword(int idx)
{
    bool ok;
    QString newPass = QInputDialog::getText(this, "Reset Password",
                                            QString("Enter new password for %1: ").arg(data[idx].name()), QLineEdit::Password,
                                            "", &ok);

    QByteArray pss = MainWidget::hashPassword(newPass.toUtf8());

    if (ok)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
        {
            newUsr = new User(data[idx]);
            (*DS) << static_cast<int>(StatCode::ChangePasswordRequest) << data[idx].name()
                  << pss;
            newAuth["pass"] = pss;
            mw->connection()->send();
        }
    }
}

void UserManagement::changeAdmin(int idx)
{
    QString msg = QString("Are you sure to change admin state of %1?").arg(data[idx].name());
    if (QMessageBox::warning(this, "Confirm", msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
        {
            newUsr = new User(data[idx]);
            newUsr->changeAdmin();
            (*DS) << static_cast<int>(StatCode::UserChangeAdmin) << data[idx].name()
                  << !data[idx].isAdmin();
            mw->connection()->send();
        }
    }
}

void UserManagement::deleteUser(int idx)
{
    QString msg = QString("Are you sure to delete user %1?").arg(data[idx].name());
    if (QMessageBox::warning(this, "Confirm", msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
        {
            (*DS) << static_cast<int>(StatCode::UserDelete) << data[idx].name();
            mw->connection()->send();
        }
    }
}

void UserManagement::changeActivation(int idx)
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
    {
        newUsr = new User(data[idx]);
        newUsr->changeActivation();
        (*DS) << static_cast<int>(StatCode::UserChangeActivation) << data[idx].name()
              << !data[idx].enabled();
        mw->connection()->send();
    }
}

void UserManagement::editDescription(int idx)
{
    bool ok;
    QString newDesc = QInputDialog::getText(this, "Edit Description",
                                            QString("Enter new description for %1: ").arg(data[idx].name()),
                                            QLineEdit::Normal, data[idx].description(), &ok);

    if (ok)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
        {
            newUsr = new User(data[idx]);
            newUsr->changeDesc(newDesc);
            (*DS) << static_cast<int>(StatCode::UserChangeDescription) << data[idx].name()
                  << newDesc;
            mw->connection()->send();
        }
    }
}

void UserManagement::rename(int idx)
{
    bool ok;
    QString newName = QInputDialog::getText(this, "Rename User",
                                            QString("Enter new username for %1: ").arg(data[idx].name()),
                                            QLineEdit::Normal, data[idx].name(), &ok);

    if (ok)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
        {
            newUsr = new User(data[idx]);
            newUsr->changeName(newName);
            (*DS) << static_cast<int>(StatCode::RenameUser) << data[idx].name()
                  << newName;
            newAuth["user"] = newName;
            mw->connection()->send();
        }
    }
}

void UserManagement::userRecord(int idx)
{
    UserRecordDialog *urd = new UserRecordDialog(mw, data[idx].name(), this);
    urd->setAttribute(Qt::WA_DeleteOnClose);
    urd->show();
    urd->raise();
    urd->activateWindow();
}
