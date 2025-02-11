glslangValidator -V VertexShader.vert -o vert.spv
glslangValidator -V UIVertexShader.vert -o UIvert.spv
glslangValidator -V UIFontVertexShader.vert -o UIFontvert.spv
glslangValidator -V UIFontVertexShader.vert -o UIFontvert.spv


glslangValidator -V --target-env vulkan1.3 -S task particleTask.task -o particleTask.spv
glslangValidator -V --target-env vulkan1.3 -S task water.task -o waterTask.spv

glslangValidator -V --target-env vulkan1.3 -S mesh particleMeshShader.mesh -o particleMesh.spv
glslangValidator -V --target-env vulkan1.3 -S mesh water.mesh -o waterMesh.spv



glslangValidator -V FragmentShader.frag -o frag.spv
glslangValidator -V UIFragmentShader.frag -o UIfrag.spv
glslangValidator -V UIFontFragmentShader.frag -o UIFontfrag.spv
glslangValidator -V UIFontFragmentShader.frag -o UIFontfrag.spv
glslangValidator -V --target-env vulkan1.3  ParticleFragmentShader.frag -o particleFrag.spv
glslangValidator -V --target-env vulkan1.3  water.frag -o waterFrag.spv


pause