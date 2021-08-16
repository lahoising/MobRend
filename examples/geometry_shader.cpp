#include <cstdint>
#include <iostream>
#include <mr_application.h>
#include "mr_asset_manager.h"
#include "mr_input.h"
#include "mr_input_event.h"
#include "mr_renderer.h"
#include "mr_shader.h"
#include "mr_vertex_layout.h"
#include "mr_window.h"
#include "mr_mesh.h"

class Sample : public mr::Program
{
public:
	virtual void OnStart() override
	{
		mr::Shader::CreateParams shaderParams = {};
		mr::AssetManager::GetAssetPath(shaderParams.vertFilePath, "resources/shaders/geometry_example.vert.spv");
		mr::AssetManager::GetAssetPath(shaderParams.fragFilePath, "resources/shaders/geometry_example.frag.spv");
		mr::AssetManager::GetAssetPath(shaderParams.geomFilePath, "resources/shaders/geometry_example.geom.spv");
		this->shader = mr::Shader::Create(shaderParams);

		mr::VertexLayout layout = {
			{ mr::ATTRIBUTE_TYPE_FLOAT, 2 }
		};

		float points[] = {
			-0.5f,  0.5f,
			 0.5f,  0.5f,
			 0.5f, -0.5f,
			-0.5f, -0.5f
		};

		std::uint32_t indices[] = {
			0, 1, 2, 3
		};

		mr::Mesh::CreateParams meshParams = {};
		meshParams.vertexLayout = &layout;
		meshParams.vertices = points;
		meshParams.verticesArraySize = sizeof(points);
		meshParams.indices = indices;
		meshParams.indexCount = sizeof(indices) / sizeof(std::uint32_t);

		this->mesh = new mr::Mesh(meshParams);
	}

	virtual void OnUpdate() override
	{
		mr::Window *win = mr::Application::GetInstance().GetMainWindow();
		mr::Input &input = win->input;

		if(input.KeyJustPressed(mr::KEY_ESC))
		{
	 		mr::Application::GetInstance().Close();
		}
	}

	virtual void OnRender(mr::Renderer *renderer) override
	{
		this->shader->Bind();

		mr::Renderer::Command cmd = {};
		cmd.mesh = this->mesh;
		cmd.renderObjectType = mr::RENDER_OBJECT_MESH;
		cmd.topologyType = mr::TOPOLOGY_POINTS;
		renderer->Render(cmd);
	}

	virtual void OnGuiRender() override
	{}

	virtual void OnDestroy() override
	{
		delete(this->mesh);
		delete(this->shader);
	}

private:
	mr::Mesh *mesh;
	mr::Shader *shader;
};

int main(int argc, char *argv[])
{
	printf("hey\n");

	Sample sample = Sample();
	mr::Window::CreateParams windowParams = {};
	windowParams.width = 800;
	windowParams.height = 600;
	windowParams.windowName = "Geometry Shader Sample";

	mr::Application::RunParams runParams = {};
	runParams.program = &sample;
	runParams.windowCreateParams = windowParams;

	mr::Application::GetInstance().Run(runParams);

	return EXIT_SUCCESS;
}
