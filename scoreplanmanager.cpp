/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include <limits>
#include "scoreplanmanager.h"
#include "scorepreview.h"
#include "mainwidget.h"
#include "connection.h"

const int ScorePlanManager::SLIDER_MAX = 2000000;
const int ScorePlanManager::SPINBOX_MAX = 13.287565;

ScorePlanManager::ScorePlanManager(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    scorePlanLabel = new QLabel("Score Plan: ");
    scorePlans = new QComboBox;
    scorePlanLabel->setBuddy(scorePlans);
    refreshButton = new QPushButton("&Request List");
    refreshButton->setMinimumSize(refreshButton->sizeHint() * 1.3);
    connect(scorePlans, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChanged(int)));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(downloadList()));

    upLayout = new QHBoxLayout;
    upLayout->addWidget(scorePlanLabel);
    upLayout->addWidget(scorePlans);
    upLayout->addStretch();
    upLayout->addWidget(refreshButton);

    gLayout = new QGridLayout;

    nameLabel = new QLabel("Plan Name: ");
    nameLineEdit = new QLineEdit;
    nameLabel->setBuddy(nameLineEdit);
    deleteCheckBox = new QCheckBox("&Delete");
    replaceComboBox = new QComboBox;

    gLayout->addWidget(nameLabel, 0, 0);
    gLayout->addWidget(nameLineEdit, 0, 1);
    gLayout->addWidget(deleteCheckBox, 0, 2);
    gLayout->addWidget(replaceComboBox, 0, 3);

    previewButton = new QPushButton("&Preview");
    previewButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(previewButton, SIGNAL(clicked()), this, SLOT(viewPreview()));

    aPenaltyLabel = new QLabel("Attempt Penalty: ");
    aPenaltySlider = new QSlider(Qt::Horizontal);
    aPenaltyLabel->setBuddy(aPenaltySlider);
    aPenaltySpinBox = new QDoubleSpinBox;
    gLayout->addWidget(aPenaltyLabel, 1, 0);
    gLayout->addWidget(aPenaltySlider, 1, 1);
    gLayout->addWidget(aPenaltySpinBox, 1, 2);
    gLayout->addWidget(previewButton, 1, 3, 2, 1);
    aPenaltySlider->setMinimum(0);
    aPenaltySlider->setMaximum(SLIDER_MAX);
    aPenaltySpinBox->setMinimum(0);
    aPenaltySpinBox->setDecimals(6);
    aPenaltySpinBox->setSingleStep(0.01);
    aPenaltySpinBox->setMaximum(SPINBOX_MAX);
    connect(aPenaltySlider, SIGNAL(valueChanged(int)), this, SLOT(aPenaltySliderChanged(int)));
    connect(aPenaltySpinBox, SIGNAL(valueChanged(double)), this, SLOT(aPenaltySpinBoxChanged(double)));

    rPenaltyLabel = new QLabel("Rank Penalty: ");
    rPenaltySlider = new QSlider(Qt::Horizontal);
    rPenaltyLabel->setBuddy(rPenaltySlider);
    rPenaltySpinBox = new QDoubleSpinBox;
    gLayout->addWidget(rPenaltyLabel, 2, 0);
    gLayout->addWidget(rPenaltySlider, 2, 1);
    gLayout->addWidget(rPenaltySpinBox, 2, 2);
    rPenaltySlider->setMinimum(0);
    rPenaltySlider->setMaximum(SLIDER_MAX);
    rPenaltySpinBox->setMinimum(0);
    rPenaltySpinBox->setDecimals(6);
    rPenaltySpinBox->setMaximum(SPINBOX_MAX);
    rPenaltySpinBox->setSingleStep(0.01);
    connect(rPenaltySlider, SIGNAL(valueChanged(int)), this, SLOT(rPenaltySliderChanged(int)));
    connect(rPenaltySpinBox, SIGNAL(valueChanged(double)), this, SLOT(rPenaltySpinBoxChanged(double)));

    convergeToLabel = new QLabel("Converge To: ");
    convergeToSpinBox = new QDoubleSpinBox;
    convergeToSpinBox->setMinimum(0);
    convergeToSpinBox->setMaximum(1);
    convergeToSpinBox->setDecimals(6);
    convergeToSpinBox->setSingleStep(0.01);
    convergeToLabel->setBuddy(convergeToSpinBox);
    multiplierLabel = new QLabel("Multiplier: ");
    multiplierLineEdit = new QLineEdit;
    multiplierLabel->setBuddy(multiplierLineEdit);
    multiplierLineEdit->setValidator(new QDoubleValidator(0.0, std::numeric_limits<qreal>::infinity(), 50));
    gLayout->addWidget(convergeToLabel, 3, 0);
    gLayout->addWidget(convergeToSpinBox, 3, 1);
    gLayout->addWidget(multiplierLabel, 3, 2);
    gLayout->addWidget(multiplierLineEdit, 3, 3);

    submit = new QPushButton;
    submit->setMinimumSize(submit->sizeHint() * 1.3);
    hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(submit);
    hLayout->addStretch();
    connect(submit, SIGNAL(clicked()), this, SLOT(submited()));

    downLayout = new QVBoxLayout;
    downLayout->addLayout(gLayout);
    downLayout->addLayout(hLayout);

    downWidget = new QWidget;
    downWidget->setLayout(downLayout);

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(upLayout);
    totalLayout->addStretch();
    totalLayout->addWidget(downWidget);
    totalLayout->addStretch();

    lastOp = Operation::None;
    connect(mw->connection(), SIGNAL(stateChanged()), this, SLOT(connectionStateChanged()));
    connect(deleteCheckBox, SIGNAL(stateChanged(int)), this, SLOT(deleteCheckChanged()));

    setLayout(totalLayout);
}

void ScorePlanManager::init()
{
    setupComboBox();
    connectionStateChanged();
}

void ScorePlanManager::onSwitch()
{
    if (!mw->connection()->isBusy())
        downloadList();
}

void ScorePlanManager::aPenaltySliderChanged(int val)
{
    aPenaltySpinBox->setValue(val / 1e6);
}

void ScorePlanManager::aPenaltySpinBoxChanged(double val)
{
    aPenaltySlider->setValue(qMin(static_cast<int>(val * 1e6), SLIDER_MAX));
}

void ScorePlanManager::rPenaltySliderChanged(int val)
{
    rPenaltySpinBox->setValue(val / 1e6);
}

void ScorePlanManager::rPenaltySpinBoxChanged(double val)
{
    rPenaltySlider->setValue(qMin(static_cast<int>(val * 1e6), SLIDER_MAX));
}

void ScorePlanManager::setupComboBox()
{
    scorePlans->clear();
    scorePlans->addItem("None");
    scorePlans->addItem("Add New Plan");
    foreach (QString pr, scores)
        scorePlans->addItem(pr);
    scorePlans->setCurrentIndex(0);
}

void ScorePlanManager::setupReplaceComboBox()
{
    replaceComboBox->clear();
    replaceComboBox->addItem("Replace plan with...");
    foreach (QString pr, scores)
        if (pr != scorePlans->currentText())
            replaceComboBox->addItem(pr);
    replaceComboBox->setCurrentIndex(0);
}

void ScorePlanManager::clearDown()
{
    nameLineEdit->clear();
    aPenaltySpinBox->setValue(0);
    rPenaltySpinBox->setValue(0);
    convergeToSpinBox->setValue(0);
    multiplierLineEdit->setText("100");
}

void ScorePlanManager::comboBoxChanged(int idx)
{
    if (idx == 0)
        downWidget->setVisible(false);
    else
    {
        if (idx == 1)
        {
            clearDown();
            lastID = -1;
            nameLineEdit->setReadOnly(false);
            submit->setText("&Add Plan");
            deleteCheckBox->setChecked(false);
            deleteCheckBox->setVisible(false);
            downWidget->setVisible(true);
            deleteCheckChanged();
        }
        else if (idx > 1)
        {
            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(detailsReady(StatCode,QByteArray))))
            {
                (*DS) << static_cast<int>(StatCode::ScorePlanDetails);
                (*DS) << scores[idx - 2];
                mw->connection()->send();
            }
        }
    }
}

void ScorePlanManager::viewPreview()
{
    ScorePreview scorePreview(aPenaltySpinBox->text().toDouble(), rPenaltySpinBox->text().toDouble(),
                              convergeToSpinBox->text().toDouble(), multiplierLineEdit->text().toDouble());
    scorePreview.exec();
}

void ScorePlanManager::submited()
{
    ScorePlan plan(lastID, nameLineEdit->text(), aPenaltySpinBox->value(), rPenaltySpinBox->value(),
                   convergeToSpinBox->value(), multiplierLineEdit->text().toDouble());

    if (scorePlans->currentIndex() != 1 && deleteCheckBox->isChecked() && replaceComboBox->currentIndex() < 1)
    {
        QMessageBox::warning(this, "Incomplete form", "You should specify a replacement for the plan you want to remove.");
        return;
    }

    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
    {
        last = scorePlans->currentText();
        newName = plan.name();

        if (scorePlans->currentIndex() == 1)
        {
            (*DS) << static_cast<int>(StatCode::ScorePlanEdit);
            (*DS) << plan;
            lastOp = Operation::Add;
        }
        else if (deleteCheckBox->isChecked())
        {
            (*DS) << static_cast<int>(StatCode::ScorePlanRemove);
            (*DS) << last << replaceComboBox->currentText();
            lastOp = Operation::Remove;
        }
        else
        {
            (*DS) << static_cast<int>(StatCode::ScorePlanEdit);
            (*DS) << plan;
            lastOp = Operation::Edit;
        }
        mw->connection()->send();
    }
}

void ScorePlanManager::downloadList()
{
    DataPointer DS;

    if (DS = mw->connection()->prepare(this, SLOT(listDownloaded(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ScorePlanQuery);
        mw->connection()->send();
    }
}

void ScorePlanManager::listDownloaded(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        scores.clear();
        DS >> scores;
        setupComboBox();
        if (replaceComboBox->isVisible())
            setupReplaceComboBox();
    }
}

void ScorePlanManager::operationDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::AlreadyExists)
            QMessageBox::warning(this, "Error", "Score profile with same name exists");
        else if (stat == StatCode::OperationSuccessful)
        {
            QMessageBox::information(this, "Info", "Operation Successful");

            if (lastOp == Operation::Remove)
                scores.removeOne(last);
            else if (lastOp == Operation::Add)
                scores.append(newName);
            else if (lastOp == Operation::Edit)
                scores[scores.indexOf(last)] = newName;

            setupComboBox();
        }
    }

    lastOp = Operation::None;
}

void ScorePlanManager::connectionStateChanged()
{
    scorePlans->setEnabled(mw->connection()->giveState() == Connection::State::Disconnected);
}

void ScorePlanManager::detailsReady(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::OperationSuccessful)
        {
            QDataStream DS(BA);
            DS.setVersion(QDataStream::Qt_4_8);

            ScorePlan plan;
            DS >> plan;

            lastID = plan.ID();
            nameLineEdit->setText(plan.name());
            aPenaltySpinBox->setValue(plan.penalty1());
            rPenaltySpinBox->setValue(plan.penalty2());
            convergeToSpinBox->setValue(plan.convergeTo());
            multiplierLineEdit->setText(QString::number(plan.multiplier()));
            submit->setText("Apply &Changes");
            deleteCheckBox->setVisible(true);
            deleteCheckBox->setChecked(Qt::Unchecked);
            downWidget->setVisible(true);
            deleteCheckChanged();
        }
        else
        {
            QMessageBox::warning(this, "Error", "There was an error downloading plan details.");
            scorePlans->setCurrentIndex(0);
        }
    }
    else
        scorePlans->setCurrentIndex(0);
}

void ScorePlanManager::deleteCheckChanged()
{
    if (deleteCheckBox->isChecked())
    {
        replaceComboBox->setVisible(true);
        setupReplaceComboBox();
    }
    else
        replaceComboBox->setVisible(false);
}
