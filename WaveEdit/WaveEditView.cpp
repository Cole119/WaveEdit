
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
	ON_COMMAND(ID_ZOOMIN_X1, &CWaveEditView::OnZoominX1)
	ON_COMMAND(ID_ZOOMIN_X2, &CWaveEditView::OnZoominX2)
	ON_COMMAND(ID_ZOOMIN_X5, &CWaveEditView::OnZoominX5)
	ON_COMMAND(ID_EDIT_COPY, &CWaveEditView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CWaveEditView::OnEditPaste)
	ON_COMMAND(ID_TOOLS_PLAY, &CWaveEditView::OnToolsPlay)
	ON_COMMAND(ID_TOOLS_SPEEDUP, &CWaveEditView::OnToolsSpeedup)
	ON_COMMAND(ID_TOOLS_SLOWDOWN, &CWaveEditView::OnToolsSlowdown)
	ON_COMMAND(ID_TOOLS_ECHO, &CWaveEditView::OnToolsEcho)
	ON_COMMAND(ID_ZOOMOUT_X1, &CWaveEditView::OnZoomoutX1)
	ON_COMMAND(ID_ZOOMOUT_X2, &CWaveEditView::OnZoomoutX2)
	ON_COMMAND(ID_ZOOMOUT_X5, &CWaveEditView::OnZoomoutX5)
	ON_COMMAND(ID_EDIT_UNDO, &CWaveEditView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO32785, &CWaveEditView::OnEditRedo)
END_MESSAGE_MAP()

// CWaveEditView construction/destruction

CWaveEditView::CWaveEditView()
{
	zoom = 1;
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

	WaveFile * wave = pDoc->wave;

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

	//Draw the cursor if there is no selection
	if(startSelection == endSelection){
		pDC->MoveTo(0, 0);
		pDC->FillSolidRect(startSelection, 0, 1, rect.Height(), RGB(0, 0, 0));
	}
	//Fill the selection rectangle
	else{
		pDC->MoveTo(0, 0);
		pDC->FillSolidRect(startSelection, 0, endSelection-startSelection, rect.Height(), RGB(0, 100, 255));
	}

    // Draw the wave
    pDC->MoveTo(0,0);
    /*int x;
    for (x=0; x < zoom*rect.Width(); x++) {
        // Assuming the whole file will be fit in the window, for every x value in the window
        // we need to find the equivalent sample in the wave file.
        float val = wave->get_sample((int) (zoom*x*wave->lastSample/rect.Width()) );

        // We need to fit the sound also in the y axis. The y axis goes from 0 in the
        // top of the window to rect.Height at the bottom. The sound goes from -32768 to 32767
        // we scale it in that way.
        int y = (int) ((val+32768) * (rect.Height()-1) / (32767+32768));
        pDC->LineTo(x,rect.Height() - y);
    }*/
	unsigned int x;
	for (x=0; x < zoom*wave->lastSample/drawScale; x++) {
        // Assuming the whole file will be fit in the window, for every x value in the window
        // we need to find the equivalent sample in the wave file.
        float val = wave->get_sample((int) (x*drawScale/zoom) );

        // We need to fit the sound also in the y axis. The y axis goes from 0 in the
        // top of the window to rect.Height at the bottom. The sound goes from -32768 to 32767
        // we scale it in that way.
        int y = (int) ((val+32768) * (rect.Height()-1) / (32767+32768));
        pDC->LineTo(x,rect.Height() - y);
    }

	CSize sizeTotal;
	sizeTotal.cx = x;
    SetScrollSizes(MM_TEXT, sizeTotal);
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
	CRect rect;
    CSize sizeTotal;
	CWaveEditDoc *doc = GetDocument();
	WaveFile * wave = doc->wave;
	doc->SetView(this);

	GetClientRect(rect);
	drawScale = wave->lastSample/rect.Width();
    /*sizeTotal.cx = 10000;
    sizeTotal.cy = 10000;*/
	//sizeTotal.cx = rect.Width();
	//sizeTotal.cx = 2000;
	//sizeTotal.cy = rect.Height();
    SetScrollSizes(MM_TEXT, sizeTotal);
}

void CWaveEditView::GetSelection(int& begin, int& end) const{
	CWaveEditDoc* pDoc = GetDocument();
    
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    WaveFile * wave = pDoc->wave;

    if (wave->hdr==NULL) {
        return;
    }
	CSize size = GetTotalSize();

	begin = wave->lastSample * (double)this->startSelection/(double)size.cx;
	end = wave->lastSample * (double)this->endSelection/(double)size.cx;
}

// CWaveEditView message handlers


void CWaveEditView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mousePressed = true;

	CPoint scrollbarPos = GetDeviceScrollPosition();
    startSelection = point.x + scrollbarPos.x;
    endSelection = point.x + scrollbarPos.x;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CWaveEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	mousePressed = false;

	CPoint scrollbarPos = GetDeviceScrollPosition();
    endSelection = point.x + scrollbarPos.x;
	if(endSelection < startSelection){
		int temp = endSelection;
		endSelection = startSelection;
		startSelection = temp;
	}
    RedrawWindow();

	CScrollView::OnLButtonUp(nFlags, point);
}


void CWaveEditView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mousePressed) {
		CPoint scrollbarPos = GetDeviceScrollPosition();
        endSelection = point.x + scrollbarPos.x;
        RedrawWindow();
    }

	CScrollView::OnMouseMove(nFlags, point);
}


void CWaveEditView::OnEditCut()
{
	CWaveEditDoc* pDoc = GetDocument();
    
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    WaveFile * wave = pDoc->wave;

    if (wave->hdr==NULL) {
        return;
    }

    // Get dimensions of the window.
    CRect rect;
    GetClientRect(rect);

	CSize size = GetTotalSize();

    // Scale the start and end of the selection.
	double startms = wave->lastSample * (double)this->startSelection/(double)size.cx;
	double endms = wave->lastSample * (double)this->endSelection/(double)size.cx;
    
	pDoc->Cut(startms, endms);

	endSelection = startSelection;
    // Update window
    this->RedrawWindow();
}

void CWaveEditView::OnEditCopy()
{
	CWaveEditDoc* pDoc = GetDocument();
    
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    WaveFile * wave = pDoc->wave;

    if (wave->hdr==NULL) {
        return;
    }

    // Get dimensions of the window.
    CRect rect;
    GetClientRect(rect);

	CSize size = GetTotalSize();
	double startms = wave->lastSample * (double)this->startSelection/(double)size.cx;
	double endms = wave->lastSample * (double)this->endSelection/(double)size.cx;
    
	pDoc->Copy(startms, endms);
}

void CWaveEditView::OnEditPaste()
{
	CWaveEditDoc* pDoc = GetDocument();
    
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    WaveFile * wave = pDoc->wave;

    if (wave->hdr==NULL) {
        return;
    }

    // Get dimensions of the window.
    CRect rect;
    GetClientRect(rect);
	CSize size = GetTotalSize();

    // Scale the start and end of the selection.
    //double startms = (1000.0 * wave->lastSample /wave->sampleRate) * this->startSelection/rect.Width();
	double startms = wave->lastSample * (double)this->startSelection/(double)size.cx;
	pDoc->Paste(startms);

	RedrawWindow();
}


void CWaveEditView::OnToolsPlay()
{
	CWaveEditDoc *doc = GetDocument();
	doc->Play();
}


void CWaveEditView::OnToolsSpeedup()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Speedup(start, end);
	RedrawWindow();
}


void CWaveEditView::OnToolsSlowdown()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Slowdown(start, end);
	RedrawWindow();
}


void CWaveEditView::OnToolsEcho()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Echo(start, end);
	RedrawWindow();
}


void CWaveEditView::OnZoominX1()
{
	zoom = zoom*1.5;
	RedrawWindow();
}


void CWaveEditView::OnZoominX2()
{
	zoom = zoom*2;
	RedrawWindow();
}


void CWaveEditView::OnZoominX5()
{
	zoom = zoom*5;
	RedrawWindow();
}


void CWaveEditView::OnZoomoutX1()
{
	zoom = zoom/1.5;
	RedrawWindow();
}


void CWaveEditView::OnZoomoutX2()
{
	zoom = zoom/2.0;
	RedrawWindow();
}


void CWaveEditView::OnZoomoutX5()
{
	zoom = zoom/5.0;
	RedrawWindow();
}



void CWaveEditView::OnEditUndo()
{
	CWaveEditDoc *doc = GetDocument();
	doc->Undo();
	RedrawWindow();
}


void CWaveEditView::OnEditRedo()
{
	CWaveEditDoc *doc = GetDocument();
	doc->Redo();
	RedrawWindow();
}
