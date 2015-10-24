#include <jni.h>

#include <algorithm>

#include "bell-model.h"

using namespace std;

extern "C" {

JNIEXPORT jlong JNICALL Java_bellmodel_MatPoint_alloc(JNIEnv *, jclass) {
	jlong res = (jlong)(new mat_point());
	//printf("matpoint: %x\n", res);fflush(stdout);
	return res;
}
JNIEXPORT void JNICALL Java_bellmodel_MatPoint_free(JNIEnv *, jclass, jlong nativePtr) {
	delete (mat_point*)nativePtr;
}

JNIEXPORT void JNICALL Java_bellmodel_MatPoint_connect(JNIEnv *, jclass, jlong nativePtr1, jlong nativePtr2) {
	mat_point& p1 = *((mat_point*)nativePtr1);
	mat_point& p2 = *((mat_point*)nativePtr2);

	//printf("p1: %x\n", &p1);fflush(stdout);
	//printf("p2: %x\n", &p2);fflush(stdout);

	p1.connected_v.push_back(&p2);
	p2.connected_v.push_back(&p1);

	//printf("p1.cv.len: %d\n", p1.connected_v.size());fflush(stdout);
	//printf("p2.cv.len: %d\n", p2.connected_v.size());fflush(stdout);

}

JNIEXPORT void JNICALL Java_bellmodel_MatPoint_disconnect(JNIEnv *, jclass, jlong nativePtr1, jlong nativePtr2) {
	mat_point& p1 = *((mat_point*)nativePtr1);
	mat_point& p2 = *((mat_point*)nativePtr2);

	p1.connected_v.erase(std::remove(p1.connected_v.begin(), p1.connected_v.end(), &p2), p1.connected_v.end());
	p2.connected_v.erase(std::remove(p2.connected_v.begin(), p2.connected_v.end(), &p1), p2.connected_v.end());
}

#define MATPOINT_UPDATE_FIELD(FNAME, type, fname)	\
		JNIEXPORT void JNICALL Java_bellmodel_MatPoint_update##FNAME(JNIEnv *, jclass, jlong nativePtr, type fname) {		\
			mat_point& p = *((mat_point*)nativePtr);																		\
			p.fname = fname;																								\
		}

MATPOINT_UPDATE_FIELD(M, jdouble, m)
MATPOINT_UPDATE_FIELD(X, jdouble, x)
MATPOINT_UPDATE_FIELD(Y, jdouble, y)
MATPOINT_UPDATE_FIELD(Z, jdouble, z)
MATPOINT_UPDATE_FIELD(X0, jdouble, x0)
MATPOINT_UPDATE_FIELD(Y0, jdouble, y0)
MATPOINT_UPDATE_FIELD(Z0, jdouble, z0)
MATPOINT_UPDATE_FIELD(Vx, jdouble, vx)
MATPOINT_UPDATE_FIELD(Vy, jdouble, vy)
MATPOINT_UPDATE_FIELD(Vz, jdouble, vz)
MATPOINT_UPDATE_FIELD(MakesSound, jboolean, makes_sound)
MATPOINT_UPDATE_FIELD(Fixed, jboolean, fixed)


}
