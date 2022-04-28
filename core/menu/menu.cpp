#include "menu.hpp"

#define GROUPBOX_FIRST_POSITION 14, 100
#define GROUPBOX_SECOND_POSITION 338,100
#define GROUPBOX_SIZE 308, 400

auto update_animation_alpha = [&]() {
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
    
    menu::settings::tahoma = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
        Droid_compressed_data,
        Droid_compressed_size,
        14.f);
    menu::settings::tahoma2 = menu::settings::tahoma = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
        Droid_compressed_data,
        Droid_compressed_size,
        16.f);
    menu::settings::tahoma3 = menu::settings::tahoma = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
        Droid_compressed_data,
        Droid_compressed_size,
        12.f);
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
    //styling
    ImGui::StyleColorsClassic();

    auto& style = ImGui::GetStyle();
    auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    style.ScrollbarSize = 9.0f;
    style.WindowRounding = 0.f;
    style.Colors[ImGuiCol_WindowBg] = ImColor(20.f / 255.f, 20.f / 255.f, 20.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_Border] = ImColor(61.f / 255.f, 65.f / 255.f, 137.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(52.f / 255.f, 50.f / 255.f, 59.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(variables::menu_clr[0] + (20.f / 255.f), variables::menu_clr[1] + (20.f / 255.f), variables::menu_clr[2] + (20.f / 255.f), style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);


    style.Colors[ImGuiCol_SliderGrab] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(65.f / 255.f, 64.f / 255.f, 74.f / 255, style.Alpha);

    style.Colors[ImGuiCol_Button] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(variables::menu_clr[0] + (20.f / 255.f), variables::menu_clr[1] + (20.f / 255.f), variables::menu_clr[2] + (20.f / 255.f), style.Alpha);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    style.Colors[ImGuiCol_Header] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(variables::menu_clr[0] + (20.f / 255.f), variables::menu_clr[1] + (20.f / 255.f), variables::menu_clr[2] + (20.f / 255.f), style.Alpha);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(85.f / 255.f, 84.f / 255.f, 94.f / 255, style.Alpha);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(45.f / 255.f, 44.f / 255.f, 54.f / 255, style.Alpha);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(45.f / 255.f, 44.f / 255.f, 54.f / 255.f, style.Alpha);

    style.Colors[ImGuiCol_ChildBg] = ImVec4(64.f / 255.f, 64.f / 255.f, 64.f / 255.f, style.Alpha);

    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], style.Alpha);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

    ImGui::SetNextWindowSize(ImVec2(800.000f, 400.000f), ImGuiCond_Always);
    if (ImGui::Begin("csgo-cheat", &menu::settings::open, flags)) {
        menu::settings::alpha = std::clamp(menu::settings::alpha + menu::settings::animation_frequency * ImGui::GetIO().DeltaTime, 0.f, 1.f);
        style.Alpha = menu::settings::alpha;

        ImVec2 P1, P2;
        ImDrawList* pDrawList;
        const auto& CurrentWindowPos = ImGui::GetWindowPos();
        const auto& pWindowDrawList = ImGui::GetWindowDrawList();
        const auto& pBackgroundDrawList = ImGui::GetBackgroundDrawList();
        const auto& pForegroundDrawList = ImGui::GetForegroundDrawList();

        P1 = ImVec2(0.000f, 0.000f);
        P1.x += CurrentWindowPos.x;
        P1.y += CurrentWindowPos.y;
        P2 = ImVec2(800.000f, 400.000f);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pWindowDrawList;
        pDrawList->AddRectFilled(P1, P2, ImColor(0.125f, 0.125f, 0.125f, 1.000f), 0.000f);

        P1 = ImVec2(0.000f, 0.000f);
        P1.x += CurrentWindowPos.x;
        P1.y += CurrentWindowPos.y;
        P2 = ImVec2(800.000f, 400.000f);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pForegroundDrawList;
        pDrawList->AddRect(P1, P2, ImColor(0.251f, 0.251f, 0.251f, 1.000f), 0.000f);

        P1 = ImVec2(0.000f, 55.000f);
        P1.x += CurrentWindowPos.x;
        P1.y += CurrentWindowPos.y;
        P2 = ImVec2(800.000f, 85.000f);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pForegroundDrawList;
        pDrawList->AddRect(P1, P2, ImColor(0.251f, 0.251f, 0.251f, 1.000f), 0.000f);

        P1 = ImVec2(250.f, 0.f);
        P1.x += CurrentWindowPos.x;
        P1.y += CurrentWindowPos.y;
        P2 = ImVec2(800.000f, 50.000f);
        P2.x += CurrentWindowPos.x;
        P2.y += CurrentWindowPos.y;
        pDrawList = pWindowDrawList;
        pDrawList->AddRectFilledMultiColor(P1, P2,
            ImColor(variables::menu_clr[2], variables::menu_clr[0], variables::menu_clr[1], 1.f),    //upper left
            ImColor(variables::menu_clr[0], variables::menu_clr[1], variables::menu_clr[2], 1.f),    //upper riggt
            ImColor(0.000f, 0.000f, 0.000f, 0.f),    //bottom left
            ImColor(0.000f, 0.000f, 0.000f, 0.f));   //bottom right

        ImGui::SetCursorPos(ImVec2(260.000f, 20.000f));
        if (ImGui::Tab("Rage", "", { 100.000f, 0.000f }, current_tab == 1 ? true : false)) {
            current_tab = 1;
            update_animation_alpha();
        }

        ImGui::SameLine();
        if (ImGui::Tab("Anti-Aim", "", { 100.000f, 0.000f }, current_tab == 2 ? true : false)) {
            current_tab = 2;
            update_animation_alpha();
        }

        ImGui::SameLine();
        if (ImGui::Tab("Legit", "", { 100.000f, 0.000f }, current_tab == 3 ? true : false)) {
            current_tab = 3;
            update_animation_alpha();
        }

        ImGui::SameLine();
        if (ImGui::Tab("Visuals", "", { 100.000f, 0.000f }, current_tab == 4 ? true : false)) {
            current_tab = 4;
            update_animation_alpha();
        }


        ImGui::SameLine();
        if (ImGui::Tab("Misc", "", { 100.000f, 0.000f }, current_tab == 5 ? true : false)) {
            current_tab = 5;
            update_animation_alpha();
        }

        if (current_tab == 1) {

        }
        else if (current_tab == 2) {

        }
        else if (current_tab == 3) {

        }
        else if (current_tab == 4) {

        }
        else if (current_tab == 5) {
            tabs::misc();
        }
        ImGui::End();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}

void menu::tabs::misc() {
    static int tab = 0;

    ImGui::SetCursorPos({ 0 ,50 });
    if (ImGui::SubTab("General", "", { 192, 37 }, tab == 0 ? true : false)) {
        tab = 0;
        update_animation_alpha();
    }

    ImGui::SetCursorPos({ 192,50 });
    if (ImGui::SubTab("Config", "", { 192, 37 }, tab == 1 ? true : false)) {
        tab = 1;
        update_animation_alpha();
    }

    ImGui::SetCursorPos({ 192 * 2,50 });
    if (ImGui::SubTab("Others", "", { 192, 37 }, tab == 2 ? true : false)) {
        tab = 2;
        update_animation_alpha();
    }

    ImGui::SetCursorPos({ 192 * 3,50 });
    if (ImGui::SubTab("Editor", "", { 192, 37 }, tab == 3 ? true : false)) {
        tab = 3;
        update_animation_alpha();
    }

    if (tab == 0) {

    }
    else if (tab == 1) {
        ImGui::SetCursorPos({ 10, 90 });
        ImGui::BeginChild("config", ImVec2(385, 302), false, ImGuiWindowFlags_NoTitleBar);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::PopFont();
        ImGui::EndChild();

        ImGui::SetCursorPos({ 405, 90 });
        ImGui::BeginChild("settings", ImVec2(385, 302), false, ImGuiWindowFlags_NoTitleBar);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::PopFont();
        ImGui::EndChild();
    }
    else if (tab == 2) {

    }
    else if (tab == 3) {
        ImGui::SetCursorPos({ 10, 90 });
        ImGui::BeginChild("Menu Themes", ImVec2(770, 302), false, ImGuiWindowFlags_NoTitleBar);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);

        ImGui::ColorEdit3("Menu Color##menu_color", variables::menu_clr);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::ShowStyleEditor();

        ImGui::EndChild();
    }
}

void conf2ig() {
    static int config_tab = 0;
    static const char* configs[] = { "1", "2", "3", "4", "5" };
    static const char* choices[]{ "  yes", "  no" };
    static int current_config = 0;

    static int tab = 0;

    ImGui::SetCursorPos({ 0 ,50 });
    if (ImGui::SubTab("Config", "", { 330,37 }, tab == 0 ? false : true)) {
        tab = 0;
        update_animation_alpha();
    }

    ImGui::SetCursorPos({ 330,50 });
    if (ImGui::SubTab("Themes", "", { 330,37 }, tab == 1 ? false : true)) {
        tab = 1;
        update_animation_alpha();
    }


    if (tab == 0) {


        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("config", ImVec2(GROUPBOX_SIZE), false, ImGuiWindowFlags_NoTitleBar);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::Combo(("##config"), &current_config, configs, IM_ARRAYSIZE(configs));
        if (ImGui::Button(("save"), ImVec2(200.f, 20))) {
            ImGui::OpenPopup(("config save popup"));
        }

        if (ImGui::BeginPopup(("config save popup"))) {
            ImGui::Text(("are you sure you want to save selected config? "));

            for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
                if (ImGui::Selectable(choices[i]))
                    if (i == 0) {
                        config::save(configs[current_config]);
                        console::log("[config]", std::string("successfully saved config at slot: ").append(std::to_string(current_config + 1)).c_str());
                        interfaces::console->console_color_printf(Color(255, 255, 0, 255), std::string("successfully saved config at slot: ").append(std::to_string(current_config + 1)).c_str());
                    }

            ImGui::Spacing();

            ImGui::EndPopup();
        }

        if (ImGui::Button(("load"), ImVec2(200.f, 20))) {
            ImGui::OpenPopup(("config load popup"));
        }

        if (ImGui::BeginPopup(("config load popup"))) {
            ImGui::Text(("are you sure you want to load selected config? "));

            for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
                if (ImGui::Selectable(choices[i]))
                    if (i == 0) {
                        config::load(configs[current_config]);
                        console::log("[config]", std::string("successfully loaded config at slot: ").append(std::to_string(current_config + 1)).c_str());
                        interfaces::console->console_color_printf(Color(255, 255, 0, 255), std::string("successfully loaded config at slot: ").append(std::to_string(current_config + 1)).c_str());
                    }

            ImGui::Spacing();

            ImGui::EndPopup();
        }

        if (ImGui::Button(("open configuration folder"), ImVec2(200.f, 20)))
            ShellExecuteW(0, L"open", L"C:/csgo_cheat/configs", NULL, NULL, SW_NORMAL);

        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::SetCursorPos({ GROUPBOX_SECOND_POSITION });
        ImGui::BeginChild("Info", ImVec2(GROUPBOX_SIZE), false, ImGuiWindowFlags_NoTitleBar);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::Text(("Compilation date: " __DATE__ " - " __TIME__));
        ImGui::Text("Will be more soon ig?...");

        ImGui::PopFont();

        ImGui::EndChild();
    }
    else if (tab == 1)
    {
        //Child 1
        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("Menu Themes", ImVec2(316 * 2, 400), false, ImGuiWindowFlags_NoTitleBar);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::ColorEdit3("Menu Color##menu_color", variables::menu_clr);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::ShowStyleEditor();

        ImGui::PopFont();
        ImGui::EndChild();
    }
}