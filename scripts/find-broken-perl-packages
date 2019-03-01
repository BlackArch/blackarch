#!/bin/bash
perllibpath="/usr/lib/perl5/vendor_perl/auto/"

tmpdir=$(mktemp -d /tmp/find-broken-perl-package.XXXXXXXX)
touch $tmpdir/{raw,perl-modules}.txt
find "$perllibpath" -name "*.so" |
	while read file; do
		module=$(echo $file | sed \
				 -e "s|$perllibpath||" \
				 -e 's|/|::|g' \
				 -e 's|.so$||' \
				 -e 's|\(.*\)::.*$|\1|')
		output=$(perl -M$module -e1 2>&1)
		ret=$?
		if (($ret != 0)); then
			echo $file >> $tmpdir/raw.txt
			echo "$module" >> "$tmpdir/perl-modules.txt"
		elif grep -q "perl: symbol lookup error:" <<< $output; then
			sed -n 's|perl: symbol lookup error: \(.*\): undefined symbol: .*|\1|p' <<< $output >> $tmpdir/raw.txt
			echo "$module" >> "$tmpdir/perl-modules.txt"
		elif grep -q "Perl API version .* of .* does not match .*" <<< $output; then
			echo $file >> $tmpdir/raw.txt
			echo "$module" >> "$tmpdir/perl-modules.txt"
		fi
	done

pacman -Qqo $(<$tmpdir/raw.txt) | sort -u >$tmpdir/pacman.txt
perl module-to-dist.pl <$tmpdir/perl-modules.txt >$tmpdir/perl-dists.txt

echo "results are in \"$tmpdir\""
