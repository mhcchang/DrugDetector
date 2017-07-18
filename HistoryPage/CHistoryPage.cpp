#include "CHistoryPage.h"
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextStream>
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include <QMessageBox>
CHistoryPage::CHistoryPage(QWidget *parent) : QWidget(parent)
{
    _InitTableWidget();
    _InitButtonWidget();
    _LoadQss();
    _InitLayout();
    // 初始化数据库
    _InitDataBase();
}

void CHistoryPage::SetTestResultDataList(QList<TestResultData *> pTestResultDataList)
{
    m_pTestResultDataList = pTestResultDataList;
    qDebug() << "get history  test size: " << m_pTestResultDataList.count();
}

void CHistoryPage::SetTestUserData(DetectorPageUserData sDetectorPageUserData)
{
    m_sDetectorPageUserData = sDetectorPageUserData;
    qDebug() << "user histroyt  data: " << m_sDetectorPageUserData.strOtherReasonComments;
}

void CHistoryPage::ShowCurrentDateTest()
{
    // 查询数据库
    QDate qCurrentDate = QDate::currentDate();// 当前时间结果
    QString strSelect = QString("SELECT * FROM drugdata WHERE TestTime like '");
    strSelect += QString::number(qCurrentDate.year()) + "%";
    strSelect += QString::number(qCurrentDate.month()) + "%";
    strSelect += QString::number(qCurrentDate.day()) + "%\'";
    qDebug() << "slel " << strSelect;
    QSqlQuery qSqlQuery(strSelect);// 69列
    while(qSqlQuery.next())
    {
        QStringList strLineDataList;
        for(int i = 0; i != 68; ++i)
        {
            strLineDataList.push_back(qSqlQuery.value(i+1).toString());
        }
        // 数据
        qDebug() << "list " << strLineDataList;
        m_strTableLineDataList.push_back(strLineDataList);
        // 表格
        _InsertOneLine(strLineDataList);
    }
    // 显示到控件
    m_pHistoryDataTableWidget->update();
}
// 插入数据库
void CHistoryPage::InsertToDatabase()
{

    if (_ConnectDataBase(QCoreApplication::applicationDirPath() + "drug.db"))
    {
        QString strInsert = "INSERT INTO drugdata (DonorFirstName, DonorLastName, TestTime, BirthDate, DonorID, TestSite, Operator, "
                            "PreEmployment, Random, ReasonSuspicionCause, PostAccident, ReturnToDuty, FollowUp, Comments, "
                            "TemperatureNormal, ProductDefinition, ExpirationDate, ProductLot, ProductID, ProgramsNumber";
        for(int i = 0; i < 16; ++i)
        {
            strInsert += QString(", ") + QString("ProgramName") + QString::number(i);
            strInsert += QString(", ") + QString("Result") + QString::number(i);
            strInsert += QString(", ") + QString("Cutoff") + QString::number(i);
        }
        // 共计68列
        strInsert += QString(") VALUES (?");
        for(int i = 0; i < 67; ++i)
        {
            strInsert += QString(", ?");
        }
        strInsert += QString(")");

        QSqlQuery qSqlQuery;
        qSqlQuery.prepare(strInsert);

        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorFirstName.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorLastName.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserData.qTestDateTime);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.qBirthDate);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strDonorID);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strTestSite.toLocal8Bit());
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strOperator);// 获得用户身份
        //
        QString strFlag = "false";
        //
        strFlag = m_sDetectorPageUserData.bPreEmployment ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bRandom ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bReasonableSuspicionCause ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bPostAccident ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bReturnToDuty ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        //
        strFlag = m_sDetectorPageUserData.bFollowUp ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        // commets
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strOtherReasonComments);
        //
        strFlag = m_sDetectorPageUserData.bTemperatureNormal ? "true" : "false";
        qSqlQuery.addBindValue(strFlag);
        // product details
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductDefinition);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.qExpriationDate);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductLot);
        qSqlQuery.addBindValue(m_sDetectorPageUserData.strProductID);
        // program and picture
        qSqlQuery.addBindValue(m_sDetectorPageUserData.iProgramsNumber);
        // 插入结果数据
        int iTestResultDataCount = m_pTestResultDataList.count();
        for(int i = 0; i < iTestResultDataCount; ++i)
        {
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strProgramName.toLocal8Bit());
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->strResult.toLocal8Bit());
            qSqlQuery.addBindValue(m_pTestResultDataList.at(i)->iCutoffValue);
        }
        for(int i = iTestResultDataCount; i < 16; ++i)
        {
            qSqlQuery.addBindValue("");
            qSqlQuery.addBindValue("");
            qSqlQuery.addBindValue(0);
        }

        if (!qSqlQuery.exec())
        {
            qDebug() << qSqlQuery.lastError();
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  qSqlQuery.lastError().text());
        }
        qSqlQuery.finish();
        // 测试查询
        qSqlQuery.exec("SELECT id, DonorFirstName FROM drugdata");
        while (qSqlQuery.next()) {
            int name = qSqlQuery.value(0).toInt();
            QString age = qSqlQuery.value(1).toString();
            qDebug() << name << ": " << age;
        }

    }
}

void CHistoryPage::_LoadQss()
{
    QFile qFile(":/qss/HistoryPage/HistoryPage.qss");
    if(!qFile.open(QFile::ReadOnly))
    {
        qDebug() << "open false";
    }
    QTextStream qTestStream(&qFile);
    QString strStylesheet = qTestStream.readAll();
    this->setStyleSheet(strStylesheet);
   qFile.close();
}
// query condition group
QGroupBox *CHistoryPage::_CreateQueryConditionGroup()
{
    QGroupBox *pGroupBox = new QGroupBox(tr("Query Condition"), this);
    pGroupBox->setMaximumHeight(200);
    //
    m_pSubjectLastNameWidget = new CLabelLineEditWidget(tr("Subject Last Name"), "", this);
    m_pDonorIDWidget = new CLabelLineEditWidget(tr("Donor ID#"), "", this);
    m_pDateCollectedWidget = new CLabelDateWidget(tr("Date Collected"), QDate::currentDate(), this);
    m_pSinceRadioButton = new QRadioButton(tr("since"), this);
    m_pExactDateQRadioButton = new QRadioButton(tr("exact date"), this);
    //
    m_pLotNumberWidget = new CLabelLineEditWidget(tr("Lot Number"), "", this);
    QStringList strProductDefinitionList;
    strProductDefinitionList << tr("test1") << tr("test2");
    m_pProductDefinitionWidget = new CLabelCommoBoxWidget(tr("Product Definition"), strProductDefinitionList, this);
    //
    m_pQueryAllWidget = new CLabelCheckBoxWidget(tr("Query All"), true, this);
    // subject
    QHBoxLayout *pSubjectLayout = new QHBoxLayout;
    pSubjectLayout->addWidget(m_pDateCollectedWidget);
    //
    QVBoxLayout *pVSinceLayout = new QVBoxLayout;
    pVSinceLayout->addWidget(m_pSinceRadioButton);
    pVSinceLayout->addWidget(m_pExactDateQRadioButton);
    // 第一行
    pSubjectLayout->addLayout(pVSinceLayout);
    // lot number
    //
    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget(m_pSubjectLastNameWidget, 0, 0, 1, 1);
    pLayout->addWidget(m_pDonorIDWidget, 0, 1, 1, 1);
    pLayout->addLayout(pSubjectLayout, 0, 2, 1, 1);
    //
    pLayout->addWidget(m_pLotNumberWidget, 1, 0, 1, 1);
    pLayout->addWidget(m_pProductDefinitionWidget, 1, 1, 1, 1);
    pLayout->addWidget(m_pQueryAllWidget, 1, 2, 1, 1);
    //
    pGroupBox->setLayout(pLayout);

    return pGroupBox;
}

void CHistoryPage::_InitTableWidget()
{
    // table
    m_pHistoryDataTableWidget = new QTableWidget(this);
    m_pHistoryDataTableWidget->setMinimumHeight(350);
    // 表单样式
    m_pHistoryDataTableWidget->setColumnCount(68);
    QHeaderView *pHeaderView = m_pHistoryDataTableWidget->horizontalHeader();
    pHeaderView->setDefaultSectionSize(120);
    pHeaderView->setDisabled(true);
    // 表头字体
    QFont qFont = pHeaderView->font();
    qFont.setBold(true);
    pHeaderView->setFont(qFont);
    // 充满表格
    pHeaderView->setStretchLastSection(true);
     // 表头背景色
    pHeaderView->setStyleSheet("QHeaderView::section{background:skyblue;}");
    // 多选
    m_pHistoryDataTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // 每次选择一行
    m_pHistoryDataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置表头内容
    QStringList qstrHeaderList;
    qstrHeaderList << tr("Donor \r\nFirst Name") << tr("Donor \r\nLast Name")
                   << tr("Test Time") << tr("Birth Date") << tr("Donor ID") << tr("Test Site")
                   << tr("Operator") << tr("Pre-Employment") << tr("Random") << tr("Reason \r\nSuspicion Cause")
                   << tr("Post Accident") << tr("Return to Duty") << tr("Follow Up") << tr("Other Reason")
                   << tr("Comments") << tr("Temperature \r\nNormal#") << tr("Product Definition") << tr("Expiration Date")
                   << tr("Product Lot") << tr("Product ID") << tr("Number \r\nof Programs");
    for(int i = 0; i < 16; ++i)
    {
        qstrHeaderList << "Program Name " + QString::number(i);
        qstrHeaderList << "Result " + QString::number(i);
        qstrHeaderList << "Cutoff " + QString::number(i);
    }
    //
    m_pHistoryDataTableWidget->setHorizontalHeaderLabels(qstrHeaderList);
    // 显示格子线
    m_pHistoryDataTableWidget->setShowGrid(true);
    //设置水平、垂直滚动条样式
    m_pHistoryDataTableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
     m_pHistoryDataTableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
}

void CHistoryPage::_InitButtonWidget()
{
    // button
    m_pQueryButton = new QPushButton(tr("Query"), this);
    m_pSelectAllButton = new QPushButton(tr("Selet All"), this);
    m_pDeselectAllButton = new QPushButton(tr("Deselect All"), this);
    m_pDeleteButton = new QPushButton(tr("Delete"), this);
    m_pExportButton = new QPushButton(tr("Export"), this);
}

void CHistoryPage::_InitLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pHistoryDataTableWidget);
    pLayout->addWidget(_CreateQueryConditionGroup());
    //
    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    pButtonLayout->addStretch(10);
    pButtonLayout->addWidget(m_pQueryButton);
    pButtonLayout->addWidget(m_pSelectAllButton);
    pButtonLayout->addWidget(m_pDeselectAllButton);
    pButtonLayout->addWidget(m_pDeleteButton);
    pButtonLayout->addWidget(m_pExportButton);
    pButtonLayout->addStretch(10);
    //
    pLayout->addLayout(pButtonLayout);
    this->setLayout(pLayout);
}

//添加数据库驱动、设置数据库名称、数据库登录用户名、密码
bool CHistoryPage::_ConnectDataBase(const QString &strDBName)
{
    QSqlDatabase qSqlDataBase = QSqlDatabase::addDatabase("QSQLITE");
//    db.setHostName("host");
//    db.setDatabaseName("dbname");
//    db.setUserName("username");
//    db.setPassword("password");
    qSqlDataBase.setDatabaseName(strDBName);
    if (!qSqlDataBase.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              qSqlDataBase.lastError().text());
        return false;
    }
    else
    {
        return true;
    }
}

bool CHistoryPage::_InsertOneLine(QStringList strContentList)
{
    int iColumnCount = m_pHistoryDataTableWidget->columnCount();
    int iContentListCount = strContentList.count();
    if(iContentListCount < 1 || iContentListCount != iColumnCount)
    {// 插入数据不正确，不进行插入操作
        return false;
    }
    // 创建行
    int iRow = m_pHistoryDataTableWidget->rowCount();
    m_pHistoryDataTableWidget->insertRow(iRow);
    //
    for(int i = 0; i != iColumnCount; ++i)
    {
        if(!_InsertOneItem(iRow, i, strContentList.at(i)))
        {
            return false;
        }
    }
    return true;
}

bool CHistoryPage::_InsertOneItem(int iRow, int iColumn, QString strContent)
{
    int iColumnCount = m_pHistoryDataTableWidget->columnCount();
    int iRowCount = m_pHistoryDataTableWidget->rowCount();
    if(iColumn < iColumnCount && iRow < iRowCount)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem;
        pItem->setText(strContent);
        m_pHistoryDataTableWidget->setItem(iRow, iColumn, pItem);
        return true;
    }
    else
    {
        return false;
    }
}
// 创建数据库
void CHistoryPage::_InitDataBase()
{
    if (_ConnectDataBase(QCoreApplication::applicationDirPath() + "drug.db"))
    {
        QString strCreateTable  = "CREATE TABLE drugdata ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "DonorFirstName VARCHAR,"
                                  "DonorLastName VARCHAR,"
                                  "TestTime VARCHAR,"
                                  "BirthDate VARCHAR,"
                                  "DonorID VARCHAR,"
                                  "TestSite VARCHAR,"
                                  "Operator VARCHAR,"
                                 "PreEmployment VARCHAR,"
                                  "Random VARCHAR,"
                                  "ReasonSuspicionCause VARCHAR,"
                                  "PostAccident VARCHAR,"
                                  "ReturnToDuty VARCHAR,"
                                  "FollowUp VARCHAR,"
                                  "Comments VARCHAR,"
                                  "TemperatureNormal VARCHAR,"
                                  "ProductDefinition VARCHAR,"
                                  "ExpirationDate VARCHAR,"
                                  "ProductLot VARCHAR,"
                                  "ProductID VARCHAR,"
                                  "ProgramsNumber INT,";
        for(int i = 0; i < 16; ++i)
        {
            strCreateTable += QString("ProgramName") + QString::number(i) + QString(" VARCHAR,");
            strCreateTable += QString("Result") + QString::number(i) + QString(" VARCHAR,");
            strCreateTable += QString("Cutoff") + QString::number(i) + QString(" INT,");
        }
        strCreateTable.replace(strCreateTable.count()-1, 1, ")");// 替换最后“,”为“)”
        // 创建
        QSqlQuery qSqlQuery;
        if (!qSqlQuery.exec(strCreateTable))
        {
            //QMessageBox::critical(0, QObject::tr("Database Error"), qSqlQuery.lastError().text());
            qDebug() << " database error: " << qSqlQuery.lastError().text();
        }
    }

}
