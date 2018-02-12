#include "../include/Mesh.h"
#include <fstream>


Mesh::Mesh(){}


Mesh::Mesh(std::string filename) : id(Mesh::MESH_NEXT_ID++)
{
    /// object file stream
    std::fstream sourceFile;
    /// opening file in read-mode
    sourceFile.open(filename.c_str(), std::ios::in);
    /// line to be read
    char line[200];
    int v_count = 0, vt_count = 0, vn_count = 0, triCounter = 0;
    std::vector<Vec3d> v;

    /// While not at the end of file
    while(!sourceFile.eof()){
        sourceFile.getline(line, 200);
        if(line[0] == 'v')
        {
            if(line[1] == ' ') /// Reading a vertex
            {
                double vx, vy, vz;
                sscanf(line+1, "%lf %lf %lf\n", &vx, &vy, &vz);
                v.push_back(Vec3d(vx,vy,vz));
                v_count++;
            }
            else if(line[1] == 'n') /// Support for reading normals from obj file
            {

            }
            else if(line[1] == 't') /// Support for reading uv-texture mapping
            {

            }

        }
        else if(line[0] == 'f') /// reading mesh faces
        {
                int a, b, c, e, f, g, h, i;
                //sscanf(line+1, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &a,&g,&h,&b,&e,&f,&c,&h,&i);
                sscanf(line+1, "%d %d %d\n", &a,&b,&c);
                this->triangles.push_back(std::make_shared<Triangle>(v[a-1],
                                                                     v[b-1],
                                                                     v[c-1],
                                                                     triCounter));
                triCounter++;
        }
    }
    printf("triangle number: %d\n", triCounter);
    sourceFile.close();
}

void Mesh::addTriangle(std::shared_ptr<Triangle> tri)
{
    this->triangles.push_back(tri);
}

int Mesh::getId() const
{
    return id;
}

Mesh::~Mesh()
{
    //dtor
}
