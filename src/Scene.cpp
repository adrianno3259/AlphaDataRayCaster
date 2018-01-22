#include "../include/Scene.h"

Scene::Scene(){}

Scene::~Scene(){}

const std::vector<Mesh*>& Scene::getMeshes() const
{
    return this->objects;
}

void Scene::addObject(Mesh* obj)
{
    this->objects.push_back(obj);
}
