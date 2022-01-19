#pragma once


// PCH_ErrorDlog dialog

class PCH_ErrorDlog : public CDialogEx
{
	DECLARE_DYNAMIC(PCH_ErrorDlog)

public:
	PCH_ErrorDlog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PCH_ErrorDlog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PCH_ERROR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
