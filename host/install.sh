#!/bin/sh

script_path=`readlink -f $0`
script_dir=`dirname $script_path`
cd $script_dir

#TODO
#/etc/udev/rules.d/99-hid.rules

[ -d $HOME/bin ] || mkdir $HOME/bin
ln -s "$script_dir/rawhid.py" $HOME/bin/
