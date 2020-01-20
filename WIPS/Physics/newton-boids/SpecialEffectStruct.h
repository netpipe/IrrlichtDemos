// SpecialEffectStruct.h: interface for the SpecialEffectStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPECIALEFFECTSTRUCT_H__82E6D6D7_1440_4BD3_A577_DA101B7A9771__INCLUDED_)
#define AFX_SPECIALEFFECTSTRUCT_H__82E6D6D7_1440_4BD3_A577_DA101B7A9771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

 


class SpecialEffectStruct
{

public:
  int m_surface1Id;
  int m_surface2Id;
  int m_arrayIdx;
  void* controller;	
  SpecialEffectStruct();
  virtual ~SpecialEffectStruct();




};


#endif // !defined(AFX_SPECIALEFFECTSTRUCT_H__82E6D6D7_1440_4BD3_A577_DA101B7A9771__INCLUDED_)
