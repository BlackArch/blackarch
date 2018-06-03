#!/bin/bash

# This is a simple mirroring script. To save bandwidth it first checks a
# timestamp via HTTP and only runs rsync when the timestamp differs from the
# local copy. As of 2016, a single rsync run without changes transfers roughly
# 6MiB of data which adds up to roughly 250GiB of traffic per month when rsync
# is run every minute. Performing a simple check via HTTP first can thus save a
# lot of traffic.

# Directory where the repo is stored locally. Example: /srv/repo (absolute PATH)
target="/srv/blackarch/target"

# Directory where files are downloaded to before being moved in place.
# This should be on the same filesystem as $target, but not a subdirectory of $target.
# Example: /srv/tmp
tmp="/srv/blackarch/tmp"

# Lockfile path
lock="/var/lock/syncrepo.lck"

# Log file (small) - TODO
log_file="/srv/blackarch/log"

# If you want to limit the bandwidth used by rsync set this.
# Use 0 to disable the limit.
# The default unit is KiB (see man rsync /--bwlimit for more)
bwlimit=0

# The source URL of the mirror you want to sync from.
source_url='rsync://blackarch.org/blackarch/'

# An HTTP(S) URL pointing to the 'lastupdate' file on your chosen mirror.
# Otherwise use the HTTP(S) URL from your chosen mirror.
lastupdate_url='https://blackarch.org/blackarch/lastupdate'

#### END CONFIG

trap interrupted INT
trap interrupted EXIT

function logging() {
    date="$(date '+%a %d %T')"
    host="$(hostname)"
    name="${0}"
    msg="${1}"

    printf "%s %s %s: %s\n" "${date}" "${host}" "${name}" "${msg}" \
        >> ${log_file}

    return 0
}

function interrupted() {
    if ! rm -rf "${lock}"; then
        logging "ERROR: removing lock file: ${lock}"
        exit 1
    fi

    logging "INFO: lock file (${lock}) removed."
}

if [[ ! -d "${target}" ]]; then
    mkdir -p "${target}"
    logging "INFO: ${target} folder created."
fi

if [[ ! -d "${tmp}" ]]; then
    mkdir -p "${tmp}"
    logging "INFO: ${tmp} created."
fi

exec 9>"${lock}"
flock -n 9 || exit

rsync_cmd() {
    local -a cmd=(rsync -rtlH --safe-links --delete-after --timeout=600 -p --delay-updates \
                  --no-motd --temp-dir="${tmp}")

    if stty &>/dev/null; then
        cmd+=(-h -v --progress)
        logging "setting tty option: ${cmd[*]}"
    else
        cmd+=(--quiet)
        logging "setting non-tty option: ${cmd[*]}"
    fi

    if ((bwlimit>0)); then
        cmd+=("--bwlimit=$bwlimit")
        logging "setting bandwidth limit option: ${bwlimit}"
    fi

    logging "syncing..."
    "${cmd[@]}" "$@"
}

# if we are called WITHOUT a tty (cronjob) only run when there are changes
# this script will always run, the sync will happen, if called on tty
if ! tty -s && [[ -f "$target/lastupdate" ]] && diff -b <(curl -Ls "$lastupdate_url") "$target/lastupdate" >/dev/null; then
    logging "no changes, leaving..."
    exit 0
fi

rsync_cmd \
    "${source_url}" \
    "${target}"

