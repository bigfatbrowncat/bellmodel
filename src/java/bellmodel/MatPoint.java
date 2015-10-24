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

	long nativePtr;

	private double m;
	private double x, y, z;
	private double x0, y0, z0;
	private double vx, vy, vz;
	private boolean makesSound;
	private boolean fixed;

	private Set<MatPoint> connected = new HashSet<>();

	public MatPoint(double m, 
	                double x, double y, double z, 
	                double x0, double y0, double z0, 
	                double vx, double vy, double vz, boolean makesSound, boolean fixed) {
		nativePtr = alloc();

		setM(m);
		setX(x);
		setY(y);
		setZ(z);
		setX0(x0);
		setY0(y0);
		setZ0(z0);
		setVx(vx);
		setVy(vy);
		setVz(vz);
		setMakesSound(makesSound);
		setFixed(fixed);
	}

	public double getM() {
		return m;
	}

	public void setM(double m) {
		this.m = m;
		updateM(nativePtr, m);
	}

	public double getX() {
		return x;
	}

	public void setX(double x) {
		this.x = x;
		updateX(nativePtr, x);
	}

	public double getY() {
		return y;
	}

	public void setY(double y) {
		this.y = y;
		updateY(nativePtr, y);
	}

	public double getZ() {
		return z;
	}

	public void setZ(double z) {
		this.z = z;
		updateZ(nativePtr, z);
	}

	public double getX0() {
		return x0;
	}

	public void setX0(double x0) {
		this.x0 = x0;
		updateX0(nativePtr, x0);
	}

	public double getY0() {
		return y0;
	}

	public void setY0(double y0) {
		this.y0 = y0;
		updateY0(nativePtr, y0);
	}

	public double getZ0() {
		return z0;
	}

	public void setZ0(double z0) {
		this.z0 = z0;
		updateZ0(nativePtr, z0);
	}

	public double getVx() {
		return vx;
	}

	public void setVx(double vx) {
		this.vx = vx;
		updateVx(nativePtr, vx);
	}

	public double getVy() {
		return vy;
	}

	public void setVy(double vy) {
		this.vy = vy;
		updateVy(nativePtr, vy);
	}

	public double getVz() {
		return vz;
	}
	
	public void setVz(double vz) {
		this.vz = vz;
		updateVz(nativePtr, vz);
	}

	public void setMakesSound(boolean makesSound) {
		this.makesSound = makesSound;
		updateMakesSound(nativePtr, makesSound);
	}

	public boolean makesSound() {
		return makesSound;
	}

	public void setFixed(boolean fixed) {
		this.fixed = fixed;
		updateFixed(nativePtr, fixed);
	}

	public boolean isFixed() {
		return fixed;
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
