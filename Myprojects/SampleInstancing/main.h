#pragma once
#include "TCore.h"
#include "SShape.h"
#include "TObject.h"
#include "SModelViewCamera.h"

class main : public TCore
{
public:

	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

TGAME_RUN;