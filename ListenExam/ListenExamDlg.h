
// ListenExamDlg.h : ͷ�ļ�
//

#pragma once

//����Ŀ��IC��������
#define  SIZE_ZI_MU         10    //��ĸ�洢���ܳ�
#define  SIZE_NO            10    //��Ŵ洢���ܳ�
#define  SUM_DUYIN          999 

typedef struct  
{
	char szZiMu[SIZE_ZI_MU];        //��ĸ������
	char szNo[SIZE_NO];             //��ţ�Ҳ�������
}FILE_NAME,*P_FILE_NAME;

typedef struct  
{
	int iSumCurrent;
	FILE_NAME stFileName[SUM_DUYIN];
}INFO_CONFIG,*P_INFO_CONFIG;


#define  SUM_OPTION          4 
//�������ṹ��
typedef struct  
{
	char szZiMu_No[SIZE_ZI_MU*2];     //�����ļ���
	int  iRandNo;                   // �������ţ����ڷֲ�A,B,C,D�𰸵���һ��
}FILENAME_ANSWER,*P_FILENAME_ANSWER;

typedef struct  
{
	int iIndex;    //��ȷ���±�
	FILENAME_ANSWER stFileNameAnswer[SUM_OPTION];
}INFO_ANSWER,*P_INFO_ANSWER;

// CListenExamDlg �Ի���
class CListenExamDlg : public CDialogEx
{
// ����
public:
	CListenExamDlg(CWnd* pParent = NULL);	// ��׼���캯��
	int GetRandomNumber( int Random[MAX_PATH], int NumRange, int SumGet, bool Repeat );  //��ȡSumGet����Χ��NumRange�ڵ�����������Random�����У�Repeat=falseΪ�����ظ�����
	int ReadConfigFile(void);
	int FindStructIndex( char cmpFlag[SIZE_NO] );
	void DisplayPic( int IDC_X, char PicFileName[MAX_PATH], int ilenPicfilename );
	void UpdateShowData();          //����ͳ������������

// �Ի�������
	enum { IDD = IDD_LISTENEXAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNextQuestion();
	afx_msg void OnBnClickedPlayAudio();
	afx_msg void OnBnClickedA();
	afx_msg void OnBnClickedB();
	afx_msg void OnBnClickedC();
	afx_msg void OnBnClickedD();
};
