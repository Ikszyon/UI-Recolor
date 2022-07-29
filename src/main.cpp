#include <Windows.h>
#include <fstream>
#include <string>
#include <array>
#include <sstream>
#include <filesystem>
#include <algorithm>

#include <cocos2d.h>
#include <cocos-ext.h>
#include <extensions2.h>

using namespace MegaHackExt;
using namespace cocos2d;

ColourPicker *Picker = ColourPicker::Create({0xff, 0xff, 0xff});

bool AutoSetColorsState = 1;

bool AutoSaveConfigState = 1;

int CurrentIndex;

const char *ColorNames[] = {"ID Panel",
							"Search Panels",
							"Gauntlet Gradient",
							"Menu Gradient",
							"Level List BG",
                            "2nd Level",
                            "1st Level",
                            "2nd Level Comment",
                            "1st Level Comment",
                            "Comment BG",
							"Level Description",
                            "Leaderboard BG",
                            "Profile Icons BG",
                            "Profile Post Tint",
                            "2nd Profile Post",
                            "1st Profile Post",
                            "Profile Post BG",
                            "Practice Mode Bar",
                            "Normal Mode Bar",
                            nullptr};

std::array<std::string, 19> ColorValues = {"002762",
										   "00388d",
										   "333333",
										   "0066ff",
										   "bf723e",
										   "c2723e",
										   "a1582c",
										   "904f27",
										   "9c552a",
										   "bf723e",
										   "a1582c",
										   "bf723e",
										   "bf723e",
										   "824021",
										   "c2723e",
										   "a1582c",
										   "bf723e",
										   "00FFFF",
										   "00FF00"};


// stole this from mat
// https://github.com/matcool/small-gd-mods/blob/main/src/menu-shaders.cpp
void Patch(void* loc, std::vector<std::uint8_t> bytes) {
    auto size = bytes.size();
    DWORD old_prot;
    VirtualProtect(loc, size, PAGE_EXECUTE_READWRITE, &old_prot);
    memcpy(loc, bytes.data(), size);
    VirtualProtect(loc, size, old_prot, &old_prot);
}


unsigned char HexToUChar(std::string String) {
    unsigned int i;
    std::istringstream Hex(String);
    Hex >> std::hex >> i;
    return i;
}


void MH_CALL SetColors() {
    auto BaseAddress = reinterpret_cast<char*>(GetModuleHandle(NULL)) + 0xC00;

	for(int i = 0; i < sizeof(ColorNames) / sizeof(char*) - 1; i++) {
		std::vector<uint8_t> Color = {HexToUChar(ColorValues[sizeof(ColorNames) / sizeof(char*) - 2 - i].substr(0, 2)),
								 	  HexToUChar(ColorValues[sizeof(ColorNames) / sizeof(char*) - 2 - i].substr(2, 2)),
									  HexToUChar(ColorValues[sizeof(ColorNames) / sizeof(char*) - 2 - i].substr(4, 2))};

		switch(i) {
			case 0: 	// Normal Mode Progress Bar
				Patch(BaseAddress + 2646284 + 512, Color);
				Patch(BaseAddress + 2646308 + 512, Color);
				Patch(BaseAddress + 2646356 + 512, Color);
				break;

			case 1: 		// Practice Mode Progress Bar
				Patch(BaseAddress + 1537717, {0x28, 0x5C, 0x3F, 0xFF, 0x52, 0x44, 0x20, 0xFF, 0x66, 0xC7, 0x45, 0xD4, Color[0], Color[1], 0xB1, Color[2]});
				Patch(BaseAddress + 1597537, {0x28, 0x5C, 0x3F, 0x8B, 0x01, 0xFF, 0x50, 0x44, 0xB1, Color[2], 0x90, 0x66, 0xC7, 0x45, 0xF8, Color[0], Color[1], 0x0F});
				Patch(BaseAddress + 1986136, {0x28, 0x5C, 0x3F, 0xFF, 0x52, 0x44, 0xB1, Color[2], 0x90, 0x66, 0xC7, 0x45, 0xEC, Color[0], Color[1], 0x66});
				break;

			case 2:			// Profile Post BG
				Patch(BaseAddress + 2174113, Color);
				break;

			case 3:			// 1st Profile Post
				Patch(BaseAddress + 394085, {Color[0], Color[1], 0xB2, Color[2]});
				break;

			case 4:			// 2nd Profile Post
				Patch(BaseAddress + 394075, {Color[0], Color[1], 0xB2, Color[2]});
				break;

			case 5:			// Profile Post Tint
				Patch(BaseAddress + 389910, {Color[0], Color[1], 0xB1, Color[2]});
				break;

			case 6:			// Profile Icons BG
				Patch(BaseAddress + 2156615, Color);
				Patch(BaseAddress + 1559020, Color);
				break;

			case 7:			// Leaderboard BG
				Patch(BaseAddress + 1410671, Color);
				break;
				
			case 8:			// Level Description BG
				Patch(BaseAddress + 1372673, {Color[0], Color[1], 0x8D, 0x55, 0x89, 0x66, 0x8B, 0x45, 0x88, 0xB1, Color[2]});
				break;

			case 9:			// Comment BG
				Patch(BaseAddress + 1382316, Color);
				break;

			case 10:		// 1st Level Comment
				Patch(BaseAddress + 394045, {Color[0], Color[1], 0xB2, Color[2]});
				break;

			case 11:		// 2nd Level Comment
				Patch(BaseAddress + 394035, {Color[0], Color[1], 0xB2, Color[2]});
				break;

			case 12:		// 1st Level
				Patch(BaseAddress + 375513, {Color[0], Color[1], 0xB1, Color[2]});
				break;

			case 13:		// 2nd Level
				Patch(BaseAddress + 375503, {Color[0], Color[1], 0xB1, Color[2]});
				break;

			case 14:		// Level List BG
				Patch(BaseAddress + 1422380, Color);
				break;
			
			case 15:		// Menu Gradient
				Patch(BaseAddress + 316278, {0x90, 0x66, 0xc7, 0x44, 0x24, 0x40, Color[0], Color[1], 0xb1, Color[2]});
				Patch(BaseAddress + 453620, {0x90, 0x66, 0xc7, 0x45, 0xb8, Color[0], Color[1], 0xb1, Color[2]});
				Patch(BaseAddress + 1408277, {0x90, 0x66, 0xc7, 0x44, 0x24, 0x0c, Color[0], Color[1], 0xb1, Color[2]});
				Patch(BaseAddress + 1414509, {0x90, 0x66, 0xc7, 0x45, 0xf4, Color[0], Color[1], 0xb1, Color[2]});
				Patch(BaseAddress + 1528878, {0x90, 0x66, 0xC7, 0x45, 0x98, Color[0], Color[1], 0xB1, Color[2]});
				Patch(BaseAddress + 1560505, {0x90, 0x66, 0xc7, 0x44, 0x24, 0x14, Color[0], Color[1], 0xb1, Color[2]});
				// Patch(BaseAddress + 1619492, {0x90, 0x66, 0xc7, 0x44, 0x24, 0x0c, Color[0], Color[1], 0xb1, Color[2]}); -- Startup bg
				break;

			case 16:		// Gauntlet Gradient
				Patch(BaseAddress + 0x175464, {Color[0], Color[1], 0xB1, Color[2]});
				Patch(BaseAddress + 1066801, {Color[0], Color[1], 0xB1, Color[2]});
				break;

			case 17:		// Search Panels
				Patch(BaseAddress + 1561349, {Color[0], Color[1], 0xB1, Color[2]});
				Patch(BaseAddress + 454164, {Color[0], Color[1], 0xB1, Color[2]});
				Patch(BaseAddress + 454822, {Color[0], Color[1], 0xB1, Color[2]});
				Patch(BaseAddress + 1563961, {Color[0], Color[1], 0xB1, Color[2]});
				Patch(BaseAddress + 1564693, {Color[0], Color[1], 0xB1, Color[2]});
				Patch(BaseAddress + 1562642, {0x66, 0xc7, 0x44, 0x24, 0x14, Color[0], Color[1], 0xb1, Color[2]});
				break;

			case 18:		// ID Panel
				Patch(BaseAddress + 1561590, {Color[0], Color[1], 0xB1, Color[2]});
				break;

			default:
				break;
		}
	}
}


void MH_CALL WriteConfig() {

	auto ConfigPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("uirecolor.txt", false);

	std::ofstream ConfigFile(ConfigPath);
	ConfigFile.clear();
	for(int i = 0; i < sizeof(ColorNames) / sizeof(char*) - 1; ++i) {
		ConfigFile << ColorNames[i] << ": " << ColorValues[i] << "\n";
	}
	ConfigFile << "AutoSetColors: " << AutoSetColorsState << "\nAutoSaveColors: " << AutoSaveConfigState << "\n";
	ConfigFile.close();
}


void MH_CALL ReadConfig() {

	auto ConfigPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("uirecolor.txt", false);

	if(!std::filesystem::exists(ConfigPath)) {
		WriteConfig();
	}

	std::ifstream ConfigFile(ConfigPath);
	std::stringstream SStr;
    SStr << ConfigFile.rdbuf();
    std::string Config = SStr.str();
		for(int i = 0; i < sizeof(ColorNames) / sizeof(char*) + 1; i++) {
			// normally would have used regex - "(?<=" ColorNames[i] ":\W)[0-9A-Fa-f]*"
			// but positive lookbehind isn't supported by std::regex
			// and I don't want to use boost::regex

		if(i < sizeof(ColorNames) / sizeof(char*) - 1) {
			std::string CurrentColor = Config.substr(Config.find(ColorNames[i]) + strlen(ColorNames[i]) + 2, 6);
			if(std::all_of(CurrentColor.begin(), CurrentColor.end(), ::isxdigit) && CurrentColor.length() == 6) {
				ColorValues[i] = CurrentColor;
			}
		} else { // this is horrible but avoids type conversion (I dont want to do type conversion)
			AutoSetColorsState = (Config.substr(Config.find("AutoSetColors: ") + strlen("AutoSetColors: "), 1) == "0" ? 0 : 1);
			AutoSaveConfigState = (Config.substr(Config.find("AutoSaveColors: ") + strlen("AutoSaveColors: "), 1) == "0" ? 0 : 1);
		}
	}
	ConfigFile.close();
}


void MH_CALL ReloadConfig(Button* Button) {
	ReadConfig();
	SetColors();
}


void MH_CALL OpenConfig(Button* Button) {
	auto ConfigPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("uirecolor.txt", false);
	ShellExecute(NULL, "open", ConfigPath.data(), NULL, NULL, SW_SHOWNORMAL); // Ignore these errors
}


void MH_CALL PickerCallback(ColourPicker* Picker, Colour col) {
	// Dogshit color management but hihi

	std::stringstream ColorHex;
	std::stringstream CurrentPart;

	unsigned char Color[] = {col.r,
							 col.g,
							 col.b};

	for(int i = 0; i < 3 ; i++) {
		CurrentPart << std::hex << (0xFF & Color[i]);
		if(CurrentPart.str().length() < 2) {
			ColorHex << "0";
		}
		ColorHex << CurrentPart.str();
		std::stringstream().swap(CurrentPart);
	}

	ColorValues[CurrentIndex] = ColorHex.str();

	if(AutoSetColorsState) {
		SetColors();
	}

	if(AutoSaveConfigState) {
		WriteConfig();
	}
}


void MH_CALL ColorSelectCallback(ComboBox* obj, int Index, const char* String) {
	CurrentIndex = Index;

	// Set Picker to the color of selected element

	int ColorBytes[] = {HexToUChar(ColorValues[CurrentIndex].substr(0, 2)),
						HexToUChar(ColorValues[CurrentIndex].substr(2, 2)),
						HexToUChar(ColorValues[CurrentIndex].substr(4, 2))};
	
	Colour NewPickerColour = {(ColorBytes[0]), (ColorBytes[1]), (ColorBytes[2])};
	Picker->set(NewPickerColour, 0);
}


void MH_CALL AutoSetColors(CheckBox* Box, bool State) {
	AutoSetColorsState = State;
}


void MH_CALL AutoSaveConfig(CheckBox* Box, bool State) {
	AutoSaveConfigState = State;
}


DWORD MainThread(LPVOID lpParam) {
	ReadConfig();

	Window* UIRecolor = Window::Create("UI Recolor");

	ComboBox* ColorSelect = ComboBox::Create("Color: ", nullptr);
	ColorSelect->setCallback(ColorSelectCallback);
	ColorSelect->setValues(ColorNames);

	// already created because global pointer, I know its aids
	Picker->setCallback(PickerCallback);

	Button* SetButton = Button::Create("Set");
	SetButton->setCallback([](Button* Button){SetColors();});

	Button* SaveConfig = Button::Create("Save");
	SaveConfig->setCallback([](Button* Button){WriteConfig();});

	CheckBox* AutoSave = CheckBox::Create("Auto Save");
	AutoSave->setCallback(AutoSaveConfig);
	AutoSave->set(AutoSaveConfigState, 0);

	CheckBox* AutoSet = CheckBox::Create("Auto Set");
	AutoSet->setCallback(AutoSetColors);
	AutoSet->set(AutoSetColorsState, 0);

	Button* Reload = Button::Create("Reload Config");
	Reload->setCallback(ReloadConfig);

	Button* Open = Button::Create("Open Config");
	Open->setCallback(OpenConfig);

	UIRecolor->add(Open);
	UIRecolor->add(Reload);
	UIRecolor->add(HorizontalLayout::Create(AutoSet, AutoSave));
	UIRecolor->add(HorizontalLayout::Create(SetButton, SaveConfig));
	UIRecolor->add(Picker);
	UIRecolor->add(ColorSelect);


	Picker->set({HexToUChar(ColorValues[sizeof(ColorNames) / sizeof(char*) - 2].substr(0, 2)),
				 HexToUChar(ColorValues[sizeof(ColorNames) / sizeof(char*) - 2].substr(2, 2)),
				 HexToUChar(ColorValues[sizeof(ColorNames) / sizeof(char*) - 2].substr(4, 2))}, 0);

	Client::commit(UIRecolor);

	SetColors();

	return S_OK;
}


DWORD DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0x1000, reinterpret_cast<LPTHREAD_START_ROUTINE>(&MainThread), NULL, 0, NULL);
		break;

	default:
		break;
	}

	return TRUE;
}