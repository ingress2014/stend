#pragma once

class CColorButton : public CButton
{
 DECLARE_DYNAMIC(CColorButton)

 public:
  CColorButton();
  virtual ~CColorButton();

 // Operations
 public:
  void SetColour(COLORREF clrTextColour, COLORREF clrBackgroundColour);

  void SetColour(COLORREF clrTextColour, COLORREF clrBackgroundColour,
                 COLORREF clrDisabledBackgroundColour);

  void ResetColour(bool bInvalid = true);

 // Overrides
 public:
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;

 protected:
  void DrawFrame(CDC* pobDc, CRect objRect, int nState);
  void DrawFilledRect(CDC* pobDc, CRect objRect, COLORREF clrColour);
  void DrawLine(CDC* pobDc, long lnSx, long lnSy, long lnEx, long lnEy, COLORREF clrColour);
  void DrawButtonText(CDC* pobDc, CRect objRect, CString& strBtnText, COLORREF clrTextColour);

 protected:
  enum
  {
   BUTTON_IN           = 0x01,
   BUTTON_OUT          = 0x02,
   BUTTON_BLACK_BORDER = 0x04,
  };

 private:
  COLORREF m_clrTextColour;
  COLORREF m_clrBackgroundColour;
  COLORREF m_clrDisabledBackgroundColour;
  COLORREF m_clrLight;
  COLORREF m_clrHighLight;
  COLORREF m_clrShadow;
  COLORREF m_clrDarkShadow;

  CString m_strBtnText;

 protected:
  DECLARE_MESSAGE_MAP()
};
