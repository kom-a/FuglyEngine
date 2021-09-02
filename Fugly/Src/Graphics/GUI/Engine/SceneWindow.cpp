#include "SceneWindow.h"

namespace Fugly
{
	namespace GUI
	{

		SceneWindow::SceneWindow(Window* window)
			: m_Window(window),
			m_Texture(Texture(window->GetWidth(), window->GetHeight(), 0))
		{

		}

		SceneWindow::~SceneWindow()
		{

		}

		void SceneWindow::Update()
		{
			ImGui::Begin("Scene");
			m_Texture.Bind(0);
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddImage(
				(void*)m_Texture.GetID(), ImVec2(pos.x, pos.y + ImGui::GetWindowSize().y - 35),
				ImVec2(pos.x + ImGui::GetWindowSize().x - 15, pos.y));

			ImGui::End();
		}

	}
}