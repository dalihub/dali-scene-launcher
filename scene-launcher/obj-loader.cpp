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

// CLASS HEADER
#include "obj-loader.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <sstream>
#include <string.h>

namespace PbrDemo
{

namespace
{
const int MAX_POINT_INDICES = 4;

} // namespace

ObjLoader::ObjLoader()
: mSceneLoaded( false )
{
  mSceneAABB.Init();
}

ObjLoader::~ObjLoader()
{
  ClearArrays();
}

bool ObjLoader::IsSceneLoaded() const
{
  return mSceneLoaded;
}

void ObjLoader::CalculateHardFaceNormals( Object& object )
{
  int numFaceVertices = 3 * object.triangles.Size();  //Vertex per face, as each point has different normals for each face.

  object.normals.Clear();
  object.normals.Resize( numFaceVertices );

  //For each triangle, calculate the normal by crossing two vectors on the triangle's plane.
  for( Dali::Vector<TriIndex>::Iterator it = object.triangles.Begin(), endIt = object.triangles.End();
       it != endIt;
       ++it )
  {
    TriIndex& triangle = *it;

    //Triangle vertices.
    const Vector3& v0 = mPoints[triangle.pointIndex[0]];
    const Vector3& v1 = mPoints[triangle.pointIndex[1]];
    const Vector3& v2 = mPoints[triangle.pointIndex[2]];

    //Triangle edges.
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    //Using edges as vectors on the plane, cross product to get the normal.
    Vector3 normalVector = edge1.Cross(edge2);
    normalVector.Normalize();

    //Assign normal index to triangle vertex and set the normal vector to the list of normals.
    for( unsigned long j = 0, normalIndex = 0; j < 3; ++j, ++normalIndex )
    {
      triangle.normalIndex[j] = normalIndex;
      object.normals[normalIndex] = normalVector;
    }
  }
}

void ObjLoader::FindNormalIndices( const Vector<int>& pointIndexToNormalIndex,
                                   int numberOfNormalIndices,
                                   TriIndex& triangle )
{
  for( int index = 0;
       ( ( index < numberOfNormalIndices ) &&
         ( ( -1 == triangle.normalIndex[0] ) || ( -1 == triangle.normalIndex[1] ) || ( -1 == triangle.normalIndex[2] ) ) );
         ++index )
  {
    if( triangle.pointIndex[0] == pointIndexToNormalIndex[index] )
    {
      triangle.normalIndex[0] = index;
    }
    if( triangle.pointIndex[1] == pointIndexToNormalIndex[index] )
    {
      triangle.normalIndex[1] = index;
    }
    if( triangle.pointIndex[2] == pointIndexToNormalIndex[index] )
    {
      triangle.normalIndex[2] = index;
    }
  }
}

void ObjLoader::CalculateSoftFaceNormals( Object& object )
{
  const unsigned int numberOfNormals =  3u * object.triangles.Count();
  object.normals.Clear();
  object.normals.Resize( numberOfNormals );  //One (averaged) normal per point.

  Vector<int> pointIndexToNormalIndex;
  pointIndexToNormalIndex.Resize( numberOfNormals );
  int numberOfNormalIndices = 0u;

  //For each triangle, calculate the normal by crossing two vectors on the triangle's plane
  //We then add the triangle's normal to the cumulative normals at each point of it
  for( Dali::Vector<TriIndex>::Iterator it = object.triangles.Begin(), endIt = object.triangles.End();
       it != endIt;
       ++it )
  {
    TriIndex& triangle = *it;

    // Find the normal indices for these points.
    triangle.normalIndex[0] = -1;
    triangle.normalIndex[1] = -1;
    triangle.normalIndex[2] = -1;

    FindNormalIndices( pointIndexToNormalIndex,
                       numberOfNormalIndices,
                       triangle );

    for( unsigned long j = 0; j < 3; ++j )
    {
      if( -1 == triangle.normalIndex[j] )
      {
        pointIndexToNormalIndex[numberOfNormalIndices] = triangle.pointIndex[j];
        triangle.normalIndex[j] = numberOfNormalIndices;
        ++numberOfNormalIndices;
      }
    }

    //Triangle points.
    const Vector3& v0 = mPoints[triangle.pointIndex[0]];
    const Vector3& v1 = mPoints[triangle.pointIndex[1]];
    const Vector3& v2 = mPoints[triangle.pointIndex[2]];

    //Triangle edges.
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    //Using edges as vectors on the plane, cross to get the normal.
    Vector3 normalVector = edge1.Cross(edge2);

    //Add this triangle's normal to the cumulative normal of each constituent triangle point and set the index of the normal accordingly.
    for( unsigned long j = 0; j < 3; ++j )
    {
      //Normal index matches up to vertex index, as one normal per vertex.
      object.normals[triangle.normalIndex[j]] += normalVector;
    }
  }

  //Normalise the normals.
  for( Vector<Vector3>::Iterator it = object.normals.Begin(), endIt = object.normals.End();
       it != endIt;
       ++it )
  {
    Vector3& normal = *it;

    normal.Normalize();
  }
}

void ObjLoader::CalculateTangentFrame( Object& object )
{
  //Reset tangent vector to hold new values.
  object.tangents.Clear();

  object.tangents.Resize( object.hasNormals ? 3 * object.triangles.Count() : object.normals.Size() );

  //For each triangle, calculate the tangent vector and then add it to the total tangent vector of each normal.
  for( Dali::Vector<TriIndex>::Iterator it = object.triangles.Begin(), endIt = object.triangles.End();
       it != endIt;
       ++it )
  {
    TriIndex& triangle = *it;

    Vector3 tangentVector;

    const Vector3& v0 = mPoints[triangle.pointIndex[0]];
    const Vector3& v1 = mPoints[triangle.pointIndex[1]];
    const Vector3& v2 = mPoints[triangle.pointIndex[2]];

    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    const Vector2& w0 = mTextureUv[triangle.textureIndex[0]];
    const Vector2& w1 = mTextureUv[triangle.textureIndex[1]];
    const Vector2& w2 = mTextureUv[triangle.textureIndex[2]];

    float deltaU1 = w1.x - w0.x;
    float deltaV1 = w1.y - w0.y;
    float deltaU2 = w2.x - w0.x;
    float deltaV2 = w2.y - w0.y;

    // 1.0/f could cause division by zero in some cases, this factor will act
    // as a weight of the tangent vector and it is fixed when it is normalised.
    float f = (deltaU1 * deltaV2 - deltaU2 * deltaV1);

    tangentVector.x = f * ( deltaV2 * edge1.x - deltaV1 * edge2.x );
    tangentVector.y = f * ( deltaV2 * edge1.y - deltaV1 * edge2.y );
    tangentVector.z = f * ( deltaV2 * edge1.z - deltaV1 * edge2.z );

    object.tangents[triangle.normalIndex[0]] += tangentVector;
    object.tangents[triangle.normalIndex[1]] += tangentVector;
    object.tangents[triangle.normalIndex[2]] += tangentVector;
  }

  //Orthogonalize tangents.
  for( unsigned long a = 0; a < object.tangents.Size(); a++ )
  {
    const Vector3& n = object.normals[a];
    const Vector3& t = object.tangents[a];

    // Gram-Schmidt orthogonalize
    object.tangents[a] = t - n * n.Dot(t);
    object.tangents[a].Normalize();
  }
}

void ObjLoader::CenterAndScale( bool center )
{
  BoundingVolume newAABB;

  Vector3 sceneSize = GetSize();

  float biggestDimension = sceneSize.x;
  if( sceneSize.y > biggestDimension )
  {
    biggestDimension = sceneSize.y;
  }
  if( sceneSize.z > biggestDimension )
  {
    biggestDimension = sceneSize.z;
  }

  newAABB.Init();
  for( Vector<Vector3>::Iterator it = mPoints.Begin(), endIt = mPoints.End();
       it != endIt;
       ++it )
  {
    Vector3& point = *it;
    point = point - GetCenter();
    point = point / biggestDimension;
    newAABB.ConsiderNewPointInVolume( point );
  }

  mSceneAABB = newAABB;
}

void ObjLoader::CreateGeometryArray( Object& object,
                                     Dali::Vector<Vector3>& positions,
                                     Dali::Vector<Vector3>& normals,
                                     Dali::Vector<Vector3>& tangents,
                                     Dali::Vector<Vector2>& textures,
                                     Dali::Vector<unsigned short>& indices,
                                     bool useSoftNormals )
{
  const int numVertices = 3 * object.triangles.Size();

  if( object.hasTextureCoords )
  {
    if( object.hasNormals )
    {
      normals.Resize( numVertices );

      int index = 0;
      // Copy the supplied normals to be used to calculate the tangents
      for( Dali::Vector<TriIndex>::Iterator it = object.triangles.Begin(), endIt = object.triangles.End();
           it != endIt;
           ++it )
      {
        TriIndex& triangle = *it;

        for( int j = 0 ; j < 3 ; ++j, ++index )
        {
          normals[index] = mNormals[triangle.normalIndex[j]];
          triangle.normalIndex[j] = index;
        }
      }

      //Copy the supplied normals to the normalized array that is used to draw.
      object.normals = normals;
    }
    else
    {
      // Normals must be calculated if they weren't supplied and tangents need to be calculated.
      if( useSoftNormals )
      {
        CalculateSoftFaceNormals( object );
      }
      else
      {
        CalculateHardFaceNormals( object );
      }

      //Copy the calculated normals to the normalized array that is used to draw.
      normals.Resize( numVertices );

      int index = 0;
      for( Dali::Vector<TriIndex>::ConstIterator it = object.triangles.Begin(), endIt = object.triangles.End();
           it != endIt;
           ++it )
      {
        const TriIndex& triangle = *it;

        for( int j = 0 ; j < 3 ; ++j, ++index )
        {
          // Store the normals ...
          normals[index] = object.normals[triangle.normalIndex[j]];
        }
      }
    }

    CalculateTangentFrame( object );
  }

  positions.Resize( numVertices );
  textures.Resize( numVertices );
  tangents.Resize( numVertices );

  int index = 0;

  //We have to normalize the arrays so we can draw we just one index array. The normals already have been copied,
  for( Dali::Vector<TriIndex>::ConstIterator it = object.triangles.Begin(), endIt = object.triangles.End();
       it != endIt;
       ++it )
  {
    const TriIndex& triangle = *it;

    for( int j = 0 ; j < 3 ; ++j )
    {
      // Store the points
      positions[index] = mPoints[triangle.pointIndex[j]];

      // Store texture coords and tangents ...
      if( object.hasTextureCoords )
      {
        textures[index] = mTextureUv[triangle.textureIndex[j]];

        tangents[index] = object.tangents[triangle.normalIndex[j]];
      }

      ++index;
    }
  }
}

bool ObjLoader::LoadObject( char* objBuffer, std::streampos fileSize )
{
  Vector3 point;
  Vector2 texture;
  std::string vet[MAX_POINT_INDICES];
  int ptIdx[MAX_POINT_INDICES];
  int nrmIdx[MAX_POINT_INDICES];
  int texIdx[MAX_POINT_INDICES];
  TriIndex triangle,triangle2;
  bool iniObj = false;
  int face = 0;
  Object object;

  for( int index = 0; index < MAX_POINT_INDICES; ++index )
  {
    ptIdx[index] = 1;
    nrmIdx[index] = 1;
    texIdx[index] = 1;
  }

  //Init AABB for the file
  mSceneAABB.Init();

  std::string strMatActual;

  std::string input( objBuffer, fileSize );
  std::istringstream ss(input);
  ss.imbue( std::locale( "C" ) );

  std::string line;
  std::getline( ss, line );

  bool readingFaces = false;

  while( std::getline( ss, line ) )
  {
    std::istringstream isline( line, std::istringstream::in );
    std::string tag;

    isline >> tag;

    if( readingFaces && ( ( tag != "f" ) && ( tag != "s" ) && ( !tag.empty() ) ) )
    {
      // This is the first line after a set of faces.
      // Store the object and initialize a new one.
      readingFaces = false;

      mObjects.push_back( object );

      object = Object();
    }

    if ( tag == "v" )
    {
      //Two different objects in the same file
      isline >> point.x;
      isline >> point.y;
      isline >> point.z;

      mPoints.PushBack( point );

      mSceneAABB.ConsiderNewPointInVolume( point );
    }
    else if ( tag == "vn" )
    {
      isline >> point.x;
      isline >> point.y;
      isline >> point.z;

      mNormals.PushBack( point );
    }
    else if ( tag == "#_#tangent" )
    {
      isline >> point.x;
      isline >> point.y;
      isline >> point.z;
    }
    else if ( tag == "#_#binormal" )
    {
      isline >> point.x;
      isline >> point.y;
      isline >> point.z;
    }
    else if ( tag == "vt" )
    {
      isline >> texture.x;
      isline >> texture.y;
      texture.y = 1.f - texture.y;

      mTextureUv.PushBack( texture );
    }
    else if ( tag == "#_#vt1" )
    {
      isline >> texture.x;
      isline >> texture.y;
    }
    else if ( tag == "s" )
    {
    }
    else if ( tag == "f" )
    {
      readingFaces = true;

      if ( !iniObj )
      {
        //name assign

        iniObj = true;
      }

      int numIndices = 0;
      while( ( numIndices < MAX_POINT_INDICES ) && ( isline >> vet[numIndices] ) )
      {
        numIndices++;
      }

      //Hold slashes that separate attributes of the same point.
      char separator;
      char separator2;

      std::size_t stringPos1 = vet[0].find_first_of( "/" ); //Search for the first '/'
      std::size_t stringPos2 = vet[0].find_first_of( "/", stringPos1 + 1 ); //Search for the second '/'

      if( std::string::npos != stringPos1 )
      {
        if( stringPos1 + 1 == stringPos2 ) // Of the form A//C, so has points and normals but no texture coordinates.
        {
          for( int i = 0 ; i < numIndices; i++)
          {
            std::istringstream isindex( vet[i] );
            isindex >> ptIdx[i] >> separator >> separator2 >> nrmIdx[i];
            texIdx[i] = 0;
          }
          object.hasTextureCoords = false;
        }
        else if( std::string::npos != stringPos2 ) // Of the form A/B/C, so has points, textures and normals.
        {
          for( int i = 0 ; i < numIndices; i++ )
          {
            std::istringstream isindex( vet[i] );
            isindex >> ptIdx[i] >> separator >> texIdx[i] >> separator2 >> nrmIdx[i];
          }
        }
        else // Of the form A/B, so has points and textures but no normals.
        {
          for( int i = 0 ; i < numIndices; i++ )
          {
            std::istringstream isindex( vet[i] );
            isindex >> ptIdx[i] >> separator >> texIdx[i];
            nrmIdx[i] = 0;
          }
          object.hasNormals = false;
        }
      }
      else // Simply of the form A, as in, point indices only.
      {
        for( int i = 0 ; i < numIndices; i++ )
        {
          std::istringstream isindex( vet[i] );
          isindex >> ptIdx[i];
          texIdx[i] = 0;
          nrmIdx[i] = 0;
        }
        object.hasNormals = false;
        object.hasTextureCoords = false;
      }

      //If it is a triangle
      if( numIndices == 3 )
      {
        for( int i = 0 ; i < 3; i++ )
        {
          triangle.pointIndex[i] = ptIdx[i] - 1;
          triangle.normalIndex[i] = nrmIdx[i] - 1;
          triangle.textureIndex[i] = texIdx[i] - 1;
        }
        object.triangles.PushBack( triangle );
        face++;
      }
      //If on the other hand it is a quad, we will create two triangles
      else if( numIndices == 4 )
      {
        for( int i = 0 ; i < 3; i++ )
        {
          triangle.pointIndex[i] = ptIdx[i] - 1;
          triangle.normalIndex[i] = nrmIdx[i] - 1;
          triangle.textureIndex[i] = texIdx[i] - 1;
        }
        object.triangles.PushBack( triangle );
        face++;

        for( int i = 0 ; i < 3; i++ )
        {
          int idx = ( i + 2 ) % numIndices;
          triangle2.pointIndex[i] = ptIdx[idx] - 1;
          triangle2.normalIndex[i] = nrmIdx[idx] - 1;
          triangle2.textureIndex[i] = texIdx[idx] - 1;
        }
        object.triangles.PushBack( triangle2 );
        face++;
      }
    }
    else if ( tag == "usemtl" )
    {
      isline >> strMatActual;
    }
    else if ( tag == "mtllib" )
    {
      isline >> strMatActual;
    }
    else if ( tag == "g" )
    {
      isline >> object.name;
    }
  }

  if( readingFaces )
  {
    // Add the last object.
    mObjects.push_back( object );
  }

  if ( iniObj )
  {
    CenterAndScale( true );
    mSceneLoaded = true;
    return true;
  }

  return false;
}

void ObjLoader::CreateGeometry( int objectProperties, bool useSoftNormals, std::vector<Geometry>& geometry, std::vector<std::string>& names )
{
  if( mObjects.empty() )
  {
    return;
  }

  for( std::vector<Object>::iterator it = mObjects.begin(), endIt = mObjects.end();
       it != endIt;
       ++it )
  {
    Object& object = *it;

    Geometry surface = Geometry::New();

    Dali::Vector<Vector3> positions;
    Dali::Vector<Vector3> normals;
    Dali::Vector<Vector3> tangents;
    Dali::Vector<Vector2> textures;
    Dali::Vector<unsigned short> indices;

    CreateGeometryArray( object, positions, normals, tangents, textures, indices, useSoftNormals );

    //All vertices need at least Position and Normal

    Property::Map positionMap;
    positionMap["aPosition"] = Property::VECTOR3;
    PropertyBuffer positionBuffer = PropertyBuffer::New( positionMap );
    positionBuffer.SetData( positions.Begin(), positions.Count() );

    Property::Map normalMap;
    normalMap["aNormal"] = Property::VECTOR3;
    PropertyBuffer normalBuffer = PropertyBuffer::New( normalMap );
    normalBuffer.SetData( normals.Begin(), normals.Count() );

    surface.AddVertexBuffer( positionBuffer );
    surface.AddVertexBuffer( normalBuffer );

    //Some need tangent
    if( ( objectProperties & TANGENTS ) && ( 0u != tangents.Count() ) )
    {
      Property::Map tangentMap;
      tangentMap["aTangent"] = Property::VECTOR3;
      PropertyBuffer tangentBuffer = PropertyBuffer::New( tangentMap );
      tangentBuffer.SetData( tangents.Begin(), tangents.Count() );

      surface.AddVertexBuffer( tangentBuffer );
    }

    //Some need texture coordinates
    if( ( objectProperties & TEXTURE_COORDINATES ) && ( 0u != textures.Count() ) )
    {
      Property::Map textCoordMap;
      textCoordMap["aTexCoord"] = Property::VECTOR2;
      PropertyBuffer texCoordBuffer = PropertyBuffer::New( textCoordMap );
      texCoordBuffer.SetData( textures.Begin(), textures.Count() );

      surface.AddVertexBuffer( texCoordBuffer );
    }

    //If indices are required, we set them.
    if ( indices.Size() )
    {
      surface.SetIndexBuffer ( &indices[0], indices.Size() );
    }

    geometry.push_back( surface );
    names.push_back( object.name );
  }
}

Vector3 ObjLoader::GetCenter() const
{
  Vector3 center = GetSize() * 0.5 + mSceneAABB.pointMin;
  return center;
}

Vector3 ObjLoader::GetSize() const
{
  Vector3 size = mSceneAABB.pointMax - mSceneAABB.pointMin;
  return size;
}

void ObjLoader::ClearArrays()
{
  mPoints.Clear();
  mTextureUv.Clear();
  mNormals.Clear();
  mObjects.clear();

  mSceneLoaded = false;
}

} // namespace PbrDemo
