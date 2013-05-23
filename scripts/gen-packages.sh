#!/bin/bash

root="$(dirname "$0")/.."

usage() {
	cat <<EOF
$(basename "$0") n[ormal]|a[rchtrack]|p[seudo] [package]...
EOF
}

if (( $# == 0 )) ; then
	usage
	exit 1
fi

if (( $# > 1 )) ; then
	packages=("${@:2}")
else
	case "$1" in
		p|pseudo)
			packages=("$root"/pseudo-packages/archtrack*)
			;;
		n|normal)
			packages=("$root"/packages/*)
			;;
		*)
			usage
			exit 1
			;;
	esac
fi

for package in "${packages[@]}" ; do
	if [[ ! -d "$package" ]] ; then
		echo >&2 "'$package' not found."
		continue
	fi

	if [[ "$1" = p ]] ; then
			if [[ $(basename "$package") = archtrack ]] ; then
				# The 'archtrack' pseudo-package depends on all of the other
				#  archtrack pseudo-packages.
				depends=$(basename -a "$root"/pseudo-packages/archtrack-*)
			else
				depends=$(grep -l "^groups.*\<$(basename "$package")\>" "$root"/packages/*/info |
				sed -e 's#/info$##' -e 's#^.*/##g')
				fi

				sed \
				-e "/%DEPENDS%/r "<(echo "$depends") \
				-e "/%DEPENDS%/d" \
				-e "s|%PKGNAME%|$(basename "$package")|" \
				-e "s|%PKGDESC%|$(< "$package/description")|" \
				"$root/pseudo-packages/PKGBUILD.in" > "$package/PKGBUILD"
		else
			info="$package/info"
			# File existence checks
			if [[ ! -r "$info" ]] ; then
				echo >&2 "'$package' info file unreadable."
				continue
			fi

			if [[ ! -r "$package/PKGBUILD.in" ]] ; then
				echo >&2 "'$package' PKGBUILD.in file unreadable."
				continue
			fi

			if grep -q "^working=false" "$info" ; then
				echo >&2 "Warning: '$package' is not working."
			fi

			case "$1" in
				n|normal)
					outdir=normal
					pkgname=$(grep '^upstream_name=' "$info" | cut -d'=' -f2)
					groups="()"
					name_suffix=
					;;
				a|archtrack)
					outdir=archtrack
					pkgname=$(basename "$package")
					groups=$(grep '^groups=' "$info" | cut -d'=' -f2)

					# We add the '-archtrack' suffix to package names if they exist in the
					#  official repositories.
					if ! grep -q 'upstream_repo=aur' $info ; then
						name_suffix='-archtrack'
					else
						name_suffix=
					fi
					;;
				*)
					usage
					exit 1
					;;
			esac
			pkgdesc=$(grep '^description=' "$info" | cut -d'=' -f2)

			#rm -rf "$package/$outdir"
			[[ -d "$package/$outdir" ]] || mkdir "$package/$outdir"

			# Field substitutions
			sed -e "s|%PKGNAME%|$pkgname$name_suffix|" \
			-e "s|%PKGDESC%|$pkgdesc|" \
			-e "s|%GROUPS%|$groups|" \
			"$package/PKGBUILD.in" > "$package/$outdir/PKGBUILD"

			# Link source files
			# Sorry for parsing ls output like this. extglob wasn't working for some
			#  reason. I made sure no source files contained strange characters in their
			#  names.
			ls "$package" | grep -Ev '^(normal|archtrack|PKGBUILD.in|info)$' |
			while read src ; do
				ln -fs "../$src" "$package/$outdir/$(basename "$src")"
				#cp "$package/$src" "$package/$outdir"
			done
		fi
	done
