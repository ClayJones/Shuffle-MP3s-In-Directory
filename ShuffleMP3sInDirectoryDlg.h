
// ShuffleMP3sInDirectoryDlg.h : header file
//

#pragma once


// CShuffleMP3sInDirectoryDlg dialog
class CShuffleMP3sInDirectoryDlg : public CDialog
{
// Construction
public:
	CShuffleMP3sInDirectoryDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SHUFFLEMP3SINDIRECTORY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
