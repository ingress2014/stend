#include "stdafx.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CColorButton, CButton)

BEGIN_MESSAGE_MAP(CColorButton, CButton)
END_MESSAGE_MAP()

CColorButton::CColorButton(void)
{
 ResetColour(false);
}

CColorButton::~CColorButton(void)
{
}

void CColorButton::SetColour(COLORREF clrTextColourNew, COLORREF clrBackgroundColourNew)
{
 m_clrTextColour               = clrTextColourNew;
 m_clrBackgroundColour         = clrBackgroundColourNew;
 m_clrDisabledBackgroundColour = ::GetSysColor(COLOR_BTNFACE);

 Invalidate(FALSE);
}

void CColorButton::SetColour(COLORREF m_clrTextColourNew, COLORREF clrBackgroundColourNew,
                        COLORREF clrDisabledBackgroundColourNew)
{
 m_clrTextColour               = m_clrTextColourNew;
 m_clrBackgroundColour         = clrBackgroundColourNew;
 m_clrDisabledBackgroundColour = clrDisabledBackgroundColourNew;

 Invalidate(FALSE);
}

void CColorButton::ResetColour(bool bInvalid /*true*/)
{
 m_clrTextColour               = ::GetSysColor(COLOR_BTNTEXT);
 m_clrBackgroundColour         = ::GetSysColor(COLOR_BTNFACE);
 m_clrDisabledBackgroundColour = m_clrBackgroundColour;
 m_clrLight                    = ::GetSysColor(COLOR_3DLIGHT);
 m_clrHighLight                = ::GetSysColor(COLOR_BTNHIGHLIGHT);
 m_clrShadow                   = ::GetSysColor(COLOR_BTNSHADOW);
 m_clrDarkShadow               = ::GetSysColor(COLOR_3DDKSHADOW);

 if(bInvalid)
 {
  Invalidate(FALSE);
 }
}

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
 CDC*  pobDc;
 CRect rctFocus;
 CRect rctButton;
 CRect rctText;
 CRect rctOffsetText;
 UINT  unState;

 pobDc   = CDC::FromHandle(lpDrawItemStruct->hDC);
 unState = lpDrawItemStruct->itemState;

 rctFocus.CopyRect(&lpDrawItemStruct->rcItem);
 rctButton.CopyRect(&lpDrawItemStruct->rcItem);

 rctText = rctButton;
 rctText.OffsetRect(-1, -1);
 rctOffsetText = rctText;
 rctOffsetText.OffsetRect(1, 1);

 // Set the focus rectangle to just past the border decoration
 rctFocus.left   += 4;
 rctFocus.right  -= 4;
 rctFocus.top    += 4;
 rctFocus.bottom -= 4;

 // Retrieve the button's caption
 GetWindowText(m_strBtnText);

 if(unState & ODS_DISABLED)
 {
  DrawFilledRect(pobDc, rctButton, m_clrDisabledBackgroundColour);
 }
 else
 {
  DrawFilledRect(pobDc, rctButton, m_clrBackgroundColour);
 }
 if(unState & ODS_SELECTED)
 {
  DrawFrame(pobDc, rctButton, BUTTON_IN);
 }
 else
 {
  if((unState & ODS_DEFAULT) || (unState & ODS_FOCUS))
  {
   DrawFrame(pobDc, rctButton, BUTTON_OUT | BUTTON_BLACK_BORDER);
  }
  else
  {
   DrawFrame(pobDc, rctButton, BUTTON_OUT);
  }
 }

 if(unState & ODS_DISABLED)
 {
  DrawButtonText(pobDc, rctOffsetText, m_strBtnText, RGB(255,255,255));
  DrawButtonText(pobDc, rctText, m_strBtnText, RGB(128,128,128));
 }
 else
 {
  if(unState & ODS_SELECTED)
  {
   DrawButtonText(pobDc, rctOffsetText, m_strBtnText, m_clrTextColour);
  }
  else
  {
   DrawButtonText(pobDc, rctText, m_strBtnText, m_clrTextColour);
  }
 }
 if(unState & ODS_FOCUS)
 {
  DrawFocusRect(lpDrawItemStruct->hDC, (LPRECT)&rctFocus);
 }
}

void CColorButton::DrawFrame(CDC* pobDc, CRect objRect, int nState)
{
 COLORREF clrColour;
 if(nState & BUTTON_BLACK_BORDER)
 {
  clrColour = RGB(0,0,0);

  // Across top
  DrawLine(pobDc, objRect.left, objRect.top, objRect.right, objRect.top, clrColour);
  // Down left
  DrawLine(pobDc, objRect.left, objRect.top, objRect.left, objRect.bottom, clrColour);

  // Across bottom
  DrawLine(pobDc, objRect.left, objRect.bottom - 1, objRect.right, objRect.bottom - 1, clrColour);
  // Down right
  DrawLine(pobDc, objRect.right - 1, objRect.top, objRect.right - 1, objRect.bottom, clrColour);

  objRect.InflateRect(-1, -1);
 }
 if(nState & BUTTON_OUT)
 {
  clrColour = m_clrHighLight;

  // Across top
  DrawLine(pobDc, objRect.left, objRect.top, objRect.right, objRect.top, clrColour);
  // Down left
  DrawLine(pobDc, objRect.left, objRect.top, objRect.left,  objRect.bottom, clrColour);

  clrColour = m_clrDarkShadow;

  // Across bottom
  DrawLine(pobDc, objRect.left, objRect.bottom - 1, objRect.right, objRect.bottom - 1, clrColour);
  // Down right
  DrawLine(pobDc, objRect.right - 1, objRect.top, objRect.right - 1, objRect.bottom, clrColour);

  objRect.InflateRect(-1, -1);

  clrColour = m_clrLight;

  // Across top
  DrawLine(pobDc, objRect.left, objRect.top, objRect.right, objRect.top, clrColour);
  // Down left
  DrawLine(pobDc, objRect.left, objRect.top, objRect.left, objRect.bottom, clrColour);

  clrColour = m_clrShadow;

  // Across bottom
  DrawLine(pobDc, objRect.left, objRect.bottom - 1, objRect.right, objRect.bottom - 1, clrColour);
  // Down right
  DrawLine(pobDc, objRect.right - 1, objRect.top, objRect.right - 1, objRect.bottom, clrColour);
 }

 if(nState & BUTTON_IN)
 {
  clrColour = m_clrDarkShadow;

  // Across top
  DrawLine(pobDc, objRect.left, objRect.top, objRect.right, objRect.top, clrColour);
  // Down left
  DrawLine(pobDc, objRect.left, objRect.top, objRect.left, objRect.bottom, clrColour);
  // Across bottom
  DrawLine(pobDc, objRect.left, objRect.bottom - 1, objRect.right, objRect.bottom - 1, clrColour);
  // Down right
  DrawLine(pobDc, objRect.right - 1, objRect.top, objRect.right - 1, objRect.bottom, clrColour);
 
  objRect.InflateRect(-1, -1);

  clrColour = m_clrShadow;

  // Across top
  DrawLine(pobDc, objRect.left, objRect.top, objRect.right, objRect.top, clrColour);
  // Down left
  DrawLine(pobDc, objRect.left, objRect.top, objRect.left,  objRect.bottom, clrColour);
  // Across bottom
  DrawLine(pobDc, objRect.left, objRect.bottom - 1, objRect.right, objRect.bottom - 1, clrColour);
  // Down right
  DrawLine(pobDc, objRect.right - 1, objRect.top, objRect.right - 1, objRect.bottom, clrColour);
 }
}

void CColorButton::DrawFilledRect(CDC* pobDc, CRect objRect, COLORREF clrColour)
{
 CBrush objBrush;

 objBrush.CreateSolidBrush(clrColour);
 pobDc->FillRect(objRect, &objBrush);
}

void CColorButton::DrawLine(CDC* pobDc, long lnSx, long lnSy, long lnEx, long lnEy,
                       COLORREF clrColour)
{
 CPen objNewPen;
 CPen* pOldPen;

 objNewPen.CreatePen(PS_SOLID, 1, clrColour);
 pOldPen = pobDc->SelectObject(&objNewPen);
 pobDc->MoveTo(lnSx, lnSy);
 pobDc->LineTo(lnEx, lnEy);
 pobDc->SelectObject(pOldPen);
 objNewPen.DeleteObject();
}


void CColorButton::DrawButtonText(CDC* pobDc, CRect objRect, CString& strBtnText,
                             COLORREF clrTextColour)
{
 COLORREF clrPreviousColour;

 clrPreviousColour = pobDc->SetTextColor(clrTextColour);
 pobDc->SetBkMode(TRANSPARENT);
 pobDc->DrawText(strBtnText, strBtnText.GetLength(),
                 objRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
 pobDc->SetTextColor(clrPreviousColour);
}

