/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SCOREPLANMANAGER_H
#define SCOREPLANMANAGER_H

#include <QWidget>
#include <QList>
#include "general.h"

class QLabel;
class QComboBox;
class QPushButton;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QSlider;
class QLineEdit;
class MainWidget;
class QCheckBox;
class QDoubleSpinBox;

class ScorePlanManager : public QWidget
{
    Q_OBJECT
private:
    static const int SLIDER_MAX;
    static const int SPINBOX_MAX;

    QLabel *scorePlanLabel;
    QComboBox *scorePlans;
    QPushButton *refreshButton;
    QHBoxLayout *upLayout;

    QList<QString> scores;

    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QCheckBox *deleteCheckBox;

    QLabel *aPenaltyLabel;
    QSlider *aPenaltySlider;
    QDoubleSpinBox *aPenaltySpinBox;

    QLabel *rPenaltyLabel;
    QSlider *rPenaltySlider;
    QDoubleSpinBox *rPenaltySpinBox;

    QPushButton *previewButton;

    QLabel *convergeToLabel;
    QDoubleSpinBox *convergeToSpinBox;

    QLabel *multiplierLabel;
    QLineEdit *multiplierLineEdit;

    QPushButton *submit;

    QGridLayout *gLayout;
    QHBoxLayout *hLayout;
    QVBoxLayout *downLayout, *totalLayout;
    QWidget *downWidget;

    MainWidget *mw;

    qint64 lastID;

    Operation lastOp;
    QString last;
    QString newName;

    QComboBox *replaceComboBox;

    void setupComboBox();
    void setupReplaceComboBox();
    void clearDown();

public:
    explicit ScorePlanManager(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

private slots:
    void aPenaltySliderChanged(int val);
    void aPenaltySpinBoxChanged(double val);
    void rPenaltySliderChanged(int val);
    void rPenaltySpinBoxChanged(double val);

    void comboBoxChanged(int idx);
    void submited();
    void viewPreview();
    void downloadList();
    void deleteCheckChanged();

public slots:
    void listDownloaded(StatCode stat, const QByteArray &BA);
    void operationDone(StatCode stat, const QByteArray &);
    void detailsReady(StatCode stat, const QByteArray &BA);
    void connectionStateChanged();
};

#endif // SCOREPLANMANAGER_H
