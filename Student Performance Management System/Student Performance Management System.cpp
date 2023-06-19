
// Student Performance Management System.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Student Performance Management System.h"
#include "MainFrm.h"

#include "Student Performance Management SystemDoc.h"
#include "Student Performance Management SystemView.h"

#include "CStuDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStudentPerformanceManagementSystemApp

BEGIN_MESSAGE_MAP(CStudentPerformanceManagementSystemApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CStudentPerformanceManagementSystemApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_STUDENT, &CStudentPerformanceManagementSystemApp::OnStuDlg)
	ON_COMMAND(ID_RANDOMDATA, &CStudentPerformanceManagementSystemApp::OnRandomdata)
	ON_COMMAND(ID_OPENDATAFILE, &CStudentPerformanceManagementSystemApp::OnOpendatafile)
END_MESSAGE_MAP()


// CStudentPerformanceManagementSystemApp 构造

CStudentPerformanceManagementSystemApp::CStudentPerformanceManagementSystemApp() noexcept
{
	m_bHiColorIcons = TRUE;


	m_nAppLook = 0;
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("StudentPerformanceManagementSystem.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CStudentPerformanceManagementSystemApp 对象

CStudentPerformanceManagementSystemApp theApp;


// CStudentPerformanceManagementSystemApp 初始化

BOOL CStudentPerformanceManagementSystemApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CStudentPerformanceManagementSystemDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CStudentPerformanceManagementSystemView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CStudentPerformanceManagementSystemApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CStudentPerformanceManagementSystemApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CStudentPerformanceManagementSystemApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



void CStudentPerformanceManagementSystemApp::OnStuDlg()
{
	CStuDlg dlg;
	dlg.DoModal();
}




// CStudentPerformanceManagementSystemApp 自定义加载/保存方法

void CStudentPerformanceManagementSystemApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CStudentPerformanceManagementSystemApp::LoadCustomState()
{
}

void CStudentPerformanceManagementSystemApp::SaveCustomState()
{
}

// CStudentPerformanceManagementSystemApp 消息处理程序




string RandStr(const int len)  //参数为字符串的长度
{
	string str;                 //声明用来保存随机字符串的str
	char c;                     //声明字符c，用来保存随机生成的字符

	//循环向字符串中添加随机生成的字符
	for (int idx = 0; idx < len; idx++)
	{
		//rand()%26是取余，余数为0~25加上'a',就是字母a~z
		c = 'a' + rand() % 26;
		str.push_back(c);
	}
	return str;
}
void CStudentPerformanceManagementSystemApp::OnRandomdata()
{
	// TODO: 在此添加命令处理程序代码
	srand((unsigned int)(time(NULL)));

	int a = 0;//左闭，控制成绩
	int b = 0;//右开

	int c = 45;//控制生成数量
	int d = 55;

	int range= (rand() % (d - c)) + c;
	int eachrange1 = (rand() % (7 - 4)) + 4;
	int eachrange2 = (rand() % (13 - 10)) + 10;
	int eachrange3 = (rand() % (19 - 16)) + 16;
	int eachrange4 = (rand() % (16 - 9)) + 9;
	//int eachrange5 = range - eachrange1 - eachrange2 - eachrange3 - eachrange4;
	//要取得[a, b)的随机整数，使用(rand() % (b - a)) + a;
	for (int i = 0; i < range; i++)
	{


		if (i < eachrange1)//1区间
		{
			a = 0;
			b = 60;

		}
		else if (i < eachrange1 + eachrange2)//2区间
		{
			a = 60;
			b = 70;
		}
		else if (i < eachrange1 + eachrange2 + eachrange3)//3区间
		{
			a = 70;
			b = 80;
		}
		else if (i < eachrange1 + eachrange2 + eachrange3 + eachrange4)//4区间
		{
			a = 80;
			b = 90;
		}
		else//5区间
		{
			a = 90;
			b = 100;
		}

		string str;                 /*声明字符串*/
		str = RandStr(3);         /*调用函数 输入字符串长度*/


		string s;                 /*声明字符串*/
		s = RandStr(4);

		int t = rand() % 2;
		string ss;
		if (t == 0)
		{
			ss = "男";
		}
		else
		{
			ss = "女";
		}

		//要取得[a, b)的随机整数，使用(rand() % (b - a)) + a;
		int number = (rand() % (999999 - 10)) + 10;
		string name = str;
		string gender = ss;
		int age = (rand() % (99 - 10)) + 10;
		string address = s;

		int math = (rand() % (b - a)) + a;
		int oop = (rand() % (b - a)) + a;

		ofstream ofs;//写文件(默认ios::out，每次写都是先清空后写)

		ofs.open("StudentData.txt", std::ios::app);


		ofs << number << "|";
		ofs << name << "|";
		ofs << gender << "|";
		ofs << age << "|";
		ofs << address << "|";
		ofs << math << "|";
		ofs << oop << endl;


		ofs.close();
	}
	//MessageBox(TEXT("成功添加随机学生信息"));
}


void CStudentPerformanceManagementSystemApp::OnOpendatafile()
{
	// TODO: 在此添加命令处理程序代码
	// 指定文件名
	LPCTSTR file_name = L"StudentData.txt";

	// 打开文件
	HINSTANCE hInstance = ShellExecute(NULL, L"open", file_name, NULL, NULL, SW_SHOWNORMAL);
}
