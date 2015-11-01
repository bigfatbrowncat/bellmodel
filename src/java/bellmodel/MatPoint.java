package bellmodel;

import java.util.HashSet;
import java.util.Set;

public class MatPoint {
	static {
		System.loadLibrary("bellmodel");
	}
	
	private static native long alloc();
	private static native void free(long nativePtr);
	private static native void connect(long nativePtr1, long nativePtr2);
	private static native void disconnect(long nativePtr1, long nativePtr2);

	private static native void updateM(long nativePtr, double m);
	private static native void updateX(long nativePtr, double x);
	private static native void updateY(long nativePtr, double y);
	private static native void updateZ(long nativePtr, double z);
	private static native void updateX0(long nativePtr, double x0);
	private static native void updateY0(long nativePtr, double y0);
	private static native void updateZ0(long nativePtr, double z0);
	private static native void updateVx(long nativePtr, double vx);
	private static native void updateVy(long nativePtr, double vy);
	private static native void updateVz(long nativePtr, double vz);
	private static native void updateMakesSound(long nativePtr, boolean makesSound);
	private static native void updateFixed(long nativePtr, boolean fixed);

	private static native double getM(long nativePtr);
	private static native double getX(long nativePtr);
	private static native double getY(long nativePtr);
	private static native double getZ(long nativePtr);
	private static native double getX0(long nativePtr);
	private static native double getY0(long nativePtr);
	private static native double getZ0(long nativePtr);
	private static native double getVx(long nativePtr);
	private static native double getVy(long nativePtr);
	private static native double getVz(long nativePtr);
	private static native boolean getMakesSound(long nativePtr);
	private static native boolean getFixed(long nativePtr);
	
	long nativePtr;

	private Set<MatPoint> connected = new HashSet<>();

	public MatPoint(double m, 
	                double x, double y, double z, 
	                double x0, double y0, double z0, 
	                double vx, double vy, double vz, boolean makesSound, boolean fixed) {
		nativePtr = alloc();

		updateM(nativePtr, m);
		updateX(nativePtr, x);
		updateY(nativePtr, y);
		updateZ(nativePtr, z);
		updateX0(nativePtr, x0);
		updateY0(nativePtr, y0);
		updateZ0(nativePtr, z0);
		updateVx(nativePtr, vx);
		updateVy(nativePtr, vy);
		updateVz(nativePtr, vz);
		updateMakesSound(nativePtr, makesSound);
		updateFixed(nativePtr, fixed);
	}

	public double getM() {
		return getM(nativePtr);
	}

	public void setM(double m) {
		updateM(nativePtr, m);
	}

	public double getX() {
		return getX(nativePtr);
	}

	public void setX(double x) {
		updateX(nativePtr, x);
	}

	public double getY() {
		return getY(nativePtr);
	}

	public void setY(double y) {
		updateY(nativePtr, y);
	}

	public double getZ() {
		return getX(nativePtr);
	}

	public void setZ(double z) {
		updateZ(nativePtr, z);
	}

	public double getX0() {
		return getX0(nativePtr);
	}

	public void setX0(double x0) {
		updateX0(nativePtr, x0);
	}

	public double getY0() {
		return getY0(nativePtr);
	}

	public void setY0(double y0) {
		updateY0(nativePtr, y0);
	}

	public double getZ0() {
		return getZ0(nativePtr);
	}

	public void setZ0(double z0) {
		updateZ0(nativePtr, z0);
	}

	public double getVx() {
		return getVx(nativePtr);
	}

	public void setVx(double vx) {
		updateVx(nativePtr, vx);
	}

	public double getVy() {
		return getVy(nativePtr);
	}

	public void setVy(double vy) {
		updateVy(nativePtr, vy);
	}

	public double getVz() {
		return getVz(nativePtr);
	}
	
	public void setVz(double vz) {
		updateVz(nativePtr, vz);
	}

	public void setMakesSound(boolean makesSound) {
		updateMakesSound(nativePtr, makesSound);
	}

	public boolean makesSound() {
		return getMakesSound(nativePtr);
	}

	public void setFixed(boolean fixed) {
		updateFixed(nativePtr, fixed);
	}

	public boolean isFixed() {
		return getFixed(nativePtr);
	}
	
	public static void connect(MatPoint p1, MatPoint p2) {
		assert(p1 != p2);
		assert(p1 != null && p2 != null);
		assert((p1.connected.contains(p2) && p2.connected.contains(p1))
				|| (!p1.connected.contains(p2)) && (!p2.connected.contains(p1)));

		if (!p1.connected.contains(p2)) {
			p1.connected.add(p2);
			p2.connected.add(p1);
			connect(p1.nativePtr, p2.nativePtr);
		}
	}

	public static void disconnect(MatPoint p1, MatPoint p2) {
		assert((p1.connected.contains(p2) && p2.connected.contains(p1))
				|| (!p1.connected.contains(p2)) && (!p2.connected.contains(p1)));

		if (p1.connected.contains(p2)) {
			p1.connected.remove(p2);
			p2.connected.remove(p1);
			disconnect(p1.nativePtr, p2.nativePtr);
		}
	}

	@Override
	protected void finalize() throws Throwable {
		free(nativePtr);
		super.finalize();
	}
}
