PSEUDO_PACKAGE_DIR=pseudo-packages

.PHONY: all list-working list-broken list-todo pseudo-packages clean
.SILENT:

all: pseudo-packages pseudo-package-sources

clean:
	echo "Removing 'pseudo-packages'..."
	@rm -rf pseudo-packages

clean-dist-only:
	echo "Removing pseudo package distributions..."
	@rm -rf $(wildcard pseudo-packages/*/{*.src.tar.gz,*.pkg.tar.xz,pkg,src})

list-working:
	cd categories; \
	grep -v '\(^$$\|^all\|^#\)' *

list-broken:
	grep '^#[a-z]' categories/* | sed 's/^.*#//' | sort -u

list-todo:
	cd research/kali-menu/items-tofind; \
	ls -1 *

# Pseudo-packages
# Pseudo-package PKGBUILDs are generated
pseudo-package-pkgbuilds:
	for cat in $(wildcard categories/*) ; do \
		cat_name=$$(basename $$cat); \
		echo "Generating $$cat_name PKGBUILD..."; \
		mkdir -p ${PSEUDO_PACKAGE_DIR}/$$cat_name; \
		sed "s/%DEPENDS%/$$(grep -v '^#' $$cat | tr '\n' '^')/" PKGBUILD.in | \
		    tr '^' '\n' | sed "s/%CATEGORY%/$$cat_name/" > \
		    ${PSEUDO_PACKAGE_DIR}/$$cat_name/PKGBUILD; \
	done
	echo done.

pseudo-package-sources: pseudo-package-pkgbuilds
	for package in $(wildcard pseudo-packages/*) ; do \
		( cd "$$package"; \
		makepkg -f --source; ) \
	done

pseudo-package-dists: pseudo-package-pkgbuilds
	for package in $(wildcard pseudo-packages/*) ; do \
		( cd "$$package"; \
		makepkg -df; ) \
	done

test-pseudo-package:
	pacman -U categories/all/archtrack-all.pkg.tar.xz

upload-pseudo-packages:
	burp $(wildcard pseudo-packages/*/*.src.tar.gz)

# Packages
package-sources:
	for package in $(wildcard packages/*) ; do \
		( cd "$$package"; \
		makepkg -f --source; ) \
	done

package-dists:
	for package in $(wildcard packages/*) ; do \
		( cd "$$package"; \
		makepkg -df; ) \
	done
