
// EditorView.h : interface of the CEditorView class
//

#pragma once


class CEditorView : public CView
{
protected: // create from serialization only
	CEditorView() noexcept;
	DECLARE_DYNCREATE(CEditorView)

// Attributes
public:
	CEditorDoc* GetDocument() const;
	CPoint pntCur;
	CEditorDoc* pDoc;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // debug version in EditorView.cpp
inline CEditorDoc* CEditorView::GetDocument() const
   { return reinterpret_cast<CEditorDoc*>(m_pDocument); }
#endif

