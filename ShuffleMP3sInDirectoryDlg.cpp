
// ShuffleMP3sInDirectoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShuffleMP3sInDirectory.h"
#include "ShuffleMP3sInDirectoryDlg.h"
#include "regkey.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShuffleMP3sInDirectoryDlg dialog




CShuffleMP3sInDirectoryDlg::CShuffleMP3sInDirectoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShuffleMP3sInDirectoryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShuffleMP3sInDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, LFiles);
	DDX_Control(pDX, IDC_EDIT1, TDirectory);
	DDX_Control(pDX, IDC_EDIT2, TStatus);
	DDX_Control(pDX, IDC_BUTTON2, CBWrite);
}

BEGIN_MESSAGE_MAP(CShuffleMP3sInDirectoryDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CShuffleMP3sInDirectoryDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON2, &CShuffleMP3sInDirectoryDlg::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDCANCEL, &CShuffleMP3sInDirectoryDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CShuffleMP3sInDirectoryDlg message handlers

BOOL CShuffleMP3sInDirectoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	memset(&list, 0, sizeof(list));

	RegKey netKey(HKEY_CURRENT_USER);
	TCHAR KeyName[256];
	_tcscpy_s(KeyName, 256, _T("Software\\Clay\\ShuffleMP3sInDirectory"));

	netKey.Open(KeyName, KEY_ALL_ACCESS);

	if(!netKey)
		netKey.Create(KeyName, KEY_ALL_ACCESS);

	if (netKey)
	{
		RegValue instValS = netKey[_T("Directory")];
        if (instValS.Type == REG_SZ)
		{
			TDirectory.SetWindowTextW(instValS);
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShuffleMP3sInDirectoryDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShuffleMP3sInDirectoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOLEAN CShuffleMP3sInDirectoryDlg::systemCommand(const wchar_t* command, 
                             bool bWaitUntilComplete,
                             int  timeoutMs)
{
    bool result = false;
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    if ( timeoutMs <= 0 ) 
        timeoutMs = INFINITE;

    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    
    // The Microsoft documentation says:
    //     The Unicode version of this function, CreateProcessW, can modify the
    // contents of this string (2nd parameter). Therefore, this parameter cannot
    // be a pointer to read-only memory (such as a const variable or a literal
    // string). If this parameter is a constant string, the function may cause
    // an access violation.
    size_t   cmdLen = wcslen(command);
    wchar_t* cmdCopy = new wchar_t[cmdLen + 1];
    wcscpy_s(cmdCopy, cmdLen + 1, command);

    int retVal = CreateProcessW(NULL, // application name
                               cmdCopy, // command line
                               NULL,  // process attributes
                               NULL,  // thread attributes
                               FALSE, // bInheritHandles
                               0,     // creation flags
                               NULL,  // environment
                               NULL,  // current directory
                               &si,   // startup info
                               &pi);  // process information
    if (retVal)
    {
        // Create process was successful
        result = true;
        if (bWaitUntilComplete)
        {
            // Block to prevent OptiView from hanging up??
            retVal = WaitForSingleObject(pi.hProcess, timeoutMs);
            if (retVal == WAIT_TIMEOUT)
            {
                result = false;
            }
        }
        CloseHandle(pi.hThread);
    }

    delete [] cmdCopy;

    return result;
}



void CShuffleMP3sInDirectoryDlg::ErrorMessage(void)
{
  LPVOID  lpMsgBuf;
  FormatMessage
  (
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Default language
    (LPTSTR) & lpMsgBuf,
    0,
    NULL
  );
  TStatus.SetWindowText((wchar_t *) lpMsgBuf);
  LocalFree(lpMsgBuf);
}

void CShuffleMP3sInDirectoryDlg::AddFile(flist *list, ULONGLONG *Size, TCHAR *Dir, WIN32_FIND_DATA data)
{
	int len = _tcslen(Dir) + _tcslen(data.cFileName) + 2;
	TCHAR *file = new TCHAR[len];
	_tcscpy_s(file, len, Dir);
	_tcscat_s(file, len, data.cFileName);
	*Size += data.nFileSizeLow;  // If you have a single mp3 larger than 4 gigs, you're out of luck.

	if (list->num_entries == list->max_entries)
	{
		int             newsize = list->max_entries == 0 ? 16 : list->max_entries + 16;
		TCHAR **temp = (TCHAR **) realloc(list->files, newsize * sizeof(TCHAR **));
		if (temp == NULL)
		{
			fprintf(stderr, "Out of memory\n");
			exit(1);
		}
		else
		{
			list->max_entries = newsize;
			list->files = temp;
		}
	}

	list->files[list->num_entries++] = file;
}


void CShuffleMP3sInDirectoryDlg::FindFiles(flist *list, ULONGLONG *Size, TCHAR *Dir)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE hFind;
   int newlen = _tcslen(Dir) + 10;
   PTCHAR Search = new TCHAR[newlen];
   if(Search == NULL)
	   return;

   _tcscpy_s(Search, newlen, Dir);
   _tcscat_s(Search, newlen, _T("*.mp3"));

//   _tprintf (TEXT("Target file is %s\n"), Search);
   hFind = FindFirstFile(Search, &FindFileData);
   if (hFind != INVALID_HANDLE_VALUE) 
   {
		do
		{
			if (_tcscmp(FindFileData.cFileName, _T(".")) == 0 || _tcscmp(FindFileData.cFileName, _T("..")) == 0)
				continue;
 			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				AddFile(list, Size, Dir, FindFileData);
			}
		} while (FindNextFile(hFind, &FindFileData));

      FindClose(hFind);
   }


   _tcscpy_s(Search, newlen, Dir);
   _tcscat_s(Search, newlen, _T("*"));

//   _tprintf (TEXT("Target dir is %s\n"), Search);
   hFind = FindFirstFile(Search, &FindFileData);
   if (hFind != INVALID_HANDLE_VALUE) 
   {
		do
		{
			if (_tcscmp(FindFileData.cFileName, _T(".")) == 0 || _tcscmp(FindFileData.cFileName, _T("..")) == 0)
				continue;
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				int len = _tcslen(Dir) + _tcslen(FindFileData.cFileName) + 2;
				TCHAR *file = new TCHAR[len];
				_tcscpy_s(file, len, Dir);
				_tcscat_s(file, len, FindFileData.cFileName);
				_tcscat_s(file, len, _T("\\"));
				FindFiles(list, Size, file);
//				_tprintf (TEXT("dir %s\n"), file);
				delete [] file;
			}
		} while (FindNextFile(hFind, &FindFileData));
//		if (GetLastError() != ERROR_NO_MORE_FILES) errormessage();
		FindClose(hFind);
   }


	delete [] Search;
}

void CShuffleMP3sInDirectoryDlg::RandomizeFiles(flist *list)
{
	TCHAR **temp = (TCHAR **) malloc(list->num_entries * sizeof(TCHAR **));
	unsigned int loop;
	for(loop = 0; loop < list->num_entries; loop++)
	{
		unsigned int newent;
		errno_t err;
		do
		{
			err = rand_s( &newent );
			newent = (unsigned int) ((double)newent /
                          ((double) UINT_MAX + 1) * (double) list->num_entries);
			if(err != 0) ErrorMessage();
		} while(newent < 0 || newent >= list->num_entries || list->files[newent] == NULL);
		temp[loop] = list->files[newent];
		list->files[newent] = NULL;
	}
	memcpy(list->files, temp, list->num_entries * sizeof(TCHAR *));
	delete [] temp;
}

TCHAR *CShuffleMP3sInDirectoryDlg::GetFileNameFromPath(TCHAR *FullFileName)
{
	int end = _tcslen(FullFileName);
	int lastch = 0;
	while(end > 0)
	{
		if(FullFileName[end] == '\\')
		{
			if(lastch != 0)
				return &FullFileName[lastch];
			return (&FullFileName[end + 1]);
		}
		if((FullFileName[end] >= 'a' && FullFileName[end] <= 'z') ||
			(FullFileName[end] >= 'A' && FullFileName[end] <= 'Z'))
			lastch = end;
		if(FullFileName[end] == '.')
			lastch = 0;
		end -= 1;
	}
	return FullFileName;
}

void CShuffleMP3sInDirectoryDlg::OnBnClickedButtonRead()
{
	TCHAR val[256];
	unsigned int loop;
	ULONGLONG Size = 0;
	TDirectory.GetWindowTextW(val, 256);
	loop = _tcslen(val);
	if(loop < 3 || val[loop - 1] != '\\')
	{
		TStatus.SetWindowTextW(L"Invalid directory name");
		return;
	}
	for(loop = 0; loop < list.num_entries; loop++)
		delete [] list.files[loop];

	list.num_entries = 0;

	LFiles.ResetContent();
	FindFiles(&list, &Size, val);
	RandomizeFiles(&list);
	for(loop = 0; loop < list.num_entries; loop++)
		LFiles.AddString(GetFileNameFromPath(list.files[loop]));
	CBWrite.EnableWindow(TRUE);
	TDirectory.EnableWindow(FALSE);
	_snwprintf_s(val, 256, L"Found %d files with %lld bytes", list.num_entries, Size);
	TStatus.SetWindowTextW(val);
}

void CShuffleMP3sInDirectoryDlg::OnBnClickedButtonWrite()
{
	unsigned int loop;
	TCHAR val[256];
	TCHAR num[10];
	TCHAR subdir[10];
	TDirectory.GetWindowTextW(val, 256);
	loop = _tcslen(val);
	if(loop < 3 || val[loop - 1] != '\\')
		return;
	for(loop = 0; loop < list.num_entries; loop++)
	{
		int len = 50 + _tcslen(list.files[loop]) + _tcslen(val) + _tcslen(GetFileNameFromPath(list.files[loop]));
		TCHAR *dest = new TCHAR[len];

		if(loop % 100 == 0)
		{
			_tcscpy_s(dest, len, _T("cmd.exe /c mkdir \""));
			_snwprintf_s(subdir, sizeof(subdir), _T("%04d\\"), loop);
			_tcscat_s(dest, len, val);
			_tcscat_s(dest, len, subdir);
			_tcscat_s(dest, len, _T("\""));
			systemCommand(dest, TRUE, 8000);
		}
		_snwprintf_s(num, sizeof(num), _T("%04d-"), loop);

		_tcscpy_s(dest, len, _T("cmd.exe /c move \""));
		_tcscat_s(dest, len, list.files[loop]);
		_tcscat_s(dest, len, _T("\" \""));
		_tcscat_s(dest, len, val);
		_tcscat_s(dest, len, subdir);
		_tcscat_s(dest, len, num);
		_tcscat_s(dest, len, GetFileNameFromPath(list.files[loop]));
		_tcscat_s(dest, len, _T("\""));
		systemCommand(dest, TRUE, 8000);
		delete [] dest;
	}
}

void CShuffleMP3sInDirectoryDlg::OnBnClickedExit()
{
	RegKey netKey(HKEY_CURRENT_USER);
	TCHAR KeyName[256];
	_tcscpy_s(KeyName, 256, _T("Software\\Clay\\ShuffleMP3sInDirectory"));

	netKey.Open(KeyName, KEY_ALL_ACCESS);

	if(!netKey)
		netKey.Create(KeyName, KEY_ALL_ACCESS);

	if (netKey)
	{
		TCHAR val[256];
		TDirectory.GetWindowTextW(val, 256);
		netKey[_T("Directory")] = val;
	}
	OnCancel();
}
