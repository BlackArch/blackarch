PSEUDO_PACKAGE_DIR=pseudo-packages

.PHONY:
.SILENT:

all: pseudo-packages pseudo-package-sources

clean-dist-only:
	echo "Removing all source and dist tarballs..."
	@rm -vrf $(wildcard pseudo-packages/*/{*.src.tar.gz,*.pkg.tar.xz,pkg,src}) \
	         $(wildcaard packages/*/{*.src.tar.gz,*.pkg.tar.xz})

clean: clean-dist-only
	echo "Removing pseudo packages..."
	@rm -vrf pseudo-packages

list-working:
	cd categories; \
	grep -v '\(^$$\|^all\|^#\)' *

list-broken:
	grep '^#[a-z]' categories/* | \
	sed 's/^.*#//' |              \
	sort -u

list-todo:
	cd research/kali-menu/items-tofind; \
	ls -1 *

# Pseudo-packages
# Pseudo-package PKGBUILDs are generated from the PKGBUILD.in file.
pseudo-package-pkgbuilds:
	for cat in $(wildcard categories/*) ; do                                     \
		cat_name=$$(basename $$cat);                                              \
		echo "Generating $$cat_name PKGBUILD...";                                 \
		mkdir -p ${PSEUDO_PACKAGE_DIR}/$$cat_name;                                \
		sed -e "s|%DEPENDS%|$$(grep -v '^#' $$cat | tr '\n' '\1')|"               \
		    -e "s|%CATEGORY_DESCRIPTION%|$$(sed -n -e '1s|^# ||' -e '1p' $$cat)|" \
		    -e "s|%CATEGORY%|$$cat_name|" PKGBUILD.in |                           \
		    tr '\1' '\n' >                                                        \
		    ${PSEUDO_PACKAGE_DIR}/$$cat_name/PKGBUILD;                            \
	done
	echo done.

pseudo-package-sources: pseudo-package-pkgbuilds
	for package in $(wildcard pseudo-packages/*) ; do \
		( cd "$$package";                              \
		makepkg -f --source; )                         \
	done

pseudo-package-dists: pseudo-package-pkgbuilds
	for package in $(wildcard pseudo-packages/*) ; do \
		( cd "$$package";                              \
		makepkg -df; )                                 \
	done

test-pseudo-package:
	pacman -U categories/all/archtrack-all.pkg.tar.xz

test-aur:
	yaourt -Sy
	yaourt -S archtrack-all

upload-pseudo-packages: clean pseudo-package-sources
	burp $(wildcard pseudo-packages/*/*.src.tar.gz)

# Packages
package-sources:
	for package in $(wildcard packages/*) ; do \
		( cd "$$package";                       \
		makepkg -f --source; )                  \
	done

package-dists:
	for package in $(wildcard packages/*) ; do \
		( cd "$$package";                       \
		makepkg -df; )                          \
	done

upload-packages: package-sources
	burp $(wildcard packages/*/*.src.tar.gz)
