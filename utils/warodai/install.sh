#!/bin/sh
echo Copying dictionary files
cp warodai.dict.dz warodai.index /usr/share/dictd
echo updating config
dictdconfig -w
echo restarting dictd service
service dictd restart
