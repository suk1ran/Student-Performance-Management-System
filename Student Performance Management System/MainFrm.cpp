
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Student Performance Management System.h"


#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_WM_PAINT()
	ON_COMMAND(ID_MATHHISTOGRAM, &CMainFrame::OnMathhistogram)
	ON_COMMAND(ID_OOPHISTOGRAM, &CMainFrame::OnOophistogram)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作: 
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));



	//更改应用程序图标
	HICON ico = AfxGetApp()->LoadIcon(IDI_STRAWBERRY);
	SetIcon(ico, TRUE);//设置大图标
	SetIcon(ico, FALSE);//设置小图标

	//设置右标题
	//SetTitle(TEXT("v1.1.0-beta.1 （Powered by he277）"));


	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = nullptr;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

//载入高数成绩
void CMainFrame::LoadMath()
{
	std::ifstream ifs("StudentData.txt");//读文件

	char buf[2048] = { 0 };
	count = 0;	//初始化学生数量为0

	//取出表头
	ifs.getline(buf, sizeof(buf));

	//清空容器
	score.clear();

	while (!ifs.eof())	//没到文件结尾
	{
		
		ifs.getline(buf, sizeof(buf));	//读取一行
		//count++;	//学生数量加一

		char* sst = strtok(buf, "|");	//以"|"分隔
		if (sst != NULL)
		{
			//tmp.Number = sst;	//读学号,只读不取
		}
		else
		{
			break;
		}

		sst = strtok(NULL, "|");//读姓名,只读不取
		//tmp.Name = sst;

		sst = strtok(NULL, "|");//读性别,只读不取
		//tmp.Gender = sst;

		sst = strtok(NULL, "|");//读年龄,只读不取
		//tmp.Age = atoi(sst);

		sst = strtok(NULL, "|");//读地址,只读不取
		//tmp.Address = sst;

		sst = strtok(NULL, "|");//读取高数成绩
		score.push_back(std::stod(sst));

		//sst = strtok(NULL, "|");//读取面向对象成绩,只读不取
		//tmp.OOPScore = stod(sst);

	}

	ifs.close();	//关闭文件
}

//载入面向对象成绩
void CMainFrame::LoadOOP()
{
	std::ifstream ifs("StudentData.txt");//读文件

	char buf[2048] = { 0 };
	count = 0;	//初始化学生数量为0

	//取出表头
	ifs.getline(buf, sizeof(buf));

	//清空容器
	score.clear();

	while (!ifs.eof())	//没到文件结尾
	{

		ifs.getline(buf, sizeof(buf));	//读取一行
		//count++;	//学生数量加一

		char* sst = strtok(buf, "|");	//以"|"分隔
		if (sst != NULL)
		{
			//tmp.Number = sst;	//读学号,只读不取
		}
		else
		{
			break;
		}

		sst = strtok(NULL, "|");//读姓名,只读不取
		//tmp.Name = sst;

		sst = strtok(NULL, "|");//读性别,只读不取
		//tmp.Gender = sst;

		sst = strtok(NULL, "|");//读年龄,只读不取
		//tmp.Age = atoi(sst);

		sst = strtok(NULL, "|");//读地址,只读不取
		//tmp.Address = sst;

		sst = strtok(NULL, "|");//读取高数成绩,只读不取
		

		sst = strtok(NULL, "|");//读取面向对象成绩
		score.push_back(std::stod(sst));

	}

	ifs.close();	//关闭文件
}

//绘制直方图
void CMainFrame::DrawHistogram(CDC* pDC, std::vector<double>, int number)
{
	CRect rc;
	GetClientRect(rc);//获取当前客户区大小的矩形
	//将矩形向中心位置缩小（以移动边的方式）（第一个参数是左右，第二个参数是上下）
	rc.DeflateRect(140, 140);
	CBrush brush1(HS_BDIAGONAL, RGB(255, 51, 153));//45 度向上向左向右阴影
	CBrush brush2(HS_FDIAGONAL, RGB(255, 51, 153));//45 度向下向左向右阴影
	//笔是坚实的。 当此笔用于采用边界矩形的任何 GDI 绘图函数中时，图形的尺寸会缩小，使其完全适合边界矩形，同时考虑到笔的宽度。 这仅适用于几何笔。
	CPen pen(PS_INSIDEFRAME, 2, RGB(255, 51, 153));
	int n = 5;//五个区间
	int width = rc.Width() / n;//一个区间的宽度

	int s[5] = { 0 };//区间人数（初始值为零）
	for (int i = 0; i < number; i++)//统计各区间人数
	{
		int m_score = score[i];
		if (m_score < 60) s[0]++;
		else if (m_score < 70) s[1]++;
		else if (m_score < 80) s[2]++;
		else if (m_score < 90) s[3]++;
		else s[4]++;
	}
	int max_s = s[0];
	for (int i = 0; i < n; i++)//找出人数最多的区间
	{
		if (max_s < s[i])
		{
			max_s = s[i];
		}
	}
	int per_Height = rc.Height() / max_s;//算出一个学生代表的矩形高度
	CRect ps_rect(rc);	//直方图的区间矩形
	CRect str_rect(rc);	//用于显示每个区间代表的分数范围
	ps_rect.right = ps_rect.left + width;//确定每个区间的大小
	CPen* oldPen = pDC->SelectObject(&pen);//使画笔生效
	//记录每个区间的范围
	CString str[5] = { _T("<60"),_T("60～70"),_T("70～80"),_T("80～90"),_T(">=90") };
	for (int i = 0; i < n; i++)//从左到右一个个绘制区间
	{
		ps_rect.top = ps_rect.bottom - per_Height * s[i] - 2;	//该区间的高度
		if (i % 2)//每隔一个区间用不同的斜线
		{
			pDC->SelectObject(&brush2);//右斜
		}
		else
		{
			pDC->SelectObject(&brush1);//左斜
		}
		pDC->Rectangle(ps_rect);	//绘制矩形
		if (s[i] >= 0)
		{
			CString str;//用于显示一个区间的人数
			str.Format(_T("%d人"), s[i]);//在矩形中间显示人数
			pDC->DrawText(str, ps_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		//开始处理显示区间分数范围的矩形
		str_rect = ps_rect;
		str_rect.bottom = ps_rect.bottom + 20;
		str_rect.top = ps_rect.bottom + 2;
		pDC->DrawText(str[i], str_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//水平方向移动区间矩形width个长度，否则五个区间都显示在同一块区域
		ps_rect.OffsetRect(width, 0);
	}
	if (m_ChartType == HistogramOfMath)
	{
		CString str2;//用于显示图表参数

		CRect crect(rc);//显示区域
		crect.MoveToXY(900, -250);//移动到右上角
		str2.Format(_T("统计学生总数：%d人"), score.size());//设置显示的内容
		pDC->DrawText(str2, crect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		crect.MoveToXY(900, -275);//往上移动
		str2 = "统计类型：高等数学成绩";//设置显示的内容
		pDC->DrawText(str2, crect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (m_ChartType == HistogramOfOOP)
	{
		CString str2;//用于显示一个区间的人数

		CRect crect(rc);//显示区域
		crect.MoveToXY(900, -250);//移动到右上角
		str2.Format(_T("统计学生总数：%d人"), score.size());//设置显示的内容
		pDC->DrawText(str2, crect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		crect.MoveToXY(900, -275);//往上移动
		str2 = "统计类型：面向对象成绩";//设置显示的内容
		pDC->DrawText(str2, crect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	DeleteObject(brush1);//回收左斜画刷
	DeleteObject(brush2);//回收右斜画刷
	DeleteObject(pen);//回收画笔资源
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 创建类视图
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“类视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建文件视图
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建输出窗口
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	// 创建属性窗口
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“属性”窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFrameWndEx::OnPaint()

}
	

//点击菜单中的“绘制高数直方图”选项
void CMainFrame::OnMathhistogram()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartType = HistogramOfMath;
	LoadMath();

	if (score.size() == 0)
	{
		MessageBox(TEXT("暂无学生成绩数据"));
		return;
	}
	
	// 清空绘制区域
	CRect rectDlg;
	GetClientRect(rectDlg);			// 获得窗体的大小
	int pointWidth = rectDlg.Width();		// 获取窗体宽度
	int pointHeight = rectDlg.Height();		// 获取窗体高度
	RedrawWindow(CRect(0, 0, pointWidth, pointHeight));		// 重绘指定区域


	CDC* pDC = GetWindowDC();
	DrawHistogram(pDC, score, score.size());


	
}


//点击菜单中的“绘制面向对象直方图”选项
void CMainFrame::OnOophistogram()
{
	// TODO: 在此添加命令处理程序代码
	m_ChartType = HistogramOfOOP;
	LoadOOP();

	if (score.size() == 0)
	{
		MessageBox(TEXT("暂无学生成绩数据"));
		return;
	}
	
	// 清空绘制区域
	CRect rectDlg;
	GetClientRect(rectDlg);			// 获得窗体的大小
	int pointWidth = rectDlg.Width();		// 获取窗体宽度
	int pointHeight = rectDlg.Height();		// 获取窗体高度
	RedrawWindow(CRect(0, 0, pointWidth, pointHeight));		// 重绘指定区域

	CDC* pDC = GetWindowDC();
	DrawHistogram(pDC, score, score.size());
}


