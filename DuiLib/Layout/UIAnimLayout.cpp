#include "stdafx.h"
#include "UIAnimLayout.h"
#include "../3rd/UiFeatureEffect/Interface/IUIEffect.h"
#include "memory"

namespace DuiLib
{

const LPCTSTR CAnimLayoutUI::kAnimLayoutClass = L"AnimLayoutUI";
const LPCTSTR CAnimLayoutUI::kAnimLayoutInterface = L"CAnimLayout";

CAnimLayoutUI::CAnimLayoutUI()
	: m_pEffect( NULL )
	, m_bPlaying( false )
	, m_dwEffectNum(2)
{
	m_pEffectManager = IUiEffectManagerImpl::GetInstance();
	m_pEffect = m_pEffectManager->GetAnimation();
	ASSERT(m_pEffect);
}


CAnimLayoutUI::~CAnimLayoutUI()
{
	m_pEffect->ClearAllAnimation();
	m_pEffectManager->ReleaseAnimation(m_pEffect);
}


LPCTSTR CAnimLayoutUI::GetClass() const
{
	return kAnimLayoutClass;
}

LPVOID CAnimLayoutUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, kAnimLayoutInterface) == 0) return static_cast<CAnimLayoutUI*>(this);
	return CVerticalLayoutUI::GetInterface(pstrName);
}

void CAnimLayoutUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if (!m_bPlaying)
	{
		__super::DoPaint(hDC, rcPaint);
		return;
	}

	typedef BOOL(WINAPI *LPALPHABLEND)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
	static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	lpAlphaBlend(hDC, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, m_hMemDc,
		0, 0, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, bf);

}

bool CAnimLayoutUI::StartEffect()
{
	if (m_bPlaying)
		return false;
	if (m_dwEffectNum > 80)
	{
		m_dwEffectNum = 2;
		return false;
	}
	
	m_hMemDc = ::CreateCompatibleDC(m_pManager->GetPaintDC());
	m_hTransBitmap = CRenderEngine::GenerateBitmap(m_pManager, this, m_rcItem); 
	if (m_hTransBitmap == NULL)
		return false;
	m_hOldBitmap = (HBITMAP) ::SelectObject(m_hMemDc, m_hTransBitmap);

	BITMAP bmDst;
	GetObject(m_hTransBitmap, sizeof(bmDst), &bmDst);
	SIZE szMemDc = { bmDst.bmWidth, bmDst.bmHeight };

	//�޲�һ��Alphaͨ��,һЩ�ؼ�(Richedit)����AlphaΪ0
	RECT rcRestore = m_rcItem;
	CRenderEngine::RestoreAlphaColor((LPBYTE)bmDst.bmBits, bmDst.bmWidth, &rcRestore);

	// ��䶯������
	AnimationParam animParam;
	animParam.effectKey = (WPARAM)this;				//�ؼ�ָ��
	animParam.animationEffect = 41;	//�������ͣ���2-80��1Ϊ�Զ��嶯������û����ֲ����
	animParam.animationFrequency = 10;				//�������
	animParam.bShow = TRUE;							//����˳��
	animParam.hBitmap = m_hTransBitmap;
	animParam.pBmpData = (BYTE*)bmDst.bmBits;
	animParam.bmpSize = szMemDc;
	animParam.hdc = m_hMemDc;

	BOOL bRet = m_pEffect->AppendAnimation(animParam);
	ASSERT(bRet);

	m_bPlaying = true;

	// ������ͬ��ִ�еģ�Animation�����ڶ�����Ϻ󷵻�
	bRet = m_pEffect->Animation(dynamic_cast<IUIEffectCallBack*>(this), 0);
	ASSERT(bRet);

	// �ݹ���ʾ���ж���Ч��,��ֻ��Ϊ����ʾЧ��,ʵ�ʿ�����Ҫ������!
	//StartEffect();

	return true;
}


void CAnimLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	__super::SetAttribute(pstrName, pstrValue);
}

void CAnimLayoutUI::OnUiEffectBegin(WPARAM effectKey, DWORD animaType)
{
	CControlUI* pCtrl = (CControlUI*)effectKey;
	if (pCtrl == NULL)
		return;
}

void CAnimLayoutUI::OnUiEffectEnd(WPARAM effectKey, DWORD animaType)
{
	CControlUI* pCtrl = (CControlUI*)effectKey;
	if (pCtrl == NULL)
		return;

	m_bPlaying = false;
	SelectObject(m_hMemDc, m_hOldBitmap);
	DeleteObject(m_hTransBitmap);
	DeleteObject(m_hMemDc);

	Invalidate();

}

void CAnimLayoutUI::OnUiEffectDraw()
{
	// �������ǿ���ػ洰�壬�����޷���ʾ���������ܹؼ�
	RedrawWindow(m_pManager->GetPaintWindow(), NULL, NULL,  RDW_INVALIDATE | RDW_UPDATENOW);
}

}