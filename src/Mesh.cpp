#include "../include/Mesh.h"
#include <fstream>


Mesh::Mesh(){}


Mesh::Mesh(std::string filename){

    // object file stream
    std::fstream sourceFile;
    // opening file in read-mode
    sourceFile.open(filename.c_str(), std::ios::in);
    char line[200]; // line to be read
    int v_count = 0, vt_count = 0, vn_count = 0;
    std::vector<Vec3d> v;

    // While not at the end of file
    while(!sourceFile.eof()){
        sourceFile.getline(line, 200);
        if(line[0] == 'v'){
            if(line[1] == ' '){ /// Reading a vertex
                double vx, vy, vz;
                sscanf(line+1, "%lf %lf %lf\n", &vx, &vy, &vz);
                v.push_back(Vec3d(vx,vy,vz));
                v_count++;
            } else if(line[1] == 'n'){ /// Support for reading normals from obj file
                //sscanf(line+2, "%f %f %f\n", &vn[vn_count].x, &vn[vn_count].y, &vn[vn_count].z);
                //if(vn_count % 5000 == 0) PV3(vn[vn_count]);
                //vn_count++;
            }else if(line[1] == 't'){ /// Support for reading uv-texture mapping
                //sscanf(line+2, "%lf %lf\n", &vt[vt_count].x, &vt[vt_count].y);
                //if(vt_count % 5000 == 0) PV2(vt[vt_count]);
                //vt_count++;
            }

        }else if(line[0] == 'f'){ /// reading mesh faces

                int a, b, c, e, f, g, h, i;
                //sscanf(line+1, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &a,&g,&h,&b,&e,&f,&c,&h,&i);
                sscanf(line+1, "%d %d %d\n", &a,&b,&c);
                this->triangles.push_back(std::make_shared<Triangle>(v[a-1], v[b-1], v[c-1]));

        }
    }

    //printf("Counts:\nv: %d, vt: %d, vn: %d\n", v_count, vt_count, vn_count);
    printf("triangle number: %d\n", triangles.size());
    sourceFile.close();
}

void Mesh::addTriangle(std::shared_ptr<Triangle> tri)
{
    this->triangles.push_back(tri);
}

Mesh::~Mesh()
{
    //dtor
}
