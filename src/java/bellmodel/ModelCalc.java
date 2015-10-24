package bellmodel;

public class ModelCalc {
	static {
		System.loadLibrary("bellmodel");
	}
	
	private static native long alloc();
	private static native void free(long nativePtr);

	private static native void updateModelData(long nativePtr, long modelDataNativePtr);
	private static native void updateGain(long nativePtr, double gain);
	private static native void updateSteps(long nativePtr, int steps);
	private static native void updateDT(long nativePtr, double dt);
	
	private static native double doStep(long nativePtr);

	private long nativePtr;

	private double gain, dt;
	private int steps;

	private ModelData modelData;

	public ModelCalc(ModelData modelData, double gain, int steps, double dt) {
		nativePtr = alloc();
		setModelData(modelData);
		setGain(gain);
		setSteps(steps);
		setDT(dt);
	}

	public double getGain() {
		return gain;
	}
	public void setGain(double gain) {
		this.gain = gain;
		updateGain(nativePtr, gain);
	}
	public double getDT() {
		return dt;
	}
	public void setDT(double dt) {
		this.dt = dt;
		updateDT(nativePtr, dt);
	}
	public int getSteps() {
		return steps;
	}
	public void setSteps(int steps) {
		this.steps = steps;
		updateSteps(nativePtr, steps);
	}
	public ModelData getModelData() {
		return modelData;
	}
	public void setModelData(ModelData modelData) {
		this.modelData = modelData;
		if (modelData != null) {
			updateModelData(nativePtr, modelData.nativePtr);
		} else {
			updateModelData(nativePtr, 0);
		}
	}
	public double doStep() {
		return doStep(nativePtr);
	}
	
	@Override
	protected void finalize() throws Throwable {
		free(nativePtr);
		super.finalize();
	}
}
