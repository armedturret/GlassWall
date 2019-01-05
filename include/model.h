#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <vertex.h>

namespace GW {
	class World;
	namespace RenderEngine {
		class ShaderProgram;

		class Model {
		public:
			Model();
			~Model();

			//loads a model from a file to be used when rendered from a .dae
			void loadFromFile(std::string fileName);

			//what shader the model should use
			void useShader(const ShaderProgram& shader);


			//clean up internals
			void cleanUp();

			//set world rules
			void setWorld(GW::World* world);
		private:
			//deals with drawing the files during rendering
			void draw();

			//stores all vertex data
			std::vector<Vertex> m_vertices;

			//shader id
			GLuint m_program;

			//vbo id
			GLuint m_vbo;

			//refrence to world
			GW::World* m_world;

			friend class GW::World;
		};
	}
}