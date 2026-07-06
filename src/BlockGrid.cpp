#include "BlockGrid.h"
#include <iostream>

BlockGrid::BlockGrid(){
    std::cout << "Making BlockGrid..." << std::endl;
   
    blocks_wide = 16;
    blocks_long = 16;
    texes_wide = 3;
    tex_u_width = 1.0f / texes_wide;
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
    for(int i = 0; i < blocks_wide * blocks_long; i++){
        blocks.push_back(0);
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
    std::cout << "BUILDING MESH..." << std::endl;
    Mesh mesh = {0};
    mesh.triangleCount = num_tris;
    mesh.vertexCount = mesh.triangleCount * 3;
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));
    mesh.normals = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));

    std::cout << num_tris << " " << mesh.vertexCount << std::endl;

    int v = 0;
    int t = 0;
    int n = 0;

    // iterate through blocks, build verts...
    for(int i = 0; i < blocks_long; i++){
        for(int j = 0; j < blocks_wide; j++){
            int b = blocks[i * blocks_wide + j];
            float x = j * 1.0f;
            float y = 0.0f;
            float z = i * 1.0f;
            if(b == 0){
                // floor
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UVs...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[1].x;
                mesh.vertices[v++] = y + cube_coords[1].y;
                mesh.vertices[v++] = z + cube_coords[1].z;
                // fix UVs...
                mesh.texcoords[t++] = 2.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UVs...
                mesh.texcoords[t++] = 2.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[3].x;
                mesh.vertices[v++] = y + cube_coords[3].y;
                mesh.vertices[v++] = z + cube_coords[3].z;
                // fix UVs...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UVs...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UVs...
                mesh.texcoords[t++] = 2.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                
                // ceiling
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UVs...
                mesh.texcoords[t++] = 2.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UVs...
                mesh.texcoords[t++] = 3.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[5].x;
                mesh.vertices[v++] = y + cube_coords[5].y;
                mesh.vertices[v++] = z + cube_coords[5].z;
                // fix UVs...
                mesh.texcoords[t++] = 3.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UVs...
                mesh.texcoords[t++] = 2.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[7].x;
                mesh.vertices[v++] = y + cube_coords[7].y;
                mesh.vertices[v++] = z + cube_coords[7].z;
                // fix UVs...
                mesh.texcoords[t++] = 2.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UVs...
                mesh.texcoords[t++] = 3.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                
            }
            if(b == 1){
                // south wall
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[1].x;
                mesh.vertices[v++] = y + cube_coords[1].y;
                mesh.vertices[v++] = z + cube_coords[1].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[5].x;
                mesh.vertices[v++] = y + cube_coords[5].y;
                mesh.vertices[v++] = z + cube_coords[5].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[5].x;
                mesh.vertices[v++] = y + cube_coords[5].y;
                mesh.vertices[v++] = z + cube_coords[5].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                // north wall
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[3].x;
                mesh.vertices[v++] = y + cube_coords[3].y;
                mesh.vertices[v++] = z + cube_coords[3].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[7].x;
                mesh.vertices[v++] = y + cube_coords[7].y;
                mesh.vertices[v++] = z + cube_coords[7].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[7].x;
                mesh.vertices[v++] = y + cube_coords[7].y;
                mesh.vertices[v++] = z + cube_coords[7].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                // west wall
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[3].x;
                mesh.vertices[v++] = y + cube_coords[3].y;
                mesh.vertices[v++] = z + cube_coords[3].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[0].x;
                mesh.vertices[v++] = y + cube_coords[0].y;
                mesh.vertices[v++] = z + cube_coords[0].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[3].x;
                mesh.vertices[v++] = y + cube_coords[3].y;
                mesh.vertices[v++] = z + cube_coords[3].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[4].x;
                mesh.vertices[v++] = y + cube_coords[4].y;
                mesh.vertices[v++] = z + cube_coords[4].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[7].x;
                mesh.vertices[v++] = y + cube_coords[7].y;
                mesh.vertices[v++] = z + cube_coords[7].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = -1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // east wall
                // tri 1
                // vert 1
                mesh.vertices[v++] = x + cube_coords[1].x;
                mesh.vertices[v++] = y + cube_coords[1].y;
                mesh.vertices[v++] = z + cube_coords[1].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[2].x;
                mesh.vertices[v++] = y + cube_coords[2].y;
                mesh.vertices[v++] = z + cube_coords[2].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // tri 2
                // vert 1
                mesh.vertices[v++] = x + cube_coords[1].x;
                mesh.vertices[v++] = y + cube_coords[1].y;
                mesh.vertices[v++] = z + cube_coords[1].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 1.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 2
                mesh.vertices[v++] = x + cube_coords[6].x;
                mesh.vertices[v++] = y + cube_coords[6].y;
                mesh.vertices[v++] = z + cube_coords[6].z;
                // fix UV coords...
                mesh.texcoords[t++] = 1.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 0.0f;
                // vert 3
                mesh.vertices[v++] = x + cube_coords[5].x;
                mesh.vertices[v++] = y + cube_coords[5].y;
                mesh.vertices[v++] = z + cube_coords[5].z;
                // fix UV coords...
                mesh.texcoords[t++] = 0.0f * tex_u_width;
                mesh.texcoords[t++] = 0.0f;
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