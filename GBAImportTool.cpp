#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"

static nlohmann::json game_data, game_strings_ja, game_strings_sc, game_strings_tc;

void replace_all_str(std::string& inout, std::string_view what, std::string_view with)
{
	for (std::string::size_type pos{}; inout.npos != (pos = inout.find(what.data(), pos, what.length())); pos += with.length()) {
		inout.replace(pos, what.length(), with.data(), with.length());
	}
}

void create_strings_json(std::string lang_folder, nlohmann::json& stringsJson) {
	std::string stringsPath = "bootapp\\resources\\strings\\" + lang_folder;
	if (!std::filesystem::exists(stringsPath))
		std::filesystem::create_directories(stringsPath);

	std::string defaultJsonStrPath = "temple\\bootapp\\resources\\strings\\" + lang_folder + "\\strings.lng";
	if (std::filesystem::exists(defaultJsonStrPath)) {
		try {
			std::ifstream ifs(defaultJsonStrPath);
			stringsJson = nlohmann::json::parse(ifs);
		}
		catch (std::exception&) {}
	}
}

void create_db() {
	nlohmann::json js_item, js_strings_ja, js_strings_sc, js_strings_tc;
	create_strings_json("ja", js_strings_ja);
	create_strings_json("zh-hans", js_strings_sc);
	create_strings_json("zh-hant", js_strings_tc);
	for (auto const& dir_entry : std::filesystem::directory_iterator("titles")) {
		std::error_code err;
		auto entry_status = std::filesystem::status(dir_entry.path(), err);
		if (err) continue;
		if (!std::filesystem::exists(dir_entry.path())) continue;

		std::cout << "当前处理title目录：" << dir_entry.path().string() << std::endl;
		if (std::filesystem::is_directory(entry_status)) {
			std::string code = dir_entry.path().filename().string();
			std::string infoJsonPath = "titles\\" + code + "\\" + (code + ".json");
			if (std::filesystem::exists(infoJsonPath)) {
				std::cout << "当前Title目录json文件存在，处理：" << infoJsonPath << std::endl;
				try {
					std::ifstream ifs(infoJsonPath);
					js_item = nlohmann::json::parse(ifs);
				}
				catch (std::exception&) {}
			}
			else {
				std::string defaultJsonStr = R"(
					{
						"titles": {
							"copyright": "©1992,2001 Nintendo Game developed by INTELLIGENT SYSTEMS.",
							"lcla6_release_date": "2023-02-01",
							"players_count": 4,
							"publisher": "任天堂",
							"release_date": "2001-07-21",
							"rewind_interval": 1.5,
							"save_count": 1,
							"simultaneous": false,
							"sort_publisher": "任天堂",
							"sort_title": "任天堂游戏",
							"title": "任天堂ゲーム",
							"title_ko": "닌텐도 게임",
							"title_zhHans": "任天堂游戏",
							"title_zhHant": "任天堂遊戲",
							"volume": 100
						}
					}
				)";
				try {
					js_item = nlohmann::json::parse(defaultJsonStr);
				}
				catch (std::exception&) {}
			}

			js_item["titles"]["code"] = code;
			if (std::filesystem::exists("titles\\" + code + "\\" + code + ".png")) {
				js_item["titles"]["cover"] = "/titles/" + code + "/" + code + ".png";
				std::string metaTitleCommentString = "META_TITLE_COMMENT_" + code;
				replace_all_str(metaTitleCommentString, "-", "_");
				if (js_item["strings"].count("META_TITLE_COMMENT_JA")) {
					js_strings_ja["strings"][metaTitleCommentString.c_str()] = js_item["strings"]["META_TITLE_COMMENT_JA"];
				}
				if (js_item["strings"].count("META_TITLE_COMMENT_SC")) {
					js_strings_sc["strings"][metaTitleCommentString.c_str()] = js_item["strings"]["META_TITLE_COMMENT_SC"];
				}
				if (js_item["strings"].count("META_TITLE_COMMENT_TC")) {
					js_strings_tc["strings"][metaTitleCommentString.c_str()] = js_item["strings"]["META_TITLE_COMMENT_TC"];
				}
			}
			if (std::filesystem::exists("\\titles\\" + code + "\\" + code + "-details.png"))
				js_item["titles"]["details_screen"] = "/titles/" + code + "/" + code + "-details.png";
			js_item["titles"]["rom"] = "/titles/" + code + "/" + code + ".gba";

			std::vector<std::string> compatible_titles;
			std::vector<std::string> connect_guides;
			std::vector<std::string> onecartridge_guides;
			for (auto const& title_entry : std::filesystem::directory_iterator(dir_entry.path())) {
				auto title_status = std::filesystem::status(title_entry.path(), err);
				if (err) continue;
				if (!std::filesystem::exists(title_entry.path())) continue;
				//if (std::filesystem::is_directory(title_status)) {
					//compatible_titles.push_back(title_entry.path().filename().string());
				//} else if (std::filesystem::is_regular_file(title_entry.path())) {
				if (std::filesystem::is_regular_file(title_entry.path())) {
					if (title_entry.path().filename().string().contains("CONNECT_GUIDE")) {
						connect_guides.push_back("/titles/" + code + "/" + title_entry.path().filename().string());
						std::string metaTitleConnectGuildString = "META_TITLE_" + title_entry.path().stem().string();
						std::string metaTitleConnectGuildStringWithCode = metaTitleConnectGuildString;
						replace_all_str(metaTitleConnectGuildString, "_" + code, "");
						replace_all_str(metaTitleConnectGuildString, "-", "_");
						replace_all_str(metaTitleConnectGuildStringWithCode, "-", "_");
						if (js_item["strings"].count(metaTitleConnectGuildString + "_JA")) {
							js_strings_ja["strings"][metaTitleConnectGuildStringWithCode.c_str()] = js_item["strings"][metaTitleConnectGuildString + "_JA"];
						}
						if (js_item["strings"].count(metaTitleConnectGuildString + "_SC")) {
							js_strings_sc["strings"][metaTitleConnectGuildStringWithCode.c_str()] = js_item["strings"][metaTitleConnectGuildString + "_SC"];
						}
						if (js_item["strings"].count(metaTitleConnectGuildString + "_TC")) {
							js_strings_tc["strings"][metaTitleConnectGuildStringWithCode.c_str()] = js_item["strings"][metaTitleConnectGuildString + "_TC"];
						}
					}
					else if (title_entry.path().filename().string().contains("ONECARTRIDGE_GUIDE")) {
						onecartridge_guides.push_back("/titles/" + code + "/" + title_entry.path().filename().string());
						std::string metaTitleOnecartridgeGuildString = "META_TITLE_" + title_entry.path().stem().string();
						std::string metaTitleOnecartridgeGuildStringWithCode = metaTitleOnecartridgeGuildString;
						replace_all_str(metaTitleOnecartridgeGuildString, "_" + code, "");
						replace_all_str(metaTitleOnecartridgeGuildString, "-", "_");
						replace_all_str(metaTitleOnecartridgeGuildStringWithCode, "-", "_");
						if (js_item["strings"].count(metaTitleOnecartridgeGuildString + "_JA")) {
							js_strings_ja["strings"][metaTitleOnecartridgeGuildStringWithCode.c_str()] = js_item["strings"][metaTitleOnecartridgeGuildString + "_JA"];
						}
						if (js_item["strings"].count(metaTitleOnecartridgeGuildString + "_SC")) {
							js_strings_sc["strings"][metaTitleOnecartridgeGuildStringWithCode.c_str()] = js_item["strings"][metaTitleOnecartridgeGuildString + "_SC"];
						}
						if (js_item["strings"].count(metaTitleOnecartridgeGuildString + "_TC")) {
							js_strings_tc["strings"][metaTitleOnecartridgeGuildStringWithCode.c_str()] = js_item["strings"][metaTitleOnecartridgeGuildString + "_TC"];
						}
					}
				}
			}
			compatible_titles.push_back(code);
			compatible_titles.push_back("SinglePak");

			if (!compatible_titles.empty())
				js_item["titles"]["compatible_titles"] = compatible_titles;
			if (!connect_guides.empty())
				js_item["titles"]["connect_guides"] = connect_guides;
			if (!onecartridge_guides.empty())
				js_item["titles"]["onecartridge_guides"] = onecartridge_guides;

			game_data["titles"][code.c_str()] = js_item["titles"];
		}
		std::cout << "------------------------------------" << std::endl;
	}

	if (!std::filesystem::exists("titles")) {
		std::cout << "错误！当前目录下titles文件夹不存在！" << std::endl;
		return;
	}

	std::string gameJsonPath = "titles\\lclassics.titlesdb";
	try {
		std::ofstream ofs(gameJsonPath);
		ofs << game_data;
	}
	catch (std::exception&) {}

	game_strings_ja["strings"] = js_strings_ja["strings"];
	game_strings_sc["strings"] = js_strings_sc["strings"];
	game_strings_tc["strings"] = js_strings_tc["strings"];
	try {
		std::ofstream ofs_ja("bootapp\\resources\\strings\\ja\\strings.lng");
		ofs_ja << game_strings_ja;
		std::ofstream ofs_sc("bootapp\\resources\\strings\\zh-hans\\strings.lng");
		ofs_sc << game_strings_sc;
		std::ofstream ofs_tc("bootapp\\resources\\strings\\zh-hant\\strings.lng");
		ofs_tc << game_strings_tc;
	}
	catch (std::exception&) {}
}

int main(int argc, char** argv) {
	create_db();
	std::cout << "处理完成！按任意键退出！" << std::endl;
	getchar();
	return 0;
}