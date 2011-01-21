
// ShuffleMP3sInDirectoryDlg.h : header file
//

#pragma once
#include "afxwin.h"

struct flist
{
  unsigned int             num_entries;
  unsigned int             max_entries;
  TCHAR           **files;
};

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
	BOOLEAN systemCommand(const wchar_t* command, 
                             bool bWaitUntilComplete,
                             int  timeoutMs);
	void ErrorMessage(void);
	void AddFile(flist *list, ULONGLONG *Size, TCHAR *Dir, WIN32_FIND_DATA data);
	void FindFiles(flist *list, ULONGLONG *Size, TCHAR *Dir);
	void RandomizeFiles(flist *list);
	TCHAR *GetFileNameFromPath(TCHAR *FullFileName);
	flist list;

public:
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	CListBox LFiles;
	CEdit TDirectory;
	CEdit TStatus;
	afx_msg void OnBnClickedExit();
	CButton CBWrite;
};
