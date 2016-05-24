@echo off
cd /d e:\1.git
git init --bare --shared=true
git daemon --export-all --enable=receive-pack