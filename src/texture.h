#pragma once

enum LoadTextureFlags {
  HORZ_FLIP = 1 << 0,
  INPUT_SRGB = 1 << 1,
  CHUNKY_PIXELS = 1 << 2,
};

void load2DTexture(const u8* data, u32 numChannels, s32 width, s32 height, GLuint* textureId, b32 textureFlags = 0) {
  glGenTextures(1, textureId);
  glBindTexture(GL_TEXTURE_2D, *textureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  b32 chunkyPixels = flagIsSet(textureFlags, LoadTextureFlags::CHUNKY_PIXELS);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, chunkyPixels ? GL_NEAREST : GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, chunkyPixels ? GL_NEAREST : GL_LINEAR_MIPMAP_NEAREST);

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
      dataColorSpace = flagIsSet(textureFlags, LoadTextureFlags::INPUT_SRGB) ? GL_SRGB : GL_RGB;
      dataComponentComposition = GL_RGB;
      break;
    case 4:
      dataColorSpace = flagIsSet(textureFlags, LoadTextureFlags::INPUT_SRGB) ? GL_SRGB_ALPHA : GL_RGBA;
      dataComponentComposition = GL_RGBA;
      break;
  }

  glTexImage2D(GL_TEXTURE_2D, // target
               0, // level of detail (level n is the nth mipmap reduction image)
               dataColorSpace, // What is the color space of the data
               width, // width of texture
               height, // height of texture
               0, // border (legacy stuff, MUST BE 0)
               dataComponentComposition, // How are the components of the data composed
               GL_UNSIGNED_BYTE, // specifies data type of pixel data
               data); // pointer to the image data
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void load2DTexture(const char* imgLocation, GLuint* textureId, s32* width, s32* height, b32 textureFlags = 0) {
  // load image data
  s32 numChannels;
  stbi_set_flip_vertically_on_load(flagIsSet(textureFlags, LoadTextureFlags::HORZ_FLIP));
  u8* data = stbi_load(imgLocation, width, height, &numChannels, 0 /*desired channels*/);

  assert(data);
  assert(numChannels > 0 && numChannels <= 4);

  load2DTexture(data, numChannels, *width, *height, textureId, textureFlags);

  stbi_image_free(data); // free texture image memory
}

internal inline void bindActiveTexture(s32 activeIndex, GLuint textureId, GLenum target) {
  glActiveTexture(GL_TEXTURE0 + activeIndex);
  glBindTexture(target, textureId);
}

void inline bindActiveTexture2d(s32 activeIndex, GLuint textureId) {
  bindActiveTexture(activeIndex, textureId, GL_TEXTURE_2D);
}