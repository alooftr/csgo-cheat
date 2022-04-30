#include "menu.hpp"

#define CHILD_FIRST_POS 100 + 12, 8
#define CHILD_SECOND_POS 100 + 12 * 2 + 302, 8 //(CHILD_POS_X + (CHILD_SIZE_SPACE * 2) + (CHILD_SIZE_SIZE_W + 2)), CHILD_POS_HEIGHT
#define CHILD_SIZE 300, 385

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
    { item_definition_indexes::WEAPON_AK47, "ak-47" },
    { item_definition_indexes::WEAPON_AUG, "aug" },
    { item_definition_indexes::WEAPON_AWP, "awp" },
    { item_definition_indexes::WEAPON_CZ75A, "cz75 auto" },
    { item_definition_indexes::WEAPON_DEAGLE, "desert eagle" },
    { item_definition_indexes::WEAPON_ELITE, "dual berettas" },
    { item_definition_indexes::WEAPON_FAMAS, "famas" },
    { item_definition_indexes::WEAPON_FIVESEVEN, "five-seveN" },
    { item_definition_indexes::WEAPON_G3SG1, "g3sg1" },
    { item_definition_indexes::WEAPON_GALILAR, "galil ar" },
    { item_definition_indexes::WEAPON_GLOCK, "glock-18" },
    { item_definition_indexes::WEAPON_M249, "m249" },
    { item_definition_indexes::WEAPON_M4A1_SILENCER, "m4a1-s" },
    { item_definition_indexes::WEAPON_M4A1, "m4a4" },
    { item_definition_indexes::WEAPON_MAC10, "mac-10" },
    { item_definition_indexes::WEAPON_MAG7, "mag-7" },
    { item_definition_indexes::WEAPON_MP7, "mp7" },
    { item_definition_indexes::WEAPON_MP5SD, "mp5" },
    { item_definition_indexes::WEAPON_MP9, "mp9" },
    { item_definition_indexes::WEAPON_NEGEV, "negev" },
    { item_definition_indexes::WEAPON_NOVA, "nova" },
    { item_definition_indexes::WEAPON_HKP2000, "p2000" },
    { item_definition_indexes::WEAPON_P250, "p250" },
    { item_definition_indexes::WEAPON_P90, "p90" },
    { item_definition_indexes::WEAPON_BIZON, "pp-bizon" },
    { item_definition_indexes::WEAPON_REVOLVER, "r8 revolver" },
    { item_definition_indexes::WEAPON_SAWEDOFF, "sawed-off" },
    { item_definition_indexes::WEAPON_SCAR20, "scar-20" },
    { item_definition_indexes::WEAPON_SSG08, "ssg 08" },
    { item_definition_indexes::WEAPON_SG556, "sg 553" },
    { item_definition_indexes::WEAPON_TEC9, "tec-9" },
    { item_definition_indexes::WEAPON_UMP45, "ump-45" },
    { item_definition_indexes::WEAPON_USP_SILENCER, "usp-s" },
    { item_definition_indexes::WEAPON_XM1014, "xm1014" },
};

void menu::initialize() {
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hooks::window);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    

    menu::settings::menu_font_12 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
        roboto_medium_compressed_data,
        roboto_medium_compressed_size,
        12.f);

    menu::settings::menu_font_14 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
        roboto_medium_compressed_data,
        roboto_medium_compressed_size,
        14.f);

    menu::settings::menu_font_16 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
        roboto_medium_compressed_data,
        roboto_medium_compressed_size,
        16.f);
}

void menu::widgets::current_weapon_button(float width) {
    if (!interfaces::engine->is_connected() || !csgo::local_player || !csgo::local_player->is_alive()) {
        ImGui::Button(("current"), ImVec2(width, 0.0f));
        return;
    }

    auto weapon = csgo::local_player->active_weapon();
    if (!weapon || !weapon->is_weapon()) {
        ImGui::Button(("current"), ImVec2(width, 0.0f));
        return;
    }

    if (ImGui::Button(("current"), ImVec2(width, 0.0f))) {
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
    if (ImGui::Begin("csgo-cheat", &menu::settings::open, flags)) {
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
        P2 = ImVec2(menu::settings::width, 4.f);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pForegroundDrawList;
        pDrawList->AddRectFilledMultiColor(P1, P2, ImColor(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], 255.f),
            ImColor(variables::menu_clr[2], variables::menu_clr[0], variables::menu_clr[1], 255.f),
            ImColor(variables::menu_clr[2], variables::menu_clr[0], variables::menu_clr[1], 255.f),
            ImColor(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], 255.f));
        //Main Tab
        ImGui::PushFont(menu::settings::menu_font_14);
        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f));
        if (ImGui::Tab("Rage", "", { 85.000f, 0.000f }, current_tab == 0 ? true : false)) {
            current_tab = 0;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f));
        if (ImGui::Tab("Legit", "", { 85.000f, 0.000f }, current_tab == 1 ? true : false)) {
            current_tab = 1;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 2));
        if (ImGui::Tab("Anti-Aim", "", { 85.000f, 0.000f }, current_tab == 2 ? true : false)) {
            current_tab = 2;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 3));
        if (ImGui::Tab("Visuals", "", { 85.000f, 0.000f }, current_tab == 3 ? true : false)) {
            current_tab = 3;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 4));
        if (ImGui::Tab("Misc", "", { 85.000f, 0.000f }, current_tab == 4 ? true : false)) {
            current_tab = 4;
            update_animation_alpha();
        }

        ImGui::SetCursorPos(ImVec2(8.000f, 20.000f + 25.f * 5));
        if (ImGui::Tab("Settings", "", { 85.000f, 0.000f }, current_tab == 5 ? true : false)) {
            current_tab = 5;
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
            tabs::misc();
        }
        else if (current_tab == 5) {
            tabs::settings();
        }

        ImGui::End();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}

void menu::tabs::rage() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild("Child 1", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild("Child 2", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}

void menu::tabs::anti_aim() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild("Child 1", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild("Child 2", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}
void menu::tabs::legit() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild("Child 1", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild("Child 2", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}
void menu::tabs::visuals() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild("Child 1", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild("Child 2", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}
void menu::tabs::misc() {
    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild("Movement", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::Checkbox("Auto Bhop##abhop", &variables::movement::bunny_hop);
    ImGui::Checkbox("Edge jump#ejump", &variables::movement::edge_jump);
    if (variables::movement::edge_jump)
        ImGui::HotKey(&variables::movement::edge_jump_key, ImVec2(195.f, 20));

    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild("Miscellaneous", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::PopFont();
    ImGui::EndChild();
}

void menu::tabs::settings() {
    static const char* configs[] = { "1", "2", "3", "4", "5" };
    static const char* choices[]{ "  yes", "  no" };
    static int current_config = 0;

    ImGui::SetCursorPos({ CHILD_FIRST_POS });
    ImGui::BeginChild("Config", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::ColorEdit3("Menu Color", variables::menu_clr);
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Combo(("##config"), &current_config, configs, IM_ARRAYSIZE(configs));
    if (ImGui::Button(("save"))) {
        ImGui::OpenPopup(("config save popup"));
    }

    if (ImGui::BeginPopup(("config save popup"))) {
        ImGui::Text(("are you sure you want to save selected config? "));

        for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
            if (ImGui::Selectable(choices[i]))
                if (i == 0) {
                    config::save(configs[current_config]);
                }

        ImGui::Spacing();

        ImGui::EndPopup();
    }

    if (ImGui::Button(("load"))) {
        ImGui::OpenPopup(("config load popup"));
    }

    if (ImGui::BeginPopup(("config load popup"))) {
        ImGui::Text(("are you sure you want to load selected config? "));

        for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
            if (ImGui::Selectable(choices[i]))
                if (i == 0) {
                    config::load(configs[current_config]);
                }

        ImGui::Spacing();

        ImGui::EndPopup();
    }

    if (ImGui::Button(("open configuration folder")))
        ShellExecuteW(0, L"open", L"C:/csgo_cheat/configs", NULL, NULL, SW_NORMAL);

    ImGui::PopFont();
    ImGui::EndChild();
    //2nd Child
    ImGui::SetCursorPos({ CHILD_SECOND_POS });
    ImGui::BeginChild("Info", { CHILD_SIZE }, true);
    ImGui::PushFont(menu::settings::menu_font_12);

    ImGui::Text(("compilation date: " __DATE__ " - " __TIME__));
    ImGui::Text(std::strstr(GetCommandLineA(), "-insecure") ? ("insecure parameter found, VAC disabled!") : ("[WARNING] insecure parameter not found make sure you injected with VAC bypass!"));

    ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text("FPS:%.1f", ImGui::GetIO().Framerate);

    ImGui::PopFont();
    ImGui::EndChild();
}