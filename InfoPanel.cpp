#include "InfoPanel.hpp"

int c_info_panel::pitch = 250;
int c_info_panel::yaw = 250;

c_info_panel g_info_panel;

template < class T >
void render_info(int x, int y, const char* description, T value)
{
	gDrawManager.DrawString(x, y, Color(255, 255, 255, 255), description, value);
}

void c_info_panel::paint_traverse()
{
}