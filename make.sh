if [[ -e floppy.img ]]; then
	su -c "sh make.su"
else
	if [[ -e init.img.bz2 ]]; then
		cp init.img.bz2 floppy.img.bz2
		bunzip2 floppy.img.bz2
		su -c "sh make.su"
	else
		su -c "sh remake.su"
	fi
fi

