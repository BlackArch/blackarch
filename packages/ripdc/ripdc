#!/bin/sh
################################################################################
#                ____                     _ __                                 #
#     ___  __ __/ / /__ ___ ______ ______(_) /___ __                           #
#    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           #
#   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            #
#                                            /___/ team                        #
#                                                                              #
# ripdc.sh - reverse ip domain checker                                         #
#                                                                              #
# FILE                                                                         #
# ripdc.sh                                                                     #
#                                                                              #
# DESCRIPTION                                                                  #
# A script which maps domains related to an given ip address or domainname.    #
#                                                                              #
# AUTHOR                                                                       #
# noptrix                                                                      #
#                                                                              #
################################################################################


# ripdc version
VERSION="ripdc.sh v0.3"

# true / false
FALSE="0"
TRUE="1"

# return codes
SUCCESS="1337"
FAILURE="31337"

# verbose mode - default: quiet
VERBOSE="/dev/null"


# reverse map ip/domain address for domains
reverse_map()
{
    # server, request and response settings
    url="https://domains.yougetsignal.com/"
    file="domains.php"
    data="remoteAddress=${target}&key="
    referer="https://www.yougetsignal.com/tools/web-sites-on-web-server/"

    echo "[+] reverse mapping ${target}"

    domains="`curl -e "${referer}" -d "${data}" "${url}${file}" 2> ${VERBOSE} |
    tr -s ':' '\n' | grep '\[\["' | tr -d '{}[]",' | tr -s ' ' '\n'`"

    if [ ! -z "${domains}" ]
    then
        echo "[+] listing found domains"
        for domain in ${domains}
        do
            echo "    > ${domain}"
        done
    else
        warn "no domains were found"
    fi

    return ${SUCCESS}
}


# print warning
warn()
{
    echo "[!] WARNING: ${*}"

    return ${SUCCESS}
}


# print error and exit
err()
{
    echo "[-] ERROR: ${*}"
    exit ${FAILURE}

    return ${SUCCESS}
}


# usage and help
usage()
{
    echo "usage:"
    echo ""
    echo "  ripdc.sh -t <arg> [options] | <misc>"
    echo ""
    echo "options:"
    echo ""
    echo "  -t <target> - domain name or ip address"
    echo "  -v          - verbose mode (default: off)"
    echo ""
    echo "misc:"
    echo ""
    echo "  -V          - print version of ripdc and exit"
    echo "  -H          - print this help and exit"

    exit ${SUCCESS}

    return ${SUCCESS}
}


# leet banner, very important
banner()
{
    echo "--==[ ripdc.sh by noptrix@nullsecurity.net ]==--"

    return ${SUCCESS}
}


# check argument count
check_argc()
{
    if [ ${#} -lt 1 ]
    then
        err "-H for help and usage"
    fi

    return ${SUCCESS}
}


# check if required arguments were selected
check_args()
{
    echo "[+] checking arguments" > ${VERBOSE} 2>&1

    if [ -z "${target}" ]
    then
        err "WTF?! mount /dev/brain"
    fi

    return ${SUCCESS}
}


# parse command line options
get_opts()
{
    while getopts t:vVH flags
    do
        case ${flags} in
            t)
                target="${OPTARG}"
                ;;
            v)
                VERBOSE="/dev/stdout"
                ;;
            V)
                echo "${VERSION}"
                exit ${SUCCESS}
                ;;
            H)
                usage
                ;;
            *)
                err "WTF?! mount /dev/brain"
                ;;
        esac
    done

    return ${SUCCESS}
}


# controller and program flow
main()
{
    banner
    check_argc ${*}
    get_opts ${*}
    check_args ${*}
    reverse_map

    echo "[+] game over"

    return ${SUCCESS}
}


# program start
main ${*}

# EOF
