$list = @("http://someonewhocares.org/hosts/zero/hosts",
        "http://winhelp2002.mvps.org/hosts.txt",
        "http://www.malwaredomainlist.com/hostslist/hosts.txt",
        "https://raw.githubusercontent.com/hoshsadiq/adblock-nocoin-list/master/hosts.txt",
        "https://pgl.yoyo.org/adservers/serverlist.php?hostformat=hosts&mimetype=plaintext&useip=0.0.0.0",
        "https://raw.githubusercontent.com/AdAway/adaway.github.io/master/hosts.txt",
        "https://raw.githubusercontent.com/mitchellkrogza/Badd-Boyz-Hosts/master/hosts",
        "https://raw.githubusercontent.com/StevenBlack/hosts/master/data/StevenBlack/hosts",
        "https://raw.githubusercontent.com/FadeMind/hosts.extras/master/UncheckyAds/hosts",
        "https://raw.githubusercontent.com/Yhonay/antipopads/master/hosts",
        "https://raw.githubusercontent.com/yous/YousList/master/hosts.txt",
        "https://raw.githubusercontent.com/marktron/fakenews/master/fakenews",
        "https://raw.githubusercontent.com/Sinfonietta/hostfiles/master/gambling-hosts")
        # "https://pastebin.com/raw/kDqbYwWr",
        # "https://raw.githubusercontent.com/Dawsey21/Lists/master/main-blacklist.txt")

[Net.ServicePointManager]::SecurityProtocol =  [System.Security.Authentication.SslProtocols] "tls, tls11, tls12"

for($i = 0; $i -lt $list.length; ++$i){
        Invoke-WebRequest -OutFile .\hosts\raw -Uri $list[$i]
        Get-Content .\hosts\raw | Out-File -Encoding ASCII .\hosts\$i.txt
        Remove-Item .\hosts\raw
}

if(Test-Path .\raw){
        Remove-Item .\raw
}

.\merge_host.exe raw .\hosts\0.txt .\hosts\1.txt .\hosts\2.txt .\hosts\3.txt .\hosts\4.txt
.\merge_host.exe raw .\hosts\5.txt .\hosts\6.txt .\hosts\7.txt .\hosts\8.txt .\hosts\9.txt
.\merge_host.exe raw .\hosts\10.txt .\hosts\11.txt .\hosts\12.txt # .\hosts\13.txt .\hosts\14.txt

Remove-Item .\hosts\*.txt