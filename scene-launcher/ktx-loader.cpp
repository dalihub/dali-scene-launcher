/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
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

// FILE HEADER
#include "ktx-loader.h"

// EXTERNAL INCLUDES
#include <memory.h>

namespace SceneLauncher
{

struct KtxFileHeader
{
  char   identifier[12];
  uint32_t  endianness;
  uint32_t  glType;    //(UNSIGNED_BYTE, UNSIGNED_SHORT_5_6_5, etc.)
  uint32_t  glTypeSize;
  uint32_t  glFormat;  //(RGB, RGBA, BGRA, etc.)
  uint32_t  glInternalFormat; //For uncompressed textures, specifies the internalformat parameter passed to glTexStorage*D or glTexImage*D
  uint32_t  glBaseInternalFormat;
  uint32_t  pixelWidth;
  uint32_t  pixelHeight;
  uint32_t  pixelDepth;
  uint32_t  numberOfArrayElements;
  uint32_t  numberOfFaces; //Cube map faces are stored in the order: +X, -X, +Y, -Y, +Z, -Z.
  uint32_t  numberOfMipmapLevels;
  uint32_t  bytesOfKeyValueData;
};

/**
 * Convert KTX format to Dali::Pixel::Format
 */
bool ConvertPixelFormat( const uint32_t ktxPixelFormat, Dali::Pixel::Format& format )
{
  switch( ktxPixelFormat )
  {
    case 0x93B0: // GL_COMPRESSED_RGBA_ASTC_4x4
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_4x4_KHR;
      break;
    }
    case 0x93B1: // GL_COMPRESSED_RGBA_ASTC_5x4
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_5x4_KHR;
      break;
    }
    case 0x93B2: // GL_COMPRESSED_RGBA_ASTC_5x5
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_5x5_KHR;
      break;
    }
    case 0x93B3: // GL_COMPRESSED_RGBA_ASTC_6x5
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_6x5_KHR;
      break;
    }
    case 0x93B4: // GL_COMPRESSED_RGBA_ASTC_6x6
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_6x6_KHR;
      break;
    }
    case 0x93B5: // GL_COMPRESSED_RGBA_ASTC_8x5
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_8x5_KHR;
      break;
    }
    case 0x93B6: // GL_COMPRESSED_RGBA_ASTC_8x6
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_8x6_KHR;
      break;
    }
    case 0x93B7: // GL_COMPRESSED_RGBA_ASTC_8x8
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_8x8_KHR;
      break;
    }
    case 0x93B8: // GL_COMPRESSED_RGBA_ASTC_10x5
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_10x5_KHR;
      break;
    }
    case 0x93B9: // GL_COMPRESSED_RGBA_ASTC_10x6
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_10x6_KHR;
      break;
    }
    case 0x93BA: // GL_COMPRESSED_RGBA_ASTC_10x8
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_10x8_KHR;
      break;
    }
    case 0x93BB: // GL_COMPRESSED_RGBA_ASTC_10x10
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_10x10_KHR;
      break;
    }
    case 0x93BC: // GL_COMPRESSED_RGBA_ASTC_12x10
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_12x10_KHR;
      break;
    }
    case 0x93BD: // GL_COMPRESSED_RGBA_ASTC_12x12
    {
      format = Dali::Pixel::COMPRESSED_RGBA_ASTC_12x12_KHR;
      break;
    }
    case 0x881B: // GL_RGB16F
    {
      format = Dali::Pixel::RGB16F;
      break;
    }
    case 0x8815: // GL_RGB32F
    {
      format = Dali::Pixel::RGB32F;
      break;
    }
    case 0x8C3A: // GL_R11F_G11F_B10F
    {
      format = Dali::Pixel::RGB32F;
      break;
    }
    case 0x8D7C: // GL_RGBA8UI
    {
      format = Dali::Pixel::RGBA8888;
      break;
    }
    case 0x8D7D: // GL_RGB8UI
    {
      format = Dali::Pixel::RGB888;
      break;
    }
    default:
    {
      return false;
    }
  }

  return true;
}

bool LoadCubeMapFromKtxFile( const std::string& path, CubeData& cubedata )
{
  FILE* fp = fopen(path.c_str(),"rb");

  if( NULL == fp )
  {
    return false;
  }

  KtxFileHeader header;

  int result = fread(&header,1,sizeof(KtxFileHeader),fp);
  if( 0 == result )
  {
    return false;
  }

  long lSize = 0;

  // Skip the key-values:
  const long int imageSizeOffset = sizeof(KtxFileHeader) + header.bytesOfKeyValueData;

  fseek(fp, imageSizeOffset, SEEK_END);
  lSize = ftell(fp);
  rewind(fp);

  if(fseek(fp, imageSizeOffset, SEEK_SET))
  {
    return false;
  }
  cubedata.img.resize(header.numberOfFaces);

  for(unsigned int face=0; face < header.numberOfFaces; ++face) //array_element must be 0 or 1
  {
    cubedata.img[face].resize(header.numberOfMipmapLevels);
  }

  unsigned char* buffer= reinterpret_cast<unsigned char*>( malloc( lSize ) );

  unsigned char* img[6];
  unsigned int imgSize[6];
  unsigned char* imgPointer = buffer;
  result = fread(buffer,1,lSize,fp);
  if( 0 == result )
  {
    return false;
  }

  fclose(fp);

  if( 0 == header.numberOfMipmapLevels )
  {
    header.numberOfMipmapLevels = 1u;
  }

  if( 0 == header.numberOfArrayElements )
  {
    header.numberOfArrayElements = 1u;
  }

  if( 0 == header.pixelDepth )
  {
    header.pixelDepth = 1u;
  }

  if( 0 == header.pixelHeight )
  {
    header.pixelHeight = 1u;
  }

  Dali::Pixel::Format daliformat = Pixel::RGB888;

  ConvertPixelFormat(header.glInternalFormat, daliformat);

  for( unsigned int mipmapLevel = 0; mipmapLevel < header.numberOfMipmapLevels; ++mipmapLevel )
  {
    long int byteSize = 0;
    int imageSize;
    memcpy(&imageSize,imgPointer,sizeof(unsigned int));
    imgPointer += 4u;
    for(unsigned int arrayElement=0; arrayElement < header.numberOfArrayElements; ++arrayElement) //arrayElement must be 0 or 1
    {
      for(unsigned int face=0; face < header.numberOfFaces; ++face)
      {
        byteSize = imageSize;
        if(byteSize % 4u)
        {
          byteSize += 4u - byteSize % 4u;
        }
        img[face] = reinterpret_cast<unsigned char*>( malloc( byteSize ) ); // resources will be freed when the PixelData is destroyed.
        memcpy(img[face],imgPointer,byteSize);
        imgSize[face] = byteSize;
        imgPointer += byteSize;
        cubedata.img[face][mipmapLevel] = PixelData::New( img[face], imgSize[face], header.pixelWidth , header.pixelHeight , daliformat, PixelData::FREE );
      }
    }
    header.pixelHeight/=2u;
    header.pixelWidth/=2u;
  }

  free(buffer);
  return true;
}

} // namespace SceneLauncher
