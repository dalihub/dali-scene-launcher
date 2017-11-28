package com.samsung.dali.modelconverter.data.document;

public class MatrixHelper {

  public static double[] createMatrix() {
    return new double[] { 1.0, .0, .0, .0,
                          .0, 1.0, .0, .0,
                          .0, .0, 1.0, .0,
                          .0, .0, .0, 1.0 };
  }

  public static double[] createMatrix(double[] data) {
    double[] matrix = new double[NUM_ELEMS];
    if (data.length != matrix.length) {
      throw new IllegalArgumentException("Matrix requires an array of 16 floating point values.");
    }

    for (int i = 0; i < matrix.length; ++i) {
      matrix[i] = data[i];
    }
    return matrix;
  }

  private static final int NUM_ELEMS = createMatrix().length;
}
