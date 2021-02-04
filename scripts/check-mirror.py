#!/bin/python3

import requests

BASE_URL="https://www.blackarch.org/blackarch/lastupdate"
MIRROR_LIST="https://raw.githubusercontent.com/BlackArch/blackarch/master/mirror/mirror.lst"

if __name__ == "__main__":
    #check_mirror(requests.get(MIRROR_LIST), requests.get(BASE_URL).text)
    mirrorlist = requests.get(MIRROR_LIST)
    lastupdate = requests.get(BASE_URL).text

    for line in mirrorlist.iter_lines():
        try:
            country, url, name = str(line).split('|')
            name = name.strip('\'')
            url = url.replace("$repo/os/$arch", "lastupdate")
        except:
            print("[SOMETHING IS WRONG] Error: {}.".format(line))

        try:
            r = requests.get(url, stream=True)
            repo_statuscode = r.status_code
            repo_lastupdate = r.text

            if repo_lastupdate < lastupdate:
                print("[OUT OF DATE] - {} -> {}".format(name, url))
            elif repo_lastupdate > lastupdate:
                print("[SOMETHING IS WRONG] - {} -> {}".format(name, url))
            else:
                print("[IN SYNC] - {} -> {}".format(name, url))
        except:
            print("[HOST DOWN?] - {} -> {}".format(name, url))



