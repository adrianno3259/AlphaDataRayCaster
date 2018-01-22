#include "../include/Scene.h"

Scene::Scene(){}

Scene::~Scene(){}

Mesh* Scene::getMesh(int index) const
{
    return this->objects[index];
}

int Scene::sceneSize() const
{
    return this->objects.size();
}

const std::vector<Mesh*>& Scene::getMeshes() const
{
    return this->objects;
}

void Scene::addObject(Mesh* obj)
{
    this->objects.push_back(obj);
}
