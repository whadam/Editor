
// EditorView.cpp : implementation of the CEditorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Editor.h"
#endif

#include "EditorDoc.h"
#include "EditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CView)

BEGIN_MESSAGE_MAP(CEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CEditorView construction/destruction

CEditorView::CEditorView() noexcept
{
	// TODO: add construction code here
	pntCur.x = 0;
	pntCur.y = 0;
}

CEditorView::~CEditorView()
{
}

BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEditorView drawing

void CEditorView::OnDraw(CDC* /*pDC*/)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CEditorView printing

BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CEditorView diagnostics

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEditorDoc* CEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditorDoc)));
	return (CEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CEditorView message handlers

int CEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	pDoc = GetDocument();

	TEXTMETRIC txtKey;

	CClientDC dc(this);	// get dc

	dc.GetTextMetrics(&txtKey);	// dc(View) fonts info

	// create caret (character avarage width / 2, height)
	CreateSolidCaret(txtKey.tmAveCharWidth / 2, txtKey.tmHeight);

	CPoint pntTmp(0, 0);

	// set caret first position (0,0)
	SetCaretPos(pntTmp);
	
	ShowCaret();

	return 0;
}

void CEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	TEXTMETRIC txtKey;
	dc.GetTextMetrics(&txtKey);

	// hide caret when input character
	HideCaret();

	if (nChar == VK_BACK) { // if backspace
		if (!pDoc->strDB[pDoc->pline].IsEmpty()) {	// if is not empty string
			// cut original string's end
			pDoc->strDB[pDoc->pline] = pDoc->strDB[pDoc->pline].Left(pDoc->strDB[pDoc->pline].GetLength() - 1);
			// print again after cutting
			dc.TextOutA(0, pntCur.y, pDoc->strDB[pDoc->pline] + " ");
		}
	}
	else if (nChar == VK_RETURN) {	// if enter
		// +1 line
		pDoc->pline++;
	}
	else if (nChar == VK_UP) {	// if up
		// -1 line
		pDoc->pline--;
	}
	else {	// if normal character
		// add input character to strDB
		pDoc->strDB[pDoc->pline] += (TCHAR)nChar;
		// output to doc
		dc.TextOutA(0, (int)pDoc->pline * txtKey.tmHeight, pDoc->strDB[pDoc->pline]);
	}

	// calc where current string ends
	CSize strSize;
	strSize = dc.GetTextExtent(pDoc->strDB[pDoc->pline]);

	// calc caret position
	pntCur.x = strSize.cx;
	pntCur.y = int(pDoc->pline) * txtKey.tmHeight;

	// set caret position
	SetCaretPos(pntCur);

	ShowCaret();

	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (pDoc->pline == 0)	// if top line
		return;
	if (nChar == VK_UP)	// if up
		OnChar(nChar, nRepCnt, nFlags);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages
	TEXTMETRIC txtKey;

	dc.GetTextMetrics(&txtKey);	// get font info from current dc

	for (int i = 0; i <= pDoc->pline; i++)
		dc.TextOutA(0, (int)i * txtKey.tmHeight, pDoc->strDB[i]);
}
