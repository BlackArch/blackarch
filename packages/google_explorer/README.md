**[+] Google Mass Explorer (PYTHON 3.6)**

This is a automated robot for google search engine.

Make a google search, and parse the results for a especific exploit you define.
The options can be listed with --help parameter.

Usage:

    google_explorer.py --dork=<arg> --browser=<arg> [--exploit_parser=<arg>]
                                                    [--language=<arg>]
                                                    [--location=<arg>]
                                                    [--last_update=<arg>]
                                                    [--google_domain=<arg>]
    google_explorer.py --xpl_filter=<arg>
    google_explorer.py --help
    google_explorer.py --version

Options:

    -h --help                                Open help menu
    -v --version                             Show version

Required options:

    --dork='google dork'                     your favorite g00gle dork :)
    --browser='browser'                      chrome
                                             chromium
    --xpl_filter='exploits filters list'     joomla_cve_2015_8562
                                             wordpress_cve_2015_1579
                                             joomla_cve_2016_8870

Optional options:

    --language='page language'               Portuguese
                                             English
                                             Arabic
                                             Romanian
                                             ...
                                             ...

    --location='server location'             Brazil
                                             Mauritania
                                             Tunisia
                                             Marroco
                                             Japan
                                             ...
                                             ...

    --last_update='page last update'         anytime
                                             past 24 hours
                                             past week
                                             past month
                                             past year

    --google_domain='google domain'          google domain to use on search.
                                             Ex: google.co.uk



**Instalation:**

Install these packages:

    libxml2-dev libxslt1-dev python-dev


This project is developed in PYTHON 3.6 Make sure u use pip3 (package: python3-pip) to install dep:

    $ sudo pip3 install -r requirements


Make sure u have installed chromedriver, if you don't have this tutorial can help:

    https://developers.supportbee.com/blog/setting-up-cucumber-to-run-with-Chrome-on-Linux/


In some distro, i had some issues with users running chrome as root.. **TO AVOID ISSUES, RUN THE TOOL AS REGULAR USER!!**



**How to use:**

Make a search with google bot, here are some examples:

    python3 google_explorer.py --browser='chrome' --dork='site:gob.ve inurl:index.php' --location="Venezuela"
    python3 google_explorer.py --dork="index.php?option=" --browser="chrome" --google_domain="google.co.il" --location="איחוד האמירויות הערביות"
    python3 google_explorer.py --browser='chrome' --dork='inurl:index.php?option' --location="Rússia" --last_update='na última semana'
    

Run the exploit filter for the specific vulnerability u are looking for, to check if results can or not be vulnerable:

    $ python3 joomla_cve_2015_8562.py --xpl_filter='joomla_cve_2015_8562'
    

The possible vulnerable targets can be explored using some utils scripts i made, or you can run the exploit as u wish:

Run the exploiter.py (if the original exploit is runned by command line - **READ THE EXPLOIT BEFORE USING IT**):

    $ cd exploits

    $ python2 exploiter.py --file <vuln file>
    

**Exploits**

I'm using exploits from other authors, so don't take their credit on that! I put the same public exploit published, and i DON'T TAKE THE AUTHOR NAME FROM IT!! So any help with the exploit, you can look for the author =))



    
    







