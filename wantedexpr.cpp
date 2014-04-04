#include <QRegExp>
#include "wantedexpr.h"

WantedExpr::WantedExpr(QObject *parent) :
    QObject(parent)
{
    setExpression("");
}

WantedExpr::WantedExpr(QString expr, QObject *parent) :
    QObject(parent)
{
    setExpression(expr);
}

QString WantedExpr::expression() const
{
    QString expr = expressionForMode(m.mode, m.customExpr, m.excludeOldVersions);
    if(m.excludeOldVersions) expr = QString("(%1) and (not unused)").arg(expr);
    if(m.includeFileCopies)  expr = QString("(%1) or approxlackingcopies=1").arg(expr);
    return expr;
}

int WantedExpr::mode() const
{
    return m.mode;
}

QString WantedExpr::customExpression() const
{
    return m.customExpr;
}

bool WantedExpr::includeOldVersions() const
{
    return !m.excludeOldVersions;
}

bool WantedExpr::includeFileCopies() const
{
    return m.includeFileCopies;
}

QString WantedExpr::description()
{
    switch(m.mode) {
    case ALL:
        if(m.includeFileCopies && m.excludeOldVersions)
            return tr("copy all files in, but move history away except files lacking copies");
        else if(m.excludeOldVersions)
            return tr("copy all files in, but move history away");
        else
            return tr("copy all files in");
    case MANUAL:
        if(m.includeFileCopies && m.excludeOldVersions)
            return tr("manage files manually, but move history away and get files lacking copies");
        else if(m.includeFileCopies)
            return tr("manage files manually, but get files lacking copies");
        else if(m.excludeOldVersions)
            return tr("manage files manually, but move history away");
        else
            return tr("manage files manually");
    case NONE:
        if(m.includeFileCopies)
            return tr("move all files away, but get files lacking copies");
        else
            return tr("move all files away");
    default:
    case CUSTOM:
        return tr("custom: %1").arg(expression());
    }
}

void WantedExpr::setExpression(QString expr)
{
    static QRegExp includeFileCopiesRegexp("\\((.*)\\) or approxlackingcopies=1");
    static QRegExp excludeOldVersionsRegexp("\\((.*)\\) and (\\(not unused\\)|not unused)");

    if ((m.includeFileCopies = includeFileCopiesRegexp.exactMatch(expr))) {
        expr = includeFileCopiesRegexp.cap(1);
    }

    if ((m.excludeOldVersions = excludeOldVersionsRegexp.exactMatch(expr))) {
        expr = includeFileCopiesRegexp.cap(1);
    }

    m.mode = modeForExpression(expr);
    m.customExpr = (m.mode == CUSTOM) ? expr : "";

    emit expressionChanged(expr);
}

void WantedExpr::setMode(int mode, QString customExpr)
{
    m.mode = mode;
    m.customExpr = customExpr;

    emit expressionChanged(expression());
}

void WantedExpr::setIncludeOldVersions(bool include)
{
    setExcludeOldVersions(! include);
}

void WantedExpr::setExcludeOldVersions(bool exclude)
{
    m.excludeOldVersions = exclude;

    emit expressionChanged(expression());
}

void WantedExpr::setIncludeFileCopies(bool include)
{
    m.includeFileCopies = include;

    emit expressionChanged(expression());
}

void WantedExpr::setExcludeFileCopies(bool exclude)
{
    setIncludeFileCopies(! exclude);
}

int WantedExpr::modeForExpression(QString expr)
{
    if(expr == "include=* or unused") return ALL;
    if(expr == "include=*")           return ALL;
    if(expr == "present")             return MANUAL;
    if(expr == "not copies=1")        return NONE;
    if(expr == "exclude=*")           return NONE;
    return CUSTOM;
}

QString WantedExpr::expressionForMode(int mode, QString customExpr, bool excludeOldVersions)
{
    switch(mode) {
    case ALL:
        return excludeOldVersions ? "include=*" : "include=* or unused";
    case MANUAL:
        return "present";
    case NONE:
        return "not copies=1";
    default:
    case CUSTOM:
        return customExpr;
    }
}
