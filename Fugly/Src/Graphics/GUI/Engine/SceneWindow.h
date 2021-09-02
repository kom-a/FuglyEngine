#pragma once

#include <imgui/imgui.h>

#include "../Graphics/Window.h"
#include "../Graphics/Texture.h"

namespace Fugly
{
	namespace GUI
	{
		class SceneWindow
		{
		public:
			SceneWindow(Window* window);
			~SceneWindow();

			void Update();

			inline unsigned int GetTextureID() const { return m_Texture.GetID(); }

		private:
			Window* m_Window;
			Texture m_Texture;
		};
	}
}