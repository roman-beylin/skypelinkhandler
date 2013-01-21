#!/bin/sh
qmake && make
mkdir -p ~/.skypehandler
cp ./skypehandler ~/.skypehandler/
gconftool-2 -t string -s /desktop/gnome/url-handlers/skype/command "$HOME/.skypehandler/skypehandler %s"
gconftool-2 -s /desktop/gnome/url-handlers/skype/needs_terminal false -t bool
gconftool-2 -t bool -s /desktop/gnome/url-handlers/skype/enabled true

mkdir -p ~/.local/share/applications/
cp ./skypehandler.desktop ~/.local/share/applications/
echo "Exec=$HOME/.skypehandler/skypehandler %u" >>  ~/.local/share/applications/skypehandler.desktop
echo >> ~/.local/share/applications/mimeapps.list
echo "[Added Associations]" >> ~/.local/share/applications/mimeapps.list
echo x-scheme-handler/skype=skypehandler.desktop >> ~/.local/share/applications/mimeapps.list
