
// ListenExamDlg.h : 头文件
//

#pragma once

//本项目中IC类型总数
#define  SIZE_ZI_MU         10    //字母存储区总长
#define  SIZE_NO            10    //序号存储区总长
#define  SUM_DUYIN          999 

typedef struct  
{
	char szZiMu[SIZE_ZI_MU];        //字母缓存区
	char szNo[SIZE_NO];             //序号，也是随机数
}FILE_NAME,*P_FILE_NAME;

typedef struct  
{
	int iSumCurrent;
	FILE_NAME stFileName[SUM_DUYIN];
}INFO_CONFIG,*P_INFO_CONFIG;


#define  SUM_OPTION          4 
//答案项管理结构体
typedef struct  
{
	char szZiMu_No[SIZE_ZI_MU*2];     //整个文件名
	int  iRandNo;                   // 随机数序号，用于分布A,B,C,D答案的哪一个
}FILENAME_ANSWER,*P_FILENAME_ANSWER;

typedef struct  
{
	int iIndex;    //正确答案下标
	FILENAME_ANSWER stFileNameAnswer[SUM_OPTION];
}INFO_ANSWER,*P_INFO_ANSWER;

// CListenExamDlg 对话框
class CListenExamDlg : public CDialogEx
{
// 构造
public:
	CListenExamDlg(CWnd* pParent = NULL);	// 标准构造函数
	int GetRandomNumber( int Random[MAX_PATH], int NumRange, int SumGet, bool Repeat );  //获取SumGet个范围在NumRange内的随机数存放在Random数组中，Repeat=false为不可重复数字
	int ReadConfigFile(void);
	int FindStructIndex( char cmpFlag[SIZE_NO] );
	void DisplayPic( int IDC_X, char PicFileName[MAX_PATH], int ilenPicfilename );
	void UpdateShowData();          //更新统计数据区内容

// 对话框数据
	enum { IDD = IDD_LISTENEXAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
