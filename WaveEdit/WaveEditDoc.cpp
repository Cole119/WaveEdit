
// WaveEditDoc.cpp : implementation of the CWaveEditDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#endif

#include "WaveEditDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWaveEditDoc

IMPLEMENT_DYNCREATE(CWaveEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaveEditDoc, CDocument)
	ON_COMMAND(ID_TOOLS_STOP, &CWaveEditDoc::OnToolsStop)
END_MESSAGE_MAP()


// CWaveEditDoc construction/destruction

CWaveEditDoc::CWaveEditDoc()
{
	// TODO: add one-time construction code here
	wave = new WaveFile();
}

void CWaveEditDoc::eraseStack(std::stack<WaveFile *> &stack){
	while(!stack.empty()){
		delete stack.top();
		stack.pop();
	}
}

CWaveEditDoc::~CWaveEditDoc()
{
	/*while(!undoStack.empty()){
		delete undoStack.top();
		undoStack.pop();
	}
	while(!redoStack.empty()){
		delete redoStack.top();
		redoStack.pop();
	}*/
	eraseStack(undoStack);
	eraseStack(redoStack);

	delete wave;
}

BOOL CWaveEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CWaveEditDoc serialization

void CWaveEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		wave->read(ar.GetFile());
        //wave.play();
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWaveEditDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWaveEditDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWaveEditDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWaveEditDoc diagnostics

#ifdef _DEBUG
void CWaveEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWaveEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CWaveEditDoc::SetView(CWaveEditView *view){
	this->view = view;
}


// CWaveEditDoc commands


void CWaveEditDoc::Play()
{
	wave->play();
}

void CWaveEditDoc::Speedup(int start, int end)
{
	//WaveFile *newWave = wave->multiply_freq(2, 0);
	WaveFile *newWave = wave->multiply_freq(2, start, end);
	//delete wave;
	undoStack.push(wave);
	eraseStack(redoStack);
	wave = newWave;
}

void CWaveEditDoc::Slowdown(int start, int end)
{
	WaveFile *newWave = wave->multiply_freq(0.5, start, end);
	//delete wave;
	undoStack.push(wave);
	eraseStack(redoStack);
	wave = newWave;
}

void CWaveEditDoc::Echo(int start, int end)
{
	WaveFile *newWave = wave->echo(0.5, 750, start, end);
	//delete wave;
	undoStack.push(wave);
	eraseStack(redoStack);
	wave = newWave;
}

void CWaveEditDoc::OnToolsStop()
{
	wave->stop();
}

void CWaveEditDoc::Undo()
{
	if(undoStack.empty()){
		return;
	}

	//delete wave;
	redoStack.push(wave);
	wave = undoStack.top();
	undoStack.pop();
}

void CWaveEditDoc::Redo()
{
	if(redoStack.empty()){
		return;
	}

	//delete wave;
	undoStack.push(wave);
	wave = redoStack.top();
	redoStack.pop();
}

void CWaveEditDoc::Copy(int start, int end)
{
	// Clear the clipboard
	delete theApp.clipboard;
    // Copy first the fragment
    theApp.clipboard = wave->get_fragment(start, end);
}

void CWaveEditDoc::Cut(int start, int end)
{
	Copy(start, end);

    // Copy the clipboard
    WaveFile * w2 = wave->remove_fragment(start, end);

    // Remove old wave
    //delete wave;
	undoStack.push(wave);
	eraseStack(redoStack);

    // Substitute old wave with new one
    wave = w2;
}

void CWaveEditDoc::Paste(int index)
{
	WaveFile * wave2 = wave->insert_fragment(theApp.clipboard, index);
	//delete wave;
	undoStack.push(wave);
	eraseStack(redoStack);
	wave = wave2;
}