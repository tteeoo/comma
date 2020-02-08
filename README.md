# COnfiguration Micro Manager (comma)

WIP. A niche tool for managing configuration files, specifically geared towards minimal linux installations.

I mainly am making this to practice c, although it is genuinely useful, atleast to me.

# Installation

```
$ git clone https://github.com/tteeoo/comma
$ cd comma
$ make
```
Then, optionally
```
# make install
```

# Usage

Comma is a very basic program and has four main functions:

Listing - comma will list out all tracked files when it has no arguments

`comma`

Loading - comma will start tracking a given file, giving it a description and a nickname to easily edit it with

`comma -l nickname "brief description" /path/to/file`


Editing - comma will attempt to open the file with the given nickname in your editor of choice, specified in its config.csv file

`comma nickname`


Unloading - comma will stop tracking a file with a specified nickname

`comma -u nickname`


# Configuration

Comma's configuration file is located at `~/.config/comma/config.csv`

A new file will be created if none exists

This is where you can change:

- The editor files are opened in

- Wether or not files are list in alternating colors


# License

All files are licensced under the MIT License, which can be found at /LICENSE
