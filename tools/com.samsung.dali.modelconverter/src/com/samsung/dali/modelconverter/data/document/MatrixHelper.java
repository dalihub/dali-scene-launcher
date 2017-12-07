package com.samsung.dali.modelconverter.data.document;

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/*
 * A collection of helper functionality to help operate on double[]s (mostly) as 4x4 matrices.
 */
public class MatrixHelper {

  /*
   * Create an identity 4x4 matrix.
   */
  public static double[] createMatrix() {
    return new double[] { 1.0, .0, .0, .0, .0, 1.0, .0, .0, .0, .0, 1.0, .0, .0, .0, .0, 1.0 };
  }

  /*
   * Create a matrix copying the data, asserting that the source is the right
   * size.
   */
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

  /*
   * Obtain the translation part of a column-major 4x4 matrix.
   */
  public static double[] getTranslation(double[] matrix) {
    assert matrix.length == 16;
    return new double[] { matrix[12] / matrix[15], matrix[13] / matrix[15], matrix[14] / matrix[15] };
  }

  /*
   * Set the translation part of a column-major 4x4 matrix.
   */
  public static void setTranslation(double[] translation, double[] matrix) {
    for (int i = 0; i < 3; ++i) {
      matrix[12 + i] = translation[i];
    }
  }

  /*
   * Obtain the rotation along the three cardinal axes expressed in the given
   * column-major 4x4 matrix.
   */
  public static double[] getRotation(double[] matrix) {
    double[] rotation = new double[] { Math.atan2(matrix[6], matrix[10]) * DEGREES_SCALE,
        Math.atan2(-matrix[2], Math.sqrt(matrix[6] * matrix[6] + matrix[10] * matrix[10])) * DEGREES_SCALE,
        Math.atan2(matrix[1], matrix[0]) * DEGREES_SCALE };

    return rotation;
  }

  // TODO: public static void setRotation(double[] rotation, double[] matrix)
  // NOTE: input is degrees.

  /*
   * Obtain the scaling values in 3 axes, from a column-major 4x4 matrix.
   */
  public static double[] getScale(double[] matrix) {
    double[] scale = new double[] { getColumnMagnitude(matrix, 0), getColumnMagnitude(matrix, 1),
        getColumnMagnitude(matrix, 2) };
    return scale;
  }

  /*
   * Set the scaling expressed by a 4x4 matrix to the given values.
   */
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

  public static final double DEGREES_SCALE = 180.0 / Math.PI;

  // Gets the magnitude of one of the 1x3 sub-columns of a column-major 4x4
  // matrix.
  private static double getColumnMagnitude(double[] matrix, int i) {
    assert i < 4 && i >= 0;
    i *= 4;
    return Math.sqrt(matrix[i] * matrix[i] + matrix[i + 1] * matrix[i + 1] + matrix[i + 2] * matrix[i + 2]);
  }

  private static final int NUM_ELEMS = createMatrix().length;
}
