﻿/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 测试页的状态栏，显示测试的当前状态，进度条
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CDETECTORPAGESTATUSBAR_H
#define CDETECTORPAGESTATUSBAR_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
class CDetectorPageStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPageStatusBar(QWidget *parent = nullptr);

signals:

public slots:
public:
    void SetLineText(QString strText);
    void SetLineStartColor();
    void SetLineStopColor();
    void SetProgressValue(int iValue);
private:
    void _InitWidget();
    void _InitLayout();

private:
    QLineEdit *m_pTestStatusBarLineEdit;
    QProgressBar *m_pTestProgressBar;
};

#endif // CDETECTORPAGESTATUSBAR_H
