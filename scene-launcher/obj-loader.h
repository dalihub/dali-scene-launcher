#ifndef DALI_DEMO_PBR_OBJ_LOADER_H
#define DALI_DEMO_PBR_OBJ_LOADER_H

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

// EXTERNAL INCLUDES
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/rendering/geometry.h>
#include <limits>

using namespace Dali;

namespace PbrDemo
{

class ObjLoader
{
public:

  struct BoundingVolume
  {
    void Init()
    {
      pointMin = Vector3( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
      pointMax = Vector3( std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() );
    }

    void ConsiderNewPointInVolume( const Vector3& position )
    {
      pointMin.x = std::min( position.x, pointMin.x );
      pointMin.y = std::min( position.y, pointMin.y );
      pointMin.z = std::min( position.z, pointMin.z );

      pointMax.x = std::max( position.x, pointMax.x );
      pointMax.y = std::max( position.y, pointMax.y );
      pointMax.z = std::max( position.z, pointMax.z );
    }

    Vector3 pointMin;
    Vector3 pointMax;
  };

  //Defines bit masks to declare which properties are needed by anyone requesting a geometry.
  enum ObjectProperties
  {
    TEXTURE_COORDINATES = 1 << 0,
    TANGENTS = 1 << 1,
    BINORMALS = 1 << 2
  };

  /**
   * @brief Constructor.
   */
  ObjLoader();

  /**
   * @brief Destructor.
   *
   */
  virtual ~ObjLoader();

  /**
   * @return Whether the scene is loaded.
   */
  bool IsSceneLoaded() const;

  /*
   * @brief Parses the file from data objBuffer and size fileSize.
   *
   * @param[in] objBuffer file content from OBJ file.
   * @param[in] fileSize length of objBuffer.
   */
  bool LoadObject( char* objBuffer, std::streampos fileSize );

  /**
   * @brief Creates a geometry from a @e obj model.
   *
   * @param[in] objectProperties A set of flags to calculate tangents, binormals
   * @param[in] useSoftNormals average normals if the OBJ file doesn't contain normals
   * @param[out] geometry, an array of Geometry, each element correspond to a group in OBJ file
   * @param[out] names, array of string that correspond to name of groups in OBJ file
   */
  void CreateGeometry( int objectProperties, bool useSoftNormals, std::vector<Geometry>& geometry, std::vector<std::string>& names );

  /**
   * @brief Retrieves the middle point of the boundary box.
   *
   * @return point
   */
  Vector3 GetCenter() const;

  /**
   * @brief Retrieves the size of the boundary box.
   *
   * @return vector size
   */
  Vector3 GetSize() const;

  /**
   * @brief Clear buffer used.
   *
   */
  void ClearArrays();

private:

  struct TriIndex
  {
    int pointIndex[3];
    int normalIndex[3];
    int textureIndex[3];
  };

  struct Object
  {
    Object()
    : name(),
      normals(),
      tangents(),
      triangles(),
      hasNormals( true ),
      hasTextureCoords( true )
    {}

    ~Object() {}

    std::string            name;
    Dali::Vector<Vector3>  normals;   ///< Normals for the object. Not shared with other objects.
    Dali::Vector<Vector3>  tangents;  ///< Tangents for the object. Not shared with other objects.
    Dali::Vector<TriIndex> triangles;

    bool hasNormals : 1;
    bool hasTextureCoords : 1;
  };

  Dali::Vector<Vector3>  mPoints;    ///< Positions, texture coords, normals and tangents of all objects are stored
  Dali::Vector<Vector2>  mTextureUv; ///< in single vectors. In a .obj file it may happen that two or more objects
  Dali::Vector<Vector3>  mNormals;   ///< share the geometry. Face indices point to these vectors.

  std::vector<Object> mObjects;

  BoundingVolume mSceneAABB;

  bool mSceneLoaded;

  /**
   * @brief Calculates normals for each point on a per-face basis.
   *
   * There are multiple normals per point, each corresponding to the normal of a face connecting to the point.
   *
   * @param[in, out] object The object with vertices, the triangles that form the faces and the normals to be calculated.
   */
  void CalculateHardFaceNormals( Object& object );

  /**
   * @brief Finds the normal indices for the given point indices.
   *
   * @param[in] pointIndexToNormalIndex Vector with indices to points. The position in the vector is the index to the normal.
   * @param[in] numberOfNormalIndices The number of normal indices already added to the vector.
   * @param[in,out] triangle Triangle with the point indices and the normal indices to be filled.
   */
  void FindNormalIndices( const Vector<int>& pointIndexToNormalIndex,
                          int numberOfNormalIndices,
                          TriIndex& triangle );

  /**
   * @brief Calculates smoothed normals for each point.
   *
   * There is one normal per point, an average of the connecting faces.
   *
   * @param[in, out] object The object with vertices, the triangles that form the faces and the normals to be calculated.
   */
  void CalculateSoftFaceNormals( Object& object );

  /**
   * @brief Calculates tangents for each point of the object.
   *
   * These are calculated using the object's points, texture coordinates and normals, so these must be initialised first.
   */
  void CalculateTangentFrame( Object& object );

  /**
   * @brief Changes the point scale to fit in 1,1,1 box and centers according with parameter center.
   *
   * @param[in] center, if the point should be moved to center.
   */
  void CenterAndScale( bool center );

  /**
   * @brief Using the data loaded from the file, create arrays of data to be used in creating the geometry.
   *
   * @param[in, out] object The object with the data loaded from file.
   * @param[out] positions The positions of the vertices of the object.
   * @param[out] normals The normals of the vertices of the object.
   * @param[out] tangents The tangents of the vertices of the object.
   * @param[out] textures The texture coordinates of the vertices of the object.
   * @param[out] indices Indices of corresponding values to match triangles to their respective data.
   * @param[in] useSoftNormals Indicates whether we should average the normals at each point to smooth the surface or not.
   */
  void CreateGeometryArray( Object& object,
                            Dali::Vector<Vector3>& positions,
                            Dali::Vector<Vector3>& normals,
                            Dali::Vector<Vector3>& tangents,
                            Dali::Vector<Vector2>& textures,
                            Dali::Vector<unsigned short>& indices,
                            bool useSoftNormals );

};

} // namespace PbrDemo

#endif // DALI_DEMO_PBR_OBJ_LOADER_H
