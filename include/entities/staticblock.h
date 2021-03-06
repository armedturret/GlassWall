#pragma once

#include <entity.h>

#include <glm/glm.hpp>
#include <shaderprogram.h>
#include <textures.h>
namespace GW {
	class StaticBlock : public GW::Entity {
	public:
		StaticBlock();

		virtual void entityStart();

		virtual void cleanUp();

		void setPosition(glm::vec3 pos);
		
		void setOrientation(glm::vec3 angles);

		void setDimensions(glm::vec3 dims);

		void setShader(const RenderEngine::ShaderProgram& program);

		void useMaterial(const GW::RenderEngine::Material& material);

	private:
		std::unique_ptr<GW::RenderEngine::Model> m_model;
	};
}