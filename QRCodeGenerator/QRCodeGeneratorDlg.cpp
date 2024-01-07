
// QRCodeGeneratorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "QRCodeGeneratorApp.h"
#include "QRCodeGeneratorDlg.h"
#include "afxdialogex.h"
#include <fstream>

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


// CQRCodeGeneratorDlg dialog



CQRCodeGeneratorDlg::CQRCodeGeneratorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QRCODEGENERATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQRCodeGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_TEXT, m_InputText);
	DDX_Control(pDX, IDC_INPUT_FILE, m_FileName);
}

BEGIN_MESSAGE_MAP(CQRCodeGeneratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CQRCodeGeneratorDlg::OnBnClickedButtonQRGenerate)
END_MESSAGE_MAP()


// CQRCodeGeneratorDlg message handlers

BOOL CQRCodeGeneratorDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQRCodeGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQRCodeGeneratorDlg::OnPaint()
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
HCURSOR CQRCodeGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*==============================================
	Button Event for Generating QR
===============================================*/
void CQRCodeGeneratorDlg::OnBnClickedButtonQRGenerate()
{
	// TODO: Add your control notification handler code here
	CString input;
	m_InputText.GetWindowTextA(input);
	if (input.IsEmpty())
	{
		AfxMessageBox("Warning!!!No input text", MB_ICONWARNING); 
	}

	CString fileName;
	m_FileName.GetWindowTextA(fileName);
	if (fileName.IsEmpty())
	{
		doBasic(input);
	}
	else
	{
		doBasic(input, fileName);
	}
	AfxMessageBox("Successfully QR Code generated!!!", MB_ICONINFORMATION);
}

/*========================================
	Methods to generate the QR
==========================================*/
void CQRCodeGeneratorDlg::doBasic(CString text, CString fileName)
{
	const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level
	// Make and print the QR Code symbol
	const QrCode qr = QrCode::encodeText(text, errCorLvl);
	//std::string QR = toSvgString(qr, 4);
	CString filePath = "../Output/" + fileName + ".bmp";
	toSvgBMP(qr, text, filePath);

	//WriteInFile(qr, fileName);
}

/*===========================================
	Store QR in buffer
=============================================*/
void CQRCodeGeneratorDlg::toSvgBMP(const QrCode& qr, const char* text, CString filePath)
{
	int size = 512;
	int scale = size / qr.getSize();
	int orgQRSize = qr.getSize() * scale;
	int startIndex = (512 - orgQRSize) / 2;
	int lastIndex = startIndex + orgQRSize - 1;
	BYTE* buf = new BYTE[512 * 3 * 512];
	int c = 0;

	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			if (i >= startIndex && i <= lastIndex && j >= startIndex && j <= lastIndex && qr.getModule((i - startIndex) / scale, (j - startIndex) / scale))
			{
				buf[c + 0] = (BYTE)0;
				buf[c + 1] = (BYTE)0;
				buf[c + 2] = (BYTE)0;
			}
			else
			{
				buf[c + 0] = (BYTE)255;
				buf[c + 1] = (BYTE)255;
				buf[c + 2] = (BYTE)255;
			}
			c+=3;
		}
	}
	
	SaveBitmapToFile((BYTE*)buf,
		512,
		512,
		24,
		0,
		filePath);
	delete[] buf;
}

/*===============================================
	Create and Save bmp image
=================================================*/
void CQRCodeGeneratorDlg::SaveBitmapToFile(BYTE* pBitmapBits,
	LONG lWidth,
	LONG lHeight,
	WORD wBitsPerPixel,
	const unsigned long& padding_size,
	CString lpszFilePath)
{
	// Some basic bitmap parameters  
	unsigned long headers_size = sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER);

	unsigned long pixel_data_size = lHeight * ((lWidth * (wBitsPerPixel / 8)) + padding_size);

	BITMAPINFOHEADER bmpInfoHeader = { 0 };

	// Set the size  
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);

	// Bit count  
	bmpInfoHeader.biBitCount = wBitsPerPixel;

	// Use all colors  
	bmpInfoHeader.biClrImportant = 0;

	// Use as many colors according to bits per pixel  
	bmpInfoHeader.biClrUsed = 0;

	// Store as un Compressed  
	bmpInfoHeader.biCompression = BI_RGB;

	// Set the height in pixels  
	bmpInfoHeader.biHeight = lHeight;

	// Width of the Image in pixels  
	bmpInfoHeader.biWidth = lWidth;

	// Default number of planes  
	bmpInfoHeader.biPlanes = 1;

	// Calculate the image size in bytes  
	bmpInfoHeader.biSizeImage = pixel_data_size;

	BITMAPFILEHEADER bfh = { 0 };

	// This value should be values of BM letters i.e 0x4D42  
	// 0x4D = M 0×42 = B storing in reverse order to match with endian  
	bfh.bfType = 0x4D42;
	//bfh.bfType = 'B'+('M' << 8); 

	// <<8 used to shift ‘M’ to end  */  

	// Offset to the RGBQUAD  
	bfh.bfOffBits = headers_size;

	// Total size of image including size of headers  
	bfh.bfSize = headers_size + pixel_data_size;

	// Create the file in disk to write  
	HANDLE hFile = CreateFileA(lpszFilePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// Return if error opening file  
	if (!hFile) return;

	DWORD dwWritten = 0;

	// Write the File header  
	WriteFile(hFile,
		&bfh,
		sizeof(bfh),
		&dwWritten,
		NULL);

	// Write the bitmap info header  
	WriteFile(hFile,
		&bmpInfoHeader,
		sizeof(bmpInfoHeader),
		&dwWritten,
		NULL);

	// Write the RGB Data  
	WriteFile(hFile,
		pBitmapBits,
		bmpInfoHeader.biSizeImage,
		&dwWritten,
		NULL);

	// Close the file handle  
	CloseHandle(hFile);
}
