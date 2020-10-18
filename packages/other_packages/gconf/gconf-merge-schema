#!/bin/bash
if [ ! "$1" -a ! "$2" ]; then
  echo "Usage: $0 output.schemas [--domain gettextdomain] file1.schemas [file2.schemas [...]]"
  exit 1
fi

OUTFILE="$1"
DOMAIN=""
shift

if [ "$1" = "--domain" ]; then
  shift
  DOMAIN=$1
  shift
fi

echo '<?xml version="1.0"?>' > "$OUTFILE"
echo '<gconfschemafile><schemalist>' >> "$OUTFILE"

while [ "$1" ]; do
  if [ -f "$1" ]; then
    sed -e '/<?xml/d' \
        -e 's|<gconfschemafile>||g' \
	-e 's|</gconfschemafile>||g' \
        -e 's|<schemalist>||g' \
	-e 's|</schemalist>||g' "$1" >> "$OUTFILE"
  fi
  shift
done
  
echo '</schemalist></gconfschemafile>' >> "$OUTFILE"
if [ "$DOMAIN" != "" ]; then
  sed -ri "s/^([[:space:]]*)(<locale name=\"C\">)/\1<gettext_domain>$DOMAIN<\/gettext_domain>\n\1\2/; /^[[:space:]]*<locale name=\"[^C]/,/^[[:space:]]*<\/locale>[[:space:]]*\$/ d; /^$/d; s/<\/schema>$/&\n/" "$OUTFILE"
fi
