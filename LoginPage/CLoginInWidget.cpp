﻿/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 登陆页面Widget，用户登陆操作
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#include "CLoginInWidget.h"
#include <QBoxLayout>
#include <QFont>
#include <QPixmap>
#include <QBitmap>
#include <QPalette>
#include <QMouseEvent>
#include "PublicFunction.h"
CLoginInWidget::CLoginInWidget(QWidget *parent) : QWidget(parent)
{
    //
    this->setFixedSize(1030, 680);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    // 设置b背景图片
    SetWidgetBackImage(this, ":/image/ico/login/wondfo_login_bgr.png");
    //
    LoadQss(this, ":/qss/LoginPage/LoginPage.qss");
    //
    _InitWidget();
    _InitLayout();

}

void CLoginInWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton)
    {
        m_iPressPoint = event->globalPos();
        m_bLeftButtonCheck = true;
    }
    event->ignore();//表示继续向下传递事件，其他的控件还可以去获取
}

void CLoginInWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
        m_bLeftButtonCheck = false;
    }
    event->ignore();
}

void CLoginInWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( m_bLeftButtonCheck )
    {
        m_iMovePoint = event->globalPos();
        this->move( this->pos() + m_iMovePoint - m_iPressPoint );
        m_iPressPoint = m_iMovePoint;
    }
    event->ignore();
}

void CLoginInWidget::_SlotCheckMinButton()
{
    this->showMinimized();
}

void CLoginInWidget::_SlotCheckCloseButton()
{
    this->close();
}

void CLoginInWidget::_SlotCheckLoginButton()
{
    this->hide();
    emit SigShowMainWindow();
}
/**
  * @brief 初始化控件
  * @param
  * @return
  */
void CLoginInWidget::_InitWidget()
{
    //
    m_pMinButton = new QPushButton(this);
    m_pMinButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pMinButton, ":/image/ico/login/login_zoomout_pressed.jpg");
    connect(m_pMinButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckMinButton()));

    //
    m_pCloseButton = new QPushButton(this);
    m_pCloseButton->setFixedSize(60, 50);
    SetButtonBackImage(m_pCloseButton, ":/image/ico/login/login_close_pressed.jpg");
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckCloseButton()));
    //
    m_pLoginLabel = new QLabel("Login", this);
    m_pLoginLabel->setObjectName("m_pLoginLabel");
    // user
    m_pUserNameLabel = new QLabel("Username: ", this);
    //m_pUserNameLabel->setFixedSize(100, 50);
   // m_pUserNameLabel->setObjectName("m_pUserNameLabel");
    m_pUserNameLineEdit = new QLineEdit(this);
    m_pUserNameLineEdit->setFixedSize(280, 50);
    // pass
    m_pPasswordLabel = new QLabel("Password:  ", this);
   // m_pPasswordLabel->setObjectName("m_pUserNameLabel");
    m_pPasswordLineEdit = new QLineEdit(this);
    m_pPasswordLineEdit->setFixedSize(280, 50);
    //
    m_pLoginButton = new QPushButton("Login", this);
    m_pLoginButton->setFixedSize(160, 35);
    m_pLoginButton->setObjectName("m_pLoginButton");

    connect(m_pLoginButton, SIGNAL(clicked(bool)), this, SLOT(_SlotCheckLoginButton()));
}
/**
  * @brief 布局
  * @param
  * @return
  */
void CLoginInWidget::_InitLayout()
{
    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    pTitleLayout->setMargin(0);
    pTitleLayout->addStretch(100);
    pTitleLayout->addWidget(m_pMinButton);
    pTitleLayout->addWidget(m_pCloseButton);
    //
    QHBoxLayout *pFirstLineHLayout = new QHBoxLayout;
    pFirstLineHLayout->addSpacing(475);
    pFirstLineHLayout->addWidget(m_pLoginLabel);
    pFirstLineHLayout->addStretch(200);
    //
    QHBoxLayout *pUserHLayout = new QHBoxLayout;
    pUserHLayout->addStretch(100);
    pUserHLayout->addWidget(m_pUserNameLabel);
    pUserHLayout->addSpacing(10);
    pUserHLayout->addWidget(m_pUserNameLineEdit);
    pUserHLayout->addStretch(100);
    //
    QHBoxLayout *pPassHLayout = new QHBoxLayout;
    pPassHLayout->addStretch(100);
    pPassHLayout->addWidget(m_pPasswordLabel);
    pPassHLayout->addSpacing(10);
    pPassHLayout->addWidget(m_pPasswordLineEdit);
    pPassHLayout->addStretch(100);
    //
    QHBoxLayout *pEndLineHLayout = new QHBoxLayout;
    pEndLineHLayout->addSpacing(435);
    pEndLineHLayout->addWidget(m_pLoginButton);
    pEndLineHLayout->addStretch(200);
    //

    QVBoxLayout *pVLayout = new QVBoxLayout;
    pVLayout->setMargin(0);
    pVLayout->addLayout(pTitleLayout);
    pVLayout->addStretch(100);
    pVLayout->addLayout(pFirstLineHLayout);
    pVLayout->addSpacing(30);
    pVLayout->addLayout(pUserHLayout);
    pVLayout->addSpacing(30);
    pVLayout->addLayout(pPassHLayout);
    pVLayout->addSpacing(71);
    pVLayout->addLayout(pEndLineHLayout);
    pVLayout->addStretch(100);
    //
    this->setLayout(pVLayout);
}
