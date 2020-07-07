/*  This file is part of soupbot.
 *  Copyright © 2018, 2019, 2020 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "version.hpp"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <mastodonpp/mastodonpp.hpp>
#include <random>
#include <string>
#include <vector>

namespace pt = boost::property_tree;
namespace masto = mastodonpp;

using std::cout;
using std::string;

const string filepath = string(getenv("HOME")) + "/.config/soupbot/";

void read_config(pt::ptree &config, string &instance, string &access_token)
{
    bool config_changed = false;

    // Read config file, get access token
    try
    {
        pt::read_json(filepath + "config.json", config);
        instance = config.get("instance", "");
        access_token = config.get("access_token", "");
    }
    catch (std::exception &e)
    {
        // most likely no config file found
        cout << "No config file found. Building new one.\n";
        const boost::filesystem::path path(filepath);
        boost::filesystem::create_directory(path);
    }

    if (instance.empty())
    {
        cout << "Instance: ";
        std::cin >> instance;
        config.put("instance", instance);
        config_changed = true;
    }
    if (access_token.empty())
    {
        cout << "Access token: ";
        std::cin >> access_token;
        config.put("access_token", access_token);
        config_changed = true;
    }
    if (config_changed)
    {
        pt::write_json(filepath + "config.json", config);
    }
}

void populate_vector(const pt::ptree &ingredients, const string &node,
                     std::vector<string> &vector)
{
    const pt::ptree childs = ingredients.get_child(node);
    std::transform(
        childs.begin(), childs.end(), std::back_inserter(vector),
        [](const pt::ptree::value_type &value) { return value.second.data(); });
}

string get_ingredient(std::vector<string> &vector)
{
    if (vector.size() == 0)
        return "nothing";

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<std::uint16_t> distribution(0, vector.size()
                                                                     - 1);
    const std::uint16_t i = distribution(generator);
    const string ingredient = vector.at(i);

    vector.erase(vector.begin() + i);
    return ingredient;
}

int main()
{
    pt::ptree config;
    string instance = "";
    string access_token = "";
    pt::ptree ingredients;
    std::vector<string> vegetables;
    std::vector<string> spices;
    std::vector<string> fruits;
    std::vector<string> misc;
    std::vector<string> highprio;

    read_config(config, instance, access_token);

    try
    {
        pt::read_json(filepath + "ingredients.json", ingredients);
    }
    catch (std::exception &e)
    {
        // most likely file not found
        std::cerr << "ERROR: " << filepath
                  << "ingredients.json not found or not readable.\n";
        return 1;
    }

    populate_vector(ingredients, "vegetables", vegetables);
    populate_vector(ingredients, "spices", spices);
    populate_vector(ingredients, "fruits", fruits);
    populate_vector(ingredients, "misc", misc);
    populate_vector(ingredients, "highprio", highprio);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<std::uint8_t> distribution(0, 3);
    auto dice = std::bind(distribution, generator);

    string toot = "Todays soup is made of:\n\n";

    toot += "1 part " + get_ingredient(vegetables);
    if (dice() > 0)
        toot += ", \n1 part " + get_ingredient(vegetables);
    if (dice() > 0)
        toot += ", \n1 part " + get_ingredient(vegetables);
    if (dice() > 1)
        toot += ", \n1 part " + get_ingredient(vegetables);
    if (dice() > 2)
        toot += ", \n1 part " + get_ingredient(vegetables);

    if (dice() > 1)
        toot += ", \na few " + get_ingredient(fruits);

    if (dice() > 1)
        toot += ", \na good handful " + get_ingredient(misc);
    if (dice() > 1)
        toot += ", \na good handful " + get_ingredient(misc);

    toot += ", \n" + get_ingredient(spices);
    if (dice() > 0)
        toot += ", \n" + get_ingredient(spices);
    if (dice() > 1)
        toot += ", \n" + get_ingredient(spices);
    if (dice() > 1)
        toot += ", \n" + get_ingredient(spices);
    if (dice() > 2)
        toot += ", \n" + get_ingredient(spices);

    toot += ", \nan ample amount of " + get_ingredient(highprio);
    if (dice() > 1)
        toot += " and " + get_ingredient(highprio);

    toot += ", \nand plenty oil. Salt to taste. \n\nHappy cooking! 🍲 \n\n#bot";

    masto::Instance server{instance, access_token};
    server.set_useragent("soupbot/" + (string)global::version);
    masto::Connection connection{server};

    masto::parametermap params = {{"status", {toot}},
                                  {"visibility", {"public"}}};
    auto ret{connection.post(masto::API::v1::statuses, params)};

    if (ret)
    {
        cout << ret.body << '\n';
    }
    else
    {
        std::cerr << "Error code: " << ret.curl_error_code << '\n';
        return ret.curl_error_code;
    }

    return 0;
}
