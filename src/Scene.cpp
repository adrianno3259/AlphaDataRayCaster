#include "../include/Scene.h"

Scene::Scene(){}

Scene::~Scene(){}

std::shared_ptr<Mesh> Scene::getMesh(int index) const
{
    return this->objects[index];
}

int Scene::sceneSize() const
{
    return this->objects.size();
}

const std::vector<std::shared_ptr<Mesh> >& Scene::getMeshes() const
{
    return this->objects;
}

void Scene::addObject(std::shared_ptr<Mesh> obj)
{
    this->objects.push_back(obj);
}
