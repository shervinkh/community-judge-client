/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "problemspecsdialog.h"
#include <limits>

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ProblemSpecsDialog::ProblemSpecsDialog(const Problem &prob, const QList<QString> &scps,
                                       const QList<QString> &cons, QWidget *parent) :
    QDialog(parent), cur(prob)
{
    nameLabel = new QLabel("Name: ");
    nameEdit = new QLineEdit;
    nameLabel->setBuddy(nameEdit);
    nameEdit->setText(prob.name());
    connect(nameEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));

    folderLabel = new QLabel("Folder: ");
    folderEdit = new QLineEdit;
    folderLabel->setBuddy(folderEdit);
    folderEdit->setText(prob.folder());
    connect(folderEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));

    descLabel = new QLabel("Description: ");
    descEdit = new QLineEdit;
    descLabel->setBuddy(descEdit);
    descEdit->setText(prob.description());

    descFileLabel = new QLabel("Description File: ");
    descFileEdit = new QLineEdit;
    descFileLabel->setBuddy(descFileEdit);
    descFileEdit->setText(prob.descriptionFile());

    ntLabel = new QLabel("Num. Tests: ");
    ntEdit = new QLineEdit;
    ntLabel->setBuddy(ntEdit);
    ntEdit->setText(QString::number(prob.numTests()));
    ntEdit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    connect(ntEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));

    tlLabel = new QLabel("Time Limit (ms): ");
    tlEdit = new QLineEdit;
    tlLabel->setBuddy(tlEdit);
    tlEdit->setText(QString::number(prob.timeLimit()));
    tlEdit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    connect(tlEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));

    mlLabel = new QLabel("Memory Limit (KB): ");
    mlEdit = new QLineEdit;
    mlLabel->setBuddy(mlEdit);
    mlEdit->setText(QString::number(prob.memoryLimit()));
    mlEdit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    connect(mlEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));

    scpLabel = new QLabel("Score Plan: ");
    scpComboBox = new QComboBox;
    scpLabel->setBuddy(scpComboBox);
    scpComboBox->addItem("Select Score Plan...");
    foreach (QString str, scps)
        scpComboBox->addItem(str);
    scpComboBox->setCurrentIndex(scps.indexOf(prob.scorePlan()) + 1);
    connect(scpComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateForm()));

    contestLabel = new QLabel("Contest: ");
    contestComboBox = new QComboBox;
    contestLabel->setBuddy(contestComboBox);
    foreach (QString str, cons)
        contestComboBox->addItem(str);
    int idx = cons.indexOf(prob.contest());
    contestComboBox->setCurrentIndex(idx == -1 ? 0 : idx);

    pubsubLabel = new QLabel("View Public Submissions' Code: ");
    pubsub = new QComboBox;
    pubsub->addItem(viewCodePermissions[0]);
    pubsub->addItem(viewCodePermissions[1]);
    pubsub->addItem(viewCodePermissions[2]);
    pubsub->setCurrentIndex(prob.publicSubmissions());

    othsubLabel = new QLabel("View Other's Submissions' Code: ");
    othsub = new QComboBox;
    othsub->addItem(viewCodePermissions[0]);
    othsub->addItem(viewCodePermissions[1]);
    othsub->addItem(viewCodePermissions[2]);
    othsub->setCurrentIndex(prob.othersSubmissions());

    glayout = new QGridLayout;
    glayout->addWidget(nameLabel, 0, 0);
    glayout->addWidget(nameEdit, 0, 1);
    glayout->addWidget(folderLabel, 1, 0);
    glayout->addWidget(folderEdit, 1, 1);
    glayout->addWidget(descLabel, 2, 0);
    glayout->addWidget(descEdit, 2, 1);
    glayout->addWidget(descFileLabel, 3, 0);
    glayout->addWidget(descFileEdit, 3, 1);
    glayout->addWidget(ntLabel, 4, 0);
    glayout->addWidget(ntEdit, 4, 1);
    glayout->addWidget(tlLabel, 5, 0);
    glayout->addWidget(tlEdit, 5, 1);
    glayout->addWidget(mlLabel, 6, 0);
    glayout->addWidget(mlEdit, 6, 1);
    glayout->addWidget(scpLabel, 7, 0);
    glayout->addWidget(scpComboBox, 7, 1);
    glayout->addWidget(contestLabel, 8, 0);
    glayout->addWidget(contestComboBox, 8, 1);
    glayout->addWidget(pubsubLabel, 9, 0);
    glayout->addWidget(pubsub, 9, 1);
    glayout->addWidget(othsubLabel, 10, 0);
    glayout->addWidget(othsub, 10, 1);

    completed = new QCheckBox("Complete Judge");
    completed->setChecked(prob.isComplete());
    enabled = new QCheckBox("Enabled");
    enabled->setChecked(prob.isEnabled());

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(glayout);
    totalLayout->addWidget(completed);
    totalLayout->addWidget(enabled);
    totalLayout->addWidget(buttons);

    setLayout(totalLayout);

    validateForm();
}

Problem ProblemSpecsDialog::resultProblem() const
{
    return Problem(cur.ID(), nameEdit->text(), descEdit->text(), ntEdit->text().toInt(), tlEdit->text().toLongLong(),
                   mlEdit->text().toLongLong(), completed->isChecked(), enabled->isChecked(),
                   scpComboBox->currentText(), contestComboBox->currentText(), descFileEdit->text(), folderEdit->text(),
                   pubsub->currentIndex(), othsub->currentIndex());
}

void ProblemSpecsDialog::validateForm()
{
    buttons->button(QDialogButtonBox::Ok)->setEnabled(!nameEdit->text().isEmpty() && !ntEdit->text().isEmpty()
                                                      && !tlEdit->text().isEmpty() && !mlEdit->text().isEmpty()
                                                      && scpComboBox->currentIndex() > 0 && !folderEdit->text().isEmpty());
}
