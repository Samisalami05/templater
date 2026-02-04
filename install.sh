# TODO: save template files in appdata so that templater can access them

sudo cp templater /usr/bin/
mkdir -p ~/.local/share/templater
cp -r presets/ ~/.local/share/templater/
