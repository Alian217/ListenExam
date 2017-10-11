
// ListenExamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mmsystem.h"
#include "ListenExam.h"
#include "ListenExamDlg.h"
#include "afxdialogex.h"
#include <io.h> 
#include <locale>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/****************************************************
*                模块中的全局变量                    *
****************************************************/
//配置文件管理结构体(文件名拆开管理)
INFO_CONFIG stInfoConfig;

//答案管理结构体
INFO_ANSWER stInfoAnswer;

//exe执行文件路径，也是声音和图片的路径
char PathExeFile[MAX_PATH] = {0};
int ilenPathExeFile =0;

//播放的正确内容声音文件名
char AnswerWavfilename[MAX_PATH] = {0};
int ilenAnswerWavfilename = 0;
//错误的其他三个答案选项声音文件名
char WrongWavfilename1[MAX_PATH] = {0};
int ilenWrongWavfilename1 = 0;
char WrongWavfilename2[MAX_PATH] = {0};
int ilenWrongWavfilename2 = 0;
char WrongWavfilename3[MAX_PATH] = {0};
int ilenWrongWavfilename3 = 0;

//音调总数，就是固定4
int SUM_TYPE_YINGDIAO = 4;

//已点击读音标志位，如果没有读音选择无效
bool bRead = false;

//第几次回答标志位
int iAnswerTimes = 0;

//总题数记录变量
int iSumQuestion = 0;

//第一到第四次回答问题正确记录变量
int iSumFirstRight = 0;
int iSumSecondRight = 0;
int iSumThirdRight = 0;
int iSumForthRight = 0;

//不回答放弃记录变量
bool bGiveUp = false;

//错误四个放弃
int iSumFourWrongGiveUp = 0;
bool bWrongGiveUp = false;

//找到正确答案标志位
bool bFindRightAnswer = false;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CListenExamDlg 对话框




CListenExamDlg::CListenExamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListenExamDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListenExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CListenExamDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_NEXT_QUESTION, &CListenExamDlg::OnBnClickedNextQuestion)
	ON_BN_CLICKED(IDC_PLAY_AUDIO, &CListenExamDlg::OnBnClickedPlayAudio)
	ON_BN_CLICKED(IDC_A, &CListenExamDlg::OnBnClickedA)
	ON_BN_CLICKED(IDC_B, &CListenExamDlg::OnBnClickedB)
	ON_BN_CLICKED(IDC_C, &CListenExamDlg::OnBnClickedC)
	ON_BN_CLICKED(IDC_D, &CListenExamDlg::OnBnClickedD)
END_MESSAGE_MAP()


// CListenExamDlg 消息处理程序

BOOL CListenExamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码 alian:改到AlphabetExam项目上
	memset( &stInfoConfig, '\0', sizeof(INFO_CONFIG) );
	memset( &stInfoAnswer, '\0', sizeof(INFO_ANSWER) );
	ReadConfigFile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CListenExamDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CListenExamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CListenExamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CListenExamDlg::OnBnClickedNextQuestion()
{
	// TODO: 在此添加控件通知处理程序代码
	int RandIndex1=-1;
	int RandIndex2=-1;
	int RandIndex3=-1;
	int RandIndex4=-1;
 	int Random[MAX_PATH] = {0};
 	int iTempOffset=0;
	char szTempBuf[MAX_PATH] = {0};

	//如果没有回答直接想放弃 或者 四次错误内，无法切换到下一题
	if ( bGiveUp )
	{
		//Alian: 考虑使用语音提示请重新完成此题
		return;
	}

	//每一次按下一题按钮都清
	memset( &stInfoAnswer, '\0', sizeof(INFO_ANSWER) );

	//获取当前执行、图片、声音文件路径
	memset(PathExeFile, '\0', sizeof(PathExeFile));
	ilenPathExeFile = GetCurrentDirectory( MAX_PATH, PathExeFile );
	if ( ilenPathExeFile < 0 )
	{
		AfxMessageBox(_T("无法获取当前执行文件路径!"));
		//Alian: 考虑使用语音提示
		return;
	}

	//随机获取4组XXX_X格式的文件名，不可重复,其中第一组为正确答案，作为答案的XXX为字母序号 X为音调
	srand((unsigned)time(NULL));
	//取得第一个下标，作为答案
	while ( true )
	{
		//现随机获取
		RandIndex1 = (rand()%(stInfoConfig.iSumCurrent*SUM_TYPE_YINGDIAO));
		//确保不同于其他三个随机数
		if ( RandIndex1!=RandIndex2 && RandIndex1!=RandIndex3 && RandIndex1!=RandIndex4  )
		{
			break;
		}
	}
	//取得第二个到第三个下标作为其他三个答案
	while ( true )
	{
		//现随机获取
		RandIndex2 = (rand()%(stInfoConfig.iSumCurrent*SUM_TYPE_YINGDIAO));
		//确保不同于其他三个随机数
		if ( RandIndex2!=RandIndex1 && RandIndex2!=RandIndex3 && RandIndex2!=RandIndex4  )
		{
			break;
		}
	}
	while ( true )
	{
		//现随机获取
		RandIndex3 = (rand()%(stInfoConfig.iSumCurrent*SUM_TYPE_YINGDIAO));
		//确保不同于其他三个随机数
		if ( RandIndex3!=RandIndex1 && RandIndex3!=RandIndex2 && RandIndex3!=RandIndex4  )
		{
			break;
		}
	}
	while ( true )
	{
		//现随机获取
		RandIndex4 = (rand()%(stInfoConfig.iSumCurrent*SUM_TYPE_YINGDIAO));
		//确保不同于其他三个随机数
		if ( RandIndex4!=RandIndex1 && RandIndex4!=RandIndex2 && RandIndex4!=RandIndex3  )
		{
			break;
		}
	}

	iTempOffset = 0;
	//正确答案存在对应对应答案管理结构体第一个元素里面
	memcpy( stInfoAnswer.stFileNameAnswer[0].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex1].szZiMu, strlen(stInfoConfig.stFileName[RandIndex1].szZiMu) );
	iTempOffset += strlen(stInfoConfig.stFileName[RandIndex1].szZiMu);
	memcpy( stInfoAnswer.stFileNameAnswer[0].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex1].szNo, strlen(stInfoConfig.stFileName[RandIndex1].szNo) );

	//错误答案存在对应对应答案管理结构体其他几个元素里面
	iTempOffset = 0;
	memcpy( stInfoAnswer.stFileNameAnswer[1].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex2].szZiMu, strlen(stInfoConfig.stFileName[RandIndex2].szZiMu) );
	iTempOffset += strlen(stInfoConfig.stFileName[RandIndex2].szZiMu);
	memcpy( stInfoAnswer.stFileNameAnswer[1].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex2].szNo, strlen(stInfoConfig.stFileName[RandIndex2].szNo) );
	iTempOffset = 0;
	memcpy( stInfoAnswer.stFileNameAnswer[2].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex3].szZiMu, strlen(stInfoConfig.stFileName[RandIndex3].szZiMu) );
	iTempOffset += strlen(stInfoConfig.stFileName[RandIndex3].szZiMu);
	memcpy( stInfoAnswer.stFileNameAnswer[2].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex3].szNo, strlen(stInfoConfig.stFileName[RandIndex3].szNo) );
	iTempOffset = 0;
	memcpy( stInfoAnswer.stFileNameAnswer[3].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex4].szZiMu, strlen(stInfoConfig.stFileName[RandIndex4].szZiMu) );
	iTempOffset += strlen(stInfoConfig.stFileName[RandIndex4].szZiMu);
	memcpy( stInfoAnswer.stFileNameAnswer[3].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex4].szNo, strlen(stInfoConfig.stFileName[RandIndex4].szNo) );

	RandIndex1 = RandIndex2 = RandIndex3 = RandIndex4 = 0;

	//获取SUM_TYPE_YINGDIAO范围内的SumGet随机数，保存在Random数组中，不可重复(false)
	GetRandomNumber( Random, SUM_TYPE_YINGDIAO, 4, false );

	//给答案管理结构体排序答案 A-1 B-2 C-3 D-4
	stInfoAnswer.iIndex = Random[0]+1;
	stInfoAnswer.stFileNameAnswer[0].iRandNo = Random[0]+1;
	stInfoAnswer.stFileNameAnswer[1].iRandNo = Random[1]+1;
	stInfoAnswer.stFileNameAnswer[2].iRandNo = Random[2]+1;
	stInfoAnswer.stFileNameAnswer[3].iRandNo = Random[3]+1;

	//显示对应的字符到对应选项上
	for ( iTempOffset=0; iTempOffset<SUM_TYPE_YINGDIAO; iTempOffset++ )
	{
		memset( szTempBuf, '\0', sizeof(szTempBuf) );
		switch ( stInfoAnswer.stFileNameAnswer[iTempOffset].iRandNo )
		{
		case 1:
			sprintf( szTempBuf,"%s.bmp",stInfoAnswer.stFileNameAnswer[iTempOffset].szZiMu_No );
			DisplayPic( IDC_PIC_A, szTempBuf, strlen(szTempBuf) );
			break;
		case 2:
			sprintf( szTempBuf,"%s.bmp",stInfoAnswer.stFileNameAnswer[iTempOffset].szZiMu_No );
			DisplayPic( IDC_PIC_B, szTempBuf, strlen(szTempBuf) );
			break;
		case 3:
			sprintf( szTempBuf,"%s.bmp",stInfoAnswer.stFileNameAnswer[iTempOffset].szZiMu_No );
			DisplayPic( IDC_PIC_C, szTempBuf, strlen(szTempBuf) );
			break;
		case 4:
			sprintf( szTempBuf,"%s.bmp",stInfoAnswer.stFileNameAnswer[iTempOffset].szZiMu_No );
			DisplayPic( IDC_PIC_D, szTempBuf, strlen(szTempBuf) );
			break;
		default:
			break;
		}
	}
	//一旦切换下一题，就一定要读音，不读音选择无效
	bRead = false;
	iAnswerTimes = 0;
	//出题就记录题数，同时清除回答次数记录变量
	iSumQuestion++;
	//错误四个开始标志位
	bGiveUp = true;
	bWrongGiveUp = false;
	bFindRightAnswer = false;
}


void CListenExamDlg::OnBnClickedPlayAudio()
{
	// TODO: 在此添加控件通知处理程序代码
	//合成对应图片的声音文件路径
	char pWavFileNamePath[MAX_PATH] = {0};
	int ilenWavFilePath =0;
	char Wavfilename[MAX_PATH] = {0};
	int ilenWavfilename = 0;
	sprintf( Wavfilename, "%s.wav",stInfoAnswer.stFileNameAnswer[0].szZiMu_No );
	ilenWavfilename = strlen(Wavfilename);
	sprintf( pWavFileNamePath, "%s\\%s",PathExeFile, Wavfilename );
	ilenWavFilePath = strlen(pWavFileNamePath); 
	//过滤只有一个'\'字符
	if ( 5>=strlen(pWavFileNamePath) )
	{
		memset( pWavFileNamePath, '\0', MAX_PATH );
	}

	//找到对应的声音文件，播放声音
	//if( 0 == access("D:\\workspace\\GitHub\\AlphabetExam\\AlphabetExam\\a00000_004_1.wav",0))
	if ( 0 == access(pWavFileNamePath,0) )
	{
		//播放声音
		sndPlaySound( pWavFileNamePath, SND_ASYNC );
		//读音后，选择才有效
		bRead = true;
	}
	else
	{
		//声音文件加载异常提示判别
		if ( 0==strlen(pWavFileNamePath) )
		{
			//路径为空，提示先要选择显示图片
			AfxMessageBox(_T("请先点击\"下一题\"按钮!"));
			//Alian: 考虑使用语音提示
		} 
		else
		{
			//路径不为空，提示加载声音文件失败
			sprintf( Wavfilename+ilenWavfilename, "\n找不到以上声音文件!" );
			AfxMessageBox(_T(Wavfilename));
			//Alian: 考虑使用语音提示
		}
	}
}

int CListenExamDlg::GetRandomNumber( int Random[MAX_PATH], int NumRange, int SumGet, bool Repeat )
{
	int iCnt = 0;
	int iRandNum = 0;
	bool bRepeat = false;

	//数值判断合法性
	if ( NumRange<=1 || SumGet<=0 )
	{
		return -1;
	}

	//在不可重复的条件下，随机数
	if ( !Repeat )
	{
		if ( SumGet > NumRange )
		{
			return -1;
		}
	}

	//随机获取4组XXX_X格式的文件名，不可重复,其中第一组为正确答案，作为答案的XXX为字母序号 X为音调
	srand((unsigned)time(NULL));
	//取得第一个下标，作为答案
	while ( iCnt < SumGet )
	{
		//现随机获取
		Random[iCnt] = (rand()%NumRange);
		//是否可以重复数值
		if ( Repeat )
		{
			iCnt ++;
		} 
		else //不可重复要做位判断
		{
			//只有一个随机数不需要判断重复性
			if ( 0 != iCnt )
			{
				while ( iRandNum < iCnt )
				{
					if ( Random[iRandNum++] == Random[iCnt] )
					{
						bRepeat = true;
						iRandNum = 0;
						break;  //只要有一样的随机数就跳出重新获取
					}
					else
					{
						bRepeat = false;
						continue; //如果是不一样就继续循环判断
					}
				}
				if ( !bRepeat ) //循环完没有重复就随机下一个数值
				{
					iCnt++;
					bRepeat = false;
					iRandNum = 0;
				}
			}
			else
			{
				iCnt++;
			}
		}
	}
	return 0;
}


void CListenExamDlg::OnBnClickedA()
{
	// TODO: 在此添加控件通知处理程序代码
	int iCmp = 1;
	char pRightFileNamePath[MAX_PATH] = {0};
	char pWrongFileNamePath[MAX_PATH] = {0};

	//拼正确和错误的语音文件路径
	sprintf( pRightFileNamePath, "%s\\正确提示音.wav",PathExeFile );
	sprintf( pWrongFileNamePath, "%s\\错误提示音.wav",PathExeFile );

	//过滤只有一个'\'字符
	if ( 1==strlen(pRightFileNamePath) )
	{
		memset( pRightFileNamePath, '\0', MAX_PATH );
	}
	if ( 1==strlen(pWrongFileNamePath) )
	{
		memset( pWrongFileNamePath, '\0', MAX_PATH );
	}

	//读音读过了才能选择，否则读音无效
	if ( bRead && !bFindRightAnswer )
	{
		//不管回答什么答案,记录回答次数
		iAnswerTimes++;
		//回答四次都错，就给记录回答四次都错记录变量，超过回答四次不算答对，但还是可以回答
		if ( iAnswerTimes>=5 && !bWrongGiveUp )
		{
			iSumFourWrongGiveUp++;
			bWrongGiveUp = true;
			bGiveUp = false;      //四次都是错误，那么允许用户切换到下一题或继续回答
			UpdateShowData();
		}

		if ( stInfoAnswer.iIndex == iCmp )  //回答正确
		{
			bGiveUp = false;
			bFindRightAnswer = true;
			//判断提示音在吗
			if ( 0 == access(pRightFileNamePath,0) )
			{
				//播放声音
				sndPlaySound( pRightFileNamePath, SND_ASYNC );
			}
			else
			{
				//提示找不到正确提示音文件
				AfxMessageBox(_T("找不到正确提示音.wav文件"));
				//Alian: 考虑使用语音提示
			}
			//如果回答正确记录，对应回答正常次数变量上累计计数
			switch (iAnswerTimes)
			{
			case 1:
				iSumFirstRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 2:
				iSumSecondRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 3:
				iSumThirdRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 4:
				iSumForthRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			default:
				break;
			}
		} 
		else
		{
			UpdateShowData();
			//判断提示音在吗
			if ( 0 == access(pWrongFileNamePath,0) )
			{
				//播放声音
				sndPlaySound( pWrongFileNamePath, SND_ASYNC );
			}
			else
			{
				//提示找不到正确提示音文件
				AfxMessageBox(_T("找不到错误提示音.wav文件"));
				//Alian: 考虑使用语音提示
			}
		}
	}
}


void CListenExamDlg::OnBnClickedB()
{
	// TODO: 在此添加控件通知处理程序代码
	int iCmp = 2;
	char pRightFileNamePath[MAX_PATH] = {0};
	char pWrongFileNamePath[MAX_PATH] = {0};

	//拼正确和错误的语音文件路径
	sprintf( pRightFileNamePath, "%s\\正确提示音.wav",PathExeFile );
	sprintf( pWrongFileNamePath, "%s\\错误提示音.wav",PathExeFile );

	//过滤只有一个'\'字符
	if ( 1==strlen(pRightFileNamePath) )
	{
		memset( pRightFileNamePath, '\0', MAX_PATH );
	}
	if ( 1==strlen(pWrongFileNamePath) )
	{
		memset( pWrongFileNamePath, '\0', MAX_PATH );
	}

	//读音读过了才能选择，否则读音无效
	if ( bRead && !bFindRightAnswer )
	{
		//不管回答什么答案,记录回答次数
		iAnswerTimes++;
		//回答四次都错，就给记录回答四次都错记录变量，超过回答四次不算答对，但还是可以回答
		if ( iAnswerTimes>=5 && !bWrongGiveUp )
		{
			iSumFourWrongGiveUp++;
			bWrongGiveUp = true;
			bGiveUp = false;      //四次都是错误，那么允许用户切换到下一题或继续回答
			UpdateShowData();
		}

		if ( stInfoAnswer.iIndex == iCmp )  //回答正确
		{
			bGiveUp = false;
			bFindRightAnswer = true;
			//判断提示音在吗
			if ( 0 == access(pRightFileNamePath,0) )
			{
				//播放声音
				sndPlaySound( pRightFileNamePath, SND_ASYNC );
			}
			else
			{
				//提示找不到正确提示音文件
				AfxMessageBox(_T("找不到正确提示音.wav文件"));
				//Alian: 考虑使用语音提示
			}
			//如果回答正确记录，对应回答正常次数变量上累计计数
			switch (iAnswerTimes)
			{
			case 1:
				iSumFirstRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 2:
				iSumSecondRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 3:
				iSumThirdRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 4:
				iSumForthRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			default:
				break;
			}
		} 
		else
		{
			UpdateShowData();
			//判断提示音在吗
			if ( 0 == access(pWrongFileNamePath,0) )
			{
				//播放声音
				sndPlaySound( pWrongFileNamePath, SND_ASYNC );
			}
			else
			{
				//提示找不到正确提示音文件
				AfxMessageBox(_T("找不到错误提示音.wav文件"));
				//Alian: 考虑使用语音提示
			}
		}
	}
}


void CListenExamDlg::OnBnClickedC()
{
	// TODO: 在此添加控件通知处理程序代码
	int iCmp = 3;
	char pRightFileNamePath[MAX_PATH] = {0};
	char pWrongFileNamePath[MAX_PATH] = {0};

	//拼正确和错误的语音文件路径
	sprintf( pRightFileNamePath, "%s\\正确提示音.wav",PathExeFile );
	sprintf( pWrongFileNamePath, "%s\\错误提示音.wav",PathExeFile );

	//过滤只有一个'\'字符
	if ( 1==strlen(pRightFileNamePath) )
	{
		memset( pRightFileNamePath, '\0', MAX_PATH );
	}
	if ( 1==strlen(pWrongFileNamePath) )
	{
		memset( pWrongFileNamePath, '\0', MAX_PATH );
	}

	//读音读过了才能选择，否则读音无效
	if ( bRead && !bFindRightAnswer )
	{
		//不管回答什么答案,记录回答次数
		iAnswerTimes++;
		//回答四次都错，就给记录回答四次都错记录变量，超过回答四次不算答对，但还是可以回答
		if ( iAnswerTimes>=5 && !bWrongGiveUp )
		{
			iSumFourWrongGiveUp++;
			bWrongGiveUp = true;
			bGiveUp = false;      //四次都是错误，那么允许用户切换到下一题或继续回答
			UpdateShowData();
		}

		if ( stInfoAnswer.iIndex == iCmp )  //回答正确
		{
			bGiveUp = false;
			bFindRightAnswer = true;
			//判断提示音在吗
			if ( 0 == access(pRightFileNamePath,0) )
			{
				//播放声音
				sndPlaySound( pRightFileNamePath, SND_ASYNC );
			}
			else
			{
				//提示找不到正确提示音文件
				AfxMessageBox(_T("找不到正确提示音.wav文件"));
				//Alian: 考虑使用语音提示
			}
			//如果回答正确记录，对应回答正常次数变量上累计计数
			switch (iAnswerTimes)
			{
			case 1:
				iSumFirstRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 2:
				iSumSecondRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 3:
				iSumThirdRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 4:
				iSumForthRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			default:
				break;
			}
		} 
		else
		{
			UpdateShowData();
			//判断提示音在吗
			if ( 0 == access(pWrongFileNamePath,0) )
			{
				//播放声音
				sndPlaySound( pWrongFileNamePath, SND_ASYNC );
			}
			else
			{
				//提示找不到正确提示音文件
				AfxMessageBox(_T("找不到错误提示音.wav文件"));
				//Alian: 考虑使用语音提示
			}
		}
	}
}


void CListenExamDlg::OnBnClickedD()
{
	// TODO: 在此添加控件通知处理程序代码
	int iCmp = 4;
	char pRightFileNamePath[MAX_PATH] = {0};
	char pWrongFileNamePath[MAX_PATH] = {0};

	//拼正确和错误的语音文件路径
	sprintf( pRightFileNamePath, "%s\\正确提示音.wav",PathExeFile );
	sprintf( pWrongFileNamePath, "%s\\错误提示音.wav",PathExeFile );

	//过滤只有一个'\'字符
	if ( 1==strlen(pRightFileNamePath) )
	{
		memset( pRightFileNamePath, '\0', MAX_PATH );
	}
	if ( 1==strlen(pWrongFileNamePath) )
	{
		memset( pWrongFileNamePath, '\0', MAX_PATH );
	}

	//读音读过了才能选择，否则读音无效
	if ( bRead && !bFindRightAnswer )
	{
		//不管回答什么答案,记录回答次数
		iAnswerTimes++;
		//回答四次都错，就给记录回答四次都错记录变量，超过回答四次不算答对，但还是可以回答
		if ( iAnswerTimes>=5 && !bWrongGiveUp )
		{
			iSumFourWrongGiveUp++;
			bWrongGiveUp = true;
			bGiveUp = false;      //四次都是错误，那么允许用户切换到下一题或继续回答
			UpdateShowData();
		}

		if ( stInfoAnswer.iIndex == iCmp )  //回答正确
		{
			bGiveUp = false;
			bFindRightAnswer = true;
			//判断提示音在吗
			if ( 0 == access(pRightFileNamePath,0) )
			{
				//播放声音
				sndPlaySound( pRightFileNamePath, SND_ASYNC );
			}
			else
			{
				//提示找不到正确提示音文件
				AfxMessageBox(_T("找不到正确提示音.wav文件"));
				//Alian: 考虑使用语音提示
			}
			//如果回答正确记录，对应回答正常次数变量上累计计数
			switch (iAnswerTimes)
			{
			case 1:
				iSumFirstRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 2:
				iSumSecondRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 3:
				iSumThirdRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			case 4:
				iSumForthRight++;
				//每一次回答都更新一次统计数据区
				UpdateShowData();
				break;

			default:
				break;
			}
		} 
		else
		{
			UpdateShowData();
			//判断提示音在吗
			if ( 0 == access(pWrongFileNamePath,0) )
			{
				//播放声音
				sndPlaySound( pWrongFileNamePath, SND_ASYNC );
			}
			else
			{
				//提示找不到正确提示音文件
				AfxMessageBox(_T("找不到错误提示音.wav文件"));
				//Alian: 考虑使用语音提示
			}
		}
	}
}


int CListenExamDlg::ReadConfigFile()
{
	//读取配置文件Config.inf，当做当前拼音个数
	FILE *fp = NULL;
	char pGetDateBuf[SIZE_ZI_MU] = {0};
	int iHangNo = 0;
	int Index = 0;

	//打开Config.inf文件
	fp = fopen( "Config.inf", "a+" );
	if( fp == NULL )    //Config.inf 文件打开失败
	{
		return 0;
	}
	else       //Config.inf 文件打开成功
	{
		//读到文件结束符(到达EOF就结束)
		while( !feof(fp) )
		{
			//从文件中读一行字符串
			fgets( pGetDateBuf, SIZE_ZI_MU, fp );
			if( 0 == iHangNo )
			{
				memcpy( stInfoConfig.stFileName[Index].szZiMu, pGetDateBuf, SIZE_ZI_MU );
				stInfoConfig.stFileName[Index].szZiMu[strlen(stInfoConfig.stFileName[Index].szZiMu)-1] = '\0';
				iHangNo = 1;
			}
			else if ( 1 == iHangNo )
			{
				memcpy( stInfoConfig.stFileName[Index].szNo, pGetDateBuf, SIZE_NO );
				stInfoConfig.stFileName[Index].szNo[strlen(stInfoConfig.stFileName[Index].szNo)-1] = '\0';
				iHangNo = 0;
				Index++;
				if ( 0 == Index%SUM_TYPE_YINGDIAO )
				{
					stInfoConfig.iSumCurrent++;
				}
			}
			memset( pGetDateBuf, '\0', SIZE_ZI_MU );
		}
	}
	return 0;
}

int CListenExamDlg::FindStructIndex( char cmpFlag[SIZE_NO] )
{
	int iRes = -1;
	int Cnt = 0;
	while ( 0 != memcmp( stInfoConfig.stFileName[Cnt].szNo, cmpFlag, 5 ) )
	{
		Cnt++;
	}
	iRes = Cnt;
	return iRes;
}

void CListenExamDlg::DisplayPic( int IDC_X, char PicFileName[MAX_PATH], int ilenPicfilename )
{
	//本按钮作用为随机显示一张拼音图片到显示窗中
	int cx, cy;  
	CImage  image;  
	CRect   rect; 
	char pPath[100] = {0};
	int iCount = 0;

	//合成需要显示图片文件路径
	char pPicFileNamePath[MAX_PATH] = {0};
	sprintf( pPicFileNamePath, "%s\\%s",PathExeFile, PicFileName );

	//根据需要显示图片文件路径载入图片  
	if( E_FAIL == image.Load(pPicFileNamePath) )
	{
		sprintf( PicFileName+ilenPicfilename, "\n找不到以上图片文件!" );
		AfxMessageBox(_T(PicFileName));
		//Alian: 考虑使用语音提示
		return;
	}

	//显示图片
	//获取图片的宽 高度  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();       
	//获取Picture Control控件的大小  
	GetDlgItem(IDC_X)->GetWindowRect(&rect);  
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);  
	//窗口移动到控件表示的区域  
	GetDlgItem(IDC_X)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);  
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(IDC_X);//获取控件句柄 
	pWnd->ShowWindow(TRUE);
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小       
	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();//获取picture的DC      
	image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内       
	ReleaseDC(pDc);
}

void CListenExamDlg::UpdateShowData()
{
	char szShowString[10] = {0};

	//总题数
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_SUM_QUESTION,szShowString);

	//第一次答对数(百分率)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumFirstRight, szShowString, 10 );
	SetDlgItemText(IDC_SUM_FIRST,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumFirstRight*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_FIRST_PER,szShowString);

	//第二次答对数(百分率)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumSecondRight, szShowString, 10 );
	SetDlgItemText(IDC_SUM_SECOND,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumSecondRight*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_SECOND_PER,szShowString);

	//第三次答对数(百分率)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumThirdRight, szShowString, 10 );
	SetDlgItemText(IDC_SUM_THIRD,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumThirdRight*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_THIRD_PER,szShowString);

	//第四次答对数(百分率)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumForthRight, szShowString, 10 );
	SetDlgItemText(IDC_SUM_FORTH,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumForthRight*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_FORTH_PER,szShowString);

	//四次回答错误放弃(放弃率)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumFourWrongGiveUp, szShowString, 10 );
	SetDlgItemText(IDC_SUM_GIVE_UP_FOUR,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumFourWrongGiveUp*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_GIVE_UP_FOUR_PER,szShowString);
}
