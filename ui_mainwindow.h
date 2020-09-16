/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace lc {

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_8;
    QTabWidget *TWExperimenterTab;
    QWidget *TExperimenterActions;
    QHBoxLayout *HLExperimenterTab;
    QVBoxLayout *VLzTreeClientActions;
    QGroupBox *GBLocalActions;
    QVBoxLayout *verticalLayout_3;
    QLabel *L_PlanSession;
    QPushButton *PBShowORSEE;
    QFrame *line_ORSEE;
    QLabel *L_PrintLocalFiles;
    QPushButton *PBShowPreprints;
    QPushButton *PBPrintPaymentFileManually;
    QFrame *line_otherNameZLeaf;
    QLabel *L_WebcamChooser;
    QComboBox *CBWebcamChooser;
    QSpacerItem *verticalSpacer_ServerActions;
    QVBoxLayout *VLFurtherSpecialActions;
    QGroupBox *GBClientActions;
    QVBoxLayout *verticalLayout_5;
    QLabel *L_ClientBootShutdown;
    QHBoxLayout *HLBootShutdown;
    QPushButton *PBShutdown;
    QPushButton *PBBoot;
    QFrame *line_bootActions;
    QLabel *L_VNC;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *PBViewDesktopFullControl;
    QPushButton *PBViewDesktopViewOnly;
    QLabel *label;
    QHBoxLayout *HLDisableRightClick;
    QPushButton *PBEnableRMB;
    QPushButton *PBDisableRMB;
    QFrame *line_viewDesktop;
    QHBoxLayout *HLFileChooser;
    QVBoxLayout *VLUploadFolder;
    QLabel *LUploadFolder;
    QHBoxLayout *HLUploadFolder;
    QPushButton *PBChooseFile;
    QLineEdit *LEFilePath;
    QPushButton *PBBeamFile;
    QFrame *line_uploadFolder;
    QLabel *L_BrowserActions;
    QHBoxLayout *HLURL;
    QLabel *L_URL;
    QLineEdit *LEURL;
    QHBoxLayout *horizontalLayout;
    QLabel *L_BrowserSelection;
    QComboBox *CB_BrowserSelection;
    QCheckBox *CBFullscreen;
    QHBoxLayout *HLbrowserActions;
    QPushButton *PBstartBrowser;
    QPushButton *PBstopBrowser;
    QSpacerItem *verticalSpacer_ClientActions;
    QWidget *TSessionStart;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *VLcleanupRecoverLastSession;
    QLabel *L_cleanupRecoverLastSession;
    QHBoxLayout *HLcleanupRecoverLastSession;
    QPushButton *PBStopZtree;
    QPushButton *PBKillzLeaf;
    QFrame *line;
    QPushButton *PBRecoverCrashedSession;
    QFrame *line_cleanupRecoverLastSession;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *VLNewSession;
    QLabel *LNewSession;
    QHBoxLayout *HLNewSession;
    QVBoxLayout *VLZtree;
    QLabel *LZTreeVersion;
    QComboBox *CBzTreeVersion;
    QLabel *LDataTargetPath;
    QComboBox *CBDataTargetPath;
    QVBoxLayout *VLZtreeOptions;
    QHBoxLayout *HLPortSelect;
    QLabel *LPort;
    QSpinBox *SBPort;
    QFrame *line_Ztree;
    QLabel *LTemplateForReceipts;
    QComboBox *CBReceiptsHeader;
    QCheckBox *ChBPrintAnonymousReceipts;
    QLabel *LReplaceParticipantNames;
    QComboBox *CBReplaceParticipantNames;
    QCheckBox *ChBReceiptsForLocalClients;
    QFrame *line_Receipts;
    QCheckBox *ChBRamDisk;
    QCheckBox *ChBSessionWithoutAttachedClients;
    QCheckBox *ChBautoStartClientZleaf;
    QPushButton *PBStartSession;
    QSpacerItem *VSpacerZtree;
    QVBoxLayout *VLZleave;
    QLabel *L_Zleaves;
    QLabel *LzLeafCommandline;
    QLineEdit *LEzLeafCommandline;
    QPushButton *PBStartzLeaf;
    QFrame *line_StartZleaves;
    QLabel *L_FakeName;
    QComboBox *CBClientNames;
    QPushButton *PBRunzLeaf;
    QFrame *line_RunWithOtherName;
    QLabel *L_LocalZleaf;
    QHBoxLayout *HLLocalzLeaf;
    QPushButton *PBStartLocalzLeaf;
    QPushButton *PBKillLocalzLeaf;
    QSpacerItem *VSpacerTemplates;
    QWidget *TAdminActions;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *GBInformation;
    QVBoxLayout *verticalLayout_7;
    QLabel *L_UserName;
    QLabel *L_AdministrativeRights;
    QGroupBox *GBOptionsForAdminActions;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *VLUseUser;
    QLabel *L_UseUser;
    QHBoxLayout *HLUserRBs;
    QRadioButton *RBUseUserRoot;
    QRadioButton *RBUseLocalUser;
    QGroupBox *GBActionsWithSelectedClients;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *PBOpenTerminal;
    QPushButton *PBOpenFilesystem;
    QGroupBox *GBExecuteOnEveryClient;
    QVBoxLayout *verticalLayout_11;
    QComboBox *CBCommandToExecute;
    QPushButton *PBExecute;
    QWidget *TInfo;
    QVBoxLayout *VLInfo;
    QLabel *LInfo;
    QTableView *TVClients;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *lc__MainWindow)
    {
        if (lc__MainWindow->objectName().isEmpty())
            lc__MainWindow->setObjectName(QStringLiteral("lc__MainWindow"));
        lc__MainWindow->resize(730, 853);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lc__MainWindow->sizePolicy().hasHeightForWidth());
        lc__MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral("../../../../../../usr/local/share/labcontrol/icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        lc__MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(lc__MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        verticalLayout_8 = new QVBoxLayout(centralWidget);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        TWExperimenterTab = new QTabWidget(centralWidget);
        TWExperimenterTab->setObjectName(QStringLiteral("TWExperimenterTab"));
        TWExperimenterTab->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(TWExperimenterTab->sizePolicy().hasHeightForWidth());
        TWExperimenterTab->setSizePolicy(sizePolicy2);
        TWExperimenterTab->setMinimumSize(QSize(640, 650));
        TWExperimenterTab->setBaseSize(QSize(0, 0));
        QFont font;
        font.setUnderline(false);
        font.setKerning(true);
        TWExperimenterTab->setFont(font);
        TWExperimenterTab->setUsesScrollButtons(true);
        TWExperimenterTab->setTabBarAutoHide(false);
        TExperimenterActions = new QWidget();
        TExperimenterActions->setObjectName(QStringLiteral("TExperimenterActions"));
        TExperimenterActions->setEnabled(true);
        sizePolicy2.setHeightForWidth(TExperimenterActions->sizePolicy().hasHeightForWidth());
        TExperimenterActions->setSizePolicy(sizePolicy2);
        HLExperimenterTab = new QHBoxLayout(TExperimenterActions);
        HLExperimenterTab->setSpacing(6);
        HLExperimenterTab->setContentsMargins(11, 11, 11, 11);
        HLExperimenterTab->setObjectName(QStringLiteral("HLExperimenterTab"));
        VLzTreeClientActions = new QVBoxLayout();
        VLzTreeClientActions->setSpacing(6);
        VLzTreeClientActions->setObjectName(QStringLiteral("VLzTreeClientActions"));
        GBLocalActions = new QGroupBox(TExperimenterActions);
        GBLocalActions->setObjectName(QStringLiteral("GBLocalActions"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(GBLocalActions->sizePolicy().hasHeightForWidth());
        GBLocalActions->setSizePolicy(sizePolicy3);
        verticalLayout_3 = new QVBoxLayout(GBLocalActions);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        L_PlanSession = new QLabel(GBLocalActions);
        L_PlanSession->setObjectName(QStringLiteral("L_PlanSession"));

        verticalLayout_3->addWidget(L_PlanSession);

        PBShowORSEE = new QPushButton(GBLocalActions);
        PBShowORSEE->setObjectName(QStringLiteral("PBShowORSEE"));

        verticalLayout_3->addWidget(PBShowORSEE);

        line_ORSEE = new QFrame(GBLocalActions);
        line_ORSEE->setObjectName(QStringLiteral("line_ORSEE"));
        line_ORSEE->setFrameShape(QFrame::HLine);
        line_ORSEE->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_ORSEE);

        L_PrintLocalFiles = new QLabel(GBLocalActions);
        L_PrintLocalFiles->setObjectName(QStringLiteral("L_PrintLocalFiles"));

        verticalLayout_3->addWidget(L_PrintLocalFiles);

        PBShowPreprints = new QPushButton(GBLocalActions);
        PBShowPreprints->setObjectName(QStringLiteral("PBShowPreprints"));

        verticalLayout_3->addWidget(PBShowPreprints);

        PBPrintPaymentFileManually = new QPushButton(GBLocalActions);
        PBPrintPaymentFileManually->setObjectName(QStringLiteral("PBPrintPaymentFileManually"));

        verticalLayout_3->addWidget(PBPrintPaymentFileManually);

        line_otherNameZLeaf = new QFrame(GBLocalActions);
        line_otherNameZLeaf->setObjectName(QStringLiteral("line_otherNameZLeaf"));
        line_otherNameZLeaf->setFrameShape(QFrame::HLine);
        line_otherNameZLeaf->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_otherNameZLeaf);

        L_WebcamChooser = new QLabel(GBLocalActions);
        L_WebcamChooser->setObjectName(QStringLiteral("L_WebcamChooser"));

        verticalLayout_3->addWidget(L_WebcamChooser);

        CBWebcamChooser = new QComboBox(GBLocalActions);
        CBWebcamChooser->addItem(QString());
        CBWebcamChooser->setObjectName(QStringLiteral("CBWebcamChooser"));

        verticalLayout_3->addWidget(CBWebcamChooser);

        verticalSpacer_ServerActions = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_ServerActions);


        VLzTreeClientActions->addWidget(GBLocalActions);


        HLExperimenterTab->addLayout(VLzTreeClientActions);

        VLFurtherSpecialActions = new QVBoxLayout();
        VLFurtherSpecialActions->setSpacing(6);
        VLFurtherSpecialActions->setObjectName(QStringLiteral("VLFurtherSpecialActions"));
        GBClientActions = new QGroupBox(TExperimenterActions);
        GBClientActions->setObjectName(QStringLiteral("GBClientActions"));
        GBClientActions->setEnabled(true);
        sizePolicy3.setHeightForWidth(GBClientActions->sizePolicy().hasHeightForWidth());
        GBClientActions->setSizePolicy(sizePolicy3);
        verticalLayout_5 = new QVBoxLayout(GBClientActions);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        L_ClientBootShutdown = new QLabel(GBClientActions);
        L_ClientBootShutdown->setObjectName(QStringLiteral("L_ClientBootShutdown"));

        verticalLayout_5->addWidget(L_ClientBootShutdown);

        HLBootShutdown = new QHBoxLayout();
        HLBootShutdown->setSpacing(6);
        HLBootShutdown->setObjectName(QStringLiteral("HLBootShutdown"));
        PBShutdown = new QPushButton(GBClientActions);
        PBShutdown->setObjectName(QStringLiteral("PBShutdown"));

        HLBootShutdown->addWidget(PBShutdown);

        PBBoot = new QPushButton(GBClientActions);
        PBBoot->setObjectName(QStringLiteral("PBBoot"));

        HLBootShutdown->addWidget(PBBoot);


        verticalLayout_5->addLayout(HLBootShutdown);

        line_bootActions = new QFrame(GBClientActions);
        line_bootActions->setObjectName(QStringLiteral("line_bootActions"));
        line_bootActions->setFrameShape(QFrame::HLine);
        line_bootActions->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(line_bootActions);

        L_VNC = new QLabel(GBClientActions);
        L_VNC->setObjectName(QStringLiteral("L_VNC"));

        verticalLayout_5->addWidget(L_VNC);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        PBViewDesktopFullControl = new QPushButton(GBClientActions);
        PBViewDesktopFullControl->setObjectName(QStringLiteral("PBViewDesktopFullControl"));

        horizontalLayout_2->addWidget(PBViewDesktopFullControl);

        PBViewDesktopViewOnly = new QPushButton(GBClientActions);
        PBViewDesktopViewOnly->setObjectName(QStringLiteral("PBViewDesktopViewOnly"));

        horizontalLayout_2->addWidget(PBViewDesktopViewOnly);


        verticalLayout_5->addLayout(horizontalLayout_2);

        label = new QLabel(GBClientActions);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_5->addWidget(label);

        HLDisableRightClick = new QHBoxLayout();
        HLDisableRightClick->setSpacing(6);
        HLDisableRightClick->setObjectName(QStringLiteral("HLDisableRightClick"));
        PBEnableRMB = new QPushButton(GBClientActions);
        PBEnableRMB->setObjectName(QStringLiteral("PBEnableRMB"));

        HLDisableRightClick->addWidget(PBEnableRMB);

        PBDisableRMB = new QPushButton(GBClientActions);
        PBDisableRMB->setObjectName(QStringLiteral("PBDisableRMB"));

        HLDisableRightClick->addWidget(PBDisableRMB);


        verticalLayout_5->addLayout(HLDisableRightClick);

        line_viewDesktop = new QFrame(GBClientActions);
        line_viewDesktop->setObjectName(QStringLiteral("line_viewDesktop"));
        line_viewDesktop->setFrameShape(QFrame::HLine);
        line_viewDesktop->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(line_viewDesktop);

        HLFileChooser = new QHBoxLayout();
        HLFileChooser->setSpacing(6);
        HLFileChooser->setObjectName(QStringLiteral("HLFileChooser"));
        VLUploadFolder = new QVBoxLayout();
        VLUploadFolder->setSpacing(6);
        VLUploadFolder->setObjectName(QStringLiteral("VLUploadFolder"));
        LUploadFolder = new QLabel(GBClientActions);
        LUploadFolder->setObjectName(QStringLiteral("LUploadFolder"));
        LUploadFolder->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        VLUploadFolder->addWidget(LUploadFolder);

        HLUploadFolder = new QHBoxLayout();
        HLUploadFolder->setSpacing(6);
        HLUploadFolder->setObjectName(QStringLiteral("HLUploadFolder"));
        PBChooseFile = new QPushButton(GBClientActions);
        PBChooseFile->setObjectName(QStringLiteral("PBChooseFile"));

        HLUploadFolder->addWidget(PBChooseFile);

        LEFilePath = new QLineEdit(GBClientActions);
        LEFilePath->setObjectName(QStringLiteral("LEFilePath"));
        LEFilePath->setReadOnly(true);

        HLUploadFolder->addWidget(LEFilePath);


        VLUploadFolder->addLayout(HLUploadFolder);

        PBBeamFile = new QPushButton(GBClientActions);
        PBBeamFile->setObjectName(QStringLiteral("PBBeamFile"));

        VLUploadFolder->addWidget(PBBeamFile);


        HLFileChooser->addLayout(VLUploadFolder);


        verticalLayout_5->addLayout(HLFileChooser);

        line_uploadFolder = new QFrame(GBClientActions);
        line_uploadFolder->setObjectName(QStringLiteral("line_uploadFolder"));
        line_uploadFolder->setFrameShape(QFrame::HLine);
        line_uploadFolder->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(line_uploadFolder);

        L_BrowserActions = new QLabel(GBClientActions);
        L_BrowserActions->setObjectName(QStringLiteral("L_BrowserActions"));

        verticalLayout_5->addWidget(L_BrowserActions);

        HLURL = new QHBoxLayout();
        HLURL->setSpacing(6);
        HLURL->setObjectName(QStringLiteral("HLURL"));
        L_URL = new QLabel(GBClientActions);
        L_URL->setObjectName(QStringLiteral("L_URL"));

        HLURL->addWidget(L_URL);

        LEURL = new QLineEdit(GBClientActions);
        LEURL->setObjectName(QStringLiteral("LEURL"));
        LEURL->setToolTipDuration(-1);

        HLURL->addWidget(LEURL);


        verticalLayout_5->addLayout(HLURL);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        L_BrowserSelection = new QLabel(GBClientActions);
        L_BrowserSelection->setObjectName(QStringLiteral("L_BrowserSelection"));

        horizontalLayout->addWidget(L_BrowserSelection);

        CB_BrowserSelection = new QComboBox(GBClientActions);
        CB_BrowserSelection->addItem(QString());
        CB_BrowserSelection->addItem(QString());
        CB_BrowserSelection->setObjectName(QStringLiteral("CB_BrowserSelection"));
        CB_BrowserSelection->setMinimumSize(QSize(240, 0));

        horizontalLayout->addWidget(CB_BrowserSelection);


        verticalLayout_5->addLayout(horizontalLayout);

        CBFullscreen = new QCheckBox(GBClientActions);
        CBFullscreen->setObjectName(QStringLiteral("CBFullscreen"));
        CBFullscreen->setChecked(true);

        verticalLayout_5->addWidget(CBFullscreen);

        HLbrowserActions = new QHBoxLayout();
        HLbrowserActions->setSpacing(6);
        HLbrowserActions->setObjectName(QStringLiteral("HLbrowserActions"));
        PBstartBrowser = new QPushButton(GBClientActions);
        PBstartBrowser->setObjectName(QStringLiteral("PBstartBrowser"));

        HLbrowserActions->addWidget(PBstartBrowser);

        PBstopBrowser = new QPushButton(GBClientActions);
        PBstopBrowser->setObjectName(QStringLiteral("PBstopBrowser"));

        HLbrowserActions->addWidget(PBstopBrowser);


        verticalLayout_5->addLayout(HLbrowserActions);

        verticalSpacer_ClientActions = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_ClientActions);


        VLFurtherSpecialActions->addWidget(GBClientActions);


        HLExperimenterTab->addLayout(VLFurtherSpecialActions);

        HLExperimenterTab->setStretch(0, 1);
        HLExperimenterTab->setStretch(1, 1);
        TWExperimenterTab->addTab(TExperimenterActions, QString());
        TSessionStart = new QWidget();
        TSessionStart->setObjectName(QStringLiteral("TSessionStart"));
        TSessionStart->setEnabled(true);
        TSessionStart->setMinimumSize(QSize(0, 439));
        verticalLayoutWidget = new QWidget(TSessionStart);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 691, 71));
        VLcleanupRecoverLastSession = new QVBoxLayout(verticalLayoutWidget);
        VLcleanupRecoverLastSession->setSpacing(6);
        VLcleanupRecoverLastSession->setContentsMargins(11, 11, 11, 11);
        VLcleanupRecoverLastSession->setObjectName(QStringLiteral("VLcleanupRecoverLastSession"));
        VLcleanupRecoverLastSession->setContentsMargins(0, 0, 0, 0);
        L_cleanupRecoverLastSession = new QLabel(verticalLayoutWidget);
        L_cleanupRecoverLastSession->setObjectName(QStringLiteral("L_cleanupRecoverLastSession"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        L_cleanupRecoverLastSession->setFont(font1);

        VLcleanupRecoverLastSession->addWidget(L_cleanupRecoverLastSession);

        HLcleanupRecoverLastSession = new QHBoxLayout();
        HLcleanupRecoverLastSession->setSpacing(6);
        HLcleanupRecoverLastSession->setObjectName(QStringLiteral("HLcleanupRecoverLastSession"));
        PBStopZtree = new QPushButton(verticalLayoutWidget);
        PBStopZtree->setObjectName(QStringLiteral("PBStopZtree"));

        HLcleanupRecoverLastSession->addWidget(PBStopZtree);

        PBKillzLeaf = new QPushButton(verticalLayoutWidget);
        PBKillzLeaf->setObjectName(QStringLiteral("PBKillzLeaf"));

        HLcleanupRecoverLastSession->addWidget(PBKillzLeaf);

        line = new QFrame(verticalLayoutWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        HLcleanupRecoverLastSession->addWidget(line);

        PBRecoverCrashedSession = new QPushButton(verticalLayoutWidget);
        PBRecoverCrashedSession->setObjectName(QStringLiteral("PBRecoverCrashedSession"));

        HLcleanupRecoverLastSession->addWidget(PBRecoverCrashedSession);


        VLcleanupRecoverLastSession->addLayout(HLcleanupRecoverLastSession);

        line_cleanupRecoverLastSession = new QFrame(verticalLayoutWidget);
        line_cleanupRecoverLastSession->setObjectName(QStringLiteral("line_cleanupRecoverLastSession"));
        line_cleanupRecoverLastSession->setFrameShape(QFrame::HLine);
        line_cleanupRecoverLastSession->setFrameShadow(QFrame::Sunken);

        VLcleanupRecoverLastSession->addWidget(line_cleanupRecoverLastSession);

        verticalLayoutWidget_3 = new QWidget(TSessionStart);
        verticalLayoutWidget_3->setObjectName(QStringLiteral("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(10, 88, 691, 539));
        VLNewSession = new QVBoxLayout(verticalLayoutWidget_3);
        VLNewSession->setSpacing(6);
        VLNewSession->setContentsMargins(11, 11, 11, 11);
        VLNewSession->setObjectName(QStringLiteral("VLNewSession"));
        VLNewSession->setSizeConstraint(QLayout::SetDefaultConstraint);
        VLNewSession->setContentsMargins(0, 0, 0, 0);
        LNewSession = new QLabel(verticalLayoutWidget_3);
        LNewSession->setObjectName(QStringLiteral("LNewSession"));

        VLNewSession->addWidget(LNewSession);

        HLNewSession = new QHBoxLayout();
        HLNewSession->setSpacing(6);
        HLNewSession->setObjectName(QStringLiteral("HLNewSession"));
        VLZtree = new QVBoxLayout();
        VLZtree->setSpacing(6);
        VLZtree->setObjectName(QStringLiteral("VLZtree"));
        VLZtree->setContentsMargins(9, 9, 9, 9);
        LZTreeVersion = new QLabel(verticalLayoutWidget_3);
        LZTreeVersion->setObjectName(QStringLiteral("LZTreeVersion"));

        VLZtree->addWidget(LZTreeVersion);

        CBzTreeVersion = new QComboBox(verticalLayoutWidget_3);
        CBzTreeVersion->setObjectName(QStringLiteral("CBzTreeVersion"));

        VLZtree->addWidget(CBzTreeVersion);

        LDataTargetPath = new QLabel(verticalLayoutWidget_3);
        LDataTargetPath->setObjectName(QStringLiteral("LDataTargetPath"));

        VLZtree->addWidget(LDataTargetPath);

        CBDataTargetPath = new QComboBox(verticalLayoutWidget_3);
        CBDataTargetPath->setObjectName(QStringLiteral("CBDataTargetPath"));

        VLZtree->addWidget(CBDataTargetPath);

        VLZtreeOptions = new QVBoxLayout();
        VLZtreeOptions->setSpacing(6);
        VLZtreeOptions->setObjectName(QStringLiteral("VLZtreeOptions"));
        VLZtreeOptions->setContentsMargins(-1, 0, -1, -1);
        HLPortSelect = new QHBoxLayout();
        HLPortSelect->setSpacing(6);
        HLPortSelect->setObjectName(QStringLiteral("HLPortSelect"));
        LPort = new QLabel(verticalLayoutWidget_3);
        LPort->setObjectName(QStringLiteral("LPort"));

        HLPortSelect->addWidget(LPort);

        SBPort = new QSpinBox(verticalLayoutWidget_3);
        SBPort->setObjectName(QStringLiteral("SBPort"));
        SBPort->setMinimum(7000);
        SBPort->setMaximum(10000);

        HLPortSelect->addWidget(SBPort);


        VLZtreeOptions->addLayout(HLPortSelect);


        VLZtree->addLayout(VLZtreeOptions);

        line_Ztree = new QFrame(verticalLayoutWidget_3);
        line_Ztree->setObjectName(QStringLiteral("line_Ztree"));
        line_Ztree->setFrameShape(QFrame::HLine);
        line_Ztree->setFrameShadow(QFrame::Sunken);

        VLZtree->addWidget(line_Ztree);

        LTemplateForReceipts = new QLabel(verticalLayoutWidget_3);
        LTemplateForReceipts->setObjectName(QStringLiteral("LTemplateForReceipts"));

        VLZtree->addWidget(LTemplateForReceipts);

        CBReceiptsHeader = new QComboBox(verticalLayoutWidget_3);
        CBReceiptsHeader->setObjectName(QStringLiteral("CBReceiptsHeader"));

        VLZtree->addWidget(CBReceiptsHeader);

        ChBPrintAnonymousReceipts = new QCheckBox(verticalLayoutWidget_3);
        ChBPrintAnonymousReceipts->setObjectName(QStringLiteral("ChBPrintAnonymousReceipts"));

        VLZtree->addWidget(ChBPrintAnonymousReceipts);

        LReplaceParticipantNames = new QLabel(verticalLayoutWidget_3);
        LReplaceParticipantNames->setObjectName(QStringLiteral("LReplaceParticipantNames"));
        LReplaceParticipantNames->setEnabled(false);

        VLZtree->addWidget(LReplaceParticipantNames);

        CBReplaceParticipantNames = new QComboBox(verticalLayoutWidget_3);
        CBReplaceParticipantNames->addItem(QString());
        CBReplaceParticipantNames->addItem(QString());
        CBReplaceParticipantNames->addItem(QString());
        CBReplaceParticipantNames->addItem(QString());
        CBReplaceParticipantNames->setObjectName(QStringLiteral("CBReplaceParticipantNames"));
        CBReplaceParticipantNames->setEnabled(false);
        CBReplaceParticipantNames->setEditable(true);

        VLZtree->addWidget(CBReplaceParticipantNames);

        ChBReceiptsForLocalClients = new QCheckBox(verticalLayoutWidget_3);
        ChBReceiptsForLocalClients->setObjectName(QStringLiteral("ChBReceiptsForLocalClients"));
        ChBReceiptsForLocalClients->setChecked(false);

        VLZtree->addWidget(ChBReceiptsForLocalClients);

        line_Receipts = new QFrame(verticalLayoutWidget_3);
        line_Receipts->setObjectName(QStringLiteral("line_Receipts"));
        line_Receipts->setFrameShape(QFrame::HLine);
        line_Receipts->setFrameShadow(QFrame::Sunken);

        VLZtree->addWidget(line_Receipts);

        ChBRamDisk = new QCheckBox(verticalLayoutWidget_3);
        ChBRamDisk->setObjectName(QStringLiteral("ChBRamDisk"));
        ChBRamDisk->setEnabled(true);

        VLZtree->addWidget(ChBRamDisk);

        ChBSessionWithoutAttachedClients = new QCheckBox(verticalLayoutWidget_3);
        ChBSessionWithoutAttachedClients->setObjectName(QStringLiteral("ChBSessionWithoutAttachedClients"));

        VLZtree->addWidget(ChBSessionWithoutAttachedClients);

        ChBautoStartClientZleaf = new QCheckBox(verticalLayoutWidget_3);
        ChBautoStartClientZleaf->setObjectName(QStringLiteral("ChBautoStartClientZleaf"));
        ChBautoStartClientZleaf->setChecked(true);

        VLZtree->addWidget(ChBautoStartClientZleaf);

        PBStartSession = new QPushButton(verticalLayoutWidget_3);
        PBStartSession->setObjectName(QStringLiteral("PBStartSession"));

        VLZtree->addWidget(PBStartSession);

        VSpacerZtree = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        VLZtree->addItem(VSpacerZtree);


        HLNewSession->addLayout(VLZtree);

        VLZleave = new QVBoxLayout();
        VLZleave->setSpacing(6);
        VLZleave->setObjectName(QStringLiteral("VLZleave"));
        L_Zleaves = new QLabel(verticalLayoutWidget_3);
        L_Zleaves->setObjectName(QStringLiteral("L_Zleaves"));
        L_Zleaves->setMargin(4);

        VLZleave->addWidget(L_Zleaves);

        LzLeafCommandline = new QLabel(verticalLayoutWidget_3);
        LzLeafCommandline->setObjectName(QStringLiteral("LzLeafCommandline"));

        VLZleave->addWidget(LzLeafCommandline);

        LEzLeafCommandline = new QLineEdit(verticalLayoutWidget_3);
        LEzLeafCommandline->setObjectName(QStringLiteral("LEzLeafCommandline"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(LEzLeafCommandline->sizePolicy().hasHeightForWidth());
        LEzLeafCommandline->setSizePolicy(sizePolicy4);

        VLZleave->addWidget(LEzLeafCommandline);

        PBStartzLeaf = new QPushButton(verticalLayoutWidget_3);
        PBStartzLeaf->setObjectName(QStringLiteral("PBStartzLeaf"));

        VLZleave->addWidget(PBStartzLeaf);

        line_StartZleaves = new QFrame(verticalLayoutWidget_3);
        line_StartZleaves->setObjectName(QStringLiteral("line_StartZleaves"));
        line_StartZleaves->setFrameShape(QFrame::HLine);
        line_StartZleaves->setFrameShadow(QFrame::Sunken);

        VLZleave->addWidget(line_StartZleaves);

        L_FakeName = new QLabel(verticalLayoutWidget_3);
        L_FakeName->setObjectName(QStringLiteral("L_FakeName"));
        L_FakeName->setEnabled(true);
        L_FakeName->setMaximumSize(QSize(16777215, 16777215));
        L_FakeName->setTextFormat(Qt::RichText);
        L_FakeName->setScaledContents(false);
        L_FakeName->setWordWrap(true);

        VLZleave->addWidget(L_FakeName);

        CBClientNames = new QComboBox(verticalLayoutWidget_3);
        CBClientNames->addItem(QString());
        CBClientNames->setObjectName(QStringLiteral("CBClientNames"));
        CBClientNames->setEditable(true);

        VLZleave->addWidget(CBClientNames);

        PBRunzLeaf = new QPushButton(verticalLayoutWidget_3);
        PBRunzLeaf->setObjectName(QStringLiteral("PBRunzLeaf"));

        VLZleave->addWidget(PBRunzLeaf);

        line_RunWithOtherName = new QFrame(verticalLayoutWidget_3);
        line_RunWithOtherName->setObjectName(QStringLiteral("line_RunWithOtherName"));
        line_RunWithOtherName->setFrameShape(QFrame::HLine);
        line_RunWithOtherName->setFrameShadow(QFrame::Sunken);

        VLZleave->addWidget(line_RunWithOtherName);

        L_LocalZleaf = new QLabel(verticalLayoutWidget_3);
        L_LocalZleaf->setObjectName(QStringLiteral("L_LocalZleaf"));

        VLZleave->addWidget(L_LocalZleaf);

        HLLocalzLeaf = new QHBoxLayout();
        HLLocalzLeaf->setSpacing(6);
        HLLocalzLeaf->setObjectName(QStringLiteral("HLLocalzLeaf"));
        PBStartLocalzLeaf = new QPushButton(verticalLayoutWidget_3);
        PBStartLocalzLeaf->setObjectName(QStringLiteral("PBStartLocalzLeaf"));

        HLLocalzLeaf->addWidget(PBStartLocalzLeaf);

        PBKillLocalzLeaf = new QPushButton(verticalLayoutWidget_3);
        PBKillLocalzLeaf->setObjectName(QStringLiteral("PBKillLocalzLeaf"));

        HLLocalzLeaf->addWidget(PBKillLocalzLeaf);


        VLZleave->addLayout(HLLocalzLeaf);

        VSpacerTemplates = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        VLZleave->addItem(VSpacerTemplates);


        HLNewSession->addLayout(VLZleave);


        VLNewSession->addLayout(HLNewSession);

        TWExperimenterTab->addTab(TSessionStart, QString());
        TAdminActions = new QWidget();
        TAdminActions->setObjectName(QStringLiteral("TAdminActions"));
        TAdminActions->setEnabled(false);
        verticalLayout_6 = new QVBoxLayout(TAdminActions);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        GBInformation = new QGroupBox(TAdminActions);
        GBInformation->setObjectName(QStringLiteral("GBInformation"));
        verticalLayout_7 = new QVBoxLayout(GBInformation);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        L_UserName = new QLabel(GBInformation);
        L_UserName->setObjectName(QStringLiteral("L_UserName"));

        verticalLayout_7->addWidget(L_UserName);

        L_AdministrativeRights = new QLabel(GBInformation);
        L_AdministrativeRights->setObjectName(QStringLiteral("L_AdministrativeRights"));

        verticalLayout_7->addWidget(L_AdministrativeRights);


        verticalLayout_6->addWidget(GBInformation);

        GBOptionsForAdminActions = new QGroupBox(TAdminActions);
        GBOptionsForAdminActions->setObjectName(QStringLiteral("GBOptionsForAdminActions"));
        verticalLayout_10 = new QVBoxLayout(GBOptionsForAdminActions);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        VLUseUser = new QVBoxLayout();
        VLUseUser->setSpacing(6);
        VLUseUser->setObjectName(QStringLiteral("VLUseUser"));
        L_UseUser = new QLabel(GBOptionsForAdminActions);
        L_UseUser->setObjectName(QStringLiteral("L_UseUser"));

        VLUseUser->addWidget(L_UseUser);

        HLUserRBs = new QHBoxLayout();
        HLUserRBs->setSpacing(6);
        HLUserRBs->setObjectName(QStringLiteral("HLUserRBs"));
        RBUseUserRoot = new QRadioButton(GBOptionsForAdminActions);
        RBUseUserRoot->setObjectName(QStringLiteral("RBUseUserRoot"));

        HLUserRBs->addWidget(RBUseUserRoot);

        RBUseLocalUser = new QRadioButton(GBOptionsForAdminActions);
        RBUseLocalUser->setObjectName(QStringLiteral("RBUseLocalUser"));

        HLUserRBs->addWidget(RBUseLocalUser);


        VLUseUser->addLayout(HLUserRBs);


        verticalLayout_10->addLayout(VLUseUser);


        verticalLayout_6->addWidget(GBOptionsForAdminActions);

        GBActionsWithSelectedClients = new QGroupBox(TAdminActions);
        GBActionsWithSelectedClients->setObjectName(QStringLiteral("GBActionsWithSelectedClients"));
        horizontalLayout_3 = new QHBoxLayout(GBActionsWithSelectedClients);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        PBOpenTerminal = new QPushButton(GBActionsWithSelectedClients);
        PBOpenTerminal->setObjectName(QStringLiteral("PBOpenTerminal"));

        horizontalLayout_3->addWidget(PBOpenTerminal);

        PBOpenFilesystem = new QPushButton(GBActionsWithSelectedClients);
        PBOpenFilesystem->setObjectName(QStringLiteral("PBOpenFilesystem"));

        horizontalLayout_3->addWidget(PBOpenFilesystem);


        verticalLayout_6->addWidget(GBActionsWithSelectedClients);

        GBExecuteOnEveryClient = new QGroupBox(TAdminActions);
        GBExecuteOnEveryClient->setObjectName(QStringLiteral("GBExecuteOnEveryClient"));
        verticalLayout_11 = new QVBoxLayout(GBExecuteOnEveryClient);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        CBCommandToExecute = new QComboBox(GBExecuteOnEveryClient);
        CBCommandToExecute->setObjectName(QStringLiteral("CBCommandToExecute"));
        CBCommandToExecute->setEditable(true);

        verticalLayout_11->addWidget(CBCommandToExecute);

        PBExecute = new QPushButton(GBExecuteOnEveryClient);
        PBExecute->setObjectName(QStringLiteral("PBExecute"));

        verticalLayout_11->addWidget(PBExecute);


        verticalLayout_6->addWidget(GBExecuteOnEveryClient);

        TWExperimenterTab->addTab(TAdminActions, QString());
        TInfo = new QWidget();
        TInfo->setObjectName(QStringLiteral("TInfo"));
        TInfo->setLayoutDirection(Qt::RightToLeft);
        VLInfo = new QVBoxLayout(TInfo);
        VLInfo->setSpacing(6);
        VLInfo->setContentsMargins(11, 11, 11, 11);
        VLInfo->setObjectName(QStringLiteral("VLInfo"));
        LInfo = new QLabel(TInfo);
        LInfo->setObjectName(QStringLiteral("LInfo"));
        LInfo->setAlignment(Qt::AlignCenter);

        VLInfo->addWidget(LInfo);

        TWExperimenterTab->addTab(TInfo, QString());

        verticalLayout_8->addWidget(TWExperimenterTab);

        TVClients = new QTableView(centralWidget);
        TVClients->setObjectName(QStringLiteral("TVClients"));
        TVClients->setEnabled(true);
        sizePolicy1.setHeightForWidth(TVClients->sizePolicy().hasHeightForWidth());
        TVClients->setSizePolicy(sizePolicy1);
        TVClients->setMinimumSize(QSize(0, 200));
        TVClients->setMaximumSize(QSize(16777215, 200));
        QFont font2;
        font2.setPointSize(8);
        TVClients->setFont(font2);
        TVClients->setAcceptDrops(false);
        TVClients->setLayoutDirection(Qt::LeftToRight);
        TVClients->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TVClients->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TVClients->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        TVClients->setAutoScroll(false);
        TVClients->setShowGrid(true);
        TVClients->setCornerButtonEnabled(false);
        TVClients->horizontalHeader()->setVisible(false);
        TVClients->horizontalHeader()->setCascadingSectionResizes(true);
        TVClients->horizontalHeader()->setDefaultSectionSize(115);
        TVClients->horizontalHeader()->setMinimumSectionSize(30);
        TVClients->horizontalHeader()->setStretchLastSection(false);
        TVClients->verticalHeader()->setVisible(false);
        TVClients->verticalHeader()->setDefaultSectionSize(35);
        TVClients->verticalHeader()->setMinimumSectionSize(25);
        TVClients->verticalHeader()->setStretchLastSection(false);

        verticalLayout_8->addWidget(TVClients);

        lc__MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(lc__MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        lc__MainWindow->setStatusBar(statusBar);

        retranslateUi(lc__MainWindow);

        TWExperimenterTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(lc__MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *lc__MainWindow)
    {
        lc__MainWindow->setWindowTitle(QApplication::translate("lc::MainWindow", "Labcontrol", nullptr));
        GBLocalActions->setTitle(QApplication::translate("lc::MainWindow", "Experiment preparation / server actions", nullptr));
        L_PlanSession->setText(QApplication::translate("lc::MainWindow", "Plan a session or print attendee list", nullptr));
        PBShowORSEE->setText(QApplication::translate("lc::MainWindow", "Show ORSEE", nullptr));
        L_PrintLocalFiles->setText(QApplication::translate("lc::MainWindow", "Print local files", nullptr));
        PBShowPreprints->setText(QApplication::translate("lc::MainWindow", "Show preprints", nullptr));
#ifndef QT_NO_TOOLTIP
        PBPrintPaymentFileManually->setToolTip(QApplication::translate("lc::MainWindow", "Choose a payment file manually, which will then be printed.", nullptr));
#endif // QT_NO_TOOLTIP
        PBPrintPaymentFileManually->setText(QApplication::translate("lc::MainWindow", "Print payment file manually", nullptr));
        L_WebcamChooser->setText(QApplication::translate("lc::MainWindow", "Show webcams", nullptr));
        CBWebcamChooser->setItemText(0, QApplication::translate("lc::MainWindow", "Choose webcam", nullptr));

        GBClientActions->setTitle(QApplication::translate("lc::MainWindow", "Client actions", nullptr));
        L_ClientBootShutdown->setText(QApplication::translate("lc::MainWindow", "Switch the selected clients on or off", nullptr));
#ifndef QT_NO_TOOLTIP
        PBShutdown->setToolTip(QApplication::translate("lc::MainWindow", "Turn off the selected clients.", nullptr));
#endif // QT_NO_TOOLTIP
        PBShutdown->setText(QApplication::translate("lc::MainWindow", "Shutdown", nullptr));
#ifndef QT_NO_TOOLTIP
        PBBoot->setToolTip(QApplication::translate("lc::MainWindow", "Boot the selected clients.", nullptr));
#endif // QT_NO_TOOLTIP
        PBBoot->setText(QApplication::translate("lc::MainWindow", "Boot", nullptr));
        L_VNC->setText(QApplication::translate("lc::MainWindow", "Remote Control selected clients", nullptr));
#ifndef QT_NO_TOOLTIP
        PBViewDesktopFullControl->setToolTip(QApplication::translate("lc::MainWindow", "Control mouse and keyboard", nullptr));
#endif // QT_NO_TOOLTIP
        PBViewDesktopFullControl->setText(QApplication::translate("lc::MainWindow", "Control desktop", nullptr));
#ifndef QT_NO_TOOLTIP
        PBViewDesktopViewOnly->setToolTip(QApplication::translate("lc::MainWindow", "View only", nullptr));
#endif // QT_NO_TOOLTIP
        PBViewDesktopViewOnly->setText(QApplication::translate("lc::MainWindow", "Show desktop", nullptr));
        label->setText(QApplication::translate("lc::MainWindow", "Control right mouse botton (RMB)", nullptr));
        PBEnableRMB->setText(QApplication::translate("lc::MainWindow", "Enable RMB", nullptr));
        PBDisableRMB->setText(QApplication::translate("lc::MainWindow", "Disable RMB", nullptr));
        LUploadFolder->setText(QApplication::translate("lc::MainWindow", "Upload folder to the selected clients", nullptr));
        PBChooseFile->setText(QApplication::translate("lc::MainWindow", "Choose folder", nullptr));
        PBBeamFile->setText(QApplication::translate("lc::MainWindow", "Start to upload folder", nullptr));
        L_BrowserActions->setText(QApplication::translate("lc::MainWindow", "Start browser on the selected clients", nullptr));
        L_URL->setText(QApplication::translate("lc::MainWindow", "URL:", nullptr));
#ifndef QT_NO_TOOLTIP
        LEURL->setToolTip(QApplication::translate("lc::MainWindow", "%CLIENT% is a placeholder for the hostname in the URL", nullptr));
#endif // QT_NO_TOOLTIP
        LEURL->setText(QApplication::translate("lc::MainWindow", "http://localhost:8000/%CLIENT%", nullptr));
        L_BrowserSelection->setText(QApplication::translate("lc::MainWindow", "Use browser:", nullptr));
        CB_BrowserSelection->setItemText(0, QApplication::translate("lc::MainWindow", "chromium", nullptr));
        CB_BrowserSelection->setItemText(1, QApplication::translate("lc::MainWindow", "firefox", nullptr));

        CBFullscreen->setText(QApplication::translate("lc::MainWindow", "Fullscreen (only FF)", nullptr));
        PBstartBrowser->setText(QApplication::translate("lc::MainWindow", "Start browser", nullptr));
        PBstopBrowser->setText(QApplication::translate("lc::MainWindow", "Stop browser", nullptr));
        TWExperimenterTab->setTabText(TWExperimenterTab->indexOf(TExperimenterActions), QApplication::translate("lc::MainWindow", "Experiment prepatation", nullptr));
        L_cleanupRecoverLastSession->setText(QApplication::translate("lc::MainWindow", "Cleanup / recover last session", nullptr));
        PBStopZtree->setText(QApplication::translate("lc::MainWindow", "Stop z-Tree", nullptr));
#ifndef QT_NO_TOOLTIP
        PBKillzLeaf->setToolTip(QApplication::translate("lc::MainWindow", "Stops zLeaf on all selected clients", nullptr));
#endif // QT_NO_TOOLTIP
        PBKillzLeaf->setText(QApplication::translate("lc::MainWindow", "Stop z-Leaf on selected clients", nullptr));
        PBRecoverCrashedSession->setText(QApplication::translate("lc::MainWindow", "Recover crashed session", nullptr));
        LNewSession->setText(QApplication::translate("lc::MainWindow", "New Session", nullptr));
        LZTreeVersion->setText(QApplication::translate("lc::MainWindow", "z-Tree version", nullptr));
        LDataTargetPath->setText(QApplication::translate("lc::MainWindow", "Data target path (No whitespaces allowed)", nullptr));
        LPort->setText(QApplication::translate("lc::MainWindow", "Port:", nullptr));
        LTemplateForReceipts->setText(QApplication::translate("lc::MainWindow", "Template for receipts", nullptr));
        ChBPrintAnonymousReceipts->setText(QApplication::translate("lc::MainWindow", "Print anonymous receipts", nullptr));
        LReplaceParticipantNames->setText(QApplication::translate("lc::MainWindow", "Replace participant names with:", nullptr));
        CBReplaceParticipantNames->setItemText(0, QApplication::translate("lc::MainWindow", "\\hspace{5cm}", nullptr));
        CBReplaceParticipantNames->setItemText(1, QApplication::translate("lc::MainWindow", "anonym", nullptr));
        CBReplaceParticipantNames->setItemText(2, QApplication::translate("lc::MainWindow", "anonymous", nullptr));
        CBReplaceParticipantNames->setItemText(3, QApplication::translate("lc::MainWindow", "nicht ausf\303\274llen", nullptr));

        ChBReceiptsForLocalClients->setText(QApplication::translate("lc::MainWindow", "Print receipt for local client", nullptr));
        ChBRamDisk->setText(QApplication::translate("lc::MainWindow", "Use ramdisk for gamesafe file", nullptr));
        ChBSessionWithoutAttachedClients->setText(QApplication::translate("lc::MainWindow", "Allow session without attached clients", nullptr));
        ChBautoStartClientZleaf->setText(QApplication::translate("lc::MainWindow", "Start z-Leaf on clients with default parameters", nullptr));
        PBStartSession->setText(QApplication::translate("lc::MainWindow", "Start session", nullptr));
        L_Zleaves->setText(QApplication::translate("lc::MainWindow", "z-Leaves", nullptr));
        LzLeafCommandline->setText(QApplication::translate("lc::MainWindow", "command line to be executed", nullptr));
#ifndef QT_NO_TOOLTIP
        PBStartzLeaf->setToolTip(QApplication::translate("lc::MainWindow", "Starts zLeaf on the selected clients with the port given in the 'zTree' groupbox.", nullptr));
#endif // QT_NO_TOOLTIP
        PBStartzLeaf->setText(QApplication::translate("lc::MainWindow", "Start z-Leaf on selected clients", nullptr));
        L_FakeName->setText(QApplication::translate("lc::MainWindow", "<html><head/><body><p>Run zLeaf with another name<br/>(select a single client):</p></body></html>", nullptr));
        CBClientNames->setItemText(0, QApplication::translate("lc::MainWindow", "Choose the name z-Leaf shall have", nullptr));

        CBClientNames->setCurrentText(QApplication::translate("lc::MainWindow", "Choose the name z-Leaf shall have", nullptr));
        PBRunzLeaf->setText(QApplication::translate("lc::MainWindow", "Run z-Leaf with chosen name", nullptr));
        L_LocalZleaf->setText(QApplication::translate("lc::MainWindow", "Local zLeaf", nullptr));
        PBStartLocalzLeaf->setText(QApplication::translate("lc::MainWindow", "Start local z-Leaf", nullptr));
        PBKillLocalzLeaf->setText(QApplication::translate("lc::MainWindow", "Stop local z-Leaf", nullptr));
        TWExperimenterTab->setTabText(TWExperimenterTab->indexOf(TSessionStart), QApplication::translate("lc::MainWindow", "Session actions", nullptr));
        GBInformation->setTitle(QApplication::translate("lc::MainWindow", "Information", nullptr));
        L_UserName->setText(QApplication::translate("lc::MainWindow", "TextLabel", nullptr));
        L_AdministrativeRights->setText(QApplication::translate("lc::MainWindow", "TextLabel", nullptr));
        GBOptionsForAdminActions->setTitle(QApplication::translate("lc::MainWindow", "Options for admin actions", nullptr));
        L_UseUser->setText(QApplication::translate("lc::MainWindow", "User to be used for remote terminal session:", nullptr));
        RBUseUserRoot->setText(QApplication::translate("lc::MainWindow", "roo&t", nullptr));
        RBUseLocalUser->setText(QApplication::translate("lc::MainWindow", "Client's user", nullptr));
        GBActionsWithSelectedClients->setTitle(QApplication::translate("lc::MainWindow", "Actions with selected clients", nullptr));
        PBOpenTerminal->setText(QApplication::translate("lc::MainWindow", "Open terminal", nullptr));
        PBOpenFilesystem->setText(QApplication::translate("lc::MainWindow", "Open filesystem", nullptr));
        GBExecuteOnEveryClient->setTitle(QApplication::translate("lc::MainWindow", "Execute command on selected clients", nullptr));
        PBExecute->setText(QApplication::translate("lc::MainWindow", "Execute", nullptr));
        TWExperimenterTab->setTabText(TWExperimenterTab->indexOf(TAdminActions), QApplication::translate("lc::MainWindow", "Admin actions", nullptr));
        LInfo->setText(QString());
        TWExperimenterTab->setTabText(TWExperimenterTab->indexOf(TInfo), QApplication::translate("lc::MainWindow", "Info", nullptr));
    } // retranslateUi

};

} // namespace lc

namespace lc {
namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui
} // namespace lc

#endif // UI_MAINWINDOW_H
