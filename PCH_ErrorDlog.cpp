// PCH_ErrorDlog.cpp : implementation file
//

#include "pch.h"
#include "Prefixer.h"
#include "PCH_ErrorDlog.h"
#include "afxdialogex.h"


// PCH_ErrorDlog dialog

IMPLEMENT_DYNAMIC(PCH_ErrorDlog, CDialogEx)

PCH_ErrorDlog::PCH_ErrorDlog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PCH_ERROR, pParent)
{

}

PCH_ErrorDlog::~PCH_ErrorDlog()
{
}

void PCH_ErrorDlog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PCH_ErrorDlog, CDialogEx)
END_MESSAGE_MAP()


// PCH_ErrorDlog message handlers
