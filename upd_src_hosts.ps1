$list = @("http://someonewhocares.org/hosts/zero/hosts",
        "http://winhelp2002.mvps.org/hosts.txt",
        "https://raw.githubusercontent.com/hoshsadiq/adblock-nocoin-list/master/hosts.txt",
        "https://raw.githubusercontent.com/FadeMind/hosts.extras/master/SpotifyAds/hosts")

for($i = 0; $i -lt $list.length; ++$i){
        Invoke-WebRequest -OutFile .\hosts\raw -Uri $list[$i]
        Get-Content .\hosts\raw | Out-File -Encoding ASCII .\hosts\$i.txt
        Remove-Item .\hosts\raw
}

.\merge_host.exe raw .\hosts\0.txt .\hosts\1.txt .\hosts\2.txt