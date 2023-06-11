#pragma once
#include "afxdialogex.h"

#include <fstream>
#include <iostream>

#include <string>
#include <list>



#include <afx.h>
#include <afxwin.h>


#include "afxtempl.h"
using namespace std;
using std::string;
using std::list;


// CStuDlg 对话框

class CStuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStuDlg)

public:
	CStuDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStuDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEdit1();
	virtual BOOL OnInitDialog();
	
	CListCtrl m_ReportList;
	afx_msg void OnBnClickedSortOop();
	afx_msg void OnBnClickedGirl();
	CString m_number;
	CString m_name;
	CString m_age;
	CString m_address;
	CString m_math;
	CString m_oop;

	//不是通过添加变量
	CString m_gender;

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedBoy();
	CComboBox m_choose;
	afx_msg void OnBnClickedSortMath();
	afx_msg void OnNMClickShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedFind();
	afx_msg void OnHdnItemchangedShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedDelete();
};


//学生类
class Student
{
public:
	string Number;//学生学号
	string Name;//学生姓名
	string Gender;//学生性别
	int Age;//学生年龄
	string Address;//学生地址
	double MathScore;//高等数学成绩
	double OOPScore;//面向对象成绩
};

//管理学生的类
class Management
{
public:
	
	list<Student> StuList;//学生链表
	int num;//记录学生个数

	//添加学生
	void AddStudent(CString number, CString name, CString gender, int age, CString address, double math, double OOP);

	//删除学生
	void DeleteStudent();

	//修改学生
	void UpdateStudent();

	//高数升序
	bool SortMathAsc(const Student& s1, const Student& s2);

	//高数降序
	bool SortMathDesc(const Student& s1, const Student& s2);

	//面向对象升序
	bool SortOOPAsc(const Student& s1, const Student& s2);

	//面向对象降序
	bool SortOOPDesc(const Student& s1, const Student& s2);

	//写入学生数据
	void SaveToFile();

	//读取学生数据
	void LoadFromFile();

};

//重载==运算符
bool operator == (const Student& s1, const Student& s2);
