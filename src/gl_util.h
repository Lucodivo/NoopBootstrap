#pragma once

u32 glSizeInBytes(u8 sizeInBytes) {
  switch(sizeInBytes) {
    case 1:
      return GL_UNSIGNED_BYTE;
    case 2:
      return GL_UNSIGNED_SHORT;
    case 4:
      return GL_UNSIGNED_INT;
    default:
      assert(false);
      return 0;
  }
}