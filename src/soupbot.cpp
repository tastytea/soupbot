/*  This file is part of soupbot.
 *  Copyright © 2018 tastytea <tastytea@tastytea.de>
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

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <cstdlib>
#include <mastodon-cpp.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <random>
#include <chrono>
#include "version.hpp"

namespace pt = boost::property_tree;
using Mastodon::API;
using std::cout;
using std::string;

const string filepath = string(getenv("HOME")) + "/.config/soupbot/";

void read_config(pt::ptree &config, string &instance, string &access_token)
{
    bool config_changed = false;

    // Read config file, get access token
    try {
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

void populate_vector(const pt::ptree &ingredients, const string &node, std::vector<string> &vector)
{
    for (const pt::ptree::value_type &value : ingredients.get_child(node))
    {
        vector.push_back(value.second.data());
    }
}

int main(int argc, char *argv[])
{
    pt::ptree config;
    string instance = "";
    string access_token = "";

    read_config(config, instance, access_token);

    pt::ptree ingredients;
    pt::read_json(filepath + "ingredients.json", ingredients);
    std::vector<string> vegetables;
    std::vector<string> spices;
    std::vector<string> fruits;
    std::vector<string> misc;
    std::vector<string> highprio;

    populate_vector(ingredients, "vegetables", vegetables);
    populate_vector(ingredients, "spices", spices);
    populate_vector(ingredients, "fruits", fruits);
    populate_vector(ingredients, "misc", misc);
    populate_vector(ingredients, "highprio", highprio);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<std::uint8_t> distribution(0, 3);
    std::uniform_int_distribution<std::uint16_t> distribution_vegetables(0, vegetables.size() - 1);
    std::uniform_int_distribution<std::uint16_t> distribution_spices(0, spices.size() - 1);
    std::uniform_int_distribution<std::uint16_t> distribution_fruits(0, fruits.size() - 1);
    std::uniform_int_distribution<std::uint16_t> distribution_misc(0, misc.size() - 1);
    std::uniform_int_distribution<std::uint16_t> distribution_highprio(0, highprio.size() - 1);

    auto dice = std::bind(distribution, generator);
    auto vegdice = std::bind(distribution_vegetables, generator);
    auto spicedice = std::bind(distribution_spices, generator);
    auto fruitdice = std::bind(distribution_fruits, generator);
    auto miscdice = std::bind(distribution_misc, generator);
    auto priodice = std::bind(distribution_highprio, generator);

    string toot = "Todays #soup is made of:\n\n";

    toot += "1 part " + vegetables[vegdice()];
    if (dice() > 0)
        toot += ",\n1 part " + vegetables[vegdice()];
    if (dice() > 1)
        toot += ",\n1 part " + vegetables[vegdice()];

    if (dice() > 1)
        toot += ",\na few " + fruits[fruitdice()];

    if (dice() > 1)
        toot += ",\na good handful " + misc[miscdice()];
    if (dice() > 1)
        toot += ",\na good handful " + misc[miscdice()];

    toot += ",\n" + spices[spicedice()];
    if (dice() > 0)
        toot += ",\n" + spices[spicedice()];
    if (dice() > 1)
        toot += ",\n" + spices[spicedice()];

    toot += ",\nan ample amount of " + highprio[priodice()];
    if (dice() > 1)
        toot += " and " + highprio[priodice()];

    toot += "\nand plenty oil.\n\n#bot";

    cout << toot << '\n';


    // string answer;
    // std::uint16_t ret;
    // Mastodon::API masto(instance, access_token);
    // masto.set_useragent("soupbot/" + (string)global::version);

    return 0;
}
