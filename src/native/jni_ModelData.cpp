#include <jni.h>

#include <algorithm>

#include "bell-model.h"

using namespace std;

extern "C" {

JNIEXPORT jlong JNICALL Java_bellmodel_ModelData_alloc(JNIEnv *, jclass) {
	return (jlong)(new mat_point());
}
JNIEXPORT void JNICALL Java_bellmodel_ModelData_free(JNIEnv *, jclass, jlong nativePtr) {
	delete (mat_point*)nativePtr;
}

JNIEXPORT void JNICALL Java_bellmodel_ModelData_add(JNIEnv *, jclass, jlong nativePtr, jlong matPointNativePtr) {
	mat_point& p = *((mat_point*)matPointNativePtr);
	model_data& d = *((model_data*)nativePtr);

	d.points_v.push_back(&p);
}

JNIEXPORT void JNICALL Java_bellmodel_ModelData_remove(JNIEnv *, jclass, jlong nativePtr, jlong matPointNativePtr) {
	mat_point& p = *((mat_point*)matPointNativePtr);
	model_data& d = *((model_data*)nativePtr);

	d.points_v.erase(std::remove(d.points_v.begin(), d.points_v.end(), &p), d.points_v.end());
}

#define MODELDATA_UPDATE_FIELD(FNAME, type, fname)	\
		JNIEXPORT void JNICALL Java_bellmodel_ModelData_update##FNAME(JNIEnv *, jclass, jlong nativePtr, type fname) {		\
			model_data& p = *((model_data*)nativePtr);																		\
			p.fname = fname;																								\
		}

MODELDATA_UPDATE_FIELD(Elasticity, jdouble, elasticity)
MODELDATA_UPDATE_FIELD(Friction, jdouble, friction)

}
