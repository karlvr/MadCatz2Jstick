installservice:
	install -d $(DESTDIR)/lib/udev/rules.d/
	install -m 0644 10-madcatz2jstick.rules $(DESTDIR)/lib/udev/rules.d/
	install -d $(DESTDIR)/lib/systemd/system/
	install -m 0644 madcatz2jstick.service $(DESTDIR)/lib/systemd/system/
	udevadm trigger --action=add --subsystem-match=input

uninstallservice:
	-rm $(DESTDIR)/lib/udev/rules.d/10-madcatz2jstick.rules
	-rm $(DESTDIR)/lib/systemd/system/madcatz2jstick.service
