**soupbot** is a soup-recommending bot for Mastodon.

# Install

## Dependencies

 * Tested OS: Linux
 * C++ compiler (tested: gcc 8)
 * [cmake](https://cmake.org/) (tested: 3.9)
 * [boost](http://www.boost.org/) (tested: 1.65)
 * [mastodon-cpp](https://schlomp.space/tastytea/mastodon-cpp) (at least: 0.105.0)

## Get sourcecode

### Development version

``` shellsession
git clone https://schlomp.space/tastytea/soupbot.git
```

## Compile

``` shellsession
mkdir build
cd build/
cmake ..
make
```

cmake options:

 * `-DCMAKE_BUILD_TYPE=Debug` for a debug build

Install with `make install`.

# Usage

Copy `ingredients.json` to `~/.config/soupbot/ingredients.json`.

You will need a pre-generated authentication code.

## Error codes

Same as
[mastodon-cpp](https://schlomp.space/tastytea/mastodon-cpp/src/branch/master/README.md#error-codes),
plus:

|      Code | Explanation                   |
| --------: |:------------------------------|
|         1 | File not readable             |

# Copyright

``` text
Copyright © 2018, 2019 tastytea <tastytea@tastytea.de>.
License GPLv3: GNU GPL version 3 <https://www.gnu.org/licenses/gpl-3.0.html>.
This program comes with ABSOLUTELY NO WARRANTY. This is free software,
and you are welcome to redistribute it under certain conditions.
```
