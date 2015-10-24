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
		if (s * params->gain > 0.99) {
			params->gain = 0.99 / s;
		}

		return s * params->gain;
    }
    return 0;
}


/*static model_data model_data_create_line(int n, double m, double d, double elasticity, double friction)
{
	mat_point** pts = (mat_point**)malloc(sizeof(mat_point*) * (n + 1));
	pts[n] = NULL; // null-terminating

	for (int i = 0; i < n; i++)
	{
		pts[i] = (mat_point*)malloc(sizeof(mat_point));
		pts[i]->m = m;

		pts[i]->x = i * d;
		pts[i]->y = 0;
		pts[i]->z = 0;

		pts[i]->x0 = i * d;
		pts[i]->y0 = 0;
		pts[i]->z0 = 0;

		pts[i]->vx = 0;
		pts[i]->vy = 0;
		pts[i]->vz = 0;

		pts[i]->makes_sound = TRUE;
	}

	if (n > 1)
	{
		pts[0]->connected = (mat_point**)malloc(sizeof(mat_point*) * 2);
		pts[0]->connected[0] = pts[1];
		pts[0]->connected[1] = NULL;

		for (int i = 1; i < n - 1; i++)
		{
			pts[i]->connected = (mat_point**)malloc(sizeof(mat_point*) * 3);
			pts[i]->connected[0] = pts[i - 1];
			pts[i]->connected[1] = pts[i + 1];
			pts[i]->connected[2] = NULL;
		}

		pts[n - 1]->connected = (mat_point**)malloc(sizeof(mat_point*) * 2);
		pts[n - 1]->connected[0] = pts[n - 2];
		pts[n - 1]->connected[1] = NULL;
	}
	else
	{
		pts[0]->connected = (mat_point**)malloc(sizeof(mat_point*));
		pts[0]->connected[0] = NULL;
	}

	model_data res;
	res.points = pts;
	res.elasticity = elasticity;
	res.friction = friction;
	return res;
}

static model_data model_data_create_triangle(double m, double d, double elasticity, double friction)
{
	mat_point** pts = (mat_point**)malloc(sizeof(mat_point*) * 4);
	pts[3] = NULL;
	for (int i = 0; i < 3; i++)
	{
		pts[i] = (mat_point*)malloc(sizeof(mat_point));

		pts[i]->m = m;

		pts[i]->x = 0;
		pts[i]->y = 0;
		pts[i]->z = 0;

		pts[i]->x0 = 0;
		pts[i]->y0 = 0;
		pts[i]->z0 = 0;

		pts[i]->vx = 0;
		pts[i]->vy = 0;
		pts[i]->vz = 0;

		pts[i]->connected = (mat_point**)malloc(sizeof(mat_point*) * 3);
		pts[i]->connected[2] = NULL;
	}

	pts[0]->x = -d / 2;
	pts[0]->y = 0;
	pts[0]->z = 0;
	pts[0]->x0 = -d / 2;
	pts[0]->y0 = 0;
	pts[0]->z0 = 0;
	pts[0]->connected[0] = pts[1];//pts[2];
	pts[0]->connected[1] = NULL;//pts[1];

	pts[1]->x = d / 2;
	pts[1]->y = 0;
	pts[1]->z = 0;
	pts[1]->x0 = d / 2;
	pts[1]->y0 = 0;
	pts[1]->z0 = 0;
	pts[1]->connected[0] = pts[0];
	pts[1]->connected[1] = pts[2];

	pts[2]->x = d;
	pts[2]->y = pow(3., 0.5) / 2 * d;
	pts[2]->z = 0;
	pts[2]->x0 = d;
	pts[2]->y0 = pow(3., 0.5) / 2 * d;
	pts[2]->z0 = 0;
	pts[2]->connected[0] = pts[1];
	pts[2]->connected[1] = NULL;//pts[0];

	model_data res;
	res.points = pts;
	res.elasticity = elasticity;
	res.friction = friction;
	return res;
}
*/

/*******************************************************************/
/*int main(void);
int main(void)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    int i;

    
    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);
    

    // Material parameters & initial conditions

    model_data md = model_data_create_line(6, 1, 1, 3.e+7, 3);
    md.points[0]->m = 1e+14;
    md.points[0]->x0 = -0.6;
    md.points[0]->x = -1;
    md.points[0]->makes_sound = FALSE;

    md.points[1]->m = 0.995;
    md.points[1]->vx = 0;
    md.points[1]->vy = 0;
    md.points[1]->x0 = 0;
    md.points[1]->x = 0;
    md.points[1]->y = 0.1;

    md.points[2]->m = 1.01;
    md.points[2]->vx = 0;
    md.points[2]->vy = 0;
    md.points[2]->x0 = 0.6;
    md.points[2]->x = 1;

    md.points[3]->m = 1.005;
    md.points[3]->vx = 0;
    md.points[3]->vy = 0;
    md.points[3]->x0 = 1.2;
    md.points[3]->x = 2;

    md.points[4]->m = 0.997;
    md.points[4]->vx = 0;
    md.points[4]->vy = 0;
    md.points[4]->x0 = 1.8;
    md.points[4]->x = 3;

    md.points[5]->m = 1e+14;
    md.points[5]->x0 = 2.4;
    md.points[5]->x = 4.0;
    md.points[5]->makes_sound = FALSE;

    calc_params cp;
    cp.gain = 0.01;
    cp.steps = 1;
    cp.data = &md;
    

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    PaDeviceIndex dev_count = Pa_GetDeviceCount();
    for (PaDeviceIndex i = 0; i < dev_count; i++) {
    	const PaDeviceInfo* di = Pa_GetDeviceInfo(i);
    	printf("%d: %s\n", i, di->name);
    }
    fflush(stdout);

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default output device.\n");
      goto error;
    }
    outputParameters.channelCount = 2;       
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff, 
              patestCallback,
              &cp);
    if( err != paNoError ) goto error;

    //sprintf( md.message, "No Message" );
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    printf("Play for %d seconds.\n", NUM_SECONDS );
    Pa_Sleep( NUM_SECONDS * 1000 );

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}*/
