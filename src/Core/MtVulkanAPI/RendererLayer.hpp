//
// Created by MTesseracT on 30-10-2017.
//

#ifndef VULKANENGINE_RENDERERLAYER_HPP
#define VULKANENGINE_RENDERERLAYER_HPP


#include <Core/Camera.hpp>
#include <glm/glm.hpp>
#include <Core/GameWorld.hpp>
#include "VulkanRendererBase.hpp"

class RendererLayer
{
    VulkanRendererBase *m_rendererBase;

    void RendererLayer::Render (GameWorld* p_world)
    {
        Camera* camera = p_world->GetMainCamera();
        Render (p_world, p_world->GetTransform(), glm::inverse(camera->getWorldTransform()), camera->getProjection(), true);
    }

    void RendererLayer::Render(GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive) {
        renderSelf(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix);
        if (pRecursive) renderChildren(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix, pRecursive);
    }

    void RendererLayer::RenderSelf (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
        renderMesh(pGameObject->getMesh(), pGameObject->getMaterial(), pModelMatrix, pViewMatrix, pProjectionMatrix);
        //renderMeshDebugInfo(pGameObject->getMesh(), pModelMatrix, pViewMatrix, pProjectionMatrix);
    }

    void Renderer::RenderMesh (Mesh* pMesh, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
        if (pMesh != NULL && pMaterial!= NULL) pMaterial->render(pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);
    }
};


#endif //VULKANENGINE_RENDERERLAYER_HPP
