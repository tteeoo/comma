# COnfiguration Micro Manager (comma)

WIP. A niche tool for managing configuration files, specifically geared towards minimal linux installations.

I mainly am making this to practice c, although it is genuinely useful, atleast to me.

This project uses <a href="https://cmake.org">CMake</a> which is a little bloat for this project, but I couldn't be bothered with Makefiles.

# Installation

```
git clone https://github.com/tteeoo/comma
cd comma/build
make
```
Then, optionally
```
sudo cp comma /usr/bin/
cd ..
rm comma
```
