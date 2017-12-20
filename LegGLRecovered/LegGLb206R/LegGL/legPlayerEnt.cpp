#include "legPlayerEnt.h"
#include "LegFrameworkClass.h"
extern LegFrameworkClass p_lfc;

void legPlayer::UpdatePlayerPosition(void)
{
	x0 = p_lfc.gCamPosition.x;
	y0 = p_lfc.gCamPosition.y;
	z0 = p_lfc.gCamPosition.z;
}