# backlightd
Simple program that polls X to check if DPMS is enabled and screen is off and turns TFT screens backlight off

Installare dtc

   pacman -S dtc

compilare backlight-overlay.dts dopo avere eventualmente cambiato il GPIO pin

   dtc -@ -I dts -O dtb -o backlight.dtbo backlight-overlay.dts

copiare backlight.dtbo a /boot/overlay

aggiungere dtoverlay=backlight a /boot/config.txt

compilare backlightd.cpp

   g++ backlightd.cpp -lX11 -lXext -o backlightd

copiare backlightd a /usr/bin

aggiungere /usr/bin/backlightd & a /etc/X11/xinit/xinitrc dopo matchbox-window-manager


calibrare schermo

reference URL:
https://forums.raspberrypi.com/viewtopic.php?t=201639

http://comfilewiki.co.kr/en/doku.php?id=comfilepi:controlling_the_lcd_backlight:index
