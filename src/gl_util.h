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

void drawTriangles(const VertexAtt& vertexAtt)
{
  glBindVertexArray(vertexAtt.arrayObject); // NOTE: Binding every time is unnecessary if the same vertexAtt is used for multiple calls in a row
  glDrawElements(GL_TRIANGLES,
                 vertexAtt.indexCount,
                 glSizeInBytes(vertexAtt.indexTypeSizeInBytes), // type of the indices
                 (void*)0); // offset in the EBO, could look like (void*)(indexOffset * vertexAtt.indexTypeSizeInBytes) for more complex implementations
}

void deleteVertexAtts(VertexAtt* vertexAtts, u32 count = 1)
{
  u32* deleteBufferObjects = new u32[count * 3];
  u32* deleteIndexBufferObjects = deleteBufferObjects + count;
  u32* deleteVertexArrays = deleteIndexBufferObjects + count;
  for(u32 i = 0; i < count; i++) {
    VertexAtt vertexAtt = vertexAtts[i];
    deleteBufferObjects[i] = vertexAtt.bufferObject;
    deleteIndexBufferObjects[i] = vertexAtt.indexObject;
    deleteVertexArrays[i] = vertexAtt.arrayObject;
  }

  glDeleteBuffers(count * 2, deleteBufferObjects);
  glDeleteVertexArrays(count, deleteVertexArrays);

  delete[] deleteBufferObjects;
}