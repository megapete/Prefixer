
// PrefixerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Prefixer.h"
#include "PrefixerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPrefixerDlg dialog



CPrefixerDlg::CPrefixerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PREFIXER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPrefixerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_FOLDERNAME, folderNameLabel);
	DDX_Control(pDX, IDC_FILELIST, currentFileList);
	DDX_Control(pDX, IDC_EDIT_OLDPREFIX, oldPrefixEditBox);
	DDX_Control(pDX, IDC_EDIT_NEWPREFIX, newPrefixEditBox);
}

BEGIN_MESSAGE_MAP(CPrefixerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPrefixerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CPrefixerDlg::OnSelectFolder)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_FILENAMES, &CPrefixerDlg::OnBnClickedButtonChangeFilenames)
	ON_LBN_SELCHANGE(IDC_FILELIST, &CPrefixerDlg::OnLbnSelchangeFilelist)
END_MESSAGE_MAP()


// CPrefixerDlg message handlers

BOOL CPrefixerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// Set the initial folder to be the My Documents folder of the user
	REFKNOWNFOLDERID rfid = FOLDERID_Documents;
	DWORD dwFlags = 0;
	PWSTR ppszPath;

	HRESULT result = SHGetKnownFolderPath(rfid, dwFlags, NULL, &ppszPath);
	if (result == S_OK) {

		this->folderPath = CString(ppszPath);
		TRACE(_T("%s\n"), (LPCTSTR)this->folderPath);
		this->folderNameLabel.SetWindowText(folderPath);
		this->SetUpFileList(this->folderPath);
	}
	else {
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPrefixerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPrefixerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPrefixerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CPrefixerDlg::SetUpFileList(CString folderPath)
{
	// This comes from the Microsoft page for CFileFind
	CFileFind finder;

	CString stringToFind = folderPath;
	stringToFind += _T("\\*.*");

	BOOL gotFile = finder.FindFile(stringToFind);

	if (!gotFile) {

		return false;
	}

	// clear the array of file names
	this->fileNames.RemoveAll();

	while (gotFile) {

		gotFile = finder.FindNextFile();

		// skip the files . and ..
		if (finder.IsDots() || finder.IsDirectory()) {

			continue;
		}

		CString fileString = finder.GetFileName();
		this->fileNames.Add(fileString);
		TRACE(_T("%s\n"), (LPCTSTR)fileString);
	}

	this->currentFileList.ResetContent();

	// we'll need to adjust things for long file names so that we can scroll horizontally
	
	int dx = 0;
	CDC* pDC = this->currentFileList.GetDC();

	for (int i = 0; i < this->fileNames.GetSize(); i++)
	{
		this->currentFileList.AddString(this->fileNames[i]);

		CSize sz = pDC->GetTextExtent(this->fileNames[i]);

		if (sz.cx > dx) {

			dx = sz.cx;
		}
	}

	this->currentFileList.ReleaseDC(pDC);

	if (this->currentFileList.GetHorizontalExtent() < dx) {

		this->currentFileList.SetHorizontalExtent(dx);
	}

	return true;
}



void CPrefixerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}



void CPrefixerDlg::OnSelectFolder()
{
	// TODO: Add your control notification handler code here
	
	CFolderPickerDialog selectFolderDlog;

	INT_PTR result = selectFolderDlog.DoModal();

	if (result == IDOK) {

		this->folderPath = selectFolderDlog.GetFolderPath();
		this->folderNameLabel.SetWindowText(folderPath);

		TRACE(_T("%s\n"), (LPCTSTR)this->folderPath);
		this->SetUpFileList(this->folderPath);
	}
}



void CPrefixerDlg::OnBnClickedButtonChangeFilenames()
{
	// TODO: Add your control notification handler code here
	TCHAR lineBuff[1000];
	int numchars = this->oldPrefixEditBox.GetLine(0, lineBuff, 999);
	lineBuff[numchars] = 0;
	
	CString oldPrefix = CString(lineBuff);

	numchars = this->newPrefixEditBox.GetLine(0, lineBuff, 999);
	lineBuff[numchars] = 0;

	CString newPrefix = CString(lineBuff);

	if (oldPrefix.GetLength() == 0 || newPrefix.GetLength() == 0) {

		TRACE(_T("Zero-length string encountered!\n"));
		return;
	}

	for (int i = 0; i < this->fileNames.GetSize(); i++)
	{
		int index = this->fileNames[i].Find(oldPrefix);

		if (index == 0) {

			CString oldFilePath = this->folderPath;
			oldFilePath += _T("\\");
			oldFilePath += this->fileNames[i];

			CString newFileName = this->fileNames[i];
			newFileName.Replace(oldPrefix, newPrefix);
			CString newFilePath = this->folderPath;
			newFilePath += _T("\\");
			newFilePath += newFileName;

			CFile::Rename(oldFilePath, newFilePath);
		}
	}

	this->SetUpFileList(this->folderPath);

}


void CPrefixerDlg::OnLbnSelchangeFilelist()
{
	// TODO: Add your control notification handler code here
	CString newSelection;

	int currentSelectionIndex = this->currentFileList.GetCurSel();

	if (currentSelectionIndex == LB_ERR) {

		return;
	}

	this->currentFileList.GetText(currentSelectionIndex, newSelection);

	this->oldPrefixEditBox.SetWindowText(newSelection);
}
