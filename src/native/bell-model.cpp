#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bell-model.h"

/*static void mat_point_free(mat_point mp)
{
	free(mp.connected);
}

static void free_model_data(model_data md)
{
	for (mat_point** n = md.points; *n != NULL; n++)
	{
		mat_point_free(**n);
	}
}*/

static float smax = 0.f;

double do_calc(model_calc_params* params)
{
    model_data* data = params->data;
    assert(data != NULL);
    if (data->points_v.size() > 0) {
		mat_point** points = &(data->points_v[0]);
		assert(points != NULL);

		double dt = params->dt;
		assert(dt > 0);

		double sx = 0, sy = 0, sz = 0;

		for (int step = 0; step < params->steps; step++)
		{
			//printf("step: %d\n", step);fflush(stdout);
			for (mat_point** ptC = points; ptC < &(*(data->points_v.end())); ptC++)
			{
//				printf("ptC: %x\n", ptC);fflush(stdout);
				mat_point* pptC = *ptC;
				//printf("pptC: %x\n", pptC);fflush(stdout);
				pptC->ax = 0;
				pptC->ay = 0;
				pptC->az = 0;
				if (pptC->connected_v.size() > 0) {
					for (mat_point** pt = &(pptC->connected_v[0]); pt < &(*(pptC->connected_v.end())); pt++)
					{
//						printf("pt: %x\n", pt);fflush(stdout);
						mat_point* ppt = *pt;
						//printf("ppt: %x\n", ppt);fflush(stdout);
						double px = ppt->x - pptC->x,
							   py = ppt->y - pptC->y,
							   pz = ppt->z - pptC->z;
						double dx = ppt->x0 - pptC->x0,
							   dy = ppt->y0 - pptC->y0,
							   dz = ppt->z0 - pptC->z0;

						double pa = pow(px*px + py*py + pz*pz, 0.5);
						double da = pow(dx*dx + dy*dy + dz*dz, 0.5);
						pptC->ax += data->elasticity * px * (1. / da - 1. / pa);
						pptC->ay += data->elasticity * py * (1. / da - 1. / pa);
						pptC->az += data->elasticity * pz * (1. / da - 1. / pa);
					}
				}
				//printf("fr: %f\n", data->friction);fflush(stdout);
				pptC->ax -= data->friction * pptC->vx;
				pptC->ay -= data->friction * pptC->vy;
				pptC->az -= data->friction * pptC->vz;

				pptC->ax *= 1.0 / pptC->m;
				pptC->ay *= 1.0 / pptC->m;
				pptC->az *= 1.0 / pptC->m;
			}
			for (mat_point** ptC = points; ptC < &(*(data->points_v.end())); ptC++)
			{
				mat_point* pptC = *ptC;
				if (!pptC->fixed) {
					pptC->vx += pptC->ax * dt;
					pptC->vy += pptC->ay * dt;
					pptC->vz += pptC->az * dt;

					sx += pptC->vx;
					sy += pptC->vy;
					sz += pptC->vz;

					pptC->x += pptC->vx * dt;
					pptC->y += pptC->vy * dt;
					pptC->z += pptC->vz * dt;
				}
			}
		}

		// Calculating sound
		int n = 0;
		for (mat_point** ptC = points; ptC < &(*(data->points_v.end())); ptC++)
		{
			mat_point* pptC = *ptC;
			if (pptC->makes_sound) {
				n++;
			}
		}

		sx /= params->steps * n;
		sy /= params->steps * n;
		sz /= params->steps * n;

		double s = (sx + sy + sz) / 3;

		// Overloading protection
		/*if (s * params->gain > 0.99) {
			params->gain = 0.99 / s;
		}*/

		if (abs(s) > smax) {
			smax = abs(s);
			printf("-------- smax: %f\n", smax);fflush(stdout);
		}

		return s * params->gain;
    }
    return 0;
}


