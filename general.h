/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef GENERAL_H
#define GENERAL_H

#include <QDataStream>
#include <QString>
#include <QColor>
#include <QSharedPointer>
#include <QMap>
#include <QVariant>
#include <QDateTime>

#if QT_VERSION < 0x050000
#ifndef NULL
#define NULL 0
#endif
#define Q_NULLPTR NULL
#endif

enum class StatCode : int
{
    RegisterationRequest, LoginRequest, SubmissionRequest, ResultRequest, CompilerLogRequest, ChangePasswordRequest,
    ViewCodeRequest,
    InternalDatabaseError,
    UserOK, NoSuchUser, UserNotApproved, InvalidPassword, AdminOK,
    OperationSuccessful, NotAuthorized, IncompatibleVersion,
    AlreadyExists,
    NoSuchProblem, SlowDown, InternalFileOperationError,
    Disabled, InQueue, Running, ServerError, ResourceLimit, RunError, WrongAnswer, Correct, CompileError, CompleteResult,
    ProblemManagementQuery, ProblemManagementDetails, ProblemManagementEdit, ProblemManagementRemove,
    ProblemManagementRejudge, ProblemNotEnoughData,
    ScorePlanQuery, ScorePlanDetails, ScorePlanEdit, ScorePlanRemove,
    UserQuery, UserChangeAdmin, UserChangeActivation, RenameUser, UserDelete, UserChangeDescription, NotTheOnlyAdmin,
    ChangePublicState, DeleteSubmission, ViewProfile, GetConfig, SetConfig,
    NotAvailable, SubmissionPurge, PurgeFiles, GiveDescription, GetNews, EditNews, RemoveNews,
    CannotViewSubmission, ContestUserQuery, ContestListQuery, ContestDetails, CurrentContests, RegisterContest, ContestEdit, ContestRemove,
    AlreadyRegistered, CannotProcessAtThisTime, ScoreboardConfig, ScoreboardEdit, ScoreboardMetaData, Scoreboard, UserRecord
};

enum class Operation {None, Remove, Add, Edit, Request, Other};

const QString timeUnits[] = {"ms", "s"};
const QString memUnits[] = {"KB", "MB", "GB"};
const QString statusString[] = {"In Queue", "Running", "Server Error", "Resource Limit", "Runtime Error", "Wrong Answer",
                                             "Correct", "Compilation Error", "Complete Result"};
const QColor backgroundColors[] = {Qt::gray, Qt::white, QColor(102, 102, 255), Qt::yellow, QColor(255, 140, 0),
                                   QColor(255, 51, 51), Qt::green, QColor(205, 92, 92), Qt::cyan};

const QString viewCodePermissions[] = {"Nobody", "Only users with correct submission", "Everybody"};
const QString scoreboardTypes[] = {"None", "Score", "#Corrects", "Score | #Corrects"};
const QString DATE_TIME_FORMAT = "yyyy/MM/dd hh:mm:ss";

class SubmissionResult
{
private:
    qint64 sid;
    QString uname;
    QString pname;
    qint64 dateTime;
    StatCode status;
    int numTest;
    int totTest;
    qint64 maxTime;
    qint64 maxMem;
    QList<qint8> fullResult;
    bool isPub;
    qreal scoreDff;

public:
    SubmissionResult() {}
    SubmissionResult(qint64 subid, const QString &un, const QString &pn, qint64 dt, StatCode stat,
                     int nt, int tt, qint64 mt, qint64 mm, const QList<qint8> &ls, bool pub)
        : sid(subid), uname(un), pname(pn), dateTime(dt), status(stat), numTest(nt), totTest(tt), maxTime(mt),
          maxMem(mm), fullResult(ls), isPub(pub) {}

    qint64 submitID() const {return sid;}
    QString username() const {return uname;}
    QString probName() const {return pname;}
    qint64 date() const {return dateTime;}
    StatCode judgeStatus() const {return status;}
    int numTests() const {return numTest;}
    int totalTests() const {return totTest;}
    qint64 maximumTime() const {return maxTime;}
    qint64 maximumMemory() const {return maxMem;}
    const QList<qint8> &completeResult() const {return fullResult;}
    bool isPublic() const {return isPub;}
    qreal scoreDiff() const {return scoreDff;}

    SubmissionResult invertedPublicState() const
    {
        SubmissionResult ret(*this);
        ret.isPub = !ret.isPub;
        return ret;
    }

    SubmissionResult setInQueue() const
    {
        SubmissionResult ret(*this);
        ret.status = StatCode::InQueue;
        return ret;
    }

    friend QDataStream &operator>>(QDataStream &DS, SubmissionResult &in)
    {
        int tmp;
        DS >> in.sid >> in.uname >> in.pname >> in.dateTime >> tmp >> in.numTest >> in.totTest
           >> in.maxTime >> in.maxMem >> in.isPub >> in.scoreDff;
        in.status = static_cast<StatCode>(tmp);
        if (in.status == StatCode::CompleteResult)
            DS >> in.fullResult;
        return DS;
    }

    friend QDataStream & operator<<(QDataStream &DS, const SubmissionResult &in)
    {
        DS << in.sid << in.uname << in.pname << in.dateTime << static_cast<int>(in.status) << in.numTest
           << in.totTest << in.maxTime << in.maxMem << in.isPub << in.scoreDff;
        if (in.status == StatCode::CompleteResult)
            DS << in.fullResult;
        return DS;
    }
};

typedef QMap<QString, QVariant> DataTable;
typedef QSharedPointer<QDataStream> DataPointer;

class Problem
{
private:
    qint64 id;
    QString nam;
    QString dsc;
    int nTest;
    qint64 TLimit;
    qint64 MLimit;
    bool isCompl;
    bool enabled;
    QString scoreP;
    QString contst;
    QString descFile;
    QString foldr;
    int pubsubs;
    int othrssubs;

public:
    Problem() {}
    Problem(qint64 _id, const QString &nm, const QString &dc, int nt, qint64 tl, qint64 ml, bool comple,
            bool enbl, const QString &sc, const QString &cont, const QString &df, const QString &fldr, int ps, int os)
        : id(_id), nam(nm), dsc(dc), nTest(nt), TLimit(tl), MLimit(ml), isCompl(comple), enabled(enbl)
        , scoreP(sc), contst(cont), descFile(df), foldr(fldr), pubsubs(ps), othrssubs(os) {}

    qint64 ID() const {return id;}
    QString name() const {return nam;}
    QString description() const {return dsc;}
    int numTests() const {return nTest;}
    qint64 timeLimit() const {return TLimit;}
    qint64 memoryLimit() const {return MLimit;}
    bool isComplete() const {return isCompl;}
    bool isEnabled() const {return enabled;}
    QString scorePlan() const {return scoreP;}
    QString contest() const {return contst;}
    QString descriptionFile() const {return descFile;}
    QString folder() const {return foldr;}
    int publicSubmissions() const {return pubsubs;}
    int othersSubmissions() const {return othrssubs;}

    friend QDataStream & operator>>(QDataStream &DS, Problem &in)
    {
        DS >> in.id >> in.nam >> in.dsc >> in.nTest >> in.TLimit >> in.MLimit >> in.isCompl
           >> in.enabled >> in.scoreP >> in.contst >> in.descFile >> in.foldr >> in.pubsubs >> in.othrssubs;
        return DS;
    }

    friend QDataStream & operator<<(QDataStream &DS, const Problem &in)
    {
        DS << in.id << in.nam << in.dsc << in.nTest << in.TLimit << in.MLimit << in.isCompl
           << in.enabled << in.scoreP << in.contst << in.descFile << in.foldr << in.pubsubs << in.othrssubs;
        return DS;
    }
};

class ScorePlan
{
private:
    qint64 id;
    QString nam;
    qreal p1;
    qreal p2;
    qreal convTo;
    qreal mul;

    qreal give(qreal in)
    {
        in = qMin(in, 10000.0);
        qreal num = convTo * in + 2 * (1 - convTo);
        return num / in;
    }

public:
    ScorePlan() {}
    ScorePlan(qint64 _id, QString _nam, qreal _p1, qreal _p2, qreal _convTo, qreal _mul) :
        id(_id), nam(_nam), p1(_p1), p2(_p2), convTo(_convTo), mul(_mul) {}

    void init(qreal _penalty1, qreal _penalty2, qreal cTo, qreal mult)
    {
        p1 = _penalty1;
        p2 = _penalty2;
        convTo = cTo;
        mul = mult;
    }

    qint64 ID() const {return id;}
    QString name() const {return nam;}
    qreal penalty1() const {return p1;}
    qreal penalty2() const {return p2;}
    qreal convergeTo() const {return convTo;}
    qreal multiplier() const {return mul;}

    qreal query(qreal val1, qreal val2)
    {
        return give(pow(val1, p1) + pow(val2, p2)) * mul;
    }

    friend QDataStream & operator<<(QDataStream &DS, const ScorePlan &in)
    {
        DS << in.id << in.nam << in.p1 << in.p2 << in.convTo << in.mul;
        return DS;
    }

    friend QDataStream & operator>>(QDataStream &DS, ScorePlan &in)
    {
        DS >> in.id >> in.nam >> in.p1 >> in.p2 >> in.convTo >> in.mul;
        return DS;
    }
};

class User
{
private:
    QString nam;
    QString desc;
    QDateTime joind;
    bool enabld;
    qint64 subs;
    qint64 cors;
    bool isAdm;
    qreal scre;

public:
    User() {}
    User(const QString &_nam, const QString &_desc, const QDateTime &_joind, bool _enabld, qint64 _subs, qint64 _cors, bool _isAdm, qreal _scre)
        : nam(_nam), desc(_desc), joind(_joind), enabld(_enabld), subs(_subs), cors(_cors), isAdm(_isAdm), scre(_scre) {}

    QString name() const {return nam;}
    QString description() const {return desc;}
    bool enabled() const {return enabld;}
    qint64 numSubmits() const {return subs;}
    qint64 numCorrects() const {return cors;}
    bool isAdmin() const {return isAdm;}
    qreal score() const {return scre;}
    QDateTime joinDate() const {return joind;}

    void changeAdmin()
    {
        isAdm = !isAdm;
    }

    void changeActivation()
    {
        enabld = !enabld;
    }

    void changeDesc(const QString &str)
    {
        desc = str;
    }

    void changeName(const QString &str)
    {
        nam = str;
    }

    friend QDataStream & operator<<(QDataStream &DS, const User &in)
    {
        DS << in.nam << in.desc << in.joind << in.enabld << in.subs << in.cors << in.isAdm << in.scre;
        return DS;
    }

    friend QDataStream & operator>>(QDataStream &DS, User &in)
    {
        DS >> in.nam >> in.desc >> in.joind >> in.enabld >> in.subs >> in.cors >> in.isAdm >> in.scre;
        return DS;
    }
};

class Contest
{
private:
    qint64 id;
    QString nam;
    QString dsc;
    bool enbld;
    QDateTime regStart;
    QDateTime regEnd;
    QDateTime conStart;
    QDateTime conEnd;
    bool aftView;
    bool aftSub;

public:
    Contest() {}
    Contest(qint64 _id, const QString &nm, const QString &dc, bool enab, const QDateTime &rs,
            const QDateTime &re, const QDateTime &cs, const QDateTime &ce, bool av, bool as)
        : id(_id), nam(nm), dsc(dc), enbld(enab), regStart(rs), regEnd(re), conStart(cs),
          conEnd(ce), aftView(av), aftSub(as) {}

    qint64 ID() const {return id;}
    QString name() const {return nam;}
    QString description() const {return dsc;}
    bool enabled() const {return enbld;}
    QDateTime registerStart() const {return regStart;}
    QDateTime registerEnd() const {return regEnd;}
    QDateTime contestStart() const {return conStart;}
    QDateTime contestEnd() const {return conEnd;}
    bool afterContestView() const {return aftView;}
    bool afterContestSubmit() const {return aftSub;}

    friend QDataStream & operator>>(QDataStream &DS, Contest &in)
    {
        DS >> in.id >> in.nam >> in.dsc >> in.enbld >> in.regStart >> in.regEnd
           >> in.conStart >> in.conEnd >> in.aftView >> in.aftSub;
        return DS;
    }

    friend QDataStream & operator<<(QDataStream &DS, const Contest &in)
    {
        DS << in.id << in.nam << in.dsc << in.enbld << in.regStart << in.regEnd
           << in.conStart << in.conEnd << in.aftView << in.aftSub;
        return DS;
    }
};

#endif // GENERAL_H
