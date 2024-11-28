#include "View/Interface/ImGuiRenderer/EditorCommands.h"

#include <iostream>
#include <MultithreatedSystems/Console.h>


void vkImGui::render_editor() {
	float screenHeight = ImGui::GetIO().DisplaySize.y;
	float screenWidth = ImGui::GetIO().DisplaySize.x;

	//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Jasny szary kolor

	if (ImGui::BeginMainMenuBar()) {
		vkSettings::menuHeight = ImGui::GetWindowHeight();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Create")) {
				// Akcja Create
			}

			// Submenu dla "Open"
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				// Akcja Open
			}

			if (ImGui::BeginMenu("Save Options")) {  // Tworzymy submenu
				if (ImGui::MenuItem("Save")) {
					// Akcja Save
				}
				if (ImGui::MenuItem("Save as..")) {
					// Akcja Save as..
				}
				ImGui::EndMenu();  // Koñczymy submenu
			}

			ImGui::EndMenu();  // Koñczymy menu g³ówne
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Create")) {
				// Akcja Create
			}

			// Submenu dla "Open"
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				// Akcja Open
			}

			if (ImGui::BeginMenu("Save Options")) {  // Tworzymy submenu
				if (ImGui::MenuItem("Save")) {
					// Akcja Save
				}
				if (ImGui::MenuItem("Save as..")) {
					// Akcja Save as..
				}
				ImGui::EndMenu();  // Koñczymy submenu
			}

			ImGui::EndMenu();  // Koñczymy menu g³ówne
		}
		ImGui::EndMainMenuBar();
	}

	//ImGui::PopStyleColor(); // Przywrócenie poprzedniego stylu




	// Pozycja i rozmiar dolnego panelu
	ImVec2 bottomPanelPos = ImVec2(0, screenHeight - vkSettings::bottomPanelHeight); // Na dole ekranu
	ImVec2 bottomPanelSize = ImVec2(screenWidth - vkSettings::rightPanelWidth, vkSettings::bottomPanelHeight); // Szerokoœæ do prawego panelu

	// Renderowanie menu g³ównego (opcjonalne, jeœli chcesz mieæ menu nad panelem)
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Project")) {
			if (ImGui::MenuItem("New Project")) {
				// Akcja dla "New Project"
			}
			if (ImGui::MenuItem("Open Project")) {
				// Akcja dla "Open Project"
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Console")) {
			if (ImGui::MenuItem("Show Console")) {
				// Akcja dla "Show Console"
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Panel dolny
	ImGui::SetNextWindowPos(bottomPanelPos); // Pozycja dolnego panelu
	ImGui::SetNextWindowSize(bottomPanelSize); // Rozmiar dolnego panelu
	ImGui::Begin("Bottom Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

	// Przyciski w dolnym panelu
	if (ImGui::Button("Button 1")) {
		std::cout << "Button 1 Clicked" << std::endl;
	}

	ImGui::SameLine(); // Umieszczamy drugi przycisk obok pierwszego
	if (ImGui::Button("Button 2")) {
		std::cout << "Button 2 Clicked" << std::endl;
	}

	// Blokada dostêpu do wiadomoœci
	std::lock_guard<std::mutex> lock(console::consoleMutex);

	// Iteracja po wiadomoœciach i wyœwietlanie ich w odpowiednich kolorach
	for (const auto& msg : console::consoleMessages) {
		ImGui::PushStyleColor(ImGuiCol_Text, msg.second);
		ImGui::TextWrapped("%s", msg.first.c_str());
		ImGui::PopStyleColor();
	}
	
	// Zaktualizowanie zmiennej wysokoœci dolnego panelu
	ImVec2 bottomPanelActualSize = ImGui::GetWindowSize();
	vkSettings::bottomPanelHeight = bottomPanelActualSize.y; // Zaktualizowanie wysokoœci

	ImGui::End();


	// Panel lewy
	ImVec2 leftPanelPos = ImVec2(0, vkSettings::menuHeight); // Na lewo od ekranu
	ImVec2 leftPanelSize = ImVec2(vkSettings::leftPanelWidth, screenHeight - vkSettings::bottomPanelHeight - vkSettings::menuHeight); // Wysokoœæ do dolnego panelu
	ImGui::SetNextWindowPos(leftPanelPos);
	ImGui::SetNextWindowSize(leftPanelSize);
	ImGui::Begin("Left Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	if (ImGui::Button("Left Button")) {
		// Akcja przycisku w lewym panelu
		std::cout << "Left Button Clicked" << std::endl;
	}
	// Jeœli zmienia siê rozmiar panelu lewego:
	ImVec2 leftPanelActualSize = ImGui::GetWindowSize();
	vkSettings::leftPanelWidth = leftPanelActualSize.x; // Zaktualizowanie zmiennej rozmiaru lewego panelu
	ImGui::End();

	// Panel prawy
	ImVec2 rightPanelPos = ImVec2(screenWidth - vkSettings::rightPanelWidth, vkSettings::menuHeight); // Na prawo od ekranu
	ImVec2 rightPanelSize = ImVec2(vkSettings::rightPanelWidth, screenHeight); // Wysokoœæ do dolnego panelu
	ImGui::SetNextWindowPos(rightPanelPos);
	ImGui::SetNextWindowSize(rightPanelSize);
	ImGui::Begin("Right Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	if (ImGui::Button("Right Button")) {
		// Akcja przycisku w prawym panelu
		std::cout << "Right Button Clicked" << std::endl;
	}
	// Jeœli zmienia siê rozmiar panelu prawego:
	ImVec2 rightPanelActualSize = ImGui::GetWindowSize();
	vkSettings::rightPanelWidth = rightPanelActualSize.x; // Zaktualizowanie zmiennej rozmiaru prawego panelu
	ImGui::End();
}
