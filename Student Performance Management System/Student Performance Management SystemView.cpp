
// Student Performance Management SystemView.cpp: CStudentPerformanceManagementSystemView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Student Performance Management System.h"
#endif

#include "Student Performance Management SystemDoc.h"
#include "Student Performance Management SystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStudentPerformanceManagementSystemView

IMPLEMENT_DYNCREATE(CStudentPerformanceManagementSystemView, CView)

BEGIN_MESSAGE_MAP(CStudentPerformanceManagementSystemView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CStudentPerformanceManagementSystemView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CStudentPerformanceManagementSystemView 构造/析构

CStudentPerformanceManagementSystemView::CStudentPerformanceManagementSystemView() noexcept
{
	// TODO: 在此处添加构造代码

}

CStudentPerformanceManagementSystemView::~CStudentPerformanceManagementSystemView()
{
}

BOOL CStudentPerformanceManagementSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CStudentPerformanceManagementSystemView 绘图

void CStudentPerformanceManagementSystemView::OnDraw(CDC* /*pDC*/)
{
	CStudentPerformanceManagementSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CStudentPerformanceManagementSystemView 打印


void CStudentPerformanceManagementSystemView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CStudentPerformanceManagementSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CStudentPerformanceManagementSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CStudentPerformanceManagementSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CStudentPerformanceManagementSystemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStudentPerformanceManagementSystemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CStudentPerformanceManagementSystemView 诊断

#ifdef _DEBUG
void CStudentPerformanceManagementSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CStudentPerformanceManagementSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStudentPerformanceManagementSystemDoc* CStudentPerformanceManagementSystemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStudentPerformanceManagementSystemDoc)));
	return (CStudentPerformanceManagementSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CStudentPerformanceManagementSystemView 消息处理程序
