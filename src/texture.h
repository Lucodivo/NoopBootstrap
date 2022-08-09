#pragma once

void load2DTexture(const char* imgLocation, GLuint* textureId, u32* width, u32* height, bool flipImageVert = false, bool inputSRGB = false)
{
  glGenTextures(1, textureId);
  glBindTexture(GL_TEXTURE_2D, *textureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // lerp when magnifying
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // "pixelated" when magnifying
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // load image data
  s32 w, h, numChannels;
  stbi_set_flip_vertically_on_load(flipImageVert);
  u8* data = stbi_load(imgLocation, &w, &h, &numChannels, 0 /*desired channels*/);

  assert(data);
  assert(numChannels > 0 && numChannels <= 4);

  u32 dataColorSpace;
  u32 dataComponentComposition;
  switch(numChannels) {
    case 1:
      dataColorSpace = dataComponentComposition = GL_RED;
      break;
    case 2:
      dataColorSpace = dataComponentComposition = GL_RG;
      break;
    case 3:
      dataColorSpace = inputSRGB ? GL_SRGB : GL_RGB;
      dataComponentComposition = GL_RGB;
      break;
    case 4:
      dataColorSpace = inputSRGB ? GL_SRGB_ALPHA : GL_RGBA;
      dataComponentComposition = GL_RGBA;
      break;
  }

  glTexImage2D(GL_TEXTURE_2D, // target
               0, // level of detail (level n is the nth mipmap reduction image)
               dataColorSpace, // What is the color space of the data
               w, // width of texture
               h, // height of texture
               0, // border (legacy stuff, MUST BE 0)
               dataComponentComposition, // How are the components of the data composed
               GL_UNSIGNED_BYTE, // specifies data type of pixel data
               data); // pointer to the image data
  glGenerateMipmap(GL_TEXTURE_2D);

  if (width != nullptr) *width = w;
  if (height != nullptr) *height = h;

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data); // free texture image memory
}

internal inline void bindActiveTexture(s32 activeIndex, GLuint textureId, GLenum target) {
  glActiveTexture(GL_TEXTURE0 + activeIndex);
  glBindTexture(target, textureId);
}

void inline bindActiveTexture2d(s32 activeIndex, GLuint textureId) {
  bindActiveTexture(activeIndex, textureId, GL_TEXTURE_2D);
}