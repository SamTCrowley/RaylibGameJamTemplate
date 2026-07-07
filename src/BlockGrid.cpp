#include "BlockGrid.h"
#include <iostream>

BlockGrid::BlockGrid(){
    std::cout << "Making BlockGrid..." << std::endl;
   
    blocks_wide = 16;
    blocks_long = 16;
    tile_size = 16.0f;
    atlas_size = 256.0f;
    tiles_wide = 16;
    inset = 0.5f / atlas_size;

    cube_coords = std::vector<Vector3>();
    cube_coords.push_back(Vector3{0.0f, 0.0f, 0.0f});
    cube_coords.push_back(Vector3{0.0f, 0.0f, 1.0f});
    cube_coords.push_back(Vector3{1.0f, 0.0f, 1.0f});
    cube_coords.push_back(Vector3{1.0f, 0.0f, 0.0f});
    cube_coords.push_back(Vector3{0.0f, 1.0f, 0.0f});
    cube_coords.push_back(Vector3{0.0f, 1.0f, 1.0f});
    cube_coords.push_back(Vector3{1.0f, 1.0f, 1.0f});
    cube_coords.push_back(Vector3{1.0f, 1.0f, 0.0f});

    blocks = std::vector<int>();
    block_texes = std::vector<TexList>();
    for(int i = 0; i < blocks_wide * blocks_long; i++){
        blocks.push_back(0);
        block_texes.push_back(TexList(1, 0, 2, 2, 2, 3));
    }
    for(int i = 0; i < blocks_long; i++){
        for(int j = 0; j < blocks_wide; j++){
            if(i == 0 || j == 0 || i == blocks_long - 1 || j == blocks_wide - 1){
                blocks[i * blocks_wide + j] = 1;
            }else{
                blocks[i * blocks_wide + j] = 0;
            }
        }
    }

    std::cout << "BlockGrid constructor done..." << std::endl;
}


Model BlockGrid::build_model(){
    // build mesh of blocks...
    // each block has size of 1.0f...
    // grid starts at XYZ 0, 0, 0...
    
    // count triangles...
    int num_tris = 0;
    for(int i : blocks){
        if(i == 0){ num_tris += 4; } // empty block has floor and ceiling
        if(i > 0){ num_tris += 8; } // full block has 4 walls
    }
    Mesh mesh = {0};
    mesh.triangleCount = num_tris;
    mesh.vertexCount = mesh.triangleCount * 3;
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));
    mesh.normals = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));

    int v = 0;
    int t = 0;
    int n = 0;

    // iterate through blocks, build verts...
    for(int i = 0; i < blocks_long; i++){
        for(int j = 0; j < blocks_wide; j++){
            int index = i * blocks_wide + j;
            int b = blocks[index];
            TexList tex = block_texes[index];
            float x = j * 1.0f;
            float y = 0.0f;
            float z = i * 1.0f;
            if(b == 0){
                // floor
                float tex_x = (float)(tex.B % tiles_wide);
                float tex_y = (float)(tex.B / tiles_wide);
                float u0 = ((tex_x + 0.0f) * tile_size) / atlas_size + inset;
                float u1 = ((tex_x + 1.0f) * tile_size) / atlas_size - inset;
                float v0 = ((tex_y + 1.0f) * tile_size) / atlas_size - inset;
                float v1 = ((tex_y + 0.0f) * tile_size) / atlas_size + inset;
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UVs...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[1].x;
                mesh.vertices[v++] = y + cube_coords[1].y;
                mesh.vertices[v++] = z + cube_coords[1].z;
                // fix UVs...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UVs...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[3].x;
                mesh.vertices[v++] = y + cube_coords[3].y;
                mesh.vertices[v++] = z + cube_coords[3].z;
                // fix UVs...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UVs...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UVs...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                
                // ceiling
                tex_x = (float)(tex.T % tiles_wide);
                tex_y = (float)(tex.T / tiles_wide);
                u0 = ((tex_x + 0.0f) * tile_size) / atlas_size + inset;
                u1 = ((tex_x + 1.0f) * tile_size) / atlas_size - inset;
                v0 = ((tex_y + 1.0f) * tile_size) / atlas_size - inset;
                v1 = ((tex_y + 0.0f) * tile_size) / atlas_size + inset;
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UVs...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UVs...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[5].x;
                mesh.vertices[v++] = y + cube_coords[5].y;
                mesh.vertices[v++] = z + cube_coords[5].z;
                // fix UVs...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UVs...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[7].x;
                mesh.vertices[v++] = y + cube_coords[7].y;
                mesh.vertices[v++] = z + cube_coords[7].z;
                // fix UVs...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UVs...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                
            }
            if(b == 1){
                // south wall
                float tex_x = (float)(tex.S % tiles_wide);
                float tex_y = (float)(tex.S / tiles_wide);
                float u0 = ((tex_x + 0.0f) * tile_size) / atlas_size + inset;
                float u1 = ((tex_x + 1.0f) * tile_size) / atlas_size - inset;
                float v0 = ((tex_y + 1.0f) * tile_size) / atlas_size - inset;
                float v1 = ((tex_y + 0.0f) * tile_size) / atlas_size + inset;
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[1].x;
                mesh.vertices[v++] = y + cube_coords[1].y;
                mesh.vertices[v++] = z + cube_coords[1].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[5].x;
                mesh.vertices[v++] = y + cube_coords[5].y;
                mesh.vertices[v++] = z + cube_coords[5].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[5].x;
                mesh.vertices[v++] = y + cube_coords[5].y;
                mesh.vertices[v++] = z + cube_coords[5].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // north wall
                tex_x = (float)(tex.N % tiles_wide);
                tex_y = (float)(tex.N / tiles_wide);
                u0 = ((tex_x + 0.0f) * tile_size) / atlas_size + inset;
                u1 = ((tex_x + 1.0f) * tile_size) / atlas_size - inset;
                v0 = ((tex_y + 1.0f) * tile_size) / atlas_size - inset;
                v1 = ((tex_y + 0.0f) * tile_size) / atlas_size + inset;
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[3].x;
                mesh.vertices[v++] = y + cube_coords[3].y;
                mesh.vertices[v++] = z + cube_coords[3].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[7].x;
                mesh.vertices[v++] = y + cube_coords[7].y;
                mesh.vertices[v++] = z + cube_coords[7].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[7].x;
                mesh.vertices[v++] = y + cube_coords[7].y;
                mesh.vertices[v++] = z + cube_coords[7].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // west wall
                tex_x = (float)(tex.W % tiles_wide);
                tex_y = (float)(tex.W / tiles_wide);
                u0 = ((tex_x + 0.0f) * tile_size) / atlas_size + inset;
                u1 = ((tex_x + 1.0f) * tile_size) / atlas_size - inset;
                v0 = ((tex_y + 1.0f) * tile_size) / atlas_size - inset;
                v1 = ((tex_y + 0.0f) * tile_size) / atlas_size + inset;
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[3].x;
                mesh.vertices[v++] = y + cube_coords[3].y;
                mesh.vertices[v++] = z + cube_coords[3].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[3].x;
                mesh.vertices[v++] = y + cube_coords[3].y;
                mesh.vertices[v++] = z + cube_coords[3].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[7].x;
                mesh.vertices[v++] = y + cube_coords[7].y;
                mesh.vertices[v++] = z + cube_coords[7].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // east wall
                tex_x = (float)(tex.E % tiles_wide);
                tex_y = (float)(tex.E / tiles_wide);
                u0 = ((tex_x + 0.0f) * tile_size) / atlas_size + inset;
                u1 = ((tex_x + 1.0f) * tile_size) / atlas_size - inset;
                v0 = ((tex_y + 1.0f) * tile_size) / atlas_size - inset;
                v1 = ((tex_y + 0.0f) * tile_size) / atlas_size + inset;
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[1].x;
                mesh.vertices[v++] = y + cube_coords[1].y;
                mesh.vertices[v++] = z + cube_coords[1].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[1].x;
                mesh.vertices[v++] = y + cube_coords[1].y;
                mesh.vertices[v++] = z + cube_coords[1].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v0;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UV coords...
                mesh.texcoords[t++] = u1;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[5].x;
                mesh.vertices[v++] = y + cube_coords[5].y;
                mesh.vertices[v++] = z + cube_coords[5].z;
                // fix UV coords...
                mesh.texcoords[t++] = u0;
                mesh.texcoords[t++] = v1;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
            }
        }
    }
    UploadMesh(&mesh, false);
    Model model = LoadModelFromMesh(mesh);
    return model;
    
}