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

  public static double[] getTranslation(double[] matrix) {
    assert matrix.length == 16;
    return new double[] { matrix[12] / matrix[15], matrix[13] / matrix[15], matrix[14] / matrix[15] };
  }

  public static void setTranslation(double[] translation, double[] matrix) {
    for (int i = 0; i < 3; ++i) {
      matrix[12 + i] = translation[i];
    }
  }

  public static double[] getRotation(double[] matrix) {
    double[] rotation = new double[] { Math.atan2(matrix[6], matrix[10]),
        Math.atan2(-matrix[2], Math.sqrt(matrix[6] * matrix[6] + matrix[10] * matrix[10])),
        Math.atan2(matrix[1], matrix[0]) };

    return rotation;
  }
  
  // TODO: public static void setRotation(double[] rotation, double[] matrix) 

  public static double[] getScale(double[] matrix) {
    double[] scale = new double[] { getColumnMagnitude(matrix, 0), getColumnMagnitude(matrix, 1),
        getColumnMagnitude(matrix, 2) };
    return scale;
  }

  public static void setScale(double[] scale, double[] matrix) {
    double[] scaleCurr = getScale(matrix);
    for (int i = 0; i < 3; ++i) {
      scale[i] /= scaleCurr[i];
    }

    matrix[0] *= scale[0];
    matrix[1] *= scale[0];
    matrix[2] *= scale[0];

    matrix[4] *= scale[1];
    matrix[5] *= scale[1];
    matrix[6] *= scale[1];

    matrix[8] *= scale[2];
    matrix[9] *= scale[2];
    matrix[10] *= scale[2];
  }

  private static double getColumnMagnitude(double[] matrix, int i) {
    assert i < 4 && i >= 0;
    i *= 4;
    return Math.sqrt(matrix[i] * matrix[i] + matrix[i + 1] * matrix[i + 1] + matrix[i + 2] * matrix[i + 2]);
  }

  private static final int NUM_ELEMS = createMatrix().length;
}
