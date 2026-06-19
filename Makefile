DRIVERS := e1000e genet macb r8169

.PHONY: modules clean deb $(DRIVERS)

modules: $(DRIVERS)

$(DRIVERS):
	$(MAKE) -C $@ modules

clean:
	@for d in $(DRIVERS); do $(MAKE) -C $$d clean; done

distclean: clean
	@for d in $(DRIVERS); do \
		rm -rf $$d/debian/.debhelper $$d/debian/debhelper-build-stamp \
		       $$d/debian/files $$d/debian/*.debhelper.log \
		       $$d/debian/*.substvars $$d/debian/$$d-xdp-dkms; \
	done
	rm -f *.deb *.buildinfo *.changes

deb:
	@for d in $(DRIVERS); do \
		echo "=== Building $$d-xdp-dkms package ==="; \
		cd $$d && dpkg-buildpackage -us -uc -b && cd ..; \
	done
	@echo "=== Done. Packages are in parent directory ==="
