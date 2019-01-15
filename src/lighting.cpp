#include <lighting.h>

GW::RenderEngine::Lighting::Lighting()
{
}

GW::RenderEngine::Lighting::~Lighting()
{
}

void GW::RenderEngine::Lighting::setDirectionalLight(const DirectionalLight & light)
{
	m_directionalLight = light;
}

void GW::RenderEngine::Lighting::addPointLight(const PointLight & light)
{
	m_pointLights.push_back(light);
}

void GW::RenderEngine::Lighting::setPointLight(unsigned int index, const PointLight & light)
{
	if (index < m_pointLights.size()) {
		m_pointLights[index] = light;
	}
}

std::vector<GW::RenderEngine::PointLight> GW::RenderEngine::Lighting::getPointLights()
{
	return m_pointLights;
}

GW::RenderEngine::DirectionalLight GW::RenderEngine::Lighting::getDirectionalLight()
{
	return m_directionalLight;
}
