#ifndef _SESSION_H_
#define _SESSION_H_ 

#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "material/Material.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "Image.h"

namespace application
{
    struct Session
     {
        bool useFPGA;
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<material::Material>> materials;
        std::vector<Mesh> meshes;
        std::vector<Light> lights;
        std::vector<Image> frames;
        std::string outputName;
     
        enum
        {
            INIT=0,
            CAMERA,
            LIGHT,
            LIGHT_POINT,
            MATERIAL,
            MATERIAL_MATTE,
            MESH
        };

        void readDarkFile(const std::string& darkfile)
        {

            std::ifstream file(darkfile);
            std::string line, input;
            int state, type;

            while(file >> input)
            {
                std::cout << input << "\n";
                if(input == "camera")
                {
                    state = CAMERA;
                }
                else if(input == "material")
                {
                    state = MATERIAL;
                }   
                else if(input == "light")
                {
                    std::cout << "LOG: LIGHT" << std::endl;
                    state = LIGHT;
                }
                else if(input == "mesh")
                {
                    state = MESH;
                }
                
                switch(state)
                {
                    case CAMERA:
                        this->_readCamera(file);
                        break;
                    
                    case MATERIAL:
                        file >> input;
                        if(input == "matte")
                            type = MATERIAL_MATTE;
                        this->_readMaterial(file, type);
                        break;

                    case LIGHT:
                        file >> input;
                        if(input == "point")
                            type = LIGHT_POINT;
                        this->_readLight(file, type);
                        break;

                    case MESH:
                        this->_readMesh(file);
                        break;
                    
                    default:
                        break;
                }

            }
        }

        void setCamera(
            Vec3d eye, 
            Vec3d lkp, 
            Vec3d upv,
            double dist,
            double psize,
            int vres, 
            int hres)
        {

            this->frames.emplace_back(hres, vres);

            this->camera = 
                std::make_shared<Camera>(
                    eye, 
                    lkp, 
                    upv,
                    dist,
                    psize,
                    vres, 
                    hres
            );
        }

        void addMesh(std::string meshFilename)
        {
            this->meshes.emplace_back(meshFilename);
        }

    private:

        void _readMesh(std::ifstream& file)
        {
            std::string meshFilename;
            file >> meshFilename;
            std::cout << meshFilename << "\n";

            std::ifstream f(meshFilename);
            if(!f.good())
            {
                std::cout 
                    << "Error"
                    << "Couldn't read mesh file..." 
                    << std::endl;
                return;
            }
            
            this->addMesh(meshFilename);
        }

        inline void _readLight(std::ifstream& file, int type)
        {
            if(type == LIGHT_POINT)
            {
                double intensity;
                double x, y, z;
                file >> x >> y >> z;
                Vec3d pos(x, y, z);
                file >> intensity;
                file >> x >> y >> z;
                Color col(x, y, z);
            
                this->lights.emplace_back(
                    intensity,
                    col,
                    pos
                );
            }
        }

        void _readMaterial(std::ifstream& file, int type)
        {
            if(type == MATERIAL_MATTE)
            {
                double diffuseCoefficient;
                double r, g, b;
                file >> diffuseCoefficient;
                file >> r >> g >> b;
                Color col(r, g, b);
                
                this->materials.push_back(
                    std::make_shared<material::Matte>(
                        diffuseCoefficient,
                        col
                    )
                );

            }
        }

        void _readCamera(std::ifstream& file)
        {
            int hres, vres;
            double dist, psize, x, y, z;
            Vec3d eye, lkp, upv;

            file >> hres >> vres >> psize;
            file >> x >> y >> z;
            eye = Vec3d(x, y, z);
            file >> x >> y >> z;
            lkp = Vec3d(x, y, z);
            file >> x >> y >> z;
            upv = Vec3d(x, y, z);
            file >> dist;

            this->setCamera(
                eye, 
                lkp, 
                upv,
                dist,
                psize,
                vres, 
                hres
            );
        }

     };
}
#endif