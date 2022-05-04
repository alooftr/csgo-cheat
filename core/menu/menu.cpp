#include "menu.hpp"

#define CHILD_FIRST_POS 100 + 12, 15
#define CHILD_SECOND_POS 100 + 12 * 2 + 302, 15 //(CHILD_POS_X + (CHILD_SIZE_SPACE * 2) + (CHILD_SIZE_SIZE_W + 2)), CHILD_POS_HEIGHT
#define CHILD_SIZE 300, 375

#define ColorEditFl ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip


void update_animation_alpha() {
    auto& style = ImGui::GetStyle();
    menu::settings::alpha = 0.f;
    menu::settings::alpha = std::clamp(menu::settings::alpha + menu::settings::animation_frequency * ImGui::GetIO().DeltaTime, 0.f, 1.f);
    style.Alpha = menu::settings::alpha;
};

auto tool_tip = [&](const std::string& string) {
    if (ImGui::IsItemHovered())
        menu::widgets::tool_tips.emplace_back(string);
};

std::vector<menu::settings::weapon_name_t> weapon_names = {
    { item_definition_indexes::WEAPON_AK47, xor("ak-47") },
    { item_definition_indexes::WEAPON_AUG, xor("aug") },
    { item_definition_indexes::WEAPON_AWP, xor("awp") },
    { item_definition_indexes::WEAPON_CZ75A, xor("cz75 auto") },
    { item_definition_indexes::WEAPON_DEAGLE, xor("desert eagle") },
    { item_definition_indexes::WEAPON_ELITE, xor("dual berettas") },
    { item_definition_indexes::WEAPON_FAMAS, xor("famas") },
    { item_definition_indexes::WEAPON_FIVESEVEN, xor("five-seveN") },
    { item_definition_indexes::WEAPON_G3SG1, xor("g3sg1") },
    { item_definition_indexes::WEAPON_GALILAR, xor("galil ar") },
    { item_definition_indexes::WEAPON_GLOCK, xor("glock-18") },
    { item_definition_indexes::WEAPON_M249, xor("m249") },
    { item_definition_indexes::WEAPON_M4A1_SILENCER, xor("m4a1-s") },
    { item_definition_indexes::WEAPON_M4A1, xor("m4a4") },
    { item_definition_indexes::WEAPON_MAC10, xor("mac-10") },
    { item_definition_indexes::WEAPON_MAG7, xor("mag-7") },
    { item_definition_indexes::WEAPON_MP7, xor("mp7") },
    { item_definition_indexes::WEAPON_MP5SD, xor("mp5") },
    { item_definition_indexes::WEAPON_MP9, xor("mp9") },
    { item_definition_indexes::WEAPON_NEGEV, xor("negev") },
    { item_definition_indexes::WEAPON_NOVA, xor("nova") },
    { item_definition_indexes::WEAPON_HKP2000, xor("p2000") },
    { item_definition_indexes::WEAPON_P250, xor("p250") },
    { item_definition_indexes::WEAPON_P90, xor("p90") },
    { item_definition_indexes::WEAPON_BIZON, xor("pp-bizon") },
    { item_definition_indexes::WEAPON_REVOLVER, xor("r8 revolver") },
    { item_definition_indexes::WEAPON_SAWEDOFF, xor("sawed-off") },
    { item_definition_indexes::WEAPON_SCAR20, xor("scar-20") },
    { item_definition_indexes::WEAPON_SSG08, xor("ssg 08") },
    { item_definition_indexes::WEAPON_SG556, xor("sg 553") },
    { item_definition_indexes::WEAPON_TEC9, xor("tec-9") },
    { item_definition_indexes::WEAPON_UMP45, xor("ump-45") },
    { item_definition_indexes::WEAPON_USP_SILENCER, xor("usp-s") },
    { item_definition_indexes::WEAPON_XM1014, xor("xm1014") },
};

void menu::initialize() {
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hooks::window);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    

    menu::settings::menu_font_12 = io.Fonts->AddFontFromMemoryCompressedTTF(
        roboto_medium_compressed_data,
        roboto_medium_compressed_size,
        12.f);

    menu::settings::menu_font_14 = io.Fonts->AddFontFromMemoryCompressedTTF(
        roboto_medium_compressed_data,
        roboto_medium_compressed_size,
        14.f);

    menu::settings::menu_font_16 = io.Fonts->AddFontFromMemoryCompressedTTF(
        roboto_medium_compressed_data,
        roboto_medium_compressed_size,
        16.f);
}

void menu::widgets::current_weapon_button(float width) {
    if (!interfaces::engine->is_connected() || !csgo::local_player || !csgo::local_player->is_alive()) {
        ImGui::Button(xor("current"), ImVec2(width, 0.0f));
        return;
    }

    auto weapon = csgo::local_player->active_weapon();
    if (!weapon || !weapon->is_weapon()) {
        ImGui::Button(xor("current"), ImVec2(width, 0.0f));
        return;
    }

    if (ImGui::Button(xor("current"), ImVec2(width, 0.0f))) {
        int wpn_idx = weapon->item_definition_index();
        auto wpn_it = std::find_if(weapon_names.begin(), weapon_names.end(), [wpn_idx](const menu::settings::weapon_name_t& a) {
            return a.definition_index == wpn_idx;
            });
        if (wpn_it != weapon_names.end()) {
            menu::settings::weapon_index = wpn_idx;
            menu::settings::weapon_vector_index = std::abs(std::distance(weapon_names.begin(), wpn_it));
        }
    }
}

void menu::render() {
    if (!menu::settings::open)
        return;
    //Styling
    ImGui::StyleColorsClassic();

    auto& style = ImGui::GetStyle();
    auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    style.ScrollbarSize = 9.0f;
    style.WindowRounding = 0.f;
    style.Colors[ImGuiCol_WindowBg] = ImColor(20.f / 255.f, 20.f / 255.f, 20.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_Border] = ImColor(96.f / 255.f, 96.f / 255.f, 96.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(64.f / 255.f, 64.f / 255.f, 64.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(variables::menu_clr[0] + (20.f / 255.f), variables::menu_clr[1] + (20.f / 255.f), variables::menu_clr[2] + (20.f / 255.f), style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    style.Colors[ImGuiCol_SliderGrab] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(64.f / 255.f, 64.f / 255.f, 64.f / 255, style.Alpha);

    style.Colors[ImGuiCol_Button] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(variables::menu_clr[0] + (20.f / 255.f), variables::menu_clr[1] + (20.f / 255.f), variables::menu_clr[2] + (20.f / 255.f), style.Alpha);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    style.Colors[ImGuiCol_Header] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(variables::menu_clr[0] + (20.f / 255.f), variables::menu_clr[1] + (20.f / 255.f), variables::menu_clr[2] + (20.f / 255.f), style.Alpha);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(96.f / 255.f, 96.f / 255.f, 96.f / 255, style.Alpha);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(64.f / 255.f, 64.f / 255.f, 64.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(64.f / 255.f, 64.f / 255.f, 64.f / 255.f, style.Alpha);

    style.Colors[ImGuiCol_ChildBg] = ImVec4(32.f / 255.f, 32.f / 255.f, 32.f / 255.f, style.Alpha);

    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    //Menu begin
    ImGui::SetNextWindowSize(ImVec2(menu::settings::width, menu::settings::height), ImGuiCond_Always);
    if (ImGui::Begin(xor("csgo-cheat"), &menu::settings::open, flags)) {
        menu::settings::alpha = std::clamp(menu::settings::alpha + menu::settings::animation_frequency * ImGui::GetIO().DeltaTime, 0.f, 1.f);
        style.Alpha = menu::settings::alpha;

        ImVec2 P1, P2;
        ImDrawList* pDrawList;
        const auto& CurrentWindowPos = ImGui::GetWindowPos();
        const auto& pWindowDrawList = ImGui::GetWindowDrawList();
        const auto& pBackgroundDrawList = ImGui::GetBackgroundDrawList();
        const auto& pForegroundDrawList = ImGui::GetForegroundDrawList();

        //Background 
        P1 = ImVec2(0.000f, 0.000f);
        P1.x += CurrentWindowPos.x;
        P1.y += CurrentWindowPos.y;
        P2 = ImVec2(menu::settings::width, menu::settings::height);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pWindowDrawList;
        pDrawList->AddRectFilled(P1, P2, ImColor(0.125f, 0.125f, 0.125f, 1.000f), 0.000f);
        //Background Outline
        P1 = ImVec2(0.000f, 0.000f);
        P1.x += CurrentWindowPos.x;
        P1.y += CurrentWindowPos.y;
        P2 = ImVec2(menu::settings::width, menu::settings::height);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pForegroundDrawList;
        pDrawList->AddRect(P1, P2, ImColor(0.25f, 0.25f, 0.25f, 1.000f), 0.000f);
        //Main tab separator
        P1 = ImVec2(100.000f, 0.000f);
        P1.x += CurrentWindowPos.x;
        P1.y += CurrentWindowPos.y;
        P2 = ImVec2(100.000f, 400.000f);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pForegroundDrawList;
        pDrawList->AddRect(P1, P2, ImColor(0.25f, 0.25f, 0.25f, 1.000f), 0.000f);
        //Cool line
        P1 = ImVec2(0.000f, 0.000f);
        P1.x += CurrentWindowPos.x;
        P1.y += CurrentWindowPos.y;
        P2 = ImVec2(menu::settings::width, 2.f);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pForegroundDrawList;
        pDrawList->AddRectFilledMultiColor(P1, P2, 
            ImColor(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], 255.f),
            ImColor(variables::menu_clr[2], variables::menu_clr[0], variables::menu_clr[1], 255.f),
            ImColor(variables::menu_clr[2], variables::menu_clr[0], variables::menu_clr[1], 255.f),
            ImColor(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], 255.f));
        //Main Tab
        ImGui::PushFont(menu::settings::menu_font_14);
        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f));
        if (ImGui::Tab(xor("Rage"), { 85.000f, 0.000f }, current_tab == 0 ? true : false)) {
            current_tab = 0;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f));
        if (ImGui::Tab(xor("Legit"), { 85.000f, 0.000f }, current_tab == 1 ? true : false)) {
            current_tab = 1;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 2));
        if (ImGui::Tab(xor("Anti-Aim"), { 85.000f, 0.000f }, current_tab == 2 ? true : false)) {
            current_tab = 2;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 3));
        if (ImGui::Tab(xor("Visuals"), { 85.000f, 0.000f }, current_tab == 3 ? true : false)) {
            current_tab = 3;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 4));
        if (ImGui::Tab(xor("Skins"), { 85.000f, 0.000f }, current_tab == 4 ? true : false)) {
            current_tab = 4;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 5));
        if (ImGui::Tab(xor("Misc"), { 85.000f, 0.000f }, current_tab == 5 ? true : false)) {
            current_tab = 5;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 6));
        if (ImGui::Tab(xor ("Players"), { 85.000f, 0.000f }, current_tab == 6 ? true : false)) {
            current_tab = 6;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 7));
        if (ImGui::Tab(xor("Settings"), { 85.000f, 0.000f }, current_tab == 7 ? true : false)) {
            current_tab = 7;
            update_animation_alpha();
        }
        ImGui::PopFont();
        //Inside tab setup
        if (current_tab == 0) {
            tabs::rage();
        }
        else if (current_tab == 1) {
            tabs::legit();
        }
        else if (current_tab == 2) {
            tabs::anti_aim();
        }
        else if (current_tab == 3) {
            tabs::visuals();
        }
        else if (current_tab == 4) {
            tabs::skin();
        }
        else if (current_tab == 5) {
            tabs::misc();
        }
        else if (current_tab == 6) {
            tabs::player();
        }
        else if (current_tab == 7) {
            tabs::settings();
        }
        ImGui::End();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}

void menu::tabs::rage() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild(xor("Child 1"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild(xor("Child 2"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}

void menu::tabs::anti_aim() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild(xor("Child 1"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild(xor("Child 2"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}
void menu::tabs::legit() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild(xor("Child 1"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild(xor("Child 2"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}
void menu::tabs::visuals() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild(xor("Child 1"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::Checkbox("Molotov fire##mlfcb", &variables::visuals::world::molotov_fire);
    ImGui::SameLine();
    ImGui::ColorEdit4("##mlfire", variables::visuals::world::molotov_fire_clr, ColorEditFl); tool_tip("Alpha bar is for the molotov's smoke alpha");
    
    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild(xor("Child 2"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}

void menu::tabs::skin() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild(xor("Child 1"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild(xor("Child 2"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}

void menu::tabs::misc() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild(xor("Movement"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::Checkbox(xor("Auto Bhop##abhop"), &variables::movement::bunny_hop);
    ImGui::Checkbox(xor ("Null strafe"), &variables::movement::null_strafe);
    ImGui::Checkbox(xor ("Infinite crouch"), &variables::movement::inifnite_crouch);
    ImGui::Checkbox(xor("Edge jump##ejump"), &variables::movement::edge_jump);
    if (variables::movement::edge_jump)
    {
        ImGui::SameLine();
        ImGui::Hotkey(xor("##1"), &variables::movement::edge_jump_key, ImVec2(80.f, 0));
    }

    ImGui::Checkbox(xor("Edge bug ##ebug"), &variables::movement::edge_bug);
    if (variables::movement::edge_bug) {
        ImGui::SameLine();
        ImGui::Hotkey(xor("##2"), &variables::movement::edge_bug_key, ImVec2(80.f, 0));
    }

    ImGui::Checkbox(xor("Jump bug##jbug"), &variables::movement::jump_bug);
    if (variables::movement::jump_bug)
    {
        ImGui::SameLine();
        ImGui::Hotkey(xor("##3"), &variables::movement::jump_bug_key, ImVec2(80.f, 0));
    }
    
    ImGui::Checkbox(xor("Mini jump##mjump"), &variables::movement::mini_jump);
    if (variables::movement::mini_jump) {
        ImGui::SameLine();
        ImGui::Hotkey(xor("##4"), &variables::movement::mini_jump_key, ImVec2(80.f, 0.f));
    }

    ImGui::Checkbox(xor("Strafe Optimize##sopti"), &variables::movement::strafe_optimizer);
    if (variables::movement::strafe_optimizer) {
        ImGui::SliderInt(xor("Min speed"), &variables::movement::strafe_optimizer_min_speed, 200, 300, "%d");
        ImGui::SliderInt(xor("Desired gain"), &variables::movement::strafe_optimizer_desired_gain, 10, 50, "%d");
    }

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild(xor("Miscellaneous"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::Checkbox(xor ("Auto pistol"), &variables::misc::auto_pistol);

    ImGui::PopFont();
    ImGui::EndChild();
}

void menu::tabs::player() {
    static int current_player = 0;
    static bool is_selected;
    static std::vector <player_list_data> players;
    char player_name[64] = {};
    player_t* entity;
    player_info_t info;

    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild(xor ("Player"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    if (interfaces::engine->is_in_game()) {
        ImGui::Text("Soon");  
    }
    else {
        ImGui::Text("Empty"); //not in game 
    }

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild(xor ("Child 2"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}

void menu::tabs::settings() {
    static const char* configs[] = { "1", "2", "3", "4", "5"};
    static const char* choices[]{ "  yes", "  no" };
    static int current_config = 0;

    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild(xor("Config"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::SliderFloat(xor("Menu animation freqency"), &menu::settings::animation_frequency, 0.1f, 10.f, "%.2f");
    ImGui::Spacing();

    ImGui::ColorEdit3(xor("Menu Color"), variables::menu_clr);
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Combo((xor("##config")), &current_config, configs, IM_ARRAYSIZE(configs));
    if (ImGui::Button(xor("save"))) {
        ImGui::OpenPopup(xor("config save popup"));
    }

    if (ImGui::BeginPopup(xor("config save popup"))) {
        ImGui::Text(xor("are you sure you want to save selected config? "));

        for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
            if (ImGui::Selectable(choices[i]))
                if (i == 0) {
                    config::save(configs[current_config]);
                }

        ImGui::Spacing();

        ImGui::EndPopup();
    }

    if (ImGui::Button(xor("load"))) {
        ImGui::OpenPopup(xor("config load popup"));
    }

    if (ImGui::BeginPopup(xor("config load popup"))) {
        ImGui::Text(xor("are you sure you want to load selected config? "));

        for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
            if (ImGui::Selectable(choices[i]))
                if (i == 0) {
                    config::load(configs[current_config]);
                }

        ImGui::Spacing();

        ImGui::EndPopup();
    }

    if (ImGui::Button(xor("open configuration folder")))
        ShellExecuteW(0, L"open", L"C:/csgo_cheat/configs", NULL, NULL, SW_NORMAL);

    ImGui::PopFont();
    ImGui::EndChild();
    //2nd Child
    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild(xor("Info"), { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::Text(xor("compilation date: " __DATE__ " - " __TIME__));
    ImGui::Text(std::strstr(GetCommandLineA(), "-insecure") ? xor("insecure parameter found, VAC disabled!") : xor("[WARNING] insecure parameter not found make sure you injected with VAC bypass!"));

    ImGui::Text(xor("Application average %.3f ms/frame"), 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text(xor("FPS:%.1f"), ImGui::GetIO().Framerate);

    ImGui::PopFont();
    ImGui::EndChild();
}