
// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

#include <fstream>
#include <string>
#include <vector>

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:
	// 声明一个数组存储学生成绩数据
	std::vector<double> score;

	//记录学生个数
	int count;

	//选择图表种类
	enum ChartType {
		HistogramOfMath, HistogramOfOOP, LineChartOfMath, LineChartOfOOP
	};//0是高数直方图，1是面向对象直方图，2是高数折线图，3是面向对象折线图

	//记录图标种类
	int m_ChartType;

	//载入学生高数成绩
	void LoadMath();

	//载入学生面向对象成绩
	void LoadOOP();

	//绘制直方图
	void DrawHistogram(CDC* pDC, std::vector<double>, int number);

	//绘制折线图
	void DrawLineChart(CDC* pDC, std::vector<double>, int number);


// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnPaint();
	afx_msg void OnMathhistogram();
	afx_msg void OnMathlinegraphs();
	afx_msg void OnOophistogram();
	afx_msg void OnOoplinegraphs();
	
};


