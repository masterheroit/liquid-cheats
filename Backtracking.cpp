#include "Backtracking.hpp"
#include "Prediction.hpp"

CData* g_Btdata;

CRaw headPositions;

void CData::run(CUserCmd* cmd)
{
	if (!cmd)
		return;

	int iBestTarget = -1;
	float bestFov = 99999;

	c_baseplayer* pLocal = GetBasePlayer(me);

	if (!pLocal || !gOpts.accuracy_backtrack)
		return;

	for (int i = 1; i <= I::Engine->GetMaxClients(); i++)
	{
		c_baseplayer* pEntity = GetBasePlayer(i);

		if (!pEntity)
			continue;
		if (pEntity->IsDormant())
			continue;
		if (pEntity->Alive() != true)
			continue;
		if (pEntity->Team() == pLocal->Team())
			continue;

		Vector hitboxpos = pEntity->GetVecHitboxPos(HITBOX_HEAD);

		headPositions.hitpos[i][cmd->command_number % 13] = CHitpos{ hitboxpos, cmd->tick_count, I::Globals->curtime};

		g_Btdata->data->hitpos[i][cmd->command_number % 13] = headPositions.hitpos[i][cmd->command_number % 13];
		Vector ViewDir = angle_vector(cmd->viewangles);
		float FOVDistance = distance_point_to_line(hitboxpos, pLocal->EyePosition(), ViewDir);

		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			iBestTarget = i;
		}

		if (iBestTarget != -1)
		{
			int bestTick = 0;
			float tempFOV = 9999;
			float bestFOV = 30;
			Vector lowestDistTicks(180, 180, 0);

			for (int t = 0; t < 12; t++)
			{
				Vector ViewDir = angle_vector(cmd->viewangles);
				float tempFOV = distance_point_to_line(headPositions.hitpos[iBestTarget][t].ptr, pLocal->EyePosition(), ViewDir);
				if (bestFOV > tempFOV)
				{
					bestTick = t, bestFOV = tempFOV;
					bttick = t;
				}
			}

			if (cmd->buttons & IN_ATTACK)
				cmd->tick_count = headPositions.hitpos[i][bestTick].tick;
		}
	}

	//memset(&headPositions, 0, sizeof(headPositions));
}