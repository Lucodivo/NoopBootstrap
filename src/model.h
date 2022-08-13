#pragma once

void initializeModelVertexData(tinygltf::Model* gltfModel, Model* model)
{
  struct gltfAttributeMetadata {
    u32 accessorIndex;
    u32 numComponents;
    u32 bufferViewIndex;
    u32 bufferIndex;
    u64 bufferByteOffset;
    u64 bufferByteLength;
  };

  const char* positionIndexKeyString = "POSITION";
  const char* normalIndexKeyString = "NORMAL";
  const char* texture0IndexKeyString = "TEXCOORD_0";

  model->meshCount = (u32)gltfModel->meshes.size();
  assert(model->meshCount != 0);
  model->meshes = new Mesh[model->meshCount];
  std::vector<tinygltf::Accessor>* gltfAccessors = &gltfModel->accessors;
  std::vector<tinygltf::BufferView>* gltfBufferViews = &gltfModel->bufferViews;

  auto populateAttributeMetadata = [gltfAccessors, gltfBufferViews](const char* keyString, const tinygltf::Primitive& gltfPrimitive) -> gltfAttributeMetadata {
    gltfAttributeMetadata result;
    result.accessorIndex = gltfPrimitive.attributes.at(keyString);
    result.numComponents = tinygltf::GetNumComponentsInType(gltfAccessors->at(result.accessorIndex).type);
    result.bufferViewIndex = gltfAccessors->at(result.accessorIndex).bufferView;
    result.bufferIndex = gltfBufferViews->at(result.bufferViewIndex).buffer;
    result.bufferByteOffset = gltfBufferViews->at(result.bufferViewIndex).byteOffset;
    result.bufferByteLength = gltfBufferViews->at(result.bufferViewIndex).byteLength;
    return result;
  };

  for(u32 i = 0; i < model->meshCount; ++i) {
    Mesh* mesh = &model->meshes[i];

    tinygltf::Mesh gltfMesh = gltfModel->meshes[i];
    assert(!gltfMesh.primitives.empty());
    // TODO: handle meshes that have more than one primitive
    tinygltf::Primitive gltfPrimitive = gltfMesh.primitives[0];
    assert(gltfPrimitive.indices > -1); // TODO: Should we deal with models that don't have indices?

    // TODO: Allow variability in attributes beyond POSITION, NORMAL, TEXCOORD_0?
    assert(gltfPrimitive.attributes.find(positionIndexKeyString) != gltfPrimitive.attributes.end());
    gltfAttributeMetadata positionAttribute = populateAttributeMetadata(positionIndexKeyString, gltfPrimitive);
    f64* minValues = gltfModel->accessors[positionAttribute.accessorIndex].minValues.data();
    f64* maxValues = gltfModel->accessors[positionAttribute.accessorIndex].maxValues.data();
    model->boundingBox.min = {(f32)minValues[0], (f32)minValues[1], (f32)minValues[2]};
    model->boundingBox.diagonal = glm::vec3{(f32)maxValues[0], (f32)maxValues[1], (f32)maxValues[2]} - model->boundingBox.min;

    b32 normalAttributesAvailable = gltfPrimitive.attributes.find(normalIndexKeyString) != gltfPrimitive.attributes.end();
    gltfAttributeMetadata normalAttribute{};
    if(normalAttributesAvailable) { // normal attribute data
      normalAttribute = populateAttributeMetadata(normalIndexKeyString, gltfPrimitive);
      assert(positionAttribute.bufferIndex == normalAttribute.bufferIndex);
    }

    b32 texture0AttributesAvailable = gltfPrimitive.attributes.find(texture0IndexKeyString) != gltfPrimitive.attributes.end();
    gltfAttributeMetadata texture0Attribute{};
    if(texture0AttributesAvailable) { // texture 0 uv coord attribute data
      texture0Attribute = populateAttributeMetadata(texture0IndexKeyString, gltfPrimitive);
      assert(positionAttribute.bufferIndex == texture0Attribute.bufferIndex);
    }

    // TODO: Handle vertex attributes that don't share the same buffer?
    u32 vertexAttBufferIndex = positionAttribute.bufferIndex;
    assert(gltfModel->buffers.size() > vertexAttBufferIndex);

    u32 indicesAccessorIndex = gltfPrimitive.indices;
    tinygltf::BufferView indicesGLTFBufferView = gltfBufferViews->at(gltfAccessors->at(indicesAccessorIndex).bufferView);
    u32 indicesGLTFBufferIndex = indicesGLTFBufferView.buffer;
    u64 indicesGLTFBufferByteOffset = indicesGLTFBufferView.byteOffset;
    u64 indicesGLTFBufferByteLength = indicesGLTFBufferView.byteLength;

    u64 minOffset = Min(positionAttribute.bufferByteOffset, Min(texture0Attribute.bufferByteOffset, normalAttribute.bufferByteOffset));
    u8* vertexAttributeDataOffset = gltfModel->buffers[indicesGLTFBufferIndex].data.data() + minOffset;
    u8* indicesDataOffset = gltfModel->buffers[indicesGLTFBufferIndex].data.data() + indicesGLTFBufferByteOffset;

    mesh->vertexAtt.indexCount = u32(gltfAccessors->at(indicesAccessorIndex).count);
    mesh->vertexAtt.indexTypeSizeInBytes = tinygltf::GetComponentSizeInBytes(gltfAccessors->at(indicesAccessorIndex).componentType);
    // TODO: Handle the possibility of the three attributes not being side-by-side in the buffer
    u64 sizeOfAttributeData = positionAttribute.bufferByteLength + normalAttribute.bufferByteLength + texture0Attribute.bufferByteLength;
    assert(gltfModel->buffers[vertexAttBufferIndex].data.size() >= sizeOfAttributeData);
    const u32 positionAttributeIndex = 0;
    const u32 normalAttributeIndex = 1;
    const u32 texture0AttributeIndex = 2;

    glGenVertexArrays(1, &mesh->vertexAtt.arrayObject);
    glGenBuffers(1, &mesh->vertexAtt.bufferObject);
    glGenBuffers(1, &mesh->vertexAtt.indexObject);

    glBindVertexArray(mesh->vertexAtt.arrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexAtt.bufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeOfAttributeData,
                 vertexAttributeDataOffset,
                 GL_STATIC_DRAW);

    // set the vertex attributes (position and texture)
    // position attribute
    glVertexAttribPointer(positionAttributeIndex,
                          positionAttribute.numComponents, // attribute size
                          GL_FLOAT, // type of data
                          GL_FALSE, // should data be normalized
                          positionAttribute.numComponents * sizeof(f32),// stride
                          (void*)(positionAttribute.bufferByteOffset - minOffset)); // offset of first component
    glEnableVertexAttribArray(positionAttributeIndex);

    // normal attribute
    if(normalAttributesAvailable) {
      glVertexAttribPointer(normalAttributeIndex,
                            normalAttribute.numComponents, // attribute size
                            GL_FLOAT,
                            GL_FALSE,
                            normalAttribute.numComponents * sizeof(f32),
                            (void*)(normalAttribute.bufferByteOffset - minOffset));
      glEnableVertexAttribArray(normalAttributeIndex);
    }

    // texture 0 UV Coord attribute
    if(texture0AttributesAvailable) {
      glVertexAttribPointer(texture0AttributeIndex,
                            texture0Attribute.numComponents, // attribute size
                            GL_FLOAT,
                            GL_FALSE,
                            texture0Attribute.numComponents * sizeof(f32),
                            (void*)(texture0Attribute.bufferByteOffset - minOffset));
      glEnableVertexAttribArray(texture0AttributeIndex);
    }

    // bind element buffer object to give indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vertexAtt.indexObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesGLTFBufferByteLength, indicesDataOffset, GL_STATIC_DRAW);

    // unbind VBO & VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    s32 gltfMaterialIndex = gltfPrimitive.material;
    if(gltfMaterialIndex >= 0) {
      tinygltf::Material gltfMaterial = gltfModel->materials[gltfMaterialIndex];
      // TODO: Handle more then just TEXCOORD_0 vertex attribute?
      assert(gltfMaterial.normalTexture.texCoord == 0 && gltfMaterial.pbrMetallicRoughness.baseColorTexture.texCoord == 0);

      f64* baseColor = gltfMaterial.pbrMetallicRoughness.baseColorFactor.data();
      mesh->baseColor = {(f32)baseColor[0], (f32)baseColor[1], (f32)baseColor[2], (f32)baseColor[3] };

      // NOTE: gltf.textures.samplers gives info about how to magnify/minify textures and how texture wrapping should work
      // TODO: Don't load the same texture multiple times if multiple meshes use the same texture
      s32 normalTextureIndex = gltfMaterial.normalTexture.index;
      if(normalTextureIndex >= 0) {
        u32 normalImageIndex = gltfModel->textures[normalTextureIndex].source;
        tinygltf::Image normalImage = gltfModel->images[normalImageIndex];
        load2DTexture(normalImage.image.data(), normalImage.component, normalImage.width, normalImage.height, &mesh->normalTextureId);
      } else {
        mesh->normalTextureId = TEXTURE_ID_NO_TEXTURE;
      }

      s32 baseColorTextureIndex = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
      if(baseColorTextureIndex >= 0) {
        u32 albedoColorImageIndex = gltfModel->textures[baseColorTextureIndex].source;
        tinygltf::Image albedoImage = gltfModel->images[albedoColorImageIndex];
        load2DTexture(albedoImage.image.data(), albedoImage.component, albedoImage.width, albedoImage.height, &mesh->albedoTextureId);
      } else {
        mesh->albedoTextureId = TEXTURE_ID_NO_TEXTURE;
      }
    } else {
      mesh->normalTextureId = TEXTURE_ID_NO_TEXTURE;
      mesh->albedoTextureId = TEXTURE_ID_NO_TEXTURE;
      mesh->baseColor = {};
    }
  }
}

void loadModel(const char* filePath, Model* returnModel) {
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;
  tinygltf::Model tinyGLTFModel;

  //bool ret = loader.LoadASCIIFromFile(&tinyGLTFModel, &err, &warn, filePath); // for .gltf
  bool ret = loader.LoadBinaryFromFile(&tinyGLTFModel, &err, &warn, filePath); // for binary glTF(.glb)

  if (!warn.empty()) {
    printf("Warning: %s\n", warn.c_str());
    return;
  }

  if (!err.empty()) {
    printf("Error: %s\n", err.c_str());
    return;
  }

  if (!ret) {
    printf("Failed to parse glTF\n");
    return;
  }

  returnModel->fileName = filePath;
  initializeModelVertexData(&tinyGLTFModel, returnModel);
}

void drawModel(const Model& model) {
  for(u32 i = 0; i < model.meshCount; ++i) {
    Mesh* meshPtr = model.meshes + i;
    drawTriangles(meshPtr->vertexAtt);
  }
}

void deleteModels(Model* models, u32 count = 1) {
  std::vector<VertexAtt> vertexAtts;
  std::vector<GLuint> textureData;

  for(u32 i = 0; i < count; ++i) {
    Model* modelPtr = models + i;
    for(u32 i = 0; i < modelPtr->meshCount; ++i) {
      Mesh* meshPtr = modelPtr->meshes;
      vertexAtts.push_back(meshPtr->vertexAtt);
      if(meshPtr->normalTextureId != TEXTURE_ID_NO_TEXTURE) {
        textureData.push_back(meshPtr->normalTextureId);
      }
      if(meshPtr->albedoTextureId != TEXTURE_ID_NO_TEXTURE) {
        textureData.push_back(meshPtr->albedoTextureId);
      }
    }
    delete[] modelPtr->meshes;
    *modelPtr = {}; // clear model to zero
  }

  deleteVertexAtts(vertexAtts.data(), (u32)vertexAtts.size());
  glDeleteTextures((GLsizei)textureData.size(), textureData.data());
}