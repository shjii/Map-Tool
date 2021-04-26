#pragma once
#include "SShape.h"
class SObject2D : public SShape
{
public:

public:
	bool	Create(ID3D11Device* pDevice, T_STR szVS, T_STR szPS, T_STR	szTex) override;
	bool	CreateInputLayout()override;
};

