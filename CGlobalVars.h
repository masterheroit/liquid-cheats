#include "SDK.h"

enum CKeyCode_t {
	CKC_NONE,
	CKC_LEFT,
	CKC_RIGHT,
	CKC_SHIFT,
};

class CVars
{
public:
	///Authentication
	bool auth_failure;
	///---->

	///Aimbot;
	int aimbot_menu_visible;
	int aimbot_active;
	int aimbot_auto_fire;
	int aimbot_silent;
	int aimbot_dm_velocity_prediction;
	int aimbot_dm_attack_teammates;
	int aimbot_targeting_mode;
	int aimbot_max_fov;
	int aimbot_hitbox;
	CKeyCode_t aimbot_aim_key;
	///---->

	///Render
	int render_active;
	int render_menu_visible;
	int render_no_hands;
	int render_colored_hands;
	int render_wireframe_hands;
	int render_wireframe_explosive_barrels;
	int render_colored_hands_r;
	int render_colored_hands_g;
	int render_colored_hands_b;
	int render_colored_hands_a;
	int render_scope_border;
	///--->

	///Trigger;
	int trigger_menu_visible;
	int trigger_auto_barrel_reflection;
	///--->

	///Accuracy;
	int accuracy_menu_visible;
	int accuracy_backtrack;
	int accuracy_no_scope;
	int accuracy_no_zoom;
	int accuracy_no_angle_fix;
	///---->

	///Anti-Aims;
	int anti_aims_menu_visible;
	int anti_aims_active;
	int anti_aims_pitch_mode;
	int anti_aims_yaw_mode;
	int anti_aims_resolve_pitch;
	int anti_aims_resolve_yaw;
	///--->

	///Generals;
	int generals_menu_visible;
	int generals_active;
	int generals_bunnyhop;
	///--->

	///Thirdperson;
	int thirdperson_menu_visible;
	int thirdperson_menu_active;
	int thirdperson_menu_show_esp;
	int thirdperson_menu_show_fake_angles;
	int thirdperson_menu_show_real_angles;
	///--->

	///ESP
	int esp_menu_visible;
	int esp_active;
	int esp_players;
	int esp_draw_names;
	int esp_draw_outline;
	int esp_draw_corner_outline;
	int esp_draw_health_text;
	int esp_draw_health_bar;
	int esp_draw_eye_angles;
	int esp_explosives;
	int esp_local_player;
	int esp_draw_teammates;
	int esp_sight_lines;
	///---->

	///Glow
	int glow_active;
	///---->

	///Radar;
	int radar_menu_visible;
	int radar_active;
	int radar_background_opacity;
	int radar_size;
	int radar_range;
	///---->

	//Menu
	bool menu_active;
	bool unload;
	//---->
};

class CPlayerVariables
{
public:
	int iWeaponID;
	bool bHasMeleeWeapon;
	bool bHasFlameThrower;
	int iClass;
	int iWeaponSlot;
	int iPlayerCond;
	int iPlayerCondEx;
	int iFlags;
	char chName[34];
};

class COffsets
{
public:
	int iCreateMoveOffset = 21, iPaintTraverseOffset = 41;
};