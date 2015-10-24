#include <jni.h>

#include <algorithm>

#include "bell-model.h"

using namespace std;

extern "C" {

JNIEXPORT jlong JNICALL Java_bellmodel_ModelCalc_alloc(JNIEnv *, jclass) {
	return (jlong)(new model_calc_params());
}
JNIEXPORT void JNICALL Java_bellmodel_ModelCalc_free(JNIEnv *, jclass, jlong nativePtr) {
	delete (model_calc_params*)nativePtr;
}

JNIEXPORT void JNICALL Java_bellmodel_ModelCalc_updateModelData(JNIEnv *, jclass, jlong nativePtr, jlong modelDataNativePtr) {
	model_calc_params& cp = *((model_calc_params*)nativePtr);
	model_data& d = *((model_data*)modelDataNativePtr);

	cp.data = &d;
}
JNIEXPORT jdouble JNICALL Java_bellmodel_ModelCalc_doStep(JNIEnv *, jclass, jlong nativePtr) {
	model_calc_params& cp = *((model_calc_params*)nativePtr);

	return do_calc(&cp);
}

#define MODELCALC_UPDATE_FIELD(FNAME, type, fname)	\
		JNIEXPORT void JNICALL Java_bellmodel_ModelCalc_update##FNAME(JNIEnv *, jclass, jlong nativePtr, type fname) {		\
			model_calc_params& p = *((model_calc_params*)nativePtr);																		\
			p.fname = fname;																								\
		}

MODELCALC_UPDATE_FIELD(Gain, jdouble, gain)
MODELCALC_UPDATE_FIELD(Steps, jint, steps)
MODELCALC_UPDATE_FIELD(DT, jdouble, dt)

}
