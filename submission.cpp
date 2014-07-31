/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "submission.h"
#include "mainwidget.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

Submission::Submission(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    title = new QLabel("<b>Submit</b>");

    QFont fnt;
    fnt.setPointSize(18);
    title->setFont(fnt);

    problemLabel = new QLabel("Problem: ");
    problemEdit = new QLineEdit;
    problemLabel->setBuddy(problemEdit);

    fileLabel = new QLabel("File: ");
    fileEdit = new QLineEdit;
    fileLabel->setBuddy(fileEdit);
    fileOpen = new QPushButton(QIcon(":/Images/open.png"), "");

    upLayout = new QGridLayout;
    upLayout->addWidget(problemLabel, 0, 0);
    upLayout->addWidget(problemEdit, 0, 1);
    upLayout->addWidget(fileLabel, 1, 0);
    upLayout->addWidget(fileEdit, 1, 1);
    upLayout->addWidget(fileOpen, 1, 2);

    titleLayout = new QHBoxLayout;
    titleLayout->addStretch();
    titleLayout->addWidget(title);
    titleLayout->addStretch();

    inputLayout = new QHBoxLayout;
    inputLayout->addStretch();
    inputLayout->addLayout(upLayout);
    inputLayout->addStretch();

    submit = new QPushButton("S&ubmit");
    submit->setMinimumSize(submit->sizeHint() * 1.3);
    submit->setEnabled(false);
    submit->setDefault(true);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(submit);
    buttonLayout->addStretch();


    totalLayout = new QVBoxLayout;
    totalLayout->addStretch();
    totalLayout->addLayout(titleLayout);
    totalLayout->addLayout(inputLayout);
    totalLayout->addLayout(buttonLayout);
    totalLayout->addStretch();

    setLayout(totalLayout);

    connect(problemEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(fileEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(submit, SIGNAL(clicked()), this, SLOT(submitAction()));
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(handleOpen()));
}

void Submission::init()
{
    problemEdit->setText("");
    fileEdit->setText("");
}

void Submission::onSwitch()
{

}

void Submission::validateForm()
{
    submit->setEnabled(!problemEdit->text().isEmpty() && !fileEdit->text().isEmpty());
}

void Submission::submitAction()
{
    QFile codeFile(fileEdit->text());

    if (!codeFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", QString("Error openning file: %1").arg(codeFile.errorString()));
        return;
    }

    QTextStream TS(&codeFile);
    QByteArray code = TS.readAll().toLocal8Bit();

    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(submitDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::SubmissionRequest);
        (*DS) << problemEdit->text() << code;
        mw->connection()->send();
    }
}

void Submission::submitDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::NoSuchProblem)
            QMessageBox::warning(this, "Error", "Invalid problem name");
        else if (stat == StatCode::SlowDown)
            QMessageBox::warning(this, "Error", "Slow Down");
        else if (stat == StatCode::OperationSuccessful)
            QMessageBox::information(this, "Submission", "Submission successful");
    }
}

void Submission::handleOpen()
{
    QString path = QFileDialog::getOpenFileName(this, "Open C++ Code", QString(), "C++ Code (*.cpp)");

    if (!path.isEmpty())
        fileEdit->setText(path);
}
