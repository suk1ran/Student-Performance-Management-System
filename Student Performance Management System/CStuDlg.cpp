// CStuDlg.cpp: 实现文件
//

#include "pch.h"
#include "Student Performance Management System.h"
#include "afxdialogex.h"
#include "CStuDlg.h"


#include "afxwin.h"
#include <string>

// CStuDlg 对话框

IMPLEMENT_DYNAMIC(CStuDlg, CDialogEx)

CStuDlg::CStuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STUDENT, pParent)

	, m_number(_T(""))
	, m_name(_T(""))
	, m_age(_T(""))
	, m_address(_T(""))
	, m_math(_T(""))
	, m_oop(_T(""))
{

}

CStuDlg::~CStuDlg()
{
}

void CStuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SHOW, m_ReportList);
	DDX_Text(pDX, IDC_NUMBER, m_number);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_AGE, m_age);
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDX_Text(pDX, IDC_MATH, m_math);
	DDX_Text(pDX, IDC_OOP, m_oop);
	DDX_Control(pDX, IDC_CHOOSE, m_choose);
}


BEGIN_MESSAGE_MAP(CStuDlg, CDialogEx)
ON_BN_CLICKED(IDC_SORT_OOP, &CStuDlg::OnBnClickedSortOop)
ON_BN_CLICKED(IDC_GIRL, &CStuDlg::OnBnClickedGirl)
ON_BN_CLICKED(IDC_ADD, &CStuDlg::OnBnClickedAdd)
ON_BN_CLICKED(IDC_BOY, &CStuDlg::OnBnClickedBoy)
ON_BN_CLICKED(IDC_SORT_MATH, &CStuDlg::OnBnClickedSortMath)
ON_BN_CLICKED(IDC_CLEAR, &CStuDlg::OnBnClickedClear)
ON_BN_CLICKED(IDC_FIND, &CStuDlg::OnBnClickedFind)
ON_BN_CLICKED(IDC_UPDATE, &CStuDlg::OnBnClickedUpdate)
ON_BN_CLICKED(IDC_DELETE, &CStuDlg::OnBnClickedDelete)
ON_NOTIFY(NM_CLICK, IDC_SHOW, &CStuDlg::OnNMClickShow)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_SHOW, &CStuDlg::OnHdnItemchangedShow)
END_MESSAGE_MAP()


// CStuDlg 消息处理程序


BOOL CStuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// TODO:  在此添加额外的初始化
	m_ReportList.SetExtendedStyle(m_ReportList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString str[] = { TEXT("学号"),TEXT("姓名"),TEXT("性别"),TEXT("年龄"),TEXT("地址"),TEXT("高等数学成绩"),TEXT("面向对象成绩") };
	for (int i = 0; i < 7; i++)
	{
		m_ReportList.InsertColumn(i, str[i], LVCFMT_LEFT, 120);
	}
	Management file;
	file.LoadFromFile();
	int i = 0;
	for (list<Student>::iterator it = file.StuList.begin(); it != file.StuList.end(); it++)
	{
		CString str;
		str = CString(it->Number.c_str());
		m_ReportList.InsertItem(i, str);

		str = CString(it->Name.c_str());
		m_ReportList.SetItemText(i, 1, str);

		str = CString(it->Gender.c_str());
		m_ReportList.SetItemText(i, 2, str);

		str.Format(_T("%d"), it->Age);
		m_ReportList.SetItemText(i, 3, str);

		str = CString(it->Address.c_str());
		m_ReportList.SetItemText(i, 4, str);

		str.Format(_T("%.2lf"), it->MathScore);
		m_ReportList.SetItemText(i, 5, str);

		str.Format(_T("%.2lf"), it->OOPScore);
		m_ReportList.SetItemText(i, 6, str);

		i++;
	}

	m_choose.AddString(TEXT("降序"));//0是降序
	m_choose.AddString(TEXT("升序"));//1是升序
	m_choose.SetCurSel(0);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//添加学生信息到链表中
void Management::AddStudent(CString number, CString name, CString gender, int age, CString address, double math, double OOP)
{
	Student tmp;

	if (StuList.size() >= 0)
	{
		//信息有效
		if (!number.IsEmpty() && math > 0 && age > 0 && OOP > 0)
		{
			CStringA str;                     //CString 转CStringA
			str = number;
			tmp.Number = str.GetBuffer();	  //CString 转为char *，学号

			str = name;
			tmp.Name = str.GetBuffer();       //姓名

			str = gender;
			tmp.Gender = str.GetBuffer();     //性别

			tmp.Age = age;		              //年龄

			str = address;
			tmp.Address = str.GetBuffer();    //地址

			tmp.MathScore = math;		      //高数成绩

			tmp.OOPScore = OOP;               //面向对象成绩

			StuList.push_back(tmp);		      //尾插进入链表
		}
	}
}

void Management::DeleteStudent()
{

}

void Management::UpdateStudent()
{
	
}

//高数升序
bool SortMathAsc(const Student& s1, const Student& s2)
{
	return s1.MathScore < s2.MathScore;
}

//高数降序
bool SortMathDesc(const Student& s1, const Student& s2)
{
	return s1.MathScore > s2.MathScore;
}

//面向对象升序
bool SortOOPAsc(const Student& s1, const Student& s2)
{
	return s1.OOPScore < s2.OOPScore;
}

//面向对象降序
bool SortOOPDesc(const Student& s1, const Student& s2)
{
	return s1.OOPScore > s2.OOPScore;
}

//写入学生信息
void Management::SaveToFile()
{
	ofstream ofs("StudentData.txt");//写文件(默认ios::out，每次写都是先清空后写)
	
	string header = "学号 | 姓名 | 性别 | 年龄 | 地址 | 高等数学成绩 | 面向对象成绩 | ";

	if (StuList.size() >= 0)
	{
		ofs << header << endl;	//写入表头

		//通过迭代器取出链表内容，写入文件，以"|"分割，结尾加换行
		for (list<Student>::iterator it = StuList.begin(); it != StuList.end(); it++)
		{
			ofs << it->Number << "|";
			ofs << it->Name << "|";
			ofs << it->Gender << "|";
			ofs << it->Age << "|";
			ofs << it->Address << "|";
			ofs << it->MathScore << "|";
			ofs << it->OOPScore << endl;
		}
	}
	ofs.close();
}

//读取学生信息
void Management::LoadFromFile()
{
	ifstream ifs("StudentData.txt");//读文件
	
	
	char buf[2048] = { 0 };
	num = 0;	//初始化学生数量为0

	StuList.clear();

	//取出表头
	ifs.getline(buf, sizeof(buf));

	while (!ifs.eof())	//没到文件结尾
	{
		Student tmp;

		ifs.getline(buf, sizeof(buf));	//读取一行
		num++;	//学生数量加一

		char* sst = strtok(buf, "|");	//以"|"分隔
		if (sst != NULL)
		{
			tmp.Number = sst;	//读取学号
		}
		else
		{
			break;
		}

		sst = strtok(NULL, "|");//读取姓名
		tmp.Name = sst;

		sst = strtok(NULL, "|");//读取性别
		tmp.Gender = sst;

		sst = strtok(NULL, "|");//读取年龄
		tmp.Age = atoi(sst);

		sst = strtok(NULL, "|");//读取地址
		tmp.Address = sst;

		sst = strtok(NULL, "|");//读取高数成绩
		tmp.MathScore = stod(sst);

		sst = strtok(NULL, "|");//读取面向对象成绩
		tmp.OOPScore = stod(sst);

		StuList.push_back(tmp);	//尾插进入链表
	}

	ifs.close();	//关闭文件
}

//面向对象排序
void CStuDlg::OnBnClickedSortOop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ReportList.DeleteAllItems();
	Management file;
	file.LoadFromFile();
	int select = m_choose.GetCurSel();
	if (select == 0)//降序
	{
		file.StuList.sort(SortOOPDesc);
	}
	else//升序
	{
		file.StuList.sort(SortOOPAsc);
	}
	int i = 0;
	for (list<Student>::iterator it = file.StuList.begin(); it != file.StuList.end(); it++)
	{
		CString str;
		str = CString(it->Number.c_str());
		m_ReportList.InsertItem(i, str);

		str = CString(it->Name.c_str());
		m_ReportList.SetItemText(i, 1, str);

		str = CString(it->Gender.c_str());
		m_ReportList.SetItemText(i, 2, str);

		str.Format(_T("%d"), it->Age);
		m_ReportList.SetItemText(i, 3, str);

		str = CString(it->Address.c_str());
		m_ReportList.SetItemText(i, 4, str);

		str.Format(_T("%.2lf"), it->MathScore);
		m_ReportList.SetItemText(i, 5, str);

		str.Format(_T("%.2lf"), it->OOPScore);
		m_ReportList.SetItemText(i, 6, str);

		i++;
	}
}

//修改性别（女）
void CStuDlg::OnBnClickedGirl()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gender = "女";
}

//添加学生信息
void CStuDlg::OnBnClickedAdd()
{
	
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_number.IsEmpty() || m_name.IsEmpty() ||    m_age.IsEmpty() || m_address.IsEmpty() || m_math.IsEmpty() || m_oop.IsEmpty())
	{
		MessageBox(TEXT("学生信息不能为空"));
		return;
	}
	Management file;
	file.LoadFromFile();
	for (list<Student>::iterator it = file.StuList.begin(); it != file.StuList.end(); it++)
	{
		string i = it->Number;
		CString str;
		str = CString(it->Number.c_str());
		if (str == m_number)
		{
			MessageBox(TEXT("该学号已存在"));
			return;
		}
	}

	//把m_xx的部分数据（CString类型）转换为所需类型
	int age;
	double math, oop;
	age = _ttoi(m_age);
	math = _wtof(m_math);
	oop = _wtof(m_oop);
	file.AddStudent(m_number, m_name, m_gender, age, m_address, math, oop);
	file.SaveToFile();
	


	//重新载入文件中的数据，实现实时显示效果
	m_ReportList.DeleteAllItems();
	m_ReportList.SetExtendedStyle(m_ReportList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString str[] = { TEXT("学号"),TEXT("姓名"),TEXT("性别"),TEXT("年龄"),TEXT("地址"),TEXT("高等数学成绩"),TEXT("面向对象成绩") };
	for (int i = 0; i < 7; i++)
	{
		m_ReportList.InsertColumn(i, str[i], LVCFMT_LEFT, 120);
	}
	Management f;
	f.LoadFromFile();
	int i = 0;
	for (list<Student>::iterator it = f.StuList.begin(); it != f.StuList.end(); it++)
	{
		CString str;
		str = CString(it->Number.c_str());
		m_ReportList.InsertItem(i, str);

		str = CString(it->Name.c_str());
		m_ReportList.SetItemText(i, 1, str);

		str = CString(it->Gender.c_str());
		m_ReportList.SetItemText(i, 2, str);

		str.Format(_T("%d"), it->Age);
		m_ReportList.SetItemText(i, 3, str);

		str = CString(it->Address.c_str());
		m_ReportList.SetItemText(i, 4, str);

		str.Format(_T("%.2lf"), it->MathScore);
		m_ReportList.SetItemText(i, 5, str);

		str.Format(_T("%.2lf"), it->OOPScore);
		m_ReportList.SetItemText(i, 6, str);

		i++;
	}

	MessageBox(TEXT("添加成功"));
}

//修改性别（男）
void CStuDlg::OnBnClickedBoy()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gender = "男";
}

//重载==运算符
bool operator==(const Student& s1, const Student& s2)
{
	if (s1.Number == s2.Number)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//高数排序
void CStuDlg::OnBnClickedSortMath()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ReportList.DeleteAllItems();
	Management file;
	file.LoadFromFile();
	int select = m_choose.GetCurSel();
	if (select == 0)//降序
	{
		file.StuList.sort(SortMathDesc);
	}
	else//升序
	{
		file.StuList.sort(SortMathAsc);
	}
	int i = 0;
	for (list<Student>::iterator it = file.StuList.begin(); it != file.StuList.end(); it++)
	{
		CString str;
		str = CString(it->Number.c_str());
		m_ReportList.InsertItem(i, str);

		str = CString(it->Name.c_str());
		m_ReportList.SetItemText(i, 1, str);

		str = CString(it->Gender.c_str());
		m_ReportList.SetItemText(i, 2, str);

		str.Format(_T("%d"), it->Age);
		m_ReportList.SetItemText(i, 3, str);

		str = CString(it->Address.c_str());
		m_ReportList.SetItemText(i, 4, str);

		str.Format(_T("%.2lf"), it->MathScore);
		m_ReportList.SetItemText(i, 5, str);

		str.Format(_T("%.2lf"), it->OOPScore);
		m_ReportList.SetItemText(i, 6, str);

		i++;
	}
}

//选中表格后能够把信息同步到编辑框中，但下面的更新函数是上位替代，这个已经可以注释掉了
void CStuDlg::OnNMClickShow(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	CString strCol;    // 选择某一列 
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		strCol = m_ReportList.GetItemText(pNMListView->iItem, 0);//学号
		// 将选择的文本显示到编辑框中   
		SetDlgItemText(IDC_NUMBER, strCol);

		strCol = m_ReportList.GetItemText(pNMListView->iItem, 1);//姓名
		SetDlgItemText(IDC_NAME, strCol);

		strCol = m_ReportList.GetItemText(pNMListView->iItem, 2);//性别
		if (strCol == "男")
		{
			CButton* radio = (CButton*)GetDlgItem(IDC_BOY);
			radio->SetCheck(1);
			CButton* radio2 = (CButton*)GetDlgItem(IDC_GIRL);
			radio2->SetCheck(0);
			OnBnClickedBoy();
		}
		else if (strCol == "女")
		{
			CButton* radio = (CButton*)GetDlgItem(IDC_GIRL);
			radio->SetCheck(1);
			CButton* radio2 = (CButton*)GetDlgItem(IDC_BOY);
			radio2->SetCheck(0);
			OnBnClickedGirl();
		}

		strCol = m_ReportList.GetItemText(pNMListView->iItem, 3);//年龄
		SetDlgItemText(IDC_AGE, strCol);

		strCol = m_ReportList.GetItemText(pNMListView->iItem, 4);//地址
		SetDlgItemText(IDC_ADDRESS, strCol);

		strCol = m_ReportList.GetItemText(pNMListView->iItem, 5);//高数
		SetDlgItemText(IDC_MATH, strCol);

		strCol = m_ReportList.GetItemText(pNMListView->iItem, 6);//C++
		SetDlgItemText(IDC_OOP, strCol);

	}
	
}

//清空编辑框中的内容
void CStuDlg::OnBnClickedClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_number = "";
	m_name = "";
	m_age = "";
	m_address = "";
	m_math = "";
	m_oop = "";
	if (m_gender == "男")
	{
		CButton* radio = (CButton*)GetDlgItem(IDC_BOY);
		radio->SetCheck(0);
	}
	else if (m_gender == "女")
	{
		CButton* radio2 = (CButton*)GetDlgItem(IDC_GIRL);
		radio2->SetCheck(0);
	}
	UpdateData(FALSE);
}

//更新学生信息
void CStuDlg::OnBnClickedUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_number.IsEmpty() || m_name.IsEmpty() || m_age.IsEmpty() || m_address.IsEmpty() || m_math.IsEmpty() || m_oop.IsEmpty())
	{
		MessageBox(TEXT("学生信息不能为空"));
		return;
	}
	Management file;
	file.LoadFromFile();
	CString s;//用来锁定链表中对应的学号
	for (list<Student>::iterator it = file.StuList.begin(); it != file.StuList.end(); it++)
	{
		s = CString(it->Number.c_str());
		if (s == m_number)
		{
			//把m_xx的部分数据（CString类型）转换为所需类型
			int age;
			double math, oop;
			age = _ttoi(m_age);
			math = _wtof(m_math);
			oop = _wtof(m_oop);

			CStringA str;                     //CString 转CStringA
			str = m_number;
			it->Number = str.GetBuffer();	  //CString 转为char *，学号

			str = m_name;
			it->Name = str.GetBuffer();       //姓名

			str = m_gender;
			it->Gender = str.GetBuffer();     //性别

			it->Age = age;		              //年龄

			str = m_address;
			it->Address = str.GetBuffer();    //地址

			it->MathScore = math;		      //高数成绩

			it->OOPScore = oop;               //面向对象成绩

			break;//当找到目标学号并完成操作后退出遍历，让s保存的数据仍为目标学号
		}
	}
	if (s != m_number)
	{
		MessageBox(TEXT("该学号不存在"));
		return;
	}
	file.SaveToFile();

	//重新载入文件中的数据，实现实时显示效果
	m_ReportList.DeleteAllItems();
	m_ReportList.SetExtendedStyle(m_ReportList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString str[] = { TEXT("学号"),TEXT("姓名"),TEXT("性别"),TEXT("年龄"),TEXT("地址"),TEXT("高等数学成绩"),TEXT("面向对象成绩") };
	for (int i = 0; i < 7; i++)
	{
		m_ReportList.InsertColumn(i, str[i], LVCFMT_LEFT, 120);
	}
	Management f;
	f.LoadFromFile();
	int i = 0;
	for (list<Student>::iterator it = f.StuList.begin(); it != f.StuList.end(); it++)
	{
		CString str;
		str = CString(it->Number.c_str());
		m_ReportList.InsertItem(i, str);

		str = CString(it->Name.c_str());
		m_ReportList.SetItemText(i, 1, str);

		str = CString(it->Gender.c_str());
		m_ReportList.SetItemText(i, 2, str);

		str.Format(_T("%d"), it->Age);
		m_ReportList.SetItemText(i, 3, str);

		str = CString(it->Address.c_str());
		m_ReportList.SetItemText(i, 4, str);

		str.Format(_T("%.2lf"), it->MathScore);
		m_ReportList.SetItemText(i, 5, str);

		str.Format(_T("%.2lf"), it->OOPScore);
		m_ReportList.SetItemText(i, 6, str);

		i++;
	}
	
	MessageBox(TEXT("修改成功"));
}

//根据学号查询学生信息
void CStuDlg::OnBnClickedFind()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_number.IsEmpty())
	{
		MessageBox(TEXT("请输入正确的学号"));
		return;
	}
	Management file;
	file.LoadFromFile();
	CString s;//用来锁定链表中对应的学号
	int index = -1;//用来获取表格的索引
	for (list<Student>::iterator it = file.StuList.begin(); it != file.StuList.end(); it++)
	{
		s = CString(it->Number.c_str());
		index++;
		if (s == m_number)
		{
			CString str;
			str = CString(it->Number.c_str());
			m_number = str;

			str = CString(it->Name.c_str());
			m_name = str;

			str = CString(it->Gender.c_str());
			m_gender = str;

			str.Format(_T("%d"), it->Age);
			m_age = str;

			str = CString(it->Address.c_str());
			m_address = str;

			str.Format(_T("%.2lf"), it->MathScore);
			m_math = str;

			str.Format(_T("%.2lf"), it->OOPScore);
			m_oop = str;

			break;//当找到目标学号并完成操作后退出遍历，让s保存的数据仍为目标学号
		}
	}
	if (s != m_number)
	{
		MessageBox(TEXT("未查询到该学号"));
		return;
	}
	if (m_gender == "男")
	{
		CButton* radio = (CButton*)GetDlgItem(IDC_BOY);
		radio->SetCheck(1);
		CButton* radio2 = (CButton*)GetDlgItem(IDC_GIRL);
		radio2->SetCheck(0);
	}
	else if (m_gender == "女")
	{
		CButton* radio2 = (CButton*)GetDlgItem(IDC_GIRL);
		radio2->SetCheck(1);
		CButton* radio = (CButton*)GetDlgItem(IDC_BOY);
		radio->SetCheck(0);
	}

	UpdateData(FALSE);

	for (int i = m_ReportList.GetItemCount() - 1; i >= 0; i--)//遍历表格的行数
	{
		if (LVIS_SELECTED == m_ReportList.GetItemState(i, LVIS_SELECTED))//该行是否被选中
		{
			m_ReportList.SetItemState(i, 0, -1);  // 把第i行设置为没有选中的状态
		}
	}
	
	m_ReportList.SetFocus();//当前行设置为焦点
	m_ReportList.SetItemState(index, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);//高亮该行
	m_ReportList.SetSelectionMark(index);//获取该行信息

}

//无论是鼠标点击还是键盘控制都能更新编辑框中的信息
void CStuDlg::OnHdnItemchangedShow(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CString strCol;    // 选择某一列 
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uChanged == LVIF_STATE)
	{
		if (pNMListView->uNewState)
		{
			int nIndex = pNMListView->iItem;

			//要进行的操作
			if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
			{
				// 获取被选择列表项第一个子项的文本   
				strCol = m_ReportList.GetItemText(pNMListView->iItem, 0);//学号
				// 将选择的文本显示到编辑框中   
				SetDlgItemText(IDC_NUMBER, strCol);

				strCol = m_ReportList.GetItemText(pNMListView->iItem, 1);//姓名
				SetDlgItemText(IDC_NAME, strCol);

				strCol = m_ReportList.GetItemText(pNMListView->iItem, 2);//性别
				if (strCol == "男")
				{
					CButton* radio = (CButton*)GetDlgItem(IDC_BOY);
					radio->SetCheck(1);
					CButton* radio2 = (CButton*)GetDlgItem(IDC_GIRL);
					radio2->SetCheck(0);
					OnBnClickedBoy();
				}
				else if (strCol == "女")
				{
					CButton* radio = (CButton*)GetDlgItem(IDC_GIRL);
					radio->SetCheck(1);
					CButton* radio2 = (CButton*)GetDlgItem(IDC_BOY);
					radio2->SetCheck(0);
					OnBnClickedGirl();
				}

				strCol = m_ReportList.GetItemText(pNMListView->iItem, 3);//年龄
				SetDlgItemText(IDC_AGE, strCol);

				strCol = m_ReportList.GetItemText(pNMListView->iItem, 4);//地址
				SetDlgItemText(IDC_ADDRESS, strCol);

				strCol = m_ReportList.GetItemText(pNMListView->iItem, 5);//高数
				SetDlgItemText(IDC_MATH, strCol);

				strCol = m_ReportList.GetItemText(pNMListView->iItem, 6);//C++
				SetDlgItemText(IDC_OOP, strCol);

			}
		}

	}



	*pResult = 0;


}

//删除学生信息
void CStuDlg::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_number.IsEmpty())
	{
		MessageBox(TEXT("学号不能为空"));
		return;
	}
	Management file;
	file.LoadFromFile();
	CString s;//用来锁定链表中对应的学号
	for (list<Student>::iterator it = file.StuList.begin(); it != file.StuList.end(); it++)
	{
		s = CString(it->Number.c_str());
		if (s == m_number)
		{
			file.StuList.remove(*it);
			//此时还不能改变m_number的值，否则下方的if判定会失效
			m_name = "";
			m_age = "";
			m_address = "";
			m_math = "";
			m_oop = "";
			if (m_gender == "男")
			{
				CButton* radio = (CButton*)GetDlgItem(IDC_BOY);
				radio->SetCheck(0);
			}
			else if (m_gender == "女")
			{
				CButton* radio2 = (CButton*)GetDlgItem(IDC_GIRL);
				radio2->SetCheck(0);
			}

			break;//当找到目标学号并完成操作后退出遍历，让s保存的数据仍为目标学号
		}
	}
	if (s != m_number)
	{
		MessageBox(TEXT("该学号不存在"));
		return;
	}
	m_number = "";
	UpdateData(FALSE);
	file.SaveToFile();

	//重新载入文件中的数据，实现实时显示效果
	m_ReportList.DeleteAllItems();
	m_ReportList.SetExtendedStyle(m_ReportList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString str[] = { TEXT("学号"),TEXT("姓名"),TEXT("性别"),TEXT("年龄"),TEXT("地址"),TEXT("高等数学成绩"),TEXT("面向对象成绩") };
	for (int i = 0; i < 7; i++)
	{
		m_ReportList.InsertColumn(i, str[i], LVCFMT_LEFT, 120);
	}
	Management f;
	f.LoadFromFile();
	int i = 0;
	for (list<Student>::iterator it = f.StuList.begin(); it != f.StuList.end(); it++)
	{
		CString str;
		str = CString(it->Number.c_str());
		m_ReportList.InsertItem(i, str);

		str = CString(it->Name.c_str());
		m_ReportList.SetItemText(i, 1, str);

		str = CString(it->Gender.c_str());
		m_ReportList.SetItemText(i, 2, str);

		str.Format(_T("%d"), it->Age);
		m_ReportList.SetItemText(i, 3, str);

		str = CString(it->Address.c_str());
		m_ReportList.SetItemText(i, 4, str);

		str.Format(_T("%.2lf"), it->MathScore);
		m_ReportList.SetItemText(i, 5, str);

		str.Format(_T("%.2lf"), it->OOPScore);
		m_ReportList.SetItemText(i, 6, str);

		i++;
	}

	MessageBox(TEXT("删除成功"));
}
