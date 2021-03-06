
// WaveEditDoc.h : interface of the CWaveEditDoc class
//


#pragma once

#include "WaveFile.h"
#include <stack>

class CWaveEditDoc : public CDocument
{
	friend class CWaveEditView;
    WaveFile *wave;
	CWaveEditView *view;
	std::stack<WaveFile *> undoStack;
	std::stack<WaveFile *> redoStack;

private:
	void eraseStack(std::stack<WaveFile *> &stack);
protected: // create from serialization only
	CWaveEditDoc();
	DECLARE_DYNCREATE(CWaveEditDoc)

// Attributes
public:
	void SetView(CWaveEditView *view);
	void Play();
	void Speedup(int start, int end);
	void Slowdown(int start, int end);
	void Echo(int start, int end);
	void Undo();
	void Redo();
	void Copy(int start, int end);
	void Cut(int start, int end);
	void Paste(int index);
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CWaveEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnToolsStop();
};
