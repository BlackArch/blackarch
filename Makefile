PSEUDO_PACKAGE_DIR=pseudo-packages

.PHONY: all list-working list-broken list-todo pseudo-packages clean
.SILENT: all list-working list-broken list-todo pseudo-packages clean

all: pseudo-packages pseudo-package-sources

clean:
	echo "Removing 'pseudo-packages'..."
	@rm -rf pseudo-packages
	echo done.

list-working:
	cd categories; \
	grep -v '\(^$$\|^all\|^#\)' *

list-broken:
	grep '^#[a-z]' categories/* | sed 's/^.*#//' | sort -u

list-todo:
	cd research/kali-menu/items-tofind; \
	ls -1 *

pseudo-packages: clean
	mkdir ${PSEUDO_PACKAGE_DIR}; \
	for cat in $(wildcard categories/*) ; do \
		cat_name=$$(basename $$cat); \
		echo "Generating $$cat_name..."; \
		mkdir ${PSEUDO_PACKAGE_DIR}/$$cat_name; \
		sed "s/%DEPENDS%/$$(grep -v '^#' $$cat | tr '\n' '^')/" PKGBUILD.in | \
		    tr '^' '\n' | sed "s/%CATEGORY%/$$cat_name/" > \
		    ${PSEUDO_PACKAGE_DIR}/$$cat_name/PKGBUILD; \
	done
	echo done.
