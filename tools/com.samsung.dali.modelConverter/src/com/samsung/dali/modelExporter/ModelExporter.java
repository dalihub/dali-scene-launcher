package com.samsung.dali.modelExporter;

public class ModelExporter {

	private static boolean s_initialised = false;
	
	public static void initialise()
	{
		if(!s_initialised)
		{
			System.loadLibrary("model-exporter-jni");
			s_initialised = true;
		}
	}
	
	/**
	 * @brief Performs the export.
	 * @param inputFile - the .dae file to process. Required.
	 * @param outputName - the name and path to save the .dli and .bin files.
	 * 						Optional. Will use the input path and name if omitted.
	 * @return 0 on success, 1 on failure.
	 */
	public static native int nativeExport(String inputFile, String outputName);
	
	/**
	 * @return An explanation on what might have gone wrong, if anything has.
	 * 			Null string if everything is fine.
	 * @note Consumes the error message.
	 */
	public static native String nativeGetErrorMessage();

	/**
	 * @return The path of the .dli file, if only nativeExport() was successful.
	 * @note Calling nativeExport resets the string returned.
	 */
	public static native String nativeGetDliPath();
}
