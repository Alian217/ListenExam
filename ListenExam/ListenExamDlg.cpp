
// ListenExamDlg.cpp : ʵ���ļ�
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
*                ģ���е�ȫ�ֱ���                    *
****************************************************/
//�����ļ�����ṹ��(�ļ����𿪹���)
INFO_CONFIG stInfoConfig;

//�𰸹���ṹ��
INFO_ANSWER stInfoAnswer;

//exeִ���ļ�·����Ҳ��������ͼƬ��·��
char PathExeFile[MAX_PATH] = {0};
int ilenPathExeFile =0;

//���ŵ���ȷ���������ļ���
char AnswerWavfilename[MAX_PATH] = {0};
int ilenAnswerWavfilename = 0;
//���������������ѡ�������ļ���
char WrongWavfilename1[MAX_PATH] = {0};
int ilenWrongWavfilename1 = 0;
char WrongWavfilename2[MAX_PATH] = {0};
int ilenWrongWavfilename2 = 0;
char WrongWavfilename3[MAX_PATH] = {0};
int ilenWrongWavfilename3 = 0;

//�������������ǹ̶�4
int SUM_TYPE_YINGDIAO = 4;

//�ѵ��������־λ�����û�ж���ѡ����Ч
bool bRead = false;

//�ڼ��λش��־λ
int iAnswerTimes = 0;

//��������¼����
int iSumQuestion = 0;

//��һ�����Ĵλش�������ȷ��¼����
int iSumFirstRight = 0;
int iSumSecondRight = 0;
int iSumThirdRight = 0;
int iSumForthRight = 0;

//���ش������¼����
bool bGiveUp = false;

//�����ĸ�����
int iSumFourWrongGiveUp = 0;
bool bWrongGiveUp = false;

//�ҵ���ȷ�𰸱�־λ
bool bFindRightAnswer = false;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CListenExamDlg �Ի���




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


// CListenExamDlg ��Ϣ�������

BOOL CListenExamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������ alian:�ĵ�AlphabetExam��Ŀ��
	memset( &stInfoConfig, '\0', sizeof(INFO_CONFIG) );
	memset( &stInfoAnswer, '\0', sizeof(INFO_ANSWER) );
	ReadConfigFile();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CListenExamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CListenExamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CListenExamDlg::OnBnClickedNextQuestion()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int RandIndex1=-1;
	int RandIndex2=-1;
	int RandIndex3=-1;
	int RandIndex4=-1;
 	int Random[MAX_PATH] = {0};
 	int iTempOffset=0;
	char szTempBuf[MAX_PATH] = {0};

	//���û�лش�ֱ������� ���� �Ĵδ����ڣ��޷��л�����һ��
	if ( bGiveUp )
	{
		//Alian: ����ʹ��������ʾ��������ɴ���
		return;
	}

	//ÿһ�ΰ���һ�ⰴť����
	memset( &stInfoAnswer, '\0', sizeof(INFO_ANSWER) );

	//��ȡ��ǰִ�С�ͼƬ�������ļ�·��
	memset(PathExeFile, '\0', sizeof(PathExeFile));
	ilenPathExeFile = GetCurrentDirectory( MAX_PATH, PathExeFile );
	if ( ilenPathExeFile < 0 )
	{
		AfxMessageBox(_T("�޷���ȡ��ǰִ���ļ�·��!"));
		//Alian: ����ʹ��������ʾ
		return;
	}

	//�����ȡ4��XXX_X��ʽ���ļ����������ظ�,���е�һ��Ϊ��ȷ�𰸣���Ϊ�𰸵�XXXΪ��ĸ��� XΪ����
	srand((unsigned)time(NULL));
	//ȡ�õ�һ���±꣬��Ϊ��
	while ( true )
	{
		//�������ȡ
		RandIndex1 = (rand()%(stInfoConfig.iSumCurrent*SUM_TYPE_YINGDIAO));
		//ȷ����ͬ���������������
		if ( RandIndex1!=RandIndex2 && RandIndex1!=RandIndex3 && RandIndex1!=RandIndex4  )
		{
			break;
		}
	}
	//ȡ�õڶ������������±���Ϊ����������
	while ( true )
	{
		//�������ȡ
		RandIndex2 = (rand()%(stInfoConfig.iSumCurrent*SUM_TYPE_YINGDIAO));
		//ȷ����ͬ���������������
		if ( RandIndex2!=RandIndex1 && RandIndex2!=RandIndex3 && RandIndex2!=RandIndex4  )
		{
			break;
		}
	}
	while ( true )
	{
		//�������ȡ
		RandIndex3 = (rand()%(stInfoConfig.iSumCurrent*SUM_TYPE_YINGDIAO));
		//ȷ����ͬ���������������
		if ( RandIndex3!=RandIndex1 && RandIndex3!=RandIndex2 && RandIndex3!=RandIndex4  )
		{
			break;
		}
	}
	while ( true )
	{
		//�������ȡ
		RandIndex4 = (rand()%(stInfoConfig.iSumCurrent*SUM_TYPE_YINGDIAO));
		//ȷ����ͬ���������������
		if ( RandIndex4!=RandIndex1 && RandIndex4!=RandIndex2 && RandIndex4!=RandIndex3  )
		{
			break;
		}
	}

	iTempOffset = 0;
	//��ȷ�𰸴��ڶ�Ӧ��Ӧ�𰸹���ṹ���һ��Ԫ������
	memcpy( stInfoAnswer.stFileNameAnswer[0].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex1].szZiMu, strlen(stInfoConfig.stFileName[RandIndex1].szZiMu) );
	iTempOffset += strlen(stInfoConfig.stFileName[RandIndex1].szZiMu);
	memcpy( stInfoAnswer.stFileNameAnswer[0].szZiMu_No+iTempOffset, stInfoConfig.stFileName[RandIndex1].szNo, strlen(stInfoConfig.stFileName[RandIndex1].szNo) );

	//����𰸴��ڶ�Ӧ��Ӧ�𰸹���ṹ����������Ԫ������
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

	//��ȡSUM_TYPE_YINGDIAO��Χ�ڵ�SumGet�������������Random�����У������ظ�(false)
	GetRandomNumber( Random, SUM_TYPE_YINGDIAO, 4, false );

	//���𰸹���ṹ������� A-1 B-2 C-3 D-4
	stInfoAnswer.iIndex = Random[0]+1;
	stInfoAnswer.stFileNameAnswer[0].iRandNo = Random[0]+1;
	stInfoAnswer.stFileNameAnswer[1].iRandNo = Random[1]+1;
	stInfoAnswer.stFileNameAnswer[2].iRandNo = Random[2]+1;
	stInfoAnswer.stFileNameAnswer[3].iRandNo = Random[3]+1;

	//��ʾ��Ӧ���ַ�����Ӧѡ����
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
	//һ���л���һ�⣬��һ��Ҫ������������ѡ����Ч
	bRead = false;
	iAnswerTimes = 0;
	//����ͼ�¼������ͬʱ����ش������¼����
	iSumQuestion++;
	//�����ĸ���ʼ��־λ
	bGiveUp = true;
	bWrongGiveUp = false;
	bFindRightAnswer = false;
}


void CListenExamDlg::OnBnClickedPlayAudio()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ϳɶ�ӦͼƬ�������ļ�·��
	char pWavFileNamePath[MAX_PATH] = {0};
	int ilenWavFilePath =0;
	char Wavfilename[MAX_PATH] = {0};
	int ilenWavfilename = 0;
	sprintf( Wavfilename, "%s.wav",stInfoAnswer.stFileNameAnswer[0].szZiMu_No );
	ilenWavfilename = strlen(Wavfilename);
	sprintf( pWavFileNamePath, "%s\\%s",PathExeFile, Wavfilename );
	ilenWavFilePath = strlen(pWavFileNamePath); 
	//����ֻ��һ��'\'�ַ�
	if ( 5>=strlen(pWavFileNamePath) )
	{
		memset( pWavFileNamePath, '\0', MAX_PATH );
	}

	//�ҵ���Ӧ�������ļ�����������
	//if( 0 == access("D:\\workspace\\GitHub\\AlphabetExam\\AlphabetExam\\a00000_004_1.wav",0))
	if ( 0 == access(pWavFileNamePath,0) )
	{
		//��������
		sndPlaySound( pWavFileNamePath, SND_ASYNC );
		//������ѡ�����Ч
		bRead = true;
	}
	else
	{
		//�����ļ������쳣��ʾ�б�
		if ( 0==strlen(pWavFileNamePath) )
		{
			//·��Ϊ�գ���ʾ��Ҫѡ����ʾͼƬ
			AfxMessageBox(_T("���ȵ��\"��һ��\"��ť!"));
			//Alian: ����ʹ��������ʾ
		} 
		else
		{
			//·����Ϊ�գ���ʾ���������ļ�ʧ��
			sprintf( Wavfilename+ilenWavfilename, "\n�Ҳ������������ļ�!" );
			AfxMessageBox(_T(Wavfilename));
			//Alian: ����ʹ��������ʾ
		}
	}
}

int CListenExamDlg::GetRandomNumber( int Random[MAX_PATH], int NumRange, int SumGet, bool Repeat )
{
	int iCnt = 0;
	int iRandNum = 0;
	bool bRepeat = false;

	//��ֵ�жϺϷ���
	if ( NumRange<=1 || SumGet<=0 )
	{
		return -1;
	}

	//�ڲ����ظ��������£������
	if ( !Repeat )
	{
		if ( SumGet > NumRange )
		{
			return -1;
		}
	}

	//�����ȡ4��XXX_X��ʽ���ļ����������ظ�,���е�һ��Ϊ��ȷ�𰸣���Ϊ�𰸵�XXXΪ��ĸ��� XΪ����
	srand((unsigned)time(NULL));
	//ȡ�õ�һ���±꣬��Ϊ��
	while ( iCnt < SumGet )
	{
		//�������ȡ
		Random[iCnt] = (rand()%NumRange);
		//�Ƿ�����ظ���ֵ
		if ( Repeat )
		{
			iCnt ++;
		} 
		else //�����ظ�Ҫ��λ�ж�
		{
			//ֻ��һ�����������Ҫ�ж��ظ���
			if ( 0 != iCnt )
			{
				while ( iRandNum < iCnt )
				{
					if ( Random[iRandNum++] == Random[iCnt] )
					{
						bRepeat = true;
						iRandNum = 0;
						break;  //ֻҪ��һ������������������»�ȡ
					}
					else
					{
						bRepeat = false;
						continue; //����ǲ�һ���ͼ���ѭ���ж�
					}
				}
				if ( !bRepeat ) //ѭ����û���ظ��������һ����ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iCmp = 1;
	char pRightFileNamePath[MAX_PATH] = {0};
	char pWrongFileNamePath[MAX_PATH] = {0};

	//ƴ��ȷ�ʹ���������ļ�·��
	sprintf( pRightFileNamePath, "%s\\��ȷ��ʾ��.wav",PathExeFile );
	sprintf( pWrongFileNamePath, "%s\\������ʾ��.wav",PathExeFile );

	//����ֻ��һ��'\'�ַ�
	if ( 1==strlen(pRightFileNamePath) )
	{
		memset( pRightFileNamePath, '\0', MAX_PATH );
	}
	if ( 1==strlen(pWrongFileNamePath) )
	{
		memset( pWrongFileNamePath, '\0', MAX_PATH );
	}

	//���������˲���ѡ�񣬷��������Ч
	if ( bRead && !bFindRightAnswer )
	{
		//���ܻش�ʲô��,��¼�ش����
		iAnswerTimes++;
		//�ش��Ĵζ����͸���¼�ش��Ĵζ����¼�����������ش��Ĵβ����ԣ������ǿ��Իش�
		if ( iAnswerTimes>=5 && !bWrongGiveUp )
		{
			iSumFourWrongGiveUp++;
			bWrongGiveUp = true;
			bGiveUp = false;      //�Ĵζ��Ǵ�����ô�����û��л�����һ�������ش�
			UpdateShowData();
		}

		if ( stInfoAnswer.iIndex == iCmp )  //�ش���ȷ
		{
			bGiveUp = false;
			bFindRightAnswer = true;
			//�ж���ʾ������
			if ( 0 == access(pRightFileNamePath,0) )
			{
				//��������
				sndPlaySound( pRightFileNamePath, SND_ASYNC );
			}
			else
			{
				//��ʾ�Ҳ�����ȷ��ʾ���ļ�
				AfxMessageBox(_T("�Ҳ�����ȷ��ʾ��.wav�ļ�"));
				//Alian: ����ʹ��������ʾ
			}
			//����ش���ȷ��¼����Ӧ�ش����������������ۼƼ���
			switch (iAnswerTimes)
			{
			case 1:
				iSumFirstRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 2:
				iSumSecondRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 3:
				iSumThirdRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 4:
				iSumForthRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			default:
				break;
			}
		} 
		else
		{
			UpdateShowData();
			//�ж���ʾ������
			if ( 0 == access(pWrongFileNamePath,0) )
			{
				//��������
				sndPlaySound( pWrongFileNamePath, SND_ASYNC );
			}
			else
			{
				//��ʾ�Ҳ�����ȷ��ʾ���ļ�
				AfxMessageBox(_T("�Ҳ���������ʾ��.wav�ļ�"));
				//Alian: ����ʹ��������ʾ
			}
		}
	}
}


void CListenExamDlg::OnBnClickedB()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iCmp = 2;
	char pRightFileNamePath[MAX_PATH] = {0};
	char pWrongFileNamePath[MAX_PATH] = {0};

	//ƴ��ȷ�ʹ���������ļ�·��
	sprintf( pRightFileNamePath, "%s\\��ȷ��ʾ��.wav",PathExeFile );
	sprintf( pWrongFileNamePath, "%s\\������ʾ��.wav",PathExeFile );

	//����ֻ��һ��'\'�ַ�
	if ( 1==strlen(pRightFileNamePath) )
	{
		memset( pRightFileNamePath, '\0', MAX_PATH );
	}
	if ( 1==strlen(pWrongFileNamePath) )
	{
		memset( pWrongFileNamePath, '\0', MAX_PATH );
	}

	//���������˲���ѡ�񣬷��������Ч
	if ( bRead && !bFindRightAnswer )
	{
		//���ܻش�ʲô��,��¼�ش����
		iAnswerTimes++;
		//�ش��Ĵζ����͸���¼�ش��Ĵζ����¼�����������ش��Ĵβ����ԣ������ǿ��Իش�
		if ( iAnswerTimes>=5 && !bWrongGiveUp )
		{
			iSumFourWrongGiveUp++;
			bWrongGiveUp = true;
			bGiveUp = false;      //�Ĵζ��Ǵ�����ô�����û��л�����һ�������ش�
			UpdateShowData();
		}

		if ( stInfoAnswer.iIndex == iCmp )  //�ش���ȷ
		{
			bGiveUp = false;
			bFindRightAnswer = true;
			//�ж���ʾ������
			if ( 0 == access(pRightFileNamePath,0) )
			{
				//��������
				sndPlaySound( pRightFileNamePath, SND_ASYNC );
			}
			else
			{
				//��ʾ�Ҳ�����ȷ��ʾ���ļ�
				AfxMessageBox(_T("�Ҳ�����ȷ��ʾ��.wav�ļ�"));
				//Alian: ����ʹ��������ʾ
			}
			//����ش���ȷ��¼����Ӧ�ش����������������ۼƼ���
			switch (iAnswerTimes)
			{
			case 1:
				iSumFirstRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 2:
				iSumSecondRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 3:
				iSumThirdRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 4:
				iSumForthRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			default:
				break;
			}
		} 
		else
		{
			UpdateShowData();
			//�ж���ʾ������
			if ( 0 == access(pWrongFileNamePath,0) )
			{
				//��������
				sndPlaySound( pWrongFileNamePath, SND_ASYNC );
			}
			else
			{
				//��ʾ�Ҳ�����ȷ��ʾ���ļ�
				AfxMessageBox(_T("�Ҳ���������ʾ��.wav�ļ�"));
				//Alian: ����ʹ��������ʾ
			}
		}
	}
}


void CListenExamDlg::OnBnClickedC()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iCmp = 3;
	char pRightFileNamePath[MAX_PATH] = {0};
	char pWrongFileNamePath[MAX_PATH] = {0};

	//ƴ��ȷ�ʹ���������ļ�·��
	sprintf( pRightFileNamePath, "%s\\��ȷ��ʾ��.wav",PathExeFile );
	sprintf( pWrongFileNamePath, "%s\\������ʾ��.wav",PathExeFile );

	//����ֻ��һ��'\'�ַ�
	if ( 1==strlen(pRightFileNamePath) )
	{
		memset( pRightFileNamePath, '\0', MAX_PATH );
	}
	if ( 1==strlen(pWrongFileNamePath) )
	{
		memset( pWrongFileNamePath, '\0', MAX_PATH );
	}

	//���������˲���ѡ�񣬷��������Ч
	if ( bRead && !bFindRightAnswer )
	{
		//���ܻش�ʲô��,��¼�ش����
		iAnswerTimes++;
		//�ش��Ĵζ����͸���¼�ش��Ĵζ����¼�����������ش��Ĵβ����ԣ������ǿ��Իش�
		if ( iAnswerTimes>=5 && !bWrongGiveUp )
		{
			iSumFourWrongGiveUp++;
			bWrongGiveUp = true;
			bGiveUp = false;      //�Ĵζ��Ǵ�����ô�����û��л�����һ�������ش�
			UpdateShowData();
		}

		if ( stInfoAnswer.iIndex == iCmp )  //�ش���ȷ
		{
			bGiveUp = false;
			bFindRightAnswer = true;
			//�ж���ʾ������
			if ( 0 == access(pRightFileNamePath,0) )
			{
				//��������
				sndPlaySound( pRightFileNamePath, SND_ASYNC );
			}
			else
			{
				//��ʾ�Ҳ�����ȷ��ʾ���ļ�
				AfxMessageBox(_T("�Ҳ�����ȷ��ʾ��.wav�ļ�"));
				//Alian: ����ʹ��������ʾ
			}
			//����ش���ȷ��¼����Ӧ�ش����������������ۼƼ���
			switch (iAnswerTimes)
			{
			case 1:
				iSumFirstRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 2:
				iSumSecondRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 3:
				iSumThirdRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 4:
				iSumForthRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			default:
				break;
			}
		} 
		else
		{
			UpdateShowData();
			//�ж���ʾ������
			if ( 0 == access(pWrongFileNamePath,0) )
			{
				//��������
				sndPlaySound( pWrongFileNamePath, SND_ASYNC );
			}
			else
			{
				//��ʾ�Ҳ�����ȷ��ʾ���ļ�
				AfxMessageBox(_T("�Ҳ���������ʾ��.wav�ļ�"));
				//Alian: ����ʹ��������ʾ
			}
		}
	}
}


void CListenExamDlg::OnBnClickedD()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iCmp = 4;
	char pRightFileNamePath[MAX_PATH] = {0};
	char pWrongFileNamePath[MAX_PATH] = {0};

	//ƴ��ȷ�ʹ���������ļ�·��
	sprintf( pRightFileNamePath, "%s\\��ȷ��ʾ��.wav",PathExeFile );
	sprintf( pWrongFileNamePath, "%s\\������ʾ��.wav",PathExeFile );

	//����ֻ��һ��'\'�ַ�
	if ( 1==strlen(pRightFileNamePath) )
	{
		memset( pRightFileNamePath, '\0', MAX_PATH );
	}
	if ( 1==strlen(pWrongFileNamePath) )
	{
		memset( pWrongFileNamePath, '\0', MAX_PATH );
	}

	//���������˲���ѡ�񣬷��������Ч
	if ( bRead && !bFindRightAnswer )
	{
		//���ܻش�ʲô��,��¼�ش����
		iAnswerTimes++;
		//�ش��Ĵζ����͸���¼�ش��Ĵζ����¼�����������ش��Ĵβ����ԣ������ǿ��Իش�
		if ( iAnswerTimes>=5 && !bWrongGiveUp )
		{
			iSumFourWrongGiveUp++;
			bWrongGiveUp = true;
			bGiveUp = false;      //�Ĵζ��Ǵ�����ô�����û��л�����һ�������ش�
			UpdateShowData();
		}

		if ( stInfoAnswer.iIndex == iCmp )  //�ش���ȷ
		{
			bGiveUp = false;
			bFindRightAnswer = true;
			//�ж���ʾ������
			if ( 0 == access(pRightFileNamePath,0) )
			{
				//��������
				sndPlaySound( pRightFileNamePath, SND_ASYNC );
			}
			else
			{
				//��ʾ�Ҳ�����ȷ��ʾ���ļ�
				AfxMessageBox(_T("�Ҳ�����ȷ��ʾ��.wav�ļ�"));
				//Alian: ����ʹ��������ʾ
			}
			//����ش���ȷ��¼����Ӧ�ش����������������ۼƼ���
			switch (iAnswerTimes)
			{
			case 1:
				iSumFirstRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 2:
				iSumSecondRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 3:
				iSumThirdRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			case 4:
				iSumForthRight++;
				//ÿһ�λش𶼸���һ��ͳ��������
				UpdateShowData();
				break;

			default:
				break;
			}
		} 
		else
		{
			UpdateShowData();
			//�ж���ʾ������
			if ( 0 == access(pWrongFileNamePath,0) )
			{
				//��������
				sndPlaySound( pWrongFileNamePath, SND_ASYNC );
			}
			else
			{
				//��ʾ�Ҳ�����ȷ��ʾ���ļ�
				AfxMessageBox(_T("�Ҳ���������ʾ��.wav�ļ�"));
				//Alian: ����ʹ��������ʾ
			}
		}
	}
}


int CListenExamDlg::ReadConfigFile()
{
	//��ȡ�����ļ�Config.inf��������ǰƴ������
	FILE *fp = NULL;
	char pGetDateBuf[SIZE_ZI_MU] = {0};
	int iHangNo = 0;
	int Index = 0;

	//��Config.inf�ļ�
	fp = fopen( "Config.inf", "a+" );
	if( fp == NULL )    //Config.inf �ļ���ʧ��
	{
		return 0;
	}
	else       //Config.inf �ļ��򿪳ɹ�
	{
		//�����ļ�������(����EOF�ͽ���)
		while( !feof(fp) )
		{
			//���ļ��ж�һ���ַ���
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
	//����ť����Ϊ�����ʾһ��ƴ��ͼƬ����ʾ����
	int cx, cy;  
	CImage  image;  
	CRect   rect; 
	char pPath[100] = {0};
	int iCount = 0;

	//�ϳ���Ҫ��ʾͼƬ�ļ�·��
	char pPicFileNamePath[MAX_PATH] = {0};
	sprintf( pPicFileNamePath, "%s\\%s",PathExeFile, PicFileName );

	//������Ҫ��ʾͼƬ�ļ�·������ͼƬ  
	if( E_FAIL == image.Load(pPicFileNamePath) )
	{
		sprintf( PicFileName+ilenPicfilename, "\n�Ҳ�������ͼƬ�ļ�!" );
		AfxMessageBox(_T(PicFileName));
		//Alian: ����ʹ��������ʾ
		return;
	}

	//��ʾͼƬ
	//��ȡͼƬ�Ŀ� �߶�  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();       
	//��ȡPicture Control�ؼ��Ĵ�С  
	GetDlgItem(IDC_X)->GetWindowRect(&rect);  
	//���ͻ���ѡ�е��ؼ���ʾ�ľ���������  
	ScreenToClient(&rect);  
	//�����ƶ����ؼ���ʾ������  
	GetDlgItem(IDC_X)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);  
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(IDC_X);//��ȡ�ؼ���� 
	pWnd->ShowWindow(TRUE);
	pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С       
	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();//��ȡpicture��DC      
	image.Draw(pDc->m_hDC, rect);//��ͼƬ���Ƶ�picture��ʾ��������       
	ReleaseDC(pDc);
}

void CListenExamDlg::UpdateShowData()
{
	char szShowString[10] = {0};

	//������
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_SUM_QUESTION,szShowString);

	//��һ�δ����(�ٷ���)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumFirstRight, szShowString, 10 );
	SetDlgItemText(IDC_SUM_FIRST,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumFirstRight*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_FIRST_PER,szShowString);

	//�ڶ��δ����(�ٷ���)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumSecondRight, szShowString, 10 );
	SetDlgItemText(IDC_SUM_SECOND,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumSecondRight*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_SECOND_PER,szShowString);

	//�����δ����(�ٷ���)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumThirdRight, szShowString, 10 );
	SetDlgItemText(IDC_SUM_THIRD,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumThirdRight*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_THIRD_PER,szShowString);

	//���Ĵδ����(�ٷ���)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumForthRight, szShowString, 10 );
	SetDlgItemText(IDC_SUM_FORTH,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumForthRight*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_FORTH_PER,szShowString);

	//�Ĵλش�������(������)
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( iSumFourWrongGiveUp, szShowString, 10 );
	SetDlgItemText(IDC_SUM_GIVE_UP_FOUR,szShowString);
	memset( szShowString, '\0', sizeof(szShowString) );
	itoa( (iSumFourWrongGiveUp*100)/iSumQuestion, szShowString, 10 );
	SetDlgItemText(IDC_GIVE_UP_FOUR_PER,szShowString);
}
