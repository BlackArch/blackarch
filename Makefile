# BlackArch Linux Makefile
# Developer automation and quality control

.PHONY: all check fix stats clean help

all: check

check:
	@python3 scripts/bautil.py check all

fix:
	@python3 scripts/bautil.py fix all

stats:
	@python3 scripts/bautil.py stats

clean:
	@rm -rf scripts/__pycache__ /tmp/baconflict.* /tmp/blackarch* 2>/dev/null || true
	@find . -name "*.pyc" -delete

help:
	@echo "BlackArch Developer Makefile"
	@echo "----------------------------"
	@echo "  make check  - Run multi-threaded validation across all PKGBUILDs"
	@echo "  make fix    - Auto-format whitespace and modelines across packages"
	@echo "  make stats  - Display repository metrics and package breakdowns"
	@echo "  make clean  - Clean temporary test and build files"
