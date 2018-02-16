//
// Created by MTesseracT on 8-2-2018.
//

#ifndef VULKANENGINE_LIGHTSOURCE_HPP
#define VULKANENGINE_LIGHTSOURCE_HPP


#include <glm/vec3.hpp>
#include <Core/Math/Transform.hpp>

enum LightType {
    Point,
    Directional,
    Spot
};

class LightSource
{
protected:
    Transform m_transform;

    bool m_hasChanged = true;
public:
    virtual void UpdateTransformData() = 0;
    virtual void SetColor(glm::vec3 p_color) = 0;
    virtual void SetAttenuation(glm::vec3 p_attenuation) = 0;
    virtual void SetIntensity(float p_intensity) = 0;

    virtual const glm::vec3 GetColor() = 0;
    virtual const glm::vec3 GetAttenuation() = 0;
    virtual const float GetIntensity() = 0;

    virtual const void * GetData() = 0;
    virtual const uint32_t  GetDataSize() = 0;

    virtual const bool HasChanged() { return m_hasChanged; }
};

class PointLight : LightSource
{
    struct DataBlock
    {
        glm::vec3 m_color        = glm::vec3(1.0f);
        glm::vec3 m_position     = glm::vec3(0.0f);
        float     m_intensity    = 0.0f;
        glm::vec3 m_attentuation = glm::vec3(0.0f);
    } m_data;

public:
    void UpdateTransformData() override
    {
        m_hasChanged = true;
        m_data.m_position = m_transform.Position();
    }

    void SetColor(glm::vec3 p_color) override
    {
        m_hasChanged = true;
        m_data.m_color = p_color;
    }

    void SetAttenuation(glm::vec3 p_attenuation) override
    {
        m_hasChanged = true;
        m_data.m_attentuation = p_attenuation;
    }

    void SetIntensity(float p_intensity) override
    {
        m_hasChanged = true;
        m_data.m_intensity = p_intensity;
    }

    void SetPosition(glm::vec3 p_position)
    {
        m_transform.SetPosition(p_position);
        UpdateTransformData();
    }

    const glm::vec3 GetColor() override { return m_data.m_color; }

    const glm::vec3 GetAttenuation() override { return m_data.m_attentuation; }

    const float GetIntensity() override { return m_data.m_intensity; }

    const glm::vec3 GetPosition() { return m_transform.Position(); }

    const uint32_t GetDataSize() override { return sizeof(DataBlock); }

    const void* GetData() override
    {
        m_hasChanged = false;
        return &m_data;
    }
};

class DirectionalLight : LightSource
{
    struct DataBlock
    {
        glm::vec3 m_color        = glm::vec3(1.0f);
        glm::vec3 m_direction    = glm::vec3(0.0f, 0.0f, 1.0f);
        float     m_intensity    = 0.0f;
        glm::vec3 m_attentuation = glm::vec3(0.0f);
    } m_data;

    void UpdateTransformData() override
    {
        m_data.m_direction = m_transform.Forward();
    }

    void SetColor(glm::vec3 p_color) override
    {
        m_hasChanged = true;
        m_data.m_color = p_color;
    }

    void SetAttenuation(glm::vec3 p_attenuation) override
    {
        m_hasChanged = true;
        m_data.m_attentuation = p_attenuation;
    }

    void SetIntensity(float p_intensity) override
    {
        m_hasChanged = true;
        m_data.m_intensity = p_intensity;
    }

    void SetRotation(glm::quat p_rotation)
    {
        m_transform.SetRotation(p_rotation);
        UpdateTransformData();
    }

    const glm::vec3 GetColor() override { return m_data.m_color; }

    const glm::vec3 GetAttenuation() override { return m_data.m_attentuation; }

    const float GetIntensity() override { return m_data.m_intensity; }

    const glm::vec3 GetDirection() { return m_transform.Forward(); }

    const glm::quat GetRotation() { return m_transform.Rotation(); }

    const uint32_t GetDataSize() override { return sizeof(DataBlock); }

    const void* GetData() override
    {
        m_hasChanged = false;
        return &m_data;
    }
};

class SpotLight :  LightSource
{
    struct DataBlock
    {
        glm::vec3 m_color        = glm::vec3(1.0f);
        glm::vec3 m_position     = glm::vec3(0.0f);
        glm::vec3 m_direction    = glm::vec3(0.0f, 0.0f, 1.0f);
        float     m_intensity    = 0.0f;
        glm::vec3 m_attentuation = glm::vec3(0.0f);
    } m_data;

    void UpdateTransformData() override
    {
        m_data.m_position = m_transform.Position();
        m_data.m_direction = m_transform.Forward();
    }

    void SetColor(glm::vec3 p_color) override
    {
        m_hasChanged = true;
        m_data.m_color = p_color;
    }

    void SetAttenuation(glm::vec3 p_attenuation) override
    {
        m_hasChanged = true;
        m_data.m_attentuation = p_attenuation;
    }

    void SetIntensity(float p_intensity) override
    {
        m_hasChanged = true;
        m_data.m_intensity = p_intensity;
    }

    void SetRotation(glm::quat p_rotation)
    {
        m_transform.SetRotation(p_rotation);
        UpdateTransformData();
    }

    void SetPosition(glm::vec3 p_position)
    {
        m_transform.SetPosition(p_position);
        UpdateTransformData();
    }

    const glm::vec3 GetColor() override { return m_data.m_color; }

    const glm::vec3 GetAttenuation() override { return m_data.m_attentuation; }

    const float GetIntensity() override { return m_data.m_intensity; }

    const glm::vec3 GetDirection() { return m_transform.Forward(); }

    const glm::quat GetRotation() { return m_transform.Rotation(); }

    const glm::vec3 GetPosition() { return m_transform.Position(); }

    const uint32_t GetDataSize() override { return sizeof(DataBlock); }

    const void* GetData() override
    {
        m_hasChanged = false;
        return &m_data;
    }
};


#endif //VULKANENGINE_LIGHTSOURCE_HPP
