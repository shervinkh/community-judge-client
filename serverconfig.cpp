/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "serverconfig.h"
#include "mainwidget.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

void ServerConfig::initData()
{
    description["can_register"] = "Enables users to make registeration requests";
    type["can_register"] = "Boolean";

    description["can_submit"] = "Enables users to make submissions";
    type["can_submit"] = "Boolean";

    description["can_viewresult"] = "Enables users to view submissions' results";
    type["can_viewresult"] = "Boolean";

    description["can_viewresult_all_nonpublic"] = "Enables users to view non-public submissions' results from other people";
    type["can_viewresult_all_nonpublic"] = "Boolean";

    description["can_viewresult_all_public"] = "Enables users to view public submissions' results from other people";
    type["can_viewresult_all_public"] = "Boolean";

    description["registered_enabled"] = "Automatically enable users while registering";
    type["registered_enabled"] = "Boolean";

    description["max_subs_in_queue"] = "Maximum number of In-Queue submissions that a user can have";
    type["max_subs_in_queue"] = "Integer";

    description["max_packet_size"] = "Incoming connections with packets larger that this many bytes will be dropped";
    type["max_packet_size"] = "Integer";

    description["server_timeout"] = "Incoming connections will be dropped after this many seconds. Do not set too low that you can't connect.";
    type["server_timeout"] = "Integer";

    description["default_result_count"] = "Default entry count for view result requests";
    type["default_result_count"] = "Integer";

    description["score_system"] = "Enables score system";
    type["score_system"] = "Boolean";

    description["judge_enabled"] = "Enables judging";
    type["judge_enabled"] = "Boolean";

    description["judge_threads"] = "Maximum number of judging threads (0 = Auto set)";
    type["judge_threads"] = "Integer";

    description["max_library_size"] = "Maximum library size in KB. This will be added to memory limit "
                                      "to set the kill threshold for programs";
    type["max_library_size"] = "Integer";

    description["real_timer"] = "Use real time instead of cpu time for time measurements";
    type["real_timer"] = "Boolean";
}

ServerConfig::ServerConfig(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    caution = new QLabel("<b><font color=\"red\">Proceed with caution!</font> Changing some configs "
                         "to an inappropriate value might make your server unusable.</b>");
    caution->setWordWrap(true);

    getConfig = new QPushButton("&Get Configs");
    getConfig->setMinimumSize(getConfig->sizeHint() * 1.3);
    setConfig = new QPushButton("&Set Configs");
    setConfig->setMinimumSize(setConfig->sizeHint() * 1.3);
    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(getConfig);
    buttonLayout->addStretch();
    buttonLayout->addWidget(setConfig);
    buttonLayout->addStretch();

    QFont fnt;
    fnt.setPointSize(12);

    nameLabel = new QLabel("<b>Name: </b>");
    nameLabel->setFont(fnt);
    name = new QLabel;
    name->setWordWrap(true);

    descLabel = new QLabel("<b>Description: </b>");
    descLabel->setFont(fnt);
    desc = new QLabel;
    desc->setWordWrap(true);

    valueLabel = new QLabel;
    valueLabel->setFont(fnt);

    valueEdit = new QLineEdit;
    valueCheckBox = new QCheckBox("Enabled");

    connect(valueEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged()));
    connect(valueCheckBox, SIGNAL(stateChanged(int)), this, SLOT(checkBoxChanged()));

    configView = new QGridLayout;
    configView->addWidget(nameLabel, 0, 0);
    configView->addWidget(name, 0, 1);
    configView->addWidget(descLabel, 1, 0);
    configView->addWidget(desc, 1, 1);
    configView->addWidget(valueLabel, 2, 0);
    configView->addWidget(valueEdit, 2, 1);
    configView->addWidget(valueCheckBox, 2, 1);

    dataWidget = new QWidget;
    dataWidget->setLayout(configView);

    rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(dataWidget);
    rightLayout->addStretch();

    configList = new QListWidget;
    configList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    downLayout = new QHBoxLayout;
    downLayout->addWidget(configList);
    downLayout->addLayout(rightLayout);
    downLayout->addStretch();

    totalLayout = new QVBoxLayout;
    totalLayout->addWidget(caution);
    totalLayout->addLayout(buttonLayout);
    totalLayout->addLayout(downLayout);

    connect(getConfig, SIGNAL(clicked()), this, SLOT(getData()));
    connect(setConfig, SIGNAL(clicked()), this, SLOT(setData()));
    connect(configList, SIGNAL(currentTextChanged(QString)), this, SLOT(handleChange()));

    setLayout(totalLayout);

    handleChange();

    initData();
}

void ServerConfig::init()
{
    original.clear();
    now.clear();
    updateData();
}

void ServerConfig::onSwitch()
{
    if (!mw->connection()->isBusy())
        getData();
}

void ServerConfig::updateData()
{
    configList->clear();
    foreach (QString key, now.keys())
        configList->addItem(key);
}

void ServerConfig::getData()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(getDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::GetConfig);
        mw->connection()->send();
    }
}

void ServerConfig::setData()
{
    DataTable DT;

    foreach (QString key, original.keys())
        if (original[key] != now[key])
            DT[key] = now[key];

    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(setDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::SetConfig);
        (*DS) << DT;
        mw->connection()->send();
    }
}

void ServerConfig::getDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        DS >> original;
        now = original;
        updateData();
    }
}

void ServerConfig::setDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QMessageBox::information(this, "Done", "Operation was successful");
        original = now;
    }
}

void ServerConfig::handleChange()
{
    if (configList->currentRow() == -1)
        dataWidget->setVisible(false);
    else
    {
        dataWidget->setVisible(true);

        QString cur = configList->currentItem()->text();
        name->setText(cur);
        desc->setText(description[cur].toString());
        valueLabel->setText(QString("<b>Value (%1):").arg(type[cur].toString()));

        if (type[cur] == "Boolean")
        {
            valueEdit->setVisible(false);
            valueCheckBox->setVisible(true);
            valueCheckBox->setChecked(now[cur].toBool());
        }
        else
        {
            valueEdit->setVisible(true);
            valueCheckBox->setVisible(false);
            valueEdit->setText(now[cur].toString());

            if (type[cur] == "Integer")
                valueEdit->setValidator(new QIntValidator);
            else if (type[cur] == "Real")
                valueEdit->setValidator(new QDoubleValidator);
            else if (type[cur] == "String")
                valueEdit->setValidator(NULL);
        }
    }
}

void ServerConfig::checkBoxChanged()
{
    if (configList->currentRow() != -1)
        now[configList->currentItem()->text()] = valueCheckBox->isChecked();
}

void ServerConfig::lineEditChanged()
{
    int x = 0;
    QString val = valueEdit->text();
    if (configList->currentRow() != -1 && (!valueEdit->validator() ||
        valueEdit->validator()->validate(val, x) == QValidator::Acceptable))
        now[configList->currentItem()->text()] = valueEdit->text();
}
