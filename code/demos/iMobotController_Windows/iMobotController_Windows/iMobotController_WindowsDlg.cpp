// iMobotController_WindowsDlg.cpp : implementation file
//
#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "iMobotController_Windows.h"
#include "iMobotController_WindowsDlg.h"
#include "configFileDialog.h"
//#include "TeachingDialog.h"
#include "gait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAXSPD 120
#define MAXSPDR 100

CRITICAL_SECTION UpdateGuiCriticalSection;
buttonState_t g_buttonState[B_NUMBUTTONS];
comboBoxState_t g_comboBoxState[4];

int getChHome(TCHAR *chhome);
HINSTANCE GotoURL(LPCTSTR url, int showcmd);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedIcon1();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//	ON_STN_CLICKED(IDI_ICON1, &CAboutDlg::OnStnClickedIcon1)
	ON_STN_CLICKED(IDI_ICON_ch, &CAboutDlg::OnStnClickedIcon1)
END_MESSAGE_MAP()


/* CiMobotController_WindowsDlg dialog */
CiMobotController_WindowsDlg::CiMobotController_WindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CiMobotController_WindowsDlg::IDD, pParent)
{
//	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_BAROBO);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_ch);
	m_numGaits = 0;
	isConnected = false;
	/* Initialize lists of controls */
	m_edit_MotorPositions[0] = &m_edit_Motor1Position;
	m_edit_MotorPositions[1] = &m_edit_Motor2Position;
	m_edit_MotorPositions[2] = &m_edit_Motor3Position;
//	m_edit_MotorPositions[3] = &m_edit_Motor4Position;
    m_edit_MotorSpeeds[0] = &m_edit_MotorSpeed1;
    m_edit_MotorSpeeds[1] = &m_edit_MotorSpeed2;
    m_edit_MotorSpeeds[2] = &m_edit_MotorSpeed3;
//    m_edit_MotorSpeeds[3] = &m_edit_MotorSpeed4;
	m_slider_Speeds[0] = &m_slider_Speed1;
	m_slider_Speeds[1] = &m_slider_Speed2;
	m_slider_Speeds[2] = &m_slider_Speed3;
//	m_slider_Speeds[3] = &m_slider_Speed4;
	m_slider_Positions[0] = &m_slider_Position1;
	m_slider_Positions[1] = &m_slider_Position2;
	m_slider_Positions[2] = &m_slider_Position3;
//	m_slider_Positions[3] = &m_slider_Position4;
	m_combo_SensorTypes[0] = &m_combo_Sensor1type;
	m_combo_SensorTypes[1] = &m_combo_Sensor2type;
	m_combo_SensorTypes[2] = &m_combo_Sensor3type;
	m_combo_SensorTypes[3] = &m_combo_Sensor4type;
	
	m_edit_SensorValues[0] = &m_edit_Sensor1value;
	m_edit_SensorValues[1] = &m_edit_Sensor2value;
	m_edit_SensorValues[2] = &m_edit_Sensor3value;
	m_edit_SensorValues[3] = &m_edit_Sensor4value;
  /* Initialize critical section */
  InitializeCriticalSection(&UpdateGuiCriticalSection);
  /* Initialize button states */
//  g_buttonState[B_PLAY].handlerFunc = &CiMobotController_WindowsDlg::OnBnClickedButtonrollback;
}

void CiMobotController_WindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_Motor1Forward, m_button_Motor1Forward);
	DDX_Control(pDX, IDC_BUTTON_Motor2Forward, m_button_Motor2Forward);
	DDX_Control(pDX, IDC_BUTTON_Motor3Forward, m_button_Motor3Forward);
	//	DDX_Control(pDX, IDC_BUTTON_Motor4Forward, m_button_Motor4Forward);
	DDX_Control(pDX, IDC_BUTTON_Motor1Stop, m_button_Motor1Stop);
	DDX_Control(pDX, IDC_BUTTON_Motor2Stop, m_button_Motor2Stop);
	DDX_Control(pDX, IDC_BUTTON_Motor3Stop, m_button_Motor3Stop);
	//	DDX_Control(pDX, IDC_BUTTON_Motor4Stop, m_button_Motor4Stop);
	DDX_Control(pDX, IDC_BUTTON_Motor1Backward, m_button_Motor1Backward);
	DDX_Control(pDX, IDC_BUTTON_Motor2Backward, m_button_Motor2Backward);
	DDX_Control(pDX, IDC_BUTTON_Motor3Backward, m_button_Motor3Backward);
	//	DDX_Control(pDX, IDC_BUTTON_Motor4Backward, m_button_Motor4Backward);
	//	DDX_Control(pDX, IDC_LIST_gaits, m_list_gaits);
	DDX_Control(pDX, IDC_EDIT_position1, m_edit_Motor1Position);
	DDX_Control(pDX, IDC_EDIT_position2, m_edit_Motor2Position);
	DDX_Control(pDX, IDC_EDIT_position3, m_edit_Motor3Position);
	//	DDX_Control(pDX, IDC_EDIT_position4, m_edit_Motor4Position);
	//DDX_Control(pDX, IDC_EDIT_address, m_edit_Address);
	//DDX_Control(pDX, IDC_EDIT_channel, m_edit_Channel);
	DDX_Control(pDX, IDC_SLIDER_speed1, m_slider_Speed1);
	DDX_Control(pDX, IDC_SLIDER_speed2, m_slider_Speed2);
	DDX_Control(pDX, IDC_SLIDER_speed3, m_slider_Speed3);
	//	DDX_Control(pDX, IDC_SLIDER_speed4, m_slider_Speed4);
	DDX_Control(pDX, IDC_SLIDER_position1, m_slider_Position1);
	DDX_Control(pDX, IDC_SLIDER_position2, m_slider_Position2);
	DDX_Control(pDX, IDC_SLIDER_position3, m_slider_Position3);
	//	DDX_Control(pDX, IDC_SLIDER_position4, m_slider_Position4);
	//DDX_Control(pDX, IDC_STATIC_IROBOT_PICTURE, m_staticicon_iMobotPicture);
	DDX_Control(pDX, IDC_BUTTON_rollForward, m_button_rollForward);
	DDX_Control(pDX, IDC_BUTTON_rollStop, m_button_rollStop);
	DDX_Control(pDX, IDC_BUTTON_rollBack, m_button_rollBack);
	DDX_Control(pDX, IDC_BUTTON_rollLeft, m_button_rollLeft);
	DDX_Control(pDX, IDC_BUTTON_rollRight, m_button_rollRight);
	DDX_Control(pDX, IDC_EDIT_speed1, m_edit_MotorSpeed1);
	DDX_Control(pDX, IDC_EDIT_speed2, m_edit_MotorSpeed2);
	DDX_Control(pDX, IDC_EDIT_speed3, m_edit_MotorSpeed3);
	//	DDX_Control(pDX, IDC_EDIT_speed4, m_edit_MotorSpeed4);
	DDX_Control(pDX, IDC_EDIT_setpos1, m_edit_setpos1);
	DDX_Control(pDX, IDC_EDIT_setpos2, m_edit_setpos2);
	DDX_Control(pDX, IDC_EDIT_setpos3, m_edit_setpos3);
	//	DDX_Control(pDX, IDC_EDIT_setpos4, m_edit_setpos4);
	DDX_Control(pDX, IDC_EDIT_setspd1, m_edit_setspd1);
	DDX_Control(pDX, IDC_EDIT_setspd2, m_edit_setspd2);
	DDX_Control(pDX, IDC_EDIT_setspd3, m_edit_setspd3);
	//	DDX_Control(pDX, IDC_EDIT_setspd4, m_edit_setspd4);
	DDX_Control(pDX, IDC_COMBO_Sensor1Type, m_combo_Sensor1type);
	DDX_Control(pDX, IDC_COMBO_Sensor2Type, m_combo_Sensor2type);
	DDX_Control(pDX, IDC_COMBO_Sensor3Type, m_combo_Sensor3type);
	DDX_Control(pDX, IDC_COMBO_Sensor4Type, m_combo_Sensor4type);
	DDX_Control(pDX, IDC_EDIT2, m_edit_Sensor1value);
	DDX_Control(pDX, IDC_EDIT3, m_edit_Sensor2value);
	DDX_Control(pDX, IDC_EDIT4, m_edit_Sensor3value);
	DDX_Control(pDX, IDC_EDIT5, m_edit_Sensor4value);
	DDX_Control(pDX, IDC_EDIT_SensorValue1, m_edit_Sensor1value);
	DDX_Control(pDX, IDC_EDIT_SensorValue2, m_edit_Sensor2value);
	DDX_Control(pDX, IDC_EDIT_SensorValue3, m_edit_Sensor3value);
	DDX_Control(pDX, IDC_EDIT_SensorValue4, m_edit_Sensor4value);
	/*
	DDX_Control(pDX, IDC_BUTTON_rotateLeft, m_button_rotateLeft);
	DDX_Control(pDX, IDC_BUTTON_rotateStop, m_button_rotateStop);
	DDX_Control(pDX, IDC_BUTTON_rotateRight, m_button_rotateRight);
	*/
	DDX_Control(pDX, IDC_BUTTON_walkForward, m_button_walkForward);
	DDX_Control(pDX, IDC_BUTTON_walkStop, m_button_walkStop);
	DDX_Control(pDX, IDC_BUTTON_walkBackward, m_button_walkBackward);
	DDX_Control(pDX, IDC_BUTTON_moveArmUp, m_botton_moveArmUp);
	DDX_Control(pDX, IDC_BUTTON_moveStop, m_botton_moveStop);
	DDX_Control(pDX, IDC_BUTTON_moveArmDown, m_botton_moveArmDown);
	DDX_Control(pDX, IDC_BUTTON_rotateArmLeft, m_botton_rotateArmLeft);
	DDX_Control(pDX, IDC_BUTTON_rotateArmRight, m_botton_rotateArmRight);
}

void CiMobotController_WindowsDlg::OnOK()
{
	CWnd* pwndCtrl = GetFocus();
	int ctrlID = pwndCtrl->GetDlgCtrlID();
	switch(ctrlID) {
		case IDC_EDIT_setpos1:
		case IDC_EDIT_setpos2:
		case IDC_EDIT_setpos3:
//		case IDC_EDIT_setpos4:
			OnBnClickedButtonGopos();
			return;
    case IDC_EDIT_setspd1:
    case IDC_EDIT_setspd2:
    case IDC_EDIT_setspd3:
//    case IDC_EDIT_setspd4:
      OnBnClickedButtonSetspd();
      return;
	}
	CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CiMobotController_WindowsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_WM_TIMER()
	//}}AFX_MSG_MAP
	
	ON_BN_CLICKED(IDC_BUTTON_Motor1Forward, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor1forward)
//	ON_BN_CLICKED(IDC_BUTTON_play, &CiMobotController_WindowsDlg::OnBnClickedButtonplay)
	ON_BN_CLICKED(IDC_BUTTON_connect, &CiMobotController_WindowsDlg::OnBnClickedButtonconnect)
//	ON_BN_CLICKED(IDC_BUTTON_Motor4Forward, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor4forward)
//	ON_LBN_SELCHANGE(IDC_LIST_gaits, &CiMobotController_WindowsDlg::OnLbnSelchangeListgaits)
//	ON_EN_CHANGE(IDC_EDIT_position4, &CiMobotController_WindowsDlg::OnEnChangeEditposition4)
	//ON_STN_CLICKED(IDC_STATIC_IROBOT_PICTURE, &CiMobotController_WindowsDlg::OnStnClickedStaticImobotPicture)
	ON_BN_CLICKED(IDC_BUTTON_Motor2Forward, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor2forward)
	ON_BN_CLICKED(IDC_BUTTON_Motor3Forward, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor3forward)
	ON_BN_CLICKED(IDC_BUTTON_Motor1Stop, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor1stop)
	ON_BN_CLICKED(IDC_BUTTON_Motor2Stop, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor2stop)
	ON_BN_CLICKED(IDC_BUTTON_Motor3Stop, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor3stop)
//	ON_BN_CLICKED(IDC_BUTTON_Motor4Stop, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor4stop)
	ON_BN_CLICKED(IDC_BUTTON_Motor1Backward, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor1backward)
	ON_BN_CLICKED(IDC_BUTTON_Motor2Backward, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor2backward)
	ON_BN_CLICKED(IDC_BUTTON_Motor3Backward, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor3backward)
//	ON_BN_CLICKED(IDC_BUTTON_Motor4Backward, &CiMobotController_WindowsDlg::OnBnClickedButtonMotor4backward)
	ON_BN_CLICKED(IDC_BUTTON_rollForward, &CiMobotController_WindowsDlg::OnBnClickedButtonrollforward)
	ON_BN_CLICKED(IDC_BUTTON_rollStop, &CiMobotController_WindowsDlg::OnBnClickedButtonrollstop)
	ON_BN_CLICKED(IDC_BUTTON_rollLeft, &CiMobotController_WindowsDlg::OnBnClickedButtonrollleft)
	ON_BN_CLICKED(IDC_BUTTON_rollRight, &CiMobotController_WindowsDlg::OnBnClickedButtonrollright)
	ON_BN_CLICKED(IDC_BUTTON_rollBack, &CiMobotController_WindowsDlg::OnBnClickedButtonrollback)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_position2, &CiMobotController_WindowsDlg::OnNMCustomdrawSliderposition2)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_position2, &CiMobotController_WindowsDlg::OnTRBNThumbPosChangingSliderposition2)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_position1, &CiMobotController_WindowsDlg::OnTRBNThumbPosChangingSliderposition1)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_position3, &CiMobotController_WindowsDlg::OnTRBNThumbPosChangingSliderposition3)
//	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_position4, &CiMobotController_WindowsDlg::OnTRBNThumbPosChangingSliderposition4)
	ON_COMMAND(ID_ROBOT_CONFIGUREROBOTBLUETOOTH, &CiMobotController_WindowsDlg::OnRobotConfigurerobotbluetooth)
	ON_BN_CLICKED(IDC_BUTTON_GOPOS, &CiMobotController_WindowsDlg::OnBnClickedButtonGopos)
	ON_COMMAND(ID_ROBOT_CONNECTTOAROBOT, &CiMobotController_WindowsDlg::OnRobotConnecttoarobot)
	ON_COMMAND(ID_CONNECT_DISCONNECTFROMROBOT, &CiMobotController_WindowsDlg::OnConnectDisconnectfromrobot)
	ON_COMMAND(ID_HELP_HELP, &CiMobotController_WindowsDlg::OnHelpHelp)
	ON_COMMAND(ID_HELP_ABOUTROBOTCONTROLLER, &CiMobotController_WindowsDlg::OnHelpAboutrobotcontroller)
	ON_COMMAND(ID_FILE_EXIT, &CiMobotController_WindowsDlg::OnFileExit)
	ON_BN_CLICKED(IDC_BUTTON_MOVETOZERO, &CiMobotController_WindowsDlg::OnBnClickedButtonMovetozero)
	ON_BN_CLICKED(IDC_BUTTON_SETSPD, &CiMobotController_WindowsDlg::OnBnClickedButtonSetspd)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CiMobotController_WindowsDlg::OnBnClickedButtonMove)
	ON_COMMAND(ID_HELP_DEMOS, &CiMobotController_WindowsDlg::OnHelpDemos)
//	ON_COMMAND(ID_TEACHINGMODE_TEACHINGMODEDIALOG, &CiMobotController_WindowsDlg::OnTeachingmodeTeachingmodedialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_position1, &CiMobotController_WindowsDlg::OnNMCustomdrawSliderposition1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_speed1, &CiMobotController_WindowsDlg::OnNMCustomdrawSliderspeed1)
	ON_CBN_SELCHANGE(IDC_COMBO_Sensor1Type, &CiMobotController_WindowsDlg::OnCbnSelchangeComboSensor1type)
ON_CBN_SELCHANGE(IDC_COMBO_Sensor2Type, &CiMobotController_WindowsDlg::OnCbnSelchangeComboSensor2type)
ON_CBN_SELCHANGE(IDC_COMBO_Sensor3Type, &CiMobotController_WindowsDlg::OnCbnSelchangeComboSensor3type)
ON_CBN_SELCHANGE(IDC_COMBO_Sensor4Type, &CiMobotController_WindowsDlg::OnCbnSelchangeComboSensor4type)
/*
	ON_CBN_EDITUPDATE(IDC_COMBO_Sensor1Type, &CiMobotController_WindowsDlg::OnCbnEditupdateComboSensor1type)
	ON_CBN_EDITUPDATE(IDC_COMBO_Sensor2Type, &CiMobotController_WindowsDlg::OnCbnEditupdateComboSensor2type)
	ON_CBN_EDITUPDATE(IDC_COMBO_Sensor3Type, &CiMobotController_WindowsDlg::OnCbnEditupdateComboSensor3type)
	ON_CBN_EDITUPDATE(IDC_COMBO_Sensor4Type, &CiMobotController_WindowsDlg::OnCbnEditupdateComboSensor4type)
	*/
	ON_BN_CLICKED(IDC_BUTTON_RESETTOZERO, &CiMobotController_WindowsDlg::OnBnClickedButtonResettozero)
	/*
	ON_BN_CLICKED(IDC_BUTTON_rotateRight, &CiMobotController_WindowsDlg::OnBnClickedButtonrotateright)
	ON_BN_CLICKED(IDC_BUTTON_rotateLeft, &CiMobotController_WindowsDlg::OnBnClickedButtonrotateleft)
	ON_BN_CLICKED(IDC_BUTTON_rotateStop, &CiMobotController_WindowsDlg::OnBnClickedButtonrotatestop)
	*/
	ON_BN_CLICKED(IDC_BUTTON_walkForward, &CiMobotController_WindowsDlg::OnBnClickedButtonwalkforward)
	ON_BN_CLICKED(IDC_BUTTON_walkStop, &CiMobotController_WindowsDlg::OnBnClickedButtonwalkstop)
	ON_BN_CLICKED(IDC_BUTTON_walkBackward, &CiMobotController_WindowsDlg::OnBnClickedButtonwalkbackward)
	ON_STN_CLICKED(IDB_STATIC_MACHINE, &CiMobotController_WindowsDlg::OnStnClickedStaticMachine)
	ON_BN_CLICKED(IDC_BUTTON_moveArmUp, &CiMobotController_WindowsDlg::OnBnClickedButtonmovearmup)
	ON_BN_CLICKED(IDC_BUTTON_rotateArmLeft, &CiMobotController_WindowsDlg::OnBnClickedButtonrotatearmleft)
	ON_BN_CLICKED(IDC_BUTTON_moveStop, &CiMobotController_WindowsDlg::OnBnClickedButtonmovestop)
	ON_BN_CLICKED(IDC_BUTTON_rotateArmRight, &CiMobotController_WindowsDlg::OnBnClickedButtonrotatearmright)
	ON_BN_CLICKED(IDC_BUTTON_moveArmDown, &CiMobotController_WindowsDlg::OnBnClickedButtonmovearmdown)
	ON_BN_CLICKED(IDC_RADIO_clawOpen, &CiMobotController_WindowsDlg::OnBnClickedRadioclawopen)
	ON_BN_CLICKED(IDC_RADIO_clawClose, &CiMobotController_WindowsDlg::OnBnClickedRadioclawclose)
END_MESSAGE_MAP()


// CiMobotController_WindowsDlg message handlers

BOOL CiMobotController_WindowsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	/* Set button icon(s) */
	InitIcons();
	/* Initialize the Gait list */
	//InitGaits();
	/* initialize the sensor combo box */
	InitSensorComboBox();
	/* Init the timer */
	SetTimer(
		1,
		500,
		(TIMERPROC) NULL );
	/* Initialize the sliders */
	InitSliders();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CiMobotController_WindowsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CiMobotController_WindowsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CiMobotController_WindowsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CiMobotController_WindowsDlg::InitIcons()
{
	m_button_Motor1Forward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_up));
	m_button_Motor2Forward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_up));
	m_button_Motor3Forward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_up));
//	m_button_Motor4Forward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_up));

	m_button_Motor1Stop.SetIcon( AfxGetApp()->LoadIconW(IDI_ICON_stop));
	m_button_Motor2Stop.SetIcon( AfxGetApp()->LoadIconW(IDI_ICON_stop));
	m_button_Motor3Stop.SetIcon( AfxGetApp()->LoadIconW(IDI_ICON_stop));
//	m_button_Motor4Stop.SetIcon( AfxGetApp()->LoadIconW(IDI_ICON_stop));

	m_button_Motor1Backward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_down));
	m_button_Motor2Backward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_down));
	m_button_Motor3Backward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_down));
//	m_button_Motor4Backward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_down));

  m_button_rollForward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_up));
  m_button_rollLeft.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_left));
  m_button_rollRight.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_right));
  m_button_rollStop.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_stop));
  m_button_rollBack.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_down));
/*
  m_button_rotateLeft.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_left));
  m_button_rotateStop.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_stop));
  m_button_rotateRight.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_right));
  */
  m_button_walkForward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_up));
  m_button_walkStop.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_stop));
  m_button_walkBackward.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_down));
  
	m_botton_moveArmUp.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_up));
	m_botton_rotateArmLeft.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_left));
	m_botton_rotateArmRight.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_right));
	m_botton_moveStop.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_stop));
	m_botton_moveArmDown.SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_down));
}

#define SET_ANGLES(angles, a, b, c, d) \
	angles[0] = a; \
	angles[1] = b; \
	angles[2] = c; \
	angles[3] = d

/*
void CiMobotController_WindowsDlg::InitGaits()
{
	// Add some pre-programmed gaits into the robot
	Gait* gait;
	gait = new Gait(L"Arch");
	addGait(gait);
	gait = new Gait(L"Inchworm Left");
	addGait(gait);
	gait = new Gait(L"Inchworm Right");
	addGait(gait);
	gait = new Gait(L"Roll Backward");
	addGait(gait);
	gait = new Gait(L"Roll Forward");
	addGait(gait);
	gait = new Gait(L"Skinny");
	addGait(gait);
	gait = new Gait(L"Stand");
	addGait(gait);
	gait = new Gait(L"Turn Left");
	addGait(gait);
	gait = new Gait(L"Turn Right");
	addGait(gait);
	gait = new Gait(L"Tumble Left");
	addGait(gait);
	gait = new Gait(L"Tumble Right");
	addGait(gait);
	gait = new Gait(L"Unstand");
	addGait(gait);

	// Add the names to the listbox
	for(int i = 0; i < m_numGaits; i++) {
		m_list_gaits.AddString(m_gaits[i]->getName());
	}
}*/

int CiMobotController_WindowsDlg::addGait(Gait* gait)
{
	m_gaits[m_numGaits] = gait;
	m_numGaits++;
	return 0;
}

/*
void CiMobotController_WindowsDlg::OnBnClickedButtonplay()
{
  g_buttonState[B_PLAY].clicked = 1;
}
*/
#if 0
void CiMobotController_WindowsDlg::handlerPlay()
{
	/* Get the index of the gait to play */
	int index;
	index = m_list_gaits.GetCurSel();
	if(index == LB_ERR) {
		return;
	}
  iMobotComms.moveContinuousNB(
      ROBOT_HOLD,
      ROBOT_HOLD,
      ROBOT_HOLD,
      ROBOT_HOLD);

	iMobotComms.setJointSpeeds(45, 45, 45, 45);
  switch(index) {
    case 0: iMobotComms.motionArch(90); break;
    case 1: iMobotComms.motionInchwormLeft(1); break;
    case 2: iMobotComms.motionInchwormRight(1); break;
    case 3: iMobotComms.motionRollBackward(360); break;
    case 4: iMobotComms.motionRollForward(360); break;
    case 5: iMobotComms.motionSkinny(90); break;
    case 6: iMobotComms.motionStand(); break;
    case 7: iMobotComms.motionTurnLeft(360); break;
    case 8: iMobotComms.motionTurnRight(360); break;
    case 9: iMobotComms.motionTumbleLeft(1); break;
    case 10: iMobotComms.motionTumbleRight(1); break;
    case 11: iMobotComms.motionUnstand(); break;
    default: break;
  }
}
#endif

afx_msg void CiMobotController_WindowsDlg::OnTimer(UINT nIDEvent)
{
	/* Call base class handler */
	CDialog::OnTimer(nIDEvent);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonconnect()
{
	int i;
	TCHAR msgbuf[512];
	/* "Connect" button clicked */
//	if(i = iMobotComms.connect()) {
	if(i = nxtComms.connect()){
//	if(i = nxtComms.connectWithAddress("00:16:53:12:e7:80", 1)){
		if (i == -3) {
			MessageBox( TEXT("Address format incorrect."), TEXT("Error"), MB_OK | MB_ICONINFORMATION );
		} else if (i == -4) {
			MessageBox( TEXT("No configured robots. Add robots in the \"Configure -> Configure Robot Bluetooth\" dialog."), TEXT("Error"), MB_OK | MB_ICONINFORMATION );
		} else if (i == -5) {
			MessageBox( TEXT("A bluetooth device could not be found on this computer. You may need to attach an external Bluetooth dongle to continue."), TEXT("Error"), MB_OK | MB_ICONINFORMATION );
    } else {
		/* Error connecting */
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL );
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		//_stprintf(msgbuf, TEXT("%d"), WSAGetLastError());
		if(WSAGetLastError() == 10048) {
			_stprintf(msgbuf, TEXT("%s\nMake sure there are no other programs currently connected to the Mobot."),(LPTSTR)lpMsgBuf);
		} else {
			_tcscpy(msgbuf, (LPTSTR)lpMsgBuf);
		}
		//MessageBox( (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK | MB_ICONINFORMATION );
		MessageBox( (LPCTSTR)msgbuf, TEXT("Error"), MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
		//MessageBox(L"Error connecting to robot.", L"Error");
		}
	} else {
		isConnected = true;
		/* Update the positions of all the sliders and such */
		UpdateSliders();
//		UpdateSensorValues();
    /* Start the handler thread */
    CreateThread(
        NULL,
        0,
		(LPTHREAD_START_ROUTINE)(HandlerThread),
        this,
        0,
        NULL );
	}
}

void CiMobotController_WindowsDlg::InitSliders()
{
	/*
	for(int i = 0; i <= 3; i=i+3) {
//		m_slider_Speeds[i]->SetRange(0, MAXSPD, TRUE);
//		m_slider_Speeds[i]->SetPos(MAXSPD - 45);
//		m_slider_Positions[i]->SetRange(-180, 180, TRUE);
		m_slider_Positions[i]->SetPos(0);
	}
	for(int i = 1; i < 3; i++) {
		m_slider_Speeds[i]->SetRange(0, MAXSPD, TRUE);
		m_slider_Speeds[i]->SetPos(MAXSPD - 45);
		m_slider_Positions[i]->SetRange(-90, 90, TRUE);
		m_slider_Positions[i]->SetPos(0);
	}*/
	for(int i = 0; i <3; i++){
		m_slider_Speeds[i]->SetRange(0, MAXSPDR, TRUE);
		m_slider_Speeds[i]->SetPos(MAXSPDR - 25);
		m_slider_Positions[i]->SetRange(0, 360, TRUE);
		m_slider_Positions[i]->SetPos(0);
	}
}

void CiMobotController_WindowsDlg::UpdateSliders()
{
	double position;
//	double speedRatio;
	int speed;
/*	
	double speed;

	for(int i = 0; i < 4; i++) {
		iMobotComms.getJointAngle((robotJointId_t)(i+1), position);
		m_slider_Positions[i]->SetPos( (int) -position );
		m_positions[i] = (int) position;
		speed = 45;
		iMobotComms.setJointSpeed((robotJointId_t)(i+1), speed);
		m_slider_Speeds[i]->SetPos( MAXSPD - speed );
		m_speeds[i] = speed;
    wchar_t buf[200];
    swprintf(buf, L"%lf", speed);
    m_edit_MotorSpeeds[i]->SetWindowTextW(buf);
	}*/
	for(int i = 0; i < 3; i++) {
		nxtComms.getJointAngle((nxtJointId_t)i, position);
		Sleep(200);
		m_slider_Positions[i]->SetPos( 360 -((int)position % 360));
//		m_slider_Positions[i]->SetPos( (int)-position);
		m_positions[i] = (int) position;
		speed = 45;
		nxtComms.setJointSpeed((nxtJointId_t)i, speed/100.0);
		m_slider_Speeds[i]->SetPos( MAXSPD - speed );
		m_speeds[i] = speed;
		wchar_t buf[200];
//		swprintf(buf, L"%lf", speed);
		swprintf(buf, L"%lf", position);
//		m_edit_MotorSpeeds[i]->SetWindowTextW(buf);
		m_edit_MotorPositions[i]->SetWindowTextW(buf);
	}
	for(int i = 0; i< 3; i++){
		/*
		nxtComms.getJointAngle((nxtJointId_t)i, position);
		m_slider_Positions[i]->SetPos( (int) -position);
		m_positions[i] = (int) position;
		*/
//		speedRatio = 0.25;
		speed = 25;
		m_slider_Speeds[i]->SetPos( MAXSPDR - speed);
		m_speedRatios[i] = speed/100.0;
		wchar_t buf[200];
		swprintf(buf, L"%.2lf", speed/100.0);
		m_edit_MotorSpeeds[i]->SetWindowTextW(buf);
	}
}

void CiMobotController_WindowsDlg::UpdateSpeedSliders(int i, double speed)
//void CiMobotController_WindowsDlg::UpdateSpeedSliders(int i, double speedRatio)
{
	/*
		m_slider_Speeds[i]->SetPos( MAXSPD - speed);
		m_speeds[i] = speed;
    TCHAR buf[200];
    _stprintf(buf, TEXT("%lf"), speed);
    m_edit_MotorSpeeds[i]->SetWindowText(buf);*/

	m_slider_Speeds[i]->SetPos( MAXSPDR - speed);
//	m_slider_Speeds[i]->SetPos( speed);
	m_speedRatios[i] = speed/100.0;
	TCHAR buf[200];
	_stprintf(buf, TEXT("%.2lf"), speed/100.0);
	m_edit_MotorSpeeds[i]->SetWindowTextW(buf);
}

void CiMobotController_WindowsDlg::OnLbnSelchangeListgaits()
{
	// TODO: Add your control notification handler code here
}

int CiMobotController_WindowsDlg::poseJoints(const double *angles, unsigned char motorMask)
{
	for(int i = 0; i < 4; i++) {
		if(motorMask & (1<<i)) {
			iMobotComms.moveJointToNB((robotJointId_t)(i+1), angles[i]);
		}
	}
	return 0;
}

int CiMobotController_WindowsDlg::moveJoints(const double *angles, unsigned char motorMask)
{
	double pos;
	for(int i = 0; i < 4; i++) {
		if(motorMask & (1<<i)) {
			/* Get the motor position first */
			iMobotComms.getJointAngle((robotJointId_t)(i+1), pos);
			/* Set the motor to an offset position */
			iMobotComms.moveJointToNB((robotJointId_t)(i+1), pos + angles[i]);
		}
	}
	return 0;
}

void CiMobotController_WindowsDlg::OnStnClickedStaticImobotPicture()
{
	// TODO: Add your control notification handler code here
}

void CiMobotController_WindowsDlg::OnBnClickedButtonMotor1forward()
{
  g_buttonState[B_M1F].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM1F()
{
	/* get the speed from the slider control */
	int speed;
	speed = m_slider_Speed1.GetPos();
//	iMobotComms.setJointSpeed(ROBOT_JOINT1, (double)speed);
    nxtComms.setJointSpeedRatio(NXT_JOINT1, (MAXSPDR - speed)/100.0);
	nxtComms.moveJointContinuousNB(NXT_JOINT1, NXT_FORWARD);
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT1, ROBOT_FORWARD);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonMotor2forward()
{
  g_buttonState[B_M2F].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM2F()
{
	/* get the speed from the slider control */
	int speed;
	speed = m_slider_Speed2.GetPos();
//	iMobotComms.setJointSpeed(ROBOT_JOINT2, (double)speed);
	nxtComms.setJointSpeedRatio(NXT_JOINT2, (MAXSPDR -speed)/100.0);
	nxtComms.moveJointContinuousNB(NXT_JOINT2, NXT_FORWARD);
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT2, ROBOT_FORWARD);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonMotor3forward()
{
  g_buttonState[B_M3F].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM3F()
{
	/* get the speed from the slider control */
	int speed;
	speed = m_slider_Speed3.GetPos();
//	iMobotComms.setJointSpeed(ROBOT_JOINT3, (double)speed);
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT3, ROBOT_FORWARD);
//	speedRatio = m_slider_Speed3.GetPos();
	nxtComms.setJointSpeedRatio(NXT_JOINT3, (MAXSPDR - speed)/100.0);
	nxtComms.moveJointContinuousNB(NXT_JOINT3, NXT_FORWARD);
}
/*
void CiMobotController_WindowsDlg::OnBnClickedButtonMotor4forward()
{
  g_buttonState[B_M4F].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM4F()
{
	// get the speed from the slider control
	int speed;
	speed = m_slider_Speed4.GetPos();
	iMobotComms.setJointSpeed(ROBOT_JOINT4, (double)speed);
	iMobotComms.moveJointContinuousNB(ROBOT_JOINT4, ROBOT_FORWARD);
}
*/
void CiMobotController_WindowsDlg::OnBnClickedButtonMotor1stop()
{
  g_buttonState[B_M1S].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM1S()
{
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT1, ROBOT_NEUTRAL);
	nxtComms.stopOneJoint(NXT_JOINT1);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonMotor2stop()
{
  g_buttonState[B_M2S].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM2S()
{
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT2, ROBOT_NEUTRAL);
	nxtComms.stopOneJoint(NXT_JOINT2);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonMotor3stop()
{
  g_buttonState[B_M3S].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM3S()
{
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT3, ROBOT_NEUTRAL);
	nxtComms.stopOneJoint(NXT_JOINT3);
}
/*
void CiMobotController_WindowsDlg::OnBnClickedButtonMotor4stop()
{
  g_buttonState[B_M4S].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM4S()
{
	iMobotComms.moveJointContinuousNB(ROBOT_JOINT4, ROBOT_NEUTRAL);
}
*/
void CiMobotController_WindowsDlg::OnBnClickedButtonMotor1backward()
{
  g_buttonState[B_M1B].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM1B()
{
	/* get the speed from the slider control */
	int speed;
	speed = m_slider_Speed1.GetPos();
//	iMobotComms.setJointSpeed(ROBOT_JOINT1, (double)speed);
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT1, ROBOT_BACKWARD);
	nxtComms.setJointSpeedRatio(NXT_JOINT1, (MAXSPDR - speed)/100.0);
	nxtComms.moveJointContinuousNB(NXT_JOINT1, NXT_BACKWARD);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonMotor2backward()
{
  g_buttonState[B_M2B].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM2B()
{
	/* get the speed from the slider control */
	int speed;
	speed = m_slider_Speed2.GetPos();
//	iMobotComms.setJointSpeed(ROBOT_JOINT2, (double)speed);
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT2, ROBOT_BACKWARD);
	nxtComms.setJointSpeedRatio(NXT_JOINT2, (MAXSPDR - speed)/100.0);
	nxtComms.moveJointContinuousNB(NXT_JOINT2, NXT_BACKWARD);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonMotor3backward()
{
  g_buttonState[B_M3B].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM3B()
{
	/* get the speed from the slider control */
	int speed;
	speed = m_slider_Speed3.GetPos();
//	iMobotComms.setJointSpeed(ROBOT_JOINT3, (double)speed);
//	iMobotComms.moveJointContinuousNB(ROBOT_JOINT3, ROBOT_BACKWARD);
	nxtComms.setJointSpeedRatio(NXT_JOINT3, (MAXSPDR - speed)/100.0);
	nxtComms.moveJointContinuousNB(NXT_JOINT3, NXT_BACKWARD);
}
/*
void CiMobotController_WindowsDlg::OnBnClickedButtonMotor4backward()
{
  g_buttonState[B_M4B].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerM4B()
{
	// get the speed from the slider control
	int speed;
	speed = m_slider_Speed4.GetPos();
	iMobotComms.setJointSpeed(ROBOT_JOINT4, (double)speed);
	iMobotComms.moveJointContinuousNB(ROBOT_JOINT4, ROBOT_BACKWARD);
}
*/
void CiMobotController_WindowsDlg::OnBnClickedButtonrollforward()
{
  g_buttonState[B_FORWARD].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerFORWARD()
{
//  iMobotComms.motionRollForwardNB(360);
	nxtComms.vehicleRollForwardNB(360);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonrollstop()
{
  g_buttonState[B_STOP].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerSTOP()
{
//  iMobotComms.stop();
//	nxtComms.stopTwoJoints(NXT_JOINT2, NXT_JOINT3);
	nxtComms.stopAllJoints();
}

void CiMobotController_WindowsDlg::OnBnClickedButtonrollleft()
{
  g_buttonState[B_LEFT].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerLEFT()
{
//  iMobotComms.motionTurnLeftNB(360);
	nxtComms.vehicleRotateLeftNB(360);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonrollright()
{
  g_buttonState[B_RIGHT].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerRIGHT()
{
//  iMobotComms.motionTurnRightNB(360);
	nxtComms.vehicleRotateRightNB(360);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonrollback()
{
  g_buttonState[B_BACK].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerBACK()
{
//  iMobotComms.motionRollBackwardNB(360);
	nxtComms.vehicleRollBackwardNB(360);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonResettozero()
{
	g_buttonState[B_RESETTOZERO].clicked = 1; 
}

void CiMobotController_WindowsDlg::handlerRESETTOZERO()
{
	nxtComms.setToZeros();
	return;
}
void CiMobotController_WindowsDlg::OnBnClickedButtonMovetozero()
{
  g_buttonState[B_MOVETOZERO].clicked = 1; 
}

void CiMobotController_WindowsDlg::handlerMOVETOZERO()
{
//  iMobotComms.moveToZeroNB();
	nxtComms.moveToZeroNB();
}

void CiMobotController_WindowsDlg::OnNMCustomdrawSliderposition2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CiMobotController_WindowsDlg::OnTRBNThumbPosChangingSliderposition1(NMHDR *pNMHDR, LRESULT *pResult)
{
  MessageBox(L"Error connecting to iMobot.", L"Error");
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
  g_buttonState[S_M1P].clicked = 1;
}

void CiMobotController_WindowsDlg::OnTRBNThumbPosChangingSliderposition2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
  MessageBox(L"Error connecting to iMobot.", L"Error");
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
  g_buttonState[S_M2P].clicked = 1;
}

void CiMobotController_WindowsDlg::OnTRBNThumbPosChangingSliderposition3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
  MessageBox(L"Error connecting to iMobot.", L"Error");
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
  g_buttonState[S_M3P].clicked = 1;
}

/*
void CiMobotController_WindowsDlg::OnTRBNThumbPosChangingSliderposition4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
  MessageBox(L"Error connecting to iMobot.", L"Error");
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
  g_buttonState[S_M4P].clicked = 1;
}
*/

DWORD WINAPI HandlerThread(void* arg)
{
  static double lastPosition[4];
  static int lastPositionCount[3];
  static int lastSpeed[4];
  static int initialized = 0;
  static int lastClawStatus = 0;

  CiMobotController_WindowsDlg* dlg;
  CMobot *mobot;
  ChNXT *nxt;
  dlg = (CiMobotController_WindowsDlg*) arg;
  mobot = &dlg->iMobotComms;
  nxt = &dlg->nxtComms;
  /*
  // Initialize values 
  for(int i = 0; i < 4; i++) {
    lastPosition[i] = -dlg->m_slider_Positions[i]->GetPos();
    lastSpeed[i] = dlg->m_slider_Speeds[i]->GetPos();
  }
  while(1) {
    // Perform iMobot update actions 
    if(mobot->isConnected() == false) {
      Sleep(250);
      continue;
    }
	if(!initialized) {
		mobot->setJointSpeeds(45, 45, 45, 45);
	}
    double value;
    wchar_t buf[200];
    // Check the Speed and Position sliders. If any of them have moved, send the appropriate message to the iMobot.
    double position;
    int speed;
    for(int i = 0; i < 4; i++) {
      mobot->getJointAngle((robotJointId_t)(i+1), value);
      swprintf(buf, L"%lf", value);
      EnterCriticalSection(&UpdateGuiCriticalSection);
      dlg->m_edit_MotorPositions[i]->SetWindowTextW(buf);
      // See if the position slider has been clicked 
      position = -dlg->m_slider_Positions[i]->GetPos();
      if(lastPosition[i] != position) {
        if(initialized) {
          mobot->moveJointToNB((robotJointId_t)(i+1), (double) position);
        }
        g_buttonState[S_M1P + i].clicked = 0;
        lastPosition[i] = position;
      } else {
        // Move the slider into position 
        dlg->m_slider_Positions[i]->SetPos((int) -value);
        lastPosition[i] = -dlg->m_slider_Positions[i]->GetPos();
      }

      // Check the speed 
      speed = MAXSPD - dlg->m_slider_Speeds[i]->GetPos();
      if(speed != dlg->m_speeds[i]) {
        mobot->setJointSpeed((robotJointId_t)(i+1), (double)speed);
        dlg->m_speeds[i] = speed;
        swprintf(buf, L"%lf", (double)(speed));
        dlg->m_edit_MotorSpeeds[i]->SetWindowTextW(buf);
      }
      LeaveCriticalSection(&UpdateGuiCriticalSection);
    }
    if(!initialized) {
      mobot->stop();
      initialized = 1;
      continue;
    }*/
  while(1){
  if(nxt->isConnected() == false){
	  Sleep(250);
	  continue;
  }
  	if(!initialized) {
		nxt->setJointSpeedRatios(0.25, 0.25, 0.25);
	}
    double value;
    wchar_t buf[200];
    // Check the Speed and Position sliders. If any of them have moved, send the appropriate message to the iMobot.
    double position;
	int positionCount;
    int speed;
    for(int i = 0; i < 3; i++) {
      nxt->getJointAngle((nxtJointId_t)i, value);
	  positionCount = value/360;
      swprintf(buf, L"%lf", value);
      EnterCriticalSection(&UpdateGuiCriticalSection);
      dlg->m_edit_MotorPositions[i]->SetWindowTextW(buf);
      // See if the position slider has been clicked 
      position = 360 - dlg->m_slider_Positions[i]->GetPos();
      if(lastPosition[i] != position) {
        if(initialized) {
//          nxt->moveJointToNB((nxtJointId_t)i, (double) (position+positionCount*360));
			nxt->moveJointNB((nxtJointId_t)i, (double)(position - lastPosition[i]));
        }
        g_buttonState[S_M1P + i].clicked = 0;
        lastPosition[i] = position;
		lastPositionCount[i] = positionCount;
      } else {
        // Move the slider into position 
		  if( value < 0){
			  dlg->m_slider_Positions[i]->SetPos(360 + (int)value%360);
		  }else{
			  dlg->m_slider_Positions[i]->SetPos(360 - (int)value%360);
		  }
		  lastPosition[i] = 360 - dlg->m_slider_Positions[i]->GetPos();
      }

      // Check the speed 
      speed = MAXSPDR - dlg->m_slider_Speeds[i]->GetPos();
      if(speed != dlg->m_speeds[i]) {
        nxt->setJointSpeedRatio((nxtJointId_t)i, speed/100.0);
        dlg->m_speeds[i] = speed;
        swprintf(buf, L"%lf", speed/100.0);
        dlg->m_edit_MotorSpeeds[i]->SetWindowTextW(buf);
      }
      LeaveCriticalSection(&UpdateGuiCriticalSection);
    }
    if(!initialized) {
      nxt->stopAllJoints();
      initialized = 1;
      continue;
    }
	/* Handle any combo box changes */
	for(int i = 0; i < 4; i++){
		int nIndex = dlg->m_combo_SensorTypes[i]->GetCurSel();
		if(nIndex != 6){
			dlg->UpdateSensorValues();
//			g_comboBoxState[i].changed = 0;
		}
	}
    /* Handle any button presses */
    for(int i = 0; i < B_NUMBUTTONS; i++) {
      if(g_buttonState[i].clicked) {
        switch(i) {
			/*
          case B_PLAY: 
			  dlg->handlerPlay(); 
			  break;
			  */
          case B_M1F: 
			  dlg->handlerM1F(); 
			  break;
          case B_M2F: 
			  dlg->handlerM2F(); 
			  break;
          case B_M3F: 
			  dlg->handlerM3F(); 
			  break;
//        case B_M4F: dlg->handlerM4F(); break;
          case B_M1S: 
			  dlg->handlerM1S(); 
			  break;
          case B_M2S: 
			  dlg->handlerM2S(); 
			  break;
          case B_M3S: 
			  dlg->handlerM3S(); 
			  break;
//        case B_M4S: dlg->handlerM4S(); break;
          case B_M1B: 
			  dlg->handlerM1B(); 
			  break;
          case B_M2B: 
			  dlg->handlerM2B(); 
			  break;
          case B_M3B: 
			  dlg->handlerM3B(); 
			  break;
//        case B_M4B: dlg->handlerM4B(); break;
          case B_SETPOS: 
			  dlg->handlerSETPOS(); 
			  break;
          case B_MOVE: 
			  dlg->handlerMOVE(); 
			  break;                         
          case B_SETSPD: 
			  dlg->handlerSETSPD(); 
			  break;
          case B_FORWARD: 
			  dlg->handlerFORWARD(); 
			  break;
          case B_STOP:
		  case B_ARMSTOP:
		  case B_WALKSTOP:
			  dlg->handlerSTOP(); 
			  break;
          case B_LEFT: 
			  dlg->handlerLEFT(); 
			  break;
          case B_RIGHT: 
			  dlg->handlerRIGHT(); 
			  break;
          case B_BACK: 
			  dlg->handlerBACK(); 
			  break;
		  case B_WALKFOR:
			  dlg->handlerWALKFOR();
			  break;
		  case B_WALKBACK:
			  dlg->handlerWALKBACK();
			  break;
		  case B_ARMUP:
			  dlg->handlerARMUP();
			  break;
		  case B_ARMDOWN:
			  dlg->handlerARMDOWN();
			  break;
		  case B_ARMLEFT:
			  dlg->handlerARMLEFT();
			  break;
		  case B_ARMRIGHT:
			  dlg->handlerARMRIGHT();
			  break;
		  case B_CLAWOPEN:
			  if(!lastClawStatus){
				  dlg->handlerCLAWOPEN();
				  lastClawStatus = !lastClawStatus;
			  }
			  break;
		  case B_CLAWCLOSE:
			  if(lastClawStatus){
				  dlg->handlerCLAWCLOSE();
				  lastClawStatus = !lastClawStatus;
			  }
			  break;
          case B_RESETTOZERO: 
			  dlg->handlerRESETTOZERO(); 
			  break;
          case B_MOVETOZERO: 
			  dlg->handlerMOVETOZERO();
			  break;
          default: 
			  break;
        }
        g_buttonState[i].clicked = 0;
      }
    }
  }
}

void CiMobotController_WindowsDlg::OnRobotConfigurerobotbluetooth()
{
  INT_PTR nRet = -1;
  ConfigFileDialog cfDialog;
  nRet = cfDialog.DoModal();
  char path[MAX_PATH];
  switch(nRet) {
    case IDOK:
      break;
    case IDABORT:
    case IDCANCEL:   
    default:  
      1+1;
  }
}

void CiMobotController_WindowsDlg::OnBnClickedButtonGopos()
{
	g_buttonState[B_SETPOS].clicked = 1;
	return;
}

void CiMobotController_WindowsDlg::handlerSETPOS()
{
	/* Get strings for all of the edit boxes containing values */
	TCHAR str[80];
	double pos;
	int len;
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setpos1.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//		iMobotComms.moveJointToNB(ROBOT_JOINT1, pos);
		nxtComms.moveJointToNB(NXT_JOINT1, pos);
	}
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setpos2.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//    if((pos < -90) || (pos > 90)) {
//      MessageBox(L"Error: Joint 2 position setting beyond join limits.");
//    }
//		iMobotComms.moveJointToNB(ROBOT_JOINT2, pos);
		nxtComms.moveJointToNB(NXT_JOINT2, pos);
	}
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setpos3.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//    if((pos < -90) || (pos > 90)) {
//      MessageBox(L"Error: Joint 3 position setting beyond join limits.");
//    }
//		iMobotComms.moveJointToNB(ROBOT_JOINT3, pos);
		nxtComms.moveJointToNB(NXT_JOINT3, pos);
	}
	/*
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setpos4.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
		iMobotComms.moveJointToNB(ROBOT_JOINT4, pos);
	}
	*/
}

void CiMobotController_WindowsDlg::OnBnClickedButtonMove()
{
  g_buttonState[B_MOVE].clicked = 1;
  return;
}

void CiMobotController_WindowsDlg::handlerMOVE()
{
	/* Get strings for all of the edit boxes containing values */
	TCHAR str[80];
	double pos;
	int len;
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setpos1.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//		iMobotComms.moveJointNB(ROBOT_JOINT1, pos);
		nxtComms.moveJointNB(NXT_JOINT1, pos);
	}
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setpos2.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//		iMobotComms.moveJointNB(ROBOT_JOINT2, pos);
		nxtComms.moveJointNB(NXT_JOINT2, pos);
	}
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setpos3.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//		iMobotComms.moveJointNB(ROBOT_JOINT3, pos);
		nxtComms.moveJointNB(NXT_JOINT3, pos);
	}
	memset(str, 0, sizeof(TCHAR)*80);
	/*
	len = m_edit_setpos4.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
		iMobotComms.moveJointNB(ROBOT_JOINT4, pos);
	}
	*/
}

void CiMobotController_WindowsDlg::OnRobotConnecttoarobot()
{ 
  /* Just activate the same handler as the "connect" button */
  OnBnClickedButtonconnect();
}

void CiMobotController_WindowsDlg::OnConnectDisconnectfromrobot()
{
//  iMobotComms.disconnect();
	nxtComms.disconnect();
}

void CiMobotController_WindowsDlg::OnHelpHelp()
{
  USES_CONVERSION;
  TCHAR chHome[MAX_PATH];
  TCHAR command[MAX_PATH];
  getChHome(chHome);
  _stprintf(command, TEXT("file://%s\\package\\chnxt\\docs\\index.html"), chHome);
  //sprintf(command, "file://%s\\package\\chmobot\\docs\\index.html", "C:\\Ch");
  GotoURL(command, 0);
  //system(command);
}

void CiMobotController_WindowsDlg::OnHelpAboutrobotcontroller()
{
  CAboutDlg dlgAbout;
  dlgAbout.DoModal();
}

void CiMobotController_WindowsDlg::OnFileExit()
{
//  iMobotComms.disconnect();
	nxtComms.disconnect();
    EndDialog(0);
}

void CAboutDlg::OnStnClickedIcon1()
{
	// TODO: Add your control notification handler code here
}

int getChHome(TCHAR *chhome)
{
  int retval = 0;
  USES_CONVERSION;
  /* the following code will work for 9x/NT */
  DWORD chHomeDirLen = MAX_PATH;
  HKEY hkResult, hStartKey = HKEY_LOCAL_MACHINE;
  LONG nResult;
  LONG NTversionLen = 128;
  unsigned char NTversion[128];

/* CHHOME set in registry Windows 95/98, during installation */
#if defined(_WIN32)
  if(!getenv("CHHOME")) 
  {
    /* the following code will work for 9x/NT */
#if defined(_WIN64)
      nResult = RegOpenKeyEx(hStartKey, TEXT("SOFTWARE\\SoftIntegration"), 0L, KEY_READ | KEY_WOW64_32KEY, &hkResult);
#else
      nResult = RegOpenKeyEx(hStartKey, TEXT("SOFTWARE\\SoftIntegration"), 0L, KEY_READ, &hkResult);
#endif
    if (ERROR_SUCCESS == nResult)
    {
       nResult = RegQueryValueEx(hkResult, TEXT("CHHOME"), 0 ,0, 
                                 (LPBYTE)chhome, &chHomeDirLen);
       if (ERROR_SUCCESS != nResult) { /* should not reach here */
/*
         fprintf(stderr, "ERROR: register value CHHOME cannot be obtained from registry\n");
*/
         retval = -1;
       }
       RegCloseKey(hkResult);
    }
    else { 
/*
      fprintf(stderr, "ERROR: cannot obtain a value for CHHOME\n");
      fprintf(stderr, "       you may set it using 'option.chhome' in Embedded Ch API Ch_Initialize2(&interp, option)\n");
*/
    }
  }
  else
    _tcscpy(chhome, A2T(getenv("CHHOME")));
#endif
  return retval;
}


LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
    TCHAR data[MAX_PATH];
    RegQueryValue(hkey, NULL, data, &datasize);
    lstrcpy(retdata,data);
    RegCloseKey(hkey);
    }

    return retval;
}


// "GotoURL" function by Stuart Patterson
// As seen in the August, 1997 Windows Developer's Journal.
HINSTANCE GotoURL(LPCTSTR url, int showcmd)
{
    USES_CONVERSION;
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) {

        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
            lstrcat(key, _T("\\shell\\open\\command"));

            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
                TCHAR *pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {                     // No quotes found
                    pos = (TCHAR*)strstr((const char*)key, (char *)_T("%1"));       // Check for %1, without quotes
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);
                result = (HINSTANCE) WinExec(T2A(key),showcmd);
            }
        }
  }

    return result;
}


void CiMobotController_WindowsDlg::OnBnClickedButtonSetspd()
{
  g_buttonState[B_SETSPD].clicked = 1;
  return;
}

void CiMobotController_WindowsDlg::handlerSETSPD()
{
	/* Get strings for all of the edit boxes containing values */
	TCHAR str[80];
	double pos;
	int len;
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setspd1.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//		iMobotComms.setJointSpeed(ROBOT_JOINT1, pos);
		nxtComms.setJointSpeedRatio(NXT_JOINT1, pos);
		UpdateSpeedSliders(0, pos*100.0);
	}
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setspd2.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//		iMobotComms.setJointSpeed(ROBOT_JOINT2, pos);
		nxtComms.setJointSpeedRatio(NXT_JOINT2, pos);
    UpdateSpeedSliders(1, pos*100.0);
	}
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setspd3.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
//		iMobotComms.setJointSpeed(ROBOT_JOINT3, pos);
		nxtComms.setJointSpeedRatio(NXT_JOINT3, pos);
    UpdateSpeedSliders(2, pos*100.0);
	}
	/*
	memset(str, 0, sizeof(TCHAR)*80);
	len = m_edit_setspd4.GetLine(0, str, 79);
	if(len > 0) {
		_stscanf(str, TEXT("%lf"), &pos);
		iMobotComms.setJointSpeed(ROBOT_JOINT4, pos);
    UpdateSpeedSliders(3, pos);
	}
	*/
}

void CiMobotController_WindowsDlg::OnHelpDemos()
{
  USES_CONVERSION;
  TCHAR chHome[MAX_PATH];
  TCHAR command[MAX_PATH];
  getChHome(chHome);
  _stprintf(command, TEXT("%s\\bin\\chide.exe -d %s\\package\\chnxt\\demos %s\\package\\chnxt\\demos\\start.ch"), chHome, chHome, chHome);
  //_stprintf(command, TEXT("file://%s\\package\\chmobot\\docs\\index.html"), chHome);
  //GotoURL(command, 0);
  //system(T2A(command));
  
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  CreateProcess(
	  NULL,
	  command,
      NULL,
      NULL,
      FALSE,
      0,
      NULL,
      NULL,
      &si,
      &pi );
}
/*
void CiMobotController_WindowsDlg::OnTeachingmodeTeachingmodedialog()
{
	CTeachingDialog dialog;
    g_teachingDialog = &dialog;
	dialog.DoModal();
}
*/

void CiMobotController_WindowsDlg::OnNMCustomdrawSliderposition1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CiMobotController_WindowsDlg::OnNMCustomdrawSliderspeed1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CiMobotController_WindowsDlg::InitSensorComboBox()
{
	for(int i = 0; i<4; i++){
		m_combo_SensorTypes[i]->InsertString(0, L"Touch");
		m_combo_SensorTypes[i]->InsertString(1, L"Light On");
		m_combo_SensorTypes[i]->InsertString(2, L"Light Off");
		m_combo_SensorTypes[i]->InsertString(3, L"Sound (DB)");
		m_combo_SensorTypes[i]->InsertString(4, L"Sound (DBA)");
		m_combo_SensorTypes[i]->InsertString(5, L"Ultrasonic");
		m_combo_SensorTypes[i]->InsertString(6, L"Color");
		m_combo_SensorTypes[i]->InsertString(7, L"No Sensor");

		/* set "No Sensor" as the default */
		m_combo_SensorTypes[i]->SetCurSel(7);
	}
}

void CiMobotController_WindowsDlg::UpdateSensorValues()
{
	int value = 0;

	for(int i = 0; i < 4; i++) {
		if(m_combo_SensorTypes[i]->GetCurSel() != 7){
			nxtComms.getSensor((nxtSensorId_t)i, value);
			Sleep(200);
			wchar_t buf[200];
			swprintf(buf, L"%d", value);
			m_edit_SensorValues[i]->SetWindowTextW(buf);
		}
	}
	/*
	int nIndex = m_combo_Sensor1type.GetCurSel();
			wchar_t buf[200];
			swprintf(buf, L"%d", nIndex);
			m_edit_SensorValues[0]->SetWindowTextW(buf);
			*/
}

/*
void CiMobotController_WindowsDlg::OnCbnEditupdateComboSensor1type()
{
	//UpdateData();
//	g_comboBoxState[0].changed = 1;
	handlerSETSENSOR(0);
	return;
}

void CiMobotController_WindowsDlg::OnCbnEditupdateComboSensor2type()
{
	//UpdateData();
//	g_comboBoxState[1].changed = 1;
	handlerSETSENSOR(1);
	return;
}

void CiMobotController_WindowsDlg::OnCbnEditupdateComboSensor3type()
{
	//UpdateData();
//	g_comboBoxState[2].changed = 1;
	handlerSETSENSOR(2);
	return;
}

void CiMobotController_WindowsDlg::OnCbnEditupdateComboSensor4type()
{
	//UpdateData();
//	g_comboBoxState[3].changed = 1;
	handlerSETSENSOR(3);
	return;
}
*/

void CiMobotController_WindowsDlg::OnCbnSelchangeComboSensor1type()
{
//	g_comboBoxState[1].changed = 1;
	handlerSETSENSOR(0);
	/*
	CString str;
	m_combo_Sensor1type.GetWindowText(str);
	wchar_t buf[200];
	swprintf(buf, L"%s", str);
	m_edit_SensorValues[0]->SetWindowTextW(buf);
	*/
	return;
}

void CiMobotController_WindowsDlg::OnCbnSelchangeComboSensor2type()
{
//	g_comboBoxState[1].changed = 1;
	handlerSETSENSOR(1);
	return;
}

void CiMobotController_WindowsDlg::OnCbnSelchangeComboSensor3type()
{
	handlerSETSENSOR(2);
	return;
}

void CiMobotController_WindowsDlg::OnCbnSelchangeComboSensor4type()
{
	handlerSETSENSOR(3);
	return;
}

void CiMobotController_WindowsDlg::handlerSETSENSOR(int i)
{
	int nIndex = m_combo_SensorTypes[i]->GetCurSel();
	/*
	wchar_t buf[200];
	swprintf(buf, L"%d", nIndex);
	m_edit_SensorValues[i]->SetWindowTextW(buf);
	*/
	switch(nIndex)
	{
	case 0:
		nxtComms.setSensor((nxtSensorId_t)i, 
					NXT_SENSORTYPE_TOUCH, NXT_SENSORMODE_BOOLEANMODE);
		break;
	case 1:
		nxtComms.setSensor((nxtSensorId_t)i, 
					NXT_SENSORTYPE_LIGHT_ACTIVE, NXT_SENSORMODE_PCTFULLSCALEMODE);
		break;
	case 2:
		nxtComms.setSensor((nxtSensorId_t)i, 
					NXT_SENSORTYPE_LIGHT_INACTIVE, NXT_SENSORMODE_PCTFULLSCALEMODE);
		break;
	case 3:
		nxtComms.setSensor((nxtSensorId_t)i, 
					NXT_SENSORTYPE_SOUND_DB, NXT_SENSORMODE_PCTFULLSCALEMODE);
		break;
	case 4:
		nxtComms.setSensor((nxtSensorId_t)i, 
					NXT_SENSORTYPE_SOUND_DBA, NXT_SENSORMODE_PCTFULLSCALEMODE);
		break;
	case 5:
		nxtComms.setSensor((nxtSensorId_t)i, 
					NXT_SENSORTYPE_ULTRASONIC, NXT_SENSORMODE_RAWMODE);
		break;
	case 6:
		nxtComms.setSensor((nxtSensorId_t)i, 
					NXT_SENSORTYPE_COLORFULL, NXT_SENSORMODE_RAWMODE);
		break;
	default:
		break;
	}

	return;
}
/*
void CiMobotController_WindowsDlg::OnBnClickedButtonrotateright()
{
  g_buttonState[B_M1F].clicked = 1;
}

void CiMobotController_WindowsDlg::OnBnClickedButtonrotateleft()
{
  g_buttonState[B_M1B].clicked = 1;
}

void CiMobotController_WindowsDlg::OnBnClickedButtonrotatestop()
{
	g_buttonState[B_M1S].clicked = 1;
}
*/
void CiMobotController_WindowsDlg::OnBnClickedButtonwalkforward()
{
	g_buttonState[B_WALKFOR].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerWALKFOR()
{
	nxtComms.humanoidWalkForward(360);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonwalkstop()
{
	g_buttonState[B_WALKSTOP].clicked = 1;
}

void CiMobotController_WindowsDlg::OnBnClickedButtonwalkbackward()
{
	g_buttonState[B_WALKBACK].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerWALKBACK()
{
	nxtComms.humanoidWalkBackward(360);
}



void CiMobotController_WindowsDlg::OnStnClickedStaticMachine()
{
	// TODO: Add your control notification handler code here
}

void CiMobotController_WindowsDlg::OnBnClickedButtonmovearmup()
{
	g_buttonState[B_ARMUP].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerARMUP()
{
	nxtComms.moveJoint(NXT_JOINT2, 360.0);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonrotatearmleft()
{
	g_buttonState[B_ARMLEFT].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerARMLEFT()
{
	nxtComms.moveJoint(NXT_JOINT3, 360.0);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonmovestop()
{
	g_buttonState[B_ARMSTOP].clicked = 1;
}

void CiMobotController_WindowsDlg::OnBnClickedButtonrotatearmright()
{
	g_buttonState[B_ARMRIGHT].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerARMRIGHT()
{
	nxtComms.moveJoint(NXT_JOINT3, -360.0);
}

void CiMobotController_WindowsDlg::OnBnClickedButtonmovearmdown()
{
	g_buttonState[B_ARMDOWN].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerARMDOWN()
{
	nxtComms.moveJoint(NXT_JOINT2, -360.0);
}

void CiMobotController_WindowsDlg::OnBnClickedRadioclawopen()
{
	g_buttonState[B_CLAWOPEN].clicked = 1;
}

void CiMobotController_WindowsDlg::handlerCLAWOPEN()
{
	nxtComms.moveJointContinuousNB(NXT_JOINT1, NXT_BACKWARD);
    Sleep(1000);
    nxtComms.stopOneJoint(NXT_JOINT1);

	return;
}

void CiMobotController_WindowsDlg::OnBnClickedRadioclawclose()
{
	g_buttonState[B_CLAWCLOSE].clicked = 1;

	return;
}

void CiMobotController_WindowsDlg::handlerCLAWCLOSE()
{
	nxtComms.moveJointContinuousNB(NXT_JOINT1, NXT_BACKWARD);
    Sleep(1000);
    nxtComms.stopOneJoint(NXT_JOINT1);

	return;
}