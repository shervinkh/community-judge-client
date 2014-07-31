/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "loginwidget.h"
#include "registeration.h"
#include "mainwidget.h"
#include "addserverdialog.h"
#include "login.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

LoginWidget::LoginWidget(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    reg = new Registeration(mw);
    log = new Login(mw);

    logButton = new QPushButton("Logi&n");
    regButton = new QPushButton("Re&gister");
    aboutButton = new QPushButton("&About");
    exitButton = new QPushButton("E&xit");

    logButton->setMinimumHeight(logButton->sizeHint().height() * 1.5);
    regButton->setMinimumHeight(regButton->sizeHint().height() * 1.5);
    aboutButton->setMinimumHeight(aboutButton->sizeHint().height() * 1.5);
    exitButton->setMinimumHeight(exitButton->sizeHint().height() * 1.5);
    logButton->setMinimumWidth(logButton->sizeHint().width() * 1.3);
    regButton->setMinimumWidth(regButton->sizeHint().width() * 1.3);
    aboutButton->setMinimumWidth(aboutButton->sizeHint().width() * 1.3);
    exitButton->setMinimumWidth(exitButton->sizeHint().width() * 1.3);

    buttonsLayout = new QVBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(logButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(regButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(aboutButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(exitButton);
    buttonsLayout->addStretch();

    sw = new QStackedWidget;
    sw->insertWidget(0, log);
    sw->insertWidget(1, reg);

    serverLabel = new QLabel("Server: ");
    serverBox = new QComboBox;
    serverBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    for (QPair<QString, QUrl> item : mw->servers())
        serverBox->addItem(QString("%1 (%4 %2:%3)").arg(item.first).arg(item.second.host()).arg(item.second.port())
                           .arg((item.second.scheme() == "ssl") ? "SSL:" : ""));

    serverLabel->setBuddy(serverBox);
    addButton = new QPushButton("A&dd");
    removeButton = new QPushButton("&Remove");

    serverLayout = new QHBoxLayout;
    serverLayout->addStretch();
    serverLayout->addWidget(serverLabel);
    serverLayout->addWidget(serverBox);
    serverLayout->addWidget(addButton);
    serverLayout->addWidget(removeButton);
    serverLayout->addStretch();

    connect(addButton, SIGNAL(clicked()), this, SLOT(addServer()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeServer()));

    rightLayout = new QVBoxLayout;
    rightLayout->addLayout(serverLayout);
    rightLayout->addStretch();
    rightLayout->addWidget(sw);
    rightLayout->addStretch();

    totalLayout = new QHBoxLayout;
    totalLayout->addLayout(buttonsLayout);
    totalLayout->addLayout(rightLayout);

    setLayout(totalLayout);
    mw->setServer(serverBox->currentIndex());

    connect(logButton, SIGNAL(clicked()), this, SLOT(selectLog()));
    connect(regButton, SIGNAL(clicked()), this, SLOT(selectReg()));
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(selectAbout()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(selectExit()));
    connect(log, SIGNAL(loginOK()), this, SIGNAL(loginOK()));
}

void LoginWidget::updateServerSelection()
{
    mw->setServer(serverBox->currentIndex());
}

void LoginWidget::init()
{
    log->init();
    reg->init();

    for (int i = 0; i < mw->servers().size(); i++)
        if (mw->servers()[i].second == mw->rememberedHostAddress())
        {
            serverBox->setCurrentIndex(i);
            break;
        }
}

void LoginWidget::selectLog()
{
    sw->setCurrentIndex(0);
}

void LoginWidget::selectReg()
{
    sw->setCurrentIndex(1);
}

void LoginWidget::selectAbout()
{
    mw->showAboutDialog();
}

void LoginWidget::selectExit()
{
    qApp->exit(EXIT_SUCCESS);
}

void LoginWidget::addServer()
{
    AddServerDialog dialog(this);

    if (dialog.exec())
    {
        for (QPair<QString, QUrl> cur : mw->servers())
            if (cur.first == dialog.name())
            {
                QMessageBox::warning(this, "Error", "Another server with this name exists");
                return;
            }
            else if (cur.second == dialog.url())
            {
                QMessageBox::warning(this, "Error", "Another server with this address and port exists");
                return;
            }

        mw->servers().append(QPair<QString, QUrl>(dialog.name(), dialog.url()));
        serverBox->addItem(QString("%1 (%4 %2:%3)").arg(dialog.name()).arg(dialog.url().host()).arg(dialog.url().port())
                           .arg((dialog.url().scheme() == "ssl") ? "SSL:" : ""));
        serverBox->setCurrentIndex(mw->servers().length() - 1);
    }
}

void LoginWidget::removeServer()
{
    int curIdx = serverBox->currentIndex();
    if (curIdx != -1)
    {
        mw->servers().removeAt(curIdx);
        serverBox->removeItem(curIdx);
    }
}

