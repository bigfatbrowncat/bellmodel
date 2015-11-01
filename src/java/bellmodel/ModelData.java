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

	private static native double getElasticity(long nativePtr);
	private static native double getFriction(long nativePtr);

	long nativePtr;

	private Set<MatPoint> points = new HashSet<>();

	ModelData(long nativePtr) {
		this.nativePtr = nativePtr;
	}	
	
	public ModelData(double elasticity, double friction) {
		nativePtr = alloc();

		setElasticity(elasticity);
		setFriction(friction);
	}

	public double getElasticity() {
		return getElasticity(nativePtr);
	}
	
	public void setElasticity(double elasticity) {
		updateElasticity(nativePtr, elasticity);
	}
	
	public double getFriction() {
		return getFriction(nativePtr);
	}
	
	public void setFriction(double friction) {
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
