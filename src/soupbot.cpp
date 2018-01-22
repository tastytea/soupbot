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

int main(int argc, char *argv[])
{
    pt::ptree config;
    string instance = "";
    string access_token = "";

    read_config(config, instance, access_token);

    pt::ptree ingredients;
    pt::read_json(filepath + "ingredients.json", ingredients);
    
    return 0;
}
