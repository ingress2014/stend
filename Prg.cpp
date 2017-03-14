#include "StdAfx.h"
#include "Prg.h"


CPrg::CPrg(void)
	: m_FilePrg(_T(""))
	, m_FileSound(_T(""))
	, m_TimePrg(0)
	, m_TimeSound(0)
	, m_FileTitle(_T(""))
	, m_FileSoundTitle(_T(""))
	, m_PrgInComp(FALSE)
	, m_CompControl(true)
	, m_HandBtnValue(TRUE)
{
}


CPrg::~CPrg(void)
{
}
