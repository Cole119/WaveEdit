
// WaveEditView.h : interface of the CWaveEditView class
//

#pragma once


class CWaveEditView : public CScrollView
{
private:
	double zoom;
	bool mousePressed;
	int startSelection;
	int endSelection;
	double drawScale;

protected: // create from serialization only
	CWaveEditView();
	DECLARE_DYNCREATE(CWaveEditView)

// Attributes
public:
	CWaveEditDoc* GetDocument() const;
	void GetSelection(int& begin, int& end) const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWaveEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnZoominX1();
	afx_msg void OnZoominX2();
	afx_msg void OnZoominX5();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnToolsPlay();
	afx_msg void OnToolsSpeedup();
	afx_msg void OnToolsSlowdown();
	afx_msg void OnToolsEcho();
	afx_msg void OnZoomoutX1();
	afx_msg void OnZoomoutX2();
	afx_msg void OnZoomoutX5();
};

#ifndef _DEBUG  // debug version in WaveEditView.cpp
inline CWaveEditDoc* CWaveEditView::GetDocument() const
   { return reinterpret_cast<CWaveEditDoc*>(m_pDocument); }
#endif

