**soupbot** is a soup-recommending bot for Mastodon.

# Install

## Dependencies

 * Tested OS: Linux
 * C++ compiler (tested: gcc 6.4)
 * [cmake](https://cmake.org/) (tested: 3.9.6)
 * [boost](http://www.boost.org/) (tested: 1.63.0)
 * [mastodon-cpp](https://github.com/tastytea/mastodon-cpp) (at least: 0.1.5)

## Get sourcecode

### Development version

    git clone https://github.com/tastytea/soupbot-cpp.git

## Compile

    mkdir build
    cd build/
    cmake ..
    make

cmake options:

 * `-DCMAKE_BUILD_TYPE=Debug` for a debug build

Install with `make install`.

# Usage

Copy `ingredients.json` to `~/.config/soupbot/ingredients.json`.

You will need a pre-generated authentication code.

## Error codes

Same as [mastodon-cpp](https://github.com/tastytea/mastodon-cpp/blob/master/README.md#error-codes), plus:

|      Code | Explanation                   |
| --------: |:------------------------------|
|         1 | File not readable             |

If you use a debug build, you get more verbose error messages.

# Copyright

    Copyright © 2018 tastytea <tastytea@tastytea.de>.
    License GPLv3: GNU GPL version 3 <https://www.gnu.org/licenses/gpl-3.0.html>.
    This program comes with ABSOLUTELY NO WARRANTY. This is free software,
    and you are welcome to redistribute it under certain conditions.
