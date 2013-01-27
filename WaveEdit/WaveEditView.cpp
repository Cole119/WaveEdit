
// WaveEditView.cpp : implementation of the CWaveEditView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#endif

#include "WaveEditDoc.h"
#include "WaveEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveEditView

IMPLEMENT_DYNCREATE(CWaveEditView, CView)

BEGIN_MESSAGE_MAP(CWaveEditView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_CUT, &CWaveEditView::OnEditCut)
END_MESSAGE_MAP()

// CWaveEditView construction/destruction

CWaveEditView::CWaveEditView()
{
	startSelection = 0;
	endSelection = 0;
	mousePressed = false;
}

CWaveEditView::~CWaveEditView()
{
}

BOOL CWaveEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

// CWaveEditView drawing

void CWaveEditView::OnDraw(CDC* pDC)
{
	CWaveEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	WaveFile * wave = &pDoc->wave;

    if (wave->hdr==NULL) {
        return;
    }

    // Get dimensions of the window.
    CRect rect;
    GetClientRect(rect);

    // Set color in pen and brush for wave
    COLORREF color = RGB( 50, 225, 0 );
    CPen pen2( PS_SOLID, 0, color );
    pDC->SelectObject( &pen2 );
    CBrush brush2(color);
    pDC->SelectObject( &brush2 );

	//Fill the selection rectangle
	if (startSelection != endSelection) {
		pDC->MoveTo(0, 0);
		pDC->FillSolidRect(startSelection, 0, endSelection-startSelection, rect.Height(), RGB(0, 100, 255));
	}

    // Draw the wave
    pDC->MoveTo(0,0);
    /*int x;
    for (x=0; x < rect.Width(); x++) {
        // Assuming the whole file will be fit in the window, for every x value in the window
        // we need to find the equivalent sample in the wave file.
        float val = wave->get_sample((int) (x*wave->lastSample/rect.Width()) );

        // We need to fit the sound also in the y axis. The y axis goes from 0 in the
        // top of the window to rect.Height at the bottom. The sound goes from -32768 to 32767
        // we scale it in that way.
        int y = (int) ((val+32768) * (rect.Height()-1) / (32767+32768));
        pDC->LineTo(x,rect.Height() - y);
    }*/
	unsigned int x;
	for (x=0; x < wave->lastSample/200; x++) {
        // Assuming the whole file will be fit in the window, for every x value in the window
        // we need to find the equivalent sample in the wave file.
        float val = wave->get_sample((int) (x*200) );

        // We need to fit the sound also in the y axis. The y axis goes from 0 in the
        // top of the window to rect.Height at the bottom. The sound goes from -32768 to 32767
        // we scale it in that way.
        int y = (int) ((val+32768) * (rect.Height()-1) / (32767+32768));
        pDC->LineTo(x,rect.Height() - y);
    }
}


// CWaveEditView printing

BOOL CWaveEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWaveEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWaveEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CWaveEditView diagnostics

#ifdef _DEBUG
void CWaveEditView::AssertValid() const
{
	CView::AssertValid();
}

void CWaveEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWaveEditDoc* CWaveEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaveEditDoc)));
	return (CWaveEditDoc*)m_pDocument;
}
#endif //_DEBUG

void
CWaveEditView::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();

    // Initial scroll sizes
    CSize sizeTotal;
    sizeTotal.cx = 10000;
    sizeTotal.cy = 10000;
    SetScrollSizes(MM_TEXT, sizeTotal);
}

// CWaveEditView message handlers


void CWaveEditView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mousePressed = true;

    startSelection = point.x;
    endSelection = point.x;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CWaveEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	mousePressed = false;
    endSelection = point.x;
    RedrawWindow();

	CScrollView::OnLButtonUp(nFlags, point);
}


void CWaveEditView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mousePressed) {
        endSelection = point.x;
        RedrawWindow();
    }

	CScrollView::OnMouseMove(nFlags, point);
}


void CWaveEditView::OnEditCut()
{
	/*CWaveEditDoc* pDoc = GetDocument();
    
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    WaveFile * wave = &pDoc->wave;

    if (wave->hdr==NULL) {
        return;
    }

    // Get dimensions of the window.
    CRect rect;
    GetClientRect(rect);

    // Scale the start and end of the selection.
    double startms = (1000.0 * wave->lastSample /wave->sampleRate) * this->startSelection/rect.Width();

    // Scale the start and end of the selection.
    double endms = (1000.0 * wave->lastSample /wave->sampleRate) * this->endSelection/rect.Width();
    
    // Ciopy first the fragment
    clipboard = wave->get_fragment(startms, endms);

    // Copy the clipboard
    WaveFile * w2 = wave->remove_fragment(startms, endms);

    // Remove old wave
    delete wave;

    // Substitute old wave with new one
    pDoc->wave = w2;

    // Update window
    this->RedrawWindow();*/
}
