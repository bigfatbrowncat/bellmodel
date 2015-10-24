package bellmodel;

import java.util.HashSet;
import java.util.Set;

public class ModelData {
	static {
		System.loadLibrary("bellmodel");
	}
	
	private static native long alloc();
	private static native void free(long nativePtr);
	private static native void add(long nativePtr, long matPointNativePtr);
	private static native void remove(long nativePtr, long matPointNativePtr);

	private static native void updateElasticity(long nativePtr, double elasticity);
	private static native void updateFriction(long nativePtr, double friction);

	long nativePtr;

	private double elasticity, friction;

	private Set<MatPoint> points = new HashSet<>();

	public ModelData(double elasticity, double friction) {
		nativePtr = alloc();

		setElasticity(elasticity);
		setFriction(friction);
	}

	public double getElasticity() {
		return elasticity;
	}
	
	public void setElasticity(double elasticity) {
		this.elasticity = elasticity;
		updateElasticity(nativePtr, elasticity);
	}
	
	public double getFriction() {
		return friction;
	}
	
	public void setFriction(double friction) {
		this.friction = friction;
		updateFriction(nativePtr, friction);
	}

	public void add(MatPoint p) {
		if (!points.contains(p)) {
			points.add(p);
			add(nativePtr, p.nativePtr);
		}
	}

	public void remove(MatPoint p) {
		if (points.contains(p)) {
			points.remove(p);
			remove(nativePtr, p.nativePtr);
		}
	}

	@Override
	protected void finalize() throws Throwable {
		free(nativePtr);
		super.finalize();
	}
}
