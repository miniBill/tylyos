if [[ -e floppy.img ]]; then
	su -c "sh make.su"
else
	su -c "sh remake.su"
fi

