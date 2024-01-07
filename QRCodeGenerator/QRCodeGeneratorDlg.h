
// QRCodeGeneratorDlg.h : header file
//

#pragma once
#include <iostream>
#include <string>
/*=============================================
	Headers for Generating QR
===============================================*/
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "qrcodegen.hpp"

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

// CQRCodeGeneratorDlg dialog
class CQRCodeGeneratorDlg : public CDialogEx
{
// Construction
public:
	CQRCodeGeneratorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QRCODEGENERATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void doBasic(CString text, CString fileName = "QR");	//methods for generating QR
	void toSvgBMP(const QrCode& qr, const char* text, CString fileName);	//generate QR bmp buffer
	void SaveBitmapToFile(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, const unsigned long& padding_size, CString lpszFileName);	//create and save bmp image

public:
	afx_msg void OnBnClickedButtonQRGenerate();	//Event for button
	CEdit m_InputText;	//input the text
	CEdit m_FileName;	//input the filename
};
